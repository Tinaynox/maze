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
#if (!defined(_MazeFastVector_hpp_))
#define _MazeFastVector_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/memory/MazeNedMemoryAllocator.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
#define MAZE_FAST_ARRAY_DEBUG (0)
#define MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR (1)
    
#if (MAZE_FAST_ARRAY_DEBUG)
#    define MAZE_FAST_ARRAY_BP_IF(x) MAZE_DEBUG_BP_IF(x);
#else
#    define MAZE_FAST_ARRAY_BP_IF(x)
#endif


    //////////////////////////////////////////
    // Class FastVector
    //
    //////////////////////////////////////////
    template <typename T>
    class FastVector
    {
    public:
    
        //////////////////////////////////////////
        using value_type = T;
        using iterator = T*;
        using const_iterator = T const*;
        
#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
        using FastVectorAllocator = NedMemoryAllocator;
#endif

    public:
        
        //////////////////////////////////////////
        FastVector() 
            : m_data(0)
            , m_size(0)
            , m_capacity(0)
        {
        }
        
        //////////////////////////////////////////
        FastVector(FastVector<T> const& _copy) 
            : m_size(_copy.m_size)
            , m_capacity(_copy.m_size)
        {
#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
#    if (MAZE_DEBUG_MEMORY)
                m_data = (T*)FastVectorAllocator::AllocateBytes(
                    m_size * sizeof(T),
                    Maze::ClassInfo<FastVector>::QualifiedName(),
                    __FILE__,
                    __LINE__,
                    __FUNCTION__);
#    else
                m_data = (T*)FastVectorAllocator::AllocateBytes(m_size * sizeof(T));
#    endif
#else
            m_data = (T*)::operator new(m_size * sizeof(T));
#endif
            for (Size i = 0; i < m_size; ++i)
            {
                new (&m_data[i]) T(_copy.m_data[i]);
            }
        }
        
        //////////////////////////////////////////
        FastVector(Size _reserveAmount) 
            : m_size(0)
            , m_capacity(_reserveAmount)
        {
#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
#    if (MAZE_DEBUG_MEMORY)
            m_data = (T*)FastVectorAllocator::AllocateBytes(
                _reserveAmount * sizeof(T),
                Maze::ClassInfo<FastVector>::QualifiedName(),
                __FILE__,
                __LINE__,
                __FUNCTION__);
#    else
            m_data = (T*)FastVectorAllocator::AllocateBytes(_reserveAmount * sizeof(T));
#    endif
#else
            m_data = (T*)::operator new(reserveAmount * sizeof(T));
#endif
        }
        
        //////////////////////////////////////////
        FastVector(Size _count, T const& _value) 
            : m_size(_count)
            , m_capacity(_count)
        {
#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
#    if (MAZE_DEBUG_MEMORY)
            m_data = (T*)FastVectorAllocator::AllocateBytes(
                _count * sizeof(T),
                Maze::ClassInfo<FastVector>::QualifiedName(),
                __FILE__,
                __LINE__,
                __FUNCTION__);
#    else
            m_data = (T*)FastVectorAllocator::AllocateBytes(_count * sizeof(T));
#    endif
#else
            m_data = (T*)::operator new(_count * sizeof(T));
#endif

            for (Size i = 0; i < _count; ++i)
            {
                new (&m_data[i]) T(_value);
            }
        }

        //////////////////////////////////////////
        FastVector(std::initializer_list<value_type> _il)
        {
            appendPOD(_il.begin(), _il.end());
        }
        
        //////////////////////////////////////////
        ~FastVector()
        {
            for (Size i = 0; i < m_size; ++i)
                m_data[i].~T();

#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
            FastVectorAllocator::DeallocateBytes(m_data);
#else
            ::operator delete(m_data);
#endif
        }
        
        //////////////////////////////////////////
        inline void swap(FastVector<T>& _other)
        {
            std::swap(this->m_data, _other.m_data);
            std::swap(this->m_size, _other.m_size);
            std::swap(this->m_capacity, _other.m_capacity);
        }
        
        //////////////////////////////////////////
        inline void operator=(FastVector<T> const& _copy)
        {
            if (&_copy != this)
            {
                for (Size i = 0; i < m_size; ++i)
                    m_data[i].~T();

#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
                FastVectorAllocator::DeallocateBytes(m_data);
#else
                ::operator delete(m_data);
#endif

                m_size       = _copy.m_size;
                m_capacity   = _copy.m_size;

#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
#    if (MAZE_DEBUG_MEMORY)
                m_data = (T*)FastVectorAllocator::AllocateBytes(
                    m_size * sizeof(T),
                    Maze::ClassInfo<FastVector>::QualifiedName(),
                    __FILE__,
                    __LINE__,
                    __FUNCTION__);
#    else
                m_data = (T*)FastVectorAllocator::AllocateBytes(m_size * sizeof(T));
#    endif
#else
                m_data = (T*)::operator new(m_size * sizeof(T));
#endif

                for (Size i = 0; i < m_size; ++i)
                {
                    new (&m_data[i]) T(_copy.m_data[i]);
                }
            }
        }
        
        //////////////////////////////////////////
        inline Size size() const
        { 
            return m_size; 
        }
        
        //////////////////////////////////////////
        inline Size capacity() const
        { 
            return m_capacity; 
        }
        
        //////////////////////////////////////////
        inline iterator begin()
        { 
            return m_data; 
        }
        
        //////////////////////////////////////////
        inline const_iterator begin() const
        { 
            return m_data; 
        }
        
        //////////////////////////////////////////
        inline iterator end()
        { 
            return m_data + m_size; 
        }
        
        //////////////////////////////////////////
        inline const_iterator end() const
        { 
            return m_data + m_size; 
        }
        
        //////////////////////////////////////////
        inline void push_back(T const& _val)
        {
            growToFit(1);
            new (&m_data[m_size]) T(_val);
            ++m_size;
        }

        //////////////////////////////////////////
        template <class... Args>
        inline void emplace_back(Args&&... _args)
        {
            growToFit(1);
            new (&m_data[m_size]) T(std::forward<Args>(_args)...);
            ++m_size;
        }

        //////////////////////////////////////////
        inline void pop_back()
        {
            MAZE_FAST_ARRAY_BP_IF(m_size == 0);

            --m_size;
            m_data[m_size].~T();
        }

        //////////////////////////////////////////
        inline iterator insert(iterator _where, T const& _val)
        {
            Size idx = (_where - m_data);

            growToFit(1);

            memmove((void*)(m_data + idx + 1), (void const*)(m_data + idx), (m_size - idx) * sizeof(T));
            new (&m_data[idx]) T(_val);
            ++m_size;

            return m_data + idx;
        }
        
        //////////////////////////////////////////
        inline iterator insertPOD(
            iterator _where,
            const_iterator _otherBegin,
            const_iterator _otherEnd)
        {
            Size idx = (_where - m_data);

            Size const otherSize = _otherEnd - _otherBegin;

            growToFit(otherSize);

            memmove((void*)(m_data + idx + otherSize), (void const*)(m_data + idx), (m_size - idx) *  sizeof(T));

            _where = m_data + idx;

            while (_otherBegin != _otherEnd)
            {
                *(_where++) = *(_otherBegin++);
            }
            m_size += otherSize;

            return m_data + idx;
        }

        //////////////////////////////////////////
        inline void appendPOD(const_iterator _otherBegin, const_iterator _otherEnd)
        {
            growToFit(_otherEnd - _otherBegin);

            memcpy((void*)(m_data + m_size), (void const*)(_otherBegin), (_otherEnd - _otherBegin) *  sizeof(T));
            m_size += (_otherEnd - _otherBegin);
        }

        //////////////////////////////////////////
        inline void expand(Size _size)
        {
            growToFit(_size);
            m_size += _size;
        }

        //////////////////////////////////////////
        template <typename TIterator>
        inline iterator erase(TIterator _toErase)
        {
            Size idx = (_toErase - m_data);
            _toErase->~T();
            memmove((void*)(m_data + idx), (void const*)(m_data + idx + 1), (m_size - idx - 1) * sizeof(T));
            --m_size;

            return m_data + idx;
        }


        //////////////////////////////////////////
        template <typename TIterator>
        inline iterator erase(TIterator _first, TIterator _last)
        {
            MAZE_FAST_ARRAY_BP_IF((first > last) || (last > end()));

            Size idx      = (_first - m_data);
            Size idxNext  = (_last - m_data);
            while (_first != _last)
            {
                _first->~T();
                ++_first;
            }
            memmove(m_data + idx, m_data + idxNext, (m_size - idxNext) * sizeof(T));
            m_size -= (idxNext - idx);

            return m_data + idx;
        }

        //////////////////////////////////////////
        template <typename TIterator>
        inline iterator eraseUnordered(TIterator _toErase)
        {
            Size idx = (_toErase - m_data);
            _toErase->~T();
            memcpy(m_data + idx, m_data + m_size - 1, sizeof(T));
            --m_size;

            return m_data + idx;
        }

        //////////////////////////////////////////
        inline iterator erasePOD(iterator _first, iterator _last)
        {
            MAZE_FAST_ARRAY_BP_IF((first > last) || (last > end()));

            Size idx      = (_first - m_data);
            Size idxNext  = (_last - m_data);
            memmove(m_data + idx, m_data + idxNext, (m_size - idxNext) * sizeof(T));
            m_size -= (idxNext - idx);

            return m_data + idx;
        }

        //////////////////////////////////////////
        inline iterator find(T const& _val)
        { 
            return std::find(begin(), end(), _val); 
        }

        //////////////////////////////////////////
        inline const_iterator find(T const& _val) const
        { 
            return std::find(begin(), end(), _val); 
        }

        //////////////////////////////////////////
        inline void clear()
        {
            for (Size i = 0; i < m_size; ++i)
                m_data[i].~T();
            m_size = 0;
        }

        //////////////////////////////////////////
        inline bool empty() const                      
        { 
            return (m_size == 0);
        }

        //////////////////////////////////////////
        inline void reserve(Size _reserveAmount)
        {
            if (_reserveAmount > m_capacity)
            {
                m_capacity = _reserveAmount;

#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
#    if (MAZE_DEBUG_MEMORY)
                T* data = (T*)FastVectorAllocator::AllocateBytes(
                    m_capacity * sizeof(T),
                    Maze::ClassInfo<FastVector>::QualifiedName(),
                    __FILE__,
                    __LINE__,
                    __FUNCTION__);
#    else
                T* data = (T*)FastVectorAllocator::AllocateBytes(m_capacity * sizeof(T));
#    endif
#else
                T* data = (T*)::operator new(m_capacity * sizeof(T));
#endif
                memcpy(data, m_data, m_size * sizeof(T));

#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
                FastVectorAllocator::DeallocateBytes(m_data);
#else
                ::operator delete(m_data);
#endif

                m_data = data;
            }
        }

        //////////////////////////////////////////
        inline void resize(Size _newSize, T const& _value = T())
        {
            if (_newSize > m_size)
            {
                growToFit(_newSize - m_size);
                for (Size i = m_size; i < _newSize; ++i)
                {
                    new (&m_data[i]) T(_value);
                }
            }
            else
            if (_newSize < m_size)
            {
                for (Size i = _newSize; i < m_size; ++i)
                    m_data[i].~T();
            }

            m_size = _newSize;
        }

        //////////////////////////////////////////
        inline T& operator[](Size _idx)
        {
            MAZE_FAST_ARRAY_BP_IF(_idx >= m_size);
            return m_data[_idx];
        }

        //////////////////////////////////////////
        inline T const& operator[](Size _idx) const
        {
            MAZE_FAST_ARRAY_BP_IF(_idx >= m_size);
            return m_data[_idx];
        }

        //////////////////////////////////////////
        inline T& back()
        {
            MAZE_FAST_ARRAY_BP_IF(m_size == 0);
            return m_data[m_size-1];
        }

        //////////////////////////////////////////
        inline const T& back() const
        {
            MAZE_FAST_ARRAY_BP_IF(m_size == 0);
            return m_data[m_size-1];
        }

        //////////////////////////////////////////
        inline T& front()
        {
            MAZE_FAST_ARRAY_BP_IF(m_size == 0);
            return m_data[0];
        }

        //////////////////////////////////////////
        inline T const& front() const
        {
            MAZE_FAST_ARRAY_BP_IF(m_size == 0);
            return m_data[0];
        }

        
        //////////////////////////////////////////
        inline void growToFit(Size _newElements)
        {
            if (m_size + _newElements > m_capacity)
            {
                m_capacity = std::max(m_size + _newElements, m_capacity + (m_capacity >> 1) + 1);

#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
#    if (MAZE_DEBUG_MEMORY)
                T* data = (T*)FastVectorAllocator::AllocateBytes(
                    m_capacity * sizeof(T),
                    Maze::ClassInfo<FastVector>::QualifiedName(),
                    __FILE__,
                    __LINE__,
                    __FUNCTION__);
#    else
                T* data = (T*)FastVectorAllocator::AllocateBytes(m_capacity * sizeof(T));
#    endif
#else
                T* data = (T*)::operator new(m_capacity * sizeof(T));
#endif

                memcpy((void*)data, (void const*)m_data, m_size * sizeof(T));

#if (MAZE_FAST_ARRAY_USE_CUSTOM_ALLOCATOR)
                FastVectorAllocator::DeallocateBytes(m_data);
#else
                ::operator delete(m_data);
#endif

                m_data = data;
            }
        }

    protected:
        T*    m_data;
        Size m_size;
        Size m_capacity;
    };


    //////////////////////////////////////////
    template <class _Ty>
    struct IsVector<Maze::FastVector<_Ty>> : std::true_type {};

    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeFastVector_h_
//////////////////////////////////////////
