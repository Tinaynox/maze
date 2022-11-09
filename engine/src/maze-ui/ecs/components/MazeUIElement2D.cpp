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
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UIElement2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UIElement2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, captureCursorHits, false, getCaptureCursorHits, setCaptureCursorHits),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, flags, Flags::UnpressOnUnfocus, getFlags, setFlags));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UIElement2D);

    //////////////////////////////////////////
    UIElement2D::UIElement2D()
        : m_transform(nullptr)
        , m_captureCursorHits(false)
        , m_flags(Flags::UnpressOnUnfocus)
        , m_cursorIndex(-1)
        , m_focused(false)
        , m_pressed(false)
    {
    }

    //////////////////////////////////////////
    UIElement2D::~UIElement2D()
    {

    }

    //////////////////////////////////////////
    UIElement2DPtr UIElement2D::Create()
    {
        UIElement2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(UIElement2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool UIElement2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void UIElement2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_bounds = getEntityRaw()->ensureComponent<Bounds2D>();
    }        

    //////////////////////////////////////////
    void UIElement2D::processCursorPress(
        CursorInputEvent const& _inputEvent)
    {
        if (m_cursorIndex != -1 && m_cursorIndex != _inputEvent.index)
            return;

        if (!eventCursorPressIn.empty() || !eventCursorPressOut.empty() || !eventPressedChanged.empty() || m_captureCursorHits)
        {
            Vec2DF const& positionWS = _inputEvent.position;

            if (!m_bounds->getBounds().contains(positionWS))
            {
                eventCursorPressOut(_inputEvent);
                return;
            }

            Vec2DF positionOS = m_transform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

            if (positionOS.x < 0 || positionOS.y < 0 ||
                positionOS.x >= m_transform->getSize().x || positionOS.y >= m_transform->getSize().y)
            {
                eventCursorPressOut(_inputEvent);
                return;
            }
            
            if (m_captureCursorHits)
                _inputEvent.captureHit((S32)getEntityId());

            m_cursorIndex = _inputEvent.index;

            eventCursorPressIn(positionOS, _inputEvent);

            setPressed(true);
        }
    }

    //////////////////////////////////////////
    void UIElement2D::processCursorClick(
        CursorInputEvent const& _inputEvent)
    {
        if (m_cursorIndex != -1 && m_cursorIndex != _inputEvent.index)
            return;

        if (!eventSingleClick.empty() || m_captureCursorHits)
        {
            Vec2DF const& positionWS = _inputEvent.position;

            if (!m_bounds->getBounds().contains(positionWS))
                return;

            Vec2DF positionOS = m_transform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

            if (positionOS.x < 0 || positionOS.y < 0 ||
                positionOS.x >= m_transform->getSize().x || positionOS.y >= m_transform->getSize().y)
                return;

            if (m_captureCursorHits)
                _inputEvent.captureHit((S32)getEntityId());

            eventSingleClick(positionOS, _inputEvent);
        }
    }

    //////////////////////////////////////////
    void UIElement2D::processCursorDoubleClick(
        CursorInputEvent const& _inputEvent)
    {
        if (m_cursorIndex != -1 && m_cursorIndex != _inputEvent.index)
            return;

        if (!eventDoubleClick.empty() || m_captureCursorHits)
        {
            Vec2DF const& positionWS = _inputEvent.position;

            if (!m_bounds->getBounds().contains(positionWS))
                return;

            Vec2DF positionOS = m_transform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

            if (positionOS.x < 0 || positionOS.y < 0 ||
                positionOS.x >= m_transform->getSize().x || positionOS.y >= m_transform->getSize().y)
                return;

            if (m_captureCursorHits)
                _inputEvent.captureHit((S32)getEntityId());

            eventDoubleClick(positionOS, _inputEvent);
        }
    }

    //////////////////////////////////////////
    void UIElement2D::processCursorRelease(
        CursorInputEvent const& _inputEvent)
    {
        if (m_cursorIndex != -1 && m_cursorIndex != _inputEvent.index)
            return;

        m_cursorIndex = -1;

        setPressed(false);

        if (!eventCursorReleaseIn.empty() || !eventPressedChanged.empty() || !eventCursorRelease.empty() || !eventCursorReleaseOut.empty() || !eventClick.empty())
        {
            Vec2DF const& positionWS = _inputEvent.position;

            eventCursorRelease(_inputEvent);

            if (!m_bounds->getBounds().contains(positionWS))
            {
                eventCursorReleaseOut(_inputEvent);
                return;
            }

            Vec2DF positionOS = m_transform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

            if (positionOS.x < 0 || positionOS.y < 0 ||
                positionOS.x >= m_transform->getSize().x || positionOS.y >= m_transform->getSize().y)
            {
                eventCursorReleaseOut(_inputEvent);
            }
            else
            {
                if (_inputEvent.capturerHandle != 0 && _inputEvent.capturerHandle != (S32)getEntityId())
                    return;

                if (m_captureCursorHits)
                    _inputEvent.captureHit((S32)getEntityId());

                eventCursorReleaseIn(positionOS, _inputEvent);

                if (m_focused)
                {
                    eventClick(positionOS, _inputEvent);
                }
            }
        }
    }

    //////////////////////////////////////////
    void UIElement2D::processCursorMove(
        CursorInputEvent const& _inputEvent)
    {
        if (m_cursorIndex != -1 && m_cursorIndex != _inputEvent.index)
            return;

        if (!eventCursorMoveIn.empty() || !eventCursorMoveOut.empty() || !eventFocusChanged.empty() || m_captureCursorHits)
        {
            Vec2DF const& positionWS = _inputEvent.position;

            if (!m_bounds->getBounds().contains(positionWS))
            {
                if (m_focused)
                    setFocused(false);

                eventCursorMoveOut(_inputEvent);

                return;
            }

            Vec2DF positionOS = m_transform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

            if (positionOS.x < 0 || positionOS.y < 0 ||
                positionOS.x >= m_transform->getSize().x || positionOS.y >= m_transform->getSize().y)
            {
                if (m_focused)
                    setFocused(false);

                eventCursorMoveOut(_inputEvent);

                return;
            }

            if (!m_focused)
                setFocused(true);
            
            if (m_captureCursorHits)
                _inputEvent.captureHit((S32)getEntityId());

            eventCursorMoveIn(positionOS, _inputEvent);
        }
    }

    //////////////////////////////////////////
    void UIElement2D::processCursorDrag(
        CursorInputEvent const& _inputEvent)
    {
        if (m_cursorIndex != -1 && m_cursorIndex != _inputEvent.index)
            return;

        if (!eventCursorDrag.empty() || !eventFocusChanged.empty() || m_captureCursorHits)
        {
            Vec2DF const& positionWS = _inputEvent.position;

            if (!m_bounds->getBounds().contains(positionWS))
            {
                if (m_focused)
                    setFocused(false);

                return;
            }

            Vec2DF positionOS = m_transform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

            if (positionOS.x < 0 || positionOS.y < 0 ||
                positionOS.x >= m_transform->getSize().x || positionOS.y >= m_transform->getSize().y)
            {
                if (m_focused)
                    setFocused(false);

                return;
            }

            if (!m_focused)
                setFocused(true);
            
            if (m_captureCursorHits)
                _inputEvent.captureHit((S32)getEntityId());

            eventCursorDrag(positionOS, _inputEvent);
        }
    }

    //////////////////////////////////////////
    void UIElement2D::processCursorWheel(CursorWheelInputEvent const& _inputEvent)
    {
        if (!eventCursorWheel.empty())
        {
            Vec2DF const& positionWS = _inputEvent.position;

            if (!m_bounds->getBounds().contains(positionWS))
                return;

            Vec2DF positionOS = m_transform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);
            if (positionOS.x < 0 || positionOS.y < 0 ||
                positionOS.x >= m_transform->getSize().x || positionOS.y >= m_transform->getSize().y)
            {
                return;
            }

            eventCursorWheel(_inputEvent);
        }
    }

    //////////////////////////////////////////
    void UIElement2D::setFocused(bool _focused)
    {
        if (m_focused == _focused)
            return;

        m_focused = _focused;

        eventFocusChanged(m_focused);

        if (!m_focused)
        {
            if (getFlag(Flags::UnpressOnUnfocus))
            {
                setPressed(false);
            }
        }
    }

    //////////////////////////////////////////
    void UIElement2D::setPressed(bool _pressed)
    {
        if (m_pressed == _pressed)
            return;

        m_pressed = _pressed;

        eventPressedChanged(m_pressed);
    }
    
    
} // namespace Maze
//////////////////////////////////////////
