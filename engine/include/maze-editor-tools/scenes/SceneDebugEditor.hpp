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
#if (!defined(_SceneDebugEditor_hpp_))
#define _SceneDebugEditor_hpp_


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
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneDebugEditor);


    //////////////////////////////////////////
    // Class SceneDebugEditor
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API SceneDebugEditor
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneDebugEditor, ECSRenderScene);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SceneDebugEditor);

    public:

        //////////////////////////////////////////
        static SceneDebugEditorPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneDebugEditor();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


    protected:

        //////////////////////////////////////////
        SceneDebugEditor();

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);

        //////////////////////////////////////////
        void notifyMouse(InputEventMouseData const& _data);

        //////////////////////////////////////////
        void create3D();

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void createSystems();


    protected:

        CanvasPtr m_hierarchyCanvas;
        Rect2DF m_hierarchyViewport;

        CanvasPtr m_inspectorCanvas;
        Rect2DF m_inspectorViewport;

        CanvasPtr m_assetsCanvas;
        Rect2DF m_assetsViewport;

        CanvasPtr m_topBarCanvas;
        Rect2DF m_topBarViewport;

        Camera3DPtr m_camera3D;
        Rect2DF m_sceneViewport;

        F32 m_yawAngle;
        F32 m_pitchAngle;
        Vec2DF m_cursorPositionLastFrame;
        bool m_cursorDrag;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneDebugEditor_hpp_
//////////////////////////////////////////
