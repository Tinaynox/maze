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
#if (!defined(_EditorSceneManager_hpp_))
#define _EditorSceneManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
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
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorSceneManager);
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(SceneWorkspace);
    MAZE_USING_SHARED_PTR(EcsAssetScene);


    //////////////////////////////////////////
    // Class EditorSceneManager
    //
    //////////////////////////////////////////
    class EditorSceneManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~EditorSceneManager();

        //////////////////////////////////////////
        static void Initialize(EditorSceneManagerPtr& _manager);
        

        //////////////////////////////////////////
        static inline EditorSceneManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorSceneManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        inline EcsAssetScenePtr const& getScene() const { return m_scene; }

        //////////////////////////////////////////
        void setScene(EcsAssetScenePtr const& _value);


        //////////////////////////////////////////
        inline AssetFilePtr const& getSceneAssetFile() const { return m_sceneAssetFile; }

        //////////////////////////////////////////
        void setSceneAssetFile(AssetFilePtr const& _value);


        //////////////////////////////////////////
        void saveAssetFile();

        //////////////////////////////////////////
        void updateSceneAssetFile();


        //////////////////////////////////////////
        inline bool getSceneAssetFileSaveEnabled() const { return m_sceneAssetFileSaveEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getSceneAssetFileSaveEnabledChangedEvent() { return m_sceneAssetFileSaveEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline MultiDelegate<U32 const&>& getSceneAssetFileSaveTimestampChangedEvent() { return m_sceneAssetFileSaveTimestamp.eventValueChanged; }

    public:

        //////////////////////////////////////////
        MultiDelegate<EcsAssetScenePtr const&> eventSceneChanged;
        MultiDelegate<AssetFilePtr const&> eventSceneAssetFileChanged;

    protected:

        //////////////////////////////////////////
        EditorSceneManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void updateSceneAssetFileSaveEnabled();

        //////////////////////////////////////////
        void notifyLastChangeTimestampChanged(U32 const& _value);

        //////////////////////////////////////////
        void notifySceneAssetFileSaveTimestampChanged(U32 const& _value);

    protected:
        static EditorSceneManager* s_instance;

        EcsAssetScenePtr m_scene;
        AssetFilePtr m_sceneAssetFile;

        ObservableValue<bool> m_sceneAssetFileSaveEnabled = false;
        ObservableValue<U32> m_sceneAssetFileSaveTimestamp = 0u;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorSceneManager_hpp_
//////////////////////////////////////////
