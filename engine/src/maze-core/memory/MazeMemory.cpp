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
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static StdVector<MemoryAllocatorBase*> s_memoryAllocators;

    //////////////////////////////////////////
    MAZE_CORE_API void RegisterMemoryAllocator(MemoryAllocatorBase* _allocator)
    {
        s_memoryAllocators.push_back(_allocator);
    }

    //////////////////////////////////////////
    MAZE_CORE_API std::string BuildMemoryAllocatorsDebugInfo()
    {
        std::string result;

        StdVector<MemoryAllocatorBase*> memoryAllocators = s_memoryAllocators;

        std::sort(
            memoryAllocators.begin(),
            memoryAllocators.end(),
            [](MemoryAllocatorBase* allocator0, MemoryAllocatorBase* allocator1)
            {
                return allocator0->getAllocatedMemorySize() < allocator1->getAllocatedMemorySize();
            });

        U64 totalMemoryAllocationSize = 0;
        for (MemoryAllocatorBase* allocator : memoryAllocators)
        {
            totalMemoryAllocationSize += allocator->getAllocatedMemorySize();
            result += std::string(allocator->getName()) + " -> " + StringHelper::F32ToStringFormatted((F32)allocator->getAllocatedMemorySize() / (1024*1024)).c_str() + " MB\n";
        }

        result += (std::string)"TOTAL ALLOCATED MEMORY SIZE: " + StringHelper::ToString(totalMemoryAllocationSize).c_str() + "(" + StringHelper::ToString((F64)totalMemoryAllocationSize/(1024.0f*1024.0f)).c_str() + "MB)";

        return result;
    }


} // namespace Maze
//////////////////////////////////////////
