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
#if (!defined(_MazePhysicsRotor2D_hpp_))
#define _MazePhysicsRotor2D_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PhysicsRotor2D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);


    //////////////////////////////////////////
    // Class PhysicsRotor2D
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PhysicsRotor2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PhysicsRotor2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PhysicsRotor2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~PhysicsRotor2D();

        //////////////////////////////////////////
        static PhysicsRotor2DPtr Create(F32 _speed = 5.0f);


        //////////////////////////////////////////
        void update(F32 _dt);

        //////////////////////////////////////////
        inline F32 getSpeed() const { return m_speed; }

        //////////////////////////////////////////
        inline void setSpeed(F32 _speed) { m_speed = _speed; }

    protected:

        //////////////////////////////////////////
        PhysicsRotor2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(F32 _speed = 5.0f);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        F32 m_speed;

        Rigidbody2DPtr m_rigidbody;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysicsRotor2D_hpp_
//////////////////////////////////////////
