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
#if (!defined(_SceneEditor_hpp_))
#define _SceneEditor_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
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
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SceneEditor);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(AssetsController);
    MAZE_USING_SHARED_PTR(EditorSceneModeController);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(DebugGridRenderer);
    

    //////////////////////////////////////////
    // Class SceneEditor
    //
    //////////////////////////////////////////
    class SceneEditor
        : public EcsRenderScene
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SceneEditor, EcsRenderScene);

    public:

        //////////////////////////////////////////
        static SceneEditorPtr Create();
    
        //////////////////////////////////////////
        virtual ~SceneEditor();

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline CanvasPtr const& getWorkspaceCanvas() const { return m_workspaceCanvas; }


    protected:

        //////////////////////////////////////////
        SceneEditor();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void create3D();

        //////////////////////////////////////////
        void create2D();

        //////////////////////////////////////////
        void notifyMainRenderWindowResized(RenderTarget* _renderTarget);

        //////////////////////////////////////////
        void notifyCurrentAssetsFullPath(String const& _currentAssetsFullPath);

        //////////////////////////////////////////
        void notifySceneModeChanged(EditorSceneMode _mode);

        //////////////////////////////////////////
        void notifyPlaytestModeEnabledChanged(bool _value);

        //////////////////////////////////////////
        void updateAssetsController();


        //////////////////////////////////////////
        void destroySceneModeController();

        //////////////////////////////////////////
        void createSceneModeController();

    protected:
        EditorSceneModeControllerPtr m_sceneModeController;

        
        CanvasPtr m_workspaceCanvas;
        SpritePtr m_workspaceSprite;
        SpriteRenderer2DPtr m_workspaceSpriteRenderer;

        CanvasPtr m_topMenuBarCanvas;
        CanvasPtr m_hierarchyCanvas;
        CanvasPtr m_inspectorCanvas;
        CanvasPtr m_assetsCanvas;
        CanvasPtr m_previewCanvas;
        CanvasPtr m_topBarCanvas;

        AssetsControllerPtr m_assetsController;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _SceneEditor_hpp_
//////////////////////////////////////////
