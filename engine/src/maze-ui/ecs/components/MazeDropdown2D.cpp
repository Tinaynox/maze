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
#include "maze-ui/ecs/components/MazeDropdown2D.hpp"
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
    // Class Dropdown2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Dropdown2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, systemTextRenderer, ComponentPtr(), getTextRendererComponent, setTextRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, listCanvas, ComponentPtr(), getListCanvasComponent, setListCanvas),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, itemPrefabTransform, ComponentPtr(), getItemPrefabTransformComponent, setItemPrefabTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, topIndent, 0.0f, getTopIndent, setTopIndent),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, bottomIndent, 0.0f, getBottomIndent, setBottomIndent),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, unselectedItemBackgroundDefaultColor, ColorU32(250, 250, 250), getUnselectedItemBackgroundDefaultColor, setUnselectedItemBackgroundDefaultColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, unselectedItemBackgroundFocusedColor, ColorU32(245, 245, 245), getUnselectedItemBackgroundFocusedColor, setUnselectedItemBackgroundFocusedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, unselectedItemBackgroundPressedColor, ColorU32(200, 200, 200), getUnselectedItemBackgroundPressedColor, setUnselectedItemBackgroundPressedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, selectedItemBackgroundDefaultColor, ColorU32(250, 250, 250), getSelectedItemBackgroundDefaultColor, setSelectedItemBackgroundDefaultColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, selectedItemBackgroundFocusedColor, ColorU32(245, 245, 245), getSelectedItemBackgroundFocusedColor, setSelectedItemBackgroundFocusedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, selectedItemBackgroundPressedColor, ColorU32(200, 200, 200), getSelectedItemBackgroundPressedColor, setSelectedItemBackgroundPressedColor));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Dropdown2D);

    //////////////////////////////////////////
    Dropdown2D::Dropdown2D()
        : m_selected(false)
        , m_value(0)
    {
    }

    //////////////////////////////////////////
    Dropdown2D::~Dropdown2D()
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
    Dropdown2DPtr Dropdown2D::Create()
    {
        Dropdown2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Dropdown2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Dropdown2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Dropdown2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void Dropdown2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &Dropdown2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &Dropdown2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &Dropdown2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &Dropdown2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &Dropdown2D::notifyClick);
    }

    //////////////////////////////////////////
    void Dropdown2D::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
            
    }

    //////////////////////////////////////////
    void Dropdown2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
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
    void Dropdown2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void Dropdown2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(!getSelected());
        }

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void Dropdown2D::notifyClick(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        eventClick(this, _inputEvent);
    }

    //////////////////////////////////////////
    void Dropdown2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void Dropdown2D::setValue(S32 _value)
    {
        if (m_value == _value)
            return;

        m_value = _value;

        updateCaption();
        rebuildOptions();
        updateSelectedOption();

        eventValueChanged(this, m_value);
    }

    //////////////////////////////////////////
    String Dropdown2D::getValueString()
    {
        if (m_value >= 0 && m_value < (S32)m_options.size())
            return m_options[m_value].text;
        else
            return String();
    }

    //////////////////////////////////////////
    void Dropdown2D::setValue(String const& _value)
    {
        S32 value = getOptionIndex(_value);
        setValue(value);
    }

    //////////////////////////////////////////
    S32 Dropdown2D::getOptionIndex(String const& _value)
    {
        for (S32 i = 0, in = (S32)m_options.size(); i < in; ++i)
        {
            if (m_options[i].text == _value)
                return i;
        }

        return -1;
    }
    
    //////////////////////////////////////////
    void Dropdown2D::updateTextRenderer()
    {
        if (!m_textRenderer)
            return;

        m_textRenderer->setText("None");
    
    }

    //////////////////////////////////////////
    void Dropdown2D::addOption(OptionData const& _optionData)
    {
        m_options.push_back(_optionData);

        updateCaption();
        rebuildOptions();
    }

    //////////////////////////////////////////
    void Dropdown2D::addOptions(Vector<OptionData> const& _options)
    {
        m_options.insert(
            m_options.end(),
            _options.begin(),
            _options.end());

        updateCaption();
        rebuildOptions();
    }

    //////////////////////////////////////////
    void Dropdown2D::addOptions(Vector<String> const& _options)
    {
        for (String const& option : _options)
        {
            m_options.push_back({ option });
        }

        updateCaption();
        rebuildOptions();
    }

    //////////////////////////////////////////
    void Dropdown2D::clearOptions()
    {
        m_options.clear();

        rebuildOptions();
    }

    //////////////////////////////////////////
    void Dropdown2D::rebuildOptions()
    {
        Transform2DPtr const& listTransform = m_listCanvas->getTransform();

        if (!m_itemPrefabTransform)
            return;

        listTransform->destroyAllChildren();

        F32 y = -m_topIndent;

        for (S32 i = 0; i < (S32)m_options.size(); ++i)
        {
            OptionData const& optionData = m_options[i];

            String optionText = optionData.text;

            Entity* itemPrefabEntity = m_itemPrefabTransform->getEntityRaw();
            EntityPtr itemEntity = itemPrefabEntity->createCopy();
            itemEntity->setActiveSelf(true);

            Transform2D* itemTransform = itemEntity->getComponentRaw<Transform2D>();

            itemTransform->setParent(listTransform);
            itemTransform->setLocalPosition(m_itemPrefabTransform->getLocalPosition().x, y);

            AbstractTextRenderer2D* labelTextRenderer = itemTransform->findChildComponentInheritedFrom<AbstractTextRenderer2D>("Label");
            if (labelTextRenderer)
            {
                labelTextRenderer->setText(optionData.text);
            }

            SpriteRenderer2D* backgroundSpriteRenderer = itemTransform->findChildComponent<SpriteRenderer2D>("Background");            

            static auto updateClickButtonState =
                [](Dropdown2D* _dropdown,
                   Button2D* _button,
                   SpriteRenderer2D* _backgroundRenderer)
            {
                S32 buttonIndex = _dropdown->getButtonIndex(_button->castRaw<ClickButton2D>());
                bool isSelected = (buttonIndex == _dropdown->getValue());

                if (isSelected)
                {
                    if (_button->getPressed())
                    {
                        _backgroundRenderer->setColor(_dropdown->getSelectedItemBackgroundPressedColor());
                    }
                    else
                    {
                        if (_button->getFocused())
                        {
                            _backgroundRenderer->setColor(_dropdown->getSelectedItemBackgroundFocusedColor());
                        }
                        else
                        {
                            _backgroundRenderer->setColor(_dropdown->getSelectedItemBackgroundDefaultColor());
                        }
                    }
                }
                else
                {
                    if (_button->getPressed())
                    {
                        _backgroundRenderer->setColor(_dropdown->getUnselectedItemBackgroundPressedColor());
                    }
                    else
                    {
                        if (_button->getFocused())
                        {
                            _backgroundRenderer->setColor(_dropdown->getUnselectedItemBackgroundFocusedColor());
                        }
                        else
                        {
                            _backgroundRenderer->setColor(_dropdown->getUnselectedItemBackgroundDefaultColor());
                        }
                    }
                }

            };

            ClickButton2D* button = itemEntity->getComponentRaw<ClickButton2D>();

            button->eventFocusChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateClickButtonState(
                        this,
                        _button,
                        backgroundSpriteRenderer);
                });

            button->eventPressedChanged.subscribe(
                [=](Button2D* _button, bool _value)
                {
                    updateClickButtonState(
                        this,
                        _button,
                        backgroundSpriteRenderer);
                });

            Dropdown2D* dropdown = this;

            button->eventClick.subscribe(
                [dropdown, i](Button2D* _button, CursorInputEvent const& _inputEvent)
                {
                    dropdown->setValue(i);
                    dropdown->setSelected(false);
                });

            updateClickButtonState(
                this,
                button,
                backgroundSpriteRenderer);

            y -= m_itemPrefabTransform->getHeight();
        }

        F32 listHeight = m_options.size() * m_itemPrefabTransform->getHeight();
        listHeight += m_topIndent;
        listHeight += m_bottomIndent;
        listTransform->setHeight(listHeight);

        updateSelectedOption();
    }

    //////////////////////////////////////////
    void Dropdown2D::updateCaption()
    {
        if (!m_textRenderer)
            return;

        m_textRenderer->setText(getValueString());
    }

    //////////////////////////////////////////
    void Dropdown2D::updateSelectedOption()
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

    //////////////////////////////////////////
    S32 Dropdown2D::getButtonIndex(ClickButton2D* _button) const
    {
        Transform2DPtr const& listTransform = m_listCanvas->getTransform();

        S32 index = 0;
        for (S32 i = 0, in = (S32)listTransform->getChildren().size(); i < in; ++i)
        {
            ClickButton2D* button = listTransform->getChild(i)->getEntityRaw()->getComponentRaw<ClickButton2D>();
            if (button)
            {
                if (_button == button)
                    return index;

                ++index;
            }
        }

        return -1;
    }

} // namespace Maze
//////////////////////////////////////////
