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
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeToggleButton2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemTextDropdown2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SystemTextDropdown2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, systemTextRenderer, ComponentPtr(), getSystemTextRendererComponent, setSystemTextRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, listCanvas, ComponentPtr(), getListCanvasComponent, setListCanvas),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, itemPrefabTransform, ComponentPtr(), getItemPrefabTransformComponent, setItemPrefabTransform));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SystemTextDropdown2D);

    //////////////////////////////////////////
    SystemTextDropdown2D::SystemTextDropdown2D()
        : m_selected(false)
        , m_value(0)
    {
    }

    //////////////////////////////////////////
    SystemTextDropdown2D::~SystemTextDropdown2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
            m_UIElement2D->eventCursorReleaseOut.unsubscribe(this);
            m_UIElement2D->eventFocusChanged.unsubscribe(this);
            m_UIElement2D->eventPressedChanged.unsubscribe(this);
            m_UIElement2D->eventClick.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    SystemTextDropdown2DPtr SystemTextDropdown2D::Create()
    {
        SystemTextDropdown2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SystemTextDropdown2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SystemTextDropdown2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &SystemTextDropdown2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &SystemTextDropdown2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &SystemTextDropdown2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &SystemTextDropdown2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &SystemTextDropdown2D::notifyClick);
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
            
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        if (m_listCanvas)
        {
            UIElement2D* uiElement = m_listCanvas->getEntityRaw()->getComponentRaw<UIElement2D>();
            if (uiElement && !uiElement->getFocused())
                setSelected(false);
        }
        else
        {
            setSelected(false);
        }
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(!getSelected());
        }

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::notifyClick(CursorInputEvent const& _inputEvent)
    {
        eventClick(this, _inputEvent);
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::setValue(S32 _value)
    {
        if (m_value == _value)
            return;

        m_value = _value;

        updateCaption();
        updateSelectedOption();

        eventValueChanged(this, m_value);
    }

    //////////////////////////////////////////
    String SystemTextDropdown2D::getValueString()
    {
        if (m_value >= 0 && m_value < (S32)m_options.size())
            return m_options[m_value].text;
        else
            return String();
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::setValue(String const& _value)
    {
        S32 value = getOptionIndex(_value);
        setValue(value);
    }

    //////////////////////////////////////////
    S32 SystemTextDropdown2D::getOptionIndex(String const& _value)
    {
        for (S32 i = 0, in = (S32)m_options.size(); i < in; ++i)
        {
            if (m_options[i].text == _value)
                return i;
        }

        return -1;
    }
    
    //////////////////////////////////////////
    void SystemTextDropdown2D::updateSystemTextRenderer()
    {
        if (!m_systemTextRenderer)
            return;

        m_systemTextRenderer->setText("None");
    
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::addOption(OptionData const& _optionData)
    {
        m_options.push_back(_optionData);

        updateCaption();
        rebuildOptions();
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::addOptions(Vector<OptionData> const& _options)
    {
        m_options.insert(
            m_options.end(),
            _options.begin(),
            _options.end());

        updateCaption();
        rebuildOptions();
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::addOptions(Vector<String> const& _options)
    {
        for (String const& option : _options)
        {
            m_options.push_back({ option });
        }

        updateCaption();
        rebuildOptions();
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::clearOptions()
    {
        m_options.clear();

        rebuildOptions();
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::rebuildOptions()
    {
        Transform2DPtr const& listTransform = m_listCanvas->getTransform();

        if (!m_itemPrefabTransform)
            return;

        listTransform->destroyAllChildren();

        F32 y = 0;

        for (OptionData const& optionData : m_options)
        {
            String optionText = optionData.text;

            Entity* itemPrefabEntity = m_itemPrefabTransform->getEntityRaw();
            EntityPtr itemEntity = itemPrefabEntity->createCopy();
            itemEntity->setActiveSelf(true);

            Transform2D* itemTransform = itemEntity->getComponentRaw<Transform2D>();

            itemTransform->setParent(listTransform);
            itemTransform->setLocalPosition(m_itemPrefabTransform->getLocalPosition().x, y);

            SystemTextRenderer2D* labelTextRenderer = itemTransform->findChildComponent<SystemTextRenderer2D>("Label");
            if (labelTextRenderer)
            {
                labelTextRenderer->setText(optionData.text);
            }

            SpriteRenderer2D* backgroundSpriteRenderer = itemTransform->findChildComponent<SpriteRenderer2D>("Background");            

            static auto updateClickButtonState =
                [](
                    Button2D* _button,
                    SpriteRenderer2D* _backgroundRenderer)
            {
                ToggleButton2D* toggleButton = _button->castRaw<ToggleButton2D>();

                if (toggleButton->getPressed())
                {
                    _backgroundRenderer->setColor(200, 200, 200);
                }
                else
                {
                    if (toggleButton->getFocused())
                    {
                        _backgroundRenderer->setColor(245, 245, 245);
                    }
                    else
                    {
                        _backgroundRenderer->setColor(250, 250, 250);
                    }
                }

            };

            ClickButton2D* button = itemEntity->getComponentRaw<ClickButton2D>();

            button->eventFocusChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateClickButtonState(
                        _button,
                        backgroundSpriteRenderer);
                });

            button->eventPressedChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateClickButtonState(
                        _button,
                        backgroundSpriteRenderer);
                });

            SystemTextDropdown2D* dropdown = this;

            button->eventClick.subscribe(
                [dropdown, optionText](Button2D* _button, CursorInputEvent const& _inputEvent)
                {
                    dropdown->setValue(optionText);
                    dropdown->setSelected(false);
                });

            updateClickButtonState(
                button,
                backgroundSpriteRenderer);

            y -= m_itemPrefabTransform->getHeight();
        }

        listTransform->setHeight(m_options.size() * m_itemPrefabTransform->getHeight());

        updateSelectedOption();
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::updateCaption()
    {
        if (!m_systemTextRenderer)
            return;

        m_systemTextRenderer->setText(getValueString());
    }

    //////////////////////////////////////////
    void SystemTextDropdown2D::updateSelectedOption()
    {
        if (!m_listCanvas)
            return;

        Transform2DPtr const& listTransform = m_listCanvas->getTransform();

        for (S32 i = 0, in = (S32)m_options.size(); i < in; ++i)
        {
            Transform2D* itemTransform = listTransform->getChild((Size)i);
            if (!itemTransform)
                continue;

            SpriteRenderer2D* checkMarkSpriteRenderer = itemTransform->findChildComponent<SpriteRenderer2D>("CheckMark");

            if (checkMarkSpriteRenderer)
            {
                checkMarkSpriteRenderer->getMeshRenderer()->setEnabled(i == m_value);
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
