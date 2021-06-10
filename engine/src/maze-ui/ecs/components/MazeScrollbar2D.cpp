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
#include "maze-ui/ecs/components/MazeScrollbar2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
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
    // Class Scrollbar2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Scrollbar2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Scrollbar2DDirection, direction, Scrollbar2DDirection::LeftToRight, getDirection, setDirection),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, handleTransform, nullptr, getHandleTransformComponent, setHandleTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, handleSpriteRenderer, nullptr, getHandleSpriteRendererComponent, setHandleSpriteRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, normalColor, ColorU32(255, 255, 255, 255), getNormalColor, setNormalColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, focusedColor, ColorU32(245, 245, 245, 255), getFocusedColor, setFocusedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, pressedColor, ColorU32(200, 200, 200, 255), getPressedColor, setPressedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, selectedColor, ColorU32(245, 245, 245, 255), getSelectedColor, setSelectedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, disabledColor, ColorU32(200, 200, 200, 128), getDisabledColor, setDisabledColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, value, 0.0f, getValue, setValue),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, handleSize, 0.2f, getHandleSize, setHandleSize)
    );

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Scrollbar2D);

    //////////////////////////////////////////
    Scrollbar2D::Scrollbar2D()
        : m_direction(Scrollbar2DDirection::LeftToRight)
        , m_normalColor(ColorU32(255, 255, 255, 255))
        , m_focusedColor(ColorU32(245, 245, 245, 255))
        , m_pressedColor(ColorU32(200, 200, 200, 255))
        , m_selectedColor(ColorU32(245, 245, 245, 255))
        , m_disabledColor(ColorU32(200, 200, 200, 128))
        , m_selected(false)
        , m_value(0.0f)
        , m_handleSize(0.2f)
        , m_deltaValue(0.0f)
    {
    }

    //////////////////////////////////////////
    Scrollbar2D::~Scrollbar2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
            m_UIElement2D->eventCursorReleaseOut.unsubscribe(this);
            m_UIElement2D->eventCursorDrag.unsubscribe(this);
            m_UIElement2D->eventCursorMoveOut.unsubscribe(this);
            m_UIElement2D->eventFocusChanged.unsubscribe(this);
            m_UIElement2D->eventPressedChanged.unsubscribe(this);
            m_UIElement2D->eventClick.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    Scrollbar2DPtr Scrollbar2D::Create()
    {
        Scrollbar2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Scrollbar2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Scrollbar2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Scrollbar2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);
        m_UIElement2D->setUnpressOnUnfocus(false);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &Scrollbar2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &Scrollbar2D::notifyCursorReleaseOut);
        m_UIElement2D->eventCursorDrag.subscribe(this, &Scrollbar2D::notifyCursorDrag);
        m_UIElement2D->eventCursorMoveOut.subscribe(this, &Scrollbar2D::notifyCursorMoveOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &Scrollbar2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &Scrollbar2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &Scrollbar2D::notifyClick);

    }

    //////////////////////////////////////////
    void Scrollbar2D::notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (m_handleTransform && m_handleTransform->calculateWorldAABB().contains(_inputEvent.position))
        {
            F32 value = cursorPositionToValue(_inputEvent.position);
            m_deltaValue = value - m_value;
        }
        else
        {
            m_deltaValue = 0.0f;
        }

        pickValueByCursor(_inputEvent.position);
    }

    //////////////////////////////////////////
    void Scrollbar2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        setSelected(false);
    }

    //////////////////////////////////////////
    void Scrollbar2D::notifyCursorDrag(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (m_UIElement2D->getPressed())
            pickValueByCursor(_inputEvent.position);
    }

    //////////////////////////////////////////
    void Scrollbar2D::notifyCursorMoveOut(CursorInputEvent const& _inputEvent)
    {

        if (m_UIElement2D->getPressed())
            pickValueByCursor(_inputEvent.position);
    }

    //////////////////////////////////////////
    void Scrollbar2D::notifyFocusChanged(bool _focused)
    {
        updateUI();

        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void Scrollbar2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(true);
        }

        updateUI();

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void Scrollbar2D::notifyClick(CursorInputEvent const& _inputEvent)
    {
        eventClick(this, _inputEvent);
    }

    //////////////////////////////////////////
    void Scrollbar2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void Scrollbar2D::setValue(F32 _value)
    {
        F32 value = Math::Clamp01(_value);

        if (m_value == value)
            return;

        m_value = value;

        updateUI();

        eventValueChanged(this, m_value);
    }

    //////////////////////////////////////////
    void Scrollbar2D::setHandleSize(F32 _value)
    {
        F32 value = Math::Clamp01(_value);

        if (m_handleSize == value)
            return;

        m_handleSize = value;

        updateUI();
    }

    //////////////////////////////////////////
    void Scrollbar2D::pickValueByCursor(Vec2DF const& _positionWS)
    {
        F32 value = cursorPositionToValue(_positionWS);
        setValue(value - m_deltaValue);
    }

    //////////////////////////////////////////
    F32 Scrollbar2D::cursorPositionToValue(Vec2DF const& _positionWS)
    {
        if (m_handleTransform)
        {
            Transform2DPtr const& handleSlideArea = m_handleTransform->getParent();
            Vec2DF handleSlideAreaPositionOS = handleSlideArea->getWorldTransform().inversedAffineCopy().transformAffine(_positionWS);

            switch (m_direction)
            {
                case Scrollbar2DDirection::LeftToRight:
                {
                    F32 widthHalf = m_handleTransform->getWidth() * 0.5f;
                    F32 from = widthHalf;
                    F32 to = m_handleTransform->getParent()->getWidth() - widthHalf;

                    F32 value = (handleSlideAreaPositionOS.x - from) / (to - from);
                    return value;
                }
                case Scrollbar2DDirection::RightToLeft:
                {
                    F32 widthHalf = m_handleTransform->getWidth() * 0.5f;
                    F32 from = widthHalf;
                    F32 to = m_handleTransform->getParent()->getWidth() - widthHalf;

                    F32 value = 1.0f - (handleSlideAreaPositionOS.x - from) / (to - from);
                    return value;
                }
                case Scrollbar2DDirection::BottomToTop:
                {
                    F32 heightHalf = m_handleTransform->getHeight() * 0.5f;
                    F32 from = heightHalf;
                    F32 to = m_handleTransform->getParent()->getHeight() - heightHalf;

                    F32 value = (handleSlideAreaPositionOS.y - from) / (to - from);
                    return value;
                }
                case Scrollbar2DDirection::TopToBottom:
                {
                    F32 heightHalf = m_handleTransform->getHeight() * 0.5f;
                    F32 from = heightHalf;
                    F32 to = m_handleTransform->getParent()->getHeight() - heightHalf;

                    F32 value = 1.0f - (handleSlideAreaPositionOS.y - from) / (to - from);
                    return value;
                }
                default:
                    break;
            }
        }

        return m_value;
    }

    //////////////////////////////////////////
    void Scrollbar2D::updateUI()
    {
        if (!m_handleTransform || !m_handleTransform->getParent())
            return;

        switch (m_direction)
        {
            case Scrollbar2DDirection::LeftToRight:
            case Scrollbar2DDirection::RightToLeft:
            {
                F32 width = m_handleTransform->getParent()->getWidth() * m_handleSize;
                F32 height = m_handleTransform->getParent()->getHeight();
                m_handleTransform->setSize(width, height);
                break;
            }
            case Scrollbar2DDirection::BottomToTop:
            case Scrollbar2DDirection::TopToBottom:
            {
                F32 width = m_handleTransform->getParent()->getWidth();
                F32 height = m_handleTransform->getParent()->getHeight() * m_handleSize;
                m_handleTransform->setSize(width, height);
                break;
            }
            default:
            {
                break;
            }
        }

        switch (m_direction)
        {
            case Scrollbar2DDirection::LeftToRight:
            {
                m_handleTransform->setAnchor(0.0f, 0.5f);
                m_handleTransform->setPivot(0.5f, 0.5f);
                F32 widthHalf = m_handleTransform->getWidth() * 0.5f;
                m_handleTransform->setLocalX(Math::Lerp(widthHalf, m_handleTransform->getParent()->getWidth() - widthHalf, m_value));
                m_handleTransform->setLocalY(0.0f);
                break;
            }
            case Scrollbar2DDirection::RightToLeft:
            {
                m_handleTransform->setAnchor(0.0f, 0.5f);
                m_handleTransform->setPivot(0.5f, 0.5f);
                F32 widthHalf = m_handleTransform->getWidth() * 0.5f;
                m_handleTransform->setLocalX(Math::Lerp(m_handleTransform->getParent()->getWidth() - widthHalf, widthHalf, m_value));
                m_handleTransform->setLocalY(0.0f);
                break;
            }
            case Scrollbar2DDirection::BottomToTop:
            {
                m_handleTransform->setAnchor(0.5f, 0.0f);
                m_handleTransform->setPivot(0.5f, 0.5f);
                F32 heightHalf = m_handleTransform->getHeight() * 0.5f;
                m_handleTransform->setLocalX(0.0f);
                m_handleTransform->setLocalY(Math::Lerp(heightHalf, m_handleTransform->getParent()->getHeight() - heightHalf, m_value));
                break;
            }
            case Scrollbar2DDirection::TopToBottom:
            {
                m_handleTransform->setAnchor(0.5f, 0.0f);
                m_handleTransform->setPivot(0.5f, 0.5f);
                F32 heightHalf = m_handleTransform->getHeight() * 0.5f;
                m_handleTransform->setLocalX(0.0f);
                m_handleTransform->setLocalY(Math::Lerp(m_handleTransform->getParent()->getHeight() - heightHalf, heightHalf, m_value));
                break;
            }
            default:
            {
                break;
            }
        }

        if (m_handleSpriteRenderer)
        {
            if (getPressed())
            {
                m_handleSpriteRenderer->setColor(getPressedColor());
            }
            else
            {
                if (getFocused())
                {
                    m_handleSpriteRenderer->setColor(getFocusedColor());
                }
                else
                {
                    m_handleSpriteRenderer->setColor(getNormalColor());
                }
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
