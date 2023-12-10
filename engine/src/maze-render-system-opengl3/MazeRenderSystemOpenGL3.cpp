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
#include "MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3.hpp"
#include "maze-render-system-opengl3/MazeExtensionsOpenGL3.hpp"
#include "maze-render-system-opengl3/MazeContextOpenGL3.hpp"
#include "maze-render-system-opengl3/MazeFunctionsOpenGL3.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderSystemOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#    include "maze-render-system-opengl3/win/wgl/MazeRenderSystemOpenGL3WGL.hpp"
#    include "maze-render-system-opengl3/egl/MazeRenderSystemOpenGL3EGL.hpp"
#elif (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
#   include "maze-render-system-opengl3/osx/ns/MazeRenderSystemOpenGL3NS.hpp"
#elif (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
#   include "maze-render-system-opengl3/linux/glx/MazeRenderSystemOpenGL3GLX.hpp"
#elif (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
#   include "maze-render-system-opengl3/egl/MazeRenderSystemOpenGL3EGL.hpp"
#elif (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
#   include "maze-render-system-opengl3/egl/MazeRenderSystemOpenGL3EGL.hpp"
#endif


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderSystemOpenGL3
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderSystemOpenGL3, RenderSystem);

    //////////////////////////////////////////
    RenderSystemOpenGL3::RenderSystemOpenGL3()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3::~RenderSystemOpenGL3()
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL3Ptr RenderSystemOpenGL3::Create(RenderSystemOpenGLConfig const& _config)
    {
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)

#if (1)
        return RenderSystemOpenGL3WGL::Create(_config);
#else
        return RenderSystemOpenGL3EGL::Create(_config);
#endif

#elif (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
        return RenderSystemOpenGL3NS::Create(_config);
#elif (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
        return RenderSystemOpenGL3GLX::Create(_config);
#elif (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
        return RenderSystemOpenGL3EGL::Create(_config);
#elif (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
        return RenderSystemOpenGL3EGL::Create(_config);
#else
        MAZE_TODO
