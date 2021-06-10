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
#if (!defined(_MazeStdMemoryAllocator_hpp_))
#define _MazeStdMemoryAllocator_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include <limits>
#include <memory>
#include <nedmalloc/nedmalloc.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class StdMemoryAllocatorBase
    //
    //////////////////////////////////////////
    template <typename T>
    struct StdMemoryAllocatorBase
    {
        using value_type = T;
    };
    
    //////////////////////////////////////////
    // Class StdMemoryAllocatorBase
    //
    //////////////////////////////////////////
    template <typename T>
    struct StdMemoryAllocatorBase<T const>
    {   
        using value_type = T;
    };
    
    
    //////////////////////////////////////////
    // Class StdMemoryAllocator
    //
    //////////////////////////////////////////
    template <
    typename TValue,
    typename TStaticAllocator>
    class StdMemoryAllocator
        : public StdMemoryAllocatorBase<TValue>
    {
    public:
        
        //////////////////////////////////////////
        using Base = StdMemoryAllocatorBase<TValue>;
        using value_type = typename Base::value_type;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = Size;
        using difference_type = std::ptrdiff_t;
        
        
        //////////////////////////////////////////
        template <typename UValue>
        struct rebind
        {
            using other = StdMemoryAllocator<UValue, TStaticAllocator>;
        };
                
        //////////////////////////////////////////
        inline explicit StdMemoryAllocator()
        { }
                
        //////////////////////////////////////////
        virtual ~StdMemoryAllocator()
        { }
                
        //////////////////////////////////////////
        inline StdMemoryAllocator(StdMemoryAllocator const&)
        { }
        
        //////////////////////////////////////////
        template <typename UValue>
        inline StdMemoryAllocator(StdMemoryAllocator<UValue, TStaticAllocator> const&)
        { }
                
        //////////////////////////////////////////
        template <typename UValue, typename P>
        inline StdMemoryAllocator(StdMemoryAllocator<UValue, P> const&)
        { }
        
                //////////////////////////////////////////
        inline pointer allocate(
            size_type _count,
            typename std::allocator<void>::const_pointer _ptr = 0)
        {
            (void)_ptr;
            
            size_type sz = _count * sizeof(TValue);
            
#if (MAZE_DEBUG_MEMORY)
            pointer p  = static_cast<pointer>(
                TStaticAllocator::AllocateBytes(
                    sz,
                    Maze::ClassInfo<StdMemoryAllocator<TValue, TStaticAllocator>>::QualifiedName(),
                    __FILE__,
                    __LINE__,
                    __FUNCTION__));
#else
            pointer p  = static_cast<pointer>(TStaticAllocator::AllocateBytes(sz));
#endif
            return p;
        }
        
        //////////////////////////////////////////
        inline void deallocate(pointer _ptr, size_type)
        {
            TStaticAllocator::DeallocateBytes(_ptr);
        }
        
        //////////////////////////////////////////
        pointer address(reference _x) const
        {
            return &_x;
        }
        
        //////////////////////////////////////////
        const_pointer address(const_reference _x) const
        {
            return &_x;
        }
        
        //////////////////////////////////////////
        size_type max_size() const throw()
        {
            return TStaticAllocator::GetMaxAllocationSize();
        }
        
#if (__cplusplus < 201103L)
        
        //////////////////////////////////////////
        void construct(pointer _p)
        {
            new(static_cast<void*>(_p)) TValue();
        }
#endif
        
        //////////////////////////////////////////
        void construct(pointer _p, TValue const& _val)
        {
            new(static_cast<void*>(_p)) TValue(_val);
        }
        
        //////////////////////////////////////////
        void destroy(pointer _p)
        {
            _p->~TValue();
        }
    };
    
    
    //////////////////////////////////////////
    // Determine equality, can memory from another allocator
    // be released by this allocator, (ISO C++)
    template <typename TValue, typename TValue2, typename P>
    inline bool operator==(
        StdMemoryAllocator<TValue, P> const&,
        StdMemoryAllocator<TValue2, P> const&)
    {
        // Same alloc policy (P), memory can be freed
        return true;
    }
    
    //////////////////////////////////////////
    // Determine equality, can memory from another allocator
    // be released by this allocator, (ISO C++)
    template <typename TValue, typename P, typename TOtherAllocator>
    inline bool operator==(
        StdMemoryAllocator<TValue, P> const&,
        TOtherAllocator const&)
    {
        return false;
    }
    
    
    //////////////////////////////////////////
    // Determine equality, can memory from another allocator
    // be released by this allocator, (ISO C++)
    template <typename TValue, typename TValue2, typename P>
    inline bool operator!=(
        StdMemoryAllocator<TValue, P> const&,
        StdMemoryAllocator<TValue2, P> const&)
    {
        // same alloc policy (P), memory can be freed
        return false;
    }
    
    
    //////////////////////////////////////////
    // Determine equality, can memory from another allocator
    // be released by this allocator, (ISO C++)
    template <typename TValue, typename P, typename TOtherAllocator>
    inline bool operator!=(
        StdMemoryAllocator<TValue, P> const&,
        TOtherAllocator const&)
    {
        return true;
    }

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeStdMemoryAllocator_hpp_
//////////////////////////////////////////
