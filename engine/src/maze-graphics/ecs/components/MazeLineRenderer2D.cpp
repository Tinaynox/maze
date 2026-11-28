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
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include <cstring>


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
    static F32 const c_lineRendererEpsilon = 1e-6f;


    //////////////////////////////////////////
    // Class LineRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(LineRenderer2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, width, 1.0f, getWidth, setWidth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, featherWidth, 0.0f, getFeatherWidth, setFeatherWidth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorGradient, color, ColorGradient(), getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<Vec2F>, positions, Vector<Vec2F>(), getPositions, setPositions));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(LineRenderer2D);

    //////////////////////////////////////////
    F32 const LineRenderer2D::c_miterLimit = 4.0f;

    //////////////////////////////////////////
    Size const LineRenderer2D::c_maxPositionsCount = 65535 / 4;


    //////////////////////////////////////////
    LineRenderer2D::LineRenderer2D()
        : m_renderSystem(nullptr)
        , m_width(1.0f)
        , m_featherWidth(0.0f)
        , m_color((Vec4F)ColorF128::c_white)
        , m_meshDirty(true)
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
    void LineRenderer2D::setWidth(F32 _width)
    {
        if (m_width == _width)
            return;

        m_width = _width;
        m_meshDirty = true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::setFeatherWidth(F32 _featherWidth)
    {
        _featherWidth = Math::Max(_featherWidth, 0.0f);
        if (m_featherWidth == _featherWidth)
            return;

        m_featherWidth = _featherWidth;
        m_meshDirty = true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::setColor(ColorF128 const& _color)
    {
        setColor(ColorGradient(0.0f, _color.toVec4F32()));
    }

    //////////////////////////////////////////
    void LineRenderer2D::setColor(ColorGradient const& _color)
    {
        m_color = _color;
        m_meshDirty = true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::resizePositions(Size _size)
    {
        if (m_positions.size() == _size)
            return;

        m_positions.resize(_size);
        m_meshDirty = true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::setPosition(Size _i, Vec2F const& _position)
    {
        MAZE_ERROR_RETURN_IF(_i >= m_positions.size(), "Position index %u is out of range (%u)!", (U32)_i, (U32)m_positions.size());

        if (m_positions[_i] == _position)
            return;

        m_positions[_i] = _position;
        m_meshDirty = true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::setPositions(Vec2F const* _positions, Size _count)
    {
        if (m_positions.size() == _count &&
            (_count == 0 || memcmp(m_positions.data(), _positions, _count * sizeof(Vec2F)) == 0))
            return;

        m_positions.assign(_positions, _positions + _count);
        m_meshDirty = true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::prepareForRender()
    {
        if (m_meshDirty)
            rebuildMesh();
    }

    //////////////////////////////////////////
    bool LineRenderer2D::calculateSegmentDirections(Size _pointsCount)
    {
        Size segmentsCount = _pointsCount - 1;

        m_segmentDirections.resize(segmentsCount);
        m_pointDistances.resize(_pointsCount);
        m_pointDistances[0] = 0.0f;

        S32 firstValidSegment = -1;
        for (Size i = 0; i < segmentsCount; ++i)
        {
            Vec2F delta = m_positions[i + 1] - m_positions[i];
            F32 length = delta.length();
            m_pointDistances[i + 1] = m_pointDistances[i] + length;

            if (length > c_lineRendererEpsilon)
            {
                m_segmentDirections[i] = delta / length;
                if (firstValidSegment < 0)
                    firstValidSegment = (S32)i;
            }
            else
            {
                m_segmentDirections[i] = (i > 0) ? m_segmentDirections[i - 1] : Vec2F::c_zero;
            }
        }

        if (firstValidSegment < 0)
            return false;

        for (S32 i = 0; i < firstValidSegment; ++i)
            m_segmentDirections[i] = m_segmentDirections[firstValidSegment];

        return true;
    }

    //////////////////////////////////////////
    void LineRenderer2D::rebuildMesh()
    {
        if (!m_vao)
            return;

        m_meshDirty = false;

        if (m_positions.size() > c_maxPositionsCount)
            MAZE_ERROR("LineRenderer2D positions count %u exceeds the limit %u, the line is truncated!", (U32)m_positions.size(), (U32)c_maxPositionsCount);

        Size pointsCount = Math::Min(m_positions.size(), c_maxPositionsCount);
        if (pointsCount < 2 || m_width <= 0.0f || !calculateSegmentDirections(pointsCount))
        {
            m_vao->clear();
            return;
        }

        bool const feathered = m_featherWidth > 0.0f;
        U32 const verticesPerPoint = feathered ? 4u : 2u;
        U32 const lanesCount = verticesPerPoint - 1u;
        U32 const verticesCount = verticesPerPoint * (U32)pointsCount;
        U32 const indicesCount = 6u * lanesCount * (U32)(pointsCount - 1);

        F32 const halfWidth = m_width * 0.5f;
        F32 const outerHalfWidth = halfWidth + m_featherWidth;
        F32 const innerV = feathered ? (m_featherWidth / (2.0f * outerHalfWidth)) : 0.0f;

        F32 const totalLength = m_pointDistances[pointsCount - 1];
        F32 const invTotalLength = totalLength > c_lineRendererEpsilon ? (1.0f / totalLength) : 0.0f;

        F32 const alpha = m_canvasRenderer ? m_canvasRenderer->getAlpha() : 1.0f;

        m_vertices.resize(verticesCount);
        m_uvs.resize(verticesCount);
        m_colors.resize(verticesCount);

        U32 vertex = 0;
        for (Size i = 0; i < pointsCount; ++i)
        {
            Vec2F normal;
            F32 miterScale = 1.0f;

            if (i == 0)
            {
                normal = m_segmentDirections[0].perpendicular();
            }
            else
            if (i == pointsCount - 1)
            {
                normal = m_segmentDirections[i - 1].perpendicular();
            }
            else
            {
                Vec2F prevNormal = m_segmentDirections[i - 1].perpendicular();
                Vec2F nextNormal = m_segmentDirections[i].perpendicular();
                Vec2F miter = prevNormal + nextNormal;
                F32 miterLength = miter.length();

                if (miterLength > c_lineRendererEpsilon)
                {
                    normal = miter / miterLength;
                    F32 cosHalfAngle = normal.dotProduct(nextNormal);
                    miterScale = Math::Min(1.0f / Math::Max(cosHalfAngle, c_lineRendererEpsilon), c_miterLimit);
                }
                else
                {
                    normal = nextNormal;
                }
            }

            Vec2F const& position = m_positions[i];
            Vec2F const coreOffset = normal * (halfWidth * miterScale);

            F32 const progress = m_pointDistances[i] * invTotalLength;
            Vec4F color = m_color.evaluate(progress);
            color.w *= alpha;

            if (feathered)
            {
                Vec2F const outerOffset = normal * (outerHalfWidth * miterScale);
                Vec4F const transparentColor(color.x, color.y, color.z, 0.0f);

                m_vertices[vertex] = Vec3F(position + outerOffset, 0.0f);
                m_uvs[vertex] = Vec2F(progress, 0.0f);
                m_colors[vertex++] = transparentColor;

                m_vertices[vertex] = Vec3F(position + coreOffset, 0.0f);
                m_uvs[vertex] = Vec2F(progress, innerV);
                m_colors[vertex++] = color;

                m_vertices[vertex] = Vec3F(position - coreOffset, 0.0f);
                m_uvs[vertex] = Vec2F(progress, 1.0f - innerV);
                m_colors[vertex++] = color;

                m_vertices[vertex] = Vec3F(position - outerOffset, 0.0f);
                m_uvs[vertex] = Vec2F(progress, 1.0f);
                m_colors[vertex++] = transparentColor;
            }
            else
            {
                m_vertices[vertex] = Vec3F(position + coreOffset, 0.0f);
                m_uvs[vertex] = Vec2F(progress, 0.0f);
                m_colors[vertex++] = color;

                m_vertices[vertex] = Vec3F(position - coreOffset, 0.0f);
                m_uvs[vertex] = Vec2F(progress, 1.0f);
                m_colors[vertex++] = color;
            }
        }

        m_indices.resize(indicesCount);
        U32 index = 0;
        for (U32 i = 0, in = (U32)pointsCount - 1u; i < in; ++i)
        {
            for (U32 lane = 0; lane < lanesCount; ++lane)
            {
                U16 const a = (U16)(i * verticesPerPoint + lane);
                U16 const b = (U16)(a + 1u);
                U16 const c = (U16)(a + verticesPerPoint);
                U16 const d = (U16)(c + 1u);

                m_indices[index++] = a;
                m_indices[index++] = b;
                m_indices[index++] = c;
                m_indices[index++] = b;
                m_indices[index++] = d;
                m_indices[index++] = c;
            }
        }

        m_vao->setVerticesData((U8 const*)m_vertices.data(), c_positionDescription, verticesCount);
        m_vao->setVerticesData((U8 const*)m_uvs.data(), c_uvDescription, verticesCount);
        m_vao->setVerticesData((U8 const*)m_colors.data(), c_colorDescription, verticesCount);
        m_vao->setIndices((U8 const*)m_indices.data(), VertexAttributeType::U16, indicesCount);
    }

    //////////////////////////////////////////
    void LineRenderer2D::ensureRequiredComponents()
    {
        if (!m_renderMask)
            m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
        if (!m_transform)
            m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        if (!m_meshRenderer)
            m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();
        if (!m_canvasRenderer)
            m_canvasRenderer = getEntityRaw()->ensureComponent<CanvasRenderer>();
    }

    //////////////////////////////////////////
    void LineRenderer2D::processEntityAwakened()
    {
        ensureRequiredComponents();
    }

    //////////////////////////////////////////
    void LineRenderer2D::processSceneSet()
    {
        MAZE_ERROR_RETURN_IF(!getEntityRaw(), "Entity is null");
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene(), "Entity Scene is null");

#if (MAZE_DEBUG)
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getEcsScene()->getMetaClass()->isInheritedFrom<EcsRenderScene>(), "Entity Scene is not Render Scene");
#endif

        ensureRequiredComponents();

        RenderTargetPtr const& renderTarget = getEntityRaw()->getEcsScene()->castRaw<EcsRenderScene>()->getRenderTarget();

        m_vao = VertexArrayObject::Create(m_renderSystem);
        m_renderMesh = renderTarget->createRenderMeshFromPool(1);
        m_renderMesh->setVertexArrayObject(m_vao);
        m_meshRenderer->setRenderMesh(m_renderMesh);

        if (m_meshRenderer->getMaterialRefs().empty())
            m_meshRenderer->setMaterial(m_renderSystem->getMaterialManager()->getColorMaterial());

        m_meshDirty = true;
        prepareForRender();
    }

    //////////////////////////////////////////
    void LineRenderer2D::processEntityRemoved()
    {
        m_renderMesh.reset();
    }

    //////////////////////////////////////////
    void LineRenderer2D::clear()
    {
        if (m_positions.empty())
            return;

        m_positions.clear();
        m_meshDirty = true;
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

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(LineRenderer2DSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render2DPostUpdateEvent& _event,
        Entity* _entity,
        LineRenderer2D* _lineRenderer)
    {
        if (_lineRenderer->getCanvasRenderer() && _lineRenderer->getCanvasRenderer()->isAlphaDirty())
            _lineRenderer->markMeshDirty();

        _lineRenderer->prepareForRender();
    }

} // namespace Maze
//////////////////////////////////////////
