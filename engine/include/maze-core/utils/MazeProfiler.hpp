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
#if (!defined(_MazeProfiler_hpp_))
#define _MazeProfiler_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/system/MazeThreadLocalPointer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #define PROFILER_ENABLED (1)


    //////////////////////////////////////////
    struct ProfilerSample
    {
        S32 callCount;
        S32 startMS;
        S32 endMS;
        S32 durationMS;
    };


    //////////////////////////////////////////
    class Profiler;


    //////////////////////////////////////////
    // Class Profiler
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Profiler
    {
    public:

        //////////////////////////////////////////
        static const Size c_samplesCount = 100;

    public:

        //////////////////////////////////////////
        Profiler(CString _name);

        //////////////////////////////////////////
        ~Profiler();

        //////////////////////////////////////////
        void start();

        //////////////////////////////////////////
        S32 stop();

        //////////////////////////////////////////
        inline CString getName() const { return m_name; }
        
        //////////////////////////////////////////
        inline CString getShortName() const { return m_shortName.c_str(); }

        //////////////////////////////////////////
        inline Profiler* getParentProfiler() const { return m_parentProfiler; }

        //////////////////////////////////////////
        inline S32 getMinDuration() const { return m_min; }

        //////////////////////////////////////////
        inline S32 getMaxDuration() const { return m_max; }

        //////////////////////////////////////////
        inline F32 getAverageDuration() const { return m_avg; }

        //////////////////////////////////////////
        inline ProfilerSample const& getSample(Size _i) const
        {
            return m_samples[(s_currentSampleIndex + _i) % c_samplesCount];
        }

    public:


        //////////////////////////////////////////
        static inline Vector<Profiler*> const& GetAllProfilers()
        {
            return s_allProfilers;
        }

        //////////////////////////////////////////
        static inline void LockProfilingMutex()
        {
            s_profilingMutex.lock();
        }
        
        //////////////////////////////////////////
        static inline void UnlockProfilingMutex()
        {
            s_profilingMutex.unlock();
        }

        //////////////////////////////////////////
        static inline Size GetCurrentSampleIndex()
        {
            return s_currentSampleIndex;
        }


        //////////////////////////////////////////
        static void SetProfiling(bool _enabled);

        //////////////////////////////////////////
        static bool GetProfiling();

        //////////////////////////////////////////
        static void FinishSample();
        

    protected:

        static bool s_currentProfiling;
        static bool s_requiredProfiling;
        static ThreadLocalPointer<Profiler> s_activeProfiler;
        static Mutex s_profilingMutex;
        static Vector<Profiler*> s_allProfilers;
        static Size s_currentSampleIndex;

    protected:
        CString m_name;
        String m_shortName;
        Profiler* m_parentProfiler;

        S32 m_min;
        S32 m_max;
        F32 m_avg;

        ProfilerSample m_samples[c_samplesCount];
        Timer m_timer;
    };


    //////////////////////////////////////////
    // Class ProfilerScope
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ProfilerScope
    {
    public:

        //////////////////////////////////////////
        inline ProfilerScope(Profiler* _profiler)
            : m_profiler(_profiler)
        {
            m_profiler->start();
        }

        //////////////////////////////////////////
        inline ~ProfilerScope()
        {
            m_profiler->stop();
        }
    
        //////////////////////////////////////////
        ProfilerScope(ProfilerScope const&) = delete;

        //////////////////////////////////////////
        ProfilerScope(ProfilerScope&&) = delete;

        //////////////////////////////////////////
        ProfilerScope& operator=(ProfilerScope const&) = delete;

        //////////////////////////////////////////
        ProfilerScope& operator=(ProfilerScope&&) = delete;

    protected:
        Profiler* m_profiler;
    };

    //////////////////////////////////////////
#if (PROFILER_ENABLED)
    #define MAZE_PROFILER_SCOPED_LOCK(__profilerName) \
        static Profiler s_profiler ## __profilerName(#__profilerName); \
        ProfilerScope profiler ## __profilerName ## ScopedLock(&s_profiler ## __profilerName);
#else
    #define MAZE_PROFILER_SCOPED_LOCK(__profilerName)
#endif // PROFILER_ENABLED

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeProfiler_hpp_
