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
#if (MAZE_PLATFORM == MAZE_PLATFORM_OSX)


//////////////////////////////////////////
#include "maze-render-system-opengl3/osx/ns/MazeContextOpenGL3NS.hpp"
#include "maze-render-system-opengl3/osx/ns/MazeRenderSystemOpenGL3NS.hpp"
#include "maze-render-system-opengl3/osx/ns/MazeExtensionsOpenGL3NS.hpp"
#include "maze-render-system-opengl3/MazeFunctionsOpenGL3.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include MAZE_INCLUDE_OS_FILE(maze-core/system, MazeWindow)
#include MAZE_INCLUDE_OS_FILE(maze-core/helpers, MazeThreadHelper)

#include <dlfcn.h>


//////////////////////////////////////////
namespace Maze
{   

    //////////////////////////////////////////
    // Class ContextOpenGL3NS
    //
    //////////////////////////////////////////
    ContextOpenGL3NS::ContextOpenGL3NS()
        : m_context(nullptr)
        , m_view(nullptr)
        , m_window(nullptr)
    {
        
    }

    //////////////////////////////////////////
    ContextOpenGL3NS::~ContextOpenGL3NS()
    {
        destroyGLContext();
    }

    //////////////////////////////////////////
    ContextOpenGL3NSPtr ContextOpenGL3NS::Create( 
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3NSPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3NS, object, init(_renderSystem, _window, _config));
        return object;
    }

    //////////////////////////////////////////
    ContextOpenGL3NSPtr ContextOpenGL3NS::Create( 
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGL3NSPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenGL3NS, object, init(_renderSystem, _width, _height, _config));
        return object;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3NS::init( 
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        if (!ContextOpenGL3::init(_renderSystem, _window, _config))
            return false;
        
        return createGLContextWindow();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3NS::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        if (!ContextOpenGL3::init(_renderSystem, _width, _height, _config))
            return false;

        return createGLContextPixelBuffer();
    }
    
    //////////////////////////////////////////
    bool ContextOpenGL3NS::createGLContextWindow()
    {
        destroyGLContext();
        
        WindowManager* windowManager = WindowManager::GetInstancePtr();

        ThreadHelper::EnsureThreadHasAutoreleasePool();
        
        WindowVideoMode windowVideoMode = windowManager->getPrimaryDisplayVideoMode();
        createNSContext(windowVideoMode.bpp, m_config);
            
        WindowOSXPtr windowOSX = m_windowRaw->cast<WindowOSX>();
        ViewOSXRef viewOSX = windowOSX->getViewOSX();
        [viewOSX setOpenGLContext: m_context];
        [m_context setView: static_cast<NSView*>(viewOSX)];
        
        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3NS::createGLContextPixelBuffer()
    {
        destroyGLContext();
        
        MAZE_TODO;
        setVSync(0);
        
        return false;
    }

    //////////////////////////////////////////
    bool ContextOpenGL3NS::isValid()
    {
        return (m_context != nil);
    }
    

    //////////////////////////////////////////
    bool ContextOpenGL3NS::makeCurrent()
    {
        MAZE_GL_MUTEX_SCOPED_LOCK(m_renderSystemRaw);
                
        [m_context makeCurrentContext];
        
        return m_context == [NSOpenGLContext currentContext];
    }

    //////////////////////////////////////////
    void ContextOpenGL3NS::setVSync(S32 _vsync)
    {
        RenderTarget* prevRenderTarget = getRenderSystemRaw()->getCurrentRenderTarget();
        ContextOpenGL* prevRenderContext = getRenderSystemRaw()->getCurrentContext();
        makeCurrent();
        

        MZGLint swapInterval = _vsync;
        [m_context setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];

        if (prevRenderContext)
        {
            getRenderSystemRaw()->makeCurrentContext(prevRenderContext);
            getRenderSystemRaw()->setCurrentRenderTarget(prevRenderTarget);
        }
    }

    //////////////////////////////////////////
    void ContextOpenGL3NS::ensureIfCurrent()
    {
        if (getRenderSystem()->getCurrentContext() != this)
            return;

        if (m_context == [NSOpenGLContext currentContext])
            return;
        
        MAZE_GL_MUTEX_SCOPED_LOCK(m_renderSystemRaw);
        [m_context makeCurrentContext];
        mzglFlush();
    }

    //////////////////////////////////////////
    bool ContextOpenGL3NS::_validateIsCurrent()
    {
        MAZE_ERROR_IF(m_context != [NSOpenGLContext currentContext], "Context is not current!!");
        
        return true;
    }

    //////////////////////////////////////////
    ContextOpenGL3NS::GlFunctionPointer ContextOpenGL3NS::getFunction(CString _name)
    {
        static void* image = nullptr;
        if (!image)
            image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
        
        return (image ? reinterpret_cast<GlFunctionPointer>(reinterpret_cast<intptr_t>(dlsym(image, _name))) : 0);
    }


    //////////////////////////////////////////
    void ContextOpenGL3NS::swapBuffers()
    {
        MAZE_GL_MUTEX_SCOPED_LOCK(m_renderSystemRaw);
        
        [m_context flushBuffer];
    }

    
    //////////////////////////////////////////
    void ContextOpenGL3NS::destroyGLContext()
    {
        if (!m_context)
            return;
        
        processContextWillBeDestroyed();
        
        ContextOpenGLScopeBind scopeBind(this);
        
        [m_context clearDrawable];
        [m_context release];
        m_context = nil;
        
        if (m_view)
        {
            [m_view release];
            m_view = nil;
        }
        
        if (m_window)
        {
            [m_window release];
            m_window = nil;
        }
        
        processContextDestroyed();
    }

    //////////////////////////////////////////
    void ContextOpenGL3NS::flushConfig()
    {
        
    }

    //////////////////////////////////////////
    void ContextOpenGL3NS::createNSContext(U32 _bpp, ContextOpenGLConfig const& _config)
    {   
        m_config = _config;
        
        Vector<NSOpenGLPixelFormatAttribute> attrs;
        attrs.reserve(20);
        
        attrs.push_back(NSOpenGLPFAClosestPolicy);
        attrs.push_back(NSOpenGLPFADoubleBuffer);
       
        
        if (_bpp > 24)
        {
            attrs.push_back(NSOpenGLPFAAlphaSize);
            attrs.push_back((NSOpenGLPixelFormatAttribute)8);
        }    

        
        attrs.push_back( NSOpenGLPFADepthSize );
        attrs.push_back( ( NSOpenGLPixelFormatAttribute )m_config.depthBits );
        
        attrs.push_back( NSOpenGLPFAStencilSize );
        attrs.push_back( ( NSOpenGLPixelFormatAttribute )m_config.stencilBits );
        
        if (m_config.antialiasingLevel > 0)
        {            
            attrs.push_back(NSOpenGLPFAMultisample);
            
            attrs.push_back(NSOpenGLPFASampleBuffers);
            attrs.push_back((NSOpenGLPixelFormatAttribute)1);
            
            attrs.push_back(NSOpenGLPFASamples );
            attrs.push_back((NSOpenGLPixelFormatAttribute)m_config.antialiasingLevel);
            
            attrs.push_back(NSOpenGLPFAAccelerated);
        }
        
        bool legacy = (m_config.majorVersion != 0 && m_config.majorVersion < 3);
        
        if (legacy)
        {
            m_config.flags &= ~ContextOpenGLFlags::Core;
            m_config.majorVersion = 2;
            m_config.minorVersion = 1;
            attrs.push_back(NSOpenGLPFAOpenGLProfile);
            attrs.push_back(NSOpenGLProfileVersionLegacy);
        }
        else
        {
            if (!(m_config.flags & ContextOpenGLFlags::Core))
            {
                MAZE_WARNING("Compatibility profile not supported on this platform.");
                m_config.flags |= ContextOpenGLFlags::Core;
            }
            m_config.majorVersion = 3;
            m_config.minorVersion = 2;
            attrs.push_back(NSOpenGLPFAOpenGLProfile);
            attrs.push_back(NSOpenGLProfileVersion3_2Core);
        }
        
        if (m_config.flags & ContextOpenGLFlags::Debug)
        {
            MAZE_WARNING("OpenGL debugging not supported on this platform.");
            m_config.flags &= ~ContextOpenGLFlags::Debug;
        }
        
        attrs.push_back((NSOpenGLPixelFormatAttribute)0);
        
        m_config.sRgbCapable = true;
        
        NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:&attrs[0]];
        
        MAZE_ERROR_RETURN_IF(pixFmt == nil, "Unable to find a suitable pixel format.");
        
        ContextOpenGLPtr const& sharedContext = getRenderSystem()->cast<RenderSystemOpenGL>()->getSharedContext();
        
        NSOpenGLContext* shared = sharedContext ? sharedContext->cast<ContextOpenGL3NS>()->m_context : nil;
        
        m_context = [[NSOpenGLContext alloc] initWithFormat:pixFmt
                                             shareContext:shared ];
        
        if (m_context == nil)
        {
            MAZE_ERROR("Unable to create the context. Retrying without shared context.");
            m_context = [[NSOpenGLContext alloc] initWithFormat:pixFmt
                                                 shareContext:nil ];
            
            if (m_context == nil)
            {
                MAZE_ERROR("Unable to create the context.");
            }
            else
            {
                MAZE_WARNING("New context created without shared context.");
            }
        }
        
        if (m_config.transparentBackground)
        {
            MZGLint aValue = 0;
            [m_context setValues:&aValue
                       forParameter:NSOpenGLCPSurfaceOpacity];
        }
        
        bool gpuProcessing;
        MZGLint fragmentGPUProcessing = 0;
        MZGLint vertexGPUProcessing = 0;
        CGLGetParameter( 
            CGLGetCurrentContext(),
            kCGLCPGPUFragmentProcessing,
            &fragmentGPUProcessing);
        
        CGLGetParameter(
            CGLGetCurrentContext(),
            kCGLCPGPUVertexProcessing,
            &vertexGPUProcessing);
        gpuProcessing = (fragmentGPUProcessing && vertexGPUProcessing) ? true : false;
        Debug::Log("GPU Processing: %d", S32(gpuProcessing));
        
        [pixFmt release];
        
        syncGLStates();
        
        createExtensions();
        m_stateMachine.reset(new StateMachineOpenGL(this));
    }
    
    //////////////////////////////////////////
    void ContextOpenGL3NS::createExtensions()
    {
        m_extensions = ExtensionsOpenGL3NS::Create(this);
        
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


#endif // (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
//////////////////////////////////////////
