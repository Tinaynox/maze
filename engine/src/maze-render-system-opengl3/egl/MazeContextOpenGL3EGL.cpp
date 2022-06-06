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


//////////////////////////////////////////
#if (  (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)    \
    || (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN) \
    || (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))


//////////////////////////////////////////
#include "maze-render-system-opengl3/egl/MazeContextOpenGL3EGL.hpp"
#include "maze-render-system-opengl3/egl/MazeRenderSystemOpenGL3EGL.hpp"
#include "maze-render-system-opengl3/egl/MazeExtensionsOpenGL3EGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeSystemManager)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL3_API bool EGLCheckOpenGLError(CString _file, U32 _line, CString _expression)
    {
        EGLint errorCode = eglGetError();
        if (errorCode == EGL_SUCCESS)
            return false;
    
        String fileString(_file);
        String error = "unknown error";
        String description  = "no description";

        switch (errorCode)
        {
            case EGL_NOT_INITIALIZED:
            {
                error = "EGL_NOT_INITIALIZED";
                description = "EGL is not initialized, or could not be initialized, for the specified display";
                break;
            }

            case EGL_BAD_ACCESS:
            {
                error = "EGL_BAD_ACCESS";
                description = "EGL cannot access a requested asset (for example, a context is bound in another thread)";
                break;
            }

            case EGL_BAD_ALLOC:
            {
                error = "EGL_BAD_ALLOC";
                description = "EGL failed to allocate assets for the requested operation";
                break;
            }
            case EGL_BAD_ATTRIBUTE:
            {
                error = "EGL_BAD_ATTRIBUTE";
                description = "an unrecognized attribute or attribute value was passed in an attribute list";
                break;
            }

            case EGL_BAD_CONTEXT:
            {
                error = "EGL_BAD_CONTEXT";
                description = "an EGLContext argument does not name a valid EGLContext";
                break;
            }

            case EGL_BAD_CONFIG:
            {
                error = "EGL_BAD_CONFIG";
                description = "an EGLConfig argument does not name a valid EGLConfig";
                break;
            }

            case EGL_BAD_CURRENT_SURFACE:
            {
                error = "EGL_BAD_CURRENT_SURFACE";
                description = "the current surface of the calling thread is a window, pbuffer, or pixmap that is no longer valid";
                break;
            }

            case EGL_BAD_DISPLAY:
            {
                error = "EGL_BAD_DISPLAY";
                description = "an EGLDisplay argument does not name a valid EGLDisplay; or, EGL is not initialized on the specified EGLDisplay";
                break;
            }

            case EGL_BAD_SURFACE:
            {
                error = "EGL_BAD_SURFACE";
                description = "an EGLSurface argument does not name a valid surface (window, pbuffer, or pixmap) configured for rendering";
                break;
            }

            case EGL_BAD_MATCH:
            {
                error = "EGL_BAD_MATCH";
                description = "arguments are inconsistent; for example, an otherwise valid context requires buffers (e.g. depth or stencil) not allocated by an otherwise valid surface";
                break;
            }

            case EGL_BAD_PARAMETER:
            {
                error = "EGL_BAD_PARAMETER";
                description = "one or more argument values are invalid";
                break;
            }

            case EGL_BAD_NATIVE_PIXMAP:
            {
                error = "EGL_BAD_NATIVE_PIXMAP";
                description = "an EGLNativePixmapType argument does not refer to a valid native pixmap";
                break;
            }

            case EGL_BAD_NATIVE_WINDOW:
            {
                error = "EGL_BAD_NATIVE_WINDOW";
                description = "an EGLNativeWindowType argument does not refer to a valid native window";
                break;
            }

            case EGL_CONTEXT_LOST:
            {
                error = "EGL_CONTEXT_LOST";
                description = "a power management event has occurred. The application must destroy all contexts and reinitialize client API state and objects to continue rendering";
                break;
            }
        }

        MAZE_ERROR("%s(%u): %s, %s",
                fileString.substr(fileString.find_last_of("\\/") + 1).c_str(),
                _line,
                error.c_str(),
                description.c_str());

        return true;
    }

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL3_API EGLNativeWindowType FetchNativeWindowHandle(WindowPtr const& _window)
    {
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)

        return static_cast<EGLNativeWindowType>(_window->castRaw< WindowWin >()->getHandle());

#elif (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)

        // Dummy window
        return EGLNativeWindowType();

#elif (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)

        SystemManagerAndroid* systemManager = static_cast<SystemManagerAndroid*>(SystemManager::GetInstancePtr());
        ANativeWindow* nativeWindow = systemManager->getNativeWindow();

        return static_cast<EGLNativeWindowType>(nativeWindow);

