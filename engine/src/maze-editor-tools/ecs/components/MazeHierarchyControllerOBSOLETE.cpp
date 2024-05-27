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
#include "maze-editor-tools/ecs/components/MazeHierarchyControllerOBSOLETE.hpp"
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
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLineOBSOLETE.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyLinePoolOBSOLETE.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class HierarchyControllerOBSOLETE
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(HierarchyControllerOBSOLETE, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(HierarchyControllerOBSOLETE);

    //////////////////////////////////////////
    HierarchyControllerOBSOLETE::HierarchyControllerOBSOLETE()
    {
    }

    //////////////////////////////////////////
    HierarchyControllerOBSOLETE::~HierarchyControllerOBSOLETE()
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

        setEcsWorld(nullptr);
    }

    //////////////////////////////////////////
    HierarchyControllerOBSOLETEPtr HierarchyControllerOBSOLETE::Create(Canvas* _canvas)
    {
        HierarchyControllerOBSOLETEPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(HierarchyControllerOBSOLETE, object, init(_canvas));
        return object;
    }

    //////////////////////////////////////////
    bool HierarchyControllerOBSOLETE::init(Canvas* _canvas)
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        setEcsWorld(EntityManager::GetInstancePtr()->getDefaultWorldRaw());

        m_canvas = _canvas;

        return true;
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::update(F32 _dt)
    {
        MAZE_PROFILE_EVENT("HierarchyControllerOBSOLETE::update");

        updateHierarchy();
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        m_hierarchyLinePool = getEntityRaw()->ensureComponent<HierarchyLinePoolOBSOLETE>();

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
            materialManager->getColorTextureMaterial(),
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
            materialManager->getColorTextureMaterial(),
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
        scrollRect->getEntityRaw()->getComponent<MeshRenderer>()->setEnabled(false);

        m_layoutTransform = scrollRect->getContentTransform();
        
        updateHierarchy();
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::updateHierarchy()
    {
        for (auto& hierarchyLineData : m_hierarchyLinesPerEntity)
            hierarchyLineData.second.line->getEntityRaw()->setActiveSelf(false);

        for (auto& hierarchyLineData : m_hierarchyLinesPerScene)
            hierarchyLineData.second.line->getEntityRaw()->setActiveSelf(false);

        SceneManager* sceneManager = SceneManager::GetInstancePtr();

        F32 x = 0;
        F32 y = 0;

        Vector<SceneManager::SceneData> const& scenes = sceneManager->getScenes();
        for (SceneManager::SceneData const& sceneData : scenes)
        {
            EcsScenePtr const& scene = sceneData.scene;
            if (!scene)
                continue;

            ClassUID sceneUID = scene->getClassUID();
            if (sceneUID == getEntityRaw()->getEcsScene()->getClassUID())
                continue;

            if (m_ignoreScenes.find(sceneUID) != m_ignoreScenes.end())
                continue;

            HierarchyLineSceneDataOBSOLETE& hierarchyLineData = m_hierarchyLinesPerScene[scene.get()];
            HierarchyLineOBSOLETEPtr hierarchyLine = createHierarchyLineOBSOLETE(scene);
            hierarchyLine->setLabel(scene->getClassName());
            hierarchyLine->getTransform()->setParent(m_layoutTransform);
            hierarchyLine->getTransform()->setLocalPosition(Vec2F(10 + x * 10, -6 - y * 14));
            hierarchyLine->setColor(ColorU32(30, 125, 0));

            if (scene->getEntities().empty())
            {
                hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::None);
            }
            else
            {
                if (hierarchyLineData.expanded)
                    hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::Expanded);
                else
                    hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::Collapsed);
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

                            HierarchyLineOBSOLETEPtr hierarchyLine = createHierarchyElement(
                                entity,
                                name.c_str(),
                                Vec2F(10 + (x + 1) * 10, -6 - y * 14));

                            ++y;
                        }
                    }
                }
            }

            m_layoutTransform->setHeight(y * 14 + 6);
            // ++y;
        }
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::updateHierarchyElement(Transform2D* _transform2D, F32 _x, F32& _y)
    {
        String name = "Unnamed 2D";

        Name* componentName = _transform2D->getEntityRaw()->getComponentRaw<Name>();
        if (componentName)
            name = componentName->getName();

        HierarchyLineOBSOLETEEntityDataOBSOLETE& hierarchyLineData = m_hierarchyLinesPerEntity[_transform2D->getEntityId()];
        HierarchyLineOBSOLETEPtr hierarchyLine = createHierarchyElement(
            _transform2D,
            name.c_str(),
            Vec2F(10 + _x * 10, -6 - _y * 14));

        if (_transform2D->getChildren().empty())
        {
            hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::None);
        }
        else
        {
            if (hierarchyLineData.expanded)
                hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::Expanded);
            else
                hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::Collapsed);
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
    void HierarchyControllerOBSOLETE::updateHierarchyElement(Transform3D* _transform3D, F32 _x, F32& _y)
    {
        String name = "Unnamed 3D";

        Name* componentName = _transform3D->getEntityRaw()->getComponentRaw<Name>();
        if (componentName)
            name = componentName->getName();

        HierarchyLineOBSOLETEEntityDataOBSOLETE& hierarchyLineData = m_hierarchyLinesPerEntity[_transform3D->getEntityId()];
        HierarchyLineOBSOLETEPtr hierarchyLine = createHierarchyElement(
            _transform3D,
            name.c_str(),
            Vec2F(10 + _x * 10, -6 - _y * 14));

        if (_transform3D->getChildren().empty())
        {
            hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::None);
        }
        else
        {
            if (hierarchyLineData.expanded)
                hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::Expanded);
            else
                hierarchyLine->setDropDownState(HierarchyLineOBSOLETEDropDownState::Collapsed);
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
    HierarchyLineOBSOLETEPtr HierarchyControllerOBSOLETE::createHierarchyElement(
        Transform2D* _transform,
        CString _name,
        Vec2F const& _position)
    {
        HierarchyLineOBSOLETEPtr hierarchyLine = createHierarchyLineOBSOLETE(_transform->getEntityId());       
        hierarchyLine->setLabel(_name);
        hierarchyLine->getTransform()->setParent(m_layoutTransform);
        hierarchyLine->getTransform()->setLocalPosition(_position);
        
        hierarchyLine->setSelected(
            SelectionManager::GetInstancePtr()->isObjectSelected(_transform->getEntityRaw()->getSharedPtr()));

        hierarchyLine->setActive(_transform->getEntityRaw()->getActiveInHierarchy());
        /*
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
        */

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLineOBSOLETEPtr HierarchyControllerOBSOLETE::createHierarchyElement(
        Transform3D* _transform,
        CString _name,
        Vec2F const& _position)
    {
        
        HierarchyLineOBSOLETEPtr hierarchyLine = createHierarchyLineOBSOLETE(_transform->getEntityId());
        
        hierarchyLine->setLabel(_name);
        hierarchyLine->getTransform()->setParent(m_layoutTransform);
        hierarchyLine->getTransform()->setLocalPosition(_position);

        hierarchyLine->setSelected(
            SelectionManager::GetInstancePtr()->isObjectSelected(_transform->getEntityRaw()->getSharedPtr()));

        hierarchyLine->setActive(_transform->getEntityRaw()->getActiveInHierarchy());
        /*
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
        */

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLineOBSOLETEPtr HierarchyControllerOBSOLETE::createHierarchyElement(
        Entity* _entity,
        CString _name,
        Vec2F const& _position)
    {

        HierarchyLineOBSOLETEPtr hierarchyLine = createHierarchyLineOBSOLETE(_entity->getId());

        hierarchyLine->setLabel(_name);
        hierarchyLine->getTransform()->setParent(m_layoutTransform);
        hierarchyLine->getTransform()->setLocalPosition(_position);

        hierarchyLine->setSelected(
            SelectionManager::GetInstancePtr()->isObjectSelected(_entity->getSharedPtr()));

        hierarchyLine->setActive(_entity->getActiveInHierarchy());
        /*
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
        */

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLineOBSOLETEPtr HierarchyControllerOBSOLETE::createHierarchyLineOBSOLETE(EntityId _entityId)
    {
        HierarchyLineOBSOLETEEntityDataOBSOLETE& hierarchyLineData = m_hierarchyLinesPerEntity[_entityId];
        HierarchyLineOBSOLETEPtr& hierarchyLine = hierarchyLineData.line;

        if (!hierarchyLine)
        {
            hierarchyLine = m_hierarchyLinePool->createHierarchyLineOBSOLETE(HierarchyLineOBSOLETEType::Entity);
            hierarchyLine->setEcsWorld(m_world);
            hierarchyLine->setUserData(reinterpret_cast<void*>((Size)(S32)_entityId));
            hierarchyLine->updateIcon();
            hierarchyLine->eventDropDownClick.subscribe(this, &HierarchyControllerOBSOLETE::notifyHierarchyLineOBSOLETEDropDownClick);
            hierarchyLine->eventLineClick.subscribe(this, &HierarchyControllerOBSOLETE::notifyHierarchyLineOBSOLETEClick);
        }

        hierarchyLine->getEntityRaw()->setActiveSelf(true);

        EntityPtr const& entity = m_world->getEntity(_entityId);
        if (entity)
        {
            hierarchyLine->setSelected(
                SelectionManager::GetInstancePtr()->isObjectSelected(entity->getSharedPtr()));

            hierarchyLine->setActive(entity->getActiveInHierarchy());
        }

        return hierarchyLine;
    }

    //////////////////////////////////////////
    HierarchyLineOBSOLETEPtr HierarchyControllerOBSOLETE::createHierarchyLineOBSOLETE(EcsScenePtr const& _scene)
    {
        HierarchyLineSceneDataOBSOLETE& hierarchyLineData = m_hierarchyLinesPerScene[_scene.get()];
        HierarchyLineOBSOLETEPtr& hierarchyLine = hierarchyLineData.line;

        if (!hierarchyLine)
        {
            hierarchyLine = m_hierarchyLinePool->createHierarchyLineOBSOLETE(HierarchyLineOBSOLETEType::Scene);
            hierarchyLine->setEcsWorld(m_world);
            hierarchyLine->setUserData(static_cast<void*>(_scene.get()));
            hierarchyLine->eventDropDownClick.subscribe(this, &HierarchyControllerOBSOLETE::notifyHierarchyLineOBSOLETEDropDownClick);
            hierarchyLine->eventLineClick.subscribe(this, &HierarchyControllerOBSOLETE::notifyHierarchyLineOBSOLETEClick);
            CString className = _scene->getClassName();
            hierarchyLine->setName(className);
        }

        hierarchyLine->getEntityRaw()->setActiveSelf(true);

        hierarchyLine->setSelected(false);
        hierarchyLine->setActive(true);

        return hierarchyLine;
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::notifyHierarchyLineOBSOLETEDropDownClick(HierarchyLineOBSOLETE* _hierarchyLine)
    {
        switch (_hierarchyLine->getType())
        {
            case HierarchyLineOBSOLETEType::Scene:
            {
                EcsScene* ecsScene = static_cast<EcsScene*>(_hierarchyLine->getUserData());
                HierarchyLineSceneDataOBSOLETE& hierarchyLineData = m_hierarchyLinesPerScene[ecsScene];
                hierarchyLineData.expanded = !hierarchyLineData.expanded;
                break;
            }
            case HierarchyLineOBSOLETEType::Entity:
            {
                EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(_hierarchyLine->getUserData()));
                HierarchyLineOBSOLETEEntityDataOBSOLETE& hierarchyLineData = m_hierarchyLinesPerEntity[entityId];
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
    void HierarchyControllerOBSOLETE::notifyHierarchyLineOBSOLETEClick(HierarchyLineOBSOLETE* _hierarchyLine)
    {
        switch (_hierarchyLine->getType())
        {
            case HierarchyLineOBSOLETEType::Entity:
            {
                EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(_hierarchyLine->getUserData()));

                EntityPtr const& entity = EntityManager::GetInstancePtr()->getDefaultWorldRaw()->getEntity(entityId);

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
    void HierarchyControllerOBSOLETE::notifyEntityRemoved(EntityPtr const& _entity)
    {
        removeHierarchyLineOBSOLETE(_entity);
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::notifyEntityChanged(EntityPtr const& _entity)
    {
        removeHierarchyLineOBSOLETE(_entity);
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::removeHierarchyLineOBSOLETE(EntityPtr const& _entity)
    {
        auto it = m_hierarchyLinesPerEntity.find(_entity->getId());
        if (it != m_hierarchyLinesPerEntity.end())
        {
            it->second.line->eventDropDownClick.unsubscribe(this);
            it->second.line->eventLineClick.unsubscribe(this);
            m_hierarchyLinePool->releaseHierarchyLineOBSOLETE(it->second.line);
            m_hierarchyLinesPerEntity.erase(it);
        }
    }

    //////////////////////////////////////////
    void HierarchyControllerOBSOLETE::setEcsWorld(EcsWorld* _world)
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
            m_world->eventEntityRemoved.subscribe(this, &HierarchyControllerOBSOLETE::notifyEntityRemoved);
            m_world->eventEntityChanged.subscribe(this, &HierarchyControllerOBSOLETE::notifyEntityChanged);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
