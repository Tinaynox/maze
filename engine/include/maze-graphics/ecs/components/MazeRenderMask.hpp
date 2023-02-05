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
#if (!defined(_MazeRenderMask_hpp_))
#define _MazeRenderMask_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMask);


    //////////////////////////////////////////
    enum class DefaultRenderMask : S32
    {
        Default = MAZE_BIT(0),
        Gizmos = MAZE_BIT(1),
        Water = MAZE_BIT(2),

        MazeReserved0 = MAZE_BIT(3),
        MazeReserved1 = MAZE_BIT(4),
        MazeReserved2 = MAZE_BIT(5),
        MazeReserved3 = MAZE_BIT(6),
        MazeReserved4 = MAZE_BIT(7),
        MazeReserved5 = MAZE_BIT(8),
        MazeReserved6 = MAZE_BIT(9),
        MazeReserved7 = MAZE_BIT(10),
        MazeReserved8 = MAZE_BIT(11),
        MazeReserved9 = MAZE_BIT(12),
        MazeReserved10 = MAZE_BIT(13),
        MazeReserved11 = MAZE_BIT(14),
        MazeReserved12 = MAZE_BIT(15),

        UserMask0 = MAZE_BIT(16),
        UserMask1 = MAZE_BIT(17),
        UserMask2 = MAZE_BIT(18),
        UserMask3 = MAZE_BIT(19),
        UserMask4 = MAZE_BIT(20),
        UserMask5 = MAZE_BIT(21),
        UserMask6 = MAZE_BIT(22),
        UserMask7 = MAZE_BIT(23),
        UserMask8 = MAZE_BIT(24),
        UserMask9 = MAZE_BIT(25),
        UserMask10 = MAZE_BIT(26),
        UserMask11 = MAZE_BIT(27),
        UserMask12 = MAZE_BIT(28),
        UserMask13 = MAZE_BIT(29),
        UserMask14 = MAZE_BIT(30),
        UserMask15 = MAZE_BIT(31),
    };


    //////////////////////////////////////////
    // Class RenderMask
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API RenderMask
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderMask, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(RenderMask);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~RenderMask();

        //////////////////////////////////////////
        static RenderMaskPtr Create();

        
        //////////////////////////////////////////
        inline void setMask(S32 _mask) { m_mask = _mask; }

        //////////////////////////////////////////
        inline void setMask(DefaultRenderMask _mask) { m_mask = (S32)_mask; }

        //////////////////////////////////////////
        inline S32 getMask() const { return m_mask; }

    protected:

        //////////////////////////////////////////
        RenderMask();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();
            

    protected:
        S32 m_mask;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderMask_hpp_
//////////////////////////////////////////
