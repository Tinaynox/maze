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
#if (!defined(_MazeSharedObjectPool_hpp_))
#define _MazeSharedObjectPool_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include <functional>
#include <stack>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SharedObjectPool
    //
    //////////////////////////////////////////
    template <typename T>
    class SharedObjectPool
    {
    public:

        //////////////////////////////////////////
        using ObjectSharedPtr = SharedPtr<T>; 

    public:

        //////////////////////////////////////////
        inline SharedObjectPool(
            std::function<ObjectSharedPtr()> const& _createFunc,
            std::function<void(ObjectSharedPtr)> const& _getFunc,
            std::function<void(ObjectSharedPtr)> const& _releaseFunc,
            std::function<void(ObjectSharedPtr)> const& _destroyFunc,
            Size _maxSize = 10000)
            : m_countAll(0)
        {
            MAZE_DEBUG_ERROR_IF(_maxSize == 0, "Max Size must be greater than 0");

            m_createFunc = _createFunc;
            m_getFunc = _getFunc;
            m_releaseFunc = _releaseFunc;
            m_destroyFunc = _destroyFunc;

            m_maxSize = _maxSize;
        }

        //////////////////////////////////////////
        inline ~SharedObjectPool()
        {
            while (!m_poolStack.empty())
            {
                ObjectSharedPtr const&  obj = m_poolStack.top();
                m_destroyFunc(obj);
                m_poolStack.pop();
            }
        }

        //////////////////////////////////////////
        inline S32 getCountAll() const { return m_countAll; }

        //////////////////////////////////////////
        inline S32 getCountInactive() const { return (S32)m_poolStack.size(); }

        //////////////////////////////////////////
        inline S32 getCountActive() const { return getCountAll() - getCountInactive(); }

        //////////////////////////////////////////
        inline ObjectSharedPtr fetch()
        {
            ObjectSharedPtr element;

            if (m_poolStack.empty())
            {
                element = m_createFunc();
                ++m_countAll;
            }
            else
            {
                element = m_poolStack.top();
                m_poolStack.pop();
            }

            m_getFunc(element);

            return element;
        }

        //////////////////////////////////////////
        inline void reserve(S32 _count)
        {
            for (S32 i = 0; i < _count; ++i)
            {
                ObjectSharedPtr element = m_createFunc();
                ++m_countAll;
                release(element);
            }
        }

        //////////////////////////////////////////
        inline void release(ObjectSharedPtr const& _element)
        {
            m_releaseFunc(_element);

            if (getCountInactive() < (S32)m_maxSize)
            {
                MAZE_DEBUG_ERROR_IF(!m_poolStack.empty() && m_poolStack.top() == _element, "Duplicate push to stack!");
                m_poolStack.push(_element);
            }
        }

        //////////////////////////////////////////
        inline void clear()
        {
            while (!m_poolStack.empty())
                m_poolStack.pop();

            m_countAll = 0;
        }

    private:
        std::function<ObjectSharedPtr()> m_createFunc;
        std::function<void(ObjectSharedPtr const&)> m_getFunc;
        std::function<void(ObjectSharedPtr const&)> m_releaseFunc;
        std::function<void(ObjectSharedPtr const&)> m_destroyFunc;
        Size m_maxSize = 0u;
        Stack<ObjectSharedPtr> m_poolStack;
        S32 m_countAll = 0;
    };

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeSharedObjectPool_hpp_
