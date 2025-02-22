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
#if (!defined(_MazeColorSliderTag2D_hpp_))
#define _MazeColorSliderTag2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ColorSliderTag2D);
    MAZE_USING_SHARED_PTR(Bounds2D);


    //////////////////////////////////////////
    // Class ColorSliderTag2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API ColorSliderTag2D
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ColorSliderTag2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ColorSliderTag2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ColorSliderTag2D();

        //////////////////////////////////////////
        static ColorSliderTag2DPtr Create();


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }


        //////////////////////////////////////////
        inline void setColorRenderer(SpriteRenderer2DPtr const& _value)
        {
            if (m_colorRenderer == _value)
                return;

            m_colorRenderer = _value;
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getColorRenderer() const { return m_colorRenderer; }

        //////////////////////////////////////////
        inline void setColorRenderer(ComponentPtr _value) { setColorRenderer(_value ? _value->cast<SpriteRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getColorRendererComponent() const { return m_colorRenderer; }


        //////////////////////////////////////////
        inline void setToggleButton(ToggleButton2DPtr const& _value)
        {
            if (m_toggleButton == _value)
                return;

            m_toggleButton = _value;
        }

        //////////////////////////////////////////
        inline ToggleButton2DPtr const& getToggleButton() const { return m_toggleButton; }

        //////////////////////////////////////////
        inline void setToggleButton(ComponentPtr _value) { setToggleButton(_value ? _value->cast<ToggleButton2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getToggleButtonComponent() const { return m_toggleButton; }


    protected:

        //////////////////////////////////////////
        ColorSliderTag2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
        

    protected:
        Transform2DPtr m_transform;
        SpriteRenderer2DPtr m_colorRenderer;
        ToggleButton2DPtr m_toggleButton;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorSliderTag2D_hpp_
//////////////////////////////////////////
