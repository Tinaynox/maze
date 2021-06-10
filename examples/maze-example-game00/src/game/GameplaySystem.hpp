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
#if (!defined(_GameplaySystem_hpp_))
#define _GameplaySystem_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(GameplaySystem);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(Gameplay);
    MAZE_USING_SHARED_PTR(SpaceObject);
    MAZE_USING_SHARED_PTR(SpaceObjectAI_SinMovement);
    MAZE_USING_SHARED_PTR(Projectile);
    MAZE_USING_SHARED_PTR(SceneGame);
    MAZE_USING_SHARED_PTR(UnitMeshPartRenderer);
    MAZE_USING_SHARED_PTR(TrailProjectile);


    //////////////////////////////////////////
    // Class GameplaySystem
    //
    //////////////////////////////////////////
    class MAZE_CORE_API GameplaySystem
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(GameplaySystem, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(GameplaySystem);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~GameplaySystem();

        //////////////////////////////////////////
        static GameplaySystemPtr Create(
            ECSWorldPtr const& _world,
            SceneGame* _sceneGame);


        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return -100; }

    public:

        //////////////////////////////////////////
        MultiDelegate<F32> eventUpdateFinished;

    protected:

        //////////////////////////////////////////
        GameplaySystem();

        //////////////////////////////////////////
        bool init(
            ECSWorldPtr const& _world,
            SceneGame* _sceneGame);

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processFixedUpdate(F32 _dt);

    protected:
        SceneGame* m_sceneGame;

        SharedPtr<GenericInclusiveEntitiesSample<SpaceObject>> m_spaceObjects;
        SharedPtr<GenericInclusiveEntitiesSample<SpaceObjectAI_SinMovement>> m_spaceObjectAI_SinMovements;
        SharedPtr<GenericInclusiveEntitiesSample<Projectile>> m_projectiles;
        SharedPtr<GenericInclusiveEntitiesSample<TrailProjectile>> m_trailProjectiles;
        SharedPtr<GenericInclusiveEntitiesSample<UnitMeshPartRenderer>> m_unitMeshPartRenderers;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _GameplaySystem_hpp_
//////////////////////////////////////////
