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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
#include "maze-editor-tools/pickers/MazeSceneColorPicker.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ColorPickerManager
    //
    //////////////////////////////////////////
    ColorPickerManager* ColorPickerManager::s_instance = nullptr;

    //////////////////////////////////////////
    ColorPickerManager::ColorPickerManager()
        : m_color(ColorU32::c_white)
        , m_colorHDR(ColorF128::c_white)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    ColorPickerManager::~ColorPickerManager()
    {
        closeColorPickerWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void ColorPickerManager::Initialize(ColorPickerManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ColorPickerManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool ColorPickerManager::init()
    {
        

        return true;
    }

    //////////////////////////////////////////
    void ColorPickerManager::openColorPicker(
        ColorPickerCallback _callback,
        ColorU32 const& _color)
    {
        m_callback = nullptr;

        setColor(_color);

        m_callback = _callback;

        openColorPickerWindow();
    }

    //////////////////////////////////////////
    void ColorPickerManager::setColor(ColorU32 const& _color)
    {
        m_color = _color;

        eventColorChanged(m_color);

        if (m_callback)
            m_callback(m_color);
    }

    //////////////////////////////////////////
    void ColorPickerManager::openColorHDRPicker(
        ColorHDRPickerCallback _callback,
        ColorF128 const& _color)
    {
        m_callbackHDR = nullptr;

        setColorHDR(_color);

        m_callbackHDR = _callback;

        openColorPickerWindow(true);
    }

    //////////////////////////////////////////
    void ColorPickerManager::setColorHDR(ColorF128 const& _color)
    {
        m_colorHDR = _color;

        eventColorHDRChanged(m_colorHDR);

        if (m_callbackHDR)
            m_callbackHDR(m_colorHDR);
    }

    //////////////////////////////////////////
    void ColorPickerManager::openColorPickerWindow(bool _hdr)
    {
        if (m_colorPickerRenderWindow &&
            m_colorPickerRenderWindow->getWindow() &&
            m_colorPickerRenderWindow->getWindow()->isOpened())
        {
            m_colorPickerRenderWindow->getWindow()->setFocused(true);

            openColorPickerScene(_hdr);

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2U(242, 480),
                32,
                "Color");
         
        m_colorPickerRenderWindow = RenderWindow::Create(params);

        m_colorPickerRenderWindow->setName("ColorPicker");

        if (m_colorPickerRenderWindow && m_colorPickerRenderWindow->getWindow())
        {
            m_colorPickerRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &ColorPickerManager::notifyWindowWillClose);
            m_colorPickerRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &ColorPickerManager::notifyWindowFocusChanged);

            openColorPickerScene(_hdr);
        }
    }

    //////////////////////////////////////////
    void ColorPickerManager::closeColorPickerWindow()
    {
        m_callback = nullptr;
        m_callbackHDR = nullptr;

        if (!m_colorPickerRenderWindow)
            return;

        RenderWindowPtr colorPickerRenderWindow = m_colorPickerRenderWindow;
        m_colorPickerRenderWindow.reset();

        if (!colorPickerRenderWindow->getWindow())
            return;

        colorPickerRenderWindow->getWindow()->eventWindowWillClose.unsubscribe(this);
        colorPickerRenderWindow->getWindow()->eventWindowFocusChanged.unsubscribe(this);
        colorPickerRenderWindow->getWindow()->close();
    }

    //////////////////////////////////////////
    void ColorPickerManager::openColorPickerScene(bool _hdr)
    {
        SceneColorPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneColorPicker>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneColorPicker>(true, m_colorPickerRenderWindow);
        }

        scene->setup(_hdr);
    }

    //////////////////////////////////////////
    void ColorPickerManager::closeColorPickerScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneColorPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneColorPicker>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void ColorPickerManager::notifyWindowWillClose(Window* _window)
    {
        closeColorPickerScene();
        closeColorPickerWindow();
    }

    //////////////////////////////////////////
    void ColorPickerManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
            _window->close();
    }
    
    //////////////////////////////////////////
    bool ColorPickerManager::isColorPickerWindowOpened()
    {
        if (!m_colorPickerRenderWindow)
            return false;

        if (!m_colorPickerRenderWindow->getWindow())
            return false;

        return m_colorPickerRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool ColorPickerManager::isColorPickerWindowFocused()
    {
        if (!m_colorPickerRenderWindow)
            return false;

        if (!m_colorPickerRenderWindow->getWindow())
            return false;

        return m_colorPickerRenderWindow->getWindow()->getFocused();
    }

} // namespace Maze
//////////////////////////////////////////