#else

        MAZE_ERROR("Unsupported NativeWindow!");
        return 0;

#endif
    }


    EGLDisplay& GetInitializedEGLDisplay()
    {
        static EGLDisplay display = EGL_NO_DISPLAY;

#if (                                                  \
       (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)          \
    || (MAZE_PLATFORM == MAZE_PLATFORM_OSX)            \
    || (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)        \
    || (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)        \
    || (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN))

        if (display == EGL_NO_DISPLAY)
        {
            MAZE_EGL_CALL(display = eglGetDisplay(EGL_DEFAULT_DISPLAY));
            MAZE_EGL_CALL(eglInitialize(display, NULL, NULL));
        }

        return display;
#else
        MAZE_ERROR("Not supported platform!");
#endif

        return display;
    }


    //////////////////////////////////////////
    // Class ContextOpenGL3EGL
    //
    //////////////////////////////////////////
    ContextOpenGL3EGL::ContextOpenGL3EGL()
        : m_eglDisplay(EGL_NO_DISPLAY)
        , m_eglContext(EGL_NO_CONTEXT)
        , m_eglSurface(EGL_NO_SURFACE)
        , m_eglConfig(nullptr)
    {
        
    }

    //////////////////////////////////////////
    ContextOpenGL3EGL::~ContextOpenGL3EGL()
    {
        destroyGLContext();
    }

    //////////////////////////////////////////
    ContextOpenGL3EGLPtr ContextOpenGL3EGL::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3EGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3EGL, object, init(_renderSystem, _window, _config));
        return object;
    }

    //////////////////////////////////////////
    ContextOpenGL3EGLPtr ContextOpenGL3EGL::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3EGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3EGL, object, init(_renderSystem, _width, _height, _config));
        return object;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        if (!ContextOpenGL3::init(_renderSystem, _window, _config))
            return false;

        return createGLContextWindow();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        if (!ContextOpenGL3::init(_renderSystem, _width, _height, _config))
            return false;

        return createGLContextPixelBuffer();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::createGLContextWindow()
    {
        destroyGLContext();

        WindowManager* windowManager = WindowManager::GetInstancePtr();
        Maze::WindowVideoMode windowVideoMode = windowManager->getPrimaryDisplayVideoMode();

        m_eglDisplay = GetInitializedEGLDisplay();

        m_eglConfig = fetchBestEGLConfig(
            m_eglDisplay,
            windowVideoMode.bpp,
            m_config);

        EGLNativeWindowType nativeWindowHandle = FetchNativeWindowHandle(m_windowRaw->getSharedPtr());
        createSurface(nativeWindowHandle);

        createEGLContext();

        flushConfig();

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::createGLContextPixelBuffer()
    {
        destroyGLContext();

#if (MAZE_PLATFORM != MAZE_PLATFORM_EMSCRIPTEN)

        WindowManager* windowManager = WindowManager::GetInstancePtr();
        Maze::WindowVideoMode windowVideoMode = windowManager->getPrimaryDisplayVideoMode();

        m_eglDisplay = GetInitializedEGLDisplay();

        m_eglConfig = fetchBestEGLConfig(
            m_eglDisplay,
            windowVideoMode.bpp,
            m_config);

        EGLint attribList[] = {
            EGL_WIDTH, static_cast<EGLint>(m_stateMachine->getPixelBufferSize().x),
            EGL_HEIGHT,static_cast<EGLint>(m_stateMachine->getPixelBufferSize().y),
            EGL_NONE
        };

        MAZE_EGL_CALL(m_eglSurface = eglCreatePbufferSurface(m_eglDisplay, m_eglConfig, attribList));

        createEGLContext();

        flushConfig();

        setVSync(0);


        return true;

#else
        MAZE_ERROR("PixelBufferContext is not supported on the current platform!");
        return false;

#endif
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::makeCurrent()
    {
        if (m_eglSurface == EGL_NO_SURFACE)
            return false;

        MAZE_EGL_CALL(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext));

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::isValid()
    {
        return m_eglContext != EGL_NO_CONTEXT;
    }

    //////////////////////////////////////////
    void ContextOpenGL3EGL::setVSync(S32 _vsync)
    {
        RenderSystemPtr renderSystem = m_renderSystemWeak.lock();
        MAZE_ERROR_RETURN_IF(!renderSystem, "Engine is not initialized yet!");

        makeCurrentContext(true);

        MAZE_EGL_CALL(eglSwapInterval(m_eglDisplay, _vsync));
    }

    //////////////////////////////////////////
    void ContextOpenGL3EGL::ensureIfCurrent()
    {
        if (m_eglContext != eglGetCurrentContext())
        {
            makeCurrent();
        }
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::_validateIsCurrentGLContext()
    {
        MAZE_ERROR_RETURN_VALUE_IF(m_eglContext != eglGetCurrentContext(), false, "Context is not wrong!");

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::_validateIsCurrentGLContextAndSurface()
    {
        if (!_validateIsCurrentGLContext())
            return false;

        MAZE_ERROR_RETURN_VALUE_IF(m_eglDisplay != eglGetCurrentDisplay(), false, "Display is not wrong!");

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::isSameGLContext(ContextOpenGL* _context)
    {
        if (m_eglContext != _context->castRaw<ContextOpenGL3EGL>()->m_eglContext)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3EGL::isSameGLSurface(ContextOpenGL* _context)
    {
        if (m_eglDisplay != _context->castRaw<ContextOpenGL3EGL>()->m_eglDisplay)
            return false;

        return true;
    }

    //////////////////////////////////////////
    ContextOpenGL3EGL::GlFunctionPointer ContextOpenGL3EGL::getFunction(CString _name)
    {
        return eglGetProcAddress(_name);
    }


    //////////////////////////////////////////
    void ContextOpenGL3EGL::swapBuffers()
    {
        if (m_eglSurface != EGL_NO_SURFACE)
        {
            MAZE_PROFILER_SCOPED_LOCK(SWAP);
            MAZE_EGL_CALL(eglSwapBuffers(m_eglDisplay, m_eglSurface));
        }

    }

    //////////////////////////////////////////
    void ContextOpenGL3EGL::destroyGLContext()
    {
        RenderTarget* prevRenderTarget = getRenderSystemRaw()->getCurrentRenderTarget();
        ContextOpenGL* prevRenderContext = getRenderSystemRaw()->getCurrentContext();

        processContextWillBeDestroyed();

        makeCurrentContext(false);

        // Destroy context
        if (m_eglContext != EGL_NO_CONTEXT)
        {
            MAZE_EGL_CALL(eglDestroyContext(m_eglDisplay, m_eglContext));
            m_eglContext = EGL_NO_CONTEXT;
        }

        // Destroy surface
        if (m_eglSurface != EGL_NO_SURFACE)
        {
            MAZE_EGL_CALL(eglDestroySurface(m_eglDisplay, m_eglSurface));
            m_eglSurface = EGL_NO_SURFACE;
        }

        m_eglDisplay = EGL_NO_DISPLAY;
        m_eglConfig = nullptr;

        if (prevRenderContext != this)
        {
            getRenderSystemRaw()->makeCurrentContext(prevRenderContext);
            getRenderSystemRaw()->setCurrentRenderTarget(prevRenderTarget);
        }

        processContextDestroyed();
    }

    //////////////////////////////////////////
    void ContextOpenGL3EGL::flushConfig()
    {
        RenderSystemOpenGL* renderSystem = getRenderSystemRaw();
        ExtensionsOpenGL3EGL* extensions = static_cast<ExtensionsOpenGL3EGL*>(getExtensionsRaw());

        MAZE_ERROR_RETURN_IF(!m_eglConfig, "No config!");

        EGLint tmp;
    
        MAZE_EGL_CALL(eglGetConfigAttrib(m_eglDisplay, m_eglConfig, EGL_CONFORMANT, &tmp));
        MAZE_EGL_CALL(eglGetConfigAttrib(m_eglDisplay, m_eglConfig, EGL_RENDERABLE_TYPE, &tmp));

        MAZE_EGL_CALL(eglGetConfigAttrib(m_eglDisplay, m_eglConfig, EGL_DEPTH_SIZE, &tmp));
        m_config.depthBits = tmp;
    
        MAZE_EGL_CALL(eglGetConfigAttrib(m_eglDisplay, m_eglConfig, EGL_STENCIL_SIZE, &tmp));
        m_config.stencilBits = tmp;
    
        MAZE_EGL_CALL(eglGetConfigAttrib(m_eglDisplay, m_eglConfig, EGL_SAMPLES, &tmp));
        m_config.antialiasingLevel = tmp;
    
#if (  (MAZE_PLATFORM != MAZE_PLATFORM_EMSCRIPTEN) \
    && (MAZE_PLATFORM != MAZE_PLATFORM_ANDROID))
        MAZE_EGL_CALL(eglQueryContext(m_eglDisplay, m_eglConfig, EGL_CONTEXT_CLIENT_VERSION, &tmp));
        m_config.majorVersion = tmp;
        m_config.minorVersion = 0;
#endif

        m_maxAntialiasingLevelSupport = m_config.antialiasingLevel;
        if (m_stateMachine)
        {
            m_stateMachine->setMultiSampleEnabled(m_maxAntialiasingLevelSupport > 0);
            m_stateMachine->setAntialiasingLevelSupport(m_maxAntialiasingLevelSupport);
        }
    }

    //////////////////////////////////////////
    EGLConfig ContextOpenGL3EGL::fetchBestEGLConfig(
        EGLDisplay _eglDisplay,
        U32 _bitsPerPixel,
        ContextOpenGLConfig& _config)
    {
        EGLint esVersion = EGL_OPENGL_BIT;

        // #TODO
        _config.flags |= ContextOpenGLFlags::EmbeddedSystems;

        if (_config.flags & ContextOpenGLFlags::EmbeddedSystems)
        {
            if (_config.majorVersion == 1)
            {
                esVersion = EGL_OPENGL_ES_BIT;
            }
            else
            if (_config.majorVersion == 2)
            {
                esVersion = EGL_OPENGL_ES2_BIT;
            }
            else
            {
                _config.majorVersion = 3;
                esVersion = EGL_OPENGL_ES3_BIT;
            }
        }
        else
        {
            esVersion = EGL_OPENGL_BIT;
        }

        EGLint configCount;
        EGLConfig configs[1] = { 0 };

        S32 antialiasingLevel = _config.antialiasingLevel;

        EGLBoolean result;
        bool repeatTry = false;
        do
        {
            const EGLint attributes[] = 
            {    
                EGL_BUFFER_SIZE,            static_cast<EGLint>(_bitsPerPixel),
                EGL_DEPTH_SIZE,             static_cast<EGLint>(_config.depthBits),
                EGL_STENCIL_SIZE,           static_cast<EGLint>(_config.stencilBits),
                EGL_SAMPLES,                static_cast<EGLint>(antialiasingLevel),
                EGL_SAMPLE_BUFFERS,         static_cast<EGLint>(antialiasingLevel > 0 ? 1 : 0),
                EGL_SURFACE_TYPE,           EGL_WINDOW_BIT | EGL_PBUFFER_BIT,
                EGL_RENDERABLE_TYPE,        esVersion,
                EGL_NONE
            };
            
            MAZE_EGL_CALL(result = eglChooseConfig(_eglDisplay, attributes, configs, 1, &configCount));
            
            if (configCount == 0)
            {
                if (antialiasingLevel > 0)
                {
                    antialiasingLevel /= 2;
                    repeatTry = true;
                }
            }
        }
        while (configCount == 0 && repeatTry);
        
        MAZE_ERROR_IF(
            configCount == 0,
            "Config count is ZERO.\n"
            "esVersion=%d\n"
            "bitsPerPixel=%d\n"
            "depthBits=%d\n"
            "stencilBits=%d\n"
            "antialiasingLevel=%d\n",
            esVersion,
            _bitsPerPixel,
            _config.depthBits,
            _config.stencilBits,
            _config.antialiasingLevel);
        
        return configs[0];
    }

    //////////////////////////////////////////
    void ContextOpenGL3EGL::createEGLContext()
    {
        // #TODO:
        EGLint contextVersion[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
        };


        ContextOpenGLPtr const& sharedContext = getRenderSystemRaw()->cast<RenderSystemOpenGL>()->getSharedContext();
        EGLContext toShared = sharedContext ? sharedContext->cast<ContextOpenGL3EGL>()->m_eglContext : EGL_NO_CONTEXT;
        
        do
        {
            MAZE_EGL_CALL(m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, toShared, contextVersion));

            if (!m_eglContext)
            {
                --contextVersion[1];
            }
        }
        while (!m_eglContext);

        m_config.majorVersion = contextVersion[1];

        createExtensions();
        m_stateMachine.reset(new StateMachineOpenGL(this));
    }

    //////////////////////////////////////////
    void ContextOpenGL3EGL::createExtensions()
    {
        m_extensions = ExtensionsOpenGL3EGL::Create(this);

        if (getRenderSystemRaw()->getSystemInited())
        {
            m_extensions->loadGLExtensions();
        }
        else
        {
            getRenderSystemRaw()->eventSystemInited.subscribe(
                [&]()
                {
                    m_extensions->loadGLExtensions();
                });
        }
    }

    //////////////////////////////////////////
    void ContextOpenGL3EGL::createSurface(EGLNativeWindowType _window)
    {
        MAZE_EGL_CALL(m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, _window, NULL));
    }


    //////////////////////////////////////////
    void ContextOpenGL3EGL::destroySurface()
    {
        makeCurrentContext(false);

        MAZE_EGL_CALL(eglDestroySurface(m_eglDisplay, m_eglSurface));
        m_eglSurface = EGL_NO_SURFACE;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // if (  (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)       \
       //    || (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)    \
       //    || (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))
//////////////////////////////////////////
