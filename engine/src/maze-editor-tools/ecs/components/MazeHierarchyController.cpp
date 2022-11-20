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
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
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
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsLayout.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLine.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLinePool.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
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
        for (auto& hierarchyLineData : m_hierarchyLinesPerEntity)
        {
            hierarchyLineData.second.line->eventDropDownClick.unsubscribe(this);
            hierarchyLineData.second.line->eventLineClick.unsubscribe(this);
        }

        for (auto& hierarchyLineData : m_hierarchyLinesPerScene)
        {
            hierarchyLineData.second.line->eventDropDownClick.unsubscribe(this);
            hierarchyLineData.second.line->eventLineClick.unsubscribe(this);
        }

        setECSWorld(nullptr);
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

        setECSWorld(EntityManager::GetInstancePtr()->getDefaultWorldRaw());

        m_canvas = _canvas;

        return true;
    }

    //////////////////////////////////////////
    void HierarchyController::update(F32 _dt)
    {
        updateHierarchy();
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
            Vec2DF(0.0f, 0.0f),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_titleTransform->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>();

        m_titleBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2DF(
                m_canvas->getTransform()->getSize().x,
                EditorToolsLayout::c_titleHeight),
            Vec2DF(0.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            m_titleTransform,
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f));
        m_titleBackground->setColor(EditorToolsLayout::c_titleBackgroundColor);
        m_titleBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setFlag(SizePolicy2D::Height, false);

        SystemTextRenderer2DPtr hierarchyText = SystemUIHelper::CreateSystemText(
            "Hierarchy",
            EditorToolsLayout::c_titleFontSize,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2DF(100, EditorToolsLayout::c_titleHeight),
            Vec2DF(EditorToolsLayout::c_titleLabelShift, 0),            
            m_titleBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 0.5f),
            Vec2DF(0.0f, 0.5f));
        hierarchyText->setColor(ColorU32::c_black);
        
        m_bodyBackground = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02),
            Vec2DF(
                m_titleTransform->getSize().x, 
                m_titleTransform->getSize().y - EditorToolsLayout::c_titleHeight),
            Vec2DF(0.0f, 0.0f),
            materialManager->getColorTextureMaterial(),
            m_canvas->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF::c_zero,
            Vec2DF::c_zero);
        m_bodyBackground->setColor(EditorToolsLayout::c_bodyBackgroundColor);
        m_bodyBackground->getEntityRaw()->ensureComponent<Maze::SizePolicy2D>()->setSizeDelta(0.0f, -EditorToolsLayout::c_titleHeight);

        ScrollRect2DPtr scrollRect = UIHelper::CreateDefaultScrollRect(
            m_bodyBackground->getTransform()->getSize(),
            Vec2DF::c_zero,
            m_bodyBackground->getTransform(),
            getEntityRaw()->getECSScene(),
            Vec2DF(0.0f, 1.0f),
            Vec2DF(0.0f, 1.0f),
            false,
            true);
        scrollRect->getViewportTransform()->getEntityRaw()->getComponent<ScissorMask2D>()->setPadding(0, 0, 0, 0);
        scrollRect->getEntityRaw()->ensureComponent<SizePolicy2D>();
        scrollRect->getEntityRaw()->getComponent<MeshRenderer>()->setEnabled(false);

        m_layoutTransform = scrollRect->getContentTransform();
        
        updateHierarchy();
    }

    //////////////////////////////////////////
    void HierarchyController::updateHierarchy()
    {
        for (auto& hierarchyLineData : m_hierarchyLinesPerEntity)
            hierarchyLineData.second.line->getEntityRaw()->setActiveSelf(false);

        for (auto& hierarchyLineData : m_hierarchyLinesPerScene)
            hierarchyLineData.second.line->getEntityRaw()->setActiveSelf(false);

        SceneManager* sceneManager = SceneManager::GetInstancePtr();

        F32 x = 0;
        F32 y = 0;

        Vector<ECSScenePtr> scenes = sceneManager->getScenes();
        for (ECSScenePtr const& scene : scenes)
        {
            ClassUID sceneUID = scene->getClassUID();
            if (sceneUID == getEntityRaw()->getECSScene()->getClassUID())
                continue;

            if (m_ignoreScenes.find(sceneUID) != m_ignoreScenes.end())
                continue;

            HierarchyLineSceneData& hierarchyLineData = m_hierarchyLinesPerScene[scene.get()];
            HierarchyLinePtr hierarchyLine = createHierarchyLine(scene);
            hierarchyLine->setLabel(scene->getClassName());
            hierarchyLine->getTransform()->setParent(m_layoutTransform);
            hierarchyLine->getTransform()->setLocalPosition(Vec2DF(10 + x * 10, -6 - y * 12));
            hierarchyLine->setColor(ColorU32(30, 125, 0));

            if (scene->getEntities().empty())
            {
                hierarchyLine->setDropDownState(HierarchyLineDropDownState::None);
            }
            else
            {
                if (hierarchyLineData.expanded)
                    hierarchyLine->setDropDownState(HierarchyLineDropDownState::Expanded);
                else
                    hierarchyLine->setDropDownState(HierarchyLineDropDownState::Collapsed);
            }

            ++y;

            if (hierarchyLineData.expanded)
            {
                for (Entity* entity : scene->getEntities())
                {
                    Transform3D* transform3D = entity->getComponentRaw<Transform3D>();
                    if (transform3D)
                    {
                        if (!transform3D->getParent())
                            updateHierarchyElement(transform3D, x + 1, y);
                    }
                    else
                    {
                        Transform2D* transform2D = entity->getComponentRaw<Transform2D>();
                        if (transform2D)
                        {
                            if (!transform2D->getParent())
                                updateHierarchyElement(transform2D, x + 1, y);
                        }
                        else
                        {
                            String name = "Unnamed";

                            Name* componentName = entity->getComponentRaw<Name>();
                            if (componentName)
                                name = componentName->getName();

                            HierarchyLinePtr hierarchyLine = createHierarchyElement(
                                entity,
                                name.c_str(),
                                Vec2DF(10 + (x + 1) * 10, -6 - y * 12));

                            ++y;
                        }
                    }
                }
            }

            m_layoutTransform->setHeight(y * 12 + 6);
            // ++y;
        }
    }

    //////////////////////////////////////////
    void HierarchyController::updateHierarchyElement(Transform2D* _transform2D, F32 _x, F32& _y)
    {
        String name = "Unnamed 2D";

        Name* componentName = _transform2D->getEntityRaw()->getComponentRaw<Name>();
        if (componentName)
            name = componentName->getName();

        HierarchyLineEntityData& hierarchyLineData = m_hierarchyLinesPerEntity[_transform2D->getEntityId()];
        HierarchyLinePtr hierarchyLine = createHierarchyElement(
            _transform2D,
            name.c_str(),
            Vec2DF(10 + _x * 10, -6 - _y * 12));

        if (_transform2D->getChildren().empty())
        {
            hierarchyLine->setDropDownState(HierarchyLineDropDownState::None);
        }
        else
        {
            if (hierarchyLineData.expanded)
                hierarchyLine->setDropDownState(HierarchyLineDropDownState::Expanded);
            else
                hierarchyLine->setDropDownState(HierarchyLineDropDownState::Collapsed);
        }

        ++_y;

        if (hierarchyLineData.expanded)
        {
            for (Transform2D* childTransform : _transform2D->getChildren())
            {
                updateHierarchyElement(childTransform, _x + 1, _y);
            }
        }
    }

    //////////////////////////////////////////
    void HierarchyController::updateHierarchyElement(Transform3D* _transform3D, F32 _x, F32& _y)
    {
        String name = "Unnamed 3D";

        Name* componentName = _transform3D->getEntityRaw()->getComponentRaw<Name>();
        if (componentName)
            name = componentName->getName();

        HierarchyLineEntityData& hierarchyLineData = m_hierarchyLinesPerEntity[_transform3D->getEntityId()];
        HierarchyLinePtr hierarchyLine = createHierarchyElement(
            _transform3D,
            name.c_str(),
            Vec2DF(10 + _x * 10, -6 - _y * 12));

        if (_transform3D->getChildren().empty())
        {
            hierarchyLine->setDropDownState(HierarchyLineDropDownState::None);
        }
        else
        {
            if (hierarchyLineData.expanded)
                hierarchyLine->setDropDownState(HierarchyLineDropDownState::Expanded);
            else
                hierarchyLine->setDropDownState(HierarchyLineDropDownState::Collapsed);
        }

        ++_y;

        if (hierarchyLineData.expanded)
        {
            for (Transform3D* childTransform : _transform3D->getChildren())
            {
                updateHierarchyElement(childTransform, _x + 1, _y);
            }
        }
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyController::createHierarchyElement(
        Transform2D* _transform,
        CString _name,
        Vec2DF const& _position)
    {
        HierarchyLinePtr hierarchyLine = createHierarchyLine(_transform->getEntityId());       
        hierarchyLine->setLabel(_name);
        hierarchyLine->getTransform()->setParent(m_layoutTransform);
        hierarchyLine->getTransform()->setLocalPosition(_position);
        
        if (SelectionManager::GetInstancePtr()->isObjectSelected(_transform->getEntityRaw()->getSharedPtr()))
        {
            hierarchyLine->setColor(ColorU32(0, 255, 0, 255));
        }
        else
        {
            if (_transform->getEntityRaw()->getActiveInHierarchy())
            {
                hierarchyLine->setColor(ColorU32(0, 0, 0, 255));
            }
            else
            {
                if (_transform->getEntityRaw()->getActiveSelf())
                    hierarchyLine->setColor(ColorU32(0, 0, 0, 50));
                else
                    hierarchyLine->setColor(ColorU32(0, 0, 0, 85));
            }
        }

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyController::createHierarchyElement(
        Transform3D* _transform,
        CString _name,
        Vec2DF const& _position)
    {
        
        HierarchyLinePtr hierarchyLine = createHierarchyLine(_transform->getEntityId());
        
        hierarchyLine->setLabel(_name);
        hierarchyLine->getTransform()->setParent(m_layoutTransform);
        hierarchyLine->getTransform()->setLocalPosition(_position);

        if (SelectionManager::GetInstancePtr()->isObjectSelected(_transform->getEntityRaw()->getSharedPtr()))
        {
            hierarchyLine->setColor(ColorU32(0, 255, 0, 255));
        }
        else
        {
            if (_transform->getEntityRaw()->getActiveInHierarchy())
                hierarchyLine->setColor(ColorU32(0, 0, 0, 255));
            else
            {
                if (_transform->getEntityRaw()->getActiveSelf())
                    hierarchyLine->setColor(ColorU32(0, 0, 0, 50));
                else
                    hierarchyLine->setColor(ColorU32(0, 0, 0, 85));
            }
        }

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyController::createHierarchyElement(
        Entity* _entity,
        CString _name,
        Vec2DF const& _position)
    {

        HierarchyLinePtr hierarchyLine = createHierarchyLine(_entity->getId());

        hierarchyLine->setLabel(_name);
        hierarchyLine->getTransform()->setParent(m_layoutTransform);
        hierarchyLine->getTransform()->setLocalPosition(_position);

        if (SelectionManager::GetInstancePtr()->isObjectSelected(_entity->getSharedPtr()))
        {
            hierarchyLine->setColor(ColorU32(0, 255, 0, 255));
        }
        else
        {
            if (_entity->getActiveInHierarchy())
                hierarchyLine->setColor(ColorU32(0, 0, 0, 255));
            else
            {
                if (_entity->getActiveSelf())
                    hierarchyLine->setColor(ColorU32(0, 0, 0, 50));
                else
                    hierarchyLine->setColor(ColorU32(0, 0, 0, 85));
            }
        }

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyController::createHierarchyLine(EntityId _entityId)
    {
        HierarchyLineEntityData& hierarchyLineData = m_hierarchyLinesPerEntity[_entityId];
        HierarchyLinePtr& hierarchyLine = hierarchyLineData.line;

        if (!hierarchyLine)
        {
            hierarchyLine = m_hierarchyLinePool->createHierarchyLine(HierarchyLineType::Entity);
            hierarchyLine->setECSWorld(m_world);
            hierarchyLine->setUserData(reinterpret_cast<void*>((Size)_entityId));
            hierarchyLine->updateIcon();
            hierarchyLine->eventDropDownClick.subscribe(this, &HierarchyController::notifyHierarchyLineDropDownClick);
            hierarchyLine->eventLineClick.subscribe(this, &HierarchyController::notifyHierarchyLineClick);
        }

        hierarchyLine->getEntityRaw()->setActiveSelf(true);

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLinePtr HierarchyController::createHierarchyLine(ECSScenePtr const& _scene)
    {
        HierarchyLineSceneData& hierarchyLineData = m_hierarchyLinesPerScene[_scene.get()];
        HierarchyLinePtr& hierarchyLine = hierarchyLineData.line;

        if (!hierarchyLine)
        {
            hierarchyLine = m_hierarchyLinePool->createHierarchyLine(HierarchyLineType::Scene);
            hierarchyLine->setECSWorld(m_world);
            hierarchyLine->setUserData(static_cast<void*>(_scene.get()));
            hierarchyLine->eventDropDownClick.subscribe(this, &HierarchyController::notifyHierarchyLineDropDownClick);
            hierarchyLine->eventLineClick.subscribe(this, &HierarchyController::notifyHierarchyLineClick);
            CString className = _scene->getClassName();
            hierarchyLine->setName(className);
        }

        hierarchyLine->getEntityRaw()->setActiveSelf(true);

        return hierarchyLine;
    }

    //////////////////////////////////////////
    void HierarchyController::notifyHierarchyLineDropDownClick(HierarchyLine* _hierarchyLine)
    {
        switch (_hierarchyLine->getType())
        {
            case HierarchyLineType::Scene:
            {
                ECSScene* ecsScene = static_cast<ECSScene*>(_hierarchyLine->getUserData());
                HierarchyLineSceneData& hierarchyLineData = m_hierarchyLinesPerScene[ecsScene];
                hierarchyLineData.expanded = !hierarchyLineData.expanded;
                break;
            }
            case HierarchyLineType::Entity:
            {
                EntityId entityId = (EntityId)(reinterpret_cast<Size>(_hierarchyLine->getUserData()));
                HierarchyLineEntityData& hierarchyLineData = m_hierarchyLinesPerEntity[entityId];
                hierarchyLineData.expanded = !hierarchyLineData.expanded;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void HierarchyController::notifyHierarchyLineClick(HierarchyLine* _hierarchyLine)
    {
        switch (_hierarchyLine->getType())
        {
            case HierarchyLineType::Entity:
            {
                EntityId entityId = (EntityId)(reinterpret_cast<Size>(_hierarchyLine->getUserData()));

                EntityPtr const& entity = EntityManager::GetInstancePtr()->getDefaultWorldRaw()->getEntityById(entityId);

                if (SelectionManager::GetInstancePtr()->isObjectSelected(entity))
                    SelectionManager::GetInstancePtr()->unselectObject(entity);
                else
                    SelectionManager::GetInstancePtr()->selectObject(entity);

                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void HierarchyController::notifyEntityRemoved(EntityPtr const& _entity)
    {
        removeHierarchyLine(_entity);
    }

    //////////////////////////////////////////
    void HierarchyController::notifyEntityChanged(Entity* _entity)
    {
        removeHierarchyLine(_entity->getSharedPtr());
    }

    //////////////////////////////////////////
    void HierarchyController::removeHierarchyLine(EntityPtr const& _entity)
    {
        auto it = m_hierarchyLinesPerEntity.find(_entity->getId());
        if (it != m_hierarchyLinesPerEntity.end())
        {
            it->second.line->eventDropDownClick.unsubscribe(this);
            it->second.line->eventLineClick.unsubscribe(this);
            m_hierarchyLinePool->releaseHierarchyLine(it->second.line);
            m_hierarchyLinesPerEntity.erase(it);
        }
    }

    //////////////////////////////////////////
    void HierarchyController::setECSWorld(ECSWorld* _world)
    {
        if (m_world == _world)
            return;

        if (m_world)
        {
            m_world->eventEntityRemoved.unsubscribe(this);
            m_world->eventEntityChanged.unsubscribe(this);
        }

        m_world = _world;

        if (m_world)
        {
            m_world->eventEntityRemoved.subscribe(this, &HierarchyController::notifyEntityRemoved);
            m_world->eventEntityChanged.subscribe(this, &HierarchyController::notifyEntityChanged);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
