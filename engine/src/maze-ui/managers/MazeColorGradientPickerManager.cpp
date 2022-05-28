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
#include "maze-ui/managers/MazeColorGradientPickerManager.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/color-picker/MazeSceneColorGradientPicker.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ColorGradientPickerManager
    //
    //////////////////////////////////////////
    ColorGradientPickerManager* ColorGradientPickerManager::s_instance = nullptr;

    //////////////////////////////////////////
    ColorGradientPickerManager::ColorGradientPickerManager()
        : m_gradient(ColorGradient(0.0f, Vec4DF::c_one, 1.0f, Vec4DF::c_one))
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    ColorGradientPickerManager::~ColorGradientPickerManager()
    {
        closeColorGradientPickerWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::Initialize(ColorGradientPickerManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(ColorGradientPickerManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool ColorGradientPickerManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::update(F32 _dt)
    {
        if (isColorGradientPickerWindowOpened() && !isColorGradientPickerWindowFocused())
            m_checkClose = true;

        if (m_checkClose)
        {
            if (!ColorPickerManager::GetInstancePtr()->isColorPickerWindowFocused())
            {
                if (m_colorPickerRenderWindow)
                    m_colorPickerRenderWindow->getWindow()->close();
            }

            m_checkClose = false;
        }
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::openGradientPicker(
        ColorPickerCallback _callback,
        ColorGradient const& _gradient)
    {
        m_callback = nullptr;

        setGradient(_gradient);

        m_callback = _callback;

        openColorGradientPickerWindow();
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::setGradient(ColorGradient const& _gradient)
    {
        m_gradient = _gradient;

        eventGradientChanged(m_gradient);

        if (m_callback)
            m_callback(m_gradient);
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::openColorGradientPickerWindow()
    {
        if (m_colorPickerRenderWindow &&
            m_colorPickerRenderWindow->getWindow() &&
            m_colorPickerRenderWindow->getWindow()->isOpened())
        {
            m_colorPickerRenderWindow->getWindow()->setFocused(true);

            openColorGradientPickerScene();

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2DU(360, 160),
                32,
                "Gradient");
         
        m_colorPickerRenderWindow = RenderWindow::Create(params);

        m_colorPickerRenderWindow->setName("ColorGradientPicker");

        if (m_colorPickerRenderWindow && m_colorPickerRenderWindow->getWindow())
        {
            m_colorPickerRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &ColorGradientPickerManager::notifyWindowWillClose);
            m_colorPickerRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &ColorGradientPickerManager::notifyWindowFocusChanged);

            openColorGradientPickerScene();
        }
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::closeColorGradientPickerWindow()
    {
        m_callback = nullptr;

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
    void ColorGradientPickerManager::openColorGradientPickerScene()
    {
        SceneColorGradientPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneColorGradientPicker>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneColorGradientPicker>(true, m_colorPickerRenderWindow);
        }

        scene->setup();
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::closeColorGradientPickerScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneColorGradientPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneColorGradientPicker>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::notifyWindowWillClose(Window* _window)
    {
        closeColorGradientPickerScene();
        closeColorGradientPickerWindow();
    }

    //////////////////////////////////////////
    void ColorGradientPickerManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
        {
            m_checkClose = true;
        }
    }

    //////////////////////////////////////////
    bool ColorGradientPickerManager::isColorGradientPickerWindowOpened()
    {
        if (!m_colorPickerRenderWindow)
            return false;

        if (!m_colorPickerRenderWindow->getWindow())
            return false;

        return m_colorPickerRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool ColorGradientPickerManager::isColorGradientPickerWindowFocused()
    {
        if (!m_colorPickerRenderWindow)
            return false;

        if (!m_colorPickerRenderWindow->getWindow())
            return false;

        return m_colorPickerRenderWindow->getWindow()->getFocused();
    }
    
} // namespace Maze
//////////////////////////////////////////
