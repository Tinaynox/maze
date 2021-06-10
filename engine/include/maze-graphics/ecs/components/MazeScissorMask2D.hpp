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
#if (!defined(_MazeScissorMask2D_hpp_))
#define _MazeScissorMask2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScissorMask2D);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Bounds2D);


    //////////////////////////////////////////
    // Class ScissorMask2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ScissorMask2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ScissorMask2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ScissorMask2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ScissorMask2D();

        //////////////////////////////////////////
        static ScissorMask2DPtr Create();


        //////////////////////////////////////////
        inline Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline Bounds2DPtr const& getBounds() const { return m_bounds; }
        

        //////////////////////////////////////////
        inline F32 getPaddingLeft() const { return m_paddingLeft; }

        //////////////////////////////////////////
        inline void setPaddingLeft(F32 _paddingLeft)
        {
            if (m_paddingLeft == _paddingLeft)
                return;

            m_paddingLeft = _paddingLeft;
            m_scissorBoundsDirty = true;
        }

        //////////////////////////////////////////
        inline F32 getPaddingRight() const { return m_paddingRight; }

        //////////////////////////////////////////
        inline void setPaddingRight(F32 _paddingRight)
        {
            if (m_paddingRight == _paddingRight)
                return;

            m_paddingRight = _paddingRight;
            m_scissorBoundsDirty = true;
        }

        //////////////////////////////////////////
        inline F32 getPaddingTop() const { return m_paddingTop; }

        //////////////////////////////////////////
        inline void setPaddingTop(F32 _paddingTop)
        {
            if (m_paddingTop == _paddingTop)
                return;

            m_paddingTop = _paddingTop;
            m_scissorBoundsDirty = true;
        }

        //////////////////////////////////////////
        inline F32 getPaddingBottom() const { return m_paddingBottom; }

        //////////////////////////////////////////
        inline void setPaddingBottom(F32 _paddingBottom)
        {
            if (m_paddingBottom == _paddingBottom)
                return;

            m_paddingBottom = _paddingBottom;
            m_scissorBoundsDirty = true;
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
        AABB2D const& getScissorBounds();

    protected:

        //////////////////////////////////////////
        ScissorMask2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();
            
        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyBoundsCalculated(AABB2D const& _bounds);

    protected:
        Transform2DPtr m_transform;
        Bounds2DPtr m_bounds;

        F32 m_paddingLeft;
        F32 m_paddingRight;
        F32 m_paddingTop;
        F32 m_paddingBottom;

        AABB2D m_scissorBounds;
        bool m_scissorBoundsDirty;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScissorMask2D_hpp_
//////////////////////////////////////////
