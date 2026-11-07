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
#if (!defined(_MazeSystemTooltip2D_hpp_))
#define _MazeSystemTooltip2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SystemTooltip2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(Entity);


    //////////////////////////////////////////
    // Class SystemTooltip2D
    // Shows the text in a panel below the element (UIElement2D) after the cursor rests on it for the delay.
    // The panel is created on first show, on top of the root canvas
    //
    //////////////////////////////////////////
    class MAZE_UI_API SystemTooltip2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SystemTooltip2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SystemTooltip2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~SystemTooltip2D();

        //////////////////////////////////////////
        static SystemTooltip2DPtr Create();


        //////////////////////////////////////////
        inline String const& getText() const { return m_text; }

        //////////////////////////////////////////
        void setText(String const& _text);

        //////////////////////////////////////////
        inline F32 getDelay() const { return m_delay; }

        //////////////////////////////////////////
        inline void setDelay(F32 _delay) { m_delay = _delay; }


        //////////////////////////////////////////
        void update(F32 _dt);

    protected:

        //////////////////////////////////////////
        SystemTooltip2D();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processComponentRemoved() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void show();

        //////////////////////////////////////////
        void hide();

        //////////////////////////////////////////
        bool createView();

        //////////////////////////////////////////
        void updateViewPosition();

        //////////////////////////////////////////
        void destroyView();

    protected:
        String m_text;
        F32 m_delay = 0.5f;

        Transform2DPtr m_transform;
        UIElement2DPtr m_element;

        F32 m_hoverTime = 0.0f;

        EntityPtr m_viewEntity;
        Transform2DPtr m_viewTransform;
        SystemTextRenderer2DPtr m_viewText;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemTooltip2D_hpp_
//////////////////////////////////////////
