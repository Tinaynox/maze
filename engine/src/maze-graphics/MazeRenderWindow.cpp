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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderWindow
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderWindow, RenderTarget);

    //////////////////////////////////////////
    RenderWindow::RenderWindow()
    {
    }

    //////////////////////////////////////////
    RenderWindow::~RenderWindow()
    {
        if (m_renderSystem)
        {
            m_renderSystem->processRenderWindowDestroyed(this);
        }

        destroySystemWindow();
    }

    //////////////////////////////////////////
    RenderWindowPtr RenderWindow::Create(RenderWindowParams const& _params)
    {
        RenderWindowParams params = _params;

        GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
        MAZE_ERROR_RETURN_VALUE_IF(!graphicsManager, RenderWindowPtr(), "GraphicsManager is not exists!");

        RenderSystemPtr& renderSystem = params.renderSystem;
        if (!renderSystem)
            renderSystem = graphicsManager->getDefaultRenderSystem();

        MAZE_ERROR_RETURN_VALUE_IF(!renderSystem, RenderWindowPtr(), "RenderSystem is not available!");

        RenderWindowPtr window = renderSystem->createRenderWindow(params);

        return window;
    }

    //////////////////////////////////////////
    bool RenderWindow::init(RenderWindowParams const& _params)
    {
        if (!RenderTarget::init(_params.renderSystem.get()))
            return false;

        if (!setupSystemWindow(_params.windowParams))
        {
            MAZE_ERROR("System window cannot be created!");
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    Vec2U RenderWindow::getRenderTargetSize() const
    {
        if (!m_window)
            return Vec2U::c_zero;

        return m_window->getClientSize();
    }

    //////////////////////////////////////////
    void RenderWindow::destroySystemWindow()
    {
        if (!m_window)
            return;

        m_window->eventWindowCreated.unsubscribe(this);
        m_window->eventWindowPositionChanged.unsubscribe(this);
        m_window->eventWindowSizeChanged.unsubscribe(this);
        m_window->eventWindowWillClose.unsubscribe(this);
        m_window->eventWindowClosed.unsubscribe(this);
        m_window->eventWindowFocusChanged.unsubscribe(this);

        m_window.reset();
    }

    //////////////////////////////////////////
    WindowPtr RenderWindow::fetchSystemWindow(WindowParamsPtr const& _params)
    {
        return Window::Create(_params);
    }

    ///////////////////////////////////////
    bool RenderWindow::setupSystemWindow(WindowParamsPtr const& _params)
    {
        destroySystemWindow();

        m_window = fetchSystemWindow(_params);
        if (!m_window)
            return false;

        m_window->eventWindowCreated.subscribe(this, &RenderWindow::notifyWindowCreated);
        m_window->eventWindowPositionChanged.subscribe(this, &RenderWindow::notifyWindowPositionChanged);
        m_window->eventWindowSizeChanged.subscribe(this, &RenderWindow::notifyWindowSizeChanged);
        m_window->eventWindowWillClose.subscribe(this, &RenderWindow::notifyWindowWillClose);
        m_window->eventWindowClosed.subscribe(this, &RenderWindow::notifyWindowClosed);
        m_window->eventWindowFocusChanged.subscribe(this, &RenderWindow::notifyWindowFocusChanged);

        return true;
    }

    //////////////////////////////////////////
    void RenderWindow::notifyWindowCreated(Window*  _window)
    {
    }

    //////////////////////////////////////////
    void RenderWindow::notifyWindowPositionChanged(Window* _window)
    {
    }

    //////////////////////////////////////////
    void RenderWindow::notifyWindowSizeChanged(Window* _window)
    {
        eventRenderTargetResized(this);
    }

    //////////////////////////////////////////
    void RenderWindow::notifyWindowWillClose(Window* _window)
    {
    }

    //////////////////////////////////////////
    void RenderWindow::notifyWindowClosed(Window* _window)
    {
    }

    //////////////////////////////////////////
    void RenderWindow::notifyWindowFocusChanged(Window* _window)
    {
    }

    //////////////////////////////////////////
    bool RenderWindow::isReadyToRender() const
    {
        if (!getWindow()->isOpened())
            return false;

        if (getWindow()->getMinimized())
            return false;

        return true;
    }

    //////////////////////////////////////////
    void RenderWindow::endDraw()
    {
        
    }

    //////////////////////////////////////////
    void RenderWindow::close()
    {
        destroySystemWindow();
    }

    //////////////////////////////////////////
    bool RenderWindow::getFocused() const
    {
        if (!m_window)
            return false;

        return m_window->getFocused();
    }

} // namespace Maze
//////////////////////////////////////////
