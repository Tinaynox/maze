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
#include "maze-graphics/ecs/components/MazeLineRenderer2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"


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
    // Class LineRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(LineRenderer2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, width, 1.0f, getWidth, setWidth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorGradient, color, ColorGradient(), getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<Vec2F>, positions, Vector<Vec2F>(), getPositions, setPositions));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(LineRenderer2D);


    //////////////////////////////////////////
    LineRenderer2D::LineRenderer2D()
        : m_renderSystem(nullptr)
        , m_width(1.0f)
        , m_color((Vec4F)ColorF128::c_white)
    {
    }

    //////////////////////////////////////////
    LineRenderer2D::~LineRenderer2D()
    {
        
    }

    //////////////////////////////////////////
    LineRenderer2DPtr LineRenderer2D::Create(RenderSystem* _renderSystem)
    {
        LineRenderer2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LineRenderer2D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool LineRenderer2D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        m_renderSystem = _renderSystem;
        
        return true;
    }

    //////////////////////////////////////////
    bool LineRenderer2D::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<LineRenderer2D>()->m_renderSystem;

        if (!Component::init(_component, _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::rebuildMesh()
    {
        if (!m_vao)
            return;

        S32 quadsCount = (S32)m_positions.size() - 1;
        if (quadsCount <= 0 || m_width <= 0.0f)
        {
            m_vao->clear();
            return;
        }

        struct EdgeData
        {
            F32 progress;
            Vec2F direction;
            F32 halfWidth;
            F32 distanceToNextEdge;
        };

        F32 totalLength = 0.0f;

        Vector<EdgeData> edges;
        edges.resize((Size)quadsCount);
        for (S32 i = 0, in = quadsCount; i < in; ++i)
        {
            EdgeData& edge = edges[i];

            Vec2F toNextEdge = m_positions[i + 1] - m_positions[i];
            F32 toNextEdgeLength = toNextEdge.length();
            edge.direction = (toNextEdgeLength != 0.0f) ? (toNextEdge / toNextEdgeLength) : Vec2F::c_zero;

            if (i >= 1)
            {
                Vec2F prevDirection = (m_positions[i] - m_positions[i - 1]).normalizedCopy();
                edge.direction = ((prevDirection + edge.direction) * 0.5f).normalizedCopy();
            }

            edge.distanceToNextEdge = toNextEdgeLength;

            totalLength += toNextEdgeLength;
        }

        F32 l = 0.0f;
        for (S32 i = 0, in = quadsCount; i < in; ++i)
        {
            EdgeData& edge = edges[i];

            edge.progress = l / totalLength;
            edge.halfWidth = getTrailWidth(edge.progress);

            l += edge.distanceToNextEdge;
        }


        U32 verticesCount = 2 * quadsCount + 2;
        U32 indicesCount = 6 * quadsCount;

        m_vertices.resize(verticesCount);
        m_uvs.resize(verticesCount);
        m_colors.resize(verticesCount);

        U32 vertex = 0;
        U32 index = 0;

        Vec2F direction;
        Vec2F perpendicular;
        Vec2F vertexA;
        Vec2F vertexB;
        F32 width;
        F32 halfWidth;
        ColorF128 color;

        Vec2F currentPosition = m_positions[0];
        Vec2F nextPosition;
        for (S32 i = 0; i < (S32)(quadsCount); ++i)
        {
            EdgeData& edge = edges[i];

            width = getTrailWidth(edge.progress);
            color = ColorF128(m_color.evaluate(edge.progress));
            halfWidth = width * 0.5f;

            nextPosition = m_positions[i + 1];
            direction = edge.direction;
            
            perpendicular = direction.perpendicular();
            vertexA = currentPosition + perpendicular * halfWidth;
            vertexB = currentPosition - perpendicular * halfWidth;

            m_vertices[vertex] = vertexA;
            m_uvs[vertex] = Vec2F(edge.progress, 0.0f);
            m_colors[vertex] = color.toVec4F32();
            ++vertex;

            m_vertices[vertex] = vertexB;
            m_uvs[vertex] = Vec2F(edge.progress, 1.0f);
            m_colors[vertex] = color.toVec4F32();
            ++vertex;

            
            currentPosition = nextPosition;
        }
        
        width = getTrailWidth(1.0f);
        color = ColorF128(m_color.evaluate(1.0f));
        halfWidth = width * 0.5f;
        direction = (m_positions.back() - m_positions[m_positions.size() - 2]).normalizedCopy();
        perpendicular = direction.perpendicular();
        vertexA = currentPosition + perpendicular * halfWidth;
        vertexB = currentPosition - perpendicular * halfWidth;

        m_vertices[vertex] = vertexA;
        m_uvs[vertex] = Vec2F(1.0f, 0.0f);
        m_colors[vertex] = color.toVec4F32();
        ++vertex;

        m_vertices[vertex] = vertexB;
        m_uvs[vertex] = Vec2F(1.0f, 1.0f);
        m_colors[vertex] = color.toVec4F32();
        ++vertex;

        m_vao->setVerticesData((U8 const*)&m_vertices[0], c_positionDescription, verticesCount);
        m_vao->setVerticesData((U8 const*)&m_uvs[0], c_uvDescription, verticesCount);
        m_vao->setVerticesData((U8 const*)&m_colors[0], c_colorDescription, verticesCount);

        m_indices.resize(indicesCount);
        for (S32 i = 0; i < quadsCount; i++)
        {
            U16 i2 = i * 2;
            m_indices[index++] = 0 + i2;
            m_indices[index++] = 1 + i2;
            m_indices[index++] = 2 + i2;
            m_indices[index++] = 1 + i2;
            m_indices[index++] = 3 + i2;
            m_indices[index++] = 2 + i2;
        }
        
        m_vao->setIndices((U8 const*)&m_indices[0], VertexAttributeType::U16, indicesCount);

    }

    //////////////////////////////////////////
    void LineRenderer2D::processEntityAwakened()
    {
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();
    }

    //////////////////////////////////////////
    void LineRenderer2D::processSceneSet()
    {
        MAZE_ERROR_RETURN_IF(!getEntityRaw(), "Entity is null");
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene(), "Entity Scene is null");

#if (MAZE_DEBUG)
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene()->getMetaClass()->isInheritedFrom<EcsRenderScene>(), "Entity Scene is not Render Scene");
#endif

        RenderTargetPtr const& renderTarget = getEntityRaw()->getEcsScene()->castRaw<EcsRenderScene>()->getRenderTarget();

        m_vao = VertexArrayObject::Create(m_renderSystem);
        m_renderMesh = renderTarget->createRenderMeshFromPool(1);
        m_renderMesh->setVertexArrayObject(m_vao);
        m_meshRenderer->setRenderMesh(m_renderMesh);

        rebuildMesh();
    }

    //////////////////////////////////////////
    void LineRenderer2D::processEntityRemoved()
    {
        m_renderMesh.reset();
    }
    
    //////////////////////////////////////////
    F32 LineRenderer2D::getTrailWidth(F32 _progress)
    {
        return m_width;
    }
    
    //////////////////////////////////////////
    void LineRenderer2D::clear()
    {
        m_positions.clear();
        if (m_vao)
            m_vao->clear();        
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(LineRenderer2DEntityRemoved,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        EntityRemovedEvent const& _event,
        Entity* _entity,
        LineRenderer2D* _lineRenderer2D)
    {
        _lineRenderer2D->processEntityRemoved();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER_EX(LineRenderer2DOnEcsWorldWillBeDestroyed,
        {},
        {},
        (U8)EntitiesSampleFlags::IncludeInactive,
        EcsWorldWillBeDestroyedEvent const& _event,
        Entity* _entity,
        LineRenderer2D* _lineRenderer)
    {
        // Release render mesh before RenderMeshPool will be destroyed
        _lineRenderer->processEntityRemoved();
    }

} // namespace Maze
//////////////////////////////////////////
