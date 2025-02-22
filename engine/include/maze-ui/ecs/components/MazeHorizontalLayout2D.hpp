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
#if (!defined(_MazeHorizontalLayout2D_hpp_))
#define _MazeHorizontalLayout2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class HorizontalLayout2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API HorizontalLayout2D
        : public Layout2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(HorizontalLayout2D, Layout2D);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(HorizontalLayout2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~HorizontalLayout2D();

        //////////////////////////////////////////
        static HorizontalLayout2DPtr Create();


        //////////////////////////////////////////
        virtual void alignChildren() MAZE_OVERRIDE;

    
        //////////////////////////////////////////
        inline F32 getSpacing() const { return m_spacing; }

        //////////////////////////////////////////
        inline void setSpacing(F32 _spacing)
        {
            if (m_spacing == _spacing)
                return;

            m_spacing = _spacing;

            m_dirty = true;
        }


        //////////////////////////////////////////
        inline bool getControlChildWidth() const { return m_controlChildWidth; }

        //////////////////////////////////////////
        inline void setControlChildWidth(bool _value)
        {
            if (m_controlChildWidth == _value)
                return;

            m_controlChildWidth = _value;

            m_dirty = true;
        }

        //////////////////////////////////////////
        inline bool getControlChildHeight() const { return m_controlChildHeight; }

        //////////////////////////////////////////
        inline void setControlChildHeight(bool _value)
        {
            if (m_controlChildHeight == _value)
                return;

            m_controlChildHeight = _value;

            m_dirty = true;
        }

        //////////////////////////////////////////
        inline bool getExpand() const { return m_expand; }

        //////////////////////////////////////////
        inline void setExpand(bool _value)
        {
            if (m_expand == _value)
                return;

            m_expand = _value;

            m_dirty = true;
        }


    protected:

        //////////////////////////////////////////
        HorizontalLayout2D();

        //////////////////////////////////////////
        using Layout2D::init;
        
        //////////////////////////////////////////
        bool init();

    protected:
        F32 m_spacing;

        bool m_controlChildWidth;
        bool m_controlChildHeight;
        bool m_expand;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHorizontalLayout2D_hpp_
//////////////////////////////////////////
