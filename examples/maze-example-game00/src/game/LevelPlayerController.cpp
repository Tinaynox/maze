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
#include "LevelPlayerController.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/helpers/MazeColorHelper.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "scenes/SceneGame.hpp"
#include "game/SpaceObject.hpp"
#include "game/PlayerOwner.hpp"
#include "game/UnitMeshPartRenderer.hpp"
#include "game/PhysicsBody.hpp"
#include "game/Health.hpp"
#include "input/PlayerGamepad.hpp"
#include "managers/PlayerManager.hpp"
#include "managers/UnitManager.hpp"
#include "player/Player.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "game/LevelAdapter.hpp"
#include "game/SpaceObjectPool.hpp"
#include "settings/GameDebugSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static F32 const c_shotValueMax = 1.5f;


    //////////////////////////////////////////
    // Class LevelPlayerController
    //
    //////////////////////////////////////////
    LevelPlayerController::LevelPlayerController()
        : m_sceneGame(nullptr)
        , m_shotValue(0.0f)
    {

    }

    //////////////////////////////////////////
    LevelPlayerController::~LevelPlayerController()
    {
        if (m_spaceObject)
        {
            m_spaceObject->eventProjectileSpawned.unsubscribe(this);
        }

        m_sceneGame->eventPostGameplayUpdate.unsubscribe(this);
        m_sceneGame->eventFixedUpdate.unsubscribe(this);
    }

    //////////////////////////////////////////
    LevelPlayerControllerPtr LevelPlayerController::Create(SceneGame* _sceneGame)
    {
        LevelPlayerControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(LevelPlayerController, object, init(_sceneGame));
        return object;
    }

    //////////////////////////////////////////
    bool LevelPlayerController::init(SceneGame* _sceneGame)
    {
        m_sceneGame = _sceneGame;
        
        RenderSystem* renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();
        

        MaterialPtr spaceshipMaterial = renderSystem->getMaterialManager()->getMaterial("PlayerShip00.mzmaterial");        
        m_shotValueUniform = spaceshipMaterial->getFirstRenderPass()->getShader()->ensureUniform("u_shotValue");


        SpaceObjectAvatarType playerAvatar = SpaceObjectAvatarType::PlayerShip000;

        GameDebugSettings* debugSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<GameDebugSettings>();
        if (debugSettings)
        {
            if (debugSettings->getForcePlayerAvatar() != SpaceObjectAvatarType::None)
            {
                playerAvatar = debugSettings->getForcePlayerAvatar();
            }
        }

        m_spaceObject = m_sceneGame->getLevelAdapter()->getSpaceObjectPool()->createSpaceObject(playerAvatar);
        m_spaceObject->eventProjectileSpawned.subscribe(this, &LevelPlayerController::notifyProjectileSpawned);
        m_spaceObject->getEntityRaw()->ensureComponent<PlayerOwner>(PlayerIndex::Player0);            
        m_spaceObject->setDirection(SpaceObjectAvatarDirection::Right);        
        m_spaceObject->setCollisionDamage(0.0f);
        

        _sceneGame->eventPostGameplayUpdate.subscribe(this, &LevelPlayerController::notifyPostGameplayUpdate);
        _sceneGame->eventFixedUpdate.subscribe(this, &LevelPlayerController::notifyFixedUpdate);

        return true;
    }

    //////////////////////////////////////////
    void LevelPlayerController::update(F32 _dt)
    {
        PlayerGamepadPtr const&  playerGamepad = PlayerManager::GetInstancePtr()->getPlayer(0)->getPlayerGamepad();
        Vec2DF moveAxis = playerGamepad->getMoveAxis();

        if (m_spaceObject)
        {
            m_spaceObject->setThrottle(moveAxis.normalizedCopy());

            if (m_spaceObject->getHealth()->isAlive())
            {
                m_spaceObject->setFire(playerGamepad->getFireButton() || true);
            }
            else
            {
                m_spaceObject->setFire(false);
            }
        }
        

        if (m_shotValue > 0.0f)
        {
            m_shotValue -= _dt * 7.5f * c_shotValueMax;
            if (m_shotValue < 0.0f)
                m_shotValue = 0.0f;

            if (m_shotValueUniform)
                m_shotValueUniform->set(m_shotValue);
        }
    }

    //////////////////////////////////////////
    void LevelPlayerController::loadFromXMLElement(tinyxml2::XMLElement const* _element)
    {
        tinyxml2::XMLNode const* node = _element->FirstChild();
        while (node)
        {
            tinyxml2::XMLElement const* childElement = node->ToElement();
            if (!childElement)
            {
                node = node->NextSibling();
                continue;
            }


            node = node->NextSibling();
        }
    }

    //////////////////////////////////////////
    void LevelPlayerController::notifyPostGameplayUpdate(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void LevelPlayerController::notifyFixedUpdate(F32 _dt)
    {
        Vec2DF const spaceshipBorder(1.0f, 0.5f);

        Rect2DF const& gameplayBounds = m_sceneGame->getGameplayBounds();

        F32 leftSpaceshipBounds = gameplayBounds.position.x + spaceshipBorder.x;
        F32 rightSpaceshipBounds = gameplayBounds.position.x + gameplayBounds.size.x - spaceshipBorder.x;
        F32 bottomSpaceshipBounds = gameplayBounds.position.y + spaceshipBorder.y;
        F32 topSpaceshipBounds = gameplayBounds.position.y + gameplayBounds.size.y - spaceshipBorder.y;

        if (m_spaceObject)
        {
            if (m_spaceObject->getRigidbody2D()->getPosition().x > rightSpaceshipBounds)
            {
                m_spaceObject->getRigidbody2D()->setPositionX(rightSpaceshipBounds);
                m_spaceObject->getRigidbody2D()->setVelocity(
                    {
                        0.0f,
                        m_spaceObject->getRigidbody2D()->getVelocity().y
                    });
            }
            else
            if (m_spaceObject->getRigidbody2D()->getPosition().x < leftSpaceshipBounds)
            {
                m_spaceObject->getRigidbody2D()->setPositionX(leftSpaceshipBounds);
                m_spaceObject->getRigidbody2D()->setVelocity(
                    {
                        0.0f,
                        m_spaceObject->getRigidbody2D()->getVelocity().y
                    });
            }

            if (m_spaceObject->getRigidbody2D()->getPosition().y > topSpaceshipBounds)
            {
                m_spaceObject->getRigidbody2D()->setPositionY(topSpaceshipBounds);
                m_spaceObject->getRigidbody2D()->setVelocity(
                    {
                        m_spaceObject->getRigidbody2D()->getVelocity().x,
                        0.0f
                    });
            }
            else
            if (m_spaceObject->getRigidbody2D()->getPosition().y < bottomSpaceshipBounds)
            {
                m_spaceObject->getRigidbody2D()->setPositionY(bottomSpaceshipBounds);
                m_spaceObject->getRigidbody2D()->setVelocity(
                    {
                        m_spaceObject->getRigidbody2D()->getVelocity().x,
                        0.0f
                    });
            }
        }
    }

    //////////////////////////////////////////
    void LevelPlayerController::notifyProjectileSpawned(ProjectilePtr const& _projectile)
    {
        m_shotValue = c_shotValueMax;

        if (m_shotValueUniform)
            m_shotValueUniform->set(m_shotValue);
    }

} // namespace Maze
//////////////////////////////////////////
