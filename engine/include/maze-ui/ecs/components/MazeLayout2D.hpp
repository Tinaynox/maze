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
#if (!defined(_MazeLayout2D_hpp_))
#define _MazeLayout2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Layout2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class Layout2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API Layout2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Layout2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Layout2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Layout2D();


        //////////////////////////////////////////
        void update();

        //////////////////////////////////////////
        virtual void alignChildren() MAZE_ABSTRACT;


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        inline void dirty() { m_dirty = true; }


        //////////////////////////////////////////
        inline F32 getPaddingLeft() const { return m_paddingLeft; }

        //////////////////////////////////////////
        inline void setPaddingLeft(F32 _paddingLeft)
        {
            if (m_paddingLeft == _paddingLeft)
                return;

            m_paddingLeft = _paddingLeft;

            dirty();
        }

        //////////////////////////////////////////
        inline F32 getPaddingRight() const { return m_paddingRight; }

        //////////////////////////////////////////
        inline void setPaddingRight(F32 _paddingRight)
        {
            if (m_paddingRight == _paddingRight)
                return;

            m_paddingRight = _paddingRight;

            dirty();
        }

        //////////////////////////////////////////
        inline F32 getPaddingTop() const { return m_paddingTop; }

        //////////////////////////////////////////
        inline void setPaddingTop(F32 _paddingTop)
        {
            if (m_paddingTop == _paddingTop)
                return;

            m_paddingTop = _paddingTop;

            dirty();
        }

        //////////////////////////////////////////
        inline F32 getPaddingBottom() const { return m_paddingBottom; }

        //////////////////////////////////////////
        inline void setPaddingBottom(F32 _paddingBottom)
        {
            if (m_paddingBottom == _paddingBottom)
                return;

            m_paddingBottom = _paddingBottom;

            dirty();
        }

        //////////////////////////////////////////
        inline void setPadding(
            F32 _left,
            F32 _right,
            F32 _bottom,
            F32 _top)
        {
            setPaddingLeft(_left);
            setPaddingRight(_right);
            setPaddingBottom(_bottom);
            setPaddingTop(_top);
        }
        

        //////////////////////////////////////////
        inline HorizontalAlignment2D getHorizontalAlignment() const { return m_horizontalAlignment; }

        //////////////////////////////////////////
        inline void setHorizontalAlignment(HorizontalAlignment2D _value)
        {
            if (m_horizontalAlignment == _value)
                return;

            m_horizontalAlignment = _value;

            dirty();
        }

        //////////////////////////////////////////
        inline VerticalAlignment2D getVerticalAlignment() const { return m_verticalAlignment; }

        //////////////////////////////////////////
        inline void setVerticalAlignment(VerticalAlignment2D _value)
        {
            if (m_verticalAlignment == _value)
                return;

            m_verticalAlignment = _value;

            dirty();
        }


        //////////////////////////////////////////
        inline bool getReverseOrder() const { return m_reverseOrder; }

        //////////////////////////////////////////
        inline void setReverseOrder(bool _value)
        {
            if (m_reverseOrder == _value)
                return;

            m_reverseOrder = _value;

            dirty();
        }


        //////////////////////////////////////////
        inline bool getAutoWidth() const { return m_autoWidth; }

        //////////////////////////////////////////
        inline void setAutoWidth(bool _value)
        {
            if (m_autoWidth == _value)
                return;

            m_autoWidth = _value;

            dirty();
        }

        //////////////////////////////////////////
        inline bool getAutoHeight() const { return m_autoHeight; }

        //////////////////////////////////////////
        inline void setAutoHeight(bool _value)
        {
            if (m_autoHeight == _value)
                return;

            m_autoHeight = _value;

            dirty();
        }

    protected:

        //////////////////////////////////////////
        Layout2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void processChildren(std::function<void(Transform2D*)> _func);

        //////////////////////////////////////////
        Size getActiveChildrenCount();

    protected:
        Transform2DPtr m_transform;

        bool m_dirty;

        F32 m_paddingLeft;
        F32 m_paddingRight;
        F32 m_paddingTop;
        F32 m_paddingBottom;

        HorizontalAlignment2D m_horizontalAlignment;
        VerticalAlignment2D m_verticalAlignment;

        bool m_reverseOrder;

        bool m_autoWidth;
        bool m_autoHeight;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLayout2D_hpp_
//////////////////////////////////////////
