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
#if (!defined(_MazeECSEvents_hpp_))
#define _MazeECSEvents_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PreUpdateEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PreUpdateEvent
        : public GenericEvent<PreUpdateEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PreUpdateEvent, Event);

    public:

        //////////////////////////////////////////
        inline PreUpdateEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    // Class UpdateEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API UpdateEvent
        : public GenericEvent<UpdateEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UpdateEvent, Event);

    public:

        //////////////////////////////////////////
        inline UpdateEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    // Class PostUpdateEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PostUpdateEvent
        : public GenericEvent<PostUpdateEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PostUpdateEvent, Event);

    public:

        //////////////////////////////////////////
        inline PostUpdateEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeECSEvents_hpp_
//////////////////////////////////////////
