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
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(Light3D);


    //////////////////////////////////////////
    // Class EditorSceneModeControllerScene
    //
    //////////////////////////////////////////
    class EditorSceneModeControllerScene
        : public EditorSceneModeController
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~EditorSceneModeControllerScene();

        //////////////////////////////////////////
        static EditorSceneModeControllerScenePtr Create();


        //////////////////////////////////////////
        virtual void shutdown() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        EditorSceneModeControllerScene();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        ToggleButton2DPtr createBarButton(
            Transform2DPtr const& _parent,
            SpritePtr const& _sprite,
            ColorU32 const& _spriteColor);


        //////////////////////////////////////////
        void updateDebugLight();

        //////////////////////////////////////////
        void updateDebugAxes();

        //////////////////////////////////////////
        void updateDebugGrid();

        //////////////////////////////////////////
        void notifyDebugLightEnabledChanged(bool const& _value);

        //////////////////////////////////////////
        void notifyDebugAxesEnabledChanged(bool const& _value);

        //////////////////////////////////////////
        void notifyDebugGridEnabledChanged(bool const& _value);


        //////////////////////////////////////////
        void updateSaveButtonEnabled();

        //////////////////////////////////////////
        void notifyPrefabAssetFileSaveEnabledChanged(bool const& _value);

    protected:
        Transform2DPtr m_canvasNode;
        Transform3DPtr m_mainNode;

        Light3DPtr m_debugLight;

        ToggleButton2DPtr m_lightButton;
        ToggleButton2DPtr m_axesButton;
        ToggleButton2DPtr m_gridButton;

        SpriteRenderer2DPtr m_topBarBackground;
        HorizontalLayout2DPtr m_topBarLeftLayout;
        HorizontalLayout2DPtr m_topBarRightLayout;

        ClickButton2DPtr m_saveButton;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorSceneModeControllerScene_hpp_
//////////////////////////////////////////
