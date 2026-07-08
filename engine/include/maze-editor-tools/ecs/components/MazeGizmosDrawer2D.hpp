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
#pragma once
#if (!defined(_MazeGizmosDrawer2D_hpp_))
#define _MazeGizmosDrawer2D_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-graphics/MazeColorF128.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmosDrawer2D);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(VertexArrayObject);
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_MANAGED_SHARED_PTR(Material);


    //////////////////////////////////////////
    // Class GizmosDrawer2D
    //
    // Draws debug gizmos for 2D (Transform2D/Canvas) entities.
    //
    // Unlike GizmosDrawer, which renders 3D wireframe meshes through the
    // scene-view Camera3D, Canvas content is rendered through its own
    // pixel-space pipeline that is entirely independent of any camera.
    // To stay pixel-aligned with the actual UI regardless of the scene-view
    // camera setup, this drawer batches thin RenderDrawTopology::Lines
    // geometry into a single MeshRenderer parented under the editor's own
    // overlay canvas (GizmosManager::getCanvas()), which shares the same
    // render target and viewport as the edited canvas -- so canvas-local
    // pixel coordinates line up directly with zero translation needed.
    //
    // Lines (not thin triangle quads) are used deliberately -- quad-based
    // strokes glitch when they degenerate to near-zero-area triangles.
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API GizmosDrawer2D
    {
    public:

        //////////////////////////////////////////
        virtual ~GizmosDrawer2D();

        //////////////////////////////////////////
        static GizmosDrawer2DPtr Create();


        //////////////////////////////////////////
        void clear();

        //////////////////////////////////////////
        void destroy();


        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color) { m_color = _color; }

        //////////////////////////////////////////
        inline ColorF128 const& getColor() const { return m_color; }


        //////////////////////////////////////////
        void drawLine(
            Vec2F const& _point0,
            Vec2F const& _point1);

        //////////////////////////////////////////
        void drawPolyline(
            Vector<Vec2F> const& _points,
            bool _closed);

        //////////////////////////////////////////
        void drawWireRect(
            Vec2F const _corners[4]);

        //////////////////////////////////////////
        inline void drawWireRect(
            AABB2D const& _aabb)
        {
            Vec2F corners[4] =
            {
                Vec2F(_aabb.getMin().x, _aabb.getMin().y),
                Vec2F(_aabb.getMax().x, _aabb.getMin().y),
                Vec2F(_aabb.getMax().x, _aabb.getMax().y),
                Vec2F(_aabb.getMin().x, _aabb.getMax().y)
            };
            drawWireRect(corners);
        }

        //////////////////////////////////////////
        void drawCross(
            Vec2F const& _center,
            F32 _size);

        //////////////////////////////////////////
        void drawCircle(
            Vec2F const& _center,
            F32 _radius,
            S32 _segments = 24);


        //////////////////////////////////////////
        void update(F32 _dt);

    protected:

        //////////////////////////////////////////
        GizmosDrawer2D();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void ensureLinesEntity();

        //////////////////////////////////////////
        void destroyLinesEntity();

        //////////////////////////////////////////
        void rebuildMesh();

    protected:
        ColorF128 m_color = ColorF128::c_white;

        void* m_canvasKey = nullptr;

        EntityPtr m_linesEntity;
        Transform2DPtr m_linesTransform;
        MeshRendererPtr m_linesMeshRenderer;
        VertexArrayObjectPtr m_linesVao;
        RenderMeshPtr m_linesRenderMesh;

        FastVector<Vec3F> m_vertices;
        FastVector<Vec4F> m_colors;
        FastVector<U32> m_indices;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGizmosDrawer2D_hpp_
//////////////////////////////////////////
