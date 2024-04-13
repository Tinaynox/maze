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
#if (!defined(_MazeAssetsController_hpp_))
#define _MazeAssetsController_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetLine.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(AssetsController);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ScrollRect2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);


    //////////////////////////////////////////
    // Class AssetsController
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API AssetsController
        : public Component
        , public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetsController, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AssetsController);

        //////////////////////////////////////////
        friend class Entity;

        //////////////////////////////////////////
        struct AssetLineInfo
        {
            bool expanded = false;
            bool rename = false;
        };

    public:

        //////////////////////////////////////////
        virtual ~AssetsController();

        //////////////////////////////////////////
        static AssetsControllerPtr Create(Canvas* _canvas);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline Path const& getSelectedAssetFolder() const { return m_selectedAssetFolder; }

        //////////////////////////////////////////
        void setSelectedAssetFolder(Path const& _value);


        //////////////////////////////////////////
        void setAssetsFullPath(Path const& _assetsFullPath);


        //////////////////////////////////////////
        void setAssetFileExpanded(AssetFilePtr const& _assetFile, bool _expanded);

        //////////////////////////////////////////
        bool getAssetFileExpanded(AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        void setAssetFileRename(AssetFilePtr const& _assetFile, bool _rename);

        //////////////////////////////////////////
        bool getAssetFileRename(AssetFilePtr const& _assetFile);


    public:
        //////////////////////////////////////////
        MultiDelegate<String const&> eventAssetDoubleClick;

    protected:

        //////////////////////////////////////////
        AssetsController();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(Canvas* _canvas);

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void updateAssetsTree();
        
        //////////////////////////////////////////
        void clearAssetsTree();

        //////////////////////////////////////////
        void notifyAssetTreeLineExpandedChanged(AssetLine* _line, bool _value);

        //////////////////////////////////////////
        void notifyAssetTreeLineClick(AssetLine* _line);

        //////////////////////////////////////////
        void notifyAssetTreeLineDoubleClick(AssetLine* _line);

        //////////////////////////////////////////
        void clearSelectedAssetsFolder();

        //////////////////////////////////////////
        void updateSelectedAssetsFolder();

        //////////////////////////////////////////
        void updateSelectedAssetsFolderSelection();

        //////////////////////////////////////////
        void notifySelectionManagerSelectionChanged();

        //////////////////////////////////////////
        void notifyAssetFileAdded(AssetFilePtr const& _file);

        //////////////////////////////////////////
        void notifyAssetFileRemoved(AssetFilePtr const& _file);

        //////////////////////////////////////////
        void notifyAssetFileMoved(AssetFilePtr const& _file, Path const& _prevFullPath);

    protected:
        Canvas* m_canvas;

        Transform2DPtr m_transform;

        SpriteRenderer2DPtr m_bodyBackground;

        Map<AssetFilePtr, AssetLineInfo> m_assetFilesInfo;

        Path m_assetsFullPath;

        HorizontalLayout2DPtr m_layout;

        ScrollRect2DPtr m_assetsTreeRoot;
        Transform2DPtr m_assetsTreeLayoutTransform;
        UnorderedMap<Path, AssetLinePtr> m_assetsTreeLines;

        ScrollRect2DPtr m_selectedAssetsFolderRoot;
        Transform2DPtr m_selectedAssetsFolderLayoutTransform;
        UnorderedMap<Path, AssetLinePtr> m_selectedAssetsFolderLines;

        Path m_selectedAssetFolder;

        bool m_assetsTreeDirty = false;
        bool m_selectedAssetsFolder = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetsController_hpp_
//////////////////////////////////////////
