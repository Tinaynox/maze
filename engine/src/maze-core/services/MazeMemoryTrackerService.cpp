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
#include "maze-core/services/MazeMemoryTrackerService.hpp"
#include "maze-core/services/MazeLogService.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static bool s_trackingEnabled = false;
    static Mutex s_memoryTrackerServiceMutex;
    static MemoryTrackerService::AllocationMap s_allocationMap;
    static Size s_totalAllocatedMemorySize = 0u;


    //////////////////////////////////////////
    // Class MemoryTrackerService
    //
    //////////////////////////////////////////
    void* MemoryTrackerService::TrackAlloc(
        void* _ptr,
        Size _size,
        CString _type,
        CString _file,
        Size _line,
        CString _func)
    {
        if (!s_trackingEnabled)
            return _ptr;

        MAZE_MUTEX_SCOPED_LOCK(s_memoryTrackerServiceMutex);

        MAZE_ERROR_IF(!_type, "Allocation type is null!");
        MAZE_ERROR_IF(s_allocationMap.find(_ptr) != s_allocationMap.end(), "Double allocation!");

        s_allocationMap.emplace(_ptr, MemoryTrackerServiceAllocationData(_size, _type, _file, _line, _func));
        s_totalAllocatedMemorySize += _size;

        return _ptr;
    }

    //////////////////////////////////////////
    void MemoryTrackerService::TrackDealloc(void* _ptr)
    {
        if (!_ptr)
            return;

        if (!s_trackingEnabled)
            return;

        MAZE_MUTEX_SCOPED_LOCK(s_memoryTrackerServiceMutex);

        auto it = s_allocationMap.find(_ptr);

        MAZE_ERROR_RETURN_IF(it == s_allocationMap.end(), "This memory was not allocated via MAZE_NEW!");
        s_totalAllocatedMemorySize -= it->second.bytes;
        s_allocationMap.erase(it);
    }

    //////////////////////////////////////////
    void MemoryTrackerService::StartMemoryTracking()
    {
        MAZE_MUTEX_SCOPED_LOCK(s_memoryTrackerServiceMutex);

        MAZE_ERROR_IF(s_trackingEnabled, "Tracking is already enabled!");

        s_trackingEnabled = true;
    }

    //////////////////////////////////////////
    void MemoryTrackerService::StopMemoryTracking()
    {
        MAZE_MUTEX_SCOPED_LOCK(s_memoryTrackerServiceMutex);

        MAZE_ERROR_IF(!s_trackingEnabled, "Tracking is not enabled!");

        s_trackingEnabled = false;
    }

    //////////////////////////////////////////
    StdString MemoryTrackerService::DumpCurrentAllocations()
    {
        StdStringStream ss;

        if (!s_allocationMap.empty())
        {
            ss << "Memory Tracker: (" << s_allocationMap.size() << ") Allocation(s) with total " << s_totalAllocatedMemorySize << " bytes:" << std::endl;

            for (AllocationMap::const_iterator it = s_allocationMap.begin();
                                               it != s_allocationMap.end();
                                               ++it)
            {
                MemoryTrackerServiceAllocationData const& alloc = it->second;

                if (alloc.file)
                    ss << alloc.file;
                else
                    ss << "(unknown source):";

                ss << "(" << alloc.line << "):\n    [" << alloc.type << "](" << alloc.bytes << " bytes)" << " function: " << alloc.function << std::endl;

            }
            ss << std::endl;
        }

        return ss.str();
    }

    //////////////////////////////////////////
    void MemoryTrackerService::LogCurrentAllocationsByType()
    {
        StdVector<StdPair<StdString, Size>> allocationByType;

        for (AllocationMap::const_iterator it = s_allocationMap.begin(),
                                           end = s_allocationMap.end();
                                           it != end;
                                           ++it)
        {
            MemoryTrackerServiceAllocationData const& alloc = it->second;
            CString allocType = alloc.type;
            Size allocBytes = alloc.bytes;

            StdVector<StdPair<StdString, Size>>::iterator it2 = std::find_if(
                allocationByType.begin(),
                allocationByType.end(),
                [allocType](const StdPair< StdString, Size >& p) -> bool
                {
                    return p.first == allocType;
                });

            if (it2 != allocationByType.end())
                it2->second += allocBytes;
            else
                allocationByType.push_back(std::make_pair(allocType, allocBytes));
        }

        std::sort(
            allocationByType.begin(),
            allocationByType.end(),
            [](StdPair<StdString, Size> const& _p0, StdPair<StdString, Size> const& _p1) -> bool
            {
                return _p0.second < _p1.second;
            });

        for (StdVector<StdPair<StdString, Size>>::const_iterator it = allocationByType.begin(),
                                                                 end = allocationByType.end();
                                                                 it != end;
                                                                 ++it)
        {
            Debug::Log("=> %s = %u (%.1fMB)", it->first.c_str(), it->second, (F32)it->second / (1024.0f * 1024.0f));
        }
        Debug::Log("TOTAL ALLOCATED MEMORY SIZE: %u (%.1fMB)", s_totalAllocatedMemorySize, (F32)s_totalAllocatedMemorySize / (1024.0f * 1024.0f));
    }
    
} // namespace Maze 
//////////////////////////////////////////
