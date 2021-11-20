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
#include "SceneExample.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "Example.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetExampleName()
    {
        return "Gamepad";
    }

    //////////////////////////////////////////
    void LoadFirstExampleScene(SceneManager* _sceneManager)
    {
        _sceneManager->loadScene<SceneExample>();
    }


    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SceneExample, ECSRenderScene);

    //////////////////////////////////////////
    SceneExample::SceneExample()
    {
    }

    //////////////////////////////////////////
    SceneExample::~SceneExample()
    {
        InputManager* inputManager = InputManager::GetInstancePtr();
        if (inputManager)
        {
            inputManager->eventMouse.unsubscribe(this);
        }

        if (GamepadManager::GetInstancePtr())
            GamepadManager::GetInstancePtr()->eventGamepadsChanged.unsubscribe(this);

        if (m_gamepadsDropdown)
            m_gamepadsDropdown->eventValueChanged.unsubscribe(this);

        Example::GetInstancePtr()->eventMainRenderWindowViewportChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    SceneExamplePtr SceneExample::Create()
    {
        SceneExamplePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SceneExample, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SceneExample::init()
    {
        if (!ECSRenderScene::init(Example::GetInstancePtr()->getMainRenderWindow()))
            return false;

        create2D();

        GamepadManager::GetInstancePtr()->eventGamepadsChanged.subscribe(this, &SceneExample::updateUI);

        return true;
    }

    //////////////////////////////////////////
    void SceneExample::notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport)
    {        
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
    }

    //////////////////////////////////////////
    void SceneExample::update(F32 _dt)
    {
        ECSRenderScene::update(_dt);

        m_gamepadsCountText->setText("Gamepads Count: " +
            StringHelper::ToString(GamepadManager::GetInstancePtr()->getGamepadsCount()));

        m_detectGamepadsTimer += _dt;
        if (m_detectGamepadsTimer >= 1.0f)
        {
            m_detectGamepadsTimer = 0.0f;
            GamepadManager::GetInstancePtr()->detectGamepads();
        }

        updateGamepadData();
    }

    //////////////////////////////////////////
    void SceneExample::create2D()
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        ShaderSystemPtr const& shaderSystem = renderSystem->getShaderSystem();
        SpriteManagerPtr const& spriteManager = renderSystem->getSpriteManager();
        RenderMeshManagerPtr const& renderMeshManager = renderSystem->getRenderMeshManager();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        spriteManager->setDefaultSpriteMaterial(materialManager->getColorTextureMaterial());

        RenderWindowPtr const& renderTarget = Example::GetInstancePtr()->getMainRenderWindow();

        // Canvas
        Maze::EntityPtr canvasEntity = createEntity();
        Maze::Transform2DPtr canvasTransform2D = canvasEntity->createComponent<Maze::Transform2D>();
        m_canvas = canvasEntity->createComponent<Maze::Canvas>();
        m_canvas->setClearColor(203, 203, 203);
        m_canvas->setRenderTarget(renderTarget);
        m_canvas->setViewport(Example::GetInstancePtr()->getMainRenderWindowViewport());
        m_canvas->setClearColorFlag(true);

        m_gamepadsUIRoot = SpriteHelper::CreateTransform2D(
            m_canvas->getTransform()->getSize(),
            { 0.0f, 0.0f },
            canvasTransform2D,
            this,
            { 0.0f, 0.0f },
            { 0.0f, 0.0f });
        
        m_gamepadsCountText = SpriteHelper::CreateSystemText(
            "123", 8,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            { 500.0f, 100.0f },
            { 10.0f, -10.0f },
            materialManager->getColorTextureMaterial(),
            canvasTransform2D,
            this,
            {0.0f, 1.0f},
            {0.0f, 1.0f});
        m_gamepadsCountText->setColor(ColorU32::c_black);
        
        updateUI();
    }

    //////////////////////////////////////////
    void SceneExample::updateUI()
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        m_enabledText.reset();
        m_axesUI.clear();
        m_buttonsUI.clear();

        if (m_gamepadsUIRoot)
            m_gamepadsUIRoot->removeAllChildren();
        
        if (m_gamepadsDropdown)
            m_gamepadsDropdown->eventValueChanged.unsubscribe(this);

        GamepadManager::GamepadListByDeviceId const& gamepads = GamepadManager::GetInstancePtr()->getGamepads();
        if (gamepads.empty())
            return;

        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            { 1024.0f, 1024.0 },
            { 10.0f, -24.0f },
            m_gamepadsUIRoot,
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        layout->setSpacing(5.0f);

        m_gamepadsDropdown = UIHelper::CreateDefaultDropdown(
            { 400.0f, 18.0f },
            { 0.0f, 0.0f },
            layout->getTransform(),
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });

        S32 index = 0;
        Vector<String> gamepadNames;
        for (auto const& gamepadData : gamepads)
            gamepadNames.emplace_back(StringHelper::ToString(++index) + " - " + gamepadData.second->getName());

        if (m_selectedGamepadIndex >= (S32)gamepads.size())
            m_selectedGamepadIndex = 0;

        m_gamepadsDropdown->addOptions(gamepadNames);

        if (!gamepads.empty())
            m_gamepadsDropdown->setValue(m_selectedGamepadIndex);

        m_gamepadsDropdown->eventValueChanged.subscribe(this, &SceneExample::notifyGamepadsDropdownValueChanged);

        if (m_selectedGamepadIndex >= (S32)gamepads.size())
            return;

        GamepadManager::GamepadListByDeviceId::const_iterator it = gamepads.begin();
        for (S32 i = 0; i < m_selectedGamepadIndex; ++i)
            ++it;
        GamepadPtr const& gamepad = it->second;
        
        {
            String str = "[CONNECTED]";
            m_enabledText = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            m_enabledText->setColor(ColorU32::c_green);
        }

        {
            String str = "Id: " + StringHelper::ToString(gamepad->getId());
            SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            text->setColor(ColorU32::c_black);
        }

        {
            String str = "Name: " + gamepad->getName();
            SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            text->setColor(ColorU32::c_black);
        }

        {
            String str = "Vendor Id: " + StringHelper::ToString(gamepad->getVendorId());
            SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            text->setColor(ColorU32::c_black);
        }

        {
            String str = "Product Id: " + StringHelper::ToString(gamepad->getProductId());
            SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            text->setColor(ColorU32::c_black);
        }

        {
            String str = "Device Id: " + StringHelper::ToString(gamepad->getDeviceId());
            SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            text->setColor(ColorU32::c_black);
        }

        {
            String str = "Axes: " + StringHelper::ToString(gamepad->getAxesCount());
            SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            text->setColor(ColorU32::c_black);
        }

        HorizontalLayout2DPtr axesLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            { 1024.0f, 40.0 },
            { 10.0f, -24.0f },
            layout->getTransform(),
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        axesLayout->setSpacing(1.0f);

        for (S32 i = 0; i < (S32)gamepad->getAxesCount(); ++i)
        {
            AxisUIData data = createAxisUI(i, axesLayout->getTransform());
            m_axesUI.push_back(data);
        }

        {
            String str = "Buttons: " + StringHelper::ToString(gamepad->getButtonsCount());
            SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
                str.c_str(),
                8,
                HorizontalAlignment2D::Left,
                VerticalAlignment2D::Top,
                { 500.0f, 10.0f },
                { 0.0f, 0.0f },
                materialManager->getColorTextureMaterial(),
                layout->getTransform(),
                this,
                { 0.0f, 1.0f },
                { 0.0f, 1.0f });
            text->setColor(ColorU32::c_black);
        }

        HorizontalLayout2DPtr buttonsLayout = UIHelper::CreateHorizontalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            { 1024.0f, 40.0 },
            { 10.0f, -24.0f },
            layout->getTransform(),
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        buttonsLayout->setSpacing(5.0f);

        for (S32 i = 0; i < (S32)gamepad->getButtonsCount(); ++i)
        {
            SpriteRenderer2DPtr data = createButtonUI(i, buttonsLayout->getTransform());
            m_buttonsUI.push_back(data);
        }

        updateGamepadData();
    }

    //////////////////////////////////////////
    void SceneExample::updateGamepadData()
    {
        GamepadManager::GamepadListByDeviceId const& gamepads = GamepadManager::GetInstancePtr()->getGamepads();
        
        if (gamepads.empty())
            return;

        GamepadManager::GamepadListByDeviceId::const_iterator it = gamepads.begin();
        for (S32 i = 0; i < m_selectedGamepadIndex; ++i)
            ++it;
        GamepadPtr const& gamepad = it->second;
        if (!gamepad)
            return;

        if (m_enabledText)
        {
            if (gamepad->getConnected())
            {
                m_enabledText->setText("[CONNECTED]");
                m_enabledText->setColor(50, 255, 50);
            }
            else
            {
                m_enabledText->setText("[DISCONNECTED]");
                m_enabledText->setColor(255, 50, 50);
            }
        }

        for (S32 i = 0; i < (S32)gamepad->getAxesCount(); ++i)
        {
            if (i >= (S32)m_axesUI.size())
                break;

            F32 axisState = gamepad->getAxisState(i);

            AxisUIData const& data = m_axesUI[i];
            
            if (axisState >= 0.0f)
            {
                data.graph->getTransform()->setPivot(0.0f, 0.5f);
                data.graph->getTransform()->setWidth(data.graph->getTransform()->getParent()->getWidth() * 0.5f * Math::Min(axisState, 1.0f));
                data.graph->setColor(50, 255, 50);    
            }
            else
            {
                data.graph->getTransform()->setPivot(1.0f, 0.5f);
                data.graph->getTransform()->setWidth(data.graph->getTransform()->getParent()->getWidth() * 0.5f * Math::Min(-axisState, 1.0f));
                data.graph->setColor(255, 50, 50);
            }

            data.value->setText(StringHelper::F32ToStringFormatted(axisState, 4));
        }

        for (S32 i = 0; i < (S32)gamepad->getButtonsCount(); ++i)
        {
            if (i >= (S32)m_buttonsUI.size())
                break;

            bool buttonState = gamepad->getButtonState(i);

            SpriteRenderer2DPtr const& data = m_buttonsUI[i];

            if (buttonState)
                data->setColor(50, 255, 50);
            else
                data->setColor(160, 160, 160);
        }
    }

    //////////////////////////////////////////
    void SceneExample::notifyGamepadsDropdownValueChanged(SystemTextDropdown2D* _dropdown, S32 _index)
    {
        m_selectedGamepadIndex = _index;

        updateUI();
    }

    //////////////////////////////////////////
    AxisUIData SceneExample::createAxisUI(S32 _index, Transform2DPtr const& _root)
    {
        AxisUIData data;

        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            { 100.0f, 10.0f },
            { 10.0f, -24.0f },
            _root,
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        layout->setSpacing(0.0f);

        String str = "Axis " + StringHelper::ToString(_index);
        SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
            str.c_str(),
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Top,
            { 80.0f, 10.0f },
            { 0.0f, 0.0f },
            nullptr,
            layout->getTransform(),
            this);
        text->setColor(ColorU32::c_black);

        SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
            ColorU32(160, 160, 160),
            { 80.0f, 20.0f },
            { 0.0f, 0.0f },
            MaterialManager::GetCurrentInstance()->getColorMaterial(),
            layout->getTransform(),
            this);

        data.graph = SpriteHelper::CreateSprite(
            ColorU32::c_green,
            { 40.0f, 20.0f },
            { 0.0f, 0.0f },
            MaterialManager::GetCurrentInstance()->getColorMaterial(),
            sprite->getTransform(),
            this,
            { 0.5f, 0.5f},
            { 0.0f, 0.5f });

        data.value = SpriteHelper::CreateSystemText(
            "0",
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Middle,
            { 80.0f, 10.0f },
            { 0.0f, 0.0f },
            nullptr,
            sprite->getTransform(),
            this);
        data.value->setColor(ColorU32::c_black);

        return data;
    }

    //////////////////////////////////////////
    SpriteRenderer2DPtr SceneExample::createButtonUI(S32 _index, Transform2DPtr const& _root)
    {
        VerticalLayout2DPtr layout = UIHelper::CreateVerticalLayout(
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Top,
            { 20.0f, 10.0f },
            { 10.0f, -24.0f },
            _root,
            this,
            { 0.0f, 1.0f },
            { 0.0f, 1.0f });
        layout->setSpacing(0.0f);

        String str = StringHelper::ToString(_index);
        SystemTextRenderer2DPtr text = SpriteHelper::CreateSystemText(
            str.c_str(),
            8,
            HorizontalAlignment2D::Center,
            VerticalAlignment2D::Top,
            { 20.0f, 10.0f },
            { 0.0f, 0.0f },
            nullptr,
            layout->getTransform(),
            this);
        text->setColor(ColorU32::c_black);

        SpriteRenderer2DPtr sprite = SpriteHelper::CreateSprite(
            ColorU32(160, 160, 160),
            { 20.0f, 20.0f },
            { 0.0f, 0.0f },
            MaterialManager::GetCurrentInstance()->getColorMaterial(),
            layout->getTransform(),
            this);

        return sprite;
    }

} // namespace Maze
//////////////////////////////////////////
