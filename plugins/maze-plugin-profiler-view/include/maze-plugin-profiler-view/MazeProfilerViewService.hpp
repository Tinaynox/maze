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
#if (!defined(_MazeProfilerViewService_hpp_))
#define _MazeProfilerViewService_hpp_


//////////////////////////////////////////
#include "maze-plugin-profiler-view/MazeProfilerViewHeader.hpp"
#include "maze-core/utils/MazeSingleton.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ProfilerViewService
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_PROFILER_VIEW_API ProfilerViewService
        : public Singleton<ProfilerViewService>
        , public MultiDelegateCallbackReceiver
    {
    public:
        
        //////////////////////////////////////////
        ProfilerViewService();

        //////////////////////////////////////////
        ~ProfilerViewService();
        

        //////////////////////////////////////////
        void initialize();

        //////////////////////////////////////////
        void shutdown();

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
        void notifyProfilerViewActiveChanged(bool const& _active);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _keyboardData);

        //////////////////////////////////////////
        void updateActive();

        //////////////////////////////////////////
        void loadScene();

        //////////////////////////////////////////
        void unloadScene();

    protected:
        RenderSystem* m_renderSystem = nullptr;
        RenderWindow* m_renderWindow = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeProfilerViewService_hpp_
//////////////////////////////////////////
