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
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class UpdateManager
    //
    //////////////////////////////////////////
    UpdateManager* UpdateManager::s_instance = nullptr;

    //////////////////////////////////////////
    UpdateManager::UpdateManager()
        : m_lastFrameTimeMS(0u)
        , m_maxDeltaTimeMS(50)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    UpdateManager::~UpdateManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void UpdateManager::Initialize(UpdateManagerPtr& _updateManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(UpdateManager, _updateManager, init());
    }

    //////////////////////////////////////////
    bool UpdateManager::init()
    {
        m_lastFrameTimeMS = m_timer.getMilliseconds();

        addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void UpdateManager::update(F32 _dt)
    {

        
    }

    //////////////////////////////////////////
    void UpdateManager::processUpdate()
    {
        U32 currentFrameTimeMS = m_timer.getMilliseconds();
        if (m_lastFrameTimeMS > currentFrameTimeMS)
        {
            MAZE_WARNING("Timer is broken! currentFrameTime=%u m_lastFrameTime=%u", currentFrameTimeMS, m_lastFrameTimeMS);
            m_lastFrameTimeMS = currentFrameTimeMS;
        }

        // Skip frame
        if (m_lastFrameTimeMS == currentFrameTimeMS)
            return;

        U32 dtMS = Math::Min((currentFrameTimeMS - m_lastFrameTimeMS), m_maxDeltaTimeMS);

        m_lastFrameTimeMS = currentFrameTimeMS;

        F32 dt = dtMS / 1000.0f;

        Updater::processUpdate(dt);
    }

} // namespace Maze
//////////////////////////////////////////
