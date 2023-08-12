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
#if (!defined(_MazeMemoryTrackerService_hpp_))
#define _MazeMemoryTrackerService_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct MemoryTrackerServiceAllocationData
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API MemoryTrackerServiceAllocationData
    {
        //////////////////////////////////////////
        inline MemoryTrackerServiceAllocationData(
            Size _bytes = 0u,
            CString _type = nullptr,
            CString _file = nullptr,
            Size _line = 0u,
            CString _function = nullptr)
            : bytes(_bytes)
            , type(_type)
            , file(_file)
            , line(_line)
            , function(_function)
        {

        }

        Size bytes = 0u;
        CString type = nullptr;
        CString file = nullptr;
        Size line = 0u;
        CString function = nullptr;
    };


    //////////////////////////////////////////
    // Class MemoryTrackerService
    //
    //////////////////////////////////////////
    class MAZE_CORE_API MemoryTrackerService MAZE_FINAL
    {
    public:

        //////////////////////////////////////////
        using AllocationMap = std::map<void const*, MemoryTrackerServiceAllocationData>;
        

        //////////////////////////////////////////
        static void* TrackAlloc(
            void* _ptr,
            Size _size,
            CString _type = nullptr,
            CString _file = nullptr,
            Size _line = 0u,
            CString _func = nullptr);

        //////////////////////////////////////////
        static void TrackDealloc(void* _ptr);

        //////////////////////////////////////////
        static void StartMemoryTracking();

        //////////////////////////////////////////
        static void StopMemoryTracking();

        //////////////////////////////////////////
        static StdString DumpCurrentAllocations();

        //////////////////////////////////////////
        static void LogCurrentAllocationsByType();

    private:

        //////////////////////////////////////////
        MemoryTrackerService() = delete;
        
    protected:
    };
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLogServiceBase_hpp_
//////////////////////////////////////////
