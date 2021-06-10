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
#if (!defined(_MazeDebuggerManager_hpp_))
#define _MazeDebuggerManager_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(DebuggerManager);
    MAZE_USING_SHARED_PTR(SelectionManager);
    MAZE_USING_SHARED_PTR(InspectorManager);
    MAZE_USING_SHARED_PTR(AssetDebuggerManager);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(GizmosDrawer);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(GizmosSystem);
    MAZE_USING_SHARED_PTR(ComponentSystem);


    //////////////////////////////////////////
    // Class DebuggerManager
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API DebuggerManager
        : public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~DebuggerManager();

        //////////////////////////////////////////
        static void Initialize(DebuggerManagerPtr& _debuggerManager);


        //////////////////////////////////////////
        static inline DebuggerManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline DebuggerManager& GetInstance() { return *s_instance; }

        //////////////////////////////////////////
        inline SelectionManagerPtr const& getSelectionManager() const { return m_selectionManager; }

        //////////////////////////////////////////
        inline InspectorManagerPtr const& getInspectorManager() const { return m_inspectorManager; }

        //////////////////////////////////////////
        inline AssetDebuggerManagerPtr const& getAssetDebuggerManager() const { return m_assetDebuggerManager; }


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

    protected:

        //////////////////////////////////////////
        DebuggerManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyComponentSystemAdded(ComponentSystemPtr const& _componentSystem);

        //////////////////////////////////////////
        void setGizmosSystem(GizmosSystemPtr const& _gizmosSystem);

    protected:
        static DebuggerManager* s_instance;

        SelectionManagerPtr m_selectionManager;
        InspectorManagerPtr m_inspectorManager;
        AssetDebuggerManagerPtr m_assetDebuggerManager;

        GizmosSystemPtr m_gizmosSystem;

        bool m_debugEditorActive;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDebuggerManager_hpp_
//////////////////////////////////////////
