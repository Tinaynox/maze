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
#if (!defined(_MazeConsoleService_hpp_))
#define _MazeConsoleService_hpp_


//////////////////////////////////////////
#include "maze-plugin-console/MazeConsoleHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-plugin-console/MazeConsoleCommand.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ConsoleService
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CONSOLE_API ConsoleService
        : public MultiDelegateCallbackReceiver
    {
    public:
        
        //////////////////////////////////////////
        ConsoleService();

        //////////////////////////////////////////
        ~ConsoleService();
        

        //////////////////////////////////////////
        void initialize();

        //////////////////////////////////////////
        void shutdown();


        //////////////////////////////////////////
        static inline ConsoleService& GetInstance()
        {
            static ConsoleService s_logService;
            return s_logService;
        }

        //////////////////////////////////////////
        static inline ConsoleService* GetInstancePtr() { return &GetInstance(); }


        //////////////////////////////////////////
        String const& getLog() const { return m_log; }


        //////////////////////////////////////////
        void registerCommand(
            HashedCString _command,
            Delegate<bool, String*, S32> const& _callback,
            S32 _argsCount = 0);

        //////////////////////////////////////////
        bool hasCommand(HashedCString _command);

        //////////////////////////////////////////
        void executeCommand(HashedCString _command, String* _argv, S32 _argc);

        //////////////////////////////////////////
        Vector<ConsoleCommand> getCommandsStartedWith(String const& _text);



        //////////////////////////////////////////
        String const& getLastCommand(S32 _num);

        //////////////////////////////////////////
        inline Size getLastCommandsSize() const { return m_lastCommands.size(); }

    public:

        //////////////////////////////////////////
        MultiDelegate<String const&> eventLogChanged;

    protected:

        //////////////////////////////////////////
        void setRenderSystem(RenderSystem* _renderSystem);

        //////////////////////////////////////////
        void setRenderWindow(RenderWindow* _renderWindow);

        //////////////////////////////////////////
        void notifyDefaultRenderSystemChanged(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void notifyDefaultRenderSystemWillBeChanged(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void notifyRenderSystemRenderWindowsChanged();

        //////////////////////////////////////////
        void notifyRenderWindowWillClose(Window* _window);

        //////////////////////////////////////////
        void notifyConsoleActiveChanged(bool const& _active);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _keyboardData);

        //////////////////////////////////////////
        void updateActive();

        //////////////////////////////////////////
        void loadScene();

        //////////////////////////////////////////
        void unloadScene();


        //////////////////////////////////////////
        void notifyLog(S32 _priority, CString _text, Size _size);

    protected:
        RenderSystem* m_renderSystem = nullptr;
        RenderWindow* m_renderWindow = nullptr;

        String m_log;

        StringKeyMap<ConsoleCommand> m_commands;


        Deque<String> m_lastCommands;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeConsoleService_hpp_
//////////////////////////////////////////
