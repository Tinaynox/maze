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
#if (!defined(_MazePhysicsWorld2D_hpp_))
#define _MazePhysicsWorld2D_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include <box2d/box2d.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PhysicsWorld2D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Struct PhysicsWorld2DConfig
    //
    //////////////////////////////////////////
    struct MAZE_PHYSICS2D_API PhysicsWorld2DConfig
    {
        F32 unitsPerMeter = 0.1f;
        S32 worldMaxSteps = 10;
        F32 worldStep = 1.0f / 50.0f;
        S32 velocityIterations = 12;
        S32 positionIterations = 16;
        Vec2DF gravityUnits = Vec2DF(0.0f, -3.8f);
    };


    //////////////////////////////////////////
    // Struct PhysicsWorld2DDefaultBodies
    //
    //////////////////////////////////////////
    struct MAZE_PHYSICS2D_API PhysicsWorld2DDefaultBodies
    {
        ////////////////////////////////////
        PhysicsWorld2DDefaultBodies()
            : ground(nullptr)
            , innerWalls(nullptr)
        {

        }

        b2Body* ground;
        b2Body* innerWalls;
    };


    //////////////////////////////////////////
    // Class PhysicsWorld2D
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API PhysicsWorld2D
        : public Updatable
        , public b2ContactListener
    {
    public:

        //////////////////////////////////////////
        virtual ~PhysicsWorld2D();

        //////////////////////////////////////////
        static PhysicsWorld2DPtr Create(PhysicsWorld2DConfig const& _config = PhysicsWorld2DConfig());


        ////////////////////////////////////
        inline b2World* getBox2DWorld() const { return m_world; }

        ////////////////////////////////////
        void setGravity(Vec2DF const& _gravityUnits);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline F32 convertMetersToUnits(F32 _meters) const
        {
            return _meters * m_unitsPerMeter;
        }

        //////////////////////////////////////////
        inline F32 convertUnitsToMeters(F32 _units) const
        {
            return _units / m_unitsPerMeter;
        }

        //////////////////////////////////////////
        inline Vec2DF convertMetersToUnits(Vec2DF const& _meters) const
        {
            return _meters * m_unitsPerMeter;
        }

        //////////////////////////////////////////
        inline Vec2DF convertUnitsToMeters(Vec2DF const& _units) const
        {
            return _units / m_unitsPerMeter;
        }

        //////////////////////////////////////////
        inline AABB2D convertMetersToUnits(AABB2D const& _meters) const
        {
            return AABB2D(
                _meters.getMin() * m_unitsPerMeter,
                _meters.getMax() * m_unitsPerMeter);
        }

        //////////////////////////////////////////
        inline AABB2D convertUnitsToMeters(AABB2D const& _units) const
        {
            return AABB2D(
                _units.getMin() / m_unitsPerMeter,
                _units.getMax() / m_unitsPerMeter);
        }

        //////////////////////////////////////////
        inline Vec2DF convertMetersToUnits(F32 _xWheelMeters, F32 _yWheelMeters)
        {
            return convertMetersToUnits({ _xWheelMeters, _yWheelMeters });
        }

        //////////////////////////////////////////
        inline Vec2DF convertUnitsToMeters(F32 _xWheelUnits, F32 _yWheelUnits)
        {
            return convertUnitsToMeters({ _xWheelUnits, _yWheelUnits });
        }

        //////////////////////////////////////////
        inline F32 getAccumulatorRatio() const { return m_accumulatorRatio; }

    public:
        //////////////////////////////////////////
        MultiDelegate<F32> eventFixedUpdateStart;
        MultiDelegate<F32> eventFixedUpdateFinished;
        MultiDelegate<F32> eventUpdateFinished;
        MultiDelegate<> eventDestroyed;
        MultiDelegate<> eventClear;

    protected:

        //////////////////////////////////////////
        PhysicsWorld2D();

        //////////////////////////////////////////
        bool init(PhysicsWorld2DConfig const& _config);

        //////////////////////////////////////////
        void createWorld();

        //////////////////////////////////////////
        void destroyWorld();

        //////////////////////////////////////////
        void clear();

        ////////////////////////////////////
        void destroyDefaultBodies();

        ////////////////////////////////////
        void createDefaultBodies();


        //////////////////////////////////////////
        void singleStep(F32 _dt);

        //////////////////////////////////////////
        void resetSmoothStates();


        ////////////////////////////////////
        virtual void BeginContact(b2Contact* _contact) MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void PreSolve(b2Contact* _contact, const b2Manifold* _oldManifold) MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void PostSolve(b2Contact* _contact, const b2ContactImpulse* _impulse) MAZE_OVERRIDE;

        ////////////////////////////////////
        virtual void EndContact(b2Contact* _contact) MAZE_OVERRIDE;

    protected:
        b2World* m_world;
        Mutex m_worldMutex;

        F32 m_accumulator;
        F32 m_accumulatorRatio;

        F32 m_unitsPerMeter;
        S32 m_worldMaxSteps;
        F32 m_worldStep;
        S32 m_velocityIterations;
        S32 m_positionIterations;

        Vec2DF m_size;

        PhysicsWorld2DDefaultBodies m_defaultBodies;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysicsWorld2D_hpp_
//////////////////////////////////////////
