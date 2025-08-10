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
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-core/ecs/components/MazePrefabInstance.hpp"
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
#include "maze-editor-tools/helpers/MazeEditorActionHelper.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityRemove.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityAdd.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionSelectEntities.hpp"
#include "maze-engine/ecs/scenes/MazeSceneEnginePlayer.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "scenes/ScenePlaytest.hpp"
#include "scenes/SceneWorkspace.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorPrefabManager.hpp"
#include "managers/EditorSceneManager.hpp"
#include "managers/EditorEntityManager.hpp"
#include "helpers/EditorHelper.hpp"
#include "ecs/components/EditorHierarchyControllerAgent.hpp"


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

        if (EditorSceneManager::GetInstancePtr())
            EditorSceneManager::GetInstancePtr()->eventSceneChanged.unsubscribe(this);
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
        EditorSceneManager::GetInstancePtr()->eventSceneChanged.subscribe(this, &EditorHierarchyController::notifySceneChanged);

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

        updateMode();
    }
    
    //////////////////////////////////////////
    void EditorHierarchyController::addIgnoreScene(ClassUID _classUID)
    {
        m_ignoreScenes.insert(_classUID);

        if (SceneManager::GetInstancePtr())
            removeEcsScene(SceneManager::GetInstancePtr()->getEcsSceneIdByClassUID(_classUID));
    }

    //////////////////////////////////////////
    void EditorHierarchyController::setEcsWorld(EcsWorld* _world)
    {
        if (m_world == _world)
            return;

        if (m_agent)
        {
            if (m_agent->getEntityRaw())
                m_agent->getEntityRaw()->removeFromEcsWorld();
            m_agent.reset();
        }

        if (m_world)
        {
            m_world->eventEntityAdded.unsubscribe(this);
            m_world->eventEntityRemoved.unsubscribe(this);
            m_world->eventEntityChanged.unsubscribe(this);
            m_world->eventOnDestroy.unsubscribe(this);

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
                    m_sceneLines.erase(it);
            }
        }

        m_world = _world;

        if (m_world)
        {
            m_world->eventEntityAdded.subscribe(this, &EditorHierarchyController::notifyEntityAdded);
            m_world->eventEntityRemoved.subscribe(this, &EditorHierarchyController::notifyEntityRemoved);
            m_world->eventEntityChanged.subscribe(this, &EditorHierarchyController::notifyEntityChanged);
            m_world->eventOnDestroy.subscribe(this, &EditorHierarchyController::notifyWorldDestroyed);

            if (m_hierarchyMode == EditorHierarchyMode::Scene)
                for (SceneManager::SceneData const& sceneData : SceneManager::GetInstancePtr()->getScenes())
                    if (sceneData.scene && sceneData.scene->getState() != EcsSceneState::Destroy)
                        addEcsScene(sceneData.scene);

            EntityPtr agentEntity = m_world->createEntity();
            agentEntity->createComponent<StaticName>("EditorHierarchyController");
            m_agent = agentEntity->createComponent<EditorHierarchyControllerAgent>(this);
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
        if (m_hierarchyMode != EditorHierarchyMode::Scene)
            return nullptr;

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
        hierarchyLine->setWorld(m_world);
        hierarchyLine->setText(_scene->getSceneName().str);
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

        EntityId parentId = EcsHelper::GetEntityParentId(_entity.get());

        HierarchyLinePtr parentLine;
        if (parentId)
        {
            EntityPtr const& parentEntity = m_world->getEntity(parentId);
            MAZE_ERROR_RETURN_VALUE_IF(!parentEntity, nullptr, "Parent entity not found!");
            parentLine = addEntity(parentEntity);
        }
        else
        {
            if (m_hierarchyMode == EditorHierarchyMode::Scene)
            {
                auto sceneIt = m_sceneLines.find(_entity->getEcsScene()->getId());
                MAZE_ERROR_RETURN_VALUE_IF(sceneIt == m_sceneLines.end(), nullptr, "Undefined scene");
                parentLine = sceneIt->second;
            }
        }

        MAZE_ERROR_RETURN_VALUE_IF(m_hierarchyMode == EditorHierarchyMode::Scene && !parentLine, nullptr, "Parent line is not found!");

        HierarchyLinePtr hierarchyLine = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine->setWorld(m_world);
        hierarchyLine->setUserData(reinterpret_cast<void*>((Size)(S32)_entity->getId()));

        if (parentLine)
            parentLine->addChild(hierarchyLine);
        else
            hierarchyLine->getTransform()->setParent(m_hierarchyMainLayoutEntity);

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
        HierarchyLinePtr const& _hierarchyLine,
        EntityPtr const& _entity)
    {
        _hierarchyLine->setSelected(
            SelectionManager::GetInstancePtr()->isObjectSelected(_entity));

        _hierarchyLine->setActive(
            _entity->getActiveInHierarchy());

        SpritePtr sprite = UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::EntityObject3D);
        ColorU32 color = ColorU32::c_black;

        if (_entity->getComponentRaw<PrefabInstance>())
        {
            sprite = UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Prefab);
            color = ColorU32::c_white;
        }

        _hierarchyLine->getIconRenderer()->setSprite(sprite);
        _hierarchyLine->getIconRenderer()->setColor(color);
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
        hierarchyLine->setWorld(m_world);
        subscribeHierarchyLine(hierarchyLine.get());

        EditorHierarchyControllerWPtr controllerWeak = cast<EditorHierarchyController>();

        HierarchyLineWPtr hierarchyLineWeak = hierarchyLine;
        hierarchyLine->getContextMenu()->setCallbackFunction(
            [controllerWeak, hierarchyLineWeak](MenuListTree2DPtr const& _menuListTree)
        {
                EditorHierarchyControllerPtr controller = controllerWeak.lock();
                if (!controller)
                    return;
                HierarchyLinePtr hierarchyLine = hierarchyLineWeak.lock();
                if (!hierarchyLine)
                    return;

                if (hierarchyLine->getType() == HierarchyLineType::Entity)
                {
                    if (!hierarchyLine->getEntityRaw()->getEcsWorld() || !controller->m_world)
                        return;

                    EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(hierarchyLine->getUserData()));

                    EntityPtr const& entity = controller->m_world->getEntity(entityId);

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
                            if (EditorToolsActionManager::GetInstancePtr())
                                EditorToolsActionManager::GetInstancePtr()->applyAction(
                                    EditorActionEntityRemove::Create(entity));
                            else
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

                                if (EditorToolsActionManager::GetInstancePtr())
                                    EditorToolsActionManager::GetInstancePtr()->applyActions(
                                        EditorActionEntityAdd::Create(entityCopy),
                                        EditorActionSelectEntities::Create(true, entityCopy));
                            }
                        });

                        _menuListTree->addItem(
                            "Add Child/3D/Empty",
                            [transform3D](String const& _text)
                        {
                            EntityPtr newEntity = EditorToolsHelper::CreateEntity3D("Entity");
                            Transform3DPtr newEntityTransform = newEntity->getComponent<Transform3D>();

                            newEntityTransform->setParent(transform3D->cast<Transform3D>());

                            if (EditorToolsActionManager::GetInstancePtr())
                                EditorToolsActionManager::GetInstancePtr()->applyActions(
                                    EditorActionEntityAdd::Create(newEntity),
                                    EditorActionSelectEntities::Create(true, newEntity));
                        });
                    }
                    else
                    {
                        Transform2D* transform2D = entity->getComponentRaw<Transform2D>();
                        if (transform2D)
                        {
                            _menuListTree->addItem(
                                "Duplicate",
                                [entityWeak](String const& _text)
                                {
                                    EntityPtr entity = entityWeak.lock();
                                    if (entity)
                                    {
                                        EntityPtr entityCopy = entity->createCopy();
                                        entityCopy->ensureComponent<Transform2D>()->setParent(
                                            entity->ensureComponent<Transform2D>()->getParent());

                                        if (EditorToolsActionManager::GetInstancePtr())
                                            EditorToolsActionManager::GetInstancePtr()->applyActions(
                                                EditorActionEntityAdd::Create(entityCopy),
                                                EditorActionSelectEntities::Create(true, entityCopy));
                                    }
                                });

                            _menuListTree->addItem(
                                "Add Child/2D/Empty",
                                [transform2D](String const& _text)
                                { EditorHelper::CreateEntity2D("Sprite", transform2D->cast<Transform2D>()); });

                            _menuListTree->addItem(
                                "Add Child/2D/Sprite",
                                [transform2D](String const& _text)
                                { EditorHelper::CreateSprite2D("Sprite", transform2D->cast<Transform2D>()); });

                            _menuListTree->addItem(
                                "Add Child/2D/Toggle Button",
                                [transform2D](String const& _text)
                                { EditorHelper::CreateToggleButton2D("Toggle Button", transform2D->cast<Transform2D>()); });

                            _menuListTree->addItem(
                                "Add Child/2D/Click Button",
                                [transform2D](String const& _text)
                                { EditorHelper::CreateClickButton2D("Click Button", transform2D->cast<Transform2D>()); });

                            _menuListTree->addItem(
                                "Add Child/2D/Text",
                                [transform2D](String const& _text)
                            { EditorHelper::CreateText("Text", transform2D->cast<Transform2D>()); });
                        }
                    }

                    EditorToolsManager::GetInstancePtr()->eventHierarchyLineEntityContextMenu(_menuListTree, entity.get());
                }
                else
                if (hierarchyLine->getType() == HierarchyLineType::Scene)
                {
                    EcsSceneId ecsSceneId = (EcsSceneId)((S32)reinterpret_cast<Size>(hierarchyLine->getUserData()));
                    EcsScenePtr const& scene = SceneManager::GetInstancePtr()->getScene(ecsSceneId);
                    if (!scene)
                        return;

                    _menuListTree->addItem(
                        "Add Child/3D/Empty",
                        [ecsSceneId](String const& _text)
                    {
                        EcsScenePtr const& scene = SceneManager::GetInstancePtr()->getScene(ecsSceneId);
                        if (!scene)
                            return;

                        EntityPtr newEntity = scene->createEntity("Entity");
                        newEntity->ensureComponent<Transform3D>();

                        if (EditorToolsActionManager::GetInstancePtr())
                            EditorToolsActionManager::GetInstancePtr()->applyActions(
                                EditorActionEntityAdd::Create(newEntity),
                                EditorActionSelectEntities::Create(true, newEntity));
                    });

                    _menuListTree->addItem(
                        "Add Child/2D/Empty",
                        [ecsSceneId](String const& _text)
                    {
                        EcsScenePtr const& scene = SceneManager::GetInstancePtr()->getScene(ecsSceneId);
                        if (!scene)
                            return;

                        EntityPtr newEntity = scene->createEntity("Entity");
                        newEntity->ensureComponent<Transform2D>();

                        if (EditorToolsActionManager::GetInstancePtr())
                            EditorToolsActionManager::GetInstancePtr()->applyActions(
                                EditorActionEntityAdd::Create(newEntity),
                                EditorActionSelectEntities::Create(true, newEntity));
                    });

                    EditorToolsManager::GetInstancePtr()->eventHierarchyLineSceneContextMenu(_menuListTree, scene.get());
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
                        EditorActionHelper::UnselectEntities(entity);
                    else
                        EditorActionHelper::SelectEntities(entity);
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
        
        if (m_hierarchyMode == EditorHierarchyMode::Scene)
        {
            if (!addEcsScene(_entity->getEcsScene()->cast<EcsScene>()))
                return;
        }
        else
        if (m_hierarchyMode == EditorHierarchyMode::Prefab)
        {
            if (!_entity->getEcsScene() ||
                (_entity->getEcsScene()->getClassUID() != ClassInfo<SceneWorkspace>::UID() &&
                _entity->getEcsScene()->getClassUID() != ClassInfo<ScenePlaytest>::UID() &&
                _entity->getEcsScene()->getClassUID() != ClassInfo<SceneEnginePlayer>::UID()))
                return;
        }

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
    void EditorHierarchyController::notifyWorldDestroyed(EcsWorld* _world)
    {
        if (m_world == _world)
            setEcsWorld(nullptr);
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
        updateMode();
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifyPrefabEntityChanged(EntityPtr const& _entity)
    {
        if (m_hierarchyMode == EditorHierarchyMode::Prefab && !_entity)
            setHierarchyMode(EditorHierarchyMode::None);
        else
        if (m_hierarchyMode != EditorHierarchyMode::Prefab && _entity)
        {
            setHierarchyMode(EditorHierarchyMode::Prefab);
            updateMode();
        }
    }

    //////////////////////////////////////////
    void EditorHierarchyController::notifySceneChanged(EcsAssetScenePtr const& _scene)
    {
        if (m_hierarchyMode == EditorHierarchyMode::Scene && !_scene)
            setHierarchyMode(EditorHierarchyMode::None);
        else
        if (m_hierarchyMode != EditorHierarchyMode::Scene && _scene)
        {
            setHierarchyMode(EditorHierarchyMode::Scene);
            updateMode();
        }
    }

    //////////////////////////////////////////
    void EditorHierarchyController::updateMode()
    {
        switch (m_hierarchyMode)
        {
            case EditorHierarchyMode::None:
            {
                setEcsWorld(nullptr);
                break;
            }
            case EditorHierarchyMode::Scene:
            {
                setEcsWorld(EditorManager::GetInstancePtr()->getMainEcsWorld());
                break;
            }
            case EditorHierarchyMode::Prefab:
            {
                setEcsWorld(nullptr);
                setEcsWorld(EditorManager::GetInstancePtr()->getMainEcsWorld());
                addEntity(EditorPrefabManager::GetInstancePtr()->getPrefabEntity());
                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void EditorHierarchyController::setHierarchyMode(EditorHierarchyMode _mode)
    {
        if (m_hierarchyMode == _mode)
            return;

        m_hierarchyMode = _mode;

        updateMode();
    }

    //////////////////////////////////////////
    void EditorHierarchyController::processEntityParentChanged(
        EntityId _entityId,
        EntityId _prevParentEntityId,
        EntityId _newParentEntityId)
    {
        auto entityIt = m_entityLines.find(_entityId);
        if (entityIt == m_entityLines.end())
            return;

        EntityPtr const& entity = m_world->getEntity(_entityId);

        if (_newParentEntityId == c_invalidEntityId && entity->getEcsScene())
        {
            auto sceneIt = m_sceneLines.find(entity->getEcsScene()->getId());
            if (sceneIt != m_sceneLines.end())
            {
                sceneIt->second->addChild(entityIt->second);
                sceneIt->second->setExpanded(true);
            }
            else
            {
                MAZE_ERROR("Scene is not found!");
            }
        }
        else
        {
            auto newParentEntityIt = m_entityLines.find(_newParentEntityId);
            if (newParentEntityIt != m_entityLines.end())
            {
                newParentEntityIt->second->addChild(entityIt->second);
                newParentEntityIt->second->setExpanded(true);
            }
            else
            {
                MAZE_ERROR("Parent is not found!");
                entityIt->second->getTransform()->setParent(m_hierarchyMainLayoutEntity);
            }
        }

        auto prevParentEntityIt = m_entityLines.find(_prevParentEntityId);
        if (prevParentEntityIt != m_entityLines.end())
        {
            prevParentEntityIt->second->updateDropDownRenderer();
        }
    }


    
} // namespace Maze
//////////////////////////////////////////
