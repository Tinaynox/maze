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
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)


//////////////////////////////////////////
#include "maze-render-system-opengl3/win/wgl/MazeContextOpenGL3WGL.hpp"
#include "maze-render-system-opengl3/win/wgl/MazeRenderSystemOpenGL3WGL.hpp"
#include "maze-render-system-opengl3/win/wgl/MazeExtensionsOpenGL3WGL.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    String GetErrorString(DWORD errorCode)
    {
        if (errorCode == 0)
            return String();

        LPSTR messageBuffer = nullptr;
        Size size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        String message(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);

        return message;
    }


    //////////////////////////////////////////
    // Class ContextOpenGL3WGL
    //
    //////////////////////////////////////////
    ContextOpenGL3WGL::ContextOpenGL3WGL()
        : m_pbuffer(0)
        , m_deviceContext(0)
        , m_openGLContext(0)
        , m_windowHandle(0)
        , m_ownsWindow(false)
    {
        
    }

    //////////////////////////////////////////
    ContextOpenGL3WGL::~ContextOpenGL3WGL()
    {
        destroyGLContext();
    }

    //////////////////////////////////////////
    ContextOpenGL3WGLPtr ContextOpenGL3WGL::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3WGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3WGL, object, init(_renderSystem, _window, _config));
        return object;
    }

    //////////////////////////////////////////
    ContextOpenGL3WGLPtr ContextOpenGL3WGL::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3WGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3WGL, object, init(_renderSystem, _width, _height, _config));
        return object;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& window,
        ContextOpenGLConfig* config)
    {
        if (!ContextOpenGL3::init(_renderSystem, window, config))
            return false;            

        return createGLContextWindow();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 width, U32 height,
        ContextOpenGLConfig* config)
    {
        if (!ContextOpenGL3::init(_renderSystem, width, height, config))
            return false;

        return createGLContextPixelBuffer();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::createGLContextWindow()
    {
        destroyGLContext();

        WindowManager* windowManager = WindowManager::GetInstancePtr();

        createSurface(
            m_windowRaw->castRaw<WindowWin>()->getHandle(),
            windowManager->getPrimaryDisplayVideoMode().bpp);

        if (m_deviceContext)
            createWGLContext();

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::createGLContextPixelBuffer()
    {
        destroyGLContext();

        WindowManager* windowManager = WindowManager::GetInstancePtr();
        U32 bpp = windowManager->getPrimaryDisplayVideoMode().bpp;

        createSurface(1, 1, bpp);

        if (m_deviceContext)
            createWGLContext();

        setVSync(0);

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::makeCurrent()
    {
        if (!m_deviceContext)
            return false;

        if (!m_openGLContext)
            return false;

        MAZE_GL_MUTEX_SCOPED_LOCK(m_renderSystemRaw);
        if (!wglMakeCurrent(m_deviceContext, m_openGLContext))
        {
            DWORD lastError = GetLastError();
            MAZE_ERROR(
                "Failed to make current context: %s",
                GetErrorString(lastError).c_str());
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::isValid()
    {
        if (!m_deviceContext)
            return false;

        if (!m_openGLContext)
            return false;

        if (m_windowRaw)
        {
            if (!m_windowRaw->isOpened() || m_windowRaw->getMinimized())
                return false;
        }

        return true;
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::setVSync(S32 _vsync)
    {
        RenderTarget* prevRenderTarget = getRenderSystemRaw()->getCurrentRenderTarget();
        ContextOpenGL* prevRenderContext = getRenderSystemRaw()->getCurrentContext();
        makeCurrentContext(true);
        

        if (mzwglSwapInterval)
        {
            if (mzwglSwapInterval(_vsync) == FALSE)
            {
                MAZE_WARNING("Setting vertical sync failed!");
            }
        }

        if (prevRenderContext)
        {
            getRenderSystemRaw()->makeCurrentContext(prevRenderContext);
            getRenderSystemRaw()->setCurrentRenderTarget(prevRenderTarget);
        }
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::ensureIfCurrent()
    {
        if (getRenderSystemRaw()->getCurrentContext() != this)
            return;

        if (wglGetCurrentContext() == m_openGLContext)
            return;

        if (wglGetCurrentDC() == m_deviceContext)
            return;

        makeCurrent();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::_validateIsCurrentGLContext()
    {
        HGLRC openGLContext = wglGetCurrentContext();
        
        if (openGLContext != m_openGLContext)
        {
            MAZE_ERROR("Current context is wrong!");
            return false;
        }
        
        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::_validateIsCurrentGLContextAndSurface()
    {
        if (!_validateIsCurrentGLContext())
            return false;

        HDC deviceContext = wglGetCurrentDC();
        if (deviceContext != m_deviceContext)
        {
            MAZE_ERROR("Current device is wrong!");
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::isSameGLContext(ContextOpenGL* _context)
    {
        if (m_openGLContext != _context->castRaw<ContextOpenGL3WGL>()->m_openGLContext)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3WGL::isSameGLSurface(ContextOpenGL* _context)
    {
        if (m_deviceContext != _context->castRaw<ContextOpenGL3WGL>()->m_deviceContext)
            return false;

        return true;
    }

    //////////////////////////////////////////
    ContextOpenGL3WGL::GlFunctionPointer ContextOpenGL3WGL::getFunction(CString _name)
    {
        GlFunctionPointer address = reinterpret_cast<GlFunctionPointer>(wglGetProcAddress(reinterpret_cast<LPCSTR>(_name)));

        if (address)
        {
            // Test whether the returned value is a valid error code
            ptrdiff_t errorCode = reinterpret_cast<ptrdiff_t>(address);

            if ((errorCode != -1) && (errorCode != 1) && (errorCode != 2) && (errorCode != 3))
                return address;
        }

        static HMODULE module = nullptr;

        if (!module)
            module = GetModuleHandleA("OpenGL32.dll");

        if (module)
            return reinterpret_cast<GlFunctionPointer>(GetProcAddress(module, reinterpret_cast<LPCSTR>(_name)));

        return 0;
    }


    //////////////////////////////////////////
    void ContextOpenGL3WGL::swapBuffers()
    {
        if (!m_deviceContext)
            return;

        if (!m_openGLContext)
            return;

#if (MAZE_DEBUG)
        _validateIsCurrentGLContextAndSurface();
#endif

        {
            MAZE_PROFILER_SCOPED_LOCK(SWAP);

            SwapBuffers(m_deviceContext);
        }
        

    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::createSurface(U32 _width, U32 _height, U32 _bitsPerPixel)
    {
        ContextOpenGLPtr const& sharedContext = getRenderSystemRaw()->cast<RenderSystemOpenGL>()->getSharedContext();
        HDC sharedDeviceContext = sharedContext ? sharedContext->cast<ContextOpenGL3WGL>()->m_deviceContext : nullptr;

        ExtensionsOpenGL3WGL* extensions = getAnyExtensions();

        // Check if the shared context already exists and pbuffers are supported
        if (sharedDeviceContext && extensions && extensions->hasGLExtension("WGL_ARB_pbuffer"))
        {
            S32 bestFormat = 0;
            
            RenderSystemOpenGL* renderSystemOpenGL = getRenderSystemRaw()->castRaw<RenderSystemOpenGL>();

            switch (renderSystemOpenGL->getConfig().multiContextPolicy)
            {
                case OpenGLMultiContextPolicy::Unified:
                {
                    if (renderSystemOpenGL->getDefaultContext())
                    {
                        ContextOpenGL3WGL* defaultContext = renderSystemOpenGL->getDefaultContext()->castRaw<ContextOpenGL3WGL>();
                        bestFormat = GetPixelFormat(defaultContext->m_deviceContext);
                    }
                    else
                    {
                        bestFormat = findBestPixelFormat(sharedDeviceContext, _bitsPerPixel, m_config, true);
                    }

                    break;
                }
                default:
                {
                    bestFormat = findBestPixelFormat(sharedDeviceContext, _bitsPerPixel, m_config, true);
                    break;
                }
            }        

            if (bestFormat > 0)
            {
                S32 attributes[] = { 0, 0 };

                m_pbuffer = mzwglCreatePbuffer(sharedDeviceContext, bestFormat, _width, _height, attributes);

                if (m_pbuffer)
                {
                    m_windowHandle = sharedContext->cast<ContextOpenGL3WGL>()->m_windowHandle;
                    m_deviceContext = mzwglGetPbufferDC(m_pbuffer);

                    if (!m_deviceContext)
                    {
                        mzwglDestroyPbuffer(m_pbuffer);
                        m_pbuffer = nullptr;
                    }
                }
            }
        }

        // If pbuffers are not available we use a hidden window as the off-screen surface to draw to
        if (!m_deviceContext)
        {
            // We can't create a memory DC, the resulting context wouldn't be compatible
            // with other contexts and thus wglShareLists would always fail

            // Create the hidden window
            m_windowHandle = CreateWindowA("STATIC", "", WS_POPUP | WS_DISABLED, 0, 0, _width, _height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
            ShowWindow(m_windowHandle, SW_HIDE);
            m_deviceContext = GetDC(m_windowHandle);

            m_ownsWindow = true;

            // Set the pixel format of the device context
            selectBestDevicePixelFormat(_bitsPerPixel);
        }

        MAZE_ERROR_IF(!m_deviceContext, "Context cannot be created!");

        // Update context settings from the selected pixel format
        // updateSettingsFromPixelFormat();
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::createSurface(HWND _handle, U32 _bitsPerPixel)
    {
        m_windowHandle = (HWND)_handle;
        m_deviceContext = GetDC(m_windowHandle);
        MAZE_ERROR_IF(!m_deviceContext, "Context cannot be created!");

        // Set the pixel format of the device context
        selectBestDevicePixelFormat(_bitsPerPixel);

        // Update context settings from the selected pixel format
        // updateSettingsFromPixelFormat();
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::createWGLContext()
    {
        RenderSystemOpenGL* renderSystemOpenGL = getRenderSystemRaw()->castRaw<RenderSystemOpenGL>();

        if (renderSystemOpenGL->getDefaultContext())
        {
            switch (renderSystemOpenGL->getConfig().multiContextPolicy)
            {
                case OpenGLMultiContextPolicy::Unified:
                {
                    ContextOpenGL3WGL* defaultContext = renderSystemOpenGL->getDefaultContext()->castRaw<ContextOpenGL3WGL>();

                    ContextOpenGL3WGLWPtr weakPtr = cast<ContextOpenGL3WGL>();
                    defaultContext->eventGLContextWillBeDestroyed.subscribe(
                        [weakPtr](ContextOpenGL* _context)
                        {
                            ContextOpenGL3WGLPtr thisContext = weakPtr.lock();
                            if (thisContext)
                                thisContext->destroyGLContext();
                        });

                    m_stateMachine = defaultContext->m_stateMachine;
                    m_openGLContext = defaultContext->m_openGLContext;
                    m_extensions = defaultContext->m_extensions;
                    m_modelMatricesArchitecture = defaultContext->m_modelMatricesArchitecture;
                    m_ownsOpenGLContext = false;
                    return;
                }
            }
        }
    
        m_ownsOpenGLContext = true;

        RenderTarget* prevRenderTarget = renderSystemOpenGL->getCurrentRenderTarget();
        ContextOpenGL* prevRenderContext = renderSystemOpenGL->getCurrentContext();
        ExtensionsOpenGL3WGL* extensions = getAnyExtensions();

        // Get the context to share display lists with
        ContextOpenGLPtr const& sharedContext = renderSystemOpenGL->getSharedContext();
        HGLRC sharedContextHandle = sharedContext ? sharedContext->cast<ContextOpenGL3WGL>()->m_openGLContext : nullptr;

        // Default version
        if (m_config.majorVersion == 0)
        {
            //m_settings.majorVersion = 1;
            //m_settings.minorVersion = 1;

            if (sharedContext)
            {
                // Try the new way first
                S32 majorVersion = 0;
                S32 minorVersion = 0;

                /*
                glGetIntegerv(MAZE_GL_MAJOR_VERSION, &majorVersion);
                glGetIntegerv(MAZE_GL_MINOR_VERSION, &minorVersion);

                if (glGetError() != MAZE_GL_INVALID_ENUM)
                {
                    m_settings.majorVersion = static_cast<U32>(majorVersion);
                    m_settings.minorVersion = static_cast<U32>(minorVersion);
                }
                */
            }
        }

        // Get a working copy of the context settings
        ContextOpenGLConfig config = m_config;
        S32 flags = m_config.flags;


        // Create the OpenGL context -- first try using wglCreateContextAttribsARB
        while (!m_openGLContext && m_config.majorVersion)
        {
            if (extensions && extensions->hasGLExtension("WGL_ARB_create_context"))
            {
                Vector<S32> attributes;

                // Check if the user requested a specific context version (anything > 1.1)
                if ((m_config.majorVersion > 1) || ((m_config.majorVersion == 1) && (m_config.minorVersion > 1)))
                {
                    attributes.push_back(MAZE_WGL_CONTEXT_MAJOR_VERSION_ARB);
                    attributes.push_back(m_config.majorVersion);
                    attributes.push_back(MAZE_WGL_CONTEXT_MINOR_VERSION_ARB);
                    attributes.push_back(m_config.minorVersion);
                }

                // Check if setting the profile is supported
                if (extensions->hasGLExtension("WGL_ARB_create_context_profile"))
                {
                    S32 profile = (m_config.flags & ContextOpenGLFlags::Core) ? MAZE_WGL_CONTEXT_CORE_PROFILE_BIT_ARB : MAZE_WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
                    S32 debug = (m_config.flags & ContextOpenGLFlags::Debug) ? MAZE_WGL_CONTEXT_DEBUG_BIT_ARB : 0;
                    //S32 profile = BFWGL_CONTEXT_CORE_PROFILE_BIT_ARB;
                    //S32 debug = 0;

                    attributes.push_back(MAZE_WGL_CONTEXT_PROFILE_MASK_ARB);
                    attributes.push_back(profile);
                    attributes.push_back(MAZE_WGL_CONTEXT_FLAGS_ARB);
                    attributes.push_back(debug);
                }
                else
                {
                    
                    if ((m_config.flags & ContextOpenGLFlags::Core) || (m_config.flags & ContextOpenGLFlags::Debug))
                        MAZE_WARNING("Selecting a profile during context creation is not supported,"
                                        "disabling comptibility and debug.");

                    m_config.flags = ContextOpenGLFlags::Default;
                    
                }

                attributes.push_back(0);
                attributes.push_back(0);

                {
                    MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                    m_openGLContext = mzwglCreateContextAttribs(m_deviceContext, sharedContextHandle, &attributes[0]);
                }
            }
            else
            {
                break;
            }

            if (!m_openGLContext)
            {
                
                if (m_config.flags != ContextOpenGLFlags::Default)
                {
                    m_config.flags = ContextOpenGLFlags::Default;
                }
                else
                if (m_config.minorVersion > 0)
                {
                    m_config.minorVersion--;
                    m_config.flags = flags;
                }
                else
                {
                    m_config.majorVersion--;
                    m_config.minorVersion = 9;

                    m_config.flags = flags;
                }
            }
        }

        do
        {
            if (!m_openGLContext)
            {
                m_config.flags = ContextOpenGLFlags::Default;

                {
                    MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                    m_openGLContext = wglCreateContext(m_deviceContext);
                }

                if (!m_openGLContext)
                {
                    MAZE_ERROR(
                        "Failed to create an OpenGL context for this window: %s",
                        GetErrorString(GetLastError()).c_str());
                    break;
                }

                if (sharedContextHandle)
                {
                    MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                    if (!wglShareLists(sharedContextHandle, m_openGLContext))
                        MAZE_ERROR("Failed to share the OpenGL context: %s",
                        GetErrorString(GetLastError()).c_str());
                }
            }

            processContextCreated();
        }
        while (0);


        getRenderSystemRaw()->makeCurrentContext(prevRenderContext);
        getRenderSystemRaw()->setCurrentRenderTarget(prevRenderTarget);

        createExtensions();

        m_stateMachine.reset(new StateMachineOpenGL(this));
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::createExtensions()
    {
        m_extensions = ExtensionsOpenGL3WGL::Create(this);

        if (getRenderSystemRaw()->getSystemInited())
        {
            m_extensions->loadGLExtensions();

            flushConfig();
        }
        else
        {
            getRenderSystemRaw()->eventSystemInited.subscribe(
                [&]()
                {
                    if (!m_extensions->isLoaded())
                        m_extensions->loadGLExtensions();

                    flushConfig();
                });
        }
    }

    //////////////////////////////////////////
    S32 ContextOpenGL3WGL::findBestPixelFormat(
        HDC _deviceContext,
        U32 _bitsPerPixel,
        ContextOpenGLConfig const& _config,
        bool _pbuffer)
    {
        ExtensionsOpenGL3WGL* extensions = getAnyExtensions();
        
        S32 bestFormat = 0;
        if (extensions && extensions->hasGLExtension("WGL_ARB_pixel_format"))
        {         
            S32 intAttributes[] =
            {
                MAZE_WGL_DRAW_TO_WINDOW_ARB, MAZE_GL_TRUE,
                MAZE_WGL_SUPPORT_OPENGL_ARB, MAZE_GL_TRUE,
                MAZE_WGL_DOUBLE_BUFFER_ARB,  MAZE_GL_TRUE,
                MAZE_WGL_PIXEL_TYPE_ARB,     MAZE_WGL_TYPE_RGBA_ARB,
                0,                      0
            };

            S32 formats[512];
            UINT nbFormats;
            bool isValid = false;
            
            {
                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                isValid = mzwglChoosePixelFormat(_deviceContext, intAttributes, nullptr, 512, formats, &nbFormats) != 0;
            }

            if (isValid && (nbFormats > 0))
            {
                S32 bestScore = 0x7FFFFFFF;
                for (UINT i = 0; i < nbFormats; ++i)
                {
                    S32 values[7];
                    S32 const attributes[] =
                    {
                        MAZE_WGL_RED_BITS_ARB,
                        MAZE_WGL_GREEN_BITS_ARB,
                        MAZE_WGL_BLUE_BITS_ARB,
                        MAZE_WGL_ALPHA_BITS_ARB,
                        MAZE_WGL_DEPTH_BITS_ARB,
                        MAZE_WGL_STENCIL_BITS_ARB,
                        MAZE_WGL_ACCELERATION_ARB
                    };

                    {
                        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                        if (!mzwglGetPixelFormatAttribiv(_deviceContext, formats[i], PFD_MAIN_PLANE, 7, attributes, values))
                        {
                            MAZE_ERROR(
                                "Failed to retrieve pixel format information: %s",
                                GetErrorString(GetLastError()).c_str());
                            break;
                        }
                    }

                    S32 sampleValues[2] = { 0, 0 };
                    if (extensions->hasGLExtension("WGL_ARB_multisample"))
                    {
                        S32 const sampleAttributes[] =
                        {
                            MAZE_WGL_SAMPLE_BUFFERS_ARB,
                            MAZE_WGL_SAMPLES_ARB
                        };

                        {
                            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                            if (!mzwglGetPixelFormatAttribiv(_deviceContext, formats[i], PFD_MAIN_PLANE, 2, sampleAttributes, sampleValues))
                            {
                                MAZE_ERROR( 
                                    "Failed to retrieve pixel format multisampling information: %s",
                                    GetErrorString(GetLastError()).c_str());
                                break;
                            }
                        }
                    }

                    S32 sRgbCapableValue = 0;
                    if (extensions->hasGLExtension("WGL_ARB_framebuffer_sRGB") || extensions->hasGLExtension("WGL_EXT_framebuffer_sRGB"))
                    {
                        S32 const sRgbCapableAttribute = MAZE_WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;

                        {
                            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                            if (!mzwglGetPixelFormatAttribiv(_deviceContext, formats[i], PFD_MAIN_PLANE, 1, &sRgbCapableAttribute, &sRgbCapableValue))
                            {
                                MAZE_ERROR(
                                    "Failed to retrieve pixel format sRGB capability information: ",
                                    GetErrorString(GetLastError()).c_str());
                                break;
                            }
                        }
                    }

                    if (_pbuffer)
                    {
                        S32 const pbufferAttributes[] =
                        {
                            MAZE_WGL_DRAW_TO_PBUFFER_ARB
                        };

                        int pbufferValue = 0;

                        {
                            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                            if (!mzwglGetPixelFormatAttribiv(_deviceContext, formats[i], PFD_MAIN_PLANE, 1, pbufferAttributes, &pbufferValue))
                            {
                                MAZE_ERROR(
                                    "Failed to retrieve pixel format pbuffer information: %s",
                                    GetErrorString(GetLastError()).c_str());
                                break;
                            }
                        }

                        if (pbufferValue != MAZE_GL_TRUE)
                            continue;
                    }

                    S32 color = values[0] + values[1] + values[2] + values[3];
                    S32 score = EvaluateFormat(_bitsPerPixel, _config, color, values[4], values[5], sampleValues[0] ? sampleValues[1] : 0, values[6] == MAZE_WGL_FULL_ACCELERATION_ARB, sRgbCapableValue == TRUE);
                    
                    if (score < bestScore)
                    {
                        bestScore  = score;
                        bestFormat = formats[i];
                    }
                }
            }
        }

        if (_pbuffer)
            return bestFormat;

        if (bestFormat == 0)
        {
            PIXELFORMATDESCRIPTOR descriptor;
            ZeroMemory(&descriptor, sizeof(descriptor));
            descriptor.nSize = sizeof(descriptor);
            descriptor.nVersion = 1;
            descriptor.iLayerType = PFD_MAIN_PLANE;
            descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            descriptor.iPixelType = PFD_TYPE_RGBA;
            descriptor.cColorBits = static_cast<BYTE>(_bitsPerPixel);
            descriptor.cDepthBits = static_cast<BYTE>(_config.depthBits);
            descriptor.cStencilBits = static_cast<BYTE>(_config.stencilBits);
            descriptor.cAlphaBits = _bitsPerPixel == 32 ? 8 : 0;

            bestFormat = ChoosePixelFormat(_deviceContext, &descriptor);
        }

        return bestFormat;
    }

    //////////////////////////////////////////
    S32 ContextOpenGL3WGL::findMaxAntialisingLevelSupport()
    {
        ExtensionsOpenGL3WGL* extensions = getAnyExtensions();

        S32 bestAntialisingLevelSupport = 0;
        if (extensions && extensions->hasGLExtension("WGL_ARB_pixel_format"))
        {
            S32 intAttributes[] =
            {
                MAZE_WGL_DRAW_TO_WINDOW_ARB, MAZE_GL_TRUE,
                MAZE_WGL_SUPPORT_OPENGL_ARB, MAZE_GL_TRUE,
                MAZE_WGL_DOUBLE_BUFFER_ARB,  MAZE_GL_TRUE,
                MAZE_WGL_PIXEL_TYPE_ARB,     MAZE_WGL_TYPE_RGBA_ARB,
                0,                      0
            };

            S32 formats[512];
            UINT nbFormats;
            bool isValid = false;

            {
                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                isValid = mzwglChoosePixelFormat(m_deviceContext, intAttributes, nullptr, 512, formats, &nbFormats) != 0;
            }

            if (isValid && (nbFormats > 0))
            {
                for (UINT i = 0; i < nbFormats; ++i)
                {
                    S32 values[7];
                    S32 const attributes[] =
                    {
                        MAZE_WGL_RED_BITS_ARB,
                        MAZE_WGL_GREEN_BITS_ARB,
                        MAZE_WGL_BLUE_BITS_ARB,
                        MAZE_WGL_ALPHA_BITS_ARB,
                        MAZE_WGL_DEPTH_BITS_ARB,
                        MAZE_WGL_STENCIL_BITS_ARB,
                        MAZE_WGL_ACCELERATION_ARB
                    };

                    {
                        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                        if (!mzwglGetPixelFormatAttribiv(m_deviceContext, formats[i], PFD_MAIN_PLANE, 7, attributes, values))
                        {
                            MAZE_ERROR(
                                "Failed to retrieve pixel format information: %s",
                                GetErrorString(GetLastError()).c_str());
                            break;
                        }
                    }

                    S32 sampleValues[2] = { 0, 0 };
                    if (extensions->hasGLExtension("WGL_ARB_multisample"))
                    {
                        S32 const sampleAttributes[] =
                        {
                            MAZE_WGL_SAMPLE_BUFFERS_ARB,
                            MAZE_WGL_SAMPLES_ARB
                        };

                        {
                            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                            if (!mzwglGetPixelFormatAttribiv(m_deviceContext, formats[i], PFD_MAIN_PLANE, 2, sampleAttributes, sampleValues))
                            {
                                MAZE_ERROR(
                                    "Failed to retrieve pixel format multisampling information: %s",
                                    GetErrorString(GetLastError()).c_str());
                                break;
                            }
                        }
                    }


                    S32 score = (sampleValues[0] ? sampleValues[1] : 0);

                    if (score >= bestAntialisingLevelSupport)
                        bestAntialisingLevelSupport = score;
                }
            }
        }

        return bestAntialisingLevelSupport;
    }

    ////////////////////////////////////
    void ContextOpenGL3WGL::selectBestDevicePixelFormat(U32 _bitsPerPixel)
    {
        S32 bestFormat = 0;
        
        RenderSystemOpenGL* renderSystemOpenGL = getRenderSystemRaw()->castRaw<RenderSystemOpenGL>();
        switch (renderSystemOpenGL->getConfig().multiContextPolicy)
        {
            case OpenGLMultiContextPolicy::Unified:
            {
                if (renderSystemOpenGL->getDefaultContext())
                {
                    ContextOpenGL3WGL* defaultContext = renderSystemOpenGL->getDefaultContext()->castRaw<ContextOpenGL3WGL>();
                    bestFormat = GetPixelFormat(defaultContext->m_deviceContext);
                }
                else
                {
                    bestFormat = findBestPixelFormat(m_deviceContext, _bitsPerPixel, m_config);
                }

                break;
            }
            default:
            {
                bestFormat = findBestPixelFormat(m_deviceContext, _bitsPerPixel, m_config);
                break;
            }
        }    

        if (bestFormat == 0)
        {
            MAZE_ERROR(
                "Failed to find a suitable pixel format for device context: %s",
                GetErrorString(GetLastError()).c_str());
            MAZE_ERROR("Cannot create OpenGL context");
            return;
        }

        setDevicePixelFormat(bestFormat);
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::setDevicePixelFormat(S32 _format)
    {
        PIXELFORMATDESCRIPTOR actualFormat;
        actualFormat.nSize = sizeof(actualFormat);
        actualFormat.nVersion = 1;
        DescribePixelFormat(m_deviceContext, _format, sizeof(actualFormat), &actualFormat);

        if (!SetPixelFormat(m_deviceContext, _format, &actualFormat))
        {
            MAZE_ERROR(
                "Failed to set pixel format for device context: %s",
                GetErrorString(GetLastError()).c_str());
            MAZE_ERROR("Cannot create OpenGL context");
            return;
        }
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::updateSettingsFromPixelFormat()
    {
        if (!m_stateMachine)
            return;

        S32 format = GetPixelFormat(m_deviceContext);

        PIXELFORMATDESCRIPTOR actualFormat;
        actualFormat.nSize    = sizeof(actualFormat);
        actualFormat.nVersion = 1;
        DescribePixelFormat(m_deviceContext, format, sizeof(actualFormat), &actualFormat);

        if (format == 0)
        {
            MAZE_ERROR("Failed to get selected pixel format");
            return;
        }

        ExtensionsOpenGL3WGL* extensions = getAnyExtensions();

        if (extensions && extensions->hasGLExtension("WGL_ARB_pixel_format"))
        {
            const S32 attributes[] = { MAZE_WGL_DEPTH_BITS_ARB, MAZE_WGL_STENCIL_BITS_ARB };
            S32 values[2];

            {
                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                if (mzwglGetPixelFormatAttribiv(m_deviceContext, format, PFD_MAIN_PLANE, 2, attributes, values))
                {
                    m_config.depthBits   = values[0];
                    m_config.stencilBits = values[1];
                }
                else
                {
                    MAZE_ERROR(
                        "Failed to retrieve pixel format information: %s",
                        GetErrorString(GetLastError()).c_str());
                    m_config.depthBits   = actualFormat.cDepthBits;
                    m_config.stencilBits = actualFormat.cStencilBits;
                }
            }

            if (extensions->hasGLExtension("WGL_ARB_multisample"))
            {
                const S32 sampleAttributes[] = { MAZE_WGL_SAMPLE_BUFFERS_ARB, MAZE_WGL_SAMPLES_ARB };
                S32 sampleValues[2];

                {
                    MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                    if (mzwglGetPixelFormatAttribiv(m_deviceContext, format, PFD_MAIN_PLANE, 2, sampleAttributes, sampleValues))
                    {
                        m_stateMachine->setAntialiasingLevelSupport(sampleValues[0] ? sampleValues[1] : 0);
                    }
                    else
                    {
                        MAZE_ERROR("Failed to retrieve pixel format multisampling information: ",
                            GetErrorString(GetLastError()).c_str());
                        m_stateMachine->setAntialiasingLevelSupport(0);
                    }
                }
            }
            else
            {
                m_stateMachine->setAntialiasingLevelSupport(0);
            }

            if (extensions->hasGLExtension("WGL_ARB_framebuffer_sRGB") || extensions->hasGLExtension("WGL_EXT_framebuffer_sRGB"))
            {
                const S32 sRgbCapableAttribute = MAZE_WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB;
                int sRgbCapableValue = 0;

                {
                    MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
                    if (mzwglGetPixelFormatAttribiv(m_deviceContext, format, PFD_MAIN_PLANE, 1, &sRgbCapableAttribute, &sRgbCapableValue))
                    {
                        m_stateMachine->setSRGBSupport(sRgbCapableValue == TRUE);
                    }
                    else
                    {
                        MAZE_ERROR("Failed to retrieve pixel format sRGB capability information: %s",
                            GetErrorString(GetLastError()).c_str());
                        m_stateMachine->setSRGBSupport(false);
                    }
                }
            }
            else
            {
                m_stateMachine->setSRGBSupport(false);
            }
        }
        else
        {
            m_config.depthBits = actualFormat.cDepthBits;
            m_config.stencilBits = actualFormat.cStencilBits;
            m_config.antialiasingLevel = 0;
        }
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::destroyGLContext()
    {
        if (m_deviceContext == nullptr)
            return;

        RenderSystemPtr renderSystem = m_renderSystemWeak.lock();
        if (!renderSystem)
            return;

        processContextWillBeDestroyed();

        RenderTarget* prevRenderTarget = getRenderSystemRaw()->getCurrentRenderTarget();
        ContextOpenGL* prevRenderContext = getRenderSystemRaw()->getCurrentContext();

        makeCurrentContext(false);

        if (m_openGLContext && m_ownsOpenGLContext)
        {
            if (wglGetCurrentContext() == m_openGLContext)
                wglMakeCurrent(nullptr, nullptr);

            if (!wglDeleteContext(m_openGLContext))
            {
                DWORD lastError = GetLastError();
                MAZE_ERROR(
                        "Failed to delete context: %s",
                        GetErrorString(lastError).c_str());
            }
            m_openGLContext = nullptr;
        }

        if (m_deviceContext)
        {
            if (m_pbuffer)
            {
                mzwglReleasePbufferDC(m_pbuffer, m_deviceContext);
                mzwglDestroyPbuffer(m_pbuffer);
            }
            else
            {
                ReleaseDC(m_windowHandle, m_deviceContext);
            }

            m_deviceContext = nullptr;
        }

        if (m_windowHandle && m_ownsWindow)
            DestroyWindow(m_windowHandle);

        m_windowHandle = nullptr;


        if (prevRenderContext != this)
        {
            getRenderSystemRaw()->makeCurrentContext(prevRenderContext);
            getRenderSystemRaw()->setCurrentRenderTarget(prevRenderTarget);
        }

        processContextDestroyed();
    }

    //////////////////////////////////////////
    void ContextOpenGL3WGL::flushConfig()
    {
        updateSettingsFromPixelFormat();

        RenderSystemOpenGL* renderSystem = getRenderSystemRaw();
        ExtensionsOpenGL3WGL* extensions = static_cast<ExtensionsOpenGL3WGL*>(getExtensionsRaw());

        RenderTarget* prevRenderTarget = renderSystem->getCurrentRenderTarget();
        ContextOpenGL* prevRenderContext = renderSystem->getCurrentContext();

        
        makeCurrentContext(true);
        

        if (m_config.majorVersion == 0 && m_config.minorVersion == 0)
        {
            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());

            S32 majorVersion = 0;
            S32 minorVersion = 0;
        
            if (mzglGetIntegerv)
            {
                MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_MAJOR_VERSION, &majorVersion));
                MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_MINOR_VERSION, &minorVersion));
            }


            if (mzglGetIntegerv && mzglGetError() != MAZE_GL_INVALID_ENUM)
            {
                m_config.majorVersion = static_cast<U32>(majorVersion);
                m_config.minorVersion = static_cast<U32>(minorVersion);
            }
            else
            {
                MZGLubyte const* version = nullptr;
                if (mzglGetString)
                    version = mzglGetString(MAZE_GL_VERSION);

                if (version)
                {
                    m_config.majorVersion = version[0] - '0';
                    m_config.minorVersion = version[2] - '0';
                }
                else
                {
                    m_config.majorVersion = 0;
                    m_config.minorVersion = 0;
                }
            }
        }

        if (mzglGetIntegerv && m_config.majorVersion >= 3)
        {
            MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());
            
            S32 flags = 0;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_CONTEXT_FLAGS, &flags));

            if (flags & MAZE_GL_CONTEXT_FLAG_DEBUG_BIT)
            {
                m_config.flags |= ContextOpenGLFlags::Debug;
            }

            if ((m_config.majorVersion == 3) && (m_config.minorVersion == 1))
            {
                m_config.flags |= ContextOpenGLFlags::Core;

                if (extensions && extensions->hasGLExtension("GL_ARB_compatibility"))
                {
                    m_config.flags &= ~static_cast<U32>(ContextOpenGLFlags::Core);
                }
                
            }
            else 
            if ((m_config.majorVersion > 3) || (m_config.minorVersion >= 2))
            {
                S32 profile = 0;
                MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_CONTEXT_PROFILE_MASK, &profile));

                if (profile & MAZE_GL_CONTEXT_CORE_PROFILE_BIT)
                {
                    m_config.flags |= ContextOpenGLFlags::Core;
                }
            }
        }
        
        if (m_stateMachine && m_extensions && m_extensions->isLoaded())
        {
            m_maxAntialiasingLevelSupport = findMaxAntialisingLevelSupport();

            if ((m_config.antialiasingLevel > 0) && m_stateMachine->getAntialiasingLevelSupport() > 0)
            {
                m_stateMachine->setMultiSampleEnabled(true);
            }
            else
            {
                m_config.antialiasingLevel = 0;
            }

            if (m_config.sRgbCapable && m_stateMachine->getSRGBSupport())
            {
                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemRaw());

                MAZE_GL_CALL(mzglEnable(MAZE_GL_FRAMEBUFFER_SRGB));

                GLboolean isSRGBEnabled = MAZE_GL_FALSE;
                MAZE_GL_CALL(isSRGBEnabled = mzglIsEnabled(MAZE_GL_FRAMEBUFFER_SRGB));
                if (isSRGBEnabled == MAZE_GL_FALSE)
                {
                    MAZE_WARNING("Failed to enable GL_FRAMEBUFFER_SRGB");
                    m_config.sRgbCapable = false;
                }
            }
            else
            {
                m_config.sRgbCapable = false;
            }
        }
    

        if (prevRenderContext != this)
        {
            getRenderSystemRaw()->makeCurrentContext(prevRenderContext);
            getRenderSystemRaw()->setCurrentRenderTarget(prevRenderTarget);
        }
    }

    //////////////////////////////////////////
    ExtensionsOpenGL3WGL* ContextOpenGL3WGL::getAnyExtensions()
    {
        ExtensionsOpenGL3WGL* extensions = static_cast<ExtensionsOpenGL3WGL*>(getExtensionsRaw());
        if (!extensions)
        {
            ContextOpenGLPtr const& sharedContext = getRenderSystemRaw()->getSharedContext();
            if (sharedContext)
                extensions = static_cast<ExtensionsOpenGL3WGL*>(sharedContext->getExtensionsRaw());
        }

        if (!extensions)
        {
            ContextOpenGLPtr const& dummyContext = getRenderSystemRaw()->getDummyContext();
            if (dummyContext)
                extensions = static_cast<ExtensionsOpenGL3WGL*>(dummyContext->getExtensionsRaw());
        }

        return extensions;
    }


} // namespace Maze
//////////////////////////////////////////


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
//////////////////////////////////////////
