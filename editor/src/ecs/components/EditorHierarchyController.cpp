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
#include "ecs/components/EditorHierarchyController.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLinePool.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorPrefabManager.hpp"
#include "managers/EditorEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorHierarchyController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditorHierarchyController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditorHierarchyController);

    //////////////////////////////////////////
    EditorHierarchyController::EditorHierarchyController()
    {
    }

    //////////////////////////////////////////
    EditorHierarchyController::~EditorHierarchyController()
    {
        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->unsubscribeEvent<EcsSceneStateChangedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<EntityNameChangedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<EcsEntityActiveChangedEvent>(this);
        }

        if (SelectionManager::GetInstancePtr())
            SelectionManager::GetInstancePtr()->eventSelectionChanged.unsubscribe(this);

        setEcsWorld(nullptr);

        if (EditorManager::GetInstancePtr())
        {
            EditorManager::GetInstancePtr()->eventSceneModeChanged.unsubscribe(this);
            EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.unsubscribe(this);
        }

        if (EditorPrefabManager::GetInstancePtr())
            EditorPrefabManager::GetInstancePtr()->eventPrefabEntityChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    EditorHierarchyControllerPtr EditorHierarchyController::Create(Canvas* _canvas)
    {
        EditorHierarchyControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorHierarchyController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool EditorHierarchyController::init(Canvas* _canvas)
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        m_canvas = _canvas;

        EditorManager::GetInstancePtr()->eventSceneModeChanged.subscribe(this, &EditorHierarchyController::notifyEditorSceneModeChanged);
        EditorManager::GetInstancePtr()->eventPlaytestModeEnabledChanged.subscribe(this, &EditorHierarchyController::notifyPlaytestModeEnabled);
        EditorPrefabManager::GetInstancePtr()->eventPrefabEntityChanged.subscribe(this, &EditorHierarchyController::notifyPrefabEntityChanged);

        return true;
    }

    //////////////////////////////////////////
    void EditorHierarchyController::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void EditorHierarchyController::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        m_hierarchyLinePool = getEntityRaw()->ensureComponent<HierarchyLinePool>();


        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        m_titleTransform = SpriteHelper::CreateTransform2D(
            m_canvas->getTransform()->getSize(),
            Vec2F(0.0f, 0.0f),
            m_canvas->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F::c_zero,
            Vec2F::c_zero);
        m_titleTransform->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();

        m_titleBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2F(
                m_canvas->getTransform()->getSize().x,
                EditorToolsStyles::GetInstancePtr()->getTitleHeight()),
            Vec2F(0.0f, 0.0f),
            materialManager->getSpriteMaterial(),
            m_titleTransform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f));
        m_titleBackground->setColor(EditorToolsStyles::GetInstancePtr()->getTitleBackgroundColor());
        m_titleBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        AbstractTextRenderer2DPtr titleText = EditorToolsUIHelper::CreateText(
            "Hierarchy",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            EditorToolsStyles::GetInstancePtr()->getTitleFontSize(),
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(100, EditorToolsStyles::GetInstancePtr()->getTitleHeight()),
            Vec2F(EditorToolsStyles::GetInstancePtr()->getTitleLabelShift(), 0),
            m_titleBackground->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        titleText->setColor(ColorU32::c_black);
        
        m_bodyBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2F(
                m_titleTransform->getSize().x, 
                m_titleTransform->getSize().y - EditorToolsStyles::GetInstancePtr()->getTitleHeight()),
            Vec2F(0.0f, 0.0f),
            materialManager->getSpriteMaterial(),
            m_canvas->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F::c_zero,
            Vec2F::c_zero);
        m_bodyBackground->setColor(EditorToolsStyles::GetInstancePtr()->getBodyBackgroundColor());
        m_bodyBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setSizeDelta(0.0f, -EditorToolsStyles::GetInstancePtr()->getTitleHeight());

        ScrollRect2DPtr scrollRect = UIHelper::CreateDefaultScrollRect(
            m_bodyBackground->getTransform()->getSize(),
            Vec2F::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 1.0f),
            Vec2F(0.0f, 1.0f),
            false,
            true);
        scrollRect->getViewportTransform()->getEntityRaw()->getComponent<ScissorMask2D>()->setPadding(0, 0, 0, 0);
        scrollRect->getEntityRaw()->ensureComponent<SizePolicy2D>();
        scrollRect->getEntityRaw()->getComponent<MeshRendererInstanced>()->setEnabled(false);

        m_hierarchyMainLayoutEntity = scrollRect->getContentTransform();

        SizePolicy2DPtr hierarchyMainLayoutSizePolicy = m_hierarchyMainLayoutEntity->getEntityRaw()->ensureComponent<SizePolicy2D>();
        hierarchyMainLayoutSizePolicy->setFlag(SizePolicy2D::Height, false);

        VerticalLayout2DPtr hierarchyMainLayout = m_hierarchyMainLayoutEntity->getEntityRaw()->ensureComponent<VerticalLayout2D>();
        hierarchyMainLayout->setPaddingTop(2.0f);
        hierarchyMainLayout->setVerticalAlignment(VerticalAlignment2D::Top);
        hierarchyMainLayout->setHorizontalAlignment(HorizontalAlignment2D::Left);
        hierarchyMainLayout->setAutoHeight(true);



        setEcsWorld(EditorManager::GetInstancePtr()->getMainEcsWorld());
        EventManager::GetInstancePtr()->subscribeEvent<EcsSceneStateChangedEvent>(
            this,
            &EditorHierarchyController::notifyEventManagerEvent);

        EventManager::GetInstancePtr()->subscribeEvent<EntityNameChangedEvent>(
            this,
            &EditorHierarchyController::notifyEventManagerEvent);
        EventManager::GetInstancePtr()->subscribeEvent<EcsEntityActiveChangedEvent>(
            this,
            &EditorHierarchyController::notifyEventManagerEvent);

        SelectionManager::GetInstancePtr()->eventSelectionChanged.subscribe(
            this,
            &EditorHierarchyController::notifySelectionChanged);
    }
    
    //////////////////////////////////////////
    void EditorHierarchyController::setEcsWorld(EcsWorld* _world)
    {
        if (m_world == _world)
            return;

        if (m_world)
        {
            m_world->eventEntityAdded.unsubscribe(this);
            m_world->eventEntityRemoved.unsubscribe(this);
            m_world->eventEntityChanged.unsubscribe(this);

            while (!m_entityLines.empty())
            {
                auto it = m_entityLines.begin();
                HierarchyLinePtr const& hierarchyLine = it->second;
                if (!hierarchyLine)
                {
                    m_entityLines.erase(it);
                    continue;
                }

                if (hierarchyLine->getEntityRaw())
                    hierarchyLine->release();
                else
                    m_entityLines.erase(it);
            }

            while (!m_sceneLines.empty())
            {
                auto it = m_sceneLines.begin();
                HierarchyLinePtr const& hierarchyLine = it->second;
                if (!hierarchyLine)
                {
                    m_sceneLines.erase(it);
                    continue;
                }

                if (hierarchyLine->getEntityRaw())
                    hierarchyLine->release();
                else
                    m_entityLines.erase(it);
            }
        }

        m_world = _world;

        if (m_world)
        {
            m_world->eventEntityAdded.subscribe(this, &EditorHierarchyController::notifyEntityAdded);
            m_world->eventEntityRemoved.subscribe(this, &EditorHierarchyController::notifyEntityRemoved);
            m_world->eventEntityChanged.subscribe(this, &EditorHierarchyController::notifyEntityChanged);

            for (SceneManager::SceneData const& sceneData : SceneManager::GetInstancePtr()->getScenes())
                if (sceneData.scene && sceneData.scene->getState() == EcsSceneState::Active)
                    addEcsScene(sceneData.scene);
        }
    }

    //////////////////////////////////////////
    void EditorHierarchyController::processEcsSceneStateChanged(
        EcsSceneId _sceneId,
        EcsSceneState _state)
    {
        switch (_state)
        {
            case EcsSceneState::Created:
            {
                EcsScenePtr const& scene = SceneManager::GetInstancePtr()->getScene(_sceneId);
                addEcsScene(scene);
                break;
            }
            case EcsSceneState::Destroy:
            {
                removeEcsScene(_sceneId);
                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    HierarchyLinePtr EditorHierarchyController::addEcsScene(EcsScenePtr const& _scene)
    {
        if (!getEntityRaw() || !getEntityRaw()->getEcsScene())
            return nullptr;

        if (!_scene)
            return nullptr;

        if (_scene->getIsSystemScene())
            return nullptr;

        if (_scene->getWorld() != m_world)
            return nullptr;

        ClassUID sceneUID = _scene->getClassUID();
        if (sceneUID == getEntityRaw()->getEcsScene()->getClassUID())
            return nullptr;

        if (m_ignoreScenes.find(sceneUID) != m_ignoreScenes.end())
            return nullptr;

        auto it = m_sceneLines.find(_scene->getId());
        if (it != m_sceneLines.end())
            return it->second;

        HierarchyLinePtr hierarchyLine = createHierarchyLine(HierarchyLineType::Scene);
        hierarchyLine->setText(_scene->getMetaClass()->getName().str);
        hierarchyLine->getTransform()->setParent(m_hierarchyMainLayoutEntity);
        hierarchyLine->setUserData(reinterpret_cast<void*>((Size)(S32)_scene->getId()));

        m_sceneLines[_scene->getId()] = hierarchyLine;

        for (Entity* entity : _scene->getEntities())
        {
            if (entity->getAdding() || entity->getRemoving())
                continue;

            addEntity(entity->cast<Entity>());
        }

        return hierarchyLine;
    }

    //////////////////////////////////////////
    void EditorHierarchyController::removeEcsScene(EcsSceneId _sceneId)
    {
        auto sceneHierarchyLineIt = m_sceneLines.find(_sceneId);
        if (sceneHierarchyLineIt == m_sceneLines.end())
            return;

        sceneHierarchyLineIt->second->release();
    }

    //////////////////////////////////////////
    static EntityId GetEntityParentId(EntityPtr const& _entity)
    {
        EntityId parentId = c_invalidEntityId;

        Transform2D* transform2D = _entity->getComponentRaw<Transform2D>();
        if (transform2D && transform2D->getParent())
            parentId = transform2D->getParent()->getEntityId();
        else
        {
            Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
            if (transform3D && transform3D->getParent())
                parentId = transform3D->getParent()->getEntityId();
        }
        return parentId;
    }

    //////////////////////////////////////////
    HierarchyLinePtr EditorHierarchyController::addEntity(EntityPtr const& _entity)
    {
        if (!_entity)
            return nullptr;

        if (_entity->getEcsWorld() != m_world)
            return nullptr;

        auto it = m_entityLines.find(_entity->getId());
        if (it != m_entityLines.end())
            return it->second;

        if (!_entity->getEcsScene())
            return nullptr;

        EntityId parentId = GetEntityParentId(_entity);

        HierarchyLinePtr parentLine;
        if (parentId)
        {
            EntityPtr const& parentEntity = m_world->getEntity(parentId);
            MAZE_ERROR_RETURN_VALUE_IF(!parentEntity, nullptr, "Parent entity not found!");
            parentLine = addEntity(parentEntity);
        }
        else
        {
            auto sceneIt = m_sceneLines.find(_entity->getEcsScene()->getId());
            MAZE_ERROR_RETURN_VALUE_IF(sceneIt == m_sceneLines.end(), nullptr, "Undefined scene");
            parentLine = sceneIt->second;
        }

        MAZE_ERROR_RETURN_VALUE_IF(!parentLine, nullptr, "Parent line is not found!");

        HierarchyLinePtr hierarchyLine = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine->setUserData(reinterpret_cast<void*>((Size)(S32)_entity->getId()));

        parentLine->addChild(hierarchyLine);

        m_entityLines[_entity->getId()] = hierarchyLine;
        updateEntityName(_entity);
        updateEntity(hierarchyLine, _entity);

        return hierarchyLine;
    }

    //////////////////////////////////////////
    void EditorHierarchyController::removeEntity(EntityId _entityId)
    {
        auto entityHierarchyLineIt = m_entityLines.find(_entityId);
        if (entityHierarchyLineIt == m_entityLines.end())
            return;

        entityHierarchyLineIt->second->release();
    }

    //////////////////////////////////////////
    void EditorHierarchyController::updateEntity(EntityPtr const& _entity)
    {
        if (!_entity)
            return;

        auto it = m_entityLines.find(_entity->getId());
        if (it == m_entityLines.end())
            return;

        HierarchyLinePtr const& hierarchyLine = it->second;
        updateEntity(hierarchyLine, _entity);
    }

    //////////////////////////////////////////
    void EditorHierarchyController::updateEntity(
        HierarchyLinePtr const& hierarchyLine,
        EntityPtr const& _entity)
    {
        hierarchyLine->setSelected(
            SelectionManager::GetInstancePtr()->isObjectSelected(_entity));

        hierarchyLine->setActive(
            _entity->getActiveInHierarchy());
    }

    //////////////////////////////////////////
    void EditorHierarchyController::updateEntityName(EntityPtr const& _entity)
    {
        auto it = m_entityLines.find(_entity->getId());
        if (it == m_entityLines.end())
            return;

        HierarchyLinePtr const& hierarchyLine = it->second;

        Name* name = _entity->getComponentRaw<Name>();
        hierarchyLine->setText(name ? name->getName() : "Unnamed");
    }

    //////////////////////////////////////////
    HierarchyLinePtr EditorHierarchyController::createHierarchyLine(HierarchyLineType _type)
    {
        HierarchyLinePtr hierarchyLine = m_hierarchyLinePool->createHierarchyLine(_type);
        subscribeHierarchyLine(hierarchyLine.get());

        HierarchyLineWPtr hierarchyLineWeak = hierarchyLine;
        hierarchyLine->getContextMenu()->setCallbackFunction(
            [hierarchyLineWeak](MenuListTree2DPtr const& _menuListTree)
            {
                HierarchyLinePtr hierarchyLine = hierarchyLineWeak.lock();

                if (hierarchyLine->getType() == HierarchyLineType::Entity)
                {
                    if (!hierarchyLine->getEntityRaw()->getEcsWorld())
                        return;

                    EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(hierarchyLine->getUserData()));

                    EntityPtr const& entity = hierarchyLine->getEntityRaw()->getEcsWorld()->getEntity(entityId);

                    if (!entity)
                        return;

                    EntityWPtr entityWeak = entity;

                    _menuListTree->addItem(
                        "Delete",
                        [entityWeak](String const& _text)
                    {
                        EntityPtr entity = entityWeak.lock();
                        if (entity)
                        {
                            entity->removeFromEcsWorld();
                        }
                    });

                    _menuListTree->addItem(
                        "Save To DataBlock",
                        [entityWeak](String const& _text)
                    {
                        EntityPtr entity = entityWeak.lock();
                        if (entity)
                        {
                            Name* name = entity->getComponentRaw<Name>();
                            String filename = name ? name->getName() : "Unnamed";
                            EntitySerializationManager::GetInstancePtr()->savePrefabToDataBlockFile(entity, filename + ".mzdata");
                        }
                    });

                    Transform3D* transform3D = entity->getComponentRaw<Transform3D>();
                    if (transform3D)
                    {
                        _menuListTree->addItem(
                            "Duplicate",
                            [entityWeak](String const& _text)
                        {
                            EntityPtr entity = entityWeak.lock();
                            if (entity)
                            {
                                EntityPtr entityCopy = entity->createCopy();
                                entityCopy->ensureComponent<Transform3D>()->setParent(
                                    entity->ensureComponent<Transform3D>()->getParent());
                            }
                        });

                        _menuListTree->addItem(
                            "Add Child/3D/Empty",
                            [transform3D](String const& _text)
                        {
                            EntityPtr newEntity = EditorToolsHelper::CreateEntity3D("Entity");
                            Transform3DPtr newEntityTransform = newEntity->getComponent<Transform3D>();

                            newEntityTransform->setParent(transform3D->cast<Transform3D>());
                        });
                    }
                    else
                    {
                        Transform2D* transform2D = entity->getComponentRaw<Transform2D>();
                        if (transform2D)
                        {
                            _menuListTree->addItem(
                                "Add Child/2D/Empty",
                                [transform2D](String const& _text)
                            {
                                EntityPtr newEntity = EditorToolsHelper::CreateEntity2D("Entity");
                                Transform2DPtr newEntityTransform = newEntity->getComponent<Transform2D>();

                                newEntityTransform->setParent(transform2D->cast<Transform2D>());
                            });
                        }
                    }

                    EditorToolsManager::GetInstancePtr()->eventHierarchyLineEntityContextMenu(_menuListTree, entity.get());
                }
                else
                if (hierarchyLine->getType() == HierarchyLineType::Scene)
                {
                    EcsScene* ecsScene = static_cast<EcsScene*>(hierarchyLine->getUserData());

                    _menuListTree->addItem(
                        "Add Child/3D/Empty",
                        [ecsScene](String const& _text)
                    {
                        EntityPtr newEntity = ecsScene->createEntity("Entity");
                        newEntity->ensureComponent<Transform3D>();
                    });

                    _menuListTree->addItem(
                        "Add Child/2D/Empty",
                        [ecsScene](String const& _text)
                    {
                        EntityPtr newEntity = ecsScene->createEntity("Entity");
                        newEntity->ensureComponent<Transform2D>();
                    });

                    EditorToolsManager::GetInstancePtr()->eventHierarchyLineSceneContextMenu(_menuListTree, ecsScene);
                }
            });

        return hierarchyLine;
    }

    //////////////////////////////////////////
    void EditorHierarchyController::subscribeHierarchyLine(HierarchyLine* _hierarchyLine)
    {
        _hierarchyLine->eventRelease.subscribe(
            this,
            &EditorHierarchyController::notifyHierarchyLineRelease);
        _hierarchyLine->eventLineDoubleClick.subscribe(
            this,
            &EditorHierarchyController::notifyHierarchyLineDoubleClick);
        _hierarchyLine->eventLineClick.subscribe(
            this,
            &EditorHierarchyController::notifyHierarchyLineClick);
        _hierarchyLine->eventLineCursorPressIn.subscribe(
            this,
            &EditorHierarchyController::notifyHierarchyLineCursorPressIn);
    }

    //////////////////////////////////////////
    void EditorHierarchyController::unsubscribeHierarchyLine(HierarchyLine* _hierarchyLine)
    {
        _hierarchyLine->eventRelease.unsubscribe(this);
        _hierarchyLine->eventLineClick.unsubscribe(this);
        _hierarchyLine->eventLineCursorPressIn.unsubscribe(this);
        _hierarchyLine->eventLineDoubleClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    void EditorHierarchyController::removeHierarchyLine(HierarchyLine* _hierarchyLine)
    {
        unsubscribeHierarchyLine(_hierarchyLine);

        switch (_hierarchyLine->getType())
        {
            case HierarchyLineType::Entity:
            {
                EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(_hierarchyLine->getUserData()));
                m_entityLines.erase(m_entityLines.find(entityId));
                break;
            }
            case HierarchyLineType::Scene:
            {
                EcsSceneId sceneId = (EcsSceneId)((S32)reinterpret_cast<Size>(_hierarchyLine->getUserData()));
                m_sceneLines.erase(m_sceneLines.find(sceneId));
                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyHierarchyLineDoubleClick(HierarchyLine* _hierarchyLine)
    {
        if (_hierarchyLine->hasChildren())
            _hierarchyLine->setExpanded(!_hierarchyLine->getExpanded());
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyHierarchyLineClick(HierarchyLine* _hierarchyLine)
    {
        switch (_hierarchyLine->getType())
        {
            case HierarchyLineType::Entity:
            {
                EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(_hierarchyLine->getUserData()));

                EntityPtr const& entity = m_world->getEntity(entityId);
                if (entity)
                {
                    if (SelectionManager::GetInstancePtr()->isObjectSelected(entity))
                        SelectionManager::GetInstancePtr()->unselectObject(entity);
                    else
                        SelectionManager::GetInstancePtr()->selectObject(entity);
                }

                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyHierarchyLineCursorPressIn(HierarchyLine* _hierarchyLine)
    {

    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyEventManagerEvent(
        ClassUID _eventUID,
        Event* _event)
    {
        if (_eventUID == ClassInfo<EcsSceneStateChangedEvent>::UID())
        {
            EcsSceneStateChangedEvent* evt = _event->castRaw<EcsSceneStateChangedEvent>();
            processEcsSceneStateChanged(evt->getSceneId(), evt->getState());
        }
        else
        if (_eventUID == ClassInfo<EntityNameChangedEvent>::UID())
        {
            EntityNameChangedEvent* evt = _event->castRaw<EntityNameChangedEvent>();

            if (m_world == evt->getWorld())
            {
                updateEntityName(m_world->getEntity(evt->getEntityId()));
            }
        }
        else
        if (_eventUID == ClassInfo<EcsEntityActiveChangedEvent>::UID())
        {
            EcsEntityActiveChangedEvent* evt = _event->castRaw<EcsEntityActiveChangedEvent>();

            if (m_world == evt->getWorld())
            {
                updateEntity(m_world->getEntity(evt->getEntityId()));
            }
        }
    }


    //////////////////////////////////////////
    void EditorHierarchyController::notifySelectionChanged()
    {
        for (auto it : m_entityLines)
        {
            EntityPtr entity = m_world->getEntity(it.first);
            if (entity)
                updateEntity(it.second, entity);
        }
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyEntityAdded(EntityPtr const& _entity)
    {
        if (!_entity->getEcsScene())
            return;
        
        if (!addEcsScene(_entity->getEcsScene()->cast<EcsScene>()))
            return;

        addEntity(_entity);
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyEntityRemoved(EntityPtr const& _entity)
    {
        removeEntity(_entity->getId());
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyEntityChanged(EntityPtr const& _entity)
    {
        updateEntity(_entity);
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyHierarchyLineRelease(HierarchyLine* _hierarchyLine)
    {
        removeHierarchyLine(_hierarchyLine);
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyEditorSceneModeChanged(EditorSceneMode _mode)
    {

    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyPlaytestModeEnabled(bool _value)
    {
        setEcsWorld(EditorManager::GetInstancePtr()->getMainEcsWorld());
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyPrefabEntityChanged(EntityPtr const& _entity)
    {

    }

    
} // namespace Maze
//////////////////////////////////////////
