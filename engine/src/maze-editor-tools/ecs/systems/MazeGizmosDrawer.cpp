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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/systems/MazeGizmosDrawer.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/math/MazeRay.hpp"
#include "maze-core/math/MazePlane.hpp"
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
        for (S32 renderMode = 0; renderMode < (S32)MeshRenderMode::MAX; ++renderMode)
        {
            if (m_lines[renderMode].entity)
                m_lines[renderMode].entity->removeFromECSWorld();

            if (m_triangles[renderMode].entity)
                m_triangles[renderMode].entity->removeFromECSWorld();
        }
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

        for (S32 renderMode = 0; renderMode < (S32)MeshRenderMode::MAX; ++renderMode)
        {
            MaterialPtr& material = m_materials[renderMode];
            material = Material::Create(renderSystem->getMaterialManager()->getColorMaterial());
            switch (MeshRenderMode(renderMode))
            {
                case MeshRenderMode::Opaque:
                {
                    material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::LessEqual);
                    material->getFirstRenderPass()->setRenderQueueIndex(2500);
                    material->getFirstRenderPass()->setDepthWriteEnabled(true);
                    break;
                }
                case MeshRenderMode::Transparent:
                {
                    material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::Always);
                    material->getFirstRenderPass()->setRenderQueueIndex(4500);
                    break;
                }
                case MeshRenderMode::TransparentTop:
                {
                    material->getFirstRenderPass()->setDepthTestCompareFunction(CompareFunction::Always);
                    material->getFirstRenderPass()->setRenderQueueIndex(5000);
                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                    break;
                }
            }

            {
                MeshData& meshData = m_lines[renderMode];

                meshData.vao = VertexArrayObject::Create(renderSystem);
                MAZE_ERROR_RETURN_VALUE_IF(!meshData.vao, false, "Failed to created Lines VAO!");
                meshData.vao->setRenderDrawTopology(RenderDrawTopology::Lines);
                meshData.renderMesh = _renderTarget->createRenderMeshFromPool(1);
                meshData.renderMesh->setName("Lines");
                meshData.renderMesh->setVertexArrayObject(meshData.vao);

                meshData.entity = Entity::Create();
                m_world->addEntity(meshData.entity);
                meshData.entity->createComponent<Transform3D>();
                MeshRendererPtr meshRenderer = meshData.entity->createComponent<MeshRenderer>();
                meshRenderer->setRenderMesh(meshData.renderMesh);
                meshRenderer->setMaterial(material);
                meshRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);
            }

            {
                MeshData& meshData = m_triangles[renderMode];

                meshData.vao = VertexArrayObject::Create(renderSystem);
                MAZE_ERROR_RETURN_VALUE_IF(!meshData.vao, false, "Failed to created Triangles VAO!");
                meshData.vao->setRenderDrawTopology(RenderDrawTopology::Triangles);
                meshData.renderMesh = _renderTarget->createRenderMeshFromPool(1);
                meshData.renderMesh->setName("Triangles");
                meshData.renderMesh->setVertexArrayObject(meshData.vao);

                meshData.entity = Entity::Create();
                m_world->addEntity(meshData.entity);
                meshData.entity->createComponent<Transform3D>();
                MeshRendererPtr meshRenderer = meshData.entity->createComponent<MeshRenderer>();
                meshRenderer->setRenderMesh(meshData.renderMesh);
                meshRenderer->setMaterial(material);
                meshRenderer->getRenderMask()->setMask(DefaultRenderMask::Gizmos);
            }
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
                    material->getFirstRenderPass()->setRenderQueueIndex(4800);

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
        for (S32 renderMode = 0; renderMode < (S32)MeshRenderMode::MAX; ++renderMode)
        {
            m_lines[renderMode].vao->clear();
            m_lines[renderMode].indices.clear();
            m_lines[renderMode].vertices.clear();
            m_lines[renderMode].colors.clear();

            m_triangles[renderMode].vao->clear();
            m_triangles[renderMode].indices.clear();
            m_triangles[renderMode].vertices.clear();
            m_triangles[renderMode].colors.clear();
        }

        m_billboardsData.clear();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawLine(
        Vec3F const& _point0,
        Vec3F const& _point1,
        MeshRenderMode _renderMode)
    {
        MeshData& lines = m_lines[(S32)_renderMode];

        U32 index0 = (U32)lines.vertices.size();
        U32 index1 = index0 + 1;

        lines.indices.emplace_back(index0);
        lines.indices.emplace_back(index1);

        lines.vertices.emplace_back(transformPoint(_point0));
        lines.vertices.emplace_back(transformPoint(_point1));

        lines.colors.emplace_back(m_color.value);
        lines.colors.emplace_back(m_color.value);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawLine(
        Vec3F const& _point0,
        Vec3F const& _point1,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        if (_duration < 0.0f)
        {
            drawLine(_point0, _point1, _renderMode);
            return;
        }

        m_bufferedLines.emplace_back(
            BufferedLine{ _duration, transformPoint(_point0), transformPoint(_point1), getColor(), _renderMode });
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireQuad(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec2F const& _size,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F right = _up.crossProduct(_forward).normalizedCopy();

        Vec2F halfSize = _size * 0.5f;

        setColor(_color);
        drawLine(
            _position - right * halfSize.x - _up * halfSize.y,
            _position - right * halfSize.x + _up * halfSize.y,
            _duration,
            _renderMode);
        drawLine(
            _position + right * halfSize.x - _up * halfSize.y,
            _position + right * halfSize.x + _up * halfSize.y,
            _duration,
            _renderMode);

        drawLine(
            _position - right * halfSize.x - _up * halfSize.y,
            _position + right * halfSize.x - _up * halfSize.y,
            _duration,
            _renderMode);
        drawLine(
            _position - right * halfSize.x + _up * halfSize.y,
            _position + right * halfSize.x + _up * halfSize.y,
            _duration,
            _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireCircle(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F forward = _direction.normalizedCopy() * _radius;
        Vec3F up = forward.perpendicular();
        Vec3F right = up.crossProduct(forward).normalizedCopy() * _radius;

        Mat4F matrix = Mat4F::CreateChangeOfBasisMatrix(right, up, forward);
        Vec3F lastPoint = _position + matrix.transformAffine(Vec3F(Math::Cos(0.0f), Math::Sin(0.0f), 0.0f));
        Vec3F nextPoint = Vec3F::c_zero;

        for (S32 i = 0; i < 91; i++)
        {
            nextPoint.x = Math::Cos(Math::DegreesToRadians(i * 4.0f));
            nextPoint.y = Math::Sin(Math::DegreesToRadians(i * 4.0f));
            nextPoint.z = 0;

            nextPoint = _position + matrix.transformAffine(nextPoint);

            drawLine(lastPoint, nextPoint, _duration, _renderMode);
            lastPoint = nextPoint;
        }
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireHemicircle(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        F32 _radius,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F forward = _forward.normalizedCopy() * _radius;
        Vec3F up = _up.normalizedCopy() * _radius;
        Vec3F right = up.crossProduct(forward).normalizedCopy() * _radius;

        Mat4F matrix = Mat4F::CreateChangeOfBasisMatrix(right, up, forward);
        Vec3F lastPoint = _position + matrix.transformAffine(Vec3F(Math::Cos(0.0f), Math::Sin(0.0f), 0.0f));
        Vec3F nextPoint = Vec3F::c_zero;

        for (S32 i = 0; i < 46; i++)
        {
            nextPoint.x = Math::Cos(Math::DegreesToRadians(i * 4.0f));
            nextPoint.y = Math::Sin(Math::DegreesToRadians(i * 4.0f));
            nextPoint.z = 0;

            nextPoint = _position + matrix.transformAffine(nextPoint);

            drawLine(lastPoint, nextPoint, _duration, _renderMode);
            lastPoint = nextPoint;
        }
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireCube(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec3F const& _scale,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F forward = _forward.normalizedCopy();
        Vec3F up = _up.normalizedCopy();
        Vec3F right = up.crossProduct(forward).normalizedCopy();

        Vec3F halfScale = _scale * 0.5f;

        setColor(_color);
        drawLine(
            _position - right * halfScale - up * halfScale - forward * halfScale,
            _position - right * halfScale - up * halfScale + forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position + right * halfScale - up * halfScale - forward * halfScale,
            _position + right * halfScale - up * halfScale + forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position - right * halfScale - up * halfScale - forward * halfScale,
            _position + right * halfScale - up * halfScale - forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position - right * halfScale - up * halfScale + forward * halfScale,
            _position + right * halfScale - up * halfScale + forward * halfScale,
            _duration, _renderMode);

        drawLine(
            _position - right * halfScale + up * halfScale - forward * halfScale,
            _position - right * halfScale + up * halfScale + forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position + right * halfScale + up * halfScale - forward * halfScale,
            _position + right * halfScale + up * halfScale + forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position - right * halfScale + up * halfScale - forward * halfScale,
            _position + right * halfScale + up * halfScale - forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position - right * halfScale + up * halfScale + forward * halfScale,
            _position + right * halfScale + up * halfScale + forward * halfScale,
            _duration, _renderMode);

        drawLine(
            _position - right * halfScale - up * halfScale - forward * halfScale,
            _position - right * halfScale + up * halfScale - forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position + right * halfScale - up * halfScale - forward * halfScale,
            _position + right * halfScale + up * halfScale - forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position + right * halfScale - up * halfScale + forward * halfScale,
            _position + right * halfScale + up * halfScale + forward * halfScale,
            _duration, _renderMode);
        drawLine(
            _position - right * halfScale - up * halfScale + forward * halfScale,
            _position - right * halfScale + up * halfScale + forward * halfScale,
            _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireSphere(
        Vec3F const& _position,
        F32 _radius,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        F32 angle = 10.0f;

        Vec3F x = Vec3F(_position.x, _position.y + _radius * Math::Sin(0.0f), _position.z + _radius * Math::Cos(0.0f));
        Vec3F y = Vec3F(_position.x + _radius * Math::Cos(0.0f), _position.y, _position.z + _radius * Math::Sin(0.0f));
        Vec3F z = Vec3F(_position.x + _radius * Math::Cos(0.0f), _position.y + _radius * Math::Sin(0.0f), _position.z);

        setColor(_color);

        for (S32 i = 1; i < 37; i++)
        {
            Vec3F newX = Vec3F(_position.x, _position.y + _radius * Math::Sin(Math::DegreesToRadians(angle * i)), _position.z + _radius * Math::Cos(Math::DegreesToRadians(angle * i)));
            Vec3F newY = Vec3F(_position.x + _radius * Math::Cos(Math::DegreesToRadians(angle * i)), _position.y, _position.z + _radius * Math::Sin(Math::DegreesToRadians(angle * i)));
            Vec3F newZ = Vec3F(_position.x + _radius * Math::Cos(Math::DegreesToRadians(angle * i)), _position.y + _radius * Math::Sin(Math::DegreesToRadians(angle * i)), _position.z);

            drawLine(x, newX, _duration, _renderMode);
            drawLine(y, newY, _duration, _renderMode);
            drawLine(z, newZ, _duration, _renderMode);

            x = newX;
            y = newY;
            z = newZ;
        }
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireHemisphere(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F forward = _direction.normalizedCopy();
        Vec3F up = forward.perpendicular();
        Vec3F right = up.crossProduct(forward).normalizedCopy();

        setColor(_color);
        drawWireCircle(_position, _direction, _radius, _duration, _renderMode);

        drawWireHemicircle(_position, up, forward, _radius, _duration, _renderMode);
        drawWireHemicircle(_position, right, forward, _radius, _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireCylinder(
        Vec3F const& _start,
        Vec3F const& _end,
        F32 _radius,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F forward = (_end - _start).normalizedCopy() * _radius;
        Vec3F up = forward.perpendicular();
        Vec3F right = up.crossProduct(forward).normalizedCopy() * _radius;

        setColor(_color);

        // Radial circles
        drawWireCircle(_start, forward, _radius, _duration, _renderMode);
        drawWireCircle(_end, -forward, _radius, _duration, _renderMode);
        drawWireCircle((_start + _end) * 0.5f, forward, _radius, _duration, _renderMode);

        // Side lines
        drawLine(_start + right, _end + right, _duration, _renderMode);
        drawLine(_start - right, _end - right, _duration, _renderMode);

        drawLine(_start + up, _end + up, _duration, _renderMode);
        drawLine(_start - up, _end - up, _duration, _renderMode);

        // Start Endcap
        drawLine(_start - right, _start + right, _duration, _renderMode);
        drawLine(_start - up, _start + up, _duration, _renderMode);

        // End Endcap
        drawLine(_end - right, _end + right, _duration, _renderMode);
        drawLine(_end - up, _end + up, _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireCone(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _angle,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        F32 len = _direction.length();

        Vec3F forward = _direction;
        Vec3F up = forward.perpendicular();
        Vec3F right = up.crossProduct(forward).normalizedCopy() * len;

        Vec3F direction = _direction.normalizedCopy();

        Vec3F slerpedVector = forward.slerp(up, _angle / Math::c_halfPi);

        Plane farPlane(_position + forward, -direction);
        Ray distRay(_position, slerpedVector);

        F32 dist;
        farPlane.raycast(distRay, dist);

        setColor(_color);

        drawRay(_position, slerpedVector.normalizedCopy() * dist, _duration, _renderMode);
        drawRay(_position, forward.slerp(-up, _angle / Math::c_halfPi).normalizedCopy() * dist, _duration, _renderMode);
        drawRay(_position, forward.slerp(right, _angle / Math::c_halfPi).normalizedCopy() * dist, _duration, _renderMode);
        drawRay(_position, forward.slerp(-right, _angle / Math::c_halfPi).normalizedCopy() * dist, _duration, _renderMode);

        drawWireCircle(_position + forward, direction, (forward - (slerpedVector.normalizedCopy() * dist)).length(), _duration, _renderMode);
        drawWireCircle(_position + (forward * 0.5f), direction, ((forward * 0.5f) - (slerpedVector.normalizedCopy() * (dist * 0.5f))).length(), _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireTruncatedCone(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _angle,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        F32 len = _direction.length();

        Vec3F forward = _direction;
        Vec3F up = forward.perpendicular();
        Vec3F right = up.crossProduct(forward).normalizedCopy() * len;

        Vec3F forwardN = forward.normalizedCopy();
        Vec3F upN = up.normalizedCopy();
        Vec3F rightN = right.normalizedCopy();

        Vec3F direction = _direction.normalizedCopy();

        Vec3F slerpedVector = forward.slerp(up, _angle / Math::c_halfPi);

        Plane farPlane(_position + forward, -direction);
        Ray distRay(_position, slerpedVector);

        F32 dist;
        farPlane.raycast(distRay, dist);

        setColor(_color);

        drawWireCircle(_position, _direction, _radius, _duration, _renderMode);

        drawRay(_position + upN * _radius, slerpedVector.normalizedCopy() * dist, _duration, _renderMode);
        drawRay(_position - upN * _radius, forward.slerp(-up, _angle / Math::c_halfPi).normalizedCopy() * dist, _duration, _renderMode);
        drawRay(_position + rightN * _radius, forward.slerp(right, _angle / Math::c_halfPi).normalizedCopy() * dist, _duration, _renderMode);
        drawRay(_position - rightN * _radius, forward.slerp(-right, _angle / Math::c_halfPi).normalizedCopy() * dist, _duration, _renderMode);

        drawWireCircle(_position + forward, direction, (forward - (slerpedVector.normalizedCopy() * dist)).length() + _radius, _duration, _renderMode);
        drawWireCircle(_position + (forward * 0.5f), direction, ((forward * 0.5f) - (slerpedVector.normalizedCopy() * (dist * 0.5f))).length() + _radius, _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireTorus(
        Vec3F const& _position,
        Vec3F const& _direction,
        F32 _radius,
        F32 _torusRadius,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F forward = _direction;
        Vec3F up = forward.perpendicular();
        Vec3F right = up.crossProduct(forward).normalizedCopy();

        setColor(_color);
        drawWireCircle(_position + _direction * _torusRadius, _direction, _radius, _duration, _renderMode);
        drawWireCircle(_position - _direction * _torusRadius, _direction, _radius, _duration, _renderMode);
        drawWireCircle(_position, _direction, _radius + _torusRadius, _duration, _renderMode);
        drawWireCircle(_position, _direction, _radius - _torusRadius, _duration, _renderMode);

        drawWireCircle(_position + up * _radius, right, _torusRadius, _duration, _renderMode);
        drawWireCircle(_position - up * _radius, right, _torusRadius, _duration, _renderMode);
        drawWireCircle(_position + right * _radius, up, _torusRadius, _duration, _renderMode);
        drawWireCircle(_position - right * _radius, up, _torusRadius, _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireArrow(
        Vec3F const& _position,
        Vec3F const& _direction,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        setColor(_color);
        drawRay(_position, _direction, _duration, _renderMode);
        drawWireCone(_position + _direction, -_direction * 0.333f, Math::DegreesToRadians(15.0f), _color, _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawWireCapsule(
        Vec3F const& _start,
        Vec3F const& _end,
        F32 _radius,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F forward = (_end - _start).normalizedCopy() * _radius;
        Vec3F up = forward.perpendicular();
        Vec3F right = up.crossProduct(forward).normalizedCopy() * _radius;

        F32 height = (_start - _end).length();
        F32 sideLength = Math::Max(0.0f, (height * 0.5f) - _radius);
        Vec3F middle = (_end + _start) * 0.5f;

        Vec3F start = middle + ((_start - middle).normalizedCopy() * sideLength);
        Vec3F end = middle + ((_end - middle).normalizedCopy() * sideLength);

        setColor(_color);

        // Radial circles
        drawWireCircle(start, forward, _radius, _duration, _renderMode);
        drawWireCircle(end, -forward, _radius, _duration, _renderMode);

        // Side lines
        drawLine(start + right, end + right, _duration, _renderMode);
        drawLine(start - right, end - right, _duration, _renderMode);

        drawLine(start + up, end + up, _duration, _renderMode);
        drawLine(start - up, end - up, _duration, _renderMode);

        for (S32 i = 1; i < 26; i++) {

            // Start endcap
            drawLine(Vec3F::SLerp(right, -forward, (F32)i / 25.0f) + start, Vec3F::SLerp(right, -forward, F32(i - 1) / 25.0f) + start, _duration, _renderMode);
            drawLine(Vec3F::SLerp(-right, -forward, (F32)i / 25.0f) + start, Vec3F::SLerp(-right, -forward, F32(i - 1) / 25.0f) + start, _duration, _renderMode);
            drawLine(Vec3F::SLerp(up, -forward, (F32)i / 25.0f) + start, Vec3F::SLerp(up, -forward, F32(i - 1) / 25.0f) + start, _duration, _renderMode);
            drawLine(Vec3F::SLerp(-up, -forward, (F32)i / 25.0f) + start, Vec3F::SLerp(-up, -forward, F32(i - 1) / 25.0f) + start, _duration, _renderMode);

            // End endcap
            drawLine(Vec3F::SLerp(right, forward, (F32)i / 25.0f) + end, Vec3F::SLerp(right, forward, F32(i - 1) / 25.0f) + end, _duration, _renderMode);
            drawLine(Vec3F::SLerp(-right, forward, (F32)i / 25.0f) + end, Vec3F::SLerp(-right, forward, F32(i - 1) / 25.0f) + end, _duration, _renderMode);
            drawLine(Vec3F::SLerp(up, forward, (F32)i / 25.0f) + end, Vec3F::SLerp(up, forward, F32(i - 1) / 25.0f) + end, _duration, _renderMode);
            drawLine(Vec3F::SLerp(-up, forward, (F32)i / 25.0f) + end, Vec3F::SLerp(-up, forward, F32(i - 1) / 25.0f) + end, _duration, _renderMode);
        }
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawAABB(
        AABB2D const& _aabb,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().x, _aabb.getMin().y, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().x, _aabb.getMax().y, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().x, _aabb.getMax().y, _duration, _renderMode);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().x, _aabb.getMin().y, _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawAABB(
        AABB3D const& _aabb,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMin().y, _aabb.getMin().z, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().z, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().z, _duration, _renderMode);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMin().y, _aabb.getMin().z, _duration, _renderMode);

        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().z, _aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().z, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().z, _aabb.getMax().x, _aabb.getMax().y, _aabb.getMax().z, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMax().z, _aabb.getMin().x, _aabb.getMax().y, _aabb.getMax().z, _duration, _renderMode);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMax().z, _aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().z, _duration, _renderMode);

        drawLine(_aabb.getMin().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMin().y, _aabb.getMax().z, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMin().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMin().y, _aabb.getMax().z, _duration, _renderMode);
        drawLine(_aabb.getMax().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMax().x, _aabb.getMax().y, _aabb.getMax().z, _duration, _renderMode);
        drawLine(_aabb.getMin().x, _aabb.getMax().y, _aabb.getMin().z, _aabb.getMin().x, _aabb.getMax().y, _aabb.getMax().z, _duration, _renderMode);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawTriangle(
        Vec3F const& _point0,
        Vec3F const& _point1,
        Vec3F const& _point2,
        MeshRenderMode _renderMode)
    {
        MeshData& triangles = m_triangles[(S32)_renderMode];

        U32 index0 = (U32)triangles.vertices.size();
        U32 index1 = index0 + 1;
        U32 index2 = index1 + 1;

        triangles.indices.emplace_back(index0);
        triangles.indices.emplace_back(index1);
        triangles.indices.emplace_back(index2);

        triangles.vertices.emplace_back(transformPoint(_point0));
        triangles.vertices.emplace_back(transformPoint(_point1));
        triangles.vertices.emplace_back(transformPoint(_point2));

        triangles.colors.emplace_back(m_color.value);
        triangles.colors.emplace_back(m_color.value);
        triangles.colors.emplace_back(m_color.value);
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawTriangle(
        Vec3F const& _point0,
        Vec3F const& _point1,
        Vec3F const& _point2,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        if (_duration < 0.0f)
        {
            drawTriangle(_point0, _point1, _point2, _renderMode);
            return;
        }

        m_bufferedTriangles.emplace_back(
            BufferedTriangle
            {
                _duration,
                transformPoint(_point0),
                transformPoint(_point1),
                transformPoint(_point2),
                getColor(),
                _renderMode
            });
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawMesh(
        MeshPtr const& _mesh,
        Vec3F const& _position,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        setColor(_color);

        for (Size s = 0; s < _mesh->getSubMeshesCount(); ++s)
        {
            SubMeshPtr const& subMesh = _mesh->getSubMesh(s);
            S32 indicesCount = (S32)subMesh->getIndicesCount();
            for (S32 i = 0; i < indicesCount; i += 3)
            {
                S32* indices = (S32*)subMesh->getIndicesData();
                S32 index0 = indices[i + 0];
                S32 index1 = indices[i + 1];
                S32 index2 = indices[i + 2];

                Vec3F* position = (Vec3F*)subMesh->getVertexData(VertexAttributeSemantic::Position);
                drawTriangle(
                    _position + position[index0],
                    _position + position[index1],
                    _position + position[index2],
                    _duration,
                    _renderMode);
            }
        }
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawQuad(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec2F const& _scale,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F right = _up.crossProduct(_forward).normalizedCopy();

        pushTransform(
            Mat4F::CreateTranslationMatrix(_position) *
            Mat4F::CreateChangeOfBasisMatrix(right, _up, _forward) *
            Mat4F::CreateScaleMatrix(_scale.x, _scale.y, 1.0f));

        MeshPtr const& mesh = MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Quad);
        drawMesh(mesh, Vec3F::c_zero, _color, _duration, _renderMode);

        popTransform();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawCube(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        Vec3F const& _scale,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F right = _up.crossProduct(_forward).normalizedCopy();

        pushTransform(
            Mat4F::CreateTranslationMatrix(_position) *
            Mat4F::CreateChangeOfBasisMatrix(right, _up, _forward) *
            Mat4F::CreateScaleMatrix(_scale));

        MeshPtr const& mesh = MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cube);
        drawMesh(mesh, Vec3F::c_zero, _color, _duration, _renderMode);

        popTransform();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawSphere(
        Vec3F const& _position,
        F32 _radius,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        pushTransform(
            Mat4F::CreateTranslationMatrix(_position) *
            Mat4F::CreateScaleMatrix(_radius * 2.0f));

        MeshPtr const& mesh = MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Sphere);
        drawMesh(mesh, Vec3F::c_zero, _color, _duration, _renderMode);

        popTransform();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawCone(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        F32 _radius,
        F32 _height,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F right = _up.crossProduct(_forward).normalizedCopy();

        pushTransform(
            Mat4F::CreateTranslationMatrix(_position) *
            Mat4F::CreateChangeOfBasisMatrix(right, _forward, _up) *
            Mat4F::CreateScaleMatrix(_radius * 2.0f, _height, _radius * 2.0f));

        MeshPtr const& mesh = MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cone);
        drawMesh(mesh, Vec3F::c_zero, _color, _duration, _renderMode);

        popTransform();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawCylinder(
        Vec3F const& _position,
        Vec3F const& _forward,
        Vec3F const& _up,
        F32 _radius,
        F32 _height,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F right = _up.crossProduct(_forward).normalizedCopy();

        pushTransform(
            Mat4F::CreateTranslationMatrix(_position) *
            Mat4F::CreateChangeOfBasisMatrix(right, _forward, _up) *
            Mat4F::CreateScaleMatrix(_radius * 2.0f, _height, _radius * 2.0f));

        MeshPtr const& mesh = MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cylinder);
        drawMesh(mesh, Vec3F::c_zero, _color, _duration, _renderMode);

        popTransform();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawTorus(
        Vec3F const& _position,
        Vec3F const& _forward,
        F32 _radius,
        ColorF128 const& _color,
        F32 _duration,
        MeshRenderMode _renderMode)
    {
        Vec3F up = _forward.perpendicular();
        Vec3F right = up.crossProduct(_forward).normalizedCopy();

        pushTransform(
            Mat4F::CreateTranslationMatrix(_position) *
            Mat4F::CreateChangeOfBasisMatrix(right, _forward, up) *
            Mat4F::CreateScaleMatrix(_radius * 2.0f));

        MeshPtr const& mesh = MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Torus);
        drawMesh(mesh, Vec3F::c_zero, _color, _duration, _renderMode);

        popTransform();
    }

    //////////////////////////////////////////
    void GizmosDrawer::drawBillboard(
        Vec3F const& _point,
        SpritePtr const& _sprite,
        ColorF128 const& _color)
    {
        m_billboardsData.emplace_back(
            BillboardData
            {
                transformPoint(_point),
                _sprite,
                _color
            });
    }

    //////////////////////////////////////////
    void GizmosDrawer::pushTransform(Mat4F const& _tm)
    {
        if (!m_transformStack.empty())
            m_transformStack.push(m_transformStack.top() * _tm);
        else
            m_transformStack.push(_tm);
    }

    //////////////////////////////////////////
    void GizmosDrawer::popTransform()
    {
        MAZE_ERROR_RETURN_IF(m_transformStack.empty(), "Transform stack is empty!");
        m_transformStack.pop();
    }

    //////////////////////////////////////////
    void GizmosDrawer::update(F32 _dt)
    {
        // Buffered lines
        for (auto it = m_bufferedLines.begin(), end = m_bufferedLines.end(); it != end;)
        {
            setColor(it->color);
            drawLine(it->point0, it->point1, it->renderMode);

            it->timer -= _dt;
            if (it->timer <= 0.0f)
                it = m_bufferedLines.erase(it);
            else
                ++it;
        }

        // Buffered triangles
        for (auto it = m_bufferedTriangles.begin(), end = m_bufferedTriangles.end(); it != end;)
        {
            setColor(it->color);
            drawTriangle(it->point0, it->point1, it->point2, it->renderMode);

            it->timer -= _dt;
            if (it->timer <= 0.0f)
                it = m_bufferedTriangles.erase(it);
            else
                ++it;
        }

        rebuildMeshes();
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

        for (S32 renderMode = 0; renderMode < (S32)MeshRenderMode::MAX; ++renderMode)
        {
            MeshData& lines = m_lines[(S32)renderMode];
            if (!lines.vertices.empty())
            {
                lines.vao->setVerticesData((U8 const*)&lines.vertices[0], c_positionDescription, lines.vertices.size());
                lines.vao->setVerticesData((U8 const*)&lines.colors[0], c_colorDescription, lines.colors.size());
                lines.vao->setIndices((U8 const*)&lines.indices[0], VertexAttributeType::U32, lines.indices.size());
            }

            MeshData& triangles = m_triangles[(S32)renderMode];
            if (!triangles.vertices.empty())
            {
                triangles.vao->setVerticesData((U8 const*)&triangles.vertices[0], c_positionDescription, triangles.vertices.size());
                triangles.vao->setVerticesData((U8 const*)&triangles.colors[0], c_colorDescription, triangles.colors.size());
                triangles.vao->setIndices((U8 const*)&triangles.indices[0], VertexAttributeType::U32, triangles.indices.size());
            }
        }

        if (GizmosManager::GetInstancePtr())
        {
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
                        material->setUniform(MAZE_HS("u_baseMap"), gizmoBillboardData.sprite->getTexture());
                        material->setUniform(MAZE_HS("u_uv0"), gizmoBillboardData.sprite->getTextureCoordLB());
                        material->setUniform(MAZE_HS("u_uv1"), gizmoBillboardData.sprite->getTextureCoordRT());
                    }
                    else
                    {
                        material->setUniform(MAZE_HS("u_baseMap"), renderSystem->getTextureManager()->getErrorTexture());
                        material->setUniform(MAZE_HS("u_uv0"), Vec2F::c_zero);
                        material->setUniform(MAZE_HS("u_uv1"), Vec2F::c_one);
                    }

                    material->setUniform(MAZE_HS("u_color"), gizmoBillboardData.color.value);

                    Mat4F mat = Mat4F::CreateLookAtMatrix(
                        gizmoBillboardData.point,
                        gizmoBillboardData.point - camera3D->getTransform()->getWorldForwardDirection(),
                        camera3D->getTransform()->getWorldUpDirection());

                    gizmoBillboard->getTransform()->setLocalTransform(mat);
                }
            }
            else
            {
                while (!m_billboards.empty())
                {
                    m_gizmoBillboardPool->release(m_billboards.back());
                    m_billboards.pop_back();
                }
            }
        }
    }

    //////////////////////////////////////////
    Vec3F GizmosDrawer::transformPoint(Vec3F const& _p)
    {
        if (m_transformStack.empty())
            return _p;
        else
            return m_transformStack.top().transformAffine(_p);
    }

    
} // namespace Maze
//////////////////////////////////////////
