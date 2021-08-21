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
#include "SceneGame.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/helpers/MazeColorHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-ui/ecs/components/MazeButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-physics2d/managers/MazePhysics2DManager.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneTest00.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "ui/VirtualJoystickController.hpp"
#include "game/Health.hpp"
#include "game/SpaceObject.hpp"
#include "game/GameplaySystem.hpp"
#include "game/PlayerOwner.hpp"
#include "game/Projectile.hpp"
#include "game/LevelAdapter.hpp"
#include "game/ProjectilePool.hpp"
#include "game/SpaceObjectPool.hpp"
#include "game/SpecialEffectPool.hpp"
#include "game/UnitMeshPartRenderer.hpp"
#include "game/level/LevelSpawnDataUnit.hpp"
#include "managers/PlayerManager.hpp"
#include "managers/GameManager.hpp"
#include "input/PlayerGamepad.hpp"
#include "player/Player.hpp"
#include "settings/GameGraphicsSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneGame
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneGame, ECSRenderScene);

    //////////////////////////////////////////
    SceneGame::SceneGame()
        : m_gameTimer(0.0f)
    {
        
    }

    //////////////////////////////////////////
    SceneGame::~SceneGame()
    {
        if (Game::GetInstancePtr())
        {
            Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
            Game::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.unsubscribe(this);
            Game::GetInstancePtr()->getPhysics2DManager()->getWorld()->eventFixedUpdateFinished.unsubscribe(this);
        }

        if (m_gameplaySystem)
        {
            ECSWorldPtr const& world = EntityManager::GetInstancePtr()->getDefaultWorld();

            removeSystem(m_gameplaySystem);

            m_gameplaySystem->eventUpdateFinished.unsubscribe(this);
            m_gameplaySystem.reset();
        }

        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SceneGamePtr SceneGame::Create()
    {
        SceneGamePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneGame, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneGame::init()
    {
        RenderWindowPtr const& renderWindow = Game::GetInstancePtr()->getMainRenderWindow();
        if (!ECSRenderScene::init(renderWindow))
            return false;

        SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->setPostProcessEnabled(true);

        Game::GetInstancePtr()->getMainRenderWindow()->eventRenderTargetResized.subscribe(this, &SceneGame::notifyRenderTargetResized);
        Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneGame::notifyMainRenderWindowViewportChanged);
        Game::GetInstancePtr()->getPhysics2DManager()->getWorld()->eventFixedUpdateFinished.subscribe(this, &SceneGame::notifyFixedUpdateFinished);

        InputManager* inputManager = InputManager::GetInstancePtr();
        inputManager->eventMouse.subscribe(this, &SceneGame::notifyMouse);


        createControllers();
        create2D();
        create3D();
        createSystems();

        m_spawnController = LevelSpawnController::Create(this);
        m_environmentController = LevelEnvironmentController::Create(this);
        m_playerController = LevelPlayerController::Create(this);

        if (SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->getPostProcessEnabled())
        {
            m_bloomController = LevelBloomController::Create(this);
        }


        loadLevel("Level00.xml");

        return true;
    }

    //////////////////////////////////////////
    void SceneGame::update(F32 _dt)
    {
        if (   !Game::GetInstancePtr()->getRunning() 
            || !Game::GetInstancePtr()->getMainRenderWindow()
            || !Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->isOpened())
            return;

        _dt = Math::Min(_dt, 1.0f/30.0f);

        m_gameTimer += _dt;

        m_spawnController->update(_dt);

        if (m_playerController->getPlayerSpaceObject())
        {
            if (!m_playerController->getPlayerSpaceObject()->getHealth()->isAlive())
            {
                if (!SceneManager::GetInstancePtr()->getScene<SceneFadePreloader>())
                {
                    ECSSceneWPtr sceneWeak = getSharedPtr();

                    SceneFadePreloaderPtr sceneFadePreloader = SceneManager::GetInstancePtr()->loadScene<SceneFadePreloader>();
                    SceneFadePreloaderWPtr sceneFadePreloaderWeak = sceneFadePreloader;
                    sceneFadePreloader->eventFade.subscribe(
                        [sceneFadePreloaderWeak, sceneWeak, this]()
                        {
                            SceneManager::GetInstancePtr()->loadScene<SceneMainMenu>();

                            ECSScenePtr scene = sceneWeak.lock();
                            if (scene)
                            {
                                SceneManager::GetInstancePtr()->destroyScene(scene);
                            }

                            sceneFadePreloaderWeak.lock()->fadeOut();
                        });
                }

                return;
            }
        }

        if (m_spawnController->isSpawnDataEmpty())
        {
            if (m_spaceObjectPool->getCountActive() == 1) 
            {
                if (!SceneManager::GetInstancePtr()->getScene<SceneFadePreloader>())
                {
                    ECSSceneWPtr sceneWeak = getSharedPtr();

                    SceneFadePreloaderPtr sceneFadePreloader =  SceneManager::GetInstancePtr()->loadScene<SceneFadePreloader>();
                    SceneFadePreloaderWPtr sceneFadePreloaderWeak = sceneFadePreloader;
                    sceneFadePreloader->eventFade.subscribe(
                        [sceneFadePreloaderWeak, sceneWeak, this]()
                        {
                            SceneManager::GetInstancePtr()->loadScene<SceneMainMenu>();

                            ECSScenePtr scene = sceneWeak.lock();
                            if (scene)
                            {
                                SceneManager::GetInstancePtr()->destroyScene(scene);
                            }

                            sceneFadePreloaderWeak.lock()->fadeOut();
                        });
                }
                return;
            }
        }

        m_environmentController->update(_dt);
        m_playerController->update(_dt);

        if (SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->getPostProcessEnabled())
        {
            m_bloomController->update(_dt);

            m_renderColorSprite->getMaterial()->ensureUniform("u_bloomMap", ShaderUniformType::UniformTexture2D)->set(m_bloomController->getBloomRenderBuffer()->getColorTexture());
        }
    }

    //////////////////////////////////////////
    void SceneGame::notifyMouse(InputEventMouseData const& _data)
    {
        
    }

    //////////////////////////////////////////
    void SceneGame::createControllers()
    {
        EntityPtr sceneControllerEntity = createEntity("Scene Controller");
        m_levelAdapter = sceneControllerEntity->ensureComponent<LevelAdapter>(this);

        EntityPtr projectilePoolEntity = createEntity("Projectile Pool");
        m_projectilePool = projectilePoolEntity->ensureComponent<ProjectilePool>(m_levelAdapter.get());
        m_levelAdapter->setProjectilePool(m_projectilePool);

        EntityPtr spaceObjectPoolEntity = createEntity("SpaceObject Pool");
        m_spaceObjectPool = spaceObjectPoolEntity->ensureComponent<SpaceObjectPool>(m_levelAdapter.get());
        m_levelAdapter->setSpaceObjectPool(m_spaceObjectPool);

        EntityPtr specialEffectPoolEntity = createEntity("SpecialEffect Pool");
        m_specialEffectPool = spaceObjectPoolEntity->ensureComponent<SpecialEffectPool>(m_levelAdapter.get());
        m_levelAdapter->setSpecialEffectPool(m_specialEffectPool);

    }

    //////////////////////////////////////////
    void SceneGame::create3D()
    {
        EntityPtr cameraEntity = createEntity("Camera");
        m_camera3D = cameraEntity->createComponent<Camera3D>();
        m_camera3D->getTransform()->setLocalPosition(Vec3DF(0.0f, 0.5f, -18.0f));
        m_camera3D->setFOV(Math::DegreesToRadians(30));
        m_camera3D->setClearColor(ColorU32::c_red);
        m_camera3D->setClearColorFlag(true);

        if (SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->getPostProcessEnabled())
            m_camera3D->setRenderTarget(m_renderBuffer);
        else
            m_camera3D->setRenderTarget(Game::GetInstancePtr()->getMainRenderWindow());
        

        m_camera3D->setRenderMask(m_camera3D->getRenderMask() & ~(S32)DefaultRenderMask::Gizmos);
        m_camera3D->setNearZ(0.01f);
        m_camera3D->setFarZ(30.0f);

        updateGameplayBounds();
    }

    //////////////////////////////////////////
    void SceneGame::create2D()
    {
        if (SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->getPostProcessEnabled())
        {
            m_renderBuffer = RenderBuffer::Create(
                {
                    Game::GetInstancePtr()->getMainRenderWindowAbsoluteSize(),
                    PixelFormat::RGBA_F16,
                    PixelFormat::DEPTH_U24
                });
            m_renderBuffer->getColorTexture()->setMinFilter(TextureFilter::Linear);
            m_renderBuffer->getColorTexture()->setMagFilter(TextureFilter::Linear);
        }

        EntityPtr canvasEntity = createEntity("Canvas");
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Game::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Game::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearColor(ColorU32::c_zero);

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);
        canvasScaler->updateCanvasScale();

        
        if (SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->getPostProcessEnabled())
        {
            m_renderColorSprite = SpriteHelper::CreateSprite(
                Sprite::Create(m_renderBuffer->getColorTexture()),
                m_canvas->getTransform()->getSize(),
                Vec2DF::c_zero,
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("PostFX00.mzmaterial"),
                m_canvas->getTransform(),
                this);
            m_renderColorSprite->getTransform()->setZ(1000);
            m_renderColorSprite->getEntityRaw()->ensureComponent<Name>("RenderColorSprite");
            m_renderColorSprite->getEntityRaw()->ensureComponent<SizePolicy2D>();
        
            m_renderDepthSprite = SpriteHelper::CreateSprite(
                Sprite::Create(m_renderBuffer->getDepthTexture()),
                m_canvas->getTransform()->getSize(),
                Vec2DF::c_zero,
                GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("DepthBuffer00.mzmaterial"),
                m_canvas->getTransform(),
                this);
            m_renderDepthSprite->getTransform()->setZ(500);
            m_renderDepthSprite->getEntityRaw()->ensureComponent<Name>("RenderDepthSprite");
            m_renderDepthSprite->getEntityRaw()->ensureComponent<SizePolicy2D>();
            m_renderDepthSprite->getEntityRaw()->setActiveSelf(false);
        }

        if (GamepadManager::GetInstancePtr()->getVirtualGamepad())
        {
            EntityPtr virtualJoystickEntity = createEntity();
            m_virtualJoystickController = virtualJoystickEntity->createComponent<VirtualJoystickController>();
            m_virtualJoystickController->getTransform()->setParent(m_canvas->getTransform());
            virtualJoystickEntity->ensureComponent<Name>("Virtual Joystick");
        }
        
    }

    //////////////////////////////////////////
    void SceneGame::createSystems()
    {
        ECSWorldPtr const& world = EntityManager::GetInstancePtr()->getDefaultWorld();

        m_gameplaySystem = GameplaySystem::Create(world, this);
        m_gameplaySystem->eventUpdateFinished.subscribe(this, &SceneGame::notifyGameplaySystemUpdateFinished);
        addSystem(m_gameplaySystem);
    }

    //////////////////////////////////////////
    void SceneGame::notifyGameplaySystemUpdateFinished(F32 _dt)
    {
        if (   !Game::GetInstancePtr()->getRunning() 
            || !Game::GetInstancePtr()->getMainRenderWindow()
            || !Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->isOpened())
            return;

        updateGameplayBounds();

        eventPostGameplayUpdate(_dt);
    }

    //////////////////////////////////////////
    void SceneGame::updateGameplayBounds()
    {
        F32 aspectRatio = m_camera3D->getAspectRatio();
        F32 halfFOVY = m_camera3D->getFOV() * 0.5f;
        F32 halfFOVX = atan(aspectRatio * tan(halfFOVY));
        F32 z = -m_camera3D->getTransform()->getLocalPosition().z;
        F32 y = tanf(halfFOVY) * z;
        F32 x = tanf(halfFOVX) * z;

        Vec2DF halfSize(x, y);

        Vec2DF cameraPosition(
            m_camera3D->getTransform()->getLocalPosition().x,
            m_camera3D->getTransform()->getLocalPosition().y);

        m_gameplayBounds.position = cameraPosition - halfSize;
        m_gameplayBounds.size = halfSize * 2;

        if (m_environmentController)
            m_environmentController->updateSkyTransform();
    }

    //////////////////////////////////////////
    void SceneGame::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        if (!Game::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        m_canvas->setViewport(_mainRenderWindowViewport);

        if (SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->getPostProcessEnabled())
        {
            if (!Game::GetInstancePtr()->isDebugEditorProgress())
                m_renderBuffer->setSize(Game::GetInstancePtr()->getMainRenderWindowAbsoluteSize());
        }
        else
        {
            m_camera3D->setViewport(_mainRenderWindowViewport);
        }

    }

    //////////////////////////////////////////
    void SceneGame::notifyFixedUpdateFinished(F32 _dt)
    {
        eventFixedUpdate(_dt);
    }

    //////////////////////////////////////////
    void SceneGame::notifyRenderTargetResized(RenderTarget* _renderTarget)
    {
        if (!Game::GetInstancePtr()->isMainWindowReadyToRender())
            return;

        if (SettingsManager::GetInstancePtr()->getSettings<GameGraphicsSettings>()->getPostProcessEnabled())
        {
            m_renderBuffer->setSize(Game::GetInstancePtr()->getMainRenderWindowAbsoluteSize());
        }
    }

    //////////////////////////////////////////
    ProjectilePtr SceneGame::createProjectile(Vec2DF const& _position)
    {
        EntityPtr entity = Entity::Create();
        ProjectilePtr projectile = entity->ensureComponent<Projectile>();
        projectile->getTransform()->setLocalPosition(Vec3DF(_position));

        return projectile;
    }

    //////////////////////////////////////////
    void SceneGame::loadLevel(String const& _levelFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_levelFileName);
        MAZE_ERROR_RETURN_IF(!assetFile, "Level '%s' is not found!", _levelFileName.c_str());

        tinyxml2::XMLDocument doc;
        MAZE_ERROR_RETURN_IF(!assetFile->readToXMLDocument(doc), "Level '%s' has invalid format!");

        tinyxml2::XMLNode* rootNode = doc.FirstChild();
        MAZE_ERROR_RETURN_IF (!rootNode, "File '%s' loading error - empty root node!", _levelFileName.c_str());

        rootNode = rootNode->NextSibling();
        MAZE_ERROR_RETURN_IF (!rootNode, "File '%s' loading error - empty root node children!", _levelFileName.c_str());
        
        tinyxml2::XMLNode* levelNode = rootNode->FirstChild();
        while (levelNode)
        {
            tinyxml2::XMLElement* levelElement = levelNode->ToElement();

            if (!levelElement)
            {
                levelNode = levelNode->NextSibling();
                continue;
            }

            if (strcmp(levelElement->Name(), "SpawnData") == 0)
            {
                m_spawnController->loadFromXMLElement(levelElement);
            }
            else
            if (strcmp(levelElement->Name(), "BackgroundData") == 0)
            {
                m_environmentController->loadFromXMLElement(levelElement);
            }

            levelNode = levelNode->NextSibling();
        }
    }


} // namespace Maze
//////////////////////////////////////////
