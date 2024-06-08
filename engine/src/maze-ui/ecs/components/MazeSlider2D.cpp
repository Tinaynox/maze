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
#include "maze-ui/ecs/components/MazeSlider2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
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
    // Class Slider2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Slider2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, fillRectTransform, ComponentPtr(), getFillRectTransformComponent, setFillRectTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, handleTransform, ComponentPtr(), getHandleTransformComponent, setHandleTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, value, 0.0f, getValue, setValue));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Slider2D);

    //////////////////////////////////////////
    Slider2D::Slider2D()
        : m_selected(false)
        , m_value(0.0f)
    {
    }

    //////////////////////////////////////////
    Slider2D::~Slider2D()
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
    Slider2DPtr Slider2D::Create()
    {
        Slider2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Slider2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Slider2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Slider2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void Slider2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);
        m_UIElement2D->setUnpressOnUnfocus(false);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &Slider2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &Slider2D::notifyCursorReleaseOut);
        m_UIElement2D->eventCursorDrag.subscribe(this, &Slider2D::notifyCursorDrag);
        m_UIElement2D->eventCursorMoveOut.subscribe(this, &Slider2D::notifyCursorMoveOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &Slider2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &Slider2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &Slider2D::notifyClick);
    }

    //////////////////////////////////////////
    void Slider2D::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        pickValueByCursor(_inputEvent.position);
    }

    //////////////////////////////////////////
    void Slider2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        setSelected(false);
    }

    //////////////////////////////////////////
    void Slider2D::notifyCursorDrag(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (m_UIElement2D->getPressed())
            pickValueByCursor(_inputEvent.position);
    }

    //////////////////////////////////////////
    void Slider2D::notifyCursorMoveOut(CursorInputEvent const& _inputEvent)
    {

        if (m_UIElement2D->getPressed())
            pickValueByCursor(_inputEvent.position);
    }

    //////////////////////////////////////////
    void Slider2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void Slider2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(true);
        }

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void Slider2D::notifyClick(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        eventClick(this, _inputEvent);
    }

    //////////////////////////////////////////
    void Slider2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void Slider2D::setValue(F32 _value)
    {
        F32 value = Math::Clamp01(_value);

        if (m_value == value)
            return;

        m_value = value;

        processValue();

        eventValueChanged(this, m_value);
    }

    //////////////////////////////////////////
    void Slider2D::processValue()
    {
        if (m_handleTransform)
        {
            m_handleTransform->setAnchor(m_value, m_handleTransform->getAnchor().y);
        }

        if (m_fillRectTransform)
        {
            m_fillRectTransform->setWidth(
                m_fillRectTransform->getParent()->getWidth() * m_value);
        }
    }
    
    //////////////////////////////////////////
    void Slider2D::pickValueByCursor(Vec2F const& _positionWS)
    {
        if (m_handleTransform)
        {
            Transform2DPtr const& handleSlideArea = m_handleTransform->getParent();
            Vec2F handleSlideAreaPositionOS = handleSlideArea->getWorldTransform().inversedAffine().transformAffine(_positionWS);
            setValue(handleSlideAreaPositionOS.x / handleSlideArea->getWidth());
        }
    }


} // namespace Maze
//////////////////////////////////////////
