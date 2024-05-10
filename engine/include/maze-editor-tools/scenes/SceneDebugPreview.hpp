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
#if (!defined(_SceneDebugPreview_hpp_))
#define _SceneDebugPreview_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
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
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneDebugPreview);
    MAZE_USING_SHARED_PTR(InputSystem2D);
    MAZE_USING_SHARED_PTR(RenderController);


    //////////////////////////////////////////
    // Class SceneDebugPreview
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API SceneDebugPreview
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneDebugPreview, EcsRenderScene);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SceneDebugPreview);

    public:

        //////////////////////////////////////////
        static SceneDebugPreviewPtr Create(RenderTargetPtr const& _renderTarget);
    
        //////////////////////////////////////////
        virtual ~SceneDebugPreview();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void clear();

        //////////////////////////////////////////
        inline Transform3DPtr const& getPreviewNodeTransform() const { return m_previewNodeTransform; }

        //////////////////////////////////////////
        Camera3DPtr const& getCamera() const { return m_camera3D; }

        //////////////////////////////////////////
        CanvasPtr const& getCanvas() const { return m_canvas; }


        //////////////////////////////////////////
        inline F32 getYawAngle() const { return m_yawAngle; }

        //////////////////////////////////////////
        inline void setYawAngle(F32 _value) { m_yawAngle = _value; }

        //////////////////////////////////////////
        inline F32 getPitchAngle() const { return m_pitchAngle; }

        //////////////////////////////////////////
        inline void setPitchAngle(F32 _value) { m_pitchAngle = _value; }

    protected:

        //////////////////////////////////////////
        SceneDebugPreview();

        //////////////////////////////////////////
        virtual bool init(RenderTargetPtr const& _renderTarget);

        //////////////////////////////////////////
        void create3D();

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void createSystems();

        //////////////////////////////////////////
        virtual EcsWorld* assignWorld() MAZE_OVERRIDE;

    protected:
        Transform3DPtr m_previewNodeTransform;

        Camera3DPtr m_camera3D;
        CanvasPtr m_canvas;

        F32 m_yawAngle;
        F32 m_pitchAngle;
        Vec2F m_cursorPositionLastFrame;
        bool m_cursorDrag;

        EcsWorldPtr m_previewWorld;

        InputSystem2DPtr m_inputSystem2D;
        RenderControllerPtr m_renderController;        
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneDebugPreview_hpp_
//////////////////////////////////////////
