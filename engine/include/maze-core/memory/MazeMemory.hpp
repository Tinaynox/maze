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
#if (!defined(_MazeMemory_hpp_))
#define _MazeMemory_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/memory/MazeMemoryAllocatorBase.hpp"
#include "maze-core/memory/MazeBlockMemoryAllocator.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include <string>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_IOS)
#    define MAZE_BLOCK_MEMORY_ALLOCATION_ENABLED (0)
#else
#    define MAZE_BLOCK_MEMORY_ALLOCATION_ENABLED (1)
#endif


    //////////////////////////////////////////
    template <class _Ty>
    struct DefaultDelete
    {    
        //////////////////////////////////////////
        MAZE_CONSTEXPR DefaultDelete() noexcept = default;

        //////////////////////////////////////////
        template <
            class _Ty2,
            typename std::enable_if<std::is_convertible<_Ty2 *, _Ty *>::value, int>::type = 0>
        DefaultDelete(const DefaultDelete<_Ty2>&) noexcept
        {
        }

        //////////////////////////////////////////
        void operator()(_Ty* _Ptr) const noexcept
        {
            static_assert(0 < sizeof(_Ty), "can't delete an incomplete type");
            MAZE_DELETE(_Ptr);
        }
    };


    //////////////////////////////////////////
    template <class _Ty>
    struct DefaultDelete<_Ty[]>
    {    
        //////////////////////////////////////////
        MAZE_CONSTEXPR DefaultDelete() noexcept = default;

        //////////////////////////////////////////
        template <
            class _Uty,
            typename std::enable_if<std::is_convertible<_Uty(*)[], _Ty(*)[]>::value, int>::type = 0>
        DefaultDelete(const DefaultDelete<_Uty[]>&) noexcept
        {
        }

        //////////////////////////////////////////
        template <
            class _Uty,
            typename std::enable_if<std::is_convertible<_Uty(*)[], _Ty(*)[]>::value, int>::type = 0>
        void operator()(_Uty* _Ptr) const noexcept
        {
            static_assert(0 < sizeof(_Uty), "can't delete an incomplete type");
            MAZE_DELETE_ARRAY(_Ptr);
        }
    };


    //////////////////////////////////////////
    MAZE_CORE_API void RegisterMemoryAllocator(MemoryAllocatorBase* _allocator);

    //////////////////////////////////////////
    MAZE_CORE_API std::string BuildMemoryAllocatorsDebugInfo();
    
    //////////////////////////////////////////
    template <Size TBlockSize, Size TPageSize = 65536, Size TAlignment = 8>
    BlockMemoryAllocator<TBlockSize, TPageSize, TAlignment>& CreateBlockMemoryAllocator()
    {
        static BlockMemoryAllocator<TBlockSize, TPageSize, TAlignment> s_allocator;
        RegisterMemoryAllocator(&s_allocator);
        return s_allocator;
    }

    //////////////////////////////////////////
    template <Size TBlockSize, Size TPageSize = 65536, Size TAlignment = 8>
    BlockMemoryAllocator<TBlockSize, TPageSize, TAlignment>& GetDefaultBlockMemoryAllocator()
    {
        static BlockMemoryAllocator<TBlockSize, TPageSize, TAlignment>& s_allocator = CreateBlockMemoryAllocator<TBlockSize, TPageSize, TAlignment>();
        return s_allocator;
    }



    //////////////////////////////////////////
    template <class T>
    inline void* MemoryBlockAllocationNew(Size _s)
    {
#if (MAZE_BLOCK_MEMORY_ALLOCATION_ENABLED)

        if (_s != sizeof(T))
        {
#   if (MAZE_DEBUG)
            MAZE_WARNING("Class has no allocator!");
#   endif
            return ::operator new(_s);
        }

        return GetDefaultBlockMemoryAllocator<Math::Align(sizeof(T), 8)>().allocBlock();

#else
        
#   if (1)
        return ::operator new(_s);
#   else
        return NedMemoryAllocator::AllocateBytes(_s);
#   endif
        
#endif
    }

    //////////////////////////////////////////
    template <class T>
    inline void MemoryBlockAllocationDelete(void* _m, Size _s)
    {
#if (MAZE_BLOCK_MEMORY_ALLOCATION_ENABLED)
        if (_s != sizeof(T))
        {
            ::operator delete(_m);
        }
        else
        if (_m != nullptr)
        {
            GetDefaultBlockMemoryAllocator<Math::Align(sizeof(T), 8)>().freeBlock(_m);
        }
#else
        
#   if (1)
            ::operator delete(_m);
#   else
            return NedMemoryAllocator::DeallocateBytes(_m);
#   endif
        
#endif
    }

    //////////////////////////////////////////
    #define MAZE_DECLARE_MEMORY_ALLOCATION(T)                                                    \
        static void* operator new(Maze::Size _s);                                                \
        static void* operator new(Maze::Size _s, void* _m);                                      \
        static void* operator new(Maze::Size _s, std::nothrow_t const& _t) throw();              \
        static void operator delete(void* _m, Maze::Size _s);                                    \
        static void operator delete(void*, void*);    

    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_BASE(DExplicitSpecialization, DClassName)     \
        DExplicitSpecialization                                                                  \
        void* DClassName::operator new(Maze::Size _s)                                            \
        {                                                                                        \
            return Maze::MemoryBlockAllocationNew<DClassName>(_s);                               \
        }                                                                                        \
        DExplicitSpecialization                                                                  \
        void* DClassName::operator new(Maze::Size _s, void* _m)                                  \
        {                                                                                        \
            return _m;                                                                           \
        }                                                                                        \
        DExplicitSpecialization                                                                  \
        void* DClassName::operator new(Maze::Size _s, std::nothrow_t const& _t) throw()          \
        {                                                                                        \
            MAZE_TODO;                                                                           \
            return nullptr;                                                                      \
        }                                                                                        \
        DExplicitSpecialization                                                                  \
        void DClassName::operator delete(void* _m, Maze::Size _s)                                \
        {                                                                                        \
            Maze::MemoryBlockAllocationDelete<DClassName>(_m, _s);                               \
        }                                                                                        \
        DExplicitSpecialization                                                                  \
        void DClassName::operator delete(void*, void*)                                           \
        {                                                                                        \
        }

    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(DClassName)                                   \
        MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_BASE(, DClassName)

    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_TEMPLATE(DClassName)                          \
        MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK_BASE(MAZE_EXPLICIT_TEMPLATE_SPECIALIZATION, DClassName)


    //////////////////////////////////////////
    #define MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(T)                                \
        static void* operator new(Maze::Size _s)                                                 \
        {                                                                                        \
            return Maze::MemoryBlockAllocationNew<T>(_s);                                        \
        }                                                                                        \
        static void* operator new(Maze::Size _s, void* _m)                                       \
        {                                                                                        \
            return _m;                                                                           \
        }                                                                                        \
        static void* operator new(Maze::Size _s, std::nothrow_t const& _t) throw()               \
        {                                                                                        \
            MAZE_TODO;                                                                           \
            return nullptr;                                                                      \
        }                                                                                        \
        static void operator delete(void* _m, Maze::Size _s)                                     \
        {                                                                                        \
            Maze::MemoryBlockAllocationDelete<T>(_m, _s);                                        \
        }                                                                                        \
        static void operator delete(void*, void*)                                                \
        {                                                                                        \
        }

    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(T)                                          \
        void* T::operator new(Maze::Size _s)                                                     \
        {                                                                                        \
            return ::operator new(_s);                                                           \
        }                                                                                        \
        void* T::operator new(Maze::Size _s, void* _m)                                           \
        {                                                                                        \
            return _m;                                                                           \
        }                                                                                        \
        void* T::operator new(Maze::Size _s, std::nothrow_t const& _t)    throw()                \
        {                                                                                        \
            MAZE_TODO;                                                                           \
            return nullptr;                                                                      \
        }                                                                                        \
        void T::operator delete(void* _m, Maze::Size _s)                                         \
        {                                                                                        \
            ::operator delete(_m);                                                               \
        }                                                                                        \
        void T::operator delete(void*, void*)                                                    \
        {                                                                                        \
        }    

    //////////////////////////////////////////
    #define MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(T)                              \
        static void* operator new(Maze::Size _s)                                                 \
        {                                                                                        \
            return ::operator new(_s);                                                           \
        }                                                                                        \
        static void* operator new(Maze::Size _s, void* _m)                                       \
        {                                                                                        \
            return _m;                                                                           \
        }                                                                                        \
        static void* operator new(Maze::Size _s, std::nothrow_t const& _t)    throw()            \
        {                                                                                        \
            MAZE_TODO;                                                                           \
            return nullptr;                                                                      \
        }                                                                                        \
        static void operator delete(void* _m, Maze::Size _s)                                     \
        {                                                                                        \
            ::operator delete(_m);                                                               \
        }                                                                                        \
        static void operator delete(void*, void*)                                                \
        {                                                                                        \
        }    

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeMemory_hpp_
//////////////////////////////////////////
