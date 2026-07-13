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
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class TaskManager
    //
    //////////////////////////////////////////
    TaskManager* TaskManager::s_instance = nullptr;
    std::thread::id TaskManager::s_mainThreadId = std::thread::id();

    //////////////////////////////////////////
    TaskManager::TaskManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    TaskManager::~TaskManager()
    {
        shutdownBackgroundThread();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void TaskManager::Initialize(
        TaskManagerPtr& _eventManager,
        DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(TaskManager, _eventManager, init(_config));
    }

    //////////////////////////////////////////
    bool TaskManager::init(DataBlock const& _config)
    {
        s_mainThreadId = std::this_thread::get_id();
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void TaskManager::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("TaskManager::update");

        if (m_update)
            return;

        m_update = true;

        FastVector<SharedPtr<TaskDelegate>>* mainThreadTasks;
        
        {
            MAZE_MUTEX_SCOPED_LOCK(m_mainThreadTasksMutex);
            mainThreadTasks = &m_mainThreadTasks.switchContainer();
        }

        for (SharedPtr<TaskDelegate>& mainThreadTask : *mainThreadTasks)
            mainThreadTask->run();

        m_mainThreadTasks.other().clear();

        for (List<DelayedTask>::iterator it = m_delayedMainThreadTasks.begin(),
                                         end = m_delayedMainThreadTasks.end();
                                         it != end;)
        {
            --it->framesDelay;
            if (it->framesDelay <= 0)
            {
                MAZE_MUTEX_SCOPED_LOCK(m_mainThreadTasksMutex);
                m_mainThreadTasks.current().emplace_back(eastl::move(it->task));
                it = m_delayedMainThreadTasks.erase(it);
            }
            else
            {
                ++it;
            }

        }

        m_update = false;
    }

    //////////////////////////////////////////
    bool TaskManager::addBackgroundTask(Delegate<void> const& _delegate)
    {
        {
            std::unique_lock<std::mutex> lock(m_backgroundTasksMutex);

            if (m_backgroundThreadShutdown)
                return false;

            m_backgroundTasks.emplace_back(
                new TaskDelegate0{ [_delegate]() { _delegate(); return 0; } });

            // Lazy start - the thread is spawned on the first background task
            if (!m_backgroundThread.joinable())
                m_backgroundThread = std::thread(&TaskManager::backgroundThreadEntry, this);
        }

        m_backgroundTasksCondVar.notify_one();

        return true;
    }

    //////////////////////////////////////////
    void TaskManager::shutdownBackgroundThread()
    {
        std::thread backgroundThread;

        {
            std::unique_lock<std::mutex> lock(m_backgroundTasksMutex);
            m_backgroundThreadShutdown = true;
            m_backgroundTasks.clear();
            backgroundThread = eastl::move(m_backgroundThread);
        }

        m_backgroundTasksCondVar.notify_all();

        if (backgroundThread.joinable())
            backgroundThread.join();
    }

    //////////////////////////////////////////
    void TaskManager::backgroundThreadEntry()
    {
        MAZE_PROFILE_THREAD("TaskManagerBackground");

        for (;;)
        {
            SharedPtr<TaskDelegate> task;

            {
                std::unique_lock<std::mutex> lock(m_backgroundTasksMutex);
                m_backgroundTasksCondVar.wait(
                    lock,
                    [this]() { return m_backgroundThreadShutdown || !m_backgroundTasks.empty(); });

                if (m_backgroundThreadShutdown)
                    return;

                task = eastl::move(m_backgroundTasks.front());
                m_backgroundTasks.pop_front();
            }

            task->run();
        }
    }

} // namespace Maze
//////////////////////////////////////////
