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
#include "SceneSettings.hpp"
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
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
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
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-debugger/settings/MazeDebuggerSettings.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "scenes/SceneDebug.hpp"
#include "managers/GameManager.hpp"
#include "game/SpaceObject.hpp"
#include "ui/UIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SceneSettings
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneSettings, ECSRenderScene);

    //////////////////////////////////////////
    SceneSettings::SceneSettings()
    {
    }

    //////////////////////////////////////////
    SceneSettings::~SceneSettings()
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
    SceneSettingsPtr SceneSettings::Create()
    {
        SceneSettingsPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneSettings, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneSettings::init()
    {
        RenderWindowPtr const& renderWindow = Game::GetInstancePtr()->getMainRenderWindow();
        if (!ECSRenderScene::init(renderWindow))
            return false;

        Game::GetInstancePtr()->eventMainRenderWindowViewportChanged.subscribe(this, &SceneSettings::notifyMainRenderWindowViewportChanged);

        create2D();

        return true;
    }

    //////////////////////////////////////////
    void SceneSettings::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void SceneSettings::create2D()
    {
        EntityPtr canvasEntity = createEntity();
        m_canvas = canvasEntity->createComponent<Canvas>();
        m_canvas->setViewport(Game::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setRenderTarget(Game::GetInstancePtr()->getMainRenderWindow());
        m_canvas->setClearColorFlag(false);
        m_canvas->setClearColor(ColorU32::c_gray);
        m_canvas->getEntityRaw()->ensureComponent<Name>("Canvas");

        m_canvasTransition = canvasEntity->ensureComponent<UITweenTransitionAlpha>();
        m_canvasTransition->hideInstantly();

        CanvasScalerPtr canvasScaler = canvasEntity->ensureComponent<CanvasScaler>();
        canvasScaler->setScaleMode(CanvasScaler::ScaleMode::ScaleWithViewportSize);
        canvasScaler->setScreenMatchMode(CanvasScaler::ScreenMatchMode::MatchWidthOrHeight);
        canvasScaler->setMatchWidthOrHeight(1.0f);

        SpriteRenderer2DPtr fadeSquare = SpriteHelper::CreateSprite(
            ColorU32(0, 0, 0, 100),
            m_canvas->getTransform()->getSize(),
            Vec2DF::c_zero,
            nullptr,
            m_canvas->getTransform(),
            this);
        fadeSquare->getTransform()->setZ(-1000);
        fadeSquare->getEntityRaw()->ensureComponent<Name>()->setName("Fade");
        fadeSquare->getEntity()->ensureComponent<UIElement2D>()->setCaptureCursorHits(true);
        fadeSquare->getEntity()->ensureComponent<SizePolicy2D>();

        SpriteRenderer2DPtr menuBackgroundSprite = SpriteHelper::CreateSprite(
            "Button00.mztexture",
            {400.0f, 500.0f},
            Vec2DF::c_zero,
            nullptr,
            m_canvas->getTransform(),
            this);
        menuBackgroundSprite->getEntityRaw()->ensureComponent<Name>()->setName("Background");


        // Bottom menu
        ClickButton2DPtr closeButton = UIHelper::CreateDefaultGameClickButton(
            this,
            menuBackgroundSprite->getTransform(),
            {0.0f, 50.0f},
            {238 * 0.75f, 60 * 0.75f},
            "CLOSE",
            CreateDelegate(this, &SceneSettings::notifyCloseButtonClick));
        closeButton->getTransform()->setAnchor(0.5f, 0.0f);

        // Top menu
        HorizontalLayout2DPtr topMenuLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            Vec2DF(menuBackgroundSprite->getTransform()->getWidth(), 60.0f),
            Vec2DF(0, -20),
            menuBackgroundSprite->getTransform(),
            this,
            Vec2DF(0.5f, 1.0f),
            Vec2DF(0.5f, 1.0f));
        topMenuLayout->setExpand(false);

        auto createTopMenuButtonFunc = 
            [&, this](SettingsMode _mode, String const& _title)
            {
                ToggleButton2DPtr button = UIHelper::CreateDefaultGameToggleButton(
                    this,
                    topMenuLayout->getTransform(),
                    { 0.0f, 0.0f },
                    { 138 * 0.7f, 60 * 0.7f },
                    _title,
                    CreateDelegate(this, &SceneSettings::notifyTopMenuButtonClick));
                button->setCheckByClick(false);
                button->setChecked(m_settingsMode == _mode);
                m_menuButtons[(Size)_mode] = button;
            };
        
        createTopMenuButtonFunc(SettingsMode::Video, "VIDEO");
        createTopMenuButtonFunc(SettingsMode::Sound, "SOUND");
        createTopMenuButtonFunc(SettingsMode::Game, "GAME");
        createTopMenuButtonFunc(SettingsMode::Controls, "CONTROLS");

        // Menus
        F32 menuWidth = menuBackgroundSprite->getTransform()->getWidth() - 20;
        auto createMenuFunc =
            [&, this](SettingsMode _mode) -> Transform2DPtr
            {
                VerticalLayout2DPtr menuLayout = UIHelper::CreateVerticalLayout(
                    HorizontalAlignment2D::Left,
                    VerticalAlignment2D::Top,
                    Vec2DF(menuWidth, 500.0f),
                    Vec2DF(10, -90),
                    menuBackgroundSprite->getTransform(),
                    this,
                    Vec2DF(0.5f, 1.0f),
                    Vec2DF(0.5f, 1.0f));
                menuLayout->setExpand(false);

                menuLayout->getEntity()->setActiveSelf(m_settingsMode == _mode);

                m_menus[(Size)_mode] = menuLayout->getTransform();

                return menuLayout->getTransform();
            };

        Transform2DPtr videoMenu = createMenuFunc(SettingsMode::Video);
        Transform2DPtr soundMenu = createMenuFunc(SettingsMode::Sound);
        Transform2DPtr gameMenu = createMenuFunc(SettingsMode::Game);
        Transform2DPtr controlsMenu = createMenuFunc(SettingsMode::Controls);

        {
            HorizontalLayout2DPtr layout = UIHelper::CreateHorizontalLayout(
                HorizontalAlignment2D::Center,
                VerticalAlignment2D::Middle,
                Vec2DF(menuWidth, 10.0f),
                Vec2DF(0, 0),
                videoMenu,
                this);

            SystemTextRenderer2DPtr label = SpriteHelper::CreateSystemText(
                "FULLSCREEN",
                10,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 200.0f, 10.0f },
                { 0.0f, 0.0f },
                nullptr,
                layout->getTransform(),
                this);
            label->setColor(ColorU32(255, 150, 0));

            ToggleButton2DPtr fullscreenToggle = UIHelper::CreateDefaultToggleButton(
                { 0.0f, 0.0f },
                layout->getTransform(),
                this);
            fullscreenToggle->setChecked(Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->getFullscreen());
            fullscreenToggle->eventCheckedChanged.subscribe(
                [](ToggleButton2D* _toggle, bool _value)
                {
                    if (!_value)
                        Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->setFullscreen(false);
                    else
                        Game::GetInstancePtr()->getMainRenderWindow()->getWindow()->maximizeFullscreen();
                });
        }
    }

    //////////////////////////////////////////
    void SceneSettings::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {
        m_canvas->setViewport(_mainRenderWindowViewport);
    }

    //////////////////////////////////////////
    void SceneSettings::processSceneStateChanged(ECSSceneState _state)
    {
        switch (_state)
        {
            case ECSSceneState::Active:
            {
                m_canvasTransition->show();
                break;
            }
        }
    }

    //////////////////////////////////////////
    void SceneSettings::notifyCloseButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        ECSSceneWPtr sceneWeak = getSharedPtr();
        m_canvasTransition->hide(
            [sceneWeak]()
            {
                ECSScenePtr scene = sceneWeak.lock();
                if (scene)
                {
                    SceneManager::GetInstancePtr()->destroyScene(scene);
                }
            });
        
    }

    //////////////////////////////////////////
    void SceneSettings::notifyTopMenuButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        Size index = 
            std::distance(
                m_menuButtons.begin(),
                std::find(
                    m_menuButtons.begin(),
                    m_menuButtons.end(),
                    _button->getSharedPtr()));

        setMode((SettingsMode)index);
    }

    //////////////////////////////////////////
    void SceneSettings::setMode(SettingsMode _value)
    {
        if (m_settingsMode == _value)
            return;

        m_settingsMode = _value;

        updateMenus();
    }

    //////////////////////////////////////////
    void SceneSettings::updateMenus()
    {
        for (Size i = 0; i < (Size)SettingsMode::MAX; ++i)
        {
            ToggleButton2DPtr const& button = m_menuButtons[i];
            Transform2DPtr const& menu = m_menus[i];

            button->setChecked(m_settingsMode == (SettingsMode)i);
            menu->getEntityRaw()->setActiveSelf(m_settingsMode == (SettingsMode)i);
        }
    }

} // namespace Maze
//////////////////////////////////////////
