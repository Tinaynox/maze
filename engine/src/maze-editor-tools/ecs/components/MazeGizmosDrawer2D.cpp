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
#include "maze-editor-tools/ecs/components/MazeGizmosDrawer2D.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static VertexAttributeDescription const c_positionDescription2D = VertexAttributeDescription
        {
            VertexAttributeSemantic::Position,
            3,
            VertexAttributeType::F32,
            false,
            3 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
            0
        };

    //////////////////////////////////////////
    static VertexAttributeDescription const c_colorDescription2D = VertexAttributeDescription
        {
            VertexAttributeSemantic::Color,
            4,
            VertexAttributeType::F32,
            false,
            4 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
            0
        };


    //////////////////////////////////////////
    GizmosDrawer2D::GizmosDrawer2D()
    {

    }

    //////////////////////////////////////////
    GizmosDrawer2D::~GizmosDrawer2D()
    {
        destroy();
    }

    //////////////////////////////////////////
    GizmosDrawer2DPtr GizmosDrawer2D::Create()
    {
        GizmosDrawer2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(GizmosDrawer2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool GizmosDrawer2D::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::clear()
    {
        m_vertices.clear();
        m_colors.clear();
        m_indices.clear();
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::destroy()
    {
        clear();
        destroyLinesEntity();
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::destroyLinesEntity()
    {
        if (m_linesEntity)
            m_linesEntity->removeFromEcsWorld();

        m_linesEntity.reset();
        m_linesTransform.reset();
        m_linesMeshRenderer.reset();
        m_linesVao.reset();
        m_linesRenderMesh.reset();
        m_canvasKey = nullptr;
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::ensureLinesEntity()
    {
        CanvasPtr const& canvas = GizmosManager::GetInstancePtr() ? GizmosManager::GetInstancePtr()->getCanvas() : CanvasPtr();
        if (!canvas || !canvas->getEntityRaw() || !canvas->getEntityRaw()->getEcsScene())
        {
            if (m_linesEntity)
                destroyLinesEntity();
            return;
        }

        // The editor overlay canvas is swapped when toggling between edit and
        // playtest mode -- rebuild instead of reusing/reparenting the entity,
        // whose old canvas (and thus the entity itself) may already be destroyed.
        if (m_canvasKey != canvas.get())
        {
            destroyLinesEntity();
            m_canvasKey = canvas.get();

            RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

            m_linesEntity = canvas->getEntityRaw()->getEcsScene()->createEntity("Gizmos2DLines");

            m_linesTransform = m_linesEntity->ensureComponent<Transform2D>();
            m_linesTransform->setParent(canvas->getTransform());
            m_linesTransform->setAnchor(Vec2F::c_zero);
            m_linesTransform->setPivot(Vec2F::c_zero);
            m_linesTransform->setLocalPosition(Vec2F::c_zero);
            m_linesTransform->setSize(Vec2F::c_zero);

            m_linesVao = VertexArrayObject::Create(renderSystem);
            m_linesVao->setRenderDrawTopology(RenderDrawTopology::Lines);

            RenderTargetPtr const& renderTarget = canvas->getRenderTarget();
            m_linesRenderMesh = renderTarget->createRenderMeshFromPool(1);
            m_linesRenderMesh->setName(MAZE_HS("Gizmos2DLines"));
            m_linesRenderMesh->setVertexArrayObject(m_linesVao);

            m_linesMeshRenderer = m_linesEntity->createComponent<MeshRenderer>();
            m_linesMeshRenderer->setRenderMesh(m_linesRenderMesh);
            m_linesMeshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getColorMaterial());
        }
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::drawLine(
        Vec2F const& _point0,
        Vec2F const& _point1)
    {
        U32 index0 = (U32)m_vertices.size();
        U32 index1 = index0 + 1;

        m_indices.emplace_back(index0);
        m_indices.emplace_back(index1);

        m_vertices.emplace_back(Vec3F(_point0.x, _point0.y, 0.0f));
        m_vertices.emplace_back(Vec3F(_point1.x, _point1.y, 0.0f));

        Vec4F color = m_color.toVec4F32();
        m_colors.emplace_back(color);
        m_colors.emplace_back(color);
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::drawPolyline(
        Vector<Vec2F> const& _points,
        bool _closed)
    {
        if (_points.size() < 2)
            return;

        for (Size i = 0, in = _points.size() - 1; i < in; ++i)
            drawLine(_points[i], _points[i + 1]);

        if (_closed)
            drawLine(_points.back(), _points.front());
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::drawWireRect(
        Vec2F const _corners[4])
    {
        drawLine(_corners[0], _corners[1]);
        drawLine(_corners[1], _corners[2]);
        drawLine(_corners[2], _corners[3]);
        drawLine(_corners[3], _corners[0]);
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::drawCross(
        Vec2F const& _center,
        F32 _size)
    {
        F32 halfSize = _size * 0.5f;
        drawLine(_center - Vec2F(halfSize, 0.0f), _center + Vec2F(halfSize, 0.0f));
        drawLine(_center - Vec2F(0.0f, halfSize), _center + Vec2F(0.0f, halfSize));
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::drawCircle(
        Vec2F const& _center,
        F32 _radius,
        S32 _segments)
    {
        if (_segments < 3)
            return;

        Vec2F lastPoint = _center + Vec2F(_radius, 0.0f);
        for (S32 i = 1; i <= _segments; ++i)
        {
            F32 angle = Math::DegreesToRadians(360.0f * (F32)i / (F32)_segments);
            Vec2F nextPoint = _center + Vec2F(Math::Cos(angle), Math::Sin(angle)) * _radius;
            drawLine(lastPoint, nextPoint);
            lastPoint = nextPoint;
        }
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::rebuildMesh()
    {
        if (!m_linesVao)
            return;

        if (m_vertices.empty())
        {
            m_linesVao->clear();
            return;
        }

        m_linesVao->setVerticesData((U8 const*)&m_vertices[0], c_positionDescription2D, m_vertices.size());
        m_linesVao->setVerticesData((U8 const*)&m_colors[0], c_colorDescription2D, m_colors.size());
        m_linesVao->setIndices((U8 const*)&m_indices[0], VertexAttributeType::U32, m_indices.size());
    }

    //////////////////////////////////////////
    void GizmosDrawer2D::update(F32 _dt)
    {
        ensureLinesEntity();
        rebuildMesh();
    }

} // namespace Maze
//////////////////////////////////////////
