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
#include "maze-core/MazeTypes.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMat3F32.hpp"
#include "maze-core/math/MazeMat4F32.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/system/MazeThread.hpp"
#include "maze-core/system/std/MazeThread_std.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazePlatformHelper.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"


//////////////////////////////////////////
Maze::SystemManagerPtr g_systemManager;
Maze::InputManagerPtr g_inputManager;
Maze::WindowManagerPtr g_windowManager;

Maze::WindowPtr g_window0;
Maze::WindowPtr g_window1;


//////////////////////////////////////////
void OnInit()
{
    Maze::Debug::log << "OnInit..." << Maze::endl;


    Maze::Debug::log << "InputManager Initialization..." << Maze::endl;
    Maze::InputManager::Initialize(g_inputManager);

    Maze::Debug::log << "WindowManager Initialization..." << Maze::endl;
    Maze::WindowManager::Initialize(g_windowManager);

    Maze::LogService::GetInstancePtr()->splitAndLog(Maze::PlatformHelper::ConstructApplicationInfo());
    Maze::LogService::GetInstancePtr()->splitAndLog(Maze::PlatformHelper::ConstructEngineInfo());
    Maze::LogService::GetInstancePtr()->splitAndLog(Maze::SystemHelper::ConstructSystemInfo());
    Maze::LogService::GetInstancePtr()->splitAndLog(g_windowManager->constructDisplaysInfo());

    Maze::WindowParamsPtr window0Params = Maze::WindowParams::Create();
    window0Params->flags |= Maze::WindowStyleFlags::MinimizeButton;
    window0Params->flags |= Maze::WindowStyleFlags::MaximizeButton;
    
    window0Params->title = "MinMax";
    g_window0 = Maze::Window::Create(window0Params);
    if (g_window0)
    {
        Maze::Debug::log << "Window0 created!" << Maze::endl;
        g_window0->setPosition(Maze::Vec2U32(0, 0));
    }
    else
    {
        Maze::Debug::log << "Window0 was failed to create!" << Maze::endl;
    }


    Maze::WindowParamsPtr window1Params = Maze::WindowParams::Create();
    window1Params->flags &= ~Maze::WindowStyleFlags::Background;
    window1Params->flags |= Maze::WindowStyleFlags::Resizable;
    window1Params->title = "NoBackground|Resizable";
    g_window1 = Maze::Window::Create(window1Params);
    if (g_window1)
    {
        Maze::Debug::log << "Window1 created!" << Maze::endl;

        Maze::Vec2U32 const& fullSize = g_window1->getFullSize();

        Maze::DisplayPtr const& display = g_window1->getRelatedDisplay();
        if (display)
        {
            Maze::WindowVideoMode videoMode = display->getCurrentMode();

            g_window1->setPosition(Maze::Vec2S32((Maze::S32)videoMode.width - (Maze::S32)fullSize.x, (Maze::S32)videoMode.height - (Maze::S32)fullSize.y));
        }
    }
    else
    {
        Maze::Debug::log << "Window1 was failed to create!" << Maze::endl;
    }

    Maze::LogService::GetInstancePtr()->splitAndLog(g_windowManager->constructWindowsInfo());
}

//////////////////////////////////////////
void OnFrame()
{
    Maze::UpdateManager::GetInstancePtr()->processUpdate();
}

//////////////////////////////////////////
void OnShutdown()
{

}


//////////////////////////////////////////
Maze::S32 main(Maze::S32 _argc, Maze::S8 const* _argv[])
{

    for (Maze::S32 i = 0; i < _argc; ++i)
    {
        Maze::Debug::log << i << " -> " << _argv[i] << Maze::endl;
    }

    Maze::Vector<Maze::S8 const*> commandLineArguments;
    for (Maze::S32 i = 0; i < _argc; ++i)
        commandLineArguments.emplace_back(_argv[i]);


    Maze::Debug::log << "SystemManager Initialization..." << Maze::endl;
    Maze::SystemManager::Initialize(g_systemManager, commandLineArguments);

    g_systemManager->eventApplicationInit.subscribe(&OnInit);
    g_systemManager->eventApplicationFrame.subscribe(&OnFrame);

    Maze::Debug::log << "Application Initialization..." << Maze::endl;
    g_systemManager->initApplication();

    while (true)
    {
        g_systemManager->processApplicationFrame();
    }

    std::cin.get();

    return 0;
}
