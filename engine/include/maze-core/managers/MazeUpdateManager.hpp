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
#if (!defined(_MazeUpdateManager_hpp_))
#define _MazeUpdateManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeTimer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(UpdateManager);


    //////////////////////////////////////////
    // Class UpdateManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API UpdateManager
        : public Updatable
        , public Updater
    {
    public:

        //////////////////////////////////////////
        virtual ~UpdateManager();

        //////////////////////////////////////////
        static void Initialize(UpdateManagerPtr& _updateManager);
        

        //////////////////////////////////////////
        void processUpdate();
    

        //////////////////////////////////////////
        inline U32 getMilliseconds() { return m_timer.getMilliseconds(); }

        //////////////////////////////////////////
        inline U32 getMicroseconds() { return m_timer.getMicroseconds(); }


        //////////////////////////////////////////
        inline F32 getTimeScale() const { return m_timeScale; }

        //////////////////////////////////////////
        inline void setTimeScale(F32 _timeScale) { m_timeScale = _timeScale; }


        //////////////////////////////////////////
        inline F32 getAppTime() const { return m_appTime; }

        //////////////////////////////////////////
        inline void pushPause() { m_pauseCounter++; }

        //////////////////////////////////////////
        inline void popPause() { if (m_pauseCounter > 0) m_pauseCounter--; }

        //////////////////////////////////////////
        inline bool isPaused() const { return (m_pauseCounter > 0); }


        //////////////////////////////////////////
        inline F32 getUnscaledDeltaTime() const { return m_unscaledDeltaTime; }

        //////////////////////////////////////////
        inline F32 getDeltaTime() const { return m_deltaTime; }


        //////////////////////////////////////////
        void processStep(F32 _dt);


        //////////////////////////////////////////
        static inline UpdateManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline UpdateManager& GetInstance() { return *s_instance; }

    protected:

        //////////////////////////////////////////
        UpdateManager();

        //////////////////////////////////////////
        virtual bool init();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    private:
        static UpdateManager* s_instance;

        Timer m_timer;

        U32 m_lastFrameTimeMS = 0u;
        U32 m_maxDeltaTimeMS = 50u;

        F32 m_timeScale = 1.0f;
        S32 m_pauseCounter = 0;

        F32 m_appTime = 0.0f;

        F32 m_unscaledDeltaTime = 0.0f;
        F32 m_deltaTime = 0.0f;
    };


    //////////////////////////////////////////
    // Class ScopedPause
    //
    //////////////////////////////////////////
    class ScopedPause
    {
    public:

        //////////////////////////////////////////
        ScopedPause(bool _value = true)
            : m_pause(_value)
        {
            if (m_pause)
                UpdateManager::GetInstancePtr()->pushPause();
        }

        //////////////////////////////////////////
        ~ScopedPause()
        {
            if (m_pause)
                UpdateManager::GetInstancePtr()->popPause();
        }

    private:
        bool m_pause = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUpdateManager_hpp_
//////////////////////////////////////////
