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
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/services/MazeLogService.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Profiler
    //
    //////////////////////////////////////////
    bool Profiler::s_currentProfiling = false;
    bool Profiler::s_requiredProfiling = false;
    ThreadLocalPointer<Profiler> Profiler::s_activeProfiler = nullptr;
    Mutex Profiler::s_profilingMutex;
    Vector<Profiler*> Profiler::s_allProfilers;
    Size Profiler::s_currentSampleIndex = 0;

    //////////////////////////////////////////
    Profiler::Profiler(CString _name)
        : m_name(_name)
        , m_parentProfiler(nullptr)
        , m_min(S32_MAX)
        , m_max(0)
        , m_avg(0.0f) 
    {
        m_parentProfiler = s_activeProfiler;

        m_shortName = _name;
        if (m_shortName.size() > 12)
            m_shortName.resize(12);

        s_allProfilers.emplace_back(this);
    }

    //////////////////////////////////////////
    Profiler::~Profiler()
    {
        s_allProfilers.erase(
            std::remove(
                s_allProfilers.begin(),
                s_allProfilers.end(),
                this),
            s_allProfilers.end());
    }

    //////////////////////////////////////////
    void Profiler::start()
    {
        s_activeProfiler = this;

        if (!s_currentProfiling)
            return;

        ProfilerSample& cs = m_samples[s_currentSampleIndex];
        ++cs.callCount;
        cs.startMS = m_timer.getMilliseconds();
    }

    //////////////////////////////////////////
    S32 Profiler::stop()
    {
        s_activeProfiler = m_parentProfiler;

        if (!s_currentProfiling)
            return 0;

        ProfilerSample& cs = m_samples[s_currentSampleIndex];
        cs.endMS = m_timer.getMilliseconds();
        S32 dt = cs.endMS - cs.startMS;
        cs.durationMS += dt;

        return dt;
    }

    //////////////////////////////////////////
    void Profiler::SetProfiling(bool _enabled)
    {
        s_requiredProfiling = _enabled;
    }

    //////////////////////////////////////////
    bool Profiler::GetProfiling()
    {
        return s_requiredProfiling;
    }

    //////////////////////////////////////////
    void Profiler::FinishSample()
    {
        MAZE_MUTEX_SCOPED_LOCK(s_profilingMutex);

        Size profilersCount = s_allProfilers.size();
        
        
        s_currentSampleIndex = (s_currentSampleIndex + 1) % c_samplesCount;
        for (Size i = 0; i < profilersCount; ++i) 
        {
            Profiler* profiler = s_allProfilers[i];
        
            S32 duration = profiler->m_samples[0].durationMS; 
            S32 sum = duration;
            S32 min = duration;
            S32 max = duration;
            for (Size j = 1; j < c_samplesCount; ++j)
            {
                duration = profiler->m_samples[j].durationMS;
                sum += duration;
                min = Math::Min(min, duration);
                max = Math::Max(max, duration);
            }
            profiler->m_min = min;
            profiler->m_max = max;
            profiler->m_avg = (F32)sum / c_samplesCount;
            
            profiler->m_samples[s_currentSampleIndex].callCount = 0;
            profiler->m_samples[s_currentSampleIndex].durationMS = 0;
        }

        if (s_requiredProfiling != s_currentProfiling)
        {
            s_currentProfiling = s_requiredProfiling;
        }
    }

} // namespace Maze
//////////////////////////////////////////
