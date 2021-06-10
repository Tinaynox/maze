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
#if (!defined(_MazePhysicsEvents_hpp_))
#define _MazePhysicsEvents_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include <box2d/box2d.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(Collider2D);


    //////////////////////////////////////////
    // Class AllowContactEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API AllowContactEvent
        : public GenericEvent<AllowContactEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AllowContactEvent, Event);


        //////////////////////////////////////////
        AllowContactEvent() = default;

        //////////////////////////////////////////
        AllowContactEvent(
            Rigidbody2D* _myRigidbody2D,
            Rigidbody2D* _contactRigidbody2D,
            Collider2D* _myCollider2D,
            Collider2D* _contactCollider2D)
            : myRigidbody2D(_myRigidbody2D)
            , contactRigidbody2D(_contactRigidbody2D)
            , myCollider2D(_myCollider2D)
            , contactCollider2D(_contactCollider2D)
            , ignore(false)
        {
            
        }

    public:

        Rigidbody2D* myRigidbody2D;
        Rigidbody2D* contactRigidbody2D;
        Collider2D* myCollider2D;
        Collider2D* contactCollider2D;
        mutable bool ignore;
    };


    //////////////////////////////////////////
    // Class BeginContactEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API BeginContactEvent
        : public GenericEvent<BeginContactEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(BeginContactEvent, Event);


        //////////////////////////////////////////
        BeginContactEvent() = default;

        //////////////////////////////////////////
        BeginContactEvent(
            Rigidbody2D* _myRigidbody2D,
            Rigidbody2D* _contactRigidbody2D,
            Collider2D* _myCollider2D,
            Collider2D* _contactCollider2D,
            Vec2DF const& _contactPower = Vec2DF::c_zero)
            : myRigidbody2D(_myRigidbody2D)
            , contactRigidbody2D(_contactRigidbody2D)
            , myCollider2D(_myCollider2D)
            , contactCollider2D(_contactCollider2D)
            , contactPower(_contactPower)
        {
            
        }

    public:

        Rigidbody2D* myRigidbody2D;
        Rigidbody2D* contactRigidbody2D;
        Collider2D* myCollider2D;
        Collider2D* contactCollider2D;
        Vec2DF contactPower;
    };


    //////////////////////////////////////////
    // Class EndContactEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API EndContactEvent
        : public GenericEvent<EndContactEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EndContactEvent, Event);

        //////////////////////////////////////////
        EndContactEvent() = default;

        //////////////////////////////////////////
        EndContactEvent(
            Rigidbody2D* _myRigidbody2D,
            Rigidbody2D* _contactRigidbody2D,
            Collider2D* _myCollider2D,
            Collider2D* _contactCollider2D)
            : myRigidbody2D(_myRigidbody2D)
            , contactRigidbody2D(_contactRigidbody2D)
            , myCollider2D(_myCollider2D)
            , contactCollider2D(_contactCollider2D)
        {
            
        }

    public:

        Rigidbody2D* myRigidbody2D;
        Rigidbody2D* contactRigidbody2D;
        Collider2D* myCollider2D;
        Collider2D* contactCollider2D;
    };


    //////////////////////////////////////////
    // Class PreSolveContactEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PreSolveContactEvent
        : public GenericEvent<PreSolveContactEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PreSolveContactEvent, Event);

        //////////////////////////////////////////
        PreSolveContactEvent() = default;

        //////////////////////////////////////////
        PreSolveContactEvent(
            Rigidbody2D* _myRigidbody2D,
            Rigidbody2D* _contactRigidbody2D,
            Collider2D* _myCollider2D,
            Collider2D* _contactCollider2D)
            : myRigidbody2D(_myRigidbody2D)
            , contactRigidbody2D(_contactRigidbody2D)
            , myCollider2D(_myCollider2D)
            , contactCollider2D(_contactCollider2D)
        {
            
        }

    public:

        Rigidbody2D* myRigidbody2D;
        Rigidbody2D* contactRigidbody2D;
        Collider2D* myCollider2D;
        Collider2D* contactCollider2D;
    };


    //////////////////////////////////////////
    // Class PostSolveContactEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PostSolveContactEvent
        : public GenericEvent<PostSolveContactEvent>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PostSolveContactEvent, Event);

        //////////////////////////////////////////
        PostSolveContactEvent() = default;

        //////////////////////////////////////////
        PostSolveContactEvent(
            Rigidbody2D* _myRigidbody2D,
            Rigidbody2D* _contactRigidbody2D,
            Collider2D* _myCollider2D,
            Collider2D* _contactCollider2D,
            F32 _impulse = 0.0f)
            : myRigidbody2D(_myRigidbody2D)
            , contactRigidbody2D(_contactRigidbody2D)
            , myCollider2D(_myCollider2D)
            , contactCollider2D(_contactCollider2D)
            , impulse(_impulse)
        {
            
        }

    public:

        Rigidbody2D* myRigidbody2D;
        Rigidbody2D* contactRigidbody2D;
        Collider2D* myCollider2D;
        Collider2D* contactCollider2D;
        F32 impulse;
    };

    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysicsEvents_hpp_
//////////////////////////////////////////
