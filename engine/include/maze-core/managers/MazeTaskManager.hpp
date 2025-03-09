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
#if (!defined(_MazeTaskManager_hpp_))
#define _MazeTaskManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeSwitchableContainer.hpp"
#include "maze-core/system/MazeTaskDelegate.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include <thread>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TaskManager);


    //////////////////////////////////////////
    // Class TaskManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API TaskManager
        : public Updatable
    {
        //////////////////////////////////////////
        struct DelayedTask
        {
        public:

            //////////////////////////////////////////
            DelayedTask() = default;

            //////////////////////////////////////////
            DelayedTask(TaskDelegate* _delegate, S32 _framesDelay)
                : task(_delegate)
                , framesDelay(_framesDelay)
            {}

            SharedPtr<TaskDelegate> task;
            S32 framesDelay = 0;
        };

    public:

        //////////////////////////////////////////
        virtual ~TaskManager();

        //////////////////////////////////////////
        static void Initialize(
            TaskManagerPtr& _eventManager,
            DataBlock const& _config = DataBlock::c_empty);

        
        //////////////////////////////////////////
        static inline TaskManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline TaskManager& GetInstance() { return *s_instance; }

        //////////////////////////////////////////
        static bool IsMainThread() { return s_mainThreadId == std::this_thread::get_id(); }


        //////////////////////////////////////////
        inline void addMainThreadTask(Delegate<void> const& _delegate)
        {
            MAZE_MUTEX_SCOPED_LOCK(m_mainThreadTasksMutex);
            m_mainThreadTasks.current().emplace_back(
                new TaskDelegate0{ [_delegate]() { _delegate(); return 0; } });
        }

        //////////////////////////////////////////
        template <typename TArg0>
        inline void addMainThreadTask(Delegate<void> const& _delegate, TArg0 const& _arg0)
        {
            MAZE_MUTEX_SCOPED_LOCK(m_mainThreadTasksMutex);
            m_mainThreadTasks.current().emplace_back(
                new TaskDelegate1<TArg0>{ [_delegate, _arg0]() { _delegate(_arg0); return 0; }, _arg0 });
        }

        //////////////////////////////////////////
        template <typename TArg0, typename TArg1>
        inline void addMainThreadTask(Delegate<void> const& _delegate, TArg0 const& _arg0, TArg0 const& _arg1)
        {
            MAZE_MUTEX_SCOPED_LOCK(m_mainThreadTasksMutex);
            m_mainThreadTasks.current().emplace_back(
                new TaskDelegate1<TArg0>{ [_delegate, _arg0, _arg1]() { _delegate(_arg0, _arg1); return 0; }, _arg0, _arg1 });
        }


        //////////////////////////////////////////
        inline void addDelayedMainThreadTask(S32 _framesDelay, Delegate<void> const& _delegate)
        {
            m_delayedMainThreadTasks.emplace_back(
                DelayedTask{ new TaskDelegate0{ [_delegate]() { _delegate(); return 0; } }, _framesDelay });
        }

        //////////////////////////////////////////
        template <typename TArg0>
        inline void addDelayedMainThreadTask(S32 _framesDelay, Delegate<void> const& _delegate, TArg0 const& _arg0)
        {
            m_delayedMainThreadTasks.emplace_back(
                DelayedTask{ new TaskDelegate1<TArg0>{ [_delegate, _arg0]() { _delegate(_arg0); return 0; }, _arg0 }, _framesDelay });
        }

        //////////////////////////////////////////
        template <typename TArg0, typename TArg1>
        inline void addDelayedMainThreadTask(S32 _framesDelay, Delegate<void> const& _delegate, TArg0 const& _arg0, TArg0 const& _arg1)
        {
            m_delayedMainThreadTasks.emplace_back(
                DelayedTask{ new TaskDelegate1<TArg0>{ [_delegate, _arg0, _arg1]() { _delegate(_arg0, _arg1); return 0; }, _arg0, _arg1 }, _framesDelay });
        }

    protected:

        //////////////////////////////////////////
        TaskManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    private:

        static TaskManager* s_instance;
        static std::thread::id s_mainThreadId;

        Mutex m_mainThreadTasksMutex;
        SwitchableContainer<FastVector<SharedPtr<TaskDelegate>>> m_mainThreadTasks;
        List<DelayedTask> m_delayedMainThreadTasks;

        bool m_update = false;

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTaskManager_hpp_
//////////////////////////////////////////
