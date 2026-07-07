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
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-ui/ecs/events/MazeEcsUIEvents.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"


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
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, cursorRenderer, ComponentPtr(), getCursorRendererComponent, setCursorRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, text, String(), getText, setText),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U8, editBoxFlags, 0, getEditBoxFlags, setEditBoxFlags),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(EntityId, eventReceiverEid, c_invalidEntityId, getEventReceiverEid, setEventReceiverEid));

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
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
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
                    {
                        addChar(c);
                    }
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
                        deleteSymbolBeforeCursor();
                        break;
                    }
                    case KeyCode::Delete:
                    {
                        deleteSymbolAtCursor();
                        break;
                    }
                    case KeyCode::Left:
                    {
                        moveCursor(-1, _event.isControlDown());
                        break;
                    }
                    case KeyCode::Right:
                    {
                        moveCursor(1, _event.isControlDown());
                        break;
                    }
                    case KeyCode::Home:
                    {
                        setCursorPosition(0);
                        break;
                    }
                    case KeyCode::End:
                    {
                        setCursorPosition(m_text.size());
                        break;
                    }
                    case KeyCode::C:
                    {
                        if (_event.isControlDown() && SystemManager::GetInstancePtr())
                            SystemManager::GetInstancePtr()->setClipboardString(m_text);
                        break;
                    }
                    case KeyCode::X:
                    {
                        if (_event.isControlDown() && SystemManager::GetInstancePtr())
                        {
                            SystemManager::GetInstancePtr()->setClipboardString(m_text);
                            setText(String());
                        }
                        break;
                    }
                    case KeyCode::V:
                    {
                        if (_event.isControlDown() && SystemManager::GetInstancePtr())
                            insertTextAtCursor(SystemManager::GetInstancePtr()->getClipboardAsString());
                        break;
                    }
                    case KeyCode::Enter:
                    {
                        if (!(m_editBoxFlags & U8(EditBoxFlags::DontDeselectOnTextInput)))
                            setSelected(false);

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
        
        m_UIElement2D->eventCursorPressIn.subscribe(this, &EditBox2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorPressOut.subscribe(this, &EditBox2D::notifyCursorPressOut);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &EditBox2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &EditBox2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &EditBox2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &EditBox2D::notifyClick);

        // Subscribed here rather than in init() - init() only runs when the component is
        // constructed via EditBox2D::Create(), but not when it's cloned as part of an entity
        // copy (Entity::createCopy(), used eg. by prefab-based popups), which would otherwise
        // silently produce an edit box that never receives text/keyboard input.
        InputManager::GetInstancePtr()->eventText.subscribe(this, &EditBox2D::notifyText);
        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &EditBox2D::notifyKeyboard);
    }

    //////////////////////////////////////////
    void EditBox2D::processEvent(Event* _event)
    {
        if (_event->getEventUID() == ClassInfo<EntityPostCopyEvent>::UID())
        {
            // Remap the event receiver to the copied entity
            EntityPostCopyEvent* postCopyEvent = _event->castRaw<EntityPostCopyEvent>();
            if (m_eventReceiverEid != c_invalidEntityId)
            {
                for (auto const& entityData : postCopyEvent->copyData.getEntities())
                {
                    if (entityData.first->getId() == m_eventReceiverEid)
                    {
                        m_eventReceiverEid = entityData.second->getId();
                        break;
                    }
                }
            }
        }
    }

    //////////////////////////////////////////
    void EditBox2D::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent& _inputEvent)
    {
        if (!m_textRenderer || !m_textRenderer->getTransform() || !m_transform)
            return;

        Vec2F positionWS = m_transform->getWorldTransform().transform(_positionOS);
        Vec2F positionTextLocal = m_textRenderer->getTransform()->getWorldTransform().inversed().transform(positionWS);

        setCursorPosition(m_textRenderer->getSymbolIndexAtPosition(positionTextLocal));
    }

    //////////////////////////////////////////
    void EditBox2D::notifyCursorPressOut(CursorInputEvent& _inputEvent)
    {
        setSelected(false);
    }

    //////////////////////////////////////////
    void EditBox2D::notifyCursorReleaseOut(CursorInputEvent& _inputEvent)
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
    void EditBox2D::notifyClick(Vec2F const& _positionOS, CursorInputEvent& _inputEvent)
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

        if (getEntityRaw() && getEntityRaw()->getEcsWorld())
        {
            EntityId receiverEid = (m_eventReceiverEid != c_invalidEntityId) ? m_eventReceiverEid : getEntityId();
            getEntityRaw()->getEcsWorld()->sendEvent<EditBox2DSelectedChangedEvent>(
                receiverEid, getEntityId(), m_selected);
        }
    }

    //////////////////////////////////////////
    void EditBox2D::setText(String const& _text)
    {
        if (m_text == _text)
            return;

        m_text = _text;
        m_cursorPosition = m_text.size();

        processTextChanged();
    }

    //////////////////////////////////////////
    void EditBox2D::setCursorPosition(Size _position)
    {
        m_cursorPosition = Math::Min(_position, m_text.size());

        resetBlinkTimer();
        updateCursorRendererPosition();
        updateCursorRendererEnabled();
    }

    //////////////////////////////////////////
    void EditBox2D::insertTextAtCursor(String const& _text)
    {
        String filteredText;
        filteredText.reserve(_text.size());
        for (S8 c : _text)
            if (c >= 32 && c < 127)
                filteredText.push_back(c);

        if (filteredText.empty())
            return;

        m_cursorPosition = Math::Min(m_cursorPosition, m_text.size());
        m_text.insert(m_cursorPosition, filteredText);
        m_cursorPosition += filteredText.size();

        processTextChanged();
    }

    //////////////////////////////////////////
    void EditBox2D::deleteSymbolBeforeCursor()
    {
        m_cursorPosition = Math::Min(m_cursorPosition, m_text.size());
        if (m_cursorPosition == 0)
            return;

        m_text.erase(m_cursorPosition - 1, 1);
        --m_cursorPosition;

        processTextChanged();
    }

    //////////////////////////////////////////
    void EditBox2D::deleteSymbolAtCursor()
    {
        m_cursorPosition = Math::Min(m_cursorPosition, m_text.size());
        if (m_cursorPosition >= m_text.size())
            return;

        m_text.erase(m_cursorPosition, 1);

        processTextChanged();
    }

    //////////////////////////////////////////
    void EditBox2D::moveCursor(S32 _delta, bool _wordJump)
    {
        Size pos = Math::Min(m_cursorPosition, m_text.size());

        if (_wordJump)
        {
            if (_delta < 0)
            {
                while (pos > 0 && m_text[pos - 1] == ' ') --pos;
                while (pos > 0 && m_text[pos - 1] != ' ') --pos;
            }
            else if (_delta > 0)
            {
                while (pos < m_text.size() && m_text[pos] != ' ') ++pos;
                while (pos < m_text.size() && m_text[pos] == ' ') ++pos;
            }
        }
        else
        {
            if (_delta < 0 && pos > 0)
                --pos;
            else if (_delta > 0 && pos < m_text.size())
                ++pos;
        }

        setCursorPosition(pos);
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

        Vec2F cursorPosition = m_textRenderer->getSymbolPosition(Math::Min(m_cursorPosition, m_text.size()));
        if (m_cursorRenderer->getTransform())
            m_cursorRenderer->getTransform()->setLocalPosition(cursorPosition);
    }

    //////////////////////////////////////////
    void EditBox2D::updateCursorRendererEnabled()
    {
        if (!m_cursorRenderer)
            return;

        bool cursorEnabled = isTextInputAvailable() && m_cursorBlink;
        if (m_cursorRenderer->getMeshRenderer())
            m_cursorRenderer->getMeshRenderer()->setEnabled(cursorEnabled);
        else
        {
            MeshRendererInstanced* meshRendererInstanced = m_cursorRenderer->getEntityRaw()->getComponentRaw<MeshRendererInstanced>();
            if (meshRendererInstanced)
                meshRendererInstanced->setEnabled(cursorEnabled);
        }
    }

    //////////////////////////////////////////
    void EditBox2D::processTextChanged()
    {
        resetBlinkTimer();
        updateTextRenderer();
        updateCursorRendererPosition();
        updateCursorRendererEnabled();

        eventTextChanged(this, m_text);

        if (getEntityRaw() && getEntityRaw()->getEcsWorld())
        {
            EntityId receiverEid = (m_eventReceiverEid != c_invalidEntityId) ? m_eventReceiverEid : getEntityId();
            getEntityRaw()->getEcsWorld()->sendEvent<EditBox2DTextChangedEvent>(
                receiverEid, getEntityId());
        }
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

    //////////////////////////////////////////
    void EditBox2D::processAppear()
    {
        if (m_textRenderer && !m_cursorRenderer)
            createCursorRenderer();

        updateTextRenderer();
        updateCursorRendererPosition();
        updateCursorRendererEnabled();
    }

    //////////////////////////////////////////
    void EditBox2D::createCursorRenderer()
    {
        if (!m_textRenderer || !m_textRenderer->getTransform())
            return;

        if (!getEntityRaw() || !getEntityRaw()->getEcsScene())
            return;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        if (!renderSystem)
            return;

        F32 fontSize = (F32)m_textRenderer->getFontSize();
        F32 cursorHeight = fontSize + 4.0f;
        F32 descent = (0.5f * (cursorHeight - fontSize) - 1);

        SpriteRenderer2DPtr cursorRenderer = SpriteHelper::CreateSprite(
            m_textRenderer->getColor(),
            Vec2F(1.0f, cursorHeight),
            Vec2F::c_zero,
            renderSystem->getMaterialManager()->getSpriteMaterial(),
            m_textRenderer->getTransform(),
            getEntityRaw()->getEcsScene(),
            Vec2F::c_zero,
            Vec2F(0.0f, descent / cursorHeight));

        cursorRenderer->getEntityRaw()->ensureComponent<StaticName>()->setStaticName("Cursor");

        // Runtime-created helper - keep it out of the serialized data
        for (auto const& componentData : cursorRenderer->getEntityRaw()->getComponents())
            componentData.second->setFlag(Component::Flags::SerializationDisabled, true);

        setCursorRenderer(cursorRenderer);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(EditBox2DAppear,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        EditBox2D* _editBox2D)
    {
        _editBox2D->processAppear();
    }

} // namespace Maze
//////////////////////////////////////////
