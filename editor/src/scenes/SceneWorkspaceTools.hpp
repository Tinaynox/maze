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
#if (!defined(_SceneWorkspaceTools_hpp_))
#define _SceneWorkspaceTools_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/MazeBaseTypes.hpp"
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
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneWorkspaceTools);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(DebugGridRenderer);
    MAZE_USING_SHARED_PTR(Camera3D);


    //////////////////////////////////////////
    // Class SceneWorkspaceTools
    //
    //////////////////////////////////////////
    class SceneWorkspaceTools
        : public ECSRenderScene
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneWorkspaceTools, ECSRenderScene);

    public:

        //////////////////////////////////////////
        static SceneWorkspaceToolsPtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneWorkspaceTools();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;



        //////////////////////////////////////////
        inline Camera3DPtr const& getCamera3D() const { return m_camera3D; }

        //////////////////////////////////////////
        inline MeshRendererPtr const& getDebugAxesRenderer() const { return m_debugAxesRenderer; }

        //////////////////////////////////////////
        inline DebugGridRendererPtr const& getDebugGridRenderer() const { return m_debugGridRenderer; }

        //////////////////////////////////////////
        inline CanvasPtr const& getMainCanvas() const { return m_mainCanvas; }

    protected:

        //////////////////////////////////////////
        SceneWorkspaceTools();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMouse(InputEventMouseData const& _data);


        //////////////////////////////////////////
        virtual ECSWorld* assignWorld() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void notifyMainRenderWindowResized(RenderTarget* _renderTarget);

    protected:
        MeshRendererPtr m_meshRenderer;

        Camera3DPtr m_camera3D;
        Vec3DF m_camera3DTargetPosition = Vec3DF::c_zero;
        MeshRendererPtr m_debugAxesRenderer;
        DebugGridRendererPtr m_debugGridRenderer;

        F32 m_yawAngle = 0.0f;
        F32 m_pitchAngle = 0.0f;
        Vec2DF m_cursorPositionLastFrame = Vec2DF::c_zero;
        bool m_cursorDrag = false;

        CanvasPtr m_mainCanvas;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneWorkspaceTools_hpp_
//////////////////////////////////////////
