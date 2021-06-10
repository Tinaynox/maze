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
#if (!defined(_MazeColorEdit2D_hpp_))
#define _MazeColorEdit2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(ColorEdit2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class ColorEdit2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API ColorEdit2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ColorEdit2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ColorEdit2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ColorEdit2D();

        //////////////////////////////////////////
        static ColorEdit2DPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }
            
        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }


        //////////////////////////////////////////
        inline bool getSelected() const { return m_selected; }

        //////////////////////////////////////////
        void setSelected(bool _selected);


        //////////////////////////////////////////
        inline void setColorRenderer(SpriteRenderer2DPtr const& _colorRenderer)
        {
            if (m_colorRenderer == _colorRenderer)
                return;

            m_colorRenderer = _colorRenderer;

            processColor();
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getColorRenderer() const { return m_colorRenderer; }

        //////////////////////////////////////////
        inline void setColorRenderer(ComponentPtr _avatar) { setColorRenderer(_avatar ? _avatar->cast<SpriteRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getColorRendererComponent() const { return m_colorRenderer; }


        //////////////////////////////////////////
        inline void setAlphaRenderer(SpriteRenderer2DPtr const& _alphaRenderer)
        {
            if (m_alphaRenderer == _alphaRenderer)
                return;

            m_alphaRenderer = _alphaRenderer;

            processColor();
        }

        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getAlphaRenderer() const { return m_alphaRenderer; }

        //////////////////////////////////////////
        inline void setAlphaRenderer(ComponentPtr _avatar) { setAlphaRenderer(_avatar ? _avatar->cast<SpriteRenderer2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getAlphaRendererComponent() const { return m_alphaRenderer; }


        //////////////////////////////////////////
        void setColor(ColorU32 const& _color);

        //////////////////////////////////////////
        inline ColorU32 const& getColor() const { return m_color; }


    public:

        MultiDelegate<ColorEdit2D*> eventClick;
        MultiDelegate<ColorEdit2D*, bool> eventFocusChanged;
        MultiDelegate<ColorEdit2D*, bool> eventPressedChanged;
        MultiDelegate<ColorEdit2D*, bool> eventSelectedChanged;
        MultiDelegate<ColorEdit2D*, ColorU32 const&> eventColorChanged;

    protected:

        //////////////////////////////////////////
        ColorEdit2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            
        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorReleaseOut(CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyFocusChanged(bool _focused);

        //////////////////////////////////////////
        void notifyPressedChanged(bool _pressed);

        //////////////////////////////////////////
        void notifyClick(CursorInputEvent const& _inputEvent);


        //////////////////////////////////////////
        void processColor();

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        SpriteRenderer2DPtr m_colorRenderer;
        SpriteRenderer2DPtr m_alphaRenderer;
        
        ColorU32 m_color;

        bool m_selected;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorEdit2D_hpp_
//////////////////////////////////////////
