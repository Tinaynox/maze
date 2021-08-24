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
#if (!defined(_UIHelper_hpp_))
#define _UIHelper_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace UIHelper
    {
        //////////////////////////////////////////
        ClickButton2DPtr CreateDefaultGameClickButton(
            ECSRenderScene* _scene,
            Transform2DPtr const& _parent,
            Vec2DF _position,
            Vec2DF _size,
            String const& _label,
            Delegate<void, Button2D*, CursorInputEvent const&> _onClickDelegate);

        //////////////////////////////////////////
        ToggleButton2DPtr CreateDefaultGameToggleButton(
            ECSRenderScene* _scene,
            Transform2DPtr const& _parent,
            Vec2DF _position,
            Vec2DF _size,
            String const& _label,
            Delegate<void, Button2D*, CursorInputEvent const&> _onClickDelegate);

    } // namespace UIHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _UIHelper_hpp_
//////////////////////////////////////////
