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
#if (!defined(_MazeOverlayLayout2D_hpp_))
#define _MazeOverlayLayout2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(OverlayLayout2D);


    //////////////////////////////////////////
    // Class OverlayLayout2D
    // Children are stacked on top of each other inside the padding (e.g. a button: background + label).
    // The content size is the largest child preferred size; children that don't answer
    // PreferredSize2DQueryEvent are ignored on the axes they are stretched along
    //
    //////////////////////////////////////////
    class MAZE_UI_API OverlayLayout2D
        : public Layout2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(OverlayLayout2D, Layout2D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(OverlayLayout2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~OverlayLayout2D();

        //////////////////////////////////////////
        static OverlayLayout2DPtr Create();


        //////////////////////////////////////////
        virtual void alignChildren() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Vec2F calculatePreferredSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool getControlsChildWidth() const MAZE_OVERRIDE { return m_controlChildWidth; }

        //////////////////////////////////////////
        virtual bool getControlsChildHeight() const MAZE_OVERRIDE { return m_controlChildHeight; }


        //////////////////////////////////////////
        // Stretches children to the padded width
        inline bool getControlChildWidth() const { return m_controlChildWidth; }

        //////////////////////////////////////////
        inline void setControlChildWidth(bool _value)
        {
            if (m_controlChildWidth == _value)
                return;

            m_controlChildWidth = _value;

            dirty();
        }

        //////////////////////////////////////////
        inline bool getControlChildHeight() const { return m_controlChildHeight; }

        //////////////////////////////////////////
        inline void setControlChildHeight(bool _value)
        {
            if (m_controlChildHeight == _value)
                return;

            m_controlChildHeight = _value;

            dirty();
        }


        //////////////////////////////////////////
        // Largest child preferred size, without padding
        Vec2F calculateContentSize();

    protected:

        //////////////////////////////////////////
        OverlayLayout2D();

        //////////////////////////////////////////
        using Layout2D::init;

        //////////////////////////////////////////
        bool init();

    protected:
        bool m_controlChildWidth = false;
        bool m_controlChildHeight = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeOverlayLayout2D_hpp_
//////////////////////////////////////////
