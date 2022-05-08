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
#include "maze-ui/ecs/components/MazeButton2D.hpp"
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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Button2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Button2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, transitionSprite, ComponentPtr(), getTransitionSpriteComponent, setTransitionSprite),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, normalColor, ColorU32(255, 255, 255, 255), getNormalColor, setNormalColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, focusedColor, ColorU32(245, 245, 245, 255), getFocusedColor, setFocusedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, pressedColor, ColorU32(200, 200, 200, 255), getPressedColor, setPressedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, selectedColor, ColorU32(245, 245, 245, 255), getSelectedColor, setSelectedColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, disabledColor, ColorU32(200, 200, 200, 128), getDisabledColor, setDisabledColor)
    );

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Button2D);

    //////////////////////////////////////////
    Button2D::Button2D()
        : m_normalColor(ColorU32(255, 255, 255, 255))
        , m_focusedColor(ColorU32(245, 245, 245, 255))
        , m_pressedColor(ColorU32(200, 200, 200, 255))
        , m_selectedColor(ColorU32(245, 245, 245, 255))
        , m_disabledColor(ColorU32(200, 200, 200, 128))
    {
    }

    //////////////////////////////////////////
    Button2D::~Button2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
            m_UIElement2D->eventFocusChanged.unsubscribe(this);
            m_UIElement2D->eventPressedChanged.unsubscribe(this);
            m_UIElement2D->eventClick.unsubscribe(this);
            m_UIElement2D->eventDoubleClick.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    bool Button2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Button2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &Button2D::notifyCursorPressIn);
        m_UIElement2D->eventFocusChanged.subscribe(this, &Button2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &Button2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &Button2D::notifyClick);
        m_UIElement2D->eventDoubleClick.subscribe(this, &Button2D::notifyDoubleClick);
    }

    //////////////////////////////////////////
    void Button2D::notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        eventCursorPressIn(this, _positionOS, _inputEvent);
    }

    //////////////////////////////////////////
    void Button2D::notifyFocusChanged(bool _focused)
    {
        updateTransitionUI();
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void Button2D::notifyPressedChanged(bool _pressed)
    {
        updateTransitionUI();
        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void Button2D::notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        eventClick(this, _inputEvent);
    }

    //////////////////////////////////////////
    void Button2D::notifyDoubleClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        eventDoubleClick(this, _inputEvent);
    }

    //////////////////////////////////////////
    void Button2D::updateTransitionUI()
    {
        if (!m_transitionSprite)
            return;

        if (!getEntityRaw())
            return;

        if (!m_UIElement2D)
            return;

        if (getPressed())
        {
            m_transitionSprite->setColor(getPressedColor());
        }
        else
        {
            if (getFocused())
            {
                m_transitionSprite->setColor(getFocusedColor());
            }
            else
            {
                m_transitionSprite->setColor(getNormalColor());
            }
        }
    }
    
    
} // namespace Maze
//////////////////////////////////////////
