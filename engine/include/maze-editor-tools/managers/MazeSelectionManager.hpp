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
#if (!defined(_MazeSelectionManager_hpp_))
#define _MazeSelectionManager_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/MazeObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SelectionManager);
    MAZE_USING_SHARED_PTR(RenderTarget);
    MAZE_USING_SHARED_PTR(ComponentGizmos);
    MAZE_USING_SHARED_PTR(GizmosDrawer);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Enum
    //
    //////////////////////////////////////////
    enum MAZE_EDITOR_TOOLS_API SelectionType
    {
        None,
        Entities,
        Objects,
    };


    //////////////////////////////////////////
    // Class SelectionManager
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API SelectionManager
        : public Updatable
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~SelectionManager();

        //////////////////////////////////////////
        static void Initialize(SelectionManagerPtr& _manager);


        //////////////////////////////////////////
        static inline SelectionManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SelectionManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void unselectObjects();

        //////////////////////////////////////////
        inline Set<EntityPtr> const& getSelectedEntities() const { return m_selectedEntities; }

        //////////////////////////////////////////
        inline Set<EntityId> getSelectedEntityIds();

        //////////////////////////////////////////
        inline Set<ObjectPtr> const& getSelectedObjects() const { return m_selectedObjects; }

        //////////////////////////////////////////
        Set<AssetFilePtr> getSelectedAssetFiles() const;

        //////////////////////////////////////////
        bool isObjectSelected(EntityPtr const& _object) const;

        //////////////////////////////////////////
        void selectObject(
            EntityPtr const& _object,
            bool _clearSelectionList = true,
            bool _throwEvent = true);

        //////////////////////////////////////////
        void selectObjects(
            Vector<EntityPtr> const& _objects,
            bool _clearSelectionList = true);

        //////////////////////////////////////////
        void unselectObject(
            EntityPtr const& _object,
            bool _throwEvent = true);

        //////////////////////////////////////////
        void unselectObjects(
            Vector<EntityPtr> const& _objects);

        //////////////////////////////////////////
        bool isSelectedObjectsEqual(Vector<EntityPtr> const& _objects);


        //////////////////////////////////////////
        bool isObjectSelected(ObjectPtr const& _object) const;

        //////////////////////////////////////////
        void selectObject(
            ObjectPtr const& _object,
            bool _clearSelectionList = true,
            bool _throwEvent = true);

        //////////////////////////////////////////
        void selectObjects(
            Vector<ObjectPtr> const& _objects,
            bool _clearSelectionList = true);

        //////////////////////////////////////////
        void unselectObject(
            ObjectPtr const& _object,
            bool _throwEvent = true);

        //////////////////////////////////////////
        void unselectObjects(
            Vector<ObjectPtr> const& _objects);

        //////////////////////////////////////////
        inline SelectionType getSelectionType() const { return m_selectionType; }

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventSelectionChanged;

    protected:

        //////////////////////////////////////////
        SelectionManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyEntityRemoved(EntityPtr const& _entity);

        //////////////////////////////////////////
        void setSelectionType(SelectionType _value);


        //////////////////////////////////////////
        void notifyAssetFileRemoved(AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        void setEcsWorld(EcsWorld* _world);

    protected:
        static SelectionManager* s_instance;

        EcsWorld* m_world = nullptr;

        SelectionType m_selectionType;

        Set<EntityPtr> m_selectedEntities;
        Set<ObjectPtr> m_selectedObjects;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSelectionManager_hpp_
//////////////////////////////////////////
