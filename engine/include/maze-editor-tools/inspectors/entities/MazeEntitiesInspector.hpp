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
#if (!defined(_MazeEntitiesInspector_hpp_))
#define _MazeEntitiesInspector_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/inspectors/MazeInspector.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesInspector);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(MetaPropertyDrawer);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ComponentEditor);
    MAZE_USING_SHARED_PTR(ToggleButton2D);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);
    MAZE_USING_SHARED_PTR(EcsWorld);
    

    //////////////////////////////////////////
    // Class EntitiesInspector
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API EntitiesInspector
        : public Inspector
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EntitiesInspector, Inspector);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(EntitiesInspector);

    public:

        //////////////////////////////////////////
        virtual ~EntitiesInspector();

        //////////////////////////////////////////
        static EntitiesInspectorPtr Create(Transform2DPtr const& _parent);


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        void setEntities(Set<EntityPtr> const& _entities);

    protected:

        //////////////////////////////////////////
        EntitiesInspector();

        //////////////////////////////////////////
        virtual bool init(Transform2DPtr const& _parent) MAZE_OVERRIDE;

        

        //////////////////////////////////////////
        void updateEntitesValues();

        //////////////////////////////////////////
        void updatePropertyValues();

        //////////////////////////////////////////
        void buildEntitiesPropertiesList();

        //////////////////////////////////////////
        void notifyEntitiesEnabledToggleCheckedChanged(ToggleButton2D* _button, bool _checked);

        //////////////////////////////////////////
        void notifyAddComponentButton(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyEntityChanged(EntityPtr const& _entity);


        //////////////////////////////////////////
        void setEcsWorld(EcsWorld* _world);

    protected:
        Map<ClassUID, ComponentEditorPtr> m_componentEditors;

        Set<EntityPtr> m_entities;

        Transform2DPtr m_topBlock;
        ToggleButton2DPtr m_entitiesEnabledToggleButton;
        AbstractTextRenderer2DPtr m_entityIdText;
        ClickButton2DPtr m_addComponentButton;

        bool m_entitiesPropertiesListDirty;

        EcsWorld* m_world = nullptr;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntitiesInspector_hpp_
//////////////////////////////////////////
