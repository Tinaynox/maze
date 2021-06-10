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
#if (!defined(_MazeThreadUnix_hpp_))
#define _MazeThreadUnix_hpp_


//////////////////////////////////////////
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeThreadEntryPoint.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/system/MazeThreadPriority.hpp"
#include <pthread.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ThreadUnix
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ThreadUnix
    {
    public:
        
        //////////////////////////////////////////
        ThreadUnix();
        
        //////////////////////////////////////////
        ~ThreadUnix();
        
        //////////////////////////////////////////
        ThreadUnix(Delegate<S32> const& _entryPoint)
            : ThreadUnix()
        {
            m_entryPoint = std::make_shared<ThreadEntryPoint0>(_entryPoint);
        }
        
        //////////////////////////////////////////
        template <typename TArg0>
        ThreadUnix(Delegate<S32> const& _entryPoint, TArg0 _arg0)
            : ThreadUnix()
        {
            m_entryPoint = std::make_shared<ThreadEntryPoint1>(_entryPoint, _arg0);
        }
        
        //////////////////////////////////////////
        template <typename TArg0, typename TArg1>
        ThreadUnix(Delegate<S32> const& _entryPoint, TArg0 _arg0, TArg1 _arg1)
            : ThreadUnix()
        {
            m_entryPoint = std::make_shared<ThreadEntryPoint2>(_entryPoint, _arg0, _arg1);
        }
        
        
        //////////////////////////////////////////
        ThreadUnix(ThreadUnix const&) = delete;
        
        //////////////////////////////////////////
        ThreadUnix(ThreadUnix&&) = delete;
        
        //////////////////////////////////////////
        ThreadUnix& operator=(ThreadUnix const&) = delete;
        
        //////////////////////////////////////////
        ThreadUnix& operator=(ThreadUnix&&) = delete;
        
        
        //////////////////////////////////////////
        bool run();
        
        //////////////////////////////////////////
        void terminate();
        
        //////////////////////////////////////////
        void wait();
        
        //////////////////////////////////////////
        bool isRunning();
        
        ////////////////////////////////////
        void setPriority(ThreadPriority _priority);
        
        //////////////////////////////////////////
        ThreadPriority getPriority() const;
        
    protected:
        
        
        //////////////////////////////////////////
        static void* EntryPoint(void* _userData);
        
    protected:
        SharedPtr<ThreadEntryPoint> m_entryPoint;
        Mutex m_mutex;

        
    protected:
        pthread_t m_thread;
        bool      m_isActive;
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeThreadUnix_hpp_
//////////////////////////////////////////
