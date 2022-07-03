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
#include "maze-ui/ecs/components/MazeEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditBox2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(EditBox2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, systemTextRenderer, ComponentPtr(), getTextRendererComponent, setTextRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, text, String(), getText, setText));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(EditBox2D);

    //////////////////////////////////////////
    EditBox2D::EditBox2D()
        : m_selected(false)
        , m_cursorBlinkTimer(0.0f)
        , m_cursorBlinkTime(0.5f)
        , m_cursorBlink(false)
    {
    }

    //////////////////////////////////////////
    EditBox2D::~EditBox2D()
    {
        InputManager::GetInstancePtr()->eventText.unsubscribe(this);
        InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);

        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressOut.unsubscribe(this);
            m_UIElement2D->eventCursorReleaseOut.unsubscribe(this);
            m_UIElement2D->eventFocusChanged.unsubscribe(this);
            m_UIElement2D->eventPressedChanged.unsubscribe(this);
            m_UIElement2D->eventClick.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    EditBox2DPtr EditBox2D::Create()
    {
        EditBox2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditBox2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool EditBox2D::init()
    {
        InputManager::GetInstancePtr()->eventText.subscribe(this, &EditBox2D::notifyText);
        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &EditBox2D::notifyKeyboard);

        return true;
    }

    //////////////////////////////////////////
    void EditBox2D::update(F32 _dt)
    {
        if (isTextInputAvailable())
        {
            m_cursorBlinkTimer += Math::Min(_dt, 1.0f/20.0f);
            if (m_cursorBlinkTimer >= m_cursorBlinkTime)
            {
                m_cursorBlinkTimer -= m_cursorBlinkTime;
                m_cursorBlink = !m_cursorBlink;
                updateCursorRendererEnabled();
            }
        }
    }

    //////////////////////////////////////////
    void EditBox2D::notifyText(InputEventTextData const& _event)
    {
        if (!isTextInputAvailable())
            return;

        switch (_event.type)
        {
            case InputEventTextType::Input:
            {
                if (_event.textUtf8[1] == 0)
                {
                    S8 c = _event.textUtf8[0];
                    if (c >= 32 && c < 127)
                        addChar(c);
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void EditBox2D::notifyKeyboard(InputEventKeyboardData const& _event)
    {
        if (!isTextInputAvailable())
            return;

        switch (_event.type)
        {
            case InputEventKeyboardType::KeyDown:
            {
                switch (_event.keyCode)
                {
                    case KeyCode::Backspace:
                    {
                        popChar();
                        break;
                    }
                    case KeyCode::Delete:
                    {
                        break;
                    }
                    case KeyCode::Enter:
                    {
                        eventTextInput(this);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                break;
            }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void EditBox2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);
        
        m_UIElement2D->eventCursorPressOut.subscribe(this, &EditBox2D::notifyCursorPressOut);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &EditBox2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &EditBox2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &EditBox2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &EditBox2D::notifyClick);
    }

    //////////////////////////////////////////
    void EditBox2D::notifyCursorPressOut(CursorInputEvent const& _inputEvent)
    {
        setSelected(false);
    }

    //////////////////////////////////////////
    void EditBox2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        setSelected(false);
    }

    //////////////////////////////////////////
    void EditBox2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void EditBox2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(true);
        }
        else
        {
            resetBlinkTimer();
        }

        updateCursorRendererEnabled();

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void EditBox2D::notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        eventClick(this, _inputEvent);
    }

    //////////////////////////////////////////
    void EditBox2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        if (m_selected)
        {
            resetBlinkTimer();
        }

        updateCursorRendererEnabled();

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void EditBox2D::setText(String const& _text)
    {
        if (m_text == _text)
            return;

        m_text = _text;

        processTextChanged();
    }
    
    //////////////////////////////////////////
    void EditBox2D::updateTextRenderer()
    {
        if (!m_textRenderer)
            return;

        m_textRenderer->setText(m_text);
    
    }

    //////////////////////////////////////////
    void EditBox2D::updateCursorRendererPosition()
    {
        if (!m_cursorRenderer)
            return;

        if (!m_textRenderer)
            return;

        Vec2DF textShift = m_textRenderer->getTextEnd();
        m_cursorRenderer->getTransform()->setLocalPosition(textShift);
    }

    //////////////////////////////////////////
    void EditBox2D::updateCursorRendererEnabled()
    {
        if (!m_cursorRenderer)
            return;

        bool cursorEnabled = isTextInputAvailable() && m_cursorBlink;
        m_cursorRenderer->getMeshRenderer()->setEnabled(cursorEnabled);
    }

    //////////////////////////////////////////
    void EditBox2D::processTextChanged()
    {
        resetBlinkTimer();
        updateTextRenderer();
        updateCursorRendererPosition();

        eventTextChanged(this, m_text);
    }

    //////////////////////////////////////////
    void EditBox2D::setHorizontalAlignment(HorizontalAlignment2D _alignment)
    {
        m_textRenderer->setHorizontalAlignment(_alignment);
        
        resetBlinkTimer();
        updateCursorRendererPosition();
    }

    //////////////////////////////////////////
    void EditBox2D::setVerticalAlignment(VerticalAlignment2D _alignment)
    {
        m_textRenderer->setVerticalAlignment(_alignment);
        
        resetBlinkTimer();
        updateCursorRendererPosition();
    }

} // namespace Maze
//////////////////////////////////////////
