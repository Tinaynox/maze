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
#if (!defined(_MazeEditorToolsUIHelper_hpp_))
#define _MazeEditorToolsUIHelper_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/utils/MazeDelegate.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace EditorToolsUIHelper
    {
        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ToggleButton2DPtr CreateDefaultToggleButton(
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ToggleButton2DPtr CreateDefaultToggleButton(
            SpritePtr const& _sprite,
            ColorU32 const& _spriteColor,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _spriteSize = Vec2DF(14.0f, 14.0f));

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ClickButton2DPtr CreateDefaultClickButton(
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene);

        //////////////////////////////////////////
        MAZE_EDITOR_TOOLS_API ClickButton2DPtr CreateDefaultClickButton(
            SpritePtr const& _sprite,
            ColorU32 const& _spriteColor,
            Transform2DPtr const& _parent,
            ECSScene* _ecsScene,
            Vec2DF const& _spriteSize = Vec2DF(14.0f, 14.0f));


    } // namespace EditorToolsUIHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorToolsUIHelper_hpp_
//////////////////////////////////////////
