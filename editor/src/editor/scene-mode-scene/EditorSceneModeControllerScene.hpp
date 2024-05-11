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
#if (!defined(_EditorSceneModeControllerScene_hpp_))
#define _EditorSceneModeControllerScene_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
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
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "editor/EditorSceneModeController.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorSceneModeControllerScene);


    //////////////////////////////////////////
    // Class EditorSceneModeControllerScene
    //
    //////////////////////////////////////////
    class EditorSceneModeControllerScene
        : public EditorSceneModeController
    {
    public:

        //////////////////////////////////////////
        virtual ~EditorSceneModeControllerScene();

        //////////////////////////////////////////
        static EditorSceneModeControllerScenePtr Create();


    protected:

        //////////////////////////////////////////
        EditorSceneModeControllerScene();

        //////////////////////////////////////////
        bool init();    
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorSceneModeControllerScene_hpp_
//////////////////////////////////////////
