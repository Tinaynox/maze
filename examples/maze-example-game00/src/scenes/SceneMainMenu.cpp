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
#include "SceneMainMenu.hpp"
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
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-ui/ecs/components/MazeButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
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
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneGame.hpp"
#include "managers/PlayerManager.hpp"
#include "input/PlayerGamepad.hpp"
#include "player/Player.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneMainMenu
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneMainMenu, ECSRenderScene);

    //////////////////////////////////////////
    SceneMainMenu::SceneMainMenu()
    {
    }

    //////////////////////////////////////////
    SceneMainMenu::~SceneMainMenu()
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
    SceneMainMenuPtr SceneMainMenu::Create()
    {
        SceneMainMenuPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneMainMenu, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneMainMenu::init()
    {
        RenderWindowPtr const& renderWindow = Game::GetInstancePtr()->getMainRenderWindow();
        if (!ECSRenderScene::init(renderWindow))
            return false;

        Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneMainMenu::notifyMainRenderWindowViewportChanged);

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();

        if (GamepadManager::GetInstancePtr()->getVirtualGamepad())
        {
            PlayerManager::GetInstancePtr()->getPlayer(0)->getPlayerGamepad()->setDevice(
                PlayerGamepadDeviceType::Gamepad,
                GamepadManager::GetInstancePtr()->getVirtualGamepad()->getId());
        }
        else
        {
            PlayerManager::GetInstancePtr()->getPlayer(0)->getPlayerGamepad()->setDevice(
                PlayerGamepadDeviceType::KeyboardMouse,
                0);
        }
    
        MaterialPtr spriteMaterial = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getMaterialManager()->getMaterial("SpriteDefault00.mzmaterial");
        spriteManager->setDefaultSpriteMaterial(spriteMaterial);
        

        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneMainMenu::createMainMenuButtons()
    {
        F32 gap = 42.0f;
        m_buttonStartGame = createButton(
            Vec2DF(0.0f, -gap * 0),
            "START GAME",
            CreateDelegate(this, &SceneMainMenu::notifyStartGameButtonClick));
    
        m_settingsGame = createButton(
            Vec2DF(0.0f, -gap * 1),
            "SETTINGS",
            CreateDelegate(this, &SceneMainMenu::notifySettingsButtonClick));

        m_creditsGame = createButton(
            Vec2DF(0.0f, -gap * 2),
            "CREDITS",
            CreateDelegate(this, &SceneMainMenu::notifyCreditsButtonClick));

        m_exitGame = createButton(
            Vec2DF(0.0f, -gap * 3),
            "EXIT",
            CreateDelegate(this, &SceneMainMenu::notifyExitButtonClick));

    }

    //////////////////////////////////////////
    ClickButton2DPtr SceneMainMenu::createButton(
        Vec2DF _position,
        String const& _label,
        Delegate<void, Button2D*, CursorInputEvent const&> _onClickDelegate)
    {
        EntityPtr buttonEntity = createEntity();
        Transform2DPtr buttonEntityTransform = buttonEntity->ensureComponent<Transform2D>();
        buttonEntityTransform->setSize(Vec2DF(238, 60));
        buttonEntityTransform->setParent(m_canvas->getTransform());
        buttonEntityTransform->setLocalPosition(_position);
        buttonEntityTransform->setLocalScale(0.75f);
        ClickButton2DPtr button = buttonEntity->ensureComponent<ClickButton2D>();
        button->eventClick.subscribe(_onClickDelegate);
        buttonEntity->ensureComponent<Name>("Button");

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        // ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        

        SpriteRenderer2DPtr buttonSprite = SpriteHelper::CreateSprite(
            "Button00.mztexture",
            buttonEntityTransform->getSize(),
            Vec2DF::c_zero,
            spriteManager->getDefaultSpriteMaterial(),
            buttonEntityTransform,
            this);
        buttonSprite->getEntityRaw()->ensureComponent<Name>("ButtonSpriteDefault");
        UITweenTransitionScalePtr const& pressedTransition = buttonSprite->getEntityRaw()->ensureComponent<UITweenTransitionScale>();
        pressedTransition->setHiddenScale(Vec2DF(1.0f, 1.0f));
        pressedTransition->setShownScale(Vec2DF(0.95f, 0.9f));
        pressedTransition->setShowTime(0.2f);
        pressedTransition->hideInstantly();

        SpriteRenderer2DPtr buttonFocusedSprite = SpriteHelper::CreateSprite(
            "Button00Focused.mztexture",
            buttonEntityTransform->getSize(),
            Vec2DF::c_zero,
            spriteManager->getDefaultSpriteMaterial(),
            buttonSprite->getTransform(),
            this);
        buttonFocusedSprite->getEntityRaw()->ensureComponent<Name>("ButtonSpriteFocused");
        UITweenTransitionAlphaPtr const& focusedTransition = buttonFocusedSprite->getEntityRaw()->ensureComponent<UITweenTransitionAlpha>();
        focusedTransition->setShowTime(0.3f);
        focusedTransition->setAlphaPower(1.0f);
        focusedTransition->setHidden(!button->getFocused());

        button->eventFocusChanged.subscribe(
            [focusedTransition](Button2D* _button, bool _value)
            {
                focusedTransition->setHidden(!_value);
            });

        button->eventPressedChanged.subscribe(
            [pressedTransition](Button2D* _button, bool _value)
            {
                pressedTransition->setHidden(!_value);
            });
    
    
        EntityPtr labelEntity = createEntity();
        Transform2DPtr labelEntityTransform = labelEntity->ensureComponent<Transform2D>();
        labelEntityTransform->setSize(buttonSprite->getTransform()->getSize());
        labelEntityTransform->setParent(buttonSprite->getTransform());
        labelEntityTransform->setLocalScale(Vec2DF(0.85f, 1.0f));
        labelEntity->ensureComponent<Name>("LabelEntity");

        for (S32 i = 0; i < 8; ++i)
        {
            F32 outlineOffset = 1.75f;
            Vec2DF offset;
            switch (i)
            {
                case 0: offset = Vec2DF(-outlineOffset, 0.0f); break;
                case 1: offset = Vec2DF(+outlineOffset, 0.0f); break;
                case 2: offset = Vec2DF(0.0f, -outlineOffset); break;
                case 3: offset = Vec2DF(0.0f, +outlineOffset); break;
                case 4: offset = Vec2DF(-outlineOffset, +outlineOffset); break;
                case 5: offset = Vec2DF(+outlineOffset, +outlineOffset); break;
                case 6: offset = Vec2DF(-outlineOffset, -outlineOffset); break;
                case 7: offset = Vec2DF(+outlineOffset, -outlineOffset); break;
            }

            SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
                _label.c_str(),
                16,
                HorizontalAlignment2D::Center,
                VerticalAlignment2D::Middle,
                buttonSprite->getTransform()->getSize(),
                Vec2DF(0.0f, 2.0f) + offset,
                spriteManager->getDefaultSpriteMaterial(),
                labelEntityTransform,
                this);
            systemText->setColor(ColorU32(38, 32, 27, 255));
            systemText->getEntityRaw()->ensureComponent<Name>("Shadow");
        }

        SystemTextRenderer2DPtr systemText = SpriteHelper::CreateSystemText(
            _label.c_str(),
            16,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            buttonSprite->getTransform()->getSize(),
            Vec2DF(0.0f, 2.0f),
            spriteManager->getDefaultSpriteMaterial(),
            labelEntityTransform,
            this);
        systemText->setColor(ColorU32(255, 160, 0, 255));
        systemText->getEntityRaw()->ensureComponent<Name>("Text");

        return button;
    }

    //////////////////////////////////////////
    void SceneMainMenu::notifyStartGameButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (!SceneManager::GetInstancePtr()->getScene<SceneFadePreloader>())
        {
            ECSSceneWPtr sceneWeak = getSharedPtr();

            SceneFadePreloaderPtr sceneFadePreloader =  SceneManager::GetInstancePtr()->loadScene<SceneFadePreloader>();
            SceneFadePreloaderWPtr sceneFadePreloaderWeak = sceneFadePreloader;
            sceneFadePreloader->eventFade.subscribe(
                [sceneFadePreloaderWeak, sceneWeak, this]()
                {
                    SceneManager::GetInstancePtr()->loadScene<SceneGame>();

                    ECSScenePtr scene = sceneWeak.lock();
                    if (scene)
                    {
                        SceneManager::GetInstancePtr()->destroyScene(scene);
                    }

                    sceneFadePreloaderWeak.lock()->fadeOut();
                });
        }
    }

    //////////////////////////////////////////
    void SceneMainMenu::notifySettingsButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
    
    }

    //////////////////////////////////////////
    void SceneMainMenu::notifyCreditsButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        
    }

    //////////////////////////////////////////
    void SceneMainMenu::notifyExitButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->close();
        Game::GetInstancePtr()->shutdown();
    }

    //////////////////////////////////////////
    void SceneMainMenu::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void SceneMainMenu::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Game::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Game::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(true);
        m_canvas->setClearColor(ColorU32::c_zero);
        m_canvas->getEntityRaw()->ensureComponent<Name>("Canvas");

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);

        m_splashSprite = SpriteHelper::CreateSprite(
            "Sky.mztexture",
            Vec2DF(1200, 600),
            Vec2DF::c_zero,
            "MenuSky00.mzmaterial",
            m_canvas->getTransform(),
            this);
        m_splashSprite->getTransform()->setZ(-1000);
        m_splashSprite->getEntityRaw()->ensureComponent<Name>("Sky");

        createMainMenuButtons();
    }

    //////////////////////////////////////////
    void SceneMainMenu::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        if (m_canvas)
            m_canvas->setViewport(_mainRenderWindowViewport);
    }

} // namespace Maze
//////////////////////////////////////////
