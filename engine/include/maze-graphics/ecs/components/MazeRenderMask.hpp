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
    enum class DefaultRenderMask
    {
        Default = MAZE_BIT(0),
        Gizmos = MAZE_BIT(1)
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
