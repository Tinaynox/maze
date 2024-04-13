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
#include "maze-ui/fonts/MazeFontMaterial.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeDropdown2D.hpp"
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
        MAZE_UI_API EditBox2DPtr CreateDefaultEditBox(
            CString _text,
            FontMaterialPtr const& _fontMaterial,
            U32 _fontSize,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f),
            HorizontalAlignment2D _horizontalAlignment = HorizontalAlignment2D::Left,
            VerticalAlignment2D _verticalAlignment = VerticalAlignment2D::Middle);

        //////////////////////////////////////////
        inline EditBox2DPtr CreateDefaultEditBox(
            CString _text,
            U32 _fontSize,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f),
            HorizontalAlignment2D _horizontalAlignment = HorizontalAlignment2D::Left,
            VerticalAlignment2D _verticalAlignment = VerticalAlignment2D::Middle)
        {
            return CreateDefaultEditBox(
                _text, FontMaterialPtr(), _fontSize, _size, _position, _parent, _ecsScene, _anchor, _pivot, _horizontalAlignment, _verticalAlignment);
        }

        //////////////////////////////////////////
        MAZE_UI_API Dropdown2DPtr CreateDefaultDropdown(
            U32 _fontSize,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API HorizontalLayout2DPtr CreateHorizontalLayout(
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API VerticalLayout2DPtr CreateVerticalLayout(
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateDefaultClickButton(
            CString _text,
            FontMaterialPtr const& _fontMaterial,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        inline ClickButton2DPtr CreateDefaultClickButton(
            CString _text,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f))
        {
            return CreateDefaultClickButton(_text, FontMaterialPtr(), _size, _position, _parent, _ecsScene, _anchor, _pivot);
        }

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateClickButton(
            SpritePtr const& _default,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ToggleButton2DPtr CreateToggleButton(
            SpritePtr const& _default,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f),
            ColorU32 _normalColor = ColorU32{ 245, 245, 245 },
            ColorU32 _focusedColor = ColorU32{ 213, 231, 248 },
            ColorU32 _checkedColor = ColorU32{ 200, 220, 240 },
            ColorU32 _pressedColor = ColorU32{ 213, 231, 248 },
            ColorU32 _focusedPressedColor = ColorU32{ 213, 231, 248 });

        //////////////////////////////////////////
        MAZE_UI_API ClickButton2DPtr CreateClickButton(
            SpritePtr const& _default,
            SpritePtr const& _focused,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API ToggleButton2DPtr CreateDefaultToggleButton(
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API Slider2DPtr CreateDefaultSlider(
            F32 _value,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API MenuListItem2DPtr CreateDefaultMenuListItem(
            String const& _name,
            FontMaterialPtr const& _fontMaterial,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API MenuList2DPtr CreateDefaultMenuList(
            FontMaterialPtr const& _fontMaterial,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        inline MenuList2DPtr CreateDefaultMenuList(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f))
        {
            return CreateDefaultMenuList(FontMaterialPtr(), _size, _position, _parent, _ecsScene, _anchor, _pivot);
        }

        //////////////////////////////////////////
        MAZE_UI_API MenuListTree2DPtr CreateDefaultMenuListTree(
            FontMaterialPtr const& _fontMaterial,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.0f, 1.0f));

        //////////////////////////////////////////
        inline MenuListTree2DPtr CreateDefaultMenuListTree(            
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.0f, 1.0f))
        {
            return CreateDefaultMenuListTree(FontMaterialPtr(), _position, _parent, _ecsScene, _anchor, _pivot);
        }

        //////////////////////////////////////////
        MAZE_UI_API MenuBar2DPtr CreateDefaultMenuBarList(
            FontMaterialPtr const& _fontMaterial,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.0f, 1.0f),
            Vec2F const& _pivot = Vec2F(0.0f, 1.0f));

        //////////////////////////////////////////
        inline MenuBar2DPtr CreateDefaultMenuBarList(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.0f, 1.0f),
            Vec2F const& _pivot = Vec2F(0.0f, 1.0f))
        {
            return CreateDefaultMenuBarList(FontMaterialPtr(), _size, _position, _parent, _ecsScene, _anchor, _pivot);
        }

        //////////////////////////////////////////
        MAZE_UI_API Scrollbar2DPtr CreateDefaultScrollbarHorizontal(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.0f, 1.0f),
            Vec2F const& _pivot = Vec2F(0.0f, 1.0f));

        //////////////////////////////////////////
        MAZE_UI_API Scrollbar2DPtr CreateDefaultScrollbarVertical(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.0f, 1.0f),
            Vec2F const& _pivot = Vec2F(0.0f, 1.0f));

        //////////////////////////////////////////
        MAZE_UI_API ScrollRect2DPtr CreateDefaultScrollRect(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.0f, 1.0f),
            Vec2F const& _pivot = Vec2F(0.0f, 1.0f),
            bool _horizontalScrollbar = true,
            bool _verticalScrollbar = true);

        //////////////////////////////////////////
        MAZE_UI_API TextRenderer2DPtr CreateText(
            CString _text,
            FontMaterialPtr const& _fontMaterial,
            U32 _fontSize,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API TextRenderer2DPtr CreateText(
            CString _text,
            U32 _fontSize,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API AbstractTextRenderer2DPtr CreateTextOrSystemText(
            CString _text,
            FontMaterialPtr const& _fontMaterial,
            U32 _fontSize,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));


    } // namespace UIHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeUIHelper_hpp_
//////////////////////////////////////////
