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
#if (!defined(_MazeThread_std_hpp_))
#define _MazeThread_std_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeTaskDelegate.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/system/MazeThreadPriority.hpp"
#include <thread>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Thread_std
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Thread_std
    {
    public:

        //////////////////////////////////////////
        Thread_std();

        //////////////////////////////////////////
        Thread_std(Delegate<S32> const& _entryPoint)
            : Thread_std()
        {
            m_entryPoint = std::make_shared<TaskDelegate0>(_entryPoint);
        }

        //////////////////////////////////////////
        template <typename TArg0>
        Thread_std(Delegate<S32> const& _entryPoint, TArg0 _arg0)
            : Thread_std()
        {
            m_entryPoint = std::make_shared<TaskDelegate1>(_entryPoint, _arg0);
        }

        //////////////////////////////////////////
        template <typename TArg0, typename TArg1>
        Thread_std(Delegate<S32> const& _entryPoint, TArg0 _arg0, TArg1 _arg1)
            : Thread_std()
        {
            m_entryPoint = std::make_shared<TaskDelegate2>(_entryPoint, _arg0, _arg1);
        }


        //////////////////////////////////////////
        Thread_std(Thread_std const&) = delete;
        
        //////////////////////////////////////////
        Thread_std(Thread_std&&) = delete;

        //////////////////////////////////////////
        Thread_std& operator=(Thread_std const&) = delete;
        
        //////////////////////////////////////////
        Thread_std& operator=(Thread_std&&) = delete;


        //////////////////////////////////////////
        bool run();

        //////////////////////////////////////////
        void terminate();

        //////////////////////////////////////////
        void wait();

        //////////////////////////////////////////
        bool isRunning();

        //////////////////////////////////////////
        void setPriority(ThreadPriority _priority);

        //////////////////////////////////////////
        ThreadPriority getPriority() const;

    protected:

        //////////////////////////////////////////
        void entryPoint();

    protected:
        SharedPtr<TaskDelegate> m_entryPoint;
        Mutex m_mutex;
        

    protected:
        SharedPtr<std::thread> m_thread;
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeThread_std_hpp_
//////////////////////////////////////////
