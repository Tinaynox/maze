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
#include "MazeCoreHeader.hpp"
#include "maze-core/system/std/MazeThread_std.hpp"
#include <thread>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Thread_std
    //
    //////////////////////////////////////////
    Thread_std::Thread_std()
    {
    }

    //////////////////////////////////////////
    bool Thread_std::run()
    {
        if (!m_entryPoint)
        {
            MAZE_TODO;
            return false;
        }

        if (m_thread)
        {
            MAZE_TODO;
            return false;
        }

        m_thread = MakeUnique<std::thread>(&Thread_std::entryPoint, this);

        if (!m_thread)
            return false;

        return true;
    }

    //////////////////////////////////////////
    void Thread_std::terminate()
    {
        if (m_thread)
        {
            m_thread.reset();
        }
    }

    //////////////////////////////////////////
    void Thread_std::wait()
    {
        if (m_thread)
        {
            m_thread->join();
        }
    }

    //////////////////////////////////////////
    bool Thread_std::isRunning()
    {
        return (m_thread != 0);
    }

    //////////////////////////////////////////
    void Thread_std::setPriority(ThreadPriority _priority)
    {
        if (!m_thread)
            return;

    }

    //////////////////////////////////////////
    ThreadPriority Thread_std::getPriority() const
    {
        return ThreadPriority::None;
    }


    //////////////////////////////////////////
    void Thread_std::entryPoint()
    {    
        m_entryPoint->run();
    }

} // namespace Maze
//////////////////////////////////////////
