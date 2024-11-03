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
#if (!defined(_EditorAssetsManager_hpp_))
#define _EditorAssetsManager_hpp_


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
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "scenes/SceneWorkspace.hpp"
#include "editor/EditorSceneMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorAssetsManager);
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(SceneWorkspace);


    //////////////////////////////////////////
    struct EditorAssetContextOption
    {
        //////////////////////////////////////////
        EditorAssetContextOption(
            String _menuName,
            std::function<void(AssetsController*, Path const&)> const& _callback,
            MenuListTree2D::ItemValidateCallback const& _validate)
            : menuName(_menuName)
            , callback(_callback)
            , validate(_validate)
        {}

        String menuName;
        std::function<void(AssetsController*, Path const&)> callback = nullptr;
        MenuListTree2D::ItemValidateCallback validate = nullptr;
    };


    //////////////////////////////////////////
    // Class EditorAssetsManager
    //
    //////////////////////////////////////////
    class EditorAssetsManager
        : public MultiDelegateCallbackReceiver
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        ~EditorAssetsManager();

        //////////////////////////////////////////
        static void Initialize(EditorAssetsManagerPtr& _manager);
        

        //////////////////////////////////////////
        static inline EditorAssetsManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorAssetsManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void addAssetContextOption(
            HashedCString _extension,
            String const& _menuName,
            std::function<void(AssetsController*, Path const&)> const& _callback,
            MenuListTree2D::ItemValidateCallback _validate = nullptr);

    protected:

        //////////////////////////////////////////
        EditorAssetsManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void notifyWindowFocusChanged(Window* _window);


        //////////////////////////////////////////
        void registerAssetFileCallbacks();


        //////////////////////////////////////////
        void notifyCoreEditorResourcesLoaded();


        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

        //////////////////////////////////////////
        void fixAssetFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void fixAssetFileNow(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void fixAssetFilesNow();

        //////////////////////////////////////////

    protected:
        static EditorAssetsManager* s_instance;

        Set<AssetFilePtr> m_assetFilessToFix;

        StringKeyMap<Vector<EditorAssetContextOption>> m_editorAssetContextOptions;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EditorAssetsManager_hpp_
//////////////////////////////////////////
