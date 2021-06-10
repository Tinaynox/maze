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
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-debugger/ecs/components/MazeAssetLine.hpp"


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
    class MAZE_DEBUGGER_API AssetsController
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
        inline String const& getSelectedAssetFolder() const { return m_selectedAssetFolder; }

        //////////////////////////////////////////
        void setSelectedAssetFolder(String const& _value);


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
        void notifyAssetTreeLinePressed(AssetLine* _line);

        //////////////////////////////////////////
        void clearSelectedAssetsFolder();

        //////////////////////////////////////////
        void updateSelectedAssetsFolder();

        //////////////////////////////////////////
        void updateSelectedAssetsFolderSelection();

        //////////////////////////////////////////
        void notifySelectionManagerSelectionChanged();

    protected:
        Canvas* m_canvas;

        Transform2DPtr m_transform;

        SpriteRenderer2DPtr m_bodyBackground;

        HorizontalLayout2DPtr m_layout;

        ScrollRect2DPtr m_assetsTreeRoot;
        Transform2DPtr m_assetsTreeLayoutTransform;
        UnorderedMap<String, AssetLinePtr> m_assetsTreeLines;

        ScrollRect2DPtr m_selectedAssetsFolderRoot;
        Transform2DPtr m_selectedAssetsFolderLayoutTransform;
        UnorderedMap<String, AssetLinePtr> m_selectedAssetsFolderLines;

        String m_selectedAssetFolder;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetsController_hpp_
//////////////////////////////////////////
