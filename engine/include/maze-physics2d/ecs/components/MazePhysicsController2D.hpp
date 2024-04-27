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
#if (!defined(_MazePhysicsController2D_hpp_))
#define _MazePhysicsController2D_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(PhysicsController2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(PhysicsWorld2D);
    MAZE_USING_SHARED_PTR(PhysicsRotor2D);
            

    //////////////////////////////////////////
    // Class PhysicsController2D
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PhysicsController2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PhysicsController2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PhysicsController2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~PhysicsController2D();

        //////////////////////////////////////////
        static PhysicsController2DPtr Create();


    protected:

        //////////////////////////////////////////
        PhysicsController2D();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void setPhysicsWorld2D(PhysicsWorld2DPtr const& _physicsWorld2D);

        //////////////////////////////////////////
        void notifyFixedUpdateStart(F32 _dt);

        //////////////////////////////////////////
        void notifyFixedUpdateFinished(F32 _dt);

        //////////////////////////////////////////
        void notifyUpdateFinished(F32 _dt);

    protected:
        PhysicsWorld2DPtr m_physicsWorld2D;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysicsController2D_hpp_
//////////////////////////////////////////
