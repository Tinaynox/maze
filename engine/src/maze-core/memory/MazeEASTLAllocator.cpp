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
#include "maze-core/MazeBaseTypes.hpp"
#include <EASTL/allocator.h>
#include <cstddef>


//////////////////////////////////////////
// EASTL's default allocator (EASTLAllocatorType = eastl::allocator, used by
// Maze::String/Vector/Map/etc in MazeTypes.hpp) requires the application to
// define these two operator new[] overloads - see EASTL/allocator.h. They are
// global (not in namespace Maze) because that's the signature EASTL declares
// and links against.
//
// eastl::allocator::deallocate always frees with a plain "delete[]" regardless
// of which overload produced the memory, so both overloads must hand out
// memory that a plain delete[] can free - i.e. plain "new", not NedMemoryAllocator
// (which pairs nedmemalign with nedfree, not delete[]).
//////////////////////////////////////////
void* operator new[](size_t _size, const char*, int, unsigned, const char*, int)
{
    return new Maze::U8[_size];
}

//////////////////////////////////////////
void* operator new[](size_t _size, size_t _alignment, size_t, const char*, int, unsigned, const char*, int)
{
    // Plain new/delete[] can't honor an aligned-alloc/aligned-free pairing, so we
    // can only guarantee whatever alignment ::operator new already provides
    // (alignof(std::max_align_t), typically 16 bytes on 64-bit platforms - enough
    // for SSE/vecmath types). Anything stricter than that is not supported here.
    MAZE_BP_IF(_alignment > alignof(std::max_align_t));
    return new Maze::U8[_size];
}
