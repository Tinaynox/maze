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
#include "SceneSplash.hpp"
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
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-ui/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "scenes/SceneDebug.hpp"
#include "configs/GameConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneSplash
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneSplash, ECSRenderScene);

    //////////////////////////////////////////
    SceneSplash::SceneSplash()
        : m_fadeProgress(1.0f)
        , m_state(State::None)
    {
    }

    //////////////////////////////////////////
    SceneSplash::~SceneSplash()
    {
        if (Game::GetInstancePtr())
        {
            Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
        }

        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SceneSplashPtr SceneSplash::Create()
    {
        SceneSplashPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneSplash, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneSplash::init()
    {
        RenderWindowPtr const& renderWindow = Game::GetInstancePtr()->getMainRenderWindow();
        if (!ECSRenderScene::init(renderWindow))
            return false;

        Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneSplash::notifyMainRenderWindowViewportChanged);

        create2D();

        Game::GetInstancePtr()->updateDebugEditor();

        SceneManager::GetInstancePtr()->loadScene<SceneDebug>();

        m_state = State::FadeOut;

        return true;
    }

    //////////////////////////////////////////
    void SceneSplash::update(F32 _dt)
    {
        _dt = Math::Min(_dt, 1.0f/60.0f);

        switch (m_state)
        {
            case State::FadeOut:
            {
                F32 fadeSpeed = 3.0f;
                m_fadeProgress -= _dt * fadeSpeed;
                if (m_fadeProgress <= 0.0f)
                {
                    m_fadeProgress = 0.0f;
                    m_state = State::ReadyToLoadNextScene;
                }

                m_blackFade->getTransform()->setSize(m_canvas->getTransform()->getSize());
                m_blackFadeCanvasGroup->setLocalAlpha(m_fadeProgress);
                break;
            }
            case State::ReadyToLoadNextScene:
            {
                SceneFadePreloaderPtr sceneFadePreloader =  SceneManager::GetInstancePtr()->loadScene<SceneFadePreloader>();
                SceneFadePreloaderWPtr sceneFadePreloaderWeak = sceneFadePreloader;
                sceneFadePreloader->eventFade.subscribe(
                    [sceneFadePreloaderWeak, this]()
                    {
                        SceneManager::GetInstancePtr()->loadScene<SceneMainMenu>();
                        Game::GetInstancePtr()->eventCoreGameResourcesLoaded();
                        SceneManager::GetInstancePtr()->destroyScene(getSharedPtr());
                        sceneFadePreloaderWeak.lock()->fadeOut();
                    });

                m_state = State::NextSceneLoading;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void SceneSplash::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Game::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Game::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearColor(ColorU32::c_zero);
        m_canvas->getEntityRaw()->ensureComponent<Name>("Canvas");

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setReferenceResolution(c_canvasReferenceResolution);
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();

        MaterialPtr spriteMaterial = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("SplashUI00.mzmaterial");
        spriteManager->setDefaultSpriteMaterial(spriteMaterial);

        SpriteRenderer2DPtr splashSprite = SpriteHelper::CreateSprite(
            "Splash00.mztexture",
            Vec2DF(800, 600),
            Vec2DF::c_zero,
            spriteManager->getDefaultSpriteMaterial(),
            m_canvas->getTransform(),
            this);
        splashSprite->getEntityRaw()->ensureComponent<Name>("SplashImage");
        splashSprite->getTransform()->setZ(-1000);


         m_graySquare = SpriteHelper::CreateSprite(
            ColorU32::c_black,
            Vec2DF(200, 100),
             Vec2DF(0.0f, -100.0f),
            spriteManager->getDefaultSpriteMaterial(),
            m_canvas->getTransform(),
            this);
         m_graySquare->getEntityRaw()->ensureComponent<Name>("GraySquare");

        m_loadingText = SystemUIHelper::CreateSystemText(
            "Loading...",
            16,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            Vec2DF(200, 100),
            Vec2DF::c_zero,
            m_graySquare->getTransform(),
            this);
        m_loadingText->getEntityRaw()->ensureComponent<Name>("LoadingText");


        m_blackFade = SpriteHelper::CreateSprite(
            ColorU32::c_black,
            m_canvas->getTransform()->getSize(),
             Vec2DF(0.0f, 0.0f),
            spriteManager->getDefaultSpriteMaterial(),
            m_canvas->getTransform(),
            this);
         m_blackFadeCanvasGroup = m_blackFade->getEntityRaw()->ensureComponent<CanvasGroup>();
         m_blackFadeCanvasGroup->setLocalAlpha(m_fadeProgress);
         m_blackFadeCanvasGroup->getEntityRaw()->ensureComponent<Name>("BlackFade");
    }

    //////////////////////////////////////////
    void SceneSplash::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_canvas->setViewport(_mainRenderWindowViewport);
    }


} // namespace Maze
//////////////////////////////////////////
