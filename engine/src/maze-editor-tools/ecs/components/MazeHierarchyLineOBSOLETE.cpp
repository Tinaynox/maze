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
#include "maze-editor-tools/ecs/components/MazeHierarchyLineOBSOLETE.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
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
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/helpers/MazeSystemUIHelper.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class HierarchyLineOBSOLETE
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(HierarchyLineOBSOLETE, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(HierarchyLineOBSOLETE);

    //////////////////////////////////////////
    HierarchyLineOBSOLETE::HierarchyLineOBSOLETE()
        : m_type(HierarchyLineOBSOLETEType::Entity)
        , m_dropDownState(HierarchyLineOBSOLETEDropDownState::None)
    {
    }

    //////////////////////////////////////////
    HierarchyLineOBSOLETE::~HierarchyLineOBSOLETE()
    {
        if (m_dropDownRenderer && m_dropDownRenderer->getEntityRaw())
        {
            ClickButton2D* button = m_dropDownRenderer->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
            {
                button->eventClick.unsubscribe(this);
                button->eventCursorPressIn.unsubscribe(this);
            }
        }
        
        if (m_textRenderer && m_textRenderer->getEntityRaw())
        {
            ClickButton2D* button = m_textRenderer->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
            {
                button->eventClick.unsubscribe(this);
                button->eventCursorPressIn.unsubscribe(this);
                button->eventDoubleClick.unsubscribe(this);
                button->eventFocusChanged.unsubscribe(this);
            }
        }
    }

    //////////////////////////////////////////
    HierarchyLineOBSOLETEPtr HierarchyLineOBSOLETE::Create(HierarchyLineOBSOLETEType _type)
    {
        HierarchyLineOBSOLETEPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(HierarchyLineOBSOLETE, object, init(_type));
        return object;
    }

    //////////////////////////////////////////
    bool HierarchyLineOBSOLETE::init(HierarchyLineOBSOLETEType _type)
    {
        m_type = _type;

        return true;
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::setEcsWorld(EcsWorld* _world)
    {
        m_world = _world;

        updateIcon();
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::processEntityAwakened()
    {
        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        MaterialManagerPtr const& materialManager = renderSystem->getMaterialManager();

        S32 const charSize = 8;

        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_transform->setSize(Vec2F(100, 14.0f));
        m_transform->setAnchor(Vec2F(0.0f, 1.0f));
        m_transform->setPivot(Vec2F(0.0f, 1.0f));

        SizePolicy2DPtr sizePolicy = getEntityRaw()->ensureComponent<SizePolicy2D>();
        sizePolicy->setFlag(SizePolicy2D::Flags::Height, false);

        m_backgroundRenderer = SpriteHelper::CreateSprite(
            ColorU32::c_lightGray,
            m_transform->getSize(),
            Vec2F::c_zero,
            MaterialPtr(),
            m_transform,
            getEntityRaw()->getEcsScene());
        m_backgroundRenderer->getEntityRaw()->ensureComponent<SizePolicy2D>();

        F32 x = 0;

        m_dropDownRenderer = SpriteHelper::CreateSprite(
            UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded),
            Vec2F(charSize, charSize) * 1.5f,
            Vec2F(x, 0) + Vec2F(charSize * 0.5f, 0.0f),
            materialManager->getSpriteMaterial(),
            m_transform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.5f, 0.5f));
        m_dropDownRenderer->setColor(ColorU32::c_black);
        ClickButton2DPtr dropDownButton = m_dropDownRenderer->getEntityRaw()->ensureComponent<ClickButton2D>();
        dropDownButton->eventClick.subscribe(this, &HierarchyLineOBSOLETE::notifyDropDownClick);
        m_dropDownRenderer->getEntityRaw()->setActiveSelf(false);
        x += (F32)charSize + 4;

        if (m_type == HierarchyLineOBSOLETEType::Entity)
        {
            m_iconRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::EntityObject3D),
                Vec2F(charSize, charSize) * 1.5f,
                Vec2F(x, 0) + Vec2F(charSize, 0.0f) * 0.5f,
                materialManager->getSpriteMaterial(),
                m_transform,
                getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.5f, 0.5f));
            m_iconRenderer->setColor(ColorU32::c_black);
            x += (F32)charSize + 4;
        }
        else
        if (m_type == HierarchyLineOBSOLETEType::Scene)
        {
            m_iconRenderer = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Scene),
                Vec2F(charSize, charSize) * 1.5f,
                Vec2F(x, 0) + Vec2F(charSize, 0.0f) * 0.5f,
                materialManager->getSpriteMaterial(),
                m_transform,
                getEntityRaw()->getEcsScene(),
                Vec2F(0.0f, 0.5f),
                Vec2F(0.5f, 0.5f));
            m_iconRenderer->setColor(ColorU32::c_black);
            x += (F32)charSize + 4;
        }

        updateIcon();

        m_textRenderer = EditorToolsUIHelper::CreateText(
            "XXX",
            EditorToolsStyles::GetInstancePtr()->getDefaultFontMaterial(),
            12,
            HorizontalAlignment2D::Left,
            VerticalAlignment2D::Middle,
            Vec2F(100, charSize + 6),
            Vec2F(x, 0),            
            m_transform,
            getEntityRaw()->getEcsScene(),
            Vec2F(0.0f, 0.5f),
            Vec2F(0.0f, 0.5f));
        m_textRenderer->setColor(ColorU32::c_black);
        m_textButton = m_textRenderer->getEntityRaw()->ensureComponent<ClickButton2D>();
        m_textButton->eventClick.subscribe(this, &HierarchyLineOBSOLETE::notifyLineClick);
        m_textButton->eventCursorPressIn.subscribe(this, &HierarchyLineOBSOLETE::notifyLineCursorPressIn);
        m_textButton->eventDoubleClick.subscribe(this, &HierarchyLineOBSOLETE::notifyLineDoubleClick);
        m_textButton->eventFocusChanged.subscribe(this, &HierarchyLineOBSOLETE::notifyLineFocusChanged);

        SizePolicy2DPtr mainLayoutSizePolicy = m_textButton->getEntityRaw()->ensureComponent<SizePolicy2D>();
        mainLayoutSizePolicy->setFlag(SizePolicy2D::Flags::Height, false);
        mainLayoutSizePolicy->setSizeDelta(-10.0f, 0.0f);

        m_contextMenu = m_textRenderer->getEntityRaw()->ensureComponent<ContextMenu2D>();
        m_contextMenu->setCallbackFunction(
            [this](MenuListTree2DPtr const& _menuListTree)
            {
                if (m_type == HierarchyLineOBSOLETEType::Entity)
                {
                    if (!m_world)
                        return;

                    EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(getUserData()));

                    EntityPtr const& entity = m_world->getEntity(entityId);

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
                if (m_type == HierarchyLineOBSOLETEType::Scene)
                {
                    EcsScene* ecsScene = static_cast<EcsScene*>(getUserData());

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

        updateState();
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::notifyDropDownClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        eventDropDownClick(this);
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::notifyLineClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        eventLineClick(this);
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::notifyLineCursorPressIn(Button2D* _button, Vec2F const& _pos, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        eventLineCursorPressIn(this);
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::notifyLineDoubleClick(Button2D* _button, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        eventLineDoubleClick(this);
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::notifyLineFocusChanged(Button2D* _button, bool _value)
    {
        updateState();
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::setDropDownState(HierarchyLineOBSOLETEDropDownState _value)
    {
        if (!m_dropDownRenderer)
            return;

        if (m_dropDownState == _value)
            return;

        m_dropDownState = _value;    

        if (m_dropDownState == HierarchyLineOBSOLETEDropDownState::None)
            m_dropDownRenderer->getEntityRaw()->setActiveSelf(false);
        else
            m_dropDownRenderer->getEntityRaw()->setActiveSelf(true);

        if (m_dropDownState == HierarchyLineOBSOLETEDropDownState::Collapsed)
            m_dropDownRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonCollapsed));
        else
        if (m_dropDownState == HierarchyLineOBSOLETEDropDownState::Expanded)
            m_dropDownRenderer->setSprite(UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::DropDownButtonExpanded));
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::setLabel(String const& _label)
    {
        m_textRenderer->setText(_label);
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::setColor(ColorU32 const& _color)
    {
        m_textRenderer->setColor(_color);

        if (m_iconRenderer)
            m_iconRenderer->setColor(_color);
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::setUserData(void* _userData)
    {
        if (m_userData == _userData)
            return;

        m_userData = _userData;

        updateIcon();

        eventUserDataChanged(m_userData);
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::updateIcon()
    {
        if (m_type == HierarchyLineOBSOLETEType::Entity)
        {
            if (!m_world)
                return;

            EntityId entityId = (EntityId)((S32)reinterpret_cast<Size>(getUserData()));

            EntityPtr const& entity = m_world->getEntity(entityId);
            if (!entity)
                return;

            SpritePtr sprite;

            if (entity)
            {
                if (entity->getComponent<Transform3D>())
                    sprite = UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::EntityObject3D);
                else
                if (entity->getComponent<Transform2D>())
                    sprite = UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::EntityObject2D);
                else
                    sprite = UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::EntityObject);
            }

            m_iconRenderer->setSprite(sprite);
        }
        else
        if (m_type == HierarchyLineOBSOLETEType::Scene)
        {
            EcsScene* ecsScene = static_cast<EcsScene*>(getUserData());

            EcsScene* mainScene = SceneManager::GetInstancePtr()->getMainScene().get();
            bool isMainScene = (mainScene == ecsScene);

            SpritePtr const&  sprite = UIManager::GetInstancePtr()->getDefaultUISprite(isMainScene ? DefaultUISprite::MainScene : DefaultUISprite::Scene);
            m_iconRenderer->setSprite(sprite);
        }
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::setSelected(bool _value)
    {
        if (m_selected == _value)
            return;

        m_selected = _value;

        updateState();
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::setActive(bool _value)
    {
        if (m_active == _value)
            return;

        m_active = _value;

        updateState();
    }

    //////////////////////////////////////////
    void HierarchyLineOBSOLETE::updateState()
    {
        if (!m_textRenderer)
            return;

        if (m_selected)
        {
            if (m_active)
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorSelected());
            else
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorInactive());

            m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjecBackgroundColorSelected());
        }
        else
        {
            if (m_active)
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorDefault());
            else
                m_textRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectTextColorInactive());

            if (m_textButton->getUIElement()->getFocused())
                m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectBackgroundColorFocused());
            else
                m_backgroundRenderer->setColor(EditorToolsStyles::GetInstancePtr()->getListObjectBackgroundColorDefault());
        }

        if (m_iconRenderer)
            m_iconRenderer->setColor(m_textRenderer->getColor());
    }
    
} // namespace Maze
//////////////////////////////////////////
