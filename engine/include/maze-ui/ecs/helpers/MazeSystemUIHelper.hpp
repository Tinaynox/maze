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
#if (!defined(_MazeSystemUIHelper_hpp_))
#define _MazeSystemUIHelper_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/math/MazeAnimationCurve.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeColorGradient.hpp"
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
    // Namespace SystemUIHelper
    namespace SystemUIHelper
    {
        //////////////////////////////////////////
        MAZE_UI_API EditBox2DPtr CreateDefaultEditBox(
            CString _text,
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

        //////////////////////////////////////////
        MAZE_UI_API Dropdown2DPtr CreateDefaultDropdown(
            Vec2F const& _size,
            Vec2F const& _position,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2F const& _anchor = Vec2F(0.5f, 0.5f),
            Vec2F const& _pivot = Vec2F(0.5f, 0.5f));

    } // namespace SystemUIHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemUIHelper_hpp_
//////////////////////////////////////////
