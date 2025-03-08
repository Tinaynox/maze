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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeMenuBar2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MenuBar2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MenuBar2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, layoutTransform, ComponentPtr(), getLayoutTransformComponent, setLayoutTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, menuButtonPrefab, ComponentPtr(), getMenuButtonPrefabComponent, setMenuButtonPrefab),
    );

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MenuBar2D);

    //////////////////////////////////////////
    MenuBar2D::MenuBar2D()
    {
    }

    //////////////////////////////////////////
    MenuBar2D::~MenuBar2D()
    {
        if (m_contextMenuCanvas)
        {
            m_contextMenuCanvas->eventContextMenuClosed.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    MenuBar2DPtr MenuBar2D::Create(FontMaterialPtr const& _fontMaterial)
    {
        MenuBar2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MenuBar2D, object, init(_fontMaterial));
        return object;
    }

    //////////////////////////////////////////
    bool MenuBar2D::init(FontMaterialPtr const& _fontMaterial)
    {
        m_fontMaterial = _fontMaterial;
        return true;
    }

    //////////////////////////////////////////
    void MenuBar2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void MenuBar2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_contextMenuCanvas = ContextMenuCanvas2D::EnsureContextMenuCanvas(
            getEntityRaw()->getEcsScene(),
            m_fontMaterial);
        if (m_contextMenuCanvas)
        {
            m_contextMenuCanvas->eventContextMenuClosed.subscribe(this, &MenuBar2D::notifyContextMenuClosed);
        }
        else
        {
            MAZE_LOG_ERROR("Context Menu Canvas is not available!");
        }
    }

    //////////////////////////////////////////
    MenuBar2D::OptionData* MenuBar2D::ensureMenu(String const& _menuName)
    {
        auto it = m_options.find(_menuName);
        if (it != m_options.end())
            return &it->second;

        if (!m_layoutTransform)
            return nullptr;

        if (!m_menuButtonPrefab)
            return nullptr;

        OptionData optionData;

        EntityPtr menuButtonObject = m_menuButtonPrefab->getEntityRaw()->createCopy();
        menuButtonObject->setActiveSelf(true);
        optionData.button = menuButtonObject->getComponent<ToggleButton2D>();

        optionData.button->eventClick.subscribe(
            [_menuName, this](Button2D* _button, CursorInputEvent const& _inputEvent)
            {
                if ((m_currentMenu == _menuName))
                {
                    closeMenu();
                }
                else
                {
                    openMenu(_menuName);
                }
            });

        optionData.button->eventFocusChanged.subscribe(
            [_menuName, this](Button2D* _button, bool _focused)
            {
                if (_focused)
                {
                    if (!m_currentMenu.empty() && m_currentMenu != _menuName)
                        openMenu(_menuName);
                }
            });

        AbstractTextRenderer2D* text = optionData.button->getTransform()->findChildComponentInheritedFromRecursive<AbstractTextRenderer2D>("Label");
        text->setText(_menuName);
        F32 buttonWidth = (F32)_menuName.size() * 8 + 16;
        optionData.button->getTransform()->setWidth(buttonWidth);
        optionData.button->getTransform()->setParent(m_layoutTransform);

        auto it2 = m_options.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_menuName),
            std::forward_as_tuple(optionData));
        
        return &it2.first->second;
    }

    //////////////////////////////////////////
    void MenuBar2D::openMenu(String const& _menuName)
    {
        auto optionsIt = m_options.find(_menuName);
        if (optionsIt == m_options.end())
            return;

        ToggleButton2DPtr const& button = optionsIt->second.button;        

        m_currentMenu = _menuName;

        ContextMenuCanvas2DPtr contextMenuCanvas = m_contextMenuCanvas;

        if (!m_currentMenu.empty())
        {
            Canvas* canvas = m_transform->getFirstTrunkComponent<Canvas>();
            Vec2F positionRTS = canvas->convertViewportCoordsToRenderTargetCoords(button->getTransform()->getWorldPosition());

            contextMenuCanvas->openContextMenu(
                this,
                positionRTS,
                [this, _menuName](MenuListTree2DPtr const& _menuListTree)
                {
                    auto optionsIt = m_options.find(_menuName);
                    if (optionsIt == m_options.end())
                        return;

                    _menuListTree->setCloseMenuByPressOutCallback(
                        [this, _menuName](CursorInputEvent const& _inputEvent) -> bool
                        {
                            auto optionsIt = m_options.find(_menuName);
                            if (optionsIt == m_options.end())
                                return true;

                            Vec2F const& positionWS = _inputEvent.position;
                            Vec2F positionRTS = _inputEvent.rootCanvas->convertViewportCoordsToRenderTargetCoords(positionWS);

                            Canvas* buttonRootCanvas = optionsIt->second.button->getTransform()->getLastTrunkComponent<Canvas>();
                            if (buttonRootCanvas)
                            {
                                AABB2D buttonAABB = optionsIt->second.button->getTransform()->calculateWorldAABB();
                                buttonAABB.set(
                                    buttonRootCanvas->convertViewportCoordsToRenderTargetCoords(buttonAABB.getLeftBottom()),
                                    buttonRootCanvas->convertViewportCoordsToRenderTargetCoords(buttonAABB.getRightTop()));

                                if (buttonAABB.contains(positionRTS))
                                    return false;
                            }

                            return true;
                        });

                    for (auto callback : optionsIt->second.callbacks)
                    {
                        _menuListTree->addItem(callback.text, callback.callback, callback.validate);
                    }
                });
        }
        else
        {
            contextMenuCanvas->closeContextMenu();
        }

    }

    //////////////////////////////////////////
    void MenuBar2D::addOption(
        String const& _menuName,
        String const& _option,
        MenuListTree2D::ItemCallback _callback,
        MenuListTree2D::ItemValidateCallback _validate,
        HashedString const& _tag)
    {
        OptionData* optionData = ensureMenu(_menuName);
        if (!optionData)
            return;

        optionData->callbacks.push_back({ _option, _callback, _validate, _tag });
    }

    //////////////////////////////////////////
    void MenuBar2D::removeOptionsWithTag(HashedString const& _tag)
    {
        for (auto& optionData : m_options)
        {
            auto& callbacks = optionData.second.callbacks;
            for (auto it = callbacks.begin(), end = callbacks.end(); it != end; )
            {
                if (it->tag == _tag)
                {
                    it = callbacks.erase(it);
                    end = callbacks.end();
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    //////////////////////////////////////////
    void MenuBar2D::closeMenu()
    {
        if (m_currentMenu.empty())
            return;

        m_currentMenu.clear();

        if (m_contextMenuCanvas->getContextMenuHolder() == this)
            m_contextMenuCanvas->closeContextMenu();
    }

    //////////////////////////////////////////
    void MenuBar2D::notifyContextMenuClosed(ContextMenuCanvas2D* _menuCanvas, MenuListTree2DPtr const& _tree)
    {
        if (_menuCanvas->getContextMenuHolder() != this)
            return;

        m_currentMenu.clear();
    }

} // namespace Maze
//////////////////////////////////////////
