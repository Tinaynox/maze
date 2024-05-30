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
#if (!defined(_SceneEditorToolsTest_hpp_))
#define _SceneEditorToolsTest_hpp_


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
#include "maze-graphics/ecs/MazeEcsRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneEditorToolsTest);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(MenuListTree2D);
    MAZE_USING_SHARED_PTR(ScrollRect2D);
    

    //////////////////////////////////////////
    // Class SceneEditorToolsTest
    //
    //////////////////////////////////////////
    class SceneEditorToolsTest
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneEditorToolsTest, EcsRenderScene);

    public:

        //////////////////////////////////////////
        static SceneEditorToolsTestPtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneEditorToolsTest();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SceneEditorToolsTest();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyMainRenderWindowViewportChanged(Rect2DF const& _mainRenderWindowViewport);

        //////////////////////////////////////////
        void create3D();

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        Transform2DPtr const& createF32Edit(
            CString _name,
            Transform2DPtr const& _parent);

        //////////////////////////////////////////
        Transform2DPtr const& createVec3F32Edit(
            CString _name,
            Transform2DPtr const& _parent);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _data);

    protected:
        Camera3DPtr m_camera3D;
        CanvasPtr m_canvas;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneEditorToolsTest_hpp_
//////////////////////////////////////////
