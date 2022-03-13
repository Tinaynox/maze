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
#if (!defined(_MazeTaskDelegate_hpp_))
#define _MazeTaskDelegate_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-core/memory/MazeMemory.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TaskDelegate
    //
    //////////////////////////////////////////
    class MAZE_CORE_API TaskDelegate
    {   
    public:

        ////////////////////////////////////
        TaskDelegate() = default;

        ////////////////////////////////////
        virtual ~TaskDelegate() = default;

        ////////////////////////////////////
        TaskDelegate(TaskDelegate const&) = delete;

        ////////////////////////////////////
        TaskDelegate(TaskDelegate&&) = delete;

        ////////////////////////////////////
        TaskDelegate& operator=(TaskDelegate const&) = delete;

        ////////////////////////////////////
        TaskDelegate& operator=(TaskDelegate&&) = delete;

        //////////////////////////////////////////
        virtual S32 run() MAZE_ABSTRACT;
    };


    //////////////////////////////////////////
    // Class TaskDelegate0
    //
    //////////////////////////////////////////
    class MAZE_CORE_API TaskDelegate0
        : public TaskDelegate
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(TaskDelegate0);

    public:

        //////////////////////////////////////////
        TaskDelegate0(Delegate<S32> const& _delegate)
            : m_delegate(_delegate)
        {}

        ////////////////////////////////////
        TaskDelegate0(TaskDelegate0 const&) = delete;

        ////////////////////////////////////
        TaskDelegate0(TaskDelegate0&&) = delete;

        ////////////////////////////////////
        TaskDelegate0& operator=(TaskDelegate0 const&) = delete;

        ////////////////////////////////////
        TaskDelegate0& operator=(TaskDelegate0&&) = delete;

        //////////////////////////////////////////
        virtual S32 run() MAZE_OVERRIDE
        {
            return m_delegate();
        }

    protected:
        Delegate<S32> m_delegate;
    };


    //////////////////////////////////////////
    // Class TaskDelegate1
    //
    //////////////////////////////////////////
    template <typename TArg0>
    class TaskDelegate1
        : public TaskDelegate
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(TaskDelegate1);

    public:

        //////////////////////////////////////////
        TaskDelegate1(
            Delegate<S32, TArg0> const& _delegate,
            TArg0 _arg0)
            : m_delegate(_delegate)
            , m_arg0(_arg0)
        {}

        ////////////////////////////////////
        TaskDelegate1(TaskDelegate1 const&) = delete;

        ////////////////////////////////////
        TaskDelegate1(TaskDelegate1&&) = delete;

        ////////////////////////////////////
        TaskDelegate1& operator=(TaskDelegate1 const&) = delete;

        ////////////////////////////////////
        TaskDelegate1& operator=(TaskDelegate1&&) = delete;

        //////////////////////////////////////////
        virtual S32 run() MAZE_OVERRIDE
        {
            return m_delegate(m_arg0);
        }

    protected:
        Delegate<S32, TArg0> m_delegate;
        TArg0 m_arg0;
    };


    //////////////////////////////////////////
    // Class TaskDelegate2
    //
    //////////////////////////////////////////
    template <typename TArg0, typename TArg1>
    class TaskDelegate2
        : public TaskDelegate
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_AND_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(TaskDelegate2);

    public:

        //////////////////////////////////////////
        TaskDelegate2(
            Delegate<S32, TArg0, TArg1> const& _delegate,
            TArg0 _arg0,
            TArg1 _arg1)
            : m_delegate(_delegate)
            , m_arg0(_arg0)
            , m_arg1(_arg1)
        {}

        ////////////////////////////////////
        TaskDelegate2(TaskDelegate2 const&) = delete;

        ////////////////////////////////////
        TaskDelegate2(TaskDelegate2&&) = delete;

        ////////////////////////////////////
        TaskDelegate2& operator=(TaskDelegate2 const&) = delete;

        ////////////////////////////////////
        TaskDelegate2& operator=(TaskDelegate2&&) = delete;

        //////////////////////////////////////////
        virtual S32 run() MAZE_OVERRIDE
        {
            return m_delegate(m_arg0, m_arg1);
        }

    protected:
        Delegate<S32, TArg0, TArg1> m_delegate;
        TArg0 m_arg0;
        TArg1 m_arg1;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTaskDelegate_hpp_
//////////////////////////////////////////
