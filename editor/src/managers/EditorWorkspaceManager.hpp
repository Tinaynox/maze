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
#if (!defined(_EditorWorkspaceManager_hpp_))
#define _EditorWorkspaceManager_hpp_


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
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "scenes/SceneWorkspace.hpp"
#include "scenes/SceneDragAndDropWorkspace.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorWorkspaceManager);
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(SceneWorkspace);
    MAZE_USING_SHARED_PTR(SceneWorkspaceTools);


    //////////////////////////////////////////
    // Class EditorWorkspaceManager
    //
    //////////////////////////////////////////
    class EditorWorkspaceManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~EditorWorkspaceManager();

        //////////////////////////////////////////
        static void Initialize(EditorWorkspaceManagerPtr& _manager);
        

        //////////////////////////////////////////
        static inline EditorWorkspaceManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorWorkspaceManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        inline RenderBufferPtr const& getWorkspaceRenderBuffer() const { return m_workspaceRenderBuffer; }

        //////////////////////////////////////////
        inline SceneWorkspacePtr const& getSceneWorkspace() const { return m_sceneWorkspace; }

        //////////////////////////////////////////
        inline SceneWorkspaceToolsPtr const& getSceneWorkspaceTools() const { return m_sceneWorkspaceTools; }


        //////////////////////////////////////////
        void start();

        //////////////////////////////////////////
        void createScenes();

        //////////////////////////////////////////
        void destroyScenes();

        //////////////////////////////////////////
        void clearWorkspace();

        //////////////////////////////////////////
        Vec2U32 calculateWorkspaceRenderBuffer();

    protected:

        //////////////////////////////////////////
        EditorWorkspaceManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void notifyMainRenderWindowResized(RenderTarget* _renderTarget);

    protected:
        static EditorWorkspaceManager* s_instance;

        RenderBufferPtr m_workspaceRenderBuffer;
        SceneWorkspacePtr m_sceneWorkspace;
        SceneWorkspaceToolsPtr m_sceneWorkspaceTools;
        SceneDragAndDropWorkspacePtr m_sceneDragAndDropWorkspace;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorWorkspaceManager_hpp_
//////////////////////////////////////////
