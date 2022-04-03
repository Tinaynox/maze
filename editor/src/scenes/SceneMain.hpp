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
#if (!defined(_SceneMain_hpp_))
#define _SceneMain_hpp_


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
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneMain);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(AssetsController);
    

    //////////////////////////////////////////
    // Class SceneMain
    //
    //////////////////////////////////////////
    class SceneMain
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneMain, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneMainPtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneMain();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SceneMain();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMouse(InputEventMouseData const& _data);

        //////////////////////////////////////////
        void create3D();

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void notifyMainRenderWindowResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void notifyCurrentAssetsFullPath(String const& _currentAssetsFullPath);

        //////////////////////////////////////////
        void updateAssetsController();

    protected:
        Camera3DPtr m_camera3D;
        Vec3DF m_camera3DTargetPosition = Vec3DF::c_zero;
        CanvasPtr m_mainCanvas;

        CanvasPtr m_topMenuBarCanvas;
        CanvasPtr m_hierarchyCanvas;
        CanvasPtr m_inspectorCanvas;
        CanvasPtr m_assetsCanvas;
        CanvasPtr m_previewCanvas;

        AssetsControllerPtr m_assetsController;

        F32 m_yawAngle;
        F32 m_pitchAngle;
        Vec2DF m_cursorPositionLastFrame;
        bool m_cursorDrag;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneMain_hpp_
//////////////////////////////////////////
