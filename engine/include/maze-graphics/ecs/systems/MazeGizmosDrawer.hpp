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
#if (!defined(_MazeGizmosDrawer_hpp_))
#define _MazeGizmosDrawer_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-graphics/ecs/components/MazeGizmoBillboard3D.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/utils/MazeSharedObjectPool.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GizmosDrawer);
    MAZE_USING_SHARED_PTR(ECSWorld);
    MAZE_USING_SHARED_PTR(VertexArrayObject);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Sprite);


    //////////////////////////////////////////
    // Class GizmosDrawer
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API GizmosDrawer
    {
    public:

        //////////////////////////////////////////
        struct MeshData
        {
            EntityPtr entity;
            VertexArrayObjectPtr vao;
            RenderMeshPtr renderMesh;
            FastVector<Vec3DF> vertices;
            FastVector<Vec4DF> colors;
            FastVector<U32> indices;
        };

        //////////////////////////////////////////
        struct BillboardData
        {
            Vec3DF point;
            SpritePtr sprite;
            ColorF128 color;
        };

    public:

        //////////////////////////////////////////
        virtual ~GizmosDrawer();

        //////////////////////////////////////////
        static GizmosDrawerPtr Create(ECSWorld* _world, RenderTarget* _renderTarget);


        //////////////////////////////////////////
        void clear();

        //////////////////////////////////////////
        void drawLine(
            Vec3DF const& _point0,
            Vec3DF const& _point1);

        //////////////////////////////////////////
        inline void drawLine(
            F32 _x0,
            F32 _y0,
            F32 _x1,
            F32 _y1)
        {
            drawLine({ _x0, _y0, 0.0f }, { _x1, _y1, 0.0f });
        }

        //////////////////////////////////////////
        inline void drawLine(
            Vec2DF const& _point0,
            Vec2DF const& _point1)
        {
            drawLine({ _point0.x, _point0.y, 0.0f }, { _point1.x, _point1.y, 0.0f });
        }

        //////////////////////////////////////////
        inline void drawLine(
            F32 _x0,
            F32 _y0,
            F32 _z0,
            F32 _x1,
            F32 _y1,
            F32 _z1)
        {
            drawLine({ _x0, _y0, _z0 }, { _x1, _y1, _z1 });
        }

        //////////////////////////////////////////
        void drawCircle(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            S32 _segmentsCount = 12);

        //////////////////////////////////////////
        void drawBillboard(
            Vec3DF const& _point,
            SpritePtr const& _sprite,
            ColorF128 const& _color);

        //////////////////////////////////////////
        void drawAABB(AABB2D const& _aabb);

        //////////////////////////////////////////
        void drawAABB(AABB3D const& _aabb);


        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color) { m_color = _color; }

        //////////////////////////////////////////
        inline ColorF128 const& getColor() const { return m_color; }


        //////////////////////////////////////////
        void rebuildMeshes();

    protected:

        //////////////////////////////////////////
        GizmosDrawer();

        //////////////////////////////////////////
        bool init(ECSWorld* _world, RenderTarget* _renderTarget);

    protected:
        ColorF128 m_color;

        ECSWorld* m_world;

        MeshData m_lines;
        MeshData m_triangles;

        UniquePtr<SharedObjectPool<GizmoBillboard3D>> m_gizmoBillboardPool;
        Vector<BillboardData> m_billboardsData;
        Vector<GizmoBillboard3DPtr> m_billboards;
    };


} // namespace Maze
//////////////////////////////////////////
 

#endif // _MazeGizmosDrawer_hpp_
//////////////////////////////////////////
