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
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static VertexAttributeDescription const c_positionDescription = VertexAttributeDescription
        {
            VertexAttributeSemantic::Position,
            3,
            VertexAttributeType::F32,
            false,
            3 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
            0
        };

    //////////////////////////////////////////
    static VertexAttributeDescription const c_uvDescription = VertexAttributeDescription
        {
            VertexAttributeSemantic::TexCoords0,
            2,
            VertexAttributeType::F32,
            false,
            2 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
            0
        };

    //////////////////////////////////////////
    static VertexAttributeDescription const c_colorDescription = VertexAttributeDescription
        {
            VertexAttributeSemantic::Color,
            4,
            VertexAttributeType::F32,
            false,
            4 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
            0
        };


    //////////////////////////////////////////
    // Class TrailRenderer3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TrailRenderer3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<MaterialAssetRef>, materials, Vector<MaterialAssetRef>(), getMaterialRefs, setMaterialRefs),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, time, 2.0f, getTime, setTime),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, minVertexDistance, 0.35f, getMinVertexDistance, setMinVertexDistance),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, width, 1.0f, getWidth, setWidth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorGradient, color, ColorGradient(), getColor, setColor));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TrailRenderer3D);


    //////////////////////////////////////////
    TrailRenderer3D::TrailRenderer3D()
        : m_renderSystem(nullptr)
        , m_time(2.0f)
        , m_width(1.0f)
        , m_color(ColorF128::c_white)
    {
        setMinVertexDistance(0.35f);
    }

    //////////////////////////////////////////
    TrailRenderer3D::~TrailRenderer3D()
    {
        
    }

    //////////////////////////////////////////
    TrailRenderer3DPtr TrailRenderer3D::Create(RenderSystem* _renderSystem)
    {
        TrailRenderer3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TrailRenderer3D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool TrailRenderer3D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        m_renderSystem = _renderSystem;
        
        return true;
    }

    //////////////////////////////////////////
    bool TrailRenderer3D::init(
        Component* _component,
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<TrailRenderer3D>()->m_renderSystem;

        if (!Component::init(_component, _world, _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void TrailRenderer3D::setMaterial(MaterialPtr const& _material)
    {
        Vector<MaterialAssetRef> materials = { MaterialAssetRef(_material) };
        setMaterialRefs(materials);
    }

    //////////////////////////////////////////
    void TrailRenderer3D::setMaterial(String const& _materialName)
    {
        MaterialPtr const& material = m_renderSystem->getMaterialManager()->getMaterial(_materialName);
        MAZE_ERROR_IF(!material, "Undefined material: %s", _materialName.c_str());
        setMaterial(material);
    }

    //////////////////////////////////////////
    void TrailRenderer3D::update(F32 _dt)
    {
        if (_dt == 0.0f)
            return;

        m_timer += _dt;

        for (S32 i = 0; i < (S32)m_edges.size() - 1;)
        {
            if (m_timer - m_edges[i + (S32)1].time > m_time)
            {
                m_edges.pop_front();
            }
            else
            {
                break;
            }
        }

        Vec3F worldPosition = getTransform()->getWorldPosition();
        if (m_edges.size() < 2)
        {
            addEdge(m_timer, worldPosition);
        }
        else
        {
            Size edgesCount = m_edges.size();
            
            TrailEdge& lastKeyframe = m_edges[edgesCount - 1];
            TrailEdge& penultimateKeyframe = m_edges[edgesCount - 2];
            
            Vec3F penultimateToLast = (worldPosition - penultimateKeyframe.position);
            F32 penultimateToLastLengthSq = penultimateToLast.squaredLength();
            if (penultimateToLastLengthSq <= 1e-6)
                return;

            lastKeyframe.position = worldPosition;

            F32 penultimateToLastLength = sqrt(penultimateToLastLengthSq);
            Vec3F direction = penultimateToLast / penultimateToLastLength;
            lastKeyframe.direction = direction;

            if (edgesCount > 2)
            {
                TrailEdge& beforePenultimateKeyframe = m_edges[edgesCount - 3];

                Vec3F prevDirection = (penultimateKeyframe.position - beforePenultimateKeyframe.position).normalizedCopy();
                F32 dotProduct = prevDirection.dotProduct(direction);
                F32 crossProductZ = prevDirection.crossProduct(direction).z;

                if (dotProduct >= 0.0f)
                {
                    penultimateKeyframe.direction = ((prevDirection + direction) * 0.5f).normalizedCopy();
                }
                else
                {
                    if (crossProductZ > 0.0)
                        penultimateKeyframe.direction = ((prevDirection.crossProduct(Vec3F::c_negativeUnitZ) + prevDirection) * 0.5f).normalizedCopy();
                    else
                        penultimateKeyframe.direction = ((prevDirection.crossProduct(Vec3F::c_unitZ) + prevDirection) * 0.5f).normalizedCopy();
                }
            }
            else
            {
                penultimateKeyframe.direction = direction;
            }

            penultimateKeyframe.distanceToNextEdge = penultimateToLastLength;

            rebuildMesh();

            if (penultimateToLastLengthSq >= m_minVertexDistanceSqr)
            {
                addEdge(m_timer, worldPosition);
            }
        }
    }

    //////////////////////////////////////////
    void TrailRenderer3D::rebuildMesh()
    {
        S32 quadsCount = (S32)m_edges.size() - 1;
        if (quadsCount <= 0)
        {
            m_vao->clear();
            return;
        }

        F32 t = m_timer - m_time;
        F32 firstEdgeTime = m_edges[0].time;
        F32 secondEdgeTime = m_edges[1].time;

        F32 p = Math::Clamp01((t - firstEdgeTime) / (secondEdgeTime - firstEdgeTime));

        F32 firstQuadLength = m_edges[0].distanceToNextEdge * (1.0f - p);
        F32 trailLength = firstQuadLength;
        for (S32 i = 1; i < (S32)(quadsCount); ++i)
            trailLength += m_edges[i].distanceToNextEdge;


        U32 verticesCount = 2 * quadsCount + 2;
        U32 indicesCount = 6 * quadsCount;

        m_vertices.resize(verticesCount);
        m_uvs.resize(verticesCount);
        m_colors.resize(verticesCount);

        U32 vertex = 0;
        U32 index = 0;

        Vec3F direction;
        Vec3F perpendicular;
        Vec3F vertexA;
        Vec3F vertexB;
        F32 width;
        F32 halfWidth;
        F32 currentLength = 0.0f;

        Vec3F currentPosition = Math::Lerp(m_edges[0].position, m_edges[1].position, p);
        Vec3F nextPosition;
        for (S32 i = 0; i < (S32)(quadsCount); ++i)
        {
            F32 progress = currentLength / trailLength;
            width = getTrailWidth(progress);
            halfWidth = width * 0.5f;

            nextPosition = m_edges[i + 1].position;
            direction = m_edges[i].direction;
            
            perpendicular = direction.crossProduct(Vec3F::c_unitZ);
            vertexA = currentPosition + perpendicular * halfWidth;
            vertexB = currentPosition - perpendicular * halfWidth;

            Vec4F color = m_color.evaluate(progress);

            m_vertices[vertex] = vertexA;
            m_uvs[vertex] = Vec2F(progress, 0.0f);
            m_colors[vertex] = color;
            ++vertex;

            m_vertices[vertex] = vertexB;
            m_uvs[vertex] = Vec2F(progress, 1.0f);
            m_colors[vertex] = color;
            ++vertex;

            
            currentPosition = nextPosition;

            if (i == 0)
                currentLength += firstQuadLength;
            else
                currentLength += m_edges[i].distanceToNextEdge;
        }
        
        width = getTrailWidth(1.0f);
        halfWidth = width * 0.5f;
        direction = (m_edges.back().position - m_edges[m_edges.size() - 2].position).normalizedCopy();
        perpendicular = direction.crossProduct(Vec3F::c_unitZ);
        vertexA = currentPosition + perpendicular * halfWidth;
        vertexB = currentPosition - perpendicular * halfWidth;

        Vec4F color = m_color.evaluate(1.0f);

        m_vertices[vertex] = vertexA;
        m_uvs[vertex] = Vec2F(1.0f, 0.0f);
        m_colors[vertex] = color;
        ++vertex;

        m_vertices[vertex] = vertexB;
        m_uvs[vertex] = Vec2F(1.0f, 1.0f);
        m_colors[vertex] = color;
        ++vertex;

        m_vao->setVerticesData((U8 const*)&m_vertices[0], c_positionDescription, verticesCount);
        m_vao->setVerticesData((U8 const*)&m_uvs[0], c_uvDescription, verticesCount);
        m_vao->setVerticesData((U8 const*)&m_colors[0], c_colorDescription, verticesCount);

        m_indices.resize(indicesCount);
        for (S32 i = 0; i < quadsCount; i++)
        {
            S32 i2 = i * 2;
            m_indices[index++] = 0 + i2;
            m_indices[index++] = 1 + i2;
            m_indices[index++] = 2 + i2;
            m_indices[index++] = 1 + i2;
            m_indices[index++] = 3 + i2;
            m_indices[index++] = 2 + i2;
        }
        
        m_vao->setIndices((U8 const*)&m_indices[0], VertexAttributeType::U32, indicesCount);

    }

    //////////////////////////////////////////
    void TrailRenderer3D::processEntityAwakened()
    {
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }

    //////////////////////////////////////////
    void TrailRenderer3D::processSceneSet()
    {
        MAZE_ERROR_RETURN_IF(!getEntityRaw(), "Entity is null");
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getECSScene(), "Entity Scene is null");

#if (MAZE_DEBUG)
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getECSScene()->getMetaClass()->isInheritedFrom<ECSRenderScene>(), "Entity Scene is not Render Scene");
#endif

        RenderTargetPtr const& renderTarget = getEntityRaw()->getECSScene()->castRaw<ECSRenderScene>()->getRenderTarget();

        m_vao = VertexArrayObject::Create(m_renderSystem);
        m_renderMesh = renderTarget->createRenderMeshFromPool(1);
        m_renderMesh->setVertexArrayObject(m_vao);
    }

    //////////////////////////////////////////
    void TrailRenderer3D::processEntityRemoved()
    {
        m_renderMesh.reset();
    }
    
    //////////////////////////////////////////
    void TrailRenderer3D::addEdge(
        F32 _time,
        Vec3F const& _position)
    {
        TrailEdge trailEdge(
            _time,
            _position);

        m_edges.push_back(trailEdge);
    }

    //////////////////////////////////////////
    F32 TrailRenderer3D::getTrailWidth(F32 _progress)
    {
        return m_width;
    }
    
    //////////////////////////////////////////
    void TrailRenderer3D::clear()
    {
        m_edges.clear();
        m_vao->clear();
        m_timer = 0.0f;
    }

} // namespace Maze
//////////////////////////////////////////
