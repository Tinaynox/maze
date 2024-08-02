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
#include "MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-render-system-opengl-core, MazeRenderWindowOpenGL)


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderWindowOpenGL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderWindowOpenGL, RenderWindow);

    //////////////////////////////////////////
    RenderWindowOpenGL::RenderWindowOpenGL()
    {
    }

    //////////////////////////////////////////
    RenderWindowOpenGL::~RenderWindowOpenGL()
    {
        destroySystemWindow();
    }

    //////////////////////////////////////////
    RenderWindowOpenGLPtr RenderWindowOpenGL::Create(RenderWindowParams const& _params)
    {
        RenderWindowOpenGLPtr window;
        MAZE_CREATE_AND_INIT_OS_OBJECT_SHARED_PTR(RenderWindowOpenGL, window, init(_params));
        return window;
    }

    //////////////////////////////////////////
    bool RenderWindowOpenGL::init(RenderWindowParams const& _params)
    {
        if (!RenderWindow::init(_params))
            return false;

        ensureContext(_params.antialiasingLevel);
        MAZE_ERROR_RETURN_VALUE_IF(!m_context, false, "Context is not created!");

        return true;
    }

    //////////////////////////////////////////
    void RenderWindowOpenGL::swapBuffers()
    {
        if (!m_context)
            return;

        ContextOpenGLScopeBind scopeBind(m_context);

        m_context->swapBuffers();
    }

    //////////////////////////////////////////
    bool RenderWindowOpenGL::processRenderTargetWillSet()
    {
        if (!m_context)
            return false;

        if (!m_context->makeCurrentContext(true, true))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool RenderWindowOpenGL::beginDraw()
    {
        if (!m_context->isValid())
            return false;

        return RenderWindow::beginDraw();
    }

    //////////////////////////////////////////
    void RenderWindowOpenGL::processRenderTargetSet()
    {
        m_context->setCurrentRenderBuffer(nullptr);
    }

    //////////////////////////////////////////
    void RenderWindowOpenGL::processRenderTargetWillReset()
    {
    }

    //////////////////////////////////////////
    void RenderWindowOpenGL::notifyWindowWillClose(Window* _window)
    {
        RenderWindow::notifyWindowWillClose(_window);
    }

    //////////////////////////////////////////
    void RenderWindowOpenGL::ensureContext(S32 _antialiasingLevel)
    {
        static S32 s_contextCounter = 0;

        if (!m_context)
        {
            ContextOpenGLConfig contextConfig;
            contextConfig.antialiasingLevel = _antialiasingLevel;

            Debug::Log("RenderWindowOpenGL: Creating context...");
            m_context = ContextOpenGL::Create(
                getRenderSystem()->cast<RenderSystemOpenGL>(),
                m_window,
                &contextConfig);
            String contextName = "RenderWindow_" + StringHelper::ToString(s_contextCounter++);
            m_context->setName(contextName);
            Debug::Log("RenderWindowOpenGL: Context created (%x).", m_context.get());

            Debug::Log("RenderWindowOpenGL: Creating render queue...");
            m_renderQueue = RenderQueueOpenGL::Create(this, m_context.get());
            Debug::Log("RenderWindowOpenGL: Rrender queue created.");
        }
        else
        if (!m_context->isValid())
        {
            Debug::Log("RenderWindowOpenGL: Context recreating (%x)...", m_context.get());
            m_context->createGLContext(m_window);
            Debug::Log("RenderWindowOpenGL: Context recreated (%x).", m_context.get());
        }
        else
        {

        }

    }

    //////////////////////////////////////////
    void RenderWindowOpenGL::destroyContext()
    {
        if (!m_context)
            return;

        m_context->destroyGLContext();
    }

    //////////////////////////////////////////
    void RenderWindowOpenGL::setVSync(S32 _vsync)
    {
        if (!m_context)
            return;

        m_context->setVSync(_vsync);
    }


} // namespace Maze
//////////////////////////////////////////
