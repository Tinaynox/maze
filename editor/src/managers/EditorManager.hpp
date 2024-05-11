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
#if (!defined(_EditorManager_hpp_))
#define _EditorManager_hpp_


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
#include "editor/EditorSceneMode.hpp"
#include "editor/EditorMode.hpp"
#include "managers/EditorPrefabManager.hpp"
#include "scenes/SceneMain.hpp"
#include "scenes/SceneMainTools.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorManager);
    MAZE_USING_SHARED_PTR(EditorAssetsManager);
    MAZE_USING_SHARED_PTR(EditorAssetsModeManager);
    MAZE_USING_SHARED_PTR(EditorProjectModeManager);
    MAZE_USING_SHARED_PTR(EditorGizmosManager);
    MAZE_USING_SHARED_PTR(EditorEntityManager);
    MAZE_USING_SHARED_PTR(EditorWorkspaceManager);
    MAZE_USING_SHARED_PTR(EditorPlaytestManager);
    MAZE_USING_SHARED_PTR(EditorUpdateManager);
   

    //////////////////////////////////////////
    // Class EditorManager
    //
    //////////////////////////////////////////
    class EditorManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~EditorManager();

        //////////////////////////////////////////
        static void Initialize(EditorManagerPtr& _gameManager);
       


        //////////////////////////////////////////
        static inline EditorManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        EditorPrefabManagerPtr const& getEditorPrefabManager() const { return m_editorPrefabManager; }


        //////////////////////////////////////////
        EditorMode getMode() const;


        //////////////////////////////////////////
        void setSceneMode(EditorSceneMode _mode);

        //////////////////////////////////////////
        inline EditorSceneMode getSceneMode() const { return m_sceneMode; }



        //////////////////////////////////////////
        void clearMode();


        //////////////////////////////////////////
        void openPrefab(EntityPtr const& _value);

        //////////////////////////////////////////
        void openPrefab(AssetFilePtr const& _value);

        //////////////////////////////////////////
        EntityPtr createNewPrefab();


        //////////////////////////////////////////
        void start();


        //////////////////////////////////////////
        inline String const& getCurrentEditFileFullPath() const { return m_currentEditFileFullPath; }

        //////////////////////////////////////////
        inline void setCurrentEditFileFullPath(String const& _value) { m_currentEditFileFullPath = _value; }


        //////////////////////////////////////////
        void setWindowTitle(CString _title, ...);

        //////////////////////////////////////////
        void setWindowTitle(String const& _title);


        //////////////////////////////////////////
        void setPlaytestModeEnabled(bool _value);

        //////////////////////////////////////////
        inline bool getPlaytestModeEnabled() const { return m_playtestModeEnabled; }


        //////////////////////////////////////////
        SceneMainPtr getSceneMain() const;

        //////////////////////////////////////////
        SceneMainToolsPtr getSceneMainTools() const;

        //////////////////////////////////////////
        EcsWorld* getMainEcsWorld() const;

    public:

        //////////////////////////////////////////
        MultiDelegate<EditorSceneMode> eventSceneModeChanged;
        MultiDelegate<bool> eventPlaytestModeEnabledChanged;


    protected:

        //////////////////////////////////////////
        EditorManager();

        //////////////////////////////////////////
        bool init();



    protected:
        static EditorManager* s_instance;

        EditorSceneMode m_sceneMode = EditorSceneMode::None;

        EditorAssetsManagerPtr m_editorAssetsManager;
        EditorPrefabManagerPtr m_editorPrefabManager;
        EditorAssetsModeManagerPtr m_editorAssetsModeManager;
        EditorProjectModeManagerPtr m_editorProjectModeManager;
        EditorGizmosManagerPtr m_editorGizmosManager;
        EditorEntityManagerPtr m_editorEntityManager;
        EditorWorkspaceManagerPtr m_editorWorkspaceManager;
        EditorPlaytestManagerPtr m_editorPlaytestManager;
        EditorUpdateManagerPtr m_editorUpdateManager;

        String m_currentEditFileFullPath;

        bool m_playtestModeEnabled = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorManager_hpp_
//////////////////////////////////////////
