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
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
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
    class MAZE_EDITOR_TOOLS_API GizmosDrawer
    {
    public:

        //////////////////////////////////////////
        enum class MeshRenderMode
        {
            Opaque = 0,
            Transparent = 1,
            TransparentTop = 2,

            MAX
        };

        //////////////////////////////////////////
        struct BufferedLine
        {
            F32 timer;
            Vec3DF point0;
            Vec3DF point1;
            ColorF128 color;
            MeshRenderMode renderMode;
        };

        //////////////////////////////////////////
        struct BufferedTriangle
        {
            F32 timer;
            Vec3DF point0;
            Vec3DF point1;
            Vec3DF point2;
            ColorF128 color;
            MeshRenderMode renderMode;
        };

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
            Vec3DF const& _point1,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent);

        //////////////////////////////////////////
        void drawLine(
            Vec3DF const& _point0,
            Vec3DF const& _point1,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent);

        //////////////////////////////////////////
        inline void drawLine(
            F32 _x0,
            F32 _y0,
            F32 _x1,
            F32 _y1,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine({ _x0, _y0, 0.0f }, { _x1, _y1, 0.0f }, _duration, _renderMode);
        }

        //////////////////////////////////////////
        inline void drawLine(
            F32 _x0,
            F32 _y0,
            F32 _x1,
            F32 _y1,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine(_x0, _y0, _x1, _y1, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        inline void drawLine(
            Vec2DF const& _point0,
            Vec2DF const& _point1,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine({ _point0.x, _point0.y, 0.0f }, { _point1.x, _point1.y, 0.0f }, _duration, _renderMode);
        }

        //////////////////////////////////////////
        inline void drawLine(
            Vec2DF const& _point0,
            Vec2DF const& _point1,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine(_point0, _point1, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        inline void drawLine(
            F32 _x0,
            F32 _y0,
            F32 _z0,
            F32 _x1,
            F32 _y1,
            F32 _z1,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine({ _x0, _y0, _z0 }, { _x1, _y1, _z1 }, _duration, _renderMode);
        }

        //////////////////////////////////////////
        inline void drawLine(
            F32 _x0,
            F32 _y0,
            F32 _z0,
            F32 _x1,
            F32 _y1,
            F32 _z1,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine(_x0, _y0, _z0, _x1, _y1, _z1, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        inline void drawRay(
            Vec3DF const& _point,
            Vec3DF const& _delta,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine(_point, _point + _delta, _duration, _renderMode);
        }

        //////////////////////////////////////////
        inline void drawRay(
            Vec3DF const& _point,
            Vec3DF const& _delta,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawLine(_point, _delta, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireQuad(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            Vec2DF const& _size,
            ColorF128 const& _color,
            F32 _duration = 0.0f,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawWireCircle(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent);

        //////////////////////////////////////////
        inline void drawWireCircle(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawWireCircle(_position, _direction, _radius, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireHemicircle(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            F32 _radius,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent);

        //////////////////////////////////////////
        inline void drawWireHemicircle(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            F32 _radius,
            MeshRenderMode _renderMode = MeshRenderMode::Transparent)
        {
            drawWireHemicircle(_position, _forward, _up, _radius, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireCube(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            Vec3DF const& _scale,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawWireCube(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            Vec3DF const& _scale,
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireCube(_position, _forward, _up, _scale, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireSphere(
            Vec3DF const& _position,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireSphere(
            Vec3DF const& _position,
            F32 _radius = 1.0f,
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireSphere(_position, _radius, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireHemisphere(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireHemisphere(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius = 1.0f,
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireHemisphere(_position, _direction, _radius, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireCylinder(
            Vec3DF const& _start,
            Vec3DF const& _end,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireCylinder(
            Vec3DF const& _start,
            Vec3DF const& _end,
            F32 _radius = 1.0f,
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireCylinder(_start, _end, _radius, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireCone(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _angle,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireCone(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _angle = Math::DegreesToRadians(45.0f),
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireCone(_position, _direction, _angle, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireTruncatedCone(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            F32 _angle,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireTruncatedCone(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius = 1.0f,
            F32 _angle = Math::DegreesToRadians(45.0f),
            ColorF128 const& _color = ColorF128::c_white,            
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireTruncatedCone(_position, _direction, _radius, _angle, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireTorus(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius,
            F32 _torusRadius,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireTorus(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            F32 _radius = 1.0f,
            F32 _torusRadius = 0.2f,
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireTorus(_position, _direction, _radius, _torusRadius, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireArrow(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireArrow(
            Vec3DF const& _position,
            Vec3DF const& _direction,
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireArrow(_position, _direction, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawWireCapsule(
            Vec3DF const& _start,
            Vec3DF const& _end,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawWireCapsule(
            Vec3DF const& _start,
            Vec3DF const& _end,
            F32 _radius = 1.0f,
            ColorF128 const& _color = ColorF128::c_white,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawWireCapsule(_start, _end, _radius, _color, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawAABB(
            AABB2D const& _aabb,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        inline void drawAABB(
            AABB2D const& _aabb,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawAABB(_aabb, -1.0f, _renderMode);
        }

        //////////////////////////////////////////
        void drawAABB(
            AABB3D const& _aabb,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawAABB(
            AABB3D const& _aabb,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque)
        {
            drawAABB(_aabb, -1.0f, _renderMode);
        }


        //////////////////////////////////////////
        void drawTriangle(
            Vec3DF const& _point0,
            Vec3DF const& _point1,
            Vec3DF const& _point2,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);


        //////////////////////////////////////////
        void drawTriangle(
            Vec3DF const& _point0,
            Vec3DF const& _point1,
            Vec3DF const& _point2,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawMesh(
            MeshPtr const& _mesh,
            Vec3DF const& _position,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawQuad(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            Vec2DF const& _scale,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawCube(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            Vec3DF const& _scale,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawSphere(
            Vec3DF const& _position,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawCone(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawCylinder(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            Vec3DF const& _up,
            F32 _radius,
            F32 _height,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);

        //////////////////////////////////////////
        void drawTorus(
            Vec3DF const& _position,
            Vec3DF const& _forward,
            F32 _radius,
            ColorF128 const& _color,
            F32 _duration,
            MeshRenderMode _renderMode = MeshRenderMode::Opaque);



        //////////////////////////////////////////
        void drawBillboard(
            Vec3DF const& _point,
            SpritePtr const& _sprite,
            ColorF128 const& _color);


        //////////////////////////////////////////
        inline void setColor(ColorF128 const& _color) { m_color = _color; }

        //////////////////////////////////////////
        inline ColorF128 const& getColor() const { return m_color; }

        //////////////////////////////////////////
        void pushTransform(Mat4DF const& _tm);

        //////////////////////////////////////////
        void popTransform();


        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        void rebuildMeshes();

    protected:

        //////////////////////////////////////////
        GizmosDrawer();

        //////////////////////////////////////////
        bool init(ECSWorld* _world, RenderTarget* _renderTarget);

        //////////////////////////////////////////
        Vec3DF transformPoint(Vec3DF const& _p);

    protected:
        ColorF128 m_color;
        Stack<Mat4DF> m_transformStack;

        ECSWorld* m_world;

        MaterialPtr m_materials[(Size)MeshRenderMode::MAX];
        MeshData m_lines[(Size)MeshRenderMode::MAX];
        MeshData m_triangles[(Size)MeshRenderMode::MAX];

        List<BufferedLine> m_bufferedLines;
        List<BufferedTriangle> m_bufferedTriangles;

        UniquePtr<SharedObjectPool<GizmoBillboard3D>> m_gizmoBillboardPool;
        Vector<BillboardData> m_billboardsData;
        Vector<GizmoBillboard3DPtr> m_billboards;
    };


} // namespace Maze
//////////////////////////////////////////
 

#endif // _MazeGizmosDrawer_hpp_
//////////////////////////////////////////
