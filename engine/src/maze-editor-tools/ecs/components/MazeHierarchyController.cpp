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
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"
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
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionEntityRemove.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class HierarchyController
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(HierarchyController, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(HierarchyController);

    //////////////////////////////////////////
    HierarchyController::HierarchyController()
    {
    }

    //////////////////////////////////////////
    HierarchyController::~HierarchyController()
    {
        EventManager::GetInstancePtr()->unsubscribeEvent<EcsSceneStateChangedEvent>(this);
        EventManager::GetInstancePtr()->unsubscribeEvent<EntityNameChangedEvent>(this);
        EventManager::GetInstancePtr()->unsubscribeEvent<EcsEntityActiveChangedEvent>(this);

        if (SelectionManager::GetInstancePtr())
            SelectionManager::GetInstancePtr()->eventSelectionChanged.unsubscribe(this);

        setEcsWorld(nullptr);
    }

    //////////////////////////////////////////
    HierarchyControllerPtr HierarchyController::Create(Canvas* _canvas)
    {
        HierarchyControllerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(HierarchyController, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool HierarchyController::init(Canvas* _canvas)
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        m_canvas = _canvas;

        return true;
    }

    //////////////////////////////////////////
    void HierarchyController::update(F32 _dt)
    {
        U32 startTime = m_timer.getMilliseconds();

        bool delayedEntitiesChanged = false;
        while (!m_delayedEntitiesToAdd.empty() && m_timer.getMilliseconds() - startTime < 5)
        {
            EntityPtr entity = m_delayedEntitiesToAdd.begin()->second;
            m_delayedEntitiesToAdd.erase(m_delayedEntitiesToAdd.begin());
            addEntity(entity);

            delayedEntitiesChanged = true;
        }

        if (delayedEntitiesChanged)
        {
            if (!m_delayedEntitiesToAdd.empty())
                m_titleText->setTextFormatted("Hierarchy (Loading: %d)", (S32)m_delayedEntitiesToAdd.size());
            else
                m_titleText->setTextFormatted("Hierarchy");
        }

    }

    //////////////////////////////////////////
    void HierarchyController::processEntityAwakened()
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

        m_titleText = EditorToolsUIHelper::CreateText(
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
        m_titleText->setColor(ColorU32::c_black);
        
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



        setEcsWorld(EntityManager::GetInstancePtr()->getDefaultWorldRaw());
        EventManager::GetInstancePtr()->subscribeEvent<EcsSceneStateChangedEvent>(
            this,
            &HierarchyController::notifyEventManagerEvent);

        EventManager::GetInstancePtr()->subscribeEvent<EntityNameChangedEvent>(
            this,
            &HierarchyController::notifyEventManagerEvent);
        EventManager::GetInstancePtr()->subscribeEvent<EcsEntityActiveChangedEvent>(
            this,
            &HierarchyController::notifyEventManagerEvent);

        SelectionManager::GetInstancePtr()->eventSelectionChanged.subscribe(
            this,
            &HierarchyController::notifySelectionChanged);
    }
    
    //////////////////////////////////////////
    void HierarchyController::setEcsWorld(EcsWorld* _world)
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
                    m_sceneLines.erase(it);
            }
        }

        m_world = _world;

        if (m_world)
        {
            m_world->eventEntityAdded.subscribe(this, &HierarchyController::notifyEntityAdded);
            m_world->eventEntityRemoved.subscribe(this, &HierarchyController::notifyEntityRemoved);
            m_world->eventEntityChanged.subscribe(this, &HierarchyController::notifyEntityChanged);

            for (SceneManager::SceneData const& sceneData : SceneManager::GetInstancePtr()->getScenes())
                if (sceneData.scene && sceneData.scene->getState() == EcsSceneState::Active)
                    addEcsScene(sceneData.scene);
        }
    }

    //////////////////////////////////////////
    void HierarchyController::processEcsSceneStateChanged(
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
    HierarchyLinePtr HierarchyController::addEcsScene(EcsScenePtr const& _scene)
    {
        MAZE_PROFILE_EVENT("HierarchyController::addEcsScene");

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

        Debug::Log("HierarchyController::addEcsScene %s...", _scene->getClassName().str);

        HierarchyLinePtr hierarchyLine = createHierarchyLine(HierarchyLineType::Scene);
        hierarchyLine->setWorld(m_world);
        hierarchyLine->setText(_scene->getMetaClass()->getName().str);
        hierarchyLine->getTransform()->setParent(m_hierarchyMainLayoutEntity);
        hierarchyLine->setUserData(reinterpret_cast<void*>((Size)(S32)_scene->getId()));

        m_sceneLines[_scene->getId()] = hierarchyLine;

        for (Entity* entity : _scene->getEntities())
        {
            if (entity->getAdding() || entity->getRemoving())
                continue;

            m_delayedEntitiesToAdd.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(entity->getId()),
                std::forward_as_tuple(entity->cast<Entity>()));
        }

        Debug::Log("HierarchyController::addEcsScene %s finished.", _scene->getClassName().str);

        return hierarchyLine;
    }

    //////////////////////////////////////////
    void HierarchyController::removeEcsScene(EcsSceneId _sceneId)
    {
        MAZE_PROFILE_EVENT("HierarchyController::removeEcsScene");

        auto sceneHierarchyLineIt = m_sceneLines.find(_sceneId);
        if (sceneHierarchyLineIt == m_sceneLines.end())
            return;

        sceneHierarchyLineIt->second->release();
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyController::addEntity(EntityPtr const& _entity)
    {
        MAZE_PROFILE_EVENT("HierarchyController::addEntity");

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
            auto sceneIt = m_sceneLines.find(_entity->getEcsScene()->getId());
            MAZE_ERROR_RETURN_VALUE_IF(sceneIt == m_sceneLines.end(), nullptr, "Undefined scene");
            parentLine = sceneIt->second;
        }

        MAZE_ERROR_RETURN_VALUE_IF(!parentLine, nullptr, "Parent line is not found!");

        HierarchyLinePtr hierarchyLine = createHierarchyLine(HierarchyLineType::Entity);
        hierarchyLine->setWorld(m_world);
        hierarchyLine->setUserData(reinterpret_cast<void*>((Size)(S32)_entity->getId()));

        parentLine->addChild(hierarchyLine);

        m_entityLines[_entity->getId()] = hierarchyLine;
        updateEntityName(_entity);
        updateEntity(hierarchyLine, _entity);

        {
            auto it = m_delayedEntitiesToAdd.find(_entity->getId());
            if (it != m_delayedEntitiesToAdd.end())
                m_delayedEntitiesToAdd.erase(it);
        }

        return hierarchyLine;
    }

    //////////////////////////////////////////
    void HierarchyController::removeEntity(EntityId _entityId)
    {
        MAZE_PROFILE_EVENT("HierarchyController::removeEntity");

        auto entityHierarchyLineIt = m_entityLines.find(_entityId);
        if (entityHierarchyLineIt == m_entityLines.end())
        {
            auto it = m_delayedEntitiesToAdd.find(_entityId);
            if (it != m_delayedEntitiesToAdd.end())
                m_delayedEntitiesToAdd.erase(it);
                
            return;
        }

        entityHierarchyLineIt->second->release();
    }

    //////////////////////////////////////////
    void HierarchyController::updateEntity(EntityPtr const& _entity)
    {
        MAZE_PROFILE_EVENT("HierarchyController::updateEntity");

        if (!_entity)
            return;

        auto it = m_entityLines.find(_entity->getId());
        if (it == m_entityLines.end())
            return;

        HierarchyLinePtr const& hierarchyLine = it->second;
        updateEntity(hierarchyLine, _entity);
    }

    //////////////////////////////////////////
    void HierarchyController::updateEntity(
        HierarchyLinePtr const& hierarchyLine,
        EntityPtr const& _entity)
    {
        hierarchyLine->setSelected(
            SelectionManager::GetInstancePtr()->isObjectSelected(_entity));

        hierarchyLine->setActive(
            _entity->getActiveInHierarchy());
    }

    //////////////////////////////////////////
    void HierarchyController::updateEntityName(EntityPtr const& _entity)
    {
        auto it = m_entityLines.find(_entity->getId());
        if (it == m_entityLines.end())
            return;

        HierarchyLinePtr const& hierarchyLine = it->second;

        CString name = EcsHelper::GetName(_entity.get());
        hierarchyLine->setText(name && name[0] ? name : "Unnamed");
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyController::createHierarchyLine(HierarchyLineType _type)
    {
        MAZE_PROFILE_EVENT("HierarchyController::createHierarchyLine");

        HierarchyLinePtr hierarchyLine = m_hierarchyLinePool->createHierarchyLine(_type);
        hierarchyLine->setWorld(m_world);
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
                            CString name = EcsHelper::GetName(entity.get());
                            String filename = name && name[0] ? name : "Unnamed";
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
    void HierarchyController::subscribeHierarchyLine(HierarchyLine* _hierarchyLine)
    {
        MAZE_PROFILE_EVENT("HierarchyController::subscribeHierarchyLine");

        _hierarchyLine->eventRelease.subscribe(
            this,
            &HierarchyController::notifyHierarchyLineRelease);
        _hierarchyLine->eventLineDoubleClick.subscribe(
            this,
            &HierarchyController::notifyHierarchyLineDoubleClick);
        _hierarchyLine->eventLineClick.subscribe(
            this,
            &HierarchyController::notifyHierarchyLineClick);
        _hierarchyLine->eventLineCursorPressIn.subscribe(
            this,
            &HierarchyController::notifyHierarchyLineCursorPressIn);
    }

    //////////////////////////////////////////
    void HierarchyController::unsubscribeHierarchyLine(HierarchyLine* _hierarchyLine)
    {
        _hierarchyLine->eventRelease.unsubscribe(this);
        _hierarchyLine->eventLineClick.unsubscribe(this);
        _hierarchyLine->eventLineCursorPressIn.unsubscribe(this);
        _hierarchyLine->eventLineDoubleClick.unsubscribe(this);
    }

    //////////////////////////////////////////
    void HierarchyController::removeHierarchyLine(HierarchyLine* _hierarchyLine)
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
    void HierarchyController::notifyHierarchyLineDoubleClick(HierarchyLine* _hierarchyLine)
    {
        if (_hierarchyLine->hasChildren())
            _hierarchyLine->setExpanded(!_hierarchyLine->getExpanded());
    }

    //////////////////////////////////////////
    void HierarchyController::notifyHierarchyLineClick(HierarchyLine* _hierarchyLine)
    {
        switch (_hierarchyLine->getType())
        {
            case HierarchyLineType::Entity:
            {
                EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(_hierarchyLine->getUserData()));

                EntityPtr const& entity = EntityManager::GetInstancePtr()->getDefaultWorldRaw()->getEntity(entityId);
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
    void HierarchyController::notifyHierarchyLineCursorPressIn(HierarchyLine* _hierarchyLine)
    {

    }

    //////////////////////////////////////////
    void HierarchyController::notifyEventManagerEvent(
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
    void HierarchyController::notifySelectionChanged()
    {
        for (auto it : m_entityLines)
        {
            EntityPtr entity = m_world->getEntity(it.first);
            if (entity)
                updateEntity(it.second, entity);
        }
    }

    //////////////////////////////////////////
    void HierarchyController::notifyEntityAdded(EntityPtr const& _entity)
    {
        if (!_entity->getEcsScene())
            return;
        
        if (!addEcsScene(_entity->getEcsScene()->cast<EcsScene>()))
            return;

        m_delayedEntitiesToAdd.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_entity->getId()),
            std::forward_as_tuple(_entity->cast<Entity>()));
    }

    //////////////////////////////////////////
    void HierarchyController::notifyEntityRemoved(EntityPtr const& _entity)
    {
        removeEntity(_entity->getId());
    }

    //////////////////////////////////////////
    void HierarchyController::notifyEntityChanged(EntityPtr const& _entity)
    {
        updateEntity(_entity);
    }

    //////////////////////////////////////////
    void HierarchyController::notifyHierarchyLineRelease(HierarchyLine* _hierarchyLine)
    {
        removeHierarchyLine(_hierarchyLine);
    }

    
} // namespace Maze
//////////////////////////////////////////
