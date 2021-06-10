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
#include "GameplaySystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "game/PhysicsBody.hpp"
#include "game/SpaceObject.hpp"
#include "game/AI/SpaceObjectAI_SinMovement.hpp"
#include "game/Projectile.hpp"
#include "game/UnitBounds.hpp"
#include "game/PlayerOwner.hpp"
#include "game/Health.hpp"
#include "game/UnitMeshPartRenderer.hpp"
#include "game/TrailProjectile.hpp"
#include "scenes/SceneGame.hpp"
#include "Game.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class GameplaySystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(GameplaySystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(GameplaySystem);

    //////////////////////////////////////////
    GameplaySystem::GameplaySystem()
        : m_sceneGame(nullptr)
    {
    }

    //////////////////////////////////////////
    GameplaySystem::~GameplaySystem()
    {
        if (m_sceneGame)
            m_sceneGame->eventFixedUpdate.unsubscribe(this);
    }

    //////////////////////////////////////////
    GameplaySystemPtr GameplaySystem::Create(
        ECSWorldPtr const& _world,
        SceneGame* _sceneGame)
    {
        GameplaySystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(GameplaySystem, object, init(_world, _sceneGame));
        return object;
    }

    //////////////////////////////////////////
    bool GameplaySystem::init(
        ECSWorldPtr const& _world,
        SceneGame* _sceneGame)
    {
        m_sceneGame = _sceneGame;

        m_spaceObjects = _world->requestInclusiveSample<SpaceObject>();
        m_spaceObjectAI_SinMovements = _world->requestInclusiveSample<SpaceObjectAI_SinMovement>();
        m_projectiles = _world->requestInclusiveSample<Projectile>();
        m_trailProjectiles = _world->requestInclusiveSample<TrailProjectile>();
        m_unitMeshPartRenderers = _world->requestInclusiveSample<UnitMeshPartRenderer>();

        m_sceneGame->eventFixedUpdate.subscribe(this, &GameplaySystem::processFixedUpdate);

        return true;
    }

    //////////////////////////////////////////
    void GameplaySystem::processUpdate(F32 _dt)
    {
        if (!Game::GetInstancePtr()->getRunning()
            || !Game::GetInstancePtr()->getMainRenderWindow()
            || !Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->isOpened())
            return;

        _dt = Math::Min(_dt, 1.0f / 30.0f);

        m_spaceObjectAI_SinMovements->process(
            [_dt](Entity* _entity, SpaceObjectAI_SinMovement* _aiSinMovement)
            {
                _aiSinMovement->update(_dt);
            });

        m_unitMeshPartRenderers->process(
            [_dt](Entity* _entity, UnitMeshPartRenderer* _unitMeshPartRenderer)
            {
                _unitMeshPartRenderer->update(_dt);
            });

        Rect2DF const& gameplayBounds = m_sceneGame->getGameplayBounds();
        Vec2DF const outOfGameplayBorder(5.0f, 5.0f);

        F32 leftSpaceshipBounds = gameplayBounds.position.x - outOfGameplayBorder.x;
        F32 rightSpaceshipBounds = gameplayBounds.position.x + gameplayBounds.size.x + outOfGameplayBorder.x;
        F32 bottomSpaceshipBounds = gameplayBounds.position.y - outOfGameplayBorder.y;
        F32 topSpaceshipBounds = gameplayBounds.position.y + gameplayBounds.size.y + outOfGameplayBorder.y;

        m_projectiles->process(
            [_dt,
            leftSpaceshipBounds,
            rightSpaceshipBounds,
            bottomSpaceshipBounds,
            topSpaceshipBounds,
            this](
                Entity* _entity,
                Projectile* _projectile)
            {
                _projectile->update(_dt);

                if (_projectile->getTransform()->getLocalPosition().x < leftSpaceshipBounds ||
                    _projectile->getTransform()->getLocalPosition().x > rightSpaceshipBounds ||
                    _projectile->getTransform()->getLocalPosition().y < bottomSpaceshipBounds ||
                    _projectile->getTransform()->getLocalPosition().y > topSpaceshipBounds)
                {
                    _projectile->release();
                    return;
                }

                PlayerIndex projectilePlayerIndex = _projectile->getPlayerOwner()->getCurrentOwner();

                AABB2D projectileBounds;

                bool collided = false;
                m_spaceObjects->process(
                    [_projectile, projectileBounds, &collided, projectilePlayerIndex](
                        Entity* _entity,
                        SpaceObject* _spaceObject)
                    {
                        if (projectilePlayerIndex == _spaceObject->getPlayerOwner()->getCurrentOwner())
                            return;

                        if (collided)
                            return;

                    });

                if (collided)
                {
                    _projectile->release();
                }
            });

        

        m_trailProjectiles->process(
            [_dt](
                Entity* _entity,
                TrailProjectile* _trailProjectile)
            {
                _trailProjectile->update(_dt);
            });


        eventUpdateFinished(_dt);
    }

    //////////////////////////////////////////
    void GameplaySystem::processFixedUpdate(F32 _dt)
    {
        if (    !Game::GetInstancePtr()->getRunning()
            ||    !Game::GetInstancePtr()->getMainRenderWindow()
            ||    !Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->isOpened())
            return;

        Rect2DF const& gameplayBounds = m_sceneGame->getGameplayBounds();
        Vec2DF const outOfGameplayBorder(5.0f, 5.0f);

        F32 leftSpaceshipBounds = gameplayBounds.position.x - outOfGameplayBorder.x;
        // F32 rightSpaceshipBounds = gameplayBounds.position.x + gameplayBounds.size.x + outOfGameplayBorder.x;
        // F32 bottomSpaceshipBounds = gameplayBounds.position.y - outOfGameplayBorder.y;
        // F32 topSpaceshipBounds = gameplayBounds.position.y + gameplayBounds.size.y + outOfGameplayBorder.y;

        m_spaceObjects->process(
            [_dt,
            leftSpaceshipBounds,
            this](Entity* _entity, SpaceObject* _spaceObject)
            {
                // PhysicsBodyPtr const& physicsBody = _spaceObject->getPhysicsBody();
                // physicsBody->addForce(_spaceObject->getEngineForce() * _spaceObject->getThrottle());

                if (_spaceObject->getHealth()->isAlive())
                    _spaceObject->getRigidbody2D()->applyForce(_spaceObject->getEngineForce() * _spaceObject->getThrottle());

                _spaceObject->update(_dt);

                if (m_sceneGame->getPlayerController()->getPlayerSpaceObject().get() != _spaceObject)
                {
                    if (_spaceObject->getTransform()->getLocalPosition().x < leftSpaceshipBounds)
                        _spaceObject->release();
                }
            });
    }
    
    
} // namespace Maze
//////////////////////////////////////////
