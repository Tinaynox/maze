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
#if (!defined(_SceneExample_hpp_))
#define _SceneExample_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "ExampleSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneExample);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Rigidbody2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(LevelBloomController);
    MAZE_USING_SHARED_PTR(ParticleSystem3D);
    MAZE_USING_SHARED_PTR(WaterRenderer3D);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(CanvasGroup);
    MAZE_USING_SHARED_PTR(TerrainMesh3D);
    MAZE_USING_SHARED_PTR(RenderWaterController);


    //////////////////////////////////////////
    // Class SceneExample
    //
    //////////////////////////////////////////
    class SceneExample
        : public ECSRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneExample, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneExamplePtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneExample();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderBufferPtr const& getRenderBuffer() const { return m_renderBuffer; }

    protected:

        //////////////////////////////////////////
        SceneExample();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void notifyRenderTargetResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void createParticleSystem();

        //////////////////////////////////////////
        void notifyMouse(InputEventMouseData const& _data);

        //////////////////////////////////////////
        void notifyTouch(InputEventTouchData const& _data);

        //////////////////////////////////////////
        void notifyExampleSettingsChanged(bool const& _value);

        //////////////////////////////////////////
        void notifyExampleSettingsChanged(ExampleWaterRenderMode const& _value);

        //////////////////////////////////////////
        void updateDebugMenu();

        //////////////////////////////////////////
        ToggleButton2DPtr createDebugMenuToggleButton(
            CString _title,
            Transform2DPtr const& _parent);

        //////////////////////////////////////////
        void updateParticleSystem();

        //////////////////////////////////////////
        void updateWater();

        //////////////////////////////////////////
        void updateBloom();

    protected:
        CanvasPtr m_canvas;
        Camera3DPtr m_camera3D;
        
        RenderWaterControllerPtr m_renderWaterController;

        SpriteRenderer2DPtr m_renderColorSprite;
        SpriteRenderer2DPtr m_renderDepthSprite;

        SpriteRenderer2DPtr m_testSprite1;
        SpriteRenderer2DPtr m_testSprite2;

        RenderBufferPtr m_renderBuffer;

        TerrainMesh3DPtr m_terrainMesh;

        WaterRenderer3DPtr m_waterRenderer;

        LevelBloomControllerPtr m_bloomController;

        ParticleSystem3DPtr m_particleSystem;

        F32 m_yawAngle = -2.565f;
        F32 m_pitchAngle = 0.253f;
        Vec2F32 m_cursorPositionLastFrame = Vec2F32::c_zero;
        bool m_cursorDrag = false;


        CanvasGroupPtr m_debugMenuCanvasGroup;
        ToggleButton2DPtr m_debugMenuButton;
        SpriteRenderer2DPtr m_debugMenuBackground;
        SpriteRenderer2DPtr m_debugMenuBackgroundCollapsed;

        ToggleButton2DPtr m_debugMenuBloomButton;
        ToggleButton2DPtr m_debugMenuParticlesButton;
        ToggleButton2DPtr m_debugMenuWaterButton;
        ToggleButton2DPtr m_debugMenuExampleWaterRenderModeButtons[(Size)ExampleWaterRenderMode::MAX];
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneExample_hpp_
//////////////////////////////////////////
