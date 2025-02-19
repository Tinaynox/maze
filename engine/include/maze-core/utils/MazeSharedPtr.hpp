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
#if (!defined(_MazeSharedPtr_hpp_))
#define _MazeSharedPtr_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template<class T> class MazeWeakPtr;
    template<class T> class MazeSharedPtr;
    template<class T> class MazeEnableSharedFromThisPtr;


    //////////////////////////////////////////
    // Struct RefCountBlock
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API RefCountBlock
    {
        std::atomic<S32> weak = 0;
        std::atomic<S32> strong = 1;
    };

    //////////////////////////////////////////
    template <typename T>
    void CheckMazeEnableSharedFromThisPtr(RefCountBlock* _refCountBlock, MazeEnableSharedFromThisPtr<T>* _enableSharedFromThisPtr, T* _ptr);

    //////////////////////////////////////////
    inline void CheckMazeEnableSharedFromThisPtr(RefCountBlock* _refCountBlock, ...) { }


    //////////////////////////////////////////
    // Class MazeWeakPtr
    //
    //////////////////////////////////////////
    template <typename T>
    class MazeWeakPtr
    {
        //////////////////////////////////////////
        friend class MazeSharedPtr<T>;

    public:

        //////////////////////////////////////////
        inline MazeWeakPtr() = default;

        //////////////////////////////////////////
        inline MazeWeakPtr::~MazeWeakPtr()
        {
            release();
        }

        //////////////////////////////////////////
        inline MazeWeakPtr(MazeSharedPtr<T> const& _r)
            : m_ptr(_r.getPtr())
            , m_refCountBlock(_r.getRefCountBlock())
        {
            if (m_refCountBlock)            
                ++m_refCountBlock->weak;
        }

        //////////////////////////////////////////
        inline MazeWeakPtr(MazeWeakPtr&& _other) noexcept
            : m_ptr(_other.m_ptr)
            , m_refCountBlock(_other.m_refCountBlock)
        {
            other.m_ptr = nullptr;
            other.m_refCountBlock = nullptr;
        }


        //////////////////////////////////////////
        inline MazeWeakPtr& operator=(MazeSharedPtr<T> const& _r)
        {
            release();
            m_ptr = _r.getPtr();
            m_refCountBlock = _r.getRefCountBlock();

            if (m_refCountBlock)
                ++m_refCountBlock->weak;

            return *this;
        }

        //////////////////////////////////////////
        inline operator bool() const
        {
            return !isNull() && !expired();
        }

        //////////////////////////////////////////
        inline Bool expired() const { return (useCount() == 0); }

        //////////////////////////////////////////
        inline S32 useCount() const { assert(m_refCountBlock); return m_refCountBlock->strong; }

        //////////////////////////////////////////
        inline MazeSharedPtr<T> lock()
        {
            if (isNull() || expired())
                return nullptr;

            return MazeSharedPtr<T>(*this);
        }

        //////////////////////////////////////////
        inline bool isNull() const { return m_ptr == nullptr; }

        //////////////////////////////////////////
        inline void setNull()
        {
            if (m_ptr)
            {
                release();
                m_ptr = nullptr;
                m_refCountBlock = nullptr;
            }
        }

        //////////////////////////////////////////
        void bind(T* _ptr, RefCountBlock* _refCountBlock)
        {
            assert(!m_ptr && !m_refCountBlock)

            m_ptr = _ptr;
            m_refCountBlock = _refCountBlock;
        }

    private:

        //////////////////////////////////////////
        inline T* getPtr() const { return m_ptr; }

        //////////////////////////////////////////
        inline RefCountBlock* getRefCountBlock() const { return m_refCountBlock; }

        //////////////////////////////////////////
        inline void release()
        {
            if (m_refCountBlock)
            {
                if (--m_refCountBlock->weak == 0 && m_refCountBlock->strong == 0)
                {
                    delete m_refCountBlock;
                    m_refCountBlock = nullptr;
                }
            }

        }

    private:
        T* m_ptr = nullptr;
        RefCountBlock* m_refCountBlock = nullptr;
    };


    //////////////////////////////////////////
    // Class MazeSharedPtr
    //
    //////////////////////////////////////////
    template <typename T>
    class MazeSharedPtr
    {
    public:

        //////////////////////////////////////////
        inline MazeSharedPtr() = default;

        //////////////////////////////////////////
        inline MazeSharedPtr(T* _ptr)
        {
            if (_ptr)
                bind(_ptr);
        }

        //////////////////////////////////////////
        inline MazeSharedPtr(MazeSharedPtr const& _r)
            : m_ptr(_r.m_ptr)
            , m_refCountBlock(_r.m_refCountBlock)
        {
            if (m_refCountBlock)
                ++m_refCountBlock->strong;
        }

        //////////////////////////////////////////
        inline MazeSharedPtr(MazeSharedPtr&& _other) noexcept
            : m_ptr(_other.m_ptr)
            , m_refCountBlock(_other.m_refCountBlock)
        {
            _other.m_ptr = nullptr;
            _other.m_refCountBlock = nullptr;
        }


        //////////////////////////////////////////
        template <typename Y>
        inline MazeSharedPtr(MazeSharedPtr<Y> const& _r)
            : m_ptr(static_cast<T*>(_r.getPtr()))
            , m_refCountBlock(_r.getRefCountBlock())
        {
            if (m_refCountBlock)
                ++m_refCountBlock->strong;
        }

        //////////////////////////////////////////
        template <typename Y>
        MazeSharedPtr(MazeWeakPtr<Y> const& _r)
            : m_ptr(_r.getPtr())
            , m_refCountBlock(_r.getRefCountBlock())
        {
            if (m_refCountBlock)
                ++m_refCountBlock->strong;
        }

        //////////////////////////////////////////
        inline ~MazeSharedPtr()
        {
            release();
        }

        //////////////////////////////////////////
        inline MazeSharedPtr& operator=(MazeSharedPtr const& _r)
        {
            if (m_ptr == _r.m_ptr)
                return *this;

            MazeSharedPtr<T> tmp(_r);
            swap(tmp);
            return *this;
        }

        //////////////////////////////////////////
        template <typename Y>
        inline MazeSharedPtr& operator=(Y* _r)
        {
            bind(r);
            return *this;
        }

        //////////////////////////////////////////
        template <typename Y>
        inline MazeSharedPtr& operator=(MazeSharedPtr<Y> const& _r)
        {
            if (m_ptr == _r.getPtr())
                return *this;

            MazeSharedPtr<T> tmp(_r);
            swap(tmp);
            return *this;
        }

        //////////////////////////////////////////
        inline MazeSharedPtr& operator=(MazeWeakPtr<T> const& _r)
        {
            if (m_ptr == _r.m_ptr)
                return *this;

            release();
            m_ptr = _r.getPtr();
            m_refCountBlock = _r.getRefCountBlock();

            if (m_refCountBlock)
                ++m_refCountBlock->strong;

            return *this;
        }

        //////////////////////////////////////////
        inline MazeSharedPtr& operator=(MazeSharedPtr&& _other) noexcept
        {
            if (this != &_other)
            {
                release();
                m_ptr = other.m_ptr;
                m_refCountBlock = other.m_refCountBlock;
                other.m_ptr = nullptr;
                other.m_refCountBlock = nullptr;
            }

            return *this;
        }
        
        //////////////////////////////////////////
        inline operator bool() const
        {
            return !isNull();
        }

        //////////////////////////////////////////
        inline bool operator!() const { return !m_ptr; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE T& operator*() const { assert(m_ptr); return *m_ptr; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE T* operator->() const { assert(m_ptr); return m_ptr; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE T* get() const { return m_ptr; }

        //////////////////////////////////////////
        void bind(T* _ptr)
        {
            assert(!m_ptr && !m_refCountBlock);

            m_ptr = _ptr;
            m_refCountBlock = new RefCountBlock();

            CheckMazeEnableSharedFromThisPtr(m_refCountBlock, m_ptr, m_ptr);
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE Bool unique() const { assert(m_refCountBlock); return m_refCountBlock->strong == 1; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE S32 useCount() const { assert(m_refCountBlock); return m_refCountBlock->strong; }

        //////////////////////////////////////////
        inline T* getPtr() const { return m_ptr; }

        //////////////////////////////////////////
        inline RefCountBlock* getRefCountBlock() const { return m_refCountBlock; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE bool isNull() const { return m_ptr == nullptr; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void setNull()
        {
            if (m_ptr)
            {
                release();

                m_ptr = nullptr;
                m_refCountBlock = nullptr;
            }
        }

        //////////////////////////////////////////
        void swap(MazeSharedPtr<T>& _other)
        {
            std::swap(m_ptr, _other.m_ptr);
            std::swap(m_refCountBlock, _other.m_refCountBlock);
        }

    private:

        //////////////////////////////////////////
        inline void release(void)
        {
            if (m_refCountBlock)
            {
                if (--m_refCountBlock->strong == 0 && m_refCountBlock->weak == 0)
                {
                    delete m_ptr;
                    m_ptr = nullptr;

                    delete m_refCountBlock;
                    m_refCountBlock = nullptr;
                }
            }
        }

    private:
        T* m_ptr = nullptr;
        RefCountBlock* m_refCountBlock = nullptr;
    };


    //////////////////////////////////////////
    template<class T, class U>
    inline bool operator==(MazeSharedPtr<T> const& _a, MazeSharedPtr<U> const& _b)
    {
        return _a.get() == _b.get();
    }

    //////////////////////////////////////////
    template<class T, class U>
    inline bool operator!=(MazeSharedPtr<T> const& _a, MazeSharedPtr<U> const& _b)
    {
        return _a.get() != _b.get();
    }

    //////////////////////////////////////////
    template<class T, class U>
    inline bool operator<(MazeSharedPtr<T> const& _a, MazeSharedPtr<U> const& _b)
    {
        return std::less<void const*>()(_a.get(), _b.get());
    }


    //////////////////////////////////////////
    // Class MazeEnableSharedFromThisPtr
    //
    //////////////////////////////////////////
    template <typename T>
    class MazeEnableSharedFromThisPtr
    {
    public:
        //////////////////////////////////////////
        inline MazeEnableSharedFromThisPtr() = default;

        //////////////////////////////////////////
        inline MazeWeakPtr<T> const& weakFromThis()
        {
            return m_thisPtr;
        }

        //////////////////////////////////////////
        inline MazeSharedPtr<T> sharedFromThis()
        {
            return MazeSharedPtr<T>(m_thisPtr);
        }

    private:

        //////////////////////////////////////////
        void enableSharedFromThisPtr(RefCountBlock* _refCountBlock, T* _ptr)
        {
            m_thisPtr.bind(_ptr, _refCountBlock);
        }

        //////////////////////////////////////////
        friend void CheckMazeEnableSharedFromThisPtr(RefCountBlock* _refCountBlock, MazeEnableSharedFromThisPtr<T>* _enableSharedFromThisPtr, T* _ptr)
        {
            _enableSharedFromThisPtr->enableSharedFromThisPtr(_refCountBlock, _ptr);
        }

    private:
        MazeWeakPtr<T> m_thisPtr;
    };
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeSharedPtr_hpp_
