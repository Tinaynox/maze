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
#if (!defined(_EditorPrefabManager_hpp_))
#define _EditorPrefabManager_hpp_


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
    MAZE_USING_SHARED_PTR(EditorPrefabManager);
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(SceneWorkspace);


    //////////////////////////////////////////
    // Class EditorPrefabManager
    //
    //////////////////////////////////////////
    class EditorPrefabManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~EditorPrefabManager();

        //////////////////////////////////////////
        static void Initialize(EditorPrefabManagerPtr& _manager);
        

        //////////////////////////////////////////
        static inline EditorPrefabManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorPrefabManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        inline EntityPtr const& getPrefabEntity() const { return m_prefabEntity; }

        //////////////////////////////////////////
        void setPrefabEntity(EntityPtr const& _value);


        //////////////////////////////////////////
        inline AssetFilePtr const& getPrefabAssetFile() const { return m_prefabAssetFile; }

        //////////////////////////////////////////
        void setPrefabAssetFile(AssetFilePtr const& _value);


        //////////////////////////////////////////
        void saveAssetFile();

        //////////////////////////////////////////
        void updatePrefabAssetFile();


        //////////////////////////////////////////
        inline bool getPrefabAssetFileSaveEnabled() const { return m_prefabAssetFileSaveEnabled.getValue(); }

        //////////////////////////////////////////
        inline MultiDelegate<bool const&>& getPrefabAssetFileSaveEnabledChangedEvent() { return m_prefabAssetFileSaveEnabled.eventValueChanged; }

        //////////////////////////////////////////
        inline MultiDelegate<U32 const&>& getPrefabAssetFileSaveTimestampChangedEvent() { return m_prefabAssetFileSaveTimestamp.eventValueChanged; }

    public:

        //////////////////////////////////////////
        MultiDelegate<EntityPtr const&> eventPrefabEntityChanged;
        MultiDelegate<AssetFilePtr const&> eventPrefabAssetFileChanged;

    protected:

        //////////////////////////////////////////
        EditorPrefabManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void updatePrefabAssetFileSaveEnabled();

        //////////////////////////////////////////
        void notifyLastChangeTimestampChanged(U32 const& _value);

        //////////////////////////////////////////
        void notifyPrefabAssetFileSaveTimestampChanged(U32 const& _value);

    protected:
        static EditorPrefabManager* s_instance;

        EntityPtr m_prefabEntity;
        AssetFilePtr m_prefabAssetFile;

        ObservableValue<bool> m_prefabAssetFileSaveEnabled = false;
        ObservableValue<U32> m_prefabAssetFileSaveTimestamp = 0u;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorPrefabManager_hpp_
//////////////////////////////////////////