#endif

        return RenderSystemOpenGL3Ptr();
    }


    //////////////////////////////////////////
    bool RenderSystemOpenGL3::init(RenderSystemOpenGLConfig const& _config)
    {
        if (!RenderSystemOpenGL::init(_config))
            return false;

#if (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
        if (m_config.useDummyContext)
        {
            MAZE_WARNING("DummyContext is not supported on the current platform.");
            m_config.useDummyContext = false;
        }
#endif

        if (m_config.useDummyContext)
        {
            Debug::Log("Creating Dummy Context...");
            m_dummyContext = ContextOpenGL::Create(cast<RenderSystemOpenGL>());
            MAZE_ERROR_RETURN_VALUE_IF(!m_dummyContext, false, "Dummy context cannot is not created!");

            m_dummyContext->setName("Dummy Context");

            assignGLFunctions(m_dummyContext);


            MAZE_GL_CALL(Debug::Log("GL_VERSION:    %s",               (CString)mzglGetString(MAZE_GL_VERSION)));
            MAZE_GL_CALL(Debug::Log("GL_RENDERER:   %s",               (CString)mzglGetString(MAZE_GL_RENDERER)));
            MAZE_GL_CALL(Debug::Log("GL_VENDOR:     %s",               (CString)mzglGetString(MAZE_GL_VENDOR)));
            MAZE_GL_CALL(Debug::Log("GL_SHADING_LANGUAGE_VERSION: %s", (CString)mzglGetString(MAZE_GL_SHADING_LANGUAGE_VERSION)));

            m_dummyContext->getExtensions()->loadGLExtensions();
            m_dummyContext->getExtensions()->printGLExtensions();
            m_dummyContext->getExtensions()->printSupportedCompressedTextureFormats();

            m_dummyContext->flushConfig();

            m_dummyContext->makeCurrentContext(false);


            if (!m_dummyContext || m_dummyContext->getMajorVersion() <= 1)
            {
                if (!m_dummyContext)
                {
                    MAZE_ERROR("Dummy Context cannot be created!");
                }
                else
                {
                    MAZE_ERROR("Dummy Context version %u.%u is not supported!", m_dummyContext->getMajorVersion(), m_dummyContext->getMinorVersion());
                }

                MAZE_GL_CALL(MAZE_ERROR("GL_VERSION:    %s",               (CString)mzglGetString(MAZE_GL_VERSION)));
                MAZE_GL_CALL(MAZE_ERROR("GL_RENDERER:   %s",               (CString)mzglGetString(MAZE_GL_RENDERER)));
                MAZE_GL_CALL(MAZE_ERROR("GL_VENDOR:     %s",               (CString)mzglGetString(MAZE_GL_VENDOR)));
                MAZE_GL_CALL(MAZE_ERROR("GL_SHADING_LANGUAGE_VERSION: %s", (CString)mzglGetString(MAZE_GL_SHADING_LANGUAGE_VERSION)));

                return false;
            }

            Debug::Log("Shared Context created. Version: %u.%u", m_dummyContext->getMajorVersion(), m_dummyContext->getMinorVersion());
        }

        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemOpenGL3::getName()
    {
        static String s_name = "Render System GL3+";
        return s_name;
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3::assignGLFunctions(ContextOpenGLPtr const& _context)
    {
        AssignFunctionsOpenGL3(_context);
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL3::setupSystem(ContextOpenGLPtr const& _context)
    {
        MAZE_PROFILE_EVENT("RenderSystemOpenGL3::setupSystem");

        MAZE_ERROR_RETURN_VALUE_IF(m_systemInited, false, "Render system is already inited!");

        if (true)
        {
            assignGLFunctions(_context);

            _context->getExtensions()->loadGLExtensions();

#if (MAZE_PLATFORM != MAZE_PLATFORM_EMSCRIPTEN)
            _context->getExtensions()->printGLExtensions();
#endif
            _context->getExtensions()->printSupportedCompressedTextureFormats();

            _context->flushConfig();
        }

        if (m_config.multiContextPolicy == OpenGLMultiContextPolicy::Shared)
        {
            if (!m_sharedContext)
                m_sharedContext = _context;
        }


        Debug::log << "[Setup Config]" << endl;
        Debug::log << "\tVersion: " << _context->getConfig().majorVersion << "." << _context->getConfig().minorVersion << endl;
        Debug::log << "\tDepth bits: " << _context->getConfig().depthBits << endl;
        Debug::log << "\tStencil bits: " << _context->getConfig().stencilBits << endl;
        Debug::log << "\tAntialiasing level: " << _context->getConfig().antialiasingLevel << endl;


        MAZE_GL_MUTEX_SCOPED_LOCK(this);

        if (mzglGetIntegerv != nullptr)
        {
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_MAX_TEXTURE_SIZE, &m_textureMaxSize));
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_MAX_TEXTURE_IMAGE_UNITS, &m_textureUniformsMaxCount));
        }

        Debug::log << "GL_MAX_TEXTURE_SIZE: " << m_textureMaxSize << endl;
        Debug::log << "GL_MAX_TEXTURE_IMAGE_UNITS: " << m_textureUniformsMaxCount << endl;

        ShaderSystemOpenGL::Initialize(m_shaderSystem, getSharedPtr());

        processSystemInited();

        return true;
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3::setDefaultRenderContext(ContextOpenGLPtr const& _context)
    {
        RenderSystemOpenGL::setDefaultRenderContext(_context);

        if (_context)
        {
            if (true)
            {
                if (!m_systemInited)
                {
                    setupSystem(_context);
                }
            }
        }
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL3::processRenderWindowCreated(RenderWindowPtr const& _renderWindow)
    {
        if (!RenderSystemOpenGL::processRenderWindowCreated(_renderWindow))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL3::processRenderWindowDestroyed(RenderWindow* _renderWindow)
    {
        if (!RenderSystemOpenGL::processRenderWindowDestroyed(_renderWindow))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL3::clearCurrentRenderTarget(
        bool _colorBuffer,
        bool _depthBuffer)
    {
        MZGLbitfield mask = 0;

        if (_colorBuffer)
        {
            mask |= MAZE_GL_COLOR_BUFFER_BIT;

            ColorU32 const& clearColor = m_currentRenderTarget->getClearColor();
            getCurrentContext()->setClearColor(clearColor.toVec4F32());
        }

        if (_depthBuffer)
        {
            mask |= MAZE_GL_DEPTH_BUFFER_BIT;

            F32 clearDepth = m_currentRenderTarget->getClearDepth();
            getCurrentContext()->setClearDepth(clearDepth);

            getCurrentContext()->setDepthWriteEnabled(true);
        }

        getCurrentContext()->_validateIsCurrentGLContext();

        MAZE_GL_CALL(mzglClear(mask));
    }


} // namespace Maze
//////////////////////////////////////////
