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
#if (!defined(_MazeEditorToolsManager_hpp_))
#define _MazeEditorToolsManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EditorToolsManager);
    MAZE_USING_SHARED_PTR(SelectionManager);
    MAZE_USING_SHARED_PTR(InspectorManager);
    MAZE_USING_SHARED_PTR(GizmosManager);
    MAZE_USING_SHARED_PTR(AssetEditorToolsManager);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(GizmosDrawer);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(GizmosSystem);
    MAZE_USING_SHARED_PTR(ComponentSystem);
    MAZE_USING_SHARED_PTR(MenuListTree2D);
    MAZE_USING_SHARED_PTR(ECSScene);


    //////////////////////////////////////////
    // Class EditorToolsManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EditorToolsManager
        : public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~EditorToolsManager();

        //////////////////////////////////////////
        static void Initialize(EditorToolsManagerPtr& _manager);


        //////////////////////////////////////////
        static inline EditorToolsManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EditorToolsManager& GetInstance() { return *s_instance; }

        //////////////////////////////////////////
        inline SelectionManagerPtr const& getSelectionManager() const { return m_selectionManager; }

        //////////////////////////////////////////
        inline InspectorManagerPtr const& getInspectorManager() const { return m_inspectorManager; }

        //////////////////////////////////////////
        inline AssetEditorToolsManagerPtr const& getAssetEditorToolsManager() const { return m_assetEditorToolsManager; }

        //////////////////////////////////////////
        inline GizmosSystemPtr const& getGizmosSystem() const { return m_gizmosSystem; }

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void openDebugEditor(RenderTargetPtr const& _renderTarget);

        //////////////////////////////////////////
        void closeDebugEditor();

        //////////////////////////////////////////
        inline bool getDebugEditorActive() const { return m_debugEditorActive; }

        //////////////////////////////////////////
        void notifyDrawGizmos(
            ComponentGizmosPtr const& _gizmos,
            Entity* _entity,
            ComponentPtr const& _component,
            GizmosDrawer* _drawer);

        //////////////////////////////////////////
        void setGizmosSystem(GizmosSystemPtr const& _gizmosSystem);

    public:

        //////////////////////////////////////////
        MultiDelegate<MenuListTree2DPtr const&, Entity*> eventHierarchyLineEntityContextMenu;
        MultiDelegate<MenuListTree2DPtr const&, ECSScene*> eventHierarchyLineSceneContextMenu;

    protected:

        //////////////////////////////////////////
        EditorToolsManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyComponentSystemAdded(ComponentSystemPtr const& _componentSystem);

        //////////////////////////////////////////
        void notifyPauseChanged(bool const& _value);

        //////////////////////////////////////////
        void updatePause();

    protected:
        static EditorToolsManager* s_instance;

        SelectionManagerPtr m_selectionManager;
        InspectorManagerPtr m_inspectorManager;
        GizmosManagerPtr m_gizmosManager;
        AssetEditorToolsManagerPtr m_assetEditorToolsManager;

        GizmosSystemPtr m_gizmosSystem;

        bool m_debugEditorActive;

        UniquePtr<ScopedPause> m_pause;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEditorToolsManager_hpp_
//////////////////////////////////////////
