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
#if (!defined(_MazePhysics2DManager_hpp_))
#define _MazePhysics2DManager_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-physics2d/physics/MazePhysicsWorld2D.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Physics2DManager);
    MAZE_USING_SHARED_PTR(PhysicsWorld2D);
    MAZE_USING_SHARED_PTR(PhysicsMaterial2DManager);


    //////////////////////////////////////////
    // Class Physics2DManager
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API Physics2DManager
        : public Updatable
    {
    public:

        //////////////////////////////////////////
        virtual ~Physics2DManager();

        //////////////////////////////////////////
        static void Initialize(
            Physics2DManagerPtr& _physics2DManager,
            PhysicsWorld2DConfig const& _defaultWorldConfig);


        //////////////////////////////////////////
        static inline Physics2DManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline Physics2DManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        PhysicsWorld2DPtr const& getWorld() const { return m_world; }


        //////////////////////////////////////////
        PhysicsMaterial2DManagerPtr const& getPhysicsMaterial2DManager() const { return m_physicsMaterial2DManager; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Physics2DManager();

        //////////////////////////////////////////
        bool init(PhysicsWorld2DConfig const& _defaultWorldConfig);

    protected:
        static Physics2DManager* s_instance;

        PhysicsWorld2DPtr m_world;

        PhysicsMaterial2DManagerPtr m_physicsMaterial2DManager;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePhysics2DManager_hpp_
//////////////////////////////////////////
