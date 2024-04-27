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
#if (!defined(_MazeECSPhysics2DEvents_hpp_))
#define _MazeECSPhysics2DEvents_hpp_


//////////////////////////////////////////
#include "maze-physics2d/MazePhysics2DHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Physics2DFixedUpdateStartEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API Physics2DFixedUpdateStartEvent
        : public GenericEvent<Physics2DFixedUpdateStartEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Physics2DFixedUpdateStartEvent, Event);

    public:

        //////////////////////////////////////////
        inline Physics2DFixedUpdateStartEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    // Class Physics2DFixedUpdateFinishedEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API Physics2DFixedUpdateFinishedEvent
        : public GenericEvent<Physics2DFixedUpdateFinishedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Physics2DFixedUpdateFinishedEvent, Event);

    public:

        //////////////////////////////////////////
        inline Physics2DFixedUpdateFinishedEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    // Class Physics2DUpdateFinishedEvent
    //
    //////////////////////////////////////////
    class MAZE_PHYSICS2D_API Physics2DUpdateFinishedEvent
        : public GenericEvent<Physics2DUpdateFinishedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Physics2DUpdateFinishedEvent, Event);

    public:

        //////////////////////////////////////////
        inline Physics2DUpdateFinishedEvent(
            F32 _dt = 0.0f,
            F32 _accumulatorRatio = 0.0f)
            : m_dt(_dt)
            , m_accumulatorRatio(_accumulatorRatio)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

        //////////////////////////////////////////
        inline F32 getAccumulatorRatio() const { return m_accumulatorRatio; }

    private:
        F32 m_dt = 0.0f;
        F32 m_accumulatorRatio = 0.0f;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeECSPhysics2DEvents_hpp_
//////////////////////////////////////////
