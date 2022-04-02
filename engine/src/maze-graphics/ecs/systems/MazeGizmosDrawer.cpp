//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/systems/MazeGizmosDrawer.hpp"
#include "maze-graphics/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/managers/MazeGizmosManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GizmosDrawer
    //
    //////////////////////////////////////////
    GizmosDrawer::GizmosDrawer()
        : m_color(ColorF128::c_white)
        , m_world(nullptr)
    {

    }

    //////////////////////////////////////////
    GizmosDrawer::~GizmosDrawer()
    {
        if (m_lines.entity)
            m_lines.entity->removeFromECSWorld();

        if (m_triangles.entity)
            m_triangles.entity->removeFromECSWorld();
    }

    //////////////////////////////////////////
    GizmosDrawerPtr GizmosDrawer::Create(ECSWorld* _world, RenderTarget* _renderTarget)
    {
        GizmosDrawerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(GizmosDrawer, object, init(_world, _renderTarget));
        return object;
    }

    //////////////////////////////////////////
    bool GizmosDrawer::init(ECSWorld* _world, RenderTarget* _renderTarget)
    {
        m_world = _world;

        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        {
            m_lines.vao = VertexArrayObject::Create(renderSystem);
            m_lines.vao->setRenderDrawTopology(RenderDrawTopology::Lines);
            m_lines.renderMesh = _renderTarget->createRenderMeshFromPool(1);
            m_lines.renderMesh->setName("Lines");
            m_lines.renderMesh->setVertexArrayObject(m_lines.vao);

            MaterialPtr material = Material::Create(renderSystem->getMaterialManager()->getColorMaterial());
            material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::Always);
            material->getFirstRenderPass()->setRenderQueueIndex(5000);

            m_lines.entity = Entity::Create();
            m_world->addEntity(m_lines.entity);
            m_lines.entity->createComponent<Transform3D>();
            MeshRendererPtr meshRenderer = m_lines.entity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(m_lines.renderMesh);
            meshRenderer->setMaterial(material);
            meshRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);
        }

        {
            m_triangles.vao = VertexArrayObject::Create(renderSystem);
            m_triangles.vao->setRenderDrawTopology(RenderDrawTopology::Triangles);
            m_triangles.renderMesh = _renderTarget->createRenderMeshFromPool(1);
            m_triangles.renderMesh->setName("Triangles");
            m_triangles.renderMesh->setVertexArrayObject(m_triangles.vao);

            MaterialPtr material = Material::Create(renderSystem->getMaterialManager()->getColorMaterial());
            material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::Always);
            material->getFirstRenderPass()->setRenderQueueIndex(5000);

            m_triangles.entity = Entity::Create();
            m_world->addEntity(m_triangles.entity);
            m_triangles.entity->createComponent<Transform3D>();
            MeshRendererPtr meshRenderer = m_triangles.entity->createComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(m_triangles.renderMesh);
            meshRenderer->setMaterial(material);
            meshRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);
        }

        m_gizmoBillboardPool.reset(
            new SharedObjectPool<GizmoBillboard3D>(
                [this]() -> GizmoBillboard3DPtr
                {
                    EntityPtr entity = Entity::Create();
                    m_world->addEntity(entity);

                    entity->ensureComponent<Name>("Gizmo Billboard");

                    MaterialPtr material = Material::Create(
                        GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getBuiltinMaterial(BuiltinMaterialType::ColorTextureCustomUV));
                    material->getFirstRenderPass()->setRenderQueueIndex(5000);

                    MeshRendererPtr meshRenderer = entity->createComponent<MeshRenderer>();
                    meshRenderer->setRenderMesh(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getDefaultQuadMesh());
                    meshRenderer->setMaterial(material);
                    meshRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);

                    GizmoBillboard3DPtr gizmoBillboard3D = entity->createComponent<GizmoBillboard3D>();
                    gizmoBillboard3D->getTransform()->setLocalScale(0.5f);

                    return gizmoBillboard3D;
                },
                [](GizmoBillboard3DPtr const& _gizmoBillboard)
                {
                    _gizmoBillboard->getEntityRaw()->setActiveSelf(true);
                },
                [](GizmoBillboard3DPtr const& _gizmoBillboard)
                {
                    _gizmoBillboard->getEntityRaw()->setActiveSelf(false);
                },
                [](GizmoBillboard3DPtr const& _gizmoBillboard)
                {
                    if (_gizmoBillboard->getEntityRaw())
                        _gizmoBillboard->getEntityRaw()->removeFromECSWorld();
                }));

        return true;
    }

    //////////////////////////////////////////
    void GizmosDrawer::clear()
    {
        m_lines.vao->clear();
        m_lines.indices.clear();
        m_lines.vertices.clear();
        m_lines.colors.clear();

        m_triangles.vao->clear();
        m_triangles.indices.clear();
        m_triangles.vertices.clear();
        m_triangles.colors.clear();

        m_billboardsData.clear();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawLine(
        Vec3DF const& _point0,
        Vec3DF const& _point1)
    {
        U32 index0 = (U32)m_lines.vertices.size();
        U32 index1 = index0 + 1;

        m_lines.indices.emplace_back(index0);
        m_lines.indices.emplace_back(index1);

        m_lines.vertices.emplace_back(_point0);
        m_lines.vertices.emplace_back(_point1);

        m_lines.colors.emplace_back(m_color.value);
        m_lines.colors.emplace_back(m_color.value);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawCircle(
        Vec3DF const& _position,
        Vec3DF const& _direction,
        F32 _radius,
        S32 _segmentsCount)
    {
        F32 angleDelta = Math::c_twoPi / (F32)_segmentsCount;
        Size pointsCount = (Size)Math::Ceil((Math::c_twoPi / angleDelta) - angleDelta * 0.5f);
        Vector<Vec3DF> circlePoints;
        for (F32 i = 0; i < Math::c_twoPi - angleDelta * 0.5f; i += angleDelta)
        {
            F32 c = Math::Cos(i);
            F32 s = Math::Sin(i);

            Vec3DF d = _direction.crossProduct(Vec3DF(c, s, 0.0f) * _radius);
            circlePoints.emplace_back(_position + d);
        }

        for (Size i = 0; i < pointsCount - 1; ++i)
        {
            Vec3DF const& circlePoint0 = circlePoints[i];
            Vec3DF const& circlePoint1 = circlePoints[i + 1];

            drawLine(circlePoint0, circlePoint1);
        }

        drawLine(circlePoints.back(), circlePoints.front());
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawBillboard(
        Vec3DF const& _point,
        SpritePtr const& _sprite,
        ColorF128 const& _color)
    {
        m_billboardsData.emplace_back(
            BillboardData
            {
                _point,
                _sprite,
                _color
            });
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawAABB(AABB2D const& _aabb)
    {
        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().x, _aabb.getMin().y);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().x, _aabb.getMax().y);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().x, _aabb.getMax().y);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().x, _aabb.getMin().y);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawAABB(AABB3D const& _aabb)
    {
        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMin().y, _aabb.getMin().z);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().z);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().z);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMin().y, _aabb.getMin().z);

        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().z, _aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().z);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().z, _aabb.getMax().x, _aabb.getMax().y, _aabb.getMax().z);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMax().z, _aabb.getMin().x, _aabb.getMax().y, _aabb.getMax().z);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMax().z, _aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().z);

        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().z);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().z);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMax().y, _aabb.getMax().z);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMax().y, _aabb.getMax().z);
    }

    //////////////////////////////////////////
    void GizmosDrawer::rebuildMeshes()
    {
        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        static VertexAttributeDescription const c_positionDescription = VertexAttributeDescription
            {
                VertexAttributeSemantic::Position,
                3,
                VertexAttributeType::F32,
                false,
                3 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
                0
            };

        static VertexAttributeDescription const c_colorDescription = VertexAttributeDescription
            {
                VertexAttributeSemantic::Color,
                4,
                VertexAttributeType::F32,
                false,
                4 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
                0
            };

        if (!m_lines.vertices.empty())
        {
            m_lines.vao->setVerticesData((U8 const*)&m_lines.vertices[0], c_positionDescription, m_lines.vertices.size());
            m_lines.vao->setVerticesData((U8 const*)&m_lines.colors[0], c_colorDescription, m_lines.colors.size());
            m_lines.vao->setIndices((U8 const*)&m_lines.indices[0], VertexAttributeType::U32, m_lines.indices.size());
        }

        if (!m_triangles.vertices.empty())
        {
            m_triangles.vao->setVerticesData((U8 const*)&m_triangles.vertices[0], c_positionDescription, m_triangles.vertices.size());
            m_triangles.vao->setVerticesData((U8 const*)&m_triangles.colors[0], c_colorDescription, m_triangles.colors.size());
            m_triangles.vao->setIndices((U8 const*)&m_triangles.indices[0], VertexAttributeType::U32, m_triangles.indices.size());
        }



        Camera3DPtr const& camera3D = GizmosManager::GetInstancePtr()->getCamera();
        if (camera3D)
        {
            while (m_billboards.size() < m_billboardsData.size())
                m_billboards.emplace_back(m_gizmoBillboardPool->fetch());

            while (m_billboards.size() > m_billboardsData.size())
            {
                m_gizmoBillboardPool->release(m_billboards.back());
                m_billboards.pop_back();
            }

            for (Size i = 0, in = m_billboards.size(); i < in; ++i)
            {
                 GizmoBillboard3DPtr const& gizmoBillboard = m_billboards[i];
                BillboardData const& gizmoBillboardData = m_billboardsData[i];

                MaterialPtr const& material = gizmoBillboard->getMeshRenderer()->getMaterial();

                if (gizmoBillboardData.sprite)
                {
                    material->setUniform("u_baseMap", gizmoBillboardData.sprite->getTexture());
                    material->setUniform("u_uv0", gizmoBillboardData.sprite->getTextureCoordLB());
                    material->setUniform("u_uv1", gizmoBillboardData.sprite->getTextureCoordRT());
                }
                else
                {
                    material->setUniform("u_baseMap", renderSystem->getTextureManager()->getErrorTexture());
                    material->setUniform("u_uv0", Vec2DF::c_zero);
                    material->setUniform("u_uv1", Vec2DF::c_one);
                }

                material->setUniform("u_color", gizmoBillboardData.color.value);
                
                Mat4DF mat = Mat4DF::CreateLookAtMatrix(
                    gizmoBillboardData.point,
                    gizmoBillboardData.point - camera3D->getTransform()->getWorldForwardDirection(),
                    camera3D->getTransform()->getWorldUpDirection());

                gizmoBillboard->getTransform()->setLocalTransform(mat);
            }
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
