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
#if (!defined(_MazeCircleCollider2D_hpp_))
#define _MazeCircleCollider2D_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-physics2d/ecs/components/MazeCollider2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(CircleCollider2D);


    //////////////////////////////////////////
    // Class CircleCollider2D
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API CircleCollider2D
        : public Collider2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(CircleCollider2D, Collider2D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(CircleCollider2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~CircleCollider2D();

        //////////////////////////////////////////
        static CircleCollider2DPtr Create();


        //////////////////////////////////////////
        inline Vec2F const& getOffset() const { return m_offset; }

        //////////////////////////////////////////
        inline void setOffset(Vec2F const& _offset) { m_offset = _offset; }

        //////////////////////////////////////////
        inline F32 getRadius() const { return m_radius; }

        //////////////////////////////////////////
        inline void setRadius(F32 _radius) { m_radius = _radius; }

    protected:

        //////////////////////////////////////////
        CircleCollider2D();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();

    protected:
        Vec2F m_offset;
        F32 m_radius;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCircleCollider2D_hpp_
//////////////////////////////////////////
