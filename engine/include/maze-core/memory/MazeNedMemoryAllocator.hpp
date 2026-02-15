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
#if (!defined(_MazeNedMemoryAllocator_hpp_))
#define _MazeNedMemoryAllocator_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include <limits>
#include <nedmalloc/nedmalloc.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #define MAZE_NEDMALLOC_DEBUG (0)

#if (MAZE_NEDMALLOC_DEBUG)
    //////////////////////////////////////////
    std::set<void*>& GetNedAllocations();
#endif


    //////////////////////////////////////////
    // Class NedMemoryAllocator
    //
    //////////////////////////////////////////
    class MAZE_CORE_API NedMemoryAllocator
    {
    public:

        //////////////////////////////////////////
        static inline void* AllocateBytes(
            Size _count,
#if (MAZE_DEBUG_MEMORY)
            CString _type = nullptr,
            CString _file = nullptr,
            Size _ln = 0,
            CString _func = nullptr
#else
            CString = nullptr,
            CString = nullptr,
            Size = 0,
            CString = nullptr
#endif
        )
        {
            void* ptr = nedalloc::nedmalloc(_count);

#if (MAZE_DEBUG_MEMORY && 0)
            ::Maze::MemoryTrackerService::TrackAlloc(ptr, _count, _type, _file, _ln, _func);
#endif

#if (MAZE_NEDMALLOC_DEBUG)
            GetNedAllocations().insert(ptr);
            MAZE_BP_IF(GetNedAllocations().find(ptr) != GetNedAllocations().end()); // Double allocation!
#endif

            return ptr;
        }

        //////////////////////////////////////////
        static inline void DeallocateBytes(void* _ptr)
        {
            if (!_ptr)
                return;

#if (MAZE_DEBUG_MEMORY && 0)
            ::Maze::MemoryTrackerService::TrackDealloc(_ptr);
#endif

#if (MAZE_NEDMALLOC_DEBUG)
            MAZE_BP_IF(GetNedAllocations().find(_ptr) == GetNedAllocations().end()); // Double deallocation!
            GetNedAllocations().erase(_ptr);
#endif

            nedalloc::nedfree(_ptr);
        }

        //////////////////////////////////////////
        static inline void* AllocateBytesAligned(Size _align, Size _count)
        {
            void* ptr = nedalloc::nedmemalign(_align, _count);
            
#if (MAZE_NEDMALLOC_DEBUG)
            MAZE_BP_IF(GetNedAllocations().find(ptr) != GetNedAllocations().end()); // Double allocation!
            GetNedAllocations().insert(ptr);
#endif

            return ptr;
        }

        //////////////////////////////////////////
        static inline void DeallocateBytesAligned(Size _align, void* _ptr)
        {
            if (!_ptr)
                return;

#if (MAZE_NEDMALLOC_DEBUG)
            MAZE_BP_IF(GetNedAllocations().find(_ptr) == GetNedAllocations().end()); // Double deallocation!
            GetNedAllocations().erase(_ptr);
#endif

            nedalloc::nedfree(_ptr);
        }

        //////////////////////////////////////////
        static inline Size GetMaxAllocationSize()
        {
            return std::numeric_limits<Size>::max();
        }

    private:

        //////////////////////////////////////////
        NedMemoryAllocator();
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeNedMemoryAllocator_hpp_
//////////////////////////////////////////
