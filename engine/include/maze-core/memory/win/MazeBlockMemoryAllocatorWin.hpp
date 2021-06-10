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
#if (!defined(_MazeBlockMemoryAllocatorWin_hpp_))
#define _MazeBlockMemoryAllocatorWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/memory/MazeMemoryAllocatorBase.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MemoryPagePool
    //
    //////////////////////////////////////////
    template <Size TPageSize = 65536>
    class MemoryPagePool
    {
    public:

        //////////////////////////////////////////
        MemoryPagePool()
        {}

        //////////////////////////////////////////
        ~MemoryPagePool()
        {
            for (StdVector<void*>::iterator it = m_pages.begin(),
                                            end = m_pages.end();
                                            it != end;
                                            ++it)
            {
                VirtualFree(*it, 0, MEM_RELEASE);
            }
        }

        //////////////////////////////////////////
        void* allocPage()
        {
            void* page = VirtualAlloc(NULL, TPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            m_pages.push_back(page);
            return page;
        }

        //////////////////////////////////////////
        inline Size getPagesCount() const { return m_pages.size(); }

    private:
        StdVector<void*> m_pages;
    };


    //////////////////////////////////////////
    // Class BlockMemoryAllocatorWin
    //
    //////////////////////////////////////////
    template <Size TBlockSize, Size TPageSize = 65536, Size TAlignment = 8>
    class BlockMemoryAllocatorWin
        : private MemoryPagePool<TPageSize>
        , public MemoryAllocatorBase
    {
    public:

        ////////////////////////////////////
        BlockMemoryAllocatorWin()
            : m_head(nullptr)
        {
            m_alignedBlockSize = Math::Align(TBlockSize, TAlignment);
            m_alignedBlocksPerPage = TPageSize / m_alignedBlockSize;
        }

        ////////////////////////////////////
        ~BlockMemoryAllocatorWin()
        {
            
        }

        ////////////////////////////////////
        inline static SharedPtr<BlockMemoryAllocatorWin> Create() { return std::make_shared<BlockMemoryAllocatorWin>(); }

        ////////////////////////////////////
        void* allocBlock()
        {
            MAZE_MUTEX_SCOPED_LOCK(m_mutex);

            if (!m_head)
                formatNewPage();

            void* tmp = m_head;
            m_head = *(void**)m_head;
            return tmp;
        }

        ////////////////////////////////////
        void freeBlock(void* _tmp) 
        {
            MAZE_MUTEX_SCOPED_LOCK(m_mutex);

            *(void**)_tmp = m_head;
            m_head = _tmp;
        }

        ////////////////////////////////////
        virtual CString getName() MAZE_OVERRIDE
        { 
            return ClassInfo<BlockMemoryAllocatorWin<TBlockSize, TPageSize, TAlignment>>::Name();
        }

        ////////////////////////////////////
        virtual Size getAllocatedMemorySize() MAZE_OVERRIDE
        { 
            return getPagesCount() * TPageSize; 
        }

    protected:

        ////////////////////////////////////
        void formatNewPage()
        {
            void* tmp = allocPage();
            m_head = tmp;
            for (Size i = 0; i < m_alignedBlocksPerPage - 1; i++)
            {
                void* next = (S8*)tmp + m_alignedBlockSize;
                *(void**)tmp = next;
                tmp = next;
            }

            *(void**)tmp = nullptr;
        }

    protected:
        void* m_head;
        Size m_alignedBlockSize;
        Size m_alignedBlocksPerPage;
        Mutex m_mutex;
    };

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeBlockMemoryAllocatorWin_hpp_
//////////////////////////////////////////
