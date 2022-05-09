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
#include "MazeConsoleHeader.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-plugin-console/MazeConsoleService.hpp"
#include "maze-plugin-console/settings/MazeConsoleSettings.hpp"
#include "maze-plugin-console/scene/MazeSceneConsole.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ConsoleService
    //
    //////////////////////////////////////////
    ConsoleService::ConsoleService()
    {
        
    }

    //////////////////////////////////////////
    ConsoleService::~ConsoleService()
    {
        LogService::GetInstancePtr()->eventLog.unsubscribe(this);
    }

    //////////////////////////////////////////
    void ConsoleService::initialize()
    {
        MAZE_ERROR_RETURN_IF(!SettingsManager::GetInstancePtr(), "SettingsManager is null!");
        MAZE_ERROR_RETURN_IF(!SceneManager::GetInstancePtr(), "SceneManager is null!");
        MAZE_ERROR_RETURN_IF(!GraphicsManager::GetInstancePtr(), "GraphicsManager is null!");
        MAZE_ERROR_RETURN_IF(!InputManager::GetInstancePtr(), "InputManager is null!");

        SettingsManager::GetInstancePtr()->registerSettings<ConsoleSettings>();
        ConsoleSettings* consoleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ConsoleSettings>();
        consoleSettings->getActiveChangedEvent().subscribe(this, &ConsoleService::notifyConsoleActiveChanged);

        if (GraphicsManager::GetInstancePtr())
        {
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemWillBeChanged.subscribe(this, &ConsoleService::notifyDefaultRenderSystemWillBeChanged);
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.subscribe(this, &ConsoleService::notifyDefaultRenderSystemChanged);

            if (GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw())
                setRenderSystem(GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw());
        }

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &ConsoleService::notifyKeyboard);

        updateActive();

        LogService::GetInstancePtr()->eventLog.subscribe(this, &ConsoleService::notifyLog);
    }

    //////////////////////////////////////////
    void ConsoleService::shutdown()
    {
        unloadScene();

        if (SettingsManager::GetInstancePtr())
        {
            ConsoleSettings* consoleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ConsoleSettings>();
            consoleSettings->getActiveChangedEvent().unsubscribe(this);

            SettingsManager::GetInstancePtr()->clearSettings<ConsoleSettings>();
        }

        setRenderSystem(nullptr);
        setRenderWindow(nullptr);
        
        if (GraphicsManager::GetInstancePtr())
        {
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemWillBeChanged.unsubscribe(this);
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.unsubscribe(this);
        }

        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);

        LogService::GetInstancePtr()->eventLog.unsubscribe(this);
    }

    //////////////////////////////////////////
    void ConsoleService::setRenderSystem(RenderSystem* _renderSystem)
    {
        if (m_renderSystem == _renderSystem)
            return;

        setRenderWindow(nullptr);

        if (m_renderSystem)
            m_renderSystem->eventRenderWindowsChanged.unsubscribe(this);

        m_renderSystem = _renderSystem;

        if (m_renderSystem)
        {
            m_renderSystem->eventRenderWindowsChanged.subscribe(this, &ConsoleService::notifyRenderSystemRenderWindowsChanged);
            notifyRenderSystemRenderWindowsChanged();
        }
    }

    //////////////////////////////////////////
    void ConsoleService::setRenderWindow(RenderWindow* _renderWindow)
    {
        if (m_renderWindow == _renderWindow)
            return;

        if (m_renderWindow)
        {
            if (m_renderWindow->getWindow())
                m_renderWindow->getWindow()->eventWindowWillClose.unsubscribe(this);

            unloadScene();
        }

        m_renderWindow = _renderWindow;

        if (m_renderWindow)
        {
            if (m_renderWindow->getWindow())
                m_renderWindow->getWindow()->eventWindowWillClose.subscribe(this, &ConsoleService::notifyRenderWindowWillClose);
        }

        updateActive();
    }

    //////////////////////////////////////////
    void ConsoleService::notifyDefaultRenderSystemChanged(RenderSystemPtr const& _renderSystem)
    {
        setRenderSystem(_renderSystem.get());
    }

    //////////////////////////////////////////
    void ConsoleService::notifyDefaultRenderSystemWillBeChanged(RenderSystemPtr const& _renderSystem)
    {
        setRenderSystem(nullptr);
    }

    //////////////////////////////////////////
    void ConsoleService::notifyRenderSystemRenderWindowsChanged()
    {
        if (m_renderWindow)
            return;

        Set<RenderWindow*> const& renderWindows = m_renderSystem->getRenderWindows();
        if (renderWindows.empty())
            return;

        for (RenderWindow* renderWindow : renderWindows)
        {
            if (renderWindow->getWindow() && renderWindow->getWindow()->isOpened())
            {
                setRenderWindow(renderWindow);
                break;
            }
        }
    }

    //////////////////////////////////////////
    void ConsoleService::notifyRenderWindowWillClose(Window* _window)
    {
        setRenderWindow(nullptr);
    }

    //////////////////////////////////////////
    void ConsoleService::notifyConsoleActiveChanged(bool const& _active)
    {
        updateActive();
    }

    //////////////////////////////////////////
    void ConsoleService::updateActive()
    {
        if (!m_renderWindow)
            return;

        ConsoleSettings* consoleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ConsoleSettings>();
        bool active = consoleSettings->getActive();

        if (active)
            loadScene();
        else
            unloadScene();
    }

    //////////////////////////////////////////
    void ConsoleService::loadScene()
    {
        SceneConsolePtr scene = SceneManager::GetInstancePtr()->getScene<SceneConsole>();
        if (!scene)
            scene = SceneManager::GetInstancePtr()->loadScene<SceneConsole>(true, m_renderWindow->cast<RenderTarget>());
    }

    //////////////////////////////////////////
    void ConsoleService::unloadScene()
    {
        if (!SceneManager::GetInstancePtr())
            return;

        SceneConsolePtr scene = SceneManager::GetInstancePtr()->getScene<SceneConsole>();
        if (scene)
            SceneManager::GetInstancePtr()->destroyScene(scene);
    }

    //////////////////////////////////////////
    void ConsoleService::notifyKeyboard(InputEventKeyboardData const& _keyboardData)
    {
        switch (_keyboardData.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                if (_keyboardData.keyCode == KeyCode::Grave)
                {
                    if (SettingsManager::GetInstancePtr())
                    {
                        ConsoleSettings* consoleSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<ConsoleSettings>();
                        consoleSettings->switchActive();
                    }
                }

                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void ConsoleService::notifyLog(S32 _priority, CString _text, Size _size)
    {
        m_log.append(_text, _text + _size);

        S32 const c_logSymbolsCount = 2048 * 2;
        if (m_log.size() > c_logSymbolsCount)
            m_log = m_log.substr(m_log.size() - c_logSymbolsCount, c_logSymbolsCount);

        eventLogChanged(m_log);
    }

    //////////////////////////////////////////
    void ConsoleService::registerCommand(
        HashedCString _command,
        Delegate<bool, String*, S32> const& _callback,
        S32 _argsCount)
    {
        ConsoleCommand command;
        command.command = _command;
        command.callback = _callback;
        command.argsCount = _argsCount;
        m_commands.insert(_command, command);
    }

    //////////////////////////////////////////
    bool ConsoleService::hasCommand(HashedCString _command)
    {
        return m_commands.find(_command) != m_commands.end();
    }

    //////////////////////////////////////////
    void ConsoleService::executeCommand(HashedCString _command, String* _argv, S32 _argc)
    {
        auto it = m_commands.find(_command);
        if (it != m_commands.end())
        {
            String lastCommand = _command.str;
            for (S32 i = 0; i < _argc; ++i)
                lastCommand += ' ' + _argv[i];

            Debug::Log("%s", lastCommand.c_str());

            ConsoleCommand const& command = it->second;
            bool result = command.callback(_argv, _argc);

            if (m_lastCommands.empty() || m_lastCommands.back() != lastCommand)
            {
                m_lastCommands.push_back(lastCommand);

                if (m_lastCommands.size() >= 16)
                    m_lastCommands.pop_front();
            }

            if (!result)
                Debug::LogWarning("Command %s failed!", _command.str);
            
        }
    }

    //////////////////////////////////////////
    Vector<ConsoleCommand> ConsoleService::getCommandsStartedWith(String const& _text)
    {
        Vector<ConsoleCommand> result;

        for (auto const& commandData : m_commands)
        {
            if (StringHelper::IsStartsWith(commandData.first.c_str(), _text.c_str()))
                result.emplace_back(commandData.second);
        }

        return result;
    }

    //////////////////////////////////////////
    String const& ConsoleService::getLastCommand(S32 _num)
    {
        static String nullString;

        if (m_lastCommands.empty())
            return nullString;

        return m_lastCommands.at(Math::Clamp((S32)m_lastCommands.size() - _num - 1, 0, (S32)m_lastCommands.size() - 1));
    }

} // namespace Maze
//////////////////////////////////////////
