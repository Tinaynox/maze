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
#include "PlayerGamepad.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
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
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "Game.hpp"
#include "scenes/SceneFadePreloader.hpp"
#include "scenes/SceneMainMenu.hpp"
#include "managers/PlayerManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(PlayerGamepadDeviceCommand);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(PlayerGamepadDeviceType);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(PlayerGamepadInput);

    //////////////////////////////////////////
    // Class PlayerGamepad
    //
    //////////////////////////////////////////
    PlayerGamepad::PlayerGamepad()
        : m_playerIndex(0)
        , m_deviceType(PlayerGamepadDeviceType::None)
        , m_deviceNumber(0)
        , m_mouseWheelState(0.0f)
    {
    }

    //////////////////////////////////////////
    PlayerGamepad::~PlayerGamepad()
    {
    }

    //////////////////////////////////////////
    PlayerGamepadPtr PlayerGamepad::Create(S32 _playerIndex)
    {
        PlayerGamepadPtr playerGamepad;
        MAZE_CREATE_AND_INIT_SHARED_PTR(PlayerGamepad, playerGamepad, init(_playerIndex));
        return playerGamepad;
    }

    //////////////////////////////////////////
    bool PlayerGamepad::init(S32 _playerIndex)
    {
        m_playerIndex = _playerIndex;

        return true;
    }

    //////////////////////////////////////////
    Vec2F32 PlayerGamepad::getMoveAxis()
    {
        Vec2F32 result;
        result.x = getBindValue(m_bindMap[PlayerGamepadDeviceCommand::MoveRight]) - getBindValue(m_bindMap[PlayerGamepadDeviceCommand::MoveLeft]);
        result.y = getBindValue(m_bindMap[PlayerGamepadDeviceCommand::MoveUp]) - getBindValue(m_bindMap[PlayerGamepadDeviceCommand::MoveDown]);
        if (result.length() > 1.0f)
            result.normalize();
    
        return result;
    }

    //////////////////////////////////////////
    bool PlayerGamepad::getFireButton()
    {
        return getBindValue(m_bindMap[PlayerGamepadDeviceCommand::Fire]) > 0;
    }

    //////////////////////////////////////////
    F32 PlayerGamepad::getBindValue(PlayerGamepadBind const& _bind)
    {
        switch (m_deviceType)
        {
            case PlayerGamepadDeviceType::KeyboardMouse:
            {
                switch (_bind.input)
                {
                    case PlayerGamepadInput::Button:
                    {
                        bool buttonState = Game::GetInstancePtr()->getInputManager()->getKeyState((KeyCode)_bind.number);
                        return (buttonState ? 1.0f : 0.0f );
                    }

                    case PlayerGamepadInput::Mouse:
                    {
                        MAZE_TODO;
                        return 0.0f;
                    }

                    case PlayerGamepadInput::MouseButton:
                    {
                        bool buttonState = Game::GetInstancePtr()->getInputManager()->getCursorButtonState(0, _bind.number);
                        return (buttonState ? 1.0f : 0.0f );
                    }

                    case PlayerGamepadInput::MouseWheelPositive:
                    {
                        return m_mouseWheelState > 0.0f ? m_mouseWheelState / 40.0f : 0.0f;
                    }

                    case PlayerGamepadInput::MouseWheelNegative:
                    {
                        return m_mouseWheelState < 0.0f ? -m_mouseWheelState / 40.0f : 0.0f;
                    }

                    default:
                    {
                        break;
                    }
                }
                
                break;
            }

            case PlayerGamepadDeviceType::Gamepad:
            {
                GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadById(m_deviceNumber);
                if (!gamepad)
                    return 0;

                switch (_bind.input)
                {
                    case PlayerGamepadInput::AxisPositive:
                    {
                        F32 axisState = gamepad->getAxisState(_bind.number);
                        return ((axisState > 0.0f) ? axisState : 0.0f);
                    }

                    case PlayerGamepadInput::AxisNegative:
                    {
                        F32 axisState = gamepad->getAxisState(_bind.number);
                        return ((axisState < 0.0f) ? -axisState : 0.0f);
                    }

                    case PlayerGamepadInput::Button:
                    {
                        F32 buttonState = gamepad->getButtonState(_bind.number);
                        return (buttonState ? 1.0f : 0.0f);
                    }

                    default:
                    {
                        break;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }


        return 0;
    }

    //////////////////////////////////////////
    String PlayerGamepad::getDeviceName()
    {
        switch (m_deviceType)
        {
            case PlayerGamepadDeviceType::KeyboardMouse: return "${KeyboardAndMouse}";
            case PlayerGamepadDeviceType::Gamepad:
            {
                GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadById(m_deviceNumber);
                MAZE_DEBUG_BP_RETURN_VALUE_IF(!gamepad, String());
                return gamepad->getName();
            }
            default:
            {
                break;
            }
        }

        return String();
    }

    //////////////////////////////////////////
    void PlayerGamepad::setBind(
        PlayerGamepadDeviceCommand _command,
        PlayerGamepadBind const& _bind)
    { 
        if (m_bindMap[_command] == _bind)
            return;

        PlayerGamepadBindData* bindData = getCurrentPlayerBindData();

        PlayerGamepadBindMap::iterator it = m_bindMap.begin();
        PlayerGamepadBindMap::iterator end = m_bindMap.end();
        for (; it != end; ++it)
        {
            if ((*it).first == _command)
                continue;

            if ((*it).second == _bind)
            {
                m_bindMap[(*it).first] = PlayerGamepadBind();

                if (bindData)
                    bindData->bindMap[(*it).first] = PlayerGamepadBind();
            }
        }

        m_bindMap[_command] = _bind; 

        if (bindData)
            bindData->bindMap[_command] = _bind;
    }

    //////////////////////////////////////////
    void PlayerGamepad::resetBinds()
    {
        // Reset previous binds
        for (PlayerGamepadBindMap::iterator it = m_bindMap.begin(),
                                            end = m_bindMap.end();
                                            it != end;
                                            ++it )
        {
            it->second.input = PlayerGamepadInput::None;
        }
    }

    //////////////////////////////////////////
    bool PlayerGamepad::getConnected()
    {
        switch (m_deviceType)
        {
            case PlayerGamepadDeviceType::KeyboardMouse:
            {
                return true;
            }
            case PlayerGamepadDeviceType::Gamepad:
            {
                GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadById(m_deviceNumber);
                if (!gamepad)
                    return false;

                return gamepad->getConnected();
            }
            default:
            {
                break;
            }
        }

        return false;
    }

    //////////////////////////////////////////
    PlayerGamepadBindData* PlayerGamepad::getCurrentPlayerBindData()
    {
        S32 vid = 0;
        S32 pid = 0;
        if (m_deviceType == PlayerGamepadDeviceType::Gamepad)
        {
            GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadById(m_deviceNumber);
            MAZE_DEBUG_BP_RETURN_VALUE_IF(!gamepad, nullptr);
            
            vid = gamepad->getVendorId();
            pid = gamepad->getProductId();
        }

        return PlayerManager::GetInstancePtr()->getPlayerBindData( 
                    m_playerIndex,
                    m_deviceType,
                    vid,
                    pid );
    }

    //////////////////////////////////////////
    void PlayerGamepad::setDevice(
        PlayerGamepadDeviceType _deviceType,
        U32 _deviceNumber)
    {
        m_deviceType = _deviceType;
        m_deviceNumber = _deviceNumber;


        // Reset binds
        for (PlayerGamepadBindMap::iterator it = m_bindMap.begin(),
                                            end = m_bindMap.end();
                                            it != end;
                                            ++it )
            it->second.input = PlayerGamepadInput::None;


        if (PlayerGamepadDeviceType::None != getDeviceType())
        {

            S32 vid = 0;
            S32 pid = 0;
            
            if (m_deviceType == PlayerGamepadDeviceType::Gamepad)
            {
                GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadById(m_deviceNumber);
                MAZE_DEBUG_BP_RETURN_IF(!gamepad);
            
                vid = gamepad->getVendorId();
                pid = gamepad->getProductId();
            }

            if (autobindWithFile("Devices.xml"))
            {
                Debug::Log("Gamepad autobind success!");

                PlayerGamepadBindData* bindData = getCurrentPlayerBindData();
                if (bindData)
                {
                    bindData->bindMap = m_bindMap;
                }
            }
            else
            {
                MAZE_WARNING("Unknown gamepad bind!");
            }
        }
    }

    //////////////////////////////////////////
    bool PlayerGamepad::autobindWithFile(String const& _fileName)
    {       
        AssetManagerPtr const& resourceManager = Game::GetInstancePtr()->getAssetManager();
        tinyxml2::XMLDocument doc;
        if (!resourceManager->openXMLDocumentAssetFile(doc, _fileName, false))
            return false;

        Debug::Log("Loading devices file %s'...", _fileName.c_str());

        tinyxml2::XMLNode* rootNode = doc.FirstChild();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node!", _fileName.c_str());
            return false;
        }

        rootNode = rootNode->NextSibling();
        if (!rootNode)
        {
            MAZE_ERROR("File '%s' loading error - empty root node children!", _fileName.c_str());
            return false;
        }


        tinyxml2::XMLNode* groupNode = rootNode->FirstChild();
        while (groupNode)
        {
            tinyxml2::XMLElement* groupElement = groupNode->ToElement();

            if (!groupElement)
            {
                groupNode = groupNode->NextSibling();
                continue;
            }

            PlayerGamepadDeviceType deviceType = static_cast<PlayerGamepadDeviceType>(groupElement->IntAttribute("type"));
            if (deviceType == m_deviceType)
            {
                if (deviceType == PlayerGamepadDeviceType::KeyboardMouse)
                {
                    bool binded = false;

                    tinyxml2::XMLNode* keyboardMouseNode = groupNode->FirstChild();
                    while (keyboardMouseNode)
                    {
                        tinyxml2::XMLElement* keyboardMouseElement = keyboardMouseNode->ToElement();

                        if (!keyboardMouseElement)
                        {
                            keyboardMouseNode = keyboardMouseNode->NextSibling();
                            continue;
                        }

                        binded = true;

                        resetBinds();

                        tinyxml2::XMLNode* bindNode = keyboardMouseNode->FirstChild();
                        while (bindNode)
                        {
                            tinyxml2::XMLElement* bindElement = bindNode->ToElement();

                            if (!bindElement)
                            {
                                bindNode = bindNode->NextSibling();
                                continue;
                            }

                            if (strcmp( bindElement->Name(), "Bind") == 0)
                            {
                                String id = bindElement->Attribute("id");
                                String input = bindElement->Attribute("input");
                                S32 number = bindElement->IntAttribute("number");

                                PlayerGamepadInput gamepadInput = PlayerGamepadInput::FromString(input);

                                PlayerGamepadDeviceCommand command = PlayerGamepadDeviceCommand::FromString(id);
                                setBind(command, PlayerGamepadBind(gamepadInput, number));
                            }

                            bindNode = bindNode->NextSibling();
                        }

                        keyboardMouseNode = keyboardMouseNode->NextSibling();
                    }

                    return binded;
                }
                else
                if (deviceType == PlayerGamepadDeviceType::Gamepad)
                {
                    GamepadPtr const& gamepad = GamepadManager::GetInstancePtr()->getGamepadById(getDeviceNumber());
        
                    if (!gamepad)
                        return false;

                    tinyxml2::XMLNode* gamepadNode = groupNode->FirstChild();
                    while (gamepadNode)
                    {
                        tinyxml2::XMLElement* gamepadElement = gamepadNode->ToElement();

                        if (!gamepadElement)
                        {
                            gamepadNode = gamepadNode->NextSibling();
                            continue;
                        }

                        CString os = gamepadElement->Attribute("os");
                        if (os)
                        {
                            CString currentOS = MAZE_PLATFORM_SUFFIX_KEBAB_STR;
                            if (strcmp(os, currentOS) != 0)
                            {
                                gamepadNode = gamepadNode->NextSibling();
                                continue;
                            }
                        }

                        S32 vid = gamepadElement->IntAttribute("vid");
                        S32 pid = gamepadElement->IntAttribute("pid");

                        if (gamepad->getVendorId() == vid && gamepad->getProductId() == pid)
                        {
                            resetBinds();

                            tinyxml2::XMLNode* bindNode = gamepadNode->FirstChild();
                            while (bindNode)
                            {
                                tinyxml2::XMLElement* bindElement = bindNode->ToElement();

                                if (!bindElement)
                                {
                                    bindNode = bindNode->NextSibling();
                                    continue;
                                }

                                if (strcmp(bindElement->Name(), "Bind") == 0)
                                {
                                    String id = bindElement->Attribute("id");
                                    String input = bindElement->Attribute("input");
                                    S32 number = bindElement->IntAttribute("number");

                                    PlayerGamepadInput gamepadInput = PlayerGamepadInput::FromString(input);

                                    PlayerGamepadDeviceCommand command = PlayerGamepadDeviceCommand::FromString(id);
                                    setBind(command, PlayerGamepadBind(gamepadInput, number));
                                }

                                bindNode = bindNode->NextSibling();
                            }

                            return true;
                        }

                        gamepadNode = gamepadNode->NextSibling();
                    }
                }

                return false;
            }


            groupNode = groupNode->NextSibling();
        }

        return false;
    }


} // namespace Maze
//////////////////////////////////////////
