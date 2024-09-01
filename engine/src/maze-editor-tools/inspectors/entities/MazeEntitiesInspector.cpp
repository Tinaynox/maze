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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/inspectors/entities/MazeEntitiesInspector.hpp"
#include "maze-editor-tools/inspectors/entities/MazeComponentEditor.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionActionsGroup.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionCustom.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class EntitiesInspector
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EntitiesInspector, Inspector);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_DEFAULT(EntitiesInspector);

    //////////////////////////////////////////
    EntitiesInspector::EntitiesInspector()
        : m_entitiesPropertiesListDirty(false)
    {
        
    }

    //////////////////////////////////////////
    EntitiesInspector::~EntitiesInspector()
    {
        if (m_entitiesEnabledToggleButton)
            m_entitiesEnabledToggleButton->eventCheckedChanged.unsubscribe(this);

        if (m_addComponentButton)
            m_addComponentButton->eventClick.unsubscribe(this);

        setEcsWorld(nullptr);
    }

    //////////////////////////////////////////
    EntitiesInspectorPtr EntitiesInspector::Create(Transform2DPtr const& _parent)
    {
        EntitiesInspectorPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EntitiesInspector, object, init(_parent));
        return object;
    }

    //////////////////////////////////////////
    bool EntitiesInspector::init(Transform2DPtr const& _parent)
    {
        if (!Inspector::init(_parent))
            return false;

        m_topBlock = SpriteHelper::CreateTransform2D(
            Vec2F(_parent->getWidth(), 50.0f),
            Vec2F::c_zero,
            _parent,
            _parent->getEntityRaw()->getEcsScene());
        m_topBlock->getEntityRaw()->ensureComponent<SizePolicy2D>()->setFlag(SizePolicy2D::Flags::Height, false);

        m_entitiesEnabledToggleButton = UIHelper::CreateDefaultToggleButton(
            Vec2F(5, 0),
            m_topBlock,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_entitiesEnabledToggleButton->eventCheckedChanged.subscribe(this, &EntitiesInspector::notifyEntitiesEnabledToggleCheckedChanged);

        m_entityIdText = EditorToolsUIHelper::CreateText(
            "Entity Id: 500",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(16.0f, 10.0f),
            Vec2F(35.0f, 0.0f),
            m_topBlock,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_entityIdText->setColor(ColorU32::c_black);

        m_addComponentButton = UIHelper::CreateDefaultClickButton(
            "Add Component",
            EditorToolsStyles::GetInstancePtr()->getDefaultBoldFontMaterial(),
            Vec2F(120.0f, 18.0f),
            Vec2F(-20.0f, 0.0f),
            m_topBlock,
            _parent->getEntityRaw()->getEcsScene(),
            Vec2F(1.0f, 0.5f),
            Vec2F(1.0f, 0.5f));
        m_addComponentButton->eventClick.subscribe(this, &EntitiesInspector::notifyAddComponentButton);

        return true;
    }

    //////////////////////////////////////////
    void EntitiesInspector::update(F32 _dt)
    {
        MAZE_PROFILER_SCOPED_LOCK(INSPECTOR);
        MAZE_PROFILE_EVENT("EntitiesInspector::update");

        if (m_entitiesPropertiesListDirty)
            buildEntitiesPropertiesList();

        updateEntitesValues();
        updatePropertyValues();
    }

    //////////////////////////////////////////
    void EntitiesInspector::setEntities(Set<EntityPtr> const& _entities)
    {
        m_entities = _entities;

        if (_entities.size())
            setEcsWorld((*_entities.begin())->getEcsWorld());
        else
            setEcsWorld(nullptr);

        m_entitiesPropertiesListDirty = true;
    }

    //////////////////////////////////////////
    void EntitiesInspector::updateEntitesValues()
    {
        if (m_entities.empty())
            return;

        bool checkedValue = (*m_entities.begin())->getActiveSelf();
        bool mixedValue = false;
        EntityId entityId = (*m_entities.begin())->getId();

        for (Set<EntityPtr>::iterator it = ++m_entities.begin(); it != m_entities.end(); ++it)
        {
            if (checkedValue != (*it)->getActiveSelf())
            {
                mixedValue = true;
                break;
            }
        }

        m_entitiesEnabledToggleButton->setChecked(checkedValue);

        m_entityIdText->setText("Entity Id: " + StringHelper::ToString(entityId));
    }

    //////////////////////////////////////////
    void EntitiesInspector::updatePropertyValues()
    {
        for (Map<ClassUID, ComponentEditorPtr>::iterator    it = m_componentEditors.begin(),
                                                            end = m_componentEditors.end();
                                                            it != end;
                                                            ++it)
        {
            ClassUID componentUID = it->first;

            Set<ComponentPtr> components;
            for (EntityPtr const& entity : m_entities)
            {
                ComponentPtr const& component = entity->getComponentByUID(componentUID);
                if (component)
                    components.insert(component);
            }

            it->second->updatePropertyValues(components);
        }
    }

    //////////////////////////////////////////
    void EntitiesInspector::buildEntitiesPropertiesList()
    {
        InspectorManager* inspectorManager = InspectorManager::GetInstancePtr();

        Set<MetaClass*> componentMetaClasses = EditorToolsHelper::CollectIntersectingComponentMetaClasses(m_entities);

        // Remove unnecessary editors
        for (Map<ClassUID, ComponentEditorPtr>::iterator    it = m_componentEditors.begin(),
                                                            end = m_componentEditors.end();
                                                            it != end;
                                                            )
        {
            if (std::find_if(
                componentMetaClasses.begin(),
                componentMetaClasses.end(),
                [&](MetaClass* _metaClass) -> bool
                {
                    return _metaClass->getClassUID() == it->first;
                }) == componentMetaClasses.end())
            {
                ComponentEditorPtr editor = it->second;

                editor->removeEditor();

                it = m_componentEditors.erase(it);
                end = m_componentEditors.end();
            }
            else
            {
                ++it;
            }
        }

        // Add necessary editors
        for (MetaClass* componentMetaClass : componentMetaClasses)
        {
            if (m_componentEditors.find(componentMetaClass->getClassUID()) == m_componentEditors.end())
            {
                ComponentEditorPtr editor = inspectorManager->createComponentEditor(componentMetaClass);
                editor->buildEditor(m_parent);
                editor->eventRemoveComponentPressed.subscribe(
                    [this](ClassUID _uid)
                    {
                        if (EditorToolsActionManager::GetInstancePtr())
                        {
                            EditorActionActionsGroupPtr group = EditorActionActionsGroup::Create();

                            for (EntityPtr const& entity : m_entities)
                            {
                                ComponentPtr component = entity->getComponentByUID(_uid);

                                group->addAction(
                                    EditorActionCustom::Create(
                                        [entity, component]() { entity->removeComponent(component); },
                                        [entity, component]() { entity->addComponent(component); }));
                            }

                            if (group->getActionsCount() > 0)
                                EditorToolsActionManager::GetInstancePtr()->applyAction(group);
                        }
                        else
                            for (EntityPtr const& entity : m_entities)
                                entity->removeComponent(_uid);
                    });

                m_componentEditors[componentMetaClass->getClassUID()] = editor;
            }
        }

        m_entitiesPropertiesListDirty = false;
    }  

    //////////////////////////////////////////
    void EntitiesInspector::notifyEntitiesEnabledToggleCheckedChanged(ToggleButton2D* _button, bool _checked)
    {
        bool value = m_entitiesEnabledToggleButton->getChecked();


        if (EditorToolsActionManager::GetInstancePtr())
        {
            EditorActionActionsGroupPtr group = EditorActionActionsGroup::Create();

            for (Set<EntityPtr>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
            {
                EntityPtr entity = (*it);
                if (entity->getActiveSelf() == value)
                    continue;
                
                group->addAction(
                    EditorActionCustom::Create(
                        [entity, value]() { entity->setActiveSelf(value); },
                        [entity, oldValue = entity->getActiveSelf()]() { entity->setActiveSelf(oldValue); }));
            }

            if (group->getActionsCount() > 0)
                EditorToolsActionManager::GetInstancePtr()->applyAction(group);
        }
        else
        {
            for (Set<EntityPtr>::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
                (*it)->setActiveSelf(value);
        }
    }

    //////////////////////////////////////////
    void EntitiesInspector::notifyAddComponentButton(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        ContextMenuCanvas2DPtr contextMenuCanvas = ContextMenuCanvas2D::EnsureContextMenuCanvas(_button->getEntityRaw()->getEcsScene());

        Canvas* canvas = _button->getTransform()->getFirstTrunkComponent<Canvas>();
        Vec2F positionRTS = canvas->convertViewportCoordsToRenderTargetCoords(_inputEvent.position);

        MenuListTree2DPtr const& menuListTree = contextMenuCanvas->openContextMenu(
            this,
            positionRTS,
            [this](MenuListTree2DPtr const& _menuListTree)
            {
                ComponentFactoryPtr const& componentFactory = EntityManager::GetInstancePtr()->getComponentFactory();
                for (auto const& sceneObjectCreationData : componentFactory->getSceneObjectCreationData())
                {
                    String itemName;
                    if (sceneObjectCreationData.second.group)
                        itemName += String(sceneObjectCreationData.second.group) + "/";
                    itemName += sceneObjectCreationData.second.name;

                    _menuListTree->addItem(
                        itemName,
                        [this, sceneObjectCreationData](String const& _text)
                        {
                            ClassUID uid = sceneObjectCreationData.second.uid;

                            for (EntityPtr const& entity : m_entities)
                            {
                                if (!entity->getComponentByUID(uid))
                                {
                                    ComponentPtr component = EntityManager::GetInstancePtr()->getComponentFactory()->createComponentByIndex(uid);
                                    if (component)
                                    {
                                        if (EditorToolsActionManager::GetInstancePtr())
                                        {
                                            EditorToolsActionManager::GetInstancePtr()->applyAction(
                                                EditorActionCustom::Create(
                                                    [entity, component]() { entity->addComponent(component); },
                                                    [entity, component]() { entity->removeComponent(component); }));
                                        }
                                        else
                                            entity->addComponent(component);
                                    }
                                }
                            }
                        },
                        nullptr,
                        false);
                }
            });

        menuListTree->rebuildItems();
    }

    //////////////////////////////////////////
    void EntitiesInspector::notifyEntityChanged(EntityPtr const& _entity)
    {
        if (!m_entitiesPropertiesListDirty)
        {
            if (m_entities.find(_entity->getSharedPtr()) != m_entities.end())
                m_entitiesPropertiesListDirty = true;
        }
    }

    //////////////////////////////////////////
    void EntitiesInspector::setEcsWorld(EcsWorld* _world)
    {
        if (m_world == _world)
            return;

        if (m_world)
        {
            m_world->eventEntityChanged.unsubscribe(this);
        }

        m_world = _world;

        if (m_world)
        {
            m_world->eventEntityChanged.subscribe(this, &EntitiesInspector::notifyEntityChanged);
        }
    }

} // namespace Maze
//////////////////////////////////////////
