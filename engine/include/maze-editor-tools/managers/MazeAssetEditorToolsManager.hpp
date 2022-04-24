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
#if (!defined(_MazeAssetEditorToolsManager_hpp_))
#define _MazeAssetEditorToolsManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetEditorToolsManager);
    MAZE_USING_SHARED_PTR(SelectionManager);
    MAZE_USING_SHARED_PTR(InspectorManager);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(GizmosDrawer);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(GizmosSystem);
    MAZE_USING_SHARED_PTR(ComponentSystem);
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(RenderSystem);


    //////////////////////////////////////////
    // Class AssetEditorToolsManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API AssetEditorToolsManager
        : public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:
        //////////////////////////////////////////
        using AssetFileContextMenuCallback = std::function<void(String const&, MenuListTree2DPtr const&)>;

    public:

        //////////////////////////////////////////
        virtual ~AssetEditorToolsManager();

        //////////////////////////////////////////
        static void Initialize(AssetEditorToolsManagerPtr& _assetEditorToolsManager);


        //////////////////////////////////////////
        static inline AssetEditorToolsManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline AssetEditorToolsManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void registerIconCallbackForAssetFileExtension(
            String const& _assetFileExtension,
            std::function<SpritePtr(AssetFilePtr const&)> _callback);

        //////////////////////////////////////////
        void registerIconCallbackForAssetFileClass(
            ClassUID _assetFileClass,
            std::function<SpritePtr()> _callback);

        //////////////////////////////////////////
        template <typename TAssetFile>
        inline void registerIconCallbackForAssetFileClass(
            std::function<SpritePtr()> _callback)
        {
            registerIconCallbackForAssetFileClass(ClassInfo<TAssetFile>::UID(), _callback);
        }

        //////////////////////////////////////////
        SpritePtr getIconForAssetFile(AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        inline void registerAssetFileContextMenuCallback(AssetFileContextMenuCallback _callback)
        {
            m_assetFileContextMenuCallbacks.push_back(_callback);
        }

        //////////////////////////////////////////
        void callAssetFileContextMenuCallback(String const& _fullPath, MenuListTree2DPtr const& _tree);

    protected:

        //////////////////////////////////////////
        AssetEditorToolsManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void registerIconCallbacks();

        //////////////////////////////////////////
        void registerAssetFileCallbacks();

        //////////////////////////////////////////
        void notifyTextureLoaderAdded(HashedCString _extension, TextureLoaderData const& _data);

        //////////////////////////////////////////
        void notifyDefaultRenderSystemWillBeChanged(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void notifyDefaultRenderSystemChanged(RenderSystemPtr const& _renderSystem);

    protected:
        static AssetEditorToolsManager* s_instance;

        StringKeyMap<std::function<SpritePtr(AssetFilePtr const&)>> m_iconCallbackPerAssetFileExtension;
        Map<ClassUID, std::function<SpritePtr()>> m_iconCallbackPerAssetFileClass;

        Vector<AssetFileContextMenuCallback> m_assetFileContextMenuCallbacks;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetEditorToolsManager_hpp_
//////////////////////////////////////////
