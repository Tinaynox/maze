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
#if (!defined(_BaseSceneExample_hpp_))
#define _BaseSceneExample_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "ExampleHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(BaseSceneExample);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ExampleFPSCameraController);
    MAZE_USING_SHARED_PTR(Rotor3D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer3D);
    MAZE_USING_SHARED_PTR(LevelBloomController);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Light3D);


    //////////////////////////////////////////
    // Class BaseSceneExample
    //
    //////////////////////////////////////////
    class BaseSceneExample
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(BaseSceneExample, EcsRenderScene);

    public:
    
        //////////////////////////////////////////
        virtual ~BaseSceneExample();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        BaseSceneExample();

        //////////////////////////////////////////
        bool init(Vec2F const& _levelSize);

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2F const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void notifyRenderTargetResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void processCursorPress(Vec2F32 const& _positionOS, CursorInputEvent const& _event);

        //////////////////////////////////////////
        void processCursorDrag(Vec2F32 const& _positionOS, CursorInputEvent const& _event);


        //////////////////////////////////////////
        virtual void updateHintText();


        //////////////////////////////////////////
        void notifyExampleCommonSettingsChanged(bool const& _value);

        //////////////////////////////////////////
        virtual void updateRenderBuffersSize();

        //////////////////////////////////////////
        void updateRenderTarget();

        //////////////////////////////////////////
        virtual void updateRenderTargetViewport();


        //////////////////////////////////////////
        virtual Rect2F getMainViewport();

        //////////////////////////////////////////
        virtual Vec2U32 getMainRenderBufferSize();

    protected:
        CanvasPtr m_canvas;
        CanvasPtr m_canvasUI;
        Camera3DPtr m_camera3D;

        Light3DPtr m_mainLight3D;

        RenderBufferPtr m_distortionRenderBuffer;
        Camera3DPtr m_distortionCamera3D;

        MaterialPtr m_postFXMaterial;

        SpriteRenderer2DPtr m_renderColorSprite;
        

        RenderBufferPtr m_renderBuffer;
        RenderBufferPtr m_renderBufferMSAA;
        LevelBloomControllerPtr m_bloomController;

        // RenderBufferPtr m_shadowBuffer;

        ExampleHelper::SimpleLevelConfig m_simpleLevelConfig;

        SystemTextRenderer2DPtr m_hintText;
        
        ExampleFPSCameraControllerPtr m_fpsController;

        Vec2F32 m_cursorPositionLastFrame = Vec2F32::c_zero;

        F32 m_cursorDragDeltaKoef = 0.0065f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _BaseSceneExample_hpp_
//////////////////////////////////////////
