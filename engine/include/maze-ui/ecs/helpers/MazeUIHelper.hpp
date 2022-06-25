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
#if (!defined(_MazeUIHelper_hpp_))
#define _MazeUIHelper_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeColorEdit2D.hpp"
#include "maze-ui/ecs/components/MazeColorHDREdit2D.hpp"
#include "maze-ui/ecs/components/MazeColorGradientEdit2D.hpp"
#include "maze-ui/ecs/components/MazeAnimationCurveEdit2D.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListItem2D.hpp"
#include "maze-ui/ecs/components/MazeMenuList2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-ui/ecs/components/MazeMenuBar2D.hpp"
#include "maze-ui/ecs/components/MazeScrollbar2D.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeTextRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Namespace UIHelper
    namespace UIHelper
    {
        //////////////////////////////////////////
        MAZE_UI_API SystemTextEditBox2DPtr CreateDefaultEditBox(
            CString _text,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API HorizontalLayout2DPtr CreateHorizontalLayout(
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API VerticalLayout2DPtr CreateVerticalLayout(
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateDefaultClickButton(
            CString _text,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateClickButton(
            SpritePtr const& _default,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ToggleButton2DPtr CreateToggleButton(
            SpritePtr const& _default,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f),
            ColorU32 _normalColor = { 245, 245, 245 },
            ColorU32 _focusedColor = { 213, 231, 248 },
            ColorU32 _checkedColor = { 200, 220, 240 },
            ColorU32 _pressedColor = { 213, 231, 248 },
            ColorU32 _focusedPressedColor = { 213, 231, 248 });

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateClickButton(
            SpritePtr const& _default,
            SpritePtr const& _focused,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ToggleButton2DPtr CreateDefaultToggleButton(
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API SystemTextDropdown2DPtr CreateDefaultDropdown(
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ColorEdit2DPtr CreateDefaultColorEdit(
            ColorU32 _color,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ColorHDREdit2DPtr CreateDefaultColorHDREdit(
            ColorF128 _color,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f),
            bool _hdrLabel = true,
            bool _alphaBand = true);

        //////////////////////////////////////////
        MAZE_UI_API ColorGradientEdit2DPtr CreateDefaultColorGradientEdit(
            ColorGradient const& _gradient,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API AnimationCurveEdit2DPtr CreateDefaultAnimationCurveEdit(
            AnimationCurve const& _curve,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API Slider2DPtr CreateDefaultSlider(
            F32 _value,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API MenuListItem2DPtr CreateDefaultMenuListItem(
            String const& _name,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API MenuList2DPtr CreateDefaultMenuList(
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API MenuListTree2DPtr CreateDefaultMenuListTree(
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.0f, 1.0f));

        //////////////////////////////////////////
        MAZE_UI_API MenuBar2DPtr CreateDefaultMenuBarList(
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.0f, 1.0f),
            Vec2DF const& _pivot = Vec2DF(0.0f, 1.0f));

        //////////////////////////////////////////
        MAZE_UI_API Scrollbar2DPtr CreateDefaultScrollbarHorizontal(
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.0f, 1.0f),
            Vec2DF const& _pivot = Vec2DF(0.0f, 1.0f));

        //////////////////////////////////////////
        MAZE_UI_API Scrollbar2DPtr CreateDefaultScrollbarVertical(
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.0f, 1.0f),
            Vec2DF const& _pivot = Vec2DF(0.0f, 1.0f));

        //////////////////////////////////////////
        MAZE_UI_API ScrollRect2DPtr CreateDefaultScrollRect(
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.0f, 1.0f),
            Vec2DF const& _pivot = Vec2DF(0.0f, 1.0f),
            bool _horizontalScrollbar = true,
            bool _verticalScrollbar = true);

        //////////////////////////////////////////
        MAZE_UI_API TextRenderer2DPtr CreateText(
            CString _text,
            U32 _fontSize,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2DF const& _size,
            Vec2DF const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _anchor = Vec2DF(0.5f, 0.5f),
            Vec2DF const& _pivot = Vec2DF(0.5f, 0.5f));

    } // namespace UIHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUIHelper_hpp_
//////////////////////////////////////////
