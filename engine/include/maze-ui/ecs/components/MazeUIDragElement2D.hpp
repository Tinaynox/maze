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
#if (!defined(_MazeUIDragElement2D_hpp_))
#define _MazeUIDragElement2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(UIDragElement2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class UIDragElement2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API UIDragElement2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UIDragElement2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(UIDragElement2D);

        //////////////////////////////////////////
        friend class Entity;


        //////////////////////////////////////////
        enum State
        {
            None = 0,
            PrepareToDrag,
            Dragging
        };

    public:

        //////////////////////////////////////////
        virtual ~UIDragElement2D();

        //////////////////////////////////////////
        static UIDragElement2DPtr Create();


        //////////////////////////////////////////
        UIElement2DPtr const& getElement() const { return m_element; }


        //////////////////////////////////////////
        State getState() const { return m_state; }


        //////////////////////////////////////////
        void processPress(Vec2S const& _pos);

        //////////////////////////////////////////
        void processMove(Vec2S const& _pos);

        //////////////////////////////////////////
        void processRelease();

    protected:

        //////////////////////////////////////////
        UIDragElement2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        

        //////////////////////////////////////////
        void setState(State _state);

    protected:
        UIElement2DPtr m_element;

        State m_state = State::None;
        Vec2S m_startPos = Vec2S::c_zero;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUIDragElement2D_hpp_
//////////////////////////////////////////
