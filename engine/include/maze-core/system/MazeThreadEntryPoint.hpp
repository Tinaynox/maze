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
#if (!defined(_MazeThreadEntryPoint_hpp_))
#define _MazeThreadEntryPoint_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/utils/MazeDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ThreadEntryPoint
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ThreadEntryPoint
    {
    public:

        ////////////////////////////////////
        ThreadEntryPoint();

        ////////////////////////////////////
        virtual ~ThreadEntryPoint();

        ////////////////////////////////////
        ThreadEntryPoint(ThreadEntryPoint const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint(ThreadEntryPoint&&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint& operator=(ThreadEntryPoint const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint& operator=(ThreadEntryPoint&&) = delete;

        //////////////////////////////////////////
        virtual S32 run() MAZE_ABSTRACT;
    };


    //////////////////////////////////////////
    // Class ThreadEntryPoint0
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ThreadEntryPoint0
        : public ThreadEntryPoint
    {
    public:

        //////////////////////////////////////////
        ThreadEntryPoint0(Delegate<S32> const& _delegate)
            : m_delegate(_delegate)
        {}

        ////////////////////////////////////
        ThreadEntryPoint0(ThreadEntryPoint0 const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint0(ThreadEntryPoint0&&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint0& operator=(ThreadEntryPoint0 const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint0& operator=(ThreadEntryPoint0&&) = delete;

        //////////////////////////////////////////
        virtual S32 run() MAZE_OVERRIDE;

    protected:
        Delegate<S32> m_delegate;
    };


    //////////////////////////////////////////
    // Class ThreadEntryPoint1
    //
    //////////////////////////////////////////
    template <typename TArg0>
    class ThreadEntryPoint1
        : public ThreadEntryPoint
    {
    public:

        //////////////////////////////////////////
        ThreadEntryPoint1(
            Delegate<S32, TArg0> const& _delegate,
            TArg0 _arg0)
            : m_delegate(_delegate)
            , m_arg0(_arg0)
        {}

        ////////////////////////////////////
        ThreadEntryPoint1(ThreadEntryPoint1 const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint1(ThreadEntryPoint1&&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint1& operator=(ThreadEntryPoint1 const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint1& operator=(ThreadEntryPoint1&&) = delete;

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
    // Class ThreadEntryPoint2
    //
    //////////////////////////////////////////
    template <typename TArg0, typename TArg1>
    class ThreadEntryPoint2
        : public ThreadEntryPoint
    {
    public:

        //////////////////////////////////////////
        ThreadEntryPoint2(
            Delegate<S32, TArg0, TArg1> const& _delegate,
            TArg0 _arg0,
            TArg1 _arg1)
            : m_delegate(_delegate)
            , m_arg0(_arg0)
            , m_arg1(_arg1)
        {}

        ////////////////////////////////////
        ThreadEntryPoint2(ThreadEntryPoint2 const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint2(ThreadEntryPoint2&&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint2& operator=(ThreadEntryPoint2 const&) = delete;

        ////////////////////////////////////
        ThreadEntryPoint2& operator=(ThreadEntryPoint2&&) = delete;

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


#endif // _MazeThreadEntryPoint_hpp_
//////////////////////////////////////////
