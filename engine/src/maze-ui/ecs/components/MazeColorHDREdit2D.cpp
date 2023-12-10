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
#include "maze-ui/ecs/components/MazeColorHDREdit2D.hpp"
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
#include "maze-graphics/MazeMaterial.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ColorHDREdit2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ColorHDREdit2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, colorRenderer, ComponentPtr(), getColorRendererComponent, setColorRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, alphaRenderer, ComponentPtr(), getAlphaRendererComponent, setAlphaRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorF128, color, ColorF128(), getColor, setColor));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ColorHDREdit2D);

    //////////////////////////////////////////
    ColorHDREdit2D::ColorHDREdit2D()
        : m_selected(false)
    {
    }

    //////////////////////////////////////////
    ColorHDREdit2D::~ColorHDREdit2D()
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
    ColorHDREdit2DPtr ColorHDREdit2D::Create()
    {
        ColorHDREdit2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ColorHDREdit2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ColorHDREdit2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &ColorHDREdit2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &ColorHDREdit2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &ColorHDREdit2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &ColorHDREdit2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &ColorHDREdit2D::notifyClick);
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
            
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        setSelected(false);
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(true);
        }
        

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::notifyClick(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        ColorPickerManager::GetInstancePtr()->openColorHDRPicker(
            [this](ColorF128 const& _color)
            {
                setColor(_color);
            },
            m_color);

        eventClick(this);
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::setColor(ColorF128 const& _color)
    {
        if (m_color == _color)
            return;

        m_color = _color;

        eventColorChanged(this, m_color);

        processColor();
    }

    //////////////////////////////////////////
    void ColorHDREdit2D::processColor()
    {
        if (m_colorRenderer)
        {
            ColorU32 color;
            F32 intensity;
            m_color.toColorU32AndIntensity(color, intensity);

            m_colorRenderer->setColor(
                color.r,
                color.g,
                color.b);

            m_colorRenderer->getMaterial()->setUniform("u_intensity", intensity);
        }

        if (m_alphaRenderer)
        {
            F32 alpha = (F32)m_color.a;
            m_alphaRenderer->getTransform()->setWidth(
                m_alphaRenderer->getTransform()->getParent()->getWidth() * alpha);
        }
    }
    

} // namespace Maze
//////////////////////////////////////////
