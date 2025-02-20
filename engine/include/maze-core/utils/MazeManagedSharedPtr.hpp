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
#if (!defined(_ManagedSharedPtr_hpp_))
#define _ManagedSharedPtr_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template<class T> class ManagedWeakPtr;
    template<class T> class ManagedSharedPtr;
    template<class T> class ManagedEnableSharedFromThisPtr;


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
    void CheckManagedEnableSharedFromThisPtr(RefCountBlock* _refCountBlock, ManagedEnableSharedFromThisPtr<T>* _enableSharedFromThisPtr, T* _ptr);

    //////////////////////////////////////////
    inline void CheckManagedEnableSharedFromThisPtr(RefCountBlock* _refCountBlock, ...) { }


    //////////////////////////////////////////
    // Class ManagedWeakPtr
    //
    //////////////////////////////////////////
    template <typename T>
    class ManagedWeakPtr
    {
        //////////////////////////////////////////
        friend class ManagedSharedPtr<T>;

    public:

        //////////////////////////////////////////
        inline ManagedWeakPtr() = default;

        //////////////////////////////////////////
        inline ManagedWeakPtr::~ManagedWeakPtr()
        {
            release();
        }

        //////////////////////////////////////////
        inline ManagedWeakPtr(ManagedSharedPtr<T> const& _r)
            : m_ptr(_r.getPtr())
            , m_refCountBlock(_r.getRefCountBlock())
        {
            if (m_refCountBlock)            
                ++m_refCountBlock->weak;
        }

        //////////////////////////////////////////
        inline ManagedWeakPtr(ManagedWeakPtr&& _other) noexcept
            : m_ptr(_other.m_ptr)
            , m_refCountBlock(_other.m_refCountBlock)
        {
            other.m_ptr = nullptr;
            other.m_refCountBlock = nullptr;
        }


        //////////////////////////////////////////
        inline ManagedWeakPtr& operator=(ManagedSharedPtr<T> const& _r)
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
        inline ManagedSharedPtr<T> lock()
        {
            if (isNull() || expired())
                return nullptr;

            return ManagedSharedPtr<T>(*this);
        }

        //////////////////////////////////////////
        inline bool isNull() const { return m_ptr == nullptr; }

        //////////////////////////////////////////
        inline void reset()
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
    // Class ManagedSharedPtr
    //
    //////////////////////////////////////////
    template <typename T>
    class ManagedSharedPtr
    {
    public:

        //////////////////////////////////////////
        inline ManagedSharedPtr() = default;

        //////////////////////////////////////////
        inline ManagedSharedPtr(T* _ptr)
        {
            if (_ptr)
                bind(_ptr);
        }

        //////////////////////////////////////////
        inline ManagedSharedPtr(ManagedSharedPtr const& _r)
            : m_ptr(_r.m_ptr)
            , m_refCountBlock(_r.m_refCountBlock)
        {
            if (m_refCountBlock)
                ++m_refCountBlock->strong;
        }

        //////////////////////////////////////////
        inline ManagedSharedPtr(ManagedSharedPtr&& _other) noexcept
            : m_ptr(_other.m_ptr)
            , m_refCountBlock(_other.m_refCountBlock)
        {
            _other.m_ptr = nullptr;
            _other.m_refCountBlock = nullptr;
        }


        //////////////////////////////////////////
        template <typename Y>
        inline ManagedSharedPtr(ManagedSharedPtr<Y> const& _r)
            : m_ptr(static_cast<T*>(_r.getPtr()))
            , m_refCountBlock(_r.getRefCountBlock())
        {
            if (m_refCountBlock)
                ++m_refCountBlock->strong;
        }

        //////////////////////////////////////////
        template <typename Y>
        ManagedSharedPtr(ManagedWeakPtr<Y> const& _r)
            : m_ptr(_r.getPtr())
            , m_refCountBlock(_r.getRefCountBlock())
        {
            if (m_refCountBlock)
                ++m_refCountBlock->strong;
        }

        //////////////////////////////////////////
        inline ~ManagedSharedPtr()
        {
            release();
        }

        //////////////////////////////////////////
        inline ManagedSharedPtr& operator=(ManagedSharedPtr const& _r)
        {
            if (m_ptr == _r.m_ptr)
                return *this;

            ManagedSharedPtr<T> tmp(_r);
            swap(tmp);
            return *this;
        }

        //////////////////////////////////////////
        template <typename Y>
        inline ManagedSharedPtr& operator=(Y* _r)
        {
            bind(r);
            return *this;
        }

        //////////////////////////////////////////
        template <typename Y>
        inline ManagedSharedPtr& operator=(ManagedSharedPtr<Y> const& _r)
        {
            if (m_ptr == _r.getPtr())
                return *this;

            ManagedSharedPtr<T> tmp(_r);
            swap(tmp);
            return *this;
        }

        //////////////////////////////////////////
        inline ManagedSharedPtr& operator=(ManagedWeakPtr<T> const& _r)
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
        inline ManagedSharedPtr& operator=(ManagedSharedPtr&& _other) noexcept
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

            CheckManagedEnableSharedFromThisPtr(m_refCountBlock, m_ptr, m_ptr);
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE Bool unique() const { assert(m_refCountBlock); return m_refCountBlock->strong == 1; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE S32 useCount() const { assert(m_refCountBlock); return m_refCountBlock->strong; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void incRef() { assert(m_refCountBlock); ++m_refCountBlock->strong; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void decRef() { assert(m_refCountBlock && m_refCountBlock->strong > 1); --m_refCountBlock->strong; }

        //////////////////////////////////////////
        inline T* getPtr() const { return m_ptr; }

        //////////////////////////////////////////
        inline RefCountBlock* getRefCountBlock() const { return m_refCountBlock; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE bool isNull() const { return m_ptr == nullptr; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void reset()
        {
            if (m_ptr)
            {
                release();

                m_ptr = nullptr;
                m_refCountBlock = nullptr;
            }
        }

        //////////////////////////////////////////
        void swap(ManagedSharedPtr<T>& _other)
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
    inline bool operator==(ManagedSharedPtr<T> const& _a, ManagedSharedPtr<U> const& _b)
    {
        return _a.get() == _b.get();
    }

    //////////////////////////////////////////
    template<class T, class U>
    inline bool operator!=(ManagedSharedPtr<T> const& _a, ManagedSharedPtr<U> const& _b)
    {
        return _a.get() != _b.get();
    }

    //////////////////////////////////////////
    template<class T, class U>
    inline bool operator<(ManagedSharedPtr<T> const& _a, ManagedSharedPtr<U> const& _b)
    {
        return std::less<void const*>()(_a.get(), _b.get());
    }


    //////////////////////////////////////////
    // Class ManagedEnableSharedFromThisPtr
    //
    //////////////////////////////////////////
    template <typename T>
    class ManagedEnableSharedFromThisPtr
    {
    public:
        //////////////////////////////////////////
        inline ManagedEnableSharedFromThisPtr() = default;

        //////////////////////////////////////////
        inline ManagedWeakPtr<T> const& weakFromThis()
        {
            return m_thisPtr;
        }

        //////////////////////////////////////////
        inline ManagedSharedPtr<T> sharedFromThis()
        {
            return ManagedSharedPtr<T>(m_thisPtr);
        }

    private:

        //////////////////////////////////////////
        void enableSharedFromThisPtr(RefCountBlock* _refCountBlock, T* _ptr)
        {
            m_thisPtr.bind(_ptr, _refCountBlock);
        }

        //////////////////////////////////////////
        friend void CheckManagedEnableSharedFromThisPtr(RefCountBlock* _refCountBlock, ManagedEnableSharedFromThisPtr<T>* _enableSharedFromThisPtr, T* _ptr)
        {
            _enableSharedFromThisPtr->enableSharedFromThisPtr(_refCountBlock, _ptr);
        }

    private:
        ManagedWeakPtr<T> m_thisPtr;
    };
    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
// Aliases
//
//////////////////////////////////////////
#define MAZE_USING_MANAGED_SHARED_PTR(__DClass)             using __DClass##Ptr             = Maze::ManagedSharedPtr<class __DClass>; \
                                                            using __DClass##ConstPtr        = Maze::ManagedSharedPtr<class __DClass const>; \
                                                            using __DClass##WPtr            = Maze::ManagedWeakPtr<class __DClass>; \
                                                            using __DClass##ConstWPtr       = Maze::ManagedWeakPtr<class __DClass const>;


//////////////////////////////////////////
#endif // _ManagedSharedPtr_hpp_
