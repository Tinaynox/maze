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
#include "maze-ui/ecs/components/MazeColorEdit2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
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
    // Class ColorEdit2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ColorEdit2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, colorRenderer, ComponentPtr(), getColorRendererComponent, setColorRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, alphaRenderer, ComponentPtr(), getAlphaRendererComponent, setAlphaRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, color, ColorU32(), getColor, setColor));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ColorEdit2D);

    //////////////////////////////////////////
    ColorEdit2D::ColorEdit2D()
        : m_selected(false)
    {
    }

    //////////////////////////////////////////
    ColorEdit2D::~ColorEdit2D()
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
    ColorEdit2DPtr ColorEdit2D::Create()
    {
        ColorEdit2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ColorEdit2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ColorEdit2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ColorEdit2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void ColorEdit2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &ColorEdit2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &ColorEdit2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &ColorEdit2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &ColorEdit2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &ColorEdit2D::notifyClick);
    }

    //////////////////////////////////////////
    void ColorEdit2D::notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
            
    }

    //////////////////////////////////////////
    void ColorEdit2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        setSelected(false);
    }

    //////////////////////////////////////////
    void ColorEdit2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void ColorEdit2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(true);
        }
        

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void ColorEdit2D::notifyClick(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        ColorPickerManager::GetInstancePtr()->openColorPicker(
            [this](ColorU32 const& _color)
            {
                setColor(_color);
            },
            m_color);

        eventClick(this);
    }

    //////////////////////////////////////////
    void ColorEdit2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void ColorEdit2D::setColor(ColorU32 const& _color)
    {
        if (m_color == _color)
            return;

        m_color = _color;

        eventColorChanged(this, m_color);

        processColor();
    }

    //////////////////////////////////////////
    void ColorEdit2D::processColor()
    {
        if (m_colorRenderer)
        {
            m_colorRenderer->setColor(m_color.r, m_color.g, m_color.b);
        }

        if (m_alphaRenderer)
        {
            F32 alpha = (F32)m_color.a / 255.0f;
            m_alphaRenderer->getTransform()->setWidth(
                m_alphaRenderer->getTransform()->getParent()->getWidth() * alpha);
        }
    }
    

} // namespace Maze
//////////////////////////////////////////
