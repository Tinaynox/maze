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
#if (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)

#define MAZE_GLAD_GLX_IMPLEMENTATION
#include "maze-render-system-opengl3/linux/glx/MazeContextOpenGL3GLX.hpp"
#include "maze-render-system-opengl3/linux/glx/MazeRenderSystemOpenGL3GLX.hpp"
#include "maze-render-system-opengl3/linux/glx/MazeExtensionsOpenGL3GLX.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/managers, MazeSystemManager)
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)


#if (MAZE_DEBUG_GL)
    #define MAZE_DEBUG_GLX (1)
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    Mutex g_glxErrorMutex;
    bool g_glxErrorOccurred = false;


    //////////////////////////////////////////
    void EnsureExtensionsInit(::Display* _display, S32 _screen)
    {
        static bool s_initialized = false;
        if (!s_initialized)
        {
            s_initialized = true;

            gladLoaderLoadGLX(_display, _screen);

            gladLoadGLX(_display, _screen, ContextOpenGL3GLX::GetFunction);
        }
    }

    //////////////////////////////////////////
    S32 HandleXError(::Display* _display, XErrorEvent* _errorEvent)
    {
        g_glxErrorOccurred = true;
        return 0;
    }

    //////////////////////////////////////////
    // Class GLXErrorHandler
    //
    //////////////////////////////////////////
    class GLXErrorHandler
    {
    public:

        //////////////////////////////////////////
        GLXErrorHandler(::Display* _display)
            : m_lock(g_glxErrorMutex)
            , m_display(_display)
        {
            g_glxErrorOccurred = false;
            m_previousHandler = XSetErrorHandler(HandleXError);
        }

        //////////////////////////////////////////
        ~GLXErrorHandler()
        {
            XSync(m_display, False);
            XSetErrorHandler(m_previousHandler);
        }

    private:
        MutexScopeLock m_lock;
        ::Display* m_display;
        S32 (*m_previousHandler)(::Display*, XErrorEvent*);
    };


    //////////////////////////////////////////
    // Class ContextOpenGL3GLX
    //
    //////////////////////////////////////////
    ContextOpenGL3GLX::ContextOpenGL3GLX()
        : m_xDisplay(0)
        , m_xWindow(0)
        , m_glxContext(0)
        , m_glxPbuffer(0)
        , m_ownsWindow(false)
    {
        m_xDisplay = RetainSharedXDisplay();

        EnsureExtensionsInit(m_xDisplay, DefaultScreen(m_xDisplay));
    }

    //////////////////////////////////////////
    ContextOpenGL3GLX::~ContextOpenGL3GLX()
    {
        destroyGLContext();

        ReleaseSharedXDisplay(m_xDisplay);
    }

    //////////////////////////////////////////
    ContextOpenGL3GLXPtr ContextOpenGL3GLX::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3GLXPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3GLX, object, init(_renderSystem, _window, _config));
        return object;
    }

    //////////////////////////////////////////
    ContextOpenGL3GLXPtr ContextOpenGL3GLX::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3GLXPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3GLX, object, init(_renderSystem, _width, _height, _config));
        return object;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3GLX::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        if (!ContextOpenGL3::init(_renderSystem, _window, _config))
            return false;

        return createGLContextWindow();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3GLX::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        if (!ContextOpenGL3::init(_renderSystem, _width, _height, _config))
            return false;

        return createGLContextPixelBuffer();
    }
    
    //////////////////////////////////////////
    bool ContextOpenGL3GLX::createGLContextWindow()
    {
        destroyGLContext();
        
        WindowManager* windowManager = WindowManager::GetInstancePtr();
        Maze::WindowVideoMode windowVideoMode = windowManager->getPrimaryDisplayVideoMode();

        createSurface(static_cast<::Window>(m_windowRaw->castRaw<WindowLinux>()->getXWindow()));

        createGLXContext();

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3GLX::createGLContextPixelBuffer()
    {
        destroyGLContext();
        
        WindowManager* windowManager = WindowManager::GetInstancePtr();
        Maze::WindowVideoMode windowVideoMode = windowManager->getPrimaryDisplayVideoMode();

        createSurface(
            m_pixelBuffer.x,
            m_pixelBuffer.y,
            windowVideoMode.bpp);

        createGLXContext();

        return true;
    }
    
    //////////////////////////////////////////
    bool ContextOpenGL3GLX::makeCurrent()
    {
        if (!m_glxContext)
            return false;

#if (MAZE_DEBUG_GLX)
        GLXErrorHandler handler(m_xDisplay);
#endif

        bool result = false;

        if (true)
        {
            if (m_glxPbuffer)
            {
                result = glXMakeContextCurrent(m_xDisplay, m_glxPbuffer, m_glxPbuffer, m_glxContext);
            }
            else
            if (m_xWindow)
            {
                result = glXMakeCurrent(m_xDisplay, m_xWindow, m_glxContext);
            }
        }
        // else
        // {
        //    result = glXMakeCurrent(m_xDisplay, 0, NULL);
        // }

#if (MAZE_DEBUG_GLX)
        if (g_glxErrorOccurred)
        {
            MAZE_ERROR("GLX error in GlxContext::makeCurrent()");
        }
#endif

        return result;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3GLX::isValid()
    {
        if (!m_glxContext)
            return false;

        return true;
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::setVSync(S32 _vsync)
    {
        S32 result = 0;

        if (MAZE_GLAD_GLX_EXT_swap_control)
        {
            glXSwapIntervalEXT(m_xDisplay, m_glxPbuffer ? m_glxPbuffer : m_xWindow, _vsync);
        }
        else
        if (MAZE_GLAD_GLX_MESA_swap_control)
        {
            result = glXSwapIntervalMESA(_vsync);
        }
        else
        if (MAZE_GLAD_GLX_SGI_swap_control)
        {
            result = glXSwapIntervalSGI(_vsync);
        }
        else
        {
            static bool s_warned = false;

            if (!s_warned)
            {
                MAZE_WARNING("Setting vertical sync not supported");
                s_warned = true;
            }
        }

        if (result != 0)
        {
            MAZE_ERROR("Setting vertical sync failed");
        }
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::ensureIfCurrent()
    {
        if (glXGetCurrentContext() != m_glxContext)
        {
            makeCurrentContext(true);
        }
    }

    //////////////////////////////////////////
    bool ContextOpenGL3GLX::_validateIsCurrent()
    {
        if (glXGetCurrentContext() != m_glxContext)
        {
            MAZE_ERROR("Context is not current!");
            makeCurrentContext(true);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////
    ContextOpenGL3GLX::GlFunctionPointer ContextOpenGL3GLX::getFunction(CString _name)
    {
        return GetFunction(_name);
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::swapBuffers()
    {
#if (MAZE_DEBUG_GLX)
        GLXErrorHandler handler(m_xDisplay);
#endif

        if (m_glxPbuffer)
        {
            glXSwapBuffers(m_xDisplay, m_glxPbuffer);
        }
        else
        if (m_xWindow)
        {
            glXSwapBuffers(m_xDisplay, m_xWindow);
        }

#if (MAZE_DEBUG_GLX)
        if (g_glxErrorOccurred)
        {
            MAZE_ERROR("GLX error in ContextOpenGL3GLX::swapBuffers()");
        }
#endif

    }
    
    //////////////////////////////////////////
    void ContextOpenGL3GLX::destroyGLContext()
    {
        processContextWillBeDestroyed();
        
        if (m_glxContext)
        {

#if (MAZE_DEBUG_GLX)
            GLXErrorHandler handler(m_xDisplay);
#endif

            makeCurrentContext( false );
            if (glXGetCurrentContext() == m_glxContext)
            {
                glXMakeCurrent(m_xDisplay, 0, NULL);
            }

            glXDestroyContext(m_xDisplay, m_glxContext);
            m_glxContext = nullptr;

#if (MAZE_DEBUG_GLX)
            if (g_glxErrorOccurred)
            {
                MAZE_ERROR("GLX error in ContextOpenGL3GLX::destroyContext()");
            }
#endif

        }

        if (m_glxPbuffer)
        {
            glXDestroyPbuffer(m_xDisplay, m_glxPbuffer);
            m_glxPbuffer = 0;
        }

        if (m_xWindow && m_ownsWindow)
        {
            XDestroyWindow(m_xDisplay, m_xWindow);
            XFlush(m_xDisplay);

            m_xWindow = 0;
            m_ownsWindow = false;
        }
        
        processContextDestroyed();
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::flushConfig()
    {

    }

    //////////////////////////////////////////
    ContextOpenGL3GLX::GlFunctionPointer ContextOpenGL3GLX::GetFunction(CString _name)
    {
        return reinterpret_cast<GlFunctionPointer>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(_name)));
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::flushConfigFromWindow()
    {
        XWindowAttributes windowAttributes;
        if (XGetWindowAttributes(m_xDisplay, m_xWindow, &windowAttributes) == 0)
        {
            MAZE_ERROR("Failed to get the window attributes");
            return;
        }

        XVisualInfo tpl;
        tpl.screen   = DefaultScreen(m_xDisplay);
        tpl.visualid = XVisualIDFromVisual(windowAttributes.visual);
        int nbVisuals = 0;
        XVisualInfo* visualInfo = XGetVisualInfo(m_xDisplay, VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);

        if (!visualInfo)
            return;

        flushConfigFromVisualInfo(visualInfo);

        XFree(visualInfo);
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::flushConfigFromVisualInfo(XVisualInfo* _visualInfo)
    {
        S32 depth;
        S32 stencil;
        S32 multiSampling;
        S32 samples;
        S32 sRgb;
        
        glXGetConfig(m_xDisplay, _visualInfo, GLX_DEPTH_SIZE, &depth);
        glXGetConfig(m_xDisplay, _visualInfo, GLX_STENCIL_SIZE, &stencil);

        if (MAZE_GLAD_GLX_ARB_multisample)
        {
            glXGetConfig(m_xDisplay, _visualInfo, GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
            glXGetConfig(m_xDisplay, _visualInfo, GLX_SAMPLES_ARB, &samples);
        }
        else
        {
            multiSampling = 0;
            samples = 0;
        }

        if (MAZE_GLAD_GLX_EXT_framebuffer_sRGB || MAZE_GLAD_GLX_ARB_framebuffer_sRGB)
        {
            glXGetConfig(m_xDisplay, _visualInfo, GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB, &sRgb);
        }
        else
        {
            sRgb = 0;
        }

        m_config.depthBits = static_cast<U32>(depth);
        m_config.stencilBits = static_cast<U32>(stencil);
        m_config.antialiasingLevel = multiSampling ? samples : 0;
        m_config.sRgbCapable = (sRgb == True);
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::createSurface(::Window _xWindow)
    {
        m_xWindow = _xWindow;

        flushConfigFromWindow();
    }

    //////////////////////////////////////////
    XVisualInfo ContextOpenGL3GLX::SelectBestVisual(
        ::Display* _xDisplay,
        U32 _bitsPerPixel,
        ContextOpenGLConfig const& _config)
    {
        EnsureExtensionsInit(_xDisplay, DefaultScreen(_xDisplay));

        S32 const screen = DefaultScreen(_xDisplay);

        S32 count;
        XVisualInfo* visuals = XGetVisualInfo(_xDisplay, 0, NULL, &count);
        if (visuals)
        {
            S32 bestScore = 0x7FFFFFFF;
            XVisualInfo bestVisual = XVisualInfo();
            for (S32 i = 0; i < count; ++i)
            {
                if (visuals[i].screen != screen)
                    continue;
                
                S32 doubleBuffer;
                glXGetConfig(_xDisplay, &visuals[i], GLX_DOUBLEBUFFER, &doubleBuffer);
                if (!doubleBuffer)
                    continue;

                S32 red, green, blue, alpha, depth, stencil, multiSampling, samples, sRgb;
                glXGetConfig(_xDisplay, &visuals[i], GLX_RED_SIZE, &red);
                glXGetConfig(_xDisplay, &visuals[i], GLX_GREEN_SIZE, &green);
                glXGetConfig(_xDisplay, &visuals[i], GLX_BLUE_SIZE, &blue);
                glXGetConfig(_xDisplay, &visuals[i], GLX_ALPHA_SIZE, &alpha);
                glXGetConfig(_xDisplay, &visuals[i], GLX_DEPTH_SIZE, &depth);
                glXGetConfig(_xDisplay, &visuals[i], GLX_STENCIL_SIZE, &stencil);

                if (MAZE_GLAD_GLX_ARB_multisample)
                {
                    glXGetConfig(_xDisplay, &visuals[i], GLX_SAMPLE_BUFFERS_ARB, &multiSampling);
                    glXGetConfig(_xDisplay, &visuals[i], GLX_SAMPLES_ARB, &samples);
                }
                else
                {
                    multiSampling = 0;
                    samples = 0;
                }

                if (MAZE_GLAD_GLX_EXT_framebuffer_sRGB || MAZE_GLAD_GLX_ARB_framebuffer_sRGB)
                {
                    glXGetConfig(_xDisplay, &visuals[i], GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB, &sRgb);
                }
                else
                {
                    sRgb = 0;
                }

                bool accelerated = true;

                int color = red + green + blue + alpha;
                int score = EvaluateFormat(
                    _bitsPerPixel,
                    _config,
                    color,
                    depth,
                    stencil,
                    (multiSampling ? samples : 0),
                    accelerated,
                    sRgb == 1);

                if (score < bestScore)
                {
                    bestScore = score;
                    bestVisual = visuals[i];
                }
            }
            
            XFree(visuals);

            return bestVisual;
        }
        else
        {
            MAZE_ERROR("No GLX visual found. You should check your graphics driver");

            return XVisualInfo();
        }
    }

    //////////////////////////////////////////
    XVisualInfo ContextOpenGL3GLX::SelectBestVisualForWindow(
        WindowLinux* _windowLinux)
    {
        ContextOpenGLConfig config;
        return SelectBestVisual(
            _windowLinux->getXDisplay(),
            _windowLinux->getParams()->bpp,
            config); // #TODO: config from RenderWindow
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::createSurface(
        U32 _width,
        U32 _height,
        U32 _bitsPerPixel)
    {
        ContextOpenGLPtr const& sharedContext = getRenderSystemRaw()->cast<RenderSystemOpenGL>()->getSharedContext();
        GLXContext toShare = sharedContext ? sharedContext->castRaw< ContextOpenGL3GLX >()->getGLXContext() : NULL;

        XVisualInfo visualInfo = SelectBestVisual(m_xDisplay, _bitsPerPixel, m_config);

        if (toShare && MAZE_GLAD_GLX_SGIX_pbuffer)
        {
            S32 major = 0;
            S32 minor = 0;

            glXQueryVersion(m_xDisplay, &major, &minor);

            bool hasCreatePbuffer = ((major > 1) || (minor >= 3));

            if (hasCreatePbuffer)
            {
                GLXFBConfig* glxConfig = NULL;

                S32 nbConfigs = 0;
                GLXFBConfig* glxConfigs = glXChooseFBConfig(m_xDisplay, DefaultScreen(m_xDisplay), NULL, &nbConfigs);

                for (S32 i = 0; glxConfigs && ( i < nbConfigs ); ++i)
                {
                    XVisualInfo* visual = glXGetVisualFromFBConfig(m_xDisplay, glxConfigs[i]);

                    if (!visual)
                        continue;

                    if (visual->visualid == visualInfo.visualid)
                    {
                        glxConfig = &glxConfigs[i];
                        XFree(visual);
                        break;
                    }

                    XFree(visual);
                }

                if (glxConfig)
                {
                    S32 attributes[] =
                    {
                        GLX_PBUFFER_WIDTH,  static_cast<S32>(_width),
                        GLX_PBUFFER_HEIGHT, static_cast<S32>(_height),
                        0,                  0
                    };

                    m_glxPbuffer = glXCreatePbuffer(m_xDisplay, *glxConfig, attributes);

                    flushConfigFromVisualInfo(&visualInfo);

                    XFree(glxConfigs);

                    return;
                }

                if (glxConfigs)
                    XFree(glxConfigs);
            }
        }
        
        S32 screen = DefaultScreen(m_xDisplay);
        
        XSetWindowAttributes attributes;
        attributes.colormap = XCreateColormap(m_xDisplay, RootWindow(m_xDisplay, screen), visualInfo.visual, AllocNone);

        m_xWindow = XCreateWindow(
            m_xDisplay,
            RootWindow(m_xDisplay, screen),
            0, 0,
            _width, _height,
            0,
            DefaultDepth(m_xDisplay, screen),
            InputOutput,
            visualInfo.visual,
            CWColormap,
            &attributes);

        m_ownsWindow = true;

        flushConfigFromWindow();
    }

    //////////////////////////////////////////
    void ContextOpenGL3GLX::createGLXContext()
    {
        ContextOpenGLPtr const& sharedContext = getRenderSystemRaw()->cast<RenderSystemOpenGL>()->getSharedContext();
        GLXContext toShare = sharedContext ? sharedContext->castRaw< ContextOpenGL3GLX >()->getGLXContext() : NULL;

        ContextOpenGLConfig config = m_config;

        XVisualInfo* visualInfo = NULL;

        if (m_glxPbuffer)
        {
            unsigned int fbConfigId = 0;

            glXQueryDrawable(m_xDisplay, m_glxPbuffer, GLX_FBCONFIG_ID, &fbConfigId);

            S32 attributes[] =
            {
                GLX_FBCONFIG_ID, static_cast<S32>(fbConfigId),
                0,               0
            };

            S32 count = 0;
            GLXFBConfig* fbconfig = glXChooseFBConfig(m_xDisplay, DefaultScreen( m_xDisplay ), attributes, &count);

            if (count == 1)
                visualInfo = glXGetVisualFromFBConfig(m_xDisplay, *fbconfig);

            if (fbconfig)
                XFree(fbconfig);
        }
        else
        {
            XWindowAttributes windowAttributes;
            if ( XGetWindowAttributes(m_xDisplay, m_xWindow, &windowAttributes) == 0)
            {
                MAZE_ERROR("Failed to get the window attributes");
                return;
            }

            XVisualInfo tpl;
            tpl.screen   = DefaultScreen(m_xDisplay);
            tpl.visualid = XVisualIDFromVisual(windowAttributes.visual);
            int nbVisuals = 0;
            visualInfo = XGetVisualInfo(m_xDisplay, VisualIDMask | VisualScreenMask, &tpl, &nbVisuals);
        }

        if (!visualInfo)
        {
            MAZE_ERROR("Failed to get visual info");
            return;
        }

        S32 major = 0;
        S32 minor = 0;

        if (!glXQueryVersion(m_xDisplay, &major, &minor))
        {
            MAZE_ERROR("Failed to query GLX version, limited to legacy context creation");
        }

        bool hasCreateContextArb = MAZE_GLAD_GLX_ARB_create_context && ((major > 1) || (minor >= 3));

        if (hasCreateContextArb)
        {
            GLXFBConfig* glxConfig = NULL;

            S32 nbConfigs = 0;
            GLXFBConfig* glxConfigs = glXChooseFBConfig(m_xDisplay, DefaultScreen(m_xDisplay), NULL, &nbConfigs);

            for (S32 i = 0; glxConfigs && (i < nbConfigs); ++i)
            {
                XVisualInfo* visual = glXGetVisualFromFBConfig(m_xDisplay, glxConfigs[i]);

                if (!visual)
                    continue;

                if (visual->visualid == visualInfo->visualid)
                {
                    glxConfig = &glxConfigs[i];
                    XFree(visual);
                    break;
                }

                XFree(visual);
            }

            if (!glxConfig)
            {
                MAZE_ERROR("Failed to get GLXFBConfig which corresponds to the window's visual");
            }

            while (glxConfig && !m_glxContext && m_config.majorVersion)
            {
                Vector<S32> attributes;

                if (( m_config.majorVersion > 1) || ((m_config.majorVersion == 1 ) && (m_config.minorVersion > 1 )))
                {
                    attributes.push_back(GLX_CONTEXT_MAJOR_VERSION_ARB);
                    attributes.push_back(m_config.majorVersion);
                    attributes.push_back(GLX_CONTEXT_MINOR_VERSION_ARB);
                    attributes.push_back(m_config.minorVersion);
                }

                if (MAZE_GLAD_GLX_ARB_create_context_profile)
                {
                    int profile = (m_config.flags & ContextOpenGLFlags::Core) ? GLX_CONTEXT_CORE_PROFILE_BIT_ARB : GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
                    int debug = (m_config.flags & ContextOpenGLFlags::Debug) ? GLX_CONTEXT_DEBUG_BIT_ARB : 0;

                    attributes.push_back(GLX_CONTEXT_PROFILE_MASK_ARB);
                    attributes.push_back(profile);
                    attributes.push_back(GLX_CONTEXT_FLAGS_ARB);
                    attributes.push_back(debug);
                }
                else
                {
                    if ((m_config.flags & ContextOpenGLFlags::Core) || (m_config.flags & ContextOpenGLFlags::Debug))
                    {
                        MAZE_ERROR(
                            "Selecting a profile during context creation is not supported,"
                            "disabling comptibility and debug");
                    }

                    m_config.flags = ContextOpenGLFlags::Default;
                }

                attributes.push_back(0);
                attributes.push_back(0);

                GLXErrorHandler handler(m_xDisplay);

                if (toShare)
                {
                    if (!glXMakeCurrent(m_xDisplay, 0, NULL))
                    {
                        MAZE_ERROR("Failed to deactivate shared context before sharing");
                        return;
                    }
                }

                m_glxContext = glXCreateContextAttribsARB(m_xDisplay, *glxConfig, toShare, true, &attributes[0]);

                if (!m_glxContext)
                {
                    if (m_config.flags != ContextOpenGLFlags::Default)
                    {
                        m_config.flags = ContextOpenGLFlags::Default;
                    }
                    else
                    if (m_config.minorVersion > 0)
                    {
                        m_config.minorVersion--;

                        m_config.flags = config.flags;
                    }
                    else
                    {                        
                        m_config.majorVersion--;
                        m_config.minorVersion = 9;

                        m_config.flags = config.flags;
                    }
                }
            }

            if (glxConfigs)
                XFree(glxConfigs);
        }

        if (!m_glxContext)
        {
            m_config.majorVersion = 2;
            m_config.minorVersion = 1;
            m_config.flags = ContextOpenGLFlags::Default;

#if (MAZE_DEBUG_GLX)
            GLXErrorHandler handler( m_xDisplay );
#endif

            if (toShare)
            {
                if (!glXMakeCurrent(m_xDisplay, 0, NULL))
                {
                    MAZE_ERROR("Failed to deactivate shared context before sharing");
                    return;
                }
            }

            m_glxContext = glXCreateContext(m_xDisplay, visualInfo, toShare, true);

#if (MAZE_DEBUG_GLX)
            if (g_glxErrorOccurred)
            {
                MAZE_ERROR("GLX error in GlxContext::createContext()");
            }
#endif

        }

        if (!m_glxContext)
        {
            MAZE_ERROR("Failed to create an OpenGL context for this window");
        }

        XFree(visualInfo);
        
        createExtensions();
        m_stateMachine.reset(new StateMachineOpenGL(this));
    }
    
    //////////////////////////////////////////
    void ContextOpenGL3GLX::createExtensions()
    {
        m_extensions = ExtensionsOpenGL3GLX::Create(this);
        
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


} // namespace Maze
//////////////////////////////////////////


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
//////////////////////////////////////////
