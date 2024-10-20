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
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeVertexBufferObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DMSOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTextureCubeOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeMaterialOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderPassOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeGPUBuffersControllerOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderBufferOpenGL.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_FORCEINLINE String GatherOpenGLContextErrorInfo(ContextOpenGL* _context)
    {
        if (!_context)
            return "null";

        String value;
        StringHelper::FormatString(
            value,
            "%x (isValid=%d isCreated=%d isDestroyed=%d)",
            _context,
            (S32)_context->isValid(),
            (S32)_context->getIsCreated(),
            (S32)_context->getIsDestroyed());
        return value;
    }


    //////////////////////////////////////////
    // Class RenderSystemOpenGL
    //
    //////////////////////////////////////////
    RenderSystemOpenGL::RenderSystemOpenGL()
        : m_renderPassOpenGLPool(this)
        , m_materialOpenGLPool(this)
    {
    }

    //////////////////////////////////////////
    RenderSystemOpenGL::~RenderSystemOpenGL()
    {
        m_spriteManager.reset();
        m_systemFontManager.reset();
        m_materialManager.reset();
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL::init(RenderSystemOpenGLConfig const& _config)
    {
        if (!RenderSystem::init())
            return false;

        m_config = _config;

        m_GPUBuffersController = GPUBuffersControllerOpenGL::Create(this);

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL::setCurrentRenderTarget(RenderTarget* _renderTarget)
    {
        MAZE_GL_MUTEX_SCOPED_LOCK(this);

        if (m_currentRenderTarget == _renderTarget)
            return true;

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetWillReset();
            m_currentRenderTarget = nullptr;
        }

        if (_renderTarget)
        {
            if (!_renderTarget->processRenderTargetWillSet())
                return false;
        }

        
        m_currentRenderTarget = _renderTarget;
        
        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetSet();
        }
        

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL::makeCurrentContext(ContextOpenGL* _context, bool _includeSurface)
    {
        ContextOpenGL* currentContext = m_currentContext;
        if (currentContext == _context)
        {
#if (MAZE_DEBUG_GL)
            if (currentContext)
                currentContext->_validateIsCurrentGLContext();
#endif
            return true;
        }

        if (currentContext && _context)
        {
            if (currentContext->isSameGLContext(_context))
            {
                if (_includeSurface)
                {
                    if (currentContext->isSameGLSurface(_context))
                    {

#if (MAZE_DEBUG_GL)
                        if (currentContext)
                            currentContext->_validateIsCurrentGLContextAndSurface();
#endif

                        return true;
                    }
                }
                else
                {

#if (MAZE_DEBUG_GL)
                    if (currentContext)
                        currentContext->_validateIsCurrentGLContext();
#endif

                    return true;
                }
            }
        }

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetWillReset();
            m_currentRenderTarget = nullptr;
        }

        if (m_currentContext)
            m_currentContext->processContextWillReset();

        if (_context)
        {
            if (!_context->isValid())
                return false;
        
            if (!_context->makeCurrent())
                return false;

            m_currentContext = _context;
            
            if (m_currentContext)
                m_currentContext->processContextSet();
        }
        else
        {
            m_currentContext = _context;
            return true;
        }
        
        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL::isNullContextCreated()
    {
        if (!m_nullContext)
            return false;

        {
            MAZE_MUTEX_SCOPED_LOCK(m_nullContextsMutex);
            for (Set<ContextOpenGLPtr>::iterator it = m_nullContexts.begin(),
                                                 end = m_nullContexts.end();
                                                 it != end;
                                                 ++it)
            {
                if ((*it).get() == m_nullContext)
                    return true;
            }
        }
        
        return false;
    }

    //////////////////////////////////////////
    ContextOpenGL* RenderSystemOpenGL::getNullContext()
    {
        if (!m_config.useNullContexts)
            return nullptr;

        if (!isNullContextCreated())
        {
             ContextOpenGLPtr nullContext = createContext();
             nullContext->setName("NullContext");
             {
                 MAZE_MUTEX_SCOPED_LOCK(m_nullContextsMutex);
                 m_nullContext = (*(m_nullContexts.insert(nullContext)).first).get();
             }
        }

        return m_nullContext;
    }

    //////////////////////////////////////////
    ContextOpenGL* RenderSystemOpenGL::getCurrentContext() const
    {
        return m_currentContext;
    }

    //////////////////////////////////////////
    ContextOpenGL* RenderSystemOpenGL::ensureCurrentContext()
    {
        if (m_currentContext)
            return m_currentContext;

        if (m_defaultRenderContext && m_defaultRenderContext->makeCurrentContext(true))
        {
            return m_currentContext;
        }

        if (m_sharedContext)
            return m_sharedContext.get();

        return nullptr;
    }

    //////////////////////////////////////////
    ContextOpenGL* RenderSystemOpenGL::ensureDefaultContext(RenderTarget* _renderTarget)
    {
        if (_renderTarget)
        {
            if (_renderTarget->getRenderContext())
                return static_cast<ContextOpenGL*>(_renderTarget->getRenderContext());
        }

        if (m_defaultRenderContext && m_defaultRenderContext->makeCurrentContext(true))
            return m_defaultRenderContext.get();

        MAZE_ERROR("Null context!");

        return nullptr;
    }

    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL::createContext(
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGLPtr context = createContextImpl(_window, _config);

        if (context)
        {
            if (!m_defaultRenderContext)
                setDefaultRenderContext(context);

            context->setupDefaultStates();
        }
        
        return context;
    }

    //////////////////////////////////////////
    ContextOpenGLPtr RenderSystemOpenGL::createContext(
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        ContextOpenGLPtr context = createContextImpl(_width, _height, _config);

        if (context)
            context->setupDefaultStates();

        return context;
    }

    //////////////////////////////////////////
    ShaderUniformPtr RenderSystemOpenGL::createShaderUniform(
        ShaderPtr const& _shader,
        ShaderUniformType _type)
    {
        return ShaderUniformOpenGL::Create(_shader, _type);
    }

    //////////////////////////////////////////
    RenderWindowPtr RenderSystemOpenGL::createRenderWindow(RenderWindowParams const& _params)
    {
        RenderWindowPtr window = RenderWindowOpenGL::Create(_params);
        if (!window)
        {
            MAZE_ERROR("RenderWindowOpenGL cannot be created!");
            return RenderWindowPtr();
        }

        if (!processRenderWindowCreated(window))
        {
            MAZE_ERROR("processRenderWindowCreated failed!");
            return RenderWindowPtr();
        }

        return window;
    }

    //////////////////////////////////////////
    VertexArrayObjectPtr RenderSystemOpenGL::createVertexArrayObject(RenderTarget* _renderTarget)
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext(_renderTarget);
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        VertexArrayObjectOpenGLPtr vao = VertexArrayObjectOpenGL::Create(this, contextOpenGL);
        MAZE_DEBUG_ERROR_IF(!vao, "VAO is null");
        return vao;
    }

    //////////////////////////////////////////
    VertexBufferObjectPtr RenderSystemOpenGL::createVertexBufferObject(
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping,
        RenderTarget* _renderTarget)
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext(_renderTarget);
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return createVertexBufferObject(
            contextOpenGL,
            _accessType,
            _singleMapping);
    }

    //////////////////////////////////////////
    VertexBufferObjectPtr RenderSystemOpenGL::createVertexBufferObject(
        ContextOpenGL* _context,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping)
    {
        return VertexBufferObjectOpenGL::Create(
            this,
            _accessType,
            _singleMapping,
            _context);
    }

    //////////////////////////////////////////
    Texture2DPtr RenderSystemOpenGL::createTexture2D()
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext();
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return Texture2DOpenGL::Create(this, contextOpenGL);
    }

    //////////////////////////////////////////
    Texture2DMSPtr RenderSystemOpenGL::createTexture2DMS()
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext();
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return Texture2DMSOpenGL::Create(this, contextOpenGL);
    }

    //////////////////////////////////////////
    TextureCubePtr RenderSystemOpenGL::createTextureCube()
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext();
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return TextureCubeOpenGL::Create(this, contextOpenGL);
    }

    //////////////////////////////////////////
    MaterialPtr RenderSystemOpenGL::createMaterial()
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext();
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return MaterialOpenGL::Create(this, contextOpenGL);
    }

    //////////////////////////////////////////
    RenderPassPtr RenderSystemOpenGL::createRenderPass(
        RenderPassType _passType)
    {
        return RenderPassOpenGL::Create(this, _passType);
    }

    //////////////////////////////////////////
    GPUVertexBufferPtr RenderSystemOpenGL::createGPUVertexBuffer(
        VertexDataDescription const& _vertexDataDescription,
        Size _vertexCount,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext();
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return createGPUVertexBuffer(
            contextOpenGL,
            _vertexDataDescription,
            _vertexCount,
            _accessType,
            _initialData);
    }

    //////////////////////////////////////////
    GPUVertexBufferPtr RenderSystemOpenGL::createGPUVertexBuffer(
        ContextOpenGL* _context,
        VertexDataDescription const& _vertexDataDescription,
        Size _vertexCount,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!_context, nullptr, "Context is null!");

        return m_GPUBuffersController->createGPUVertexBuffer(
            _context,
            _vertexDataDescription,
            _vertexCount,
            _accessType,
            _initialData);
    }

    //////////////////////////////////////////
    GPUTextureBufferPtr RenderSystemOpenGL::createGPUTextureBuffer(
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext();
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return createGPUTextureBuffer(
            contextOpenGL,
            _size,
            _pixelFormat,
            _accessType,
            _initialData);
    }

    //////////////////////////////////////////
    GPUTextureBufferPtr RenderSystemOpenGL::createGPUTextureBuffer(
        ContextOpenGL* _context,
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!_context, nullptr, "Context is null!");

        return m_GPUBuffersController->createGPUTextureBuffer(
            _context,
            _size,
            _pixelFormat,
            _accessType,
            _initialData);
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderSystemOpenGL::createRenderBuffer(
        RenderBuffer::RenderBufferDeleter const& _deleter,
        RenderTarget* _renderTarget)
    {
        ContextOpenGL* contextOpenGL = ensureDefaultContext(_renderTarget);
        MAZE_ERROR_RETURN_VALUE_IF(
            !contextOpenGL,
            nullptr,
            "Context is null! current currentContext=%s, defaultRenderContext=%s sharedContext=%s",
            GatherOpenGLContextErrorInfo((ContextOpenGL*)m_currentContext).c_str(),
            GatherOpenGLContextErrorInfo(m_defaultRenderContext.get()).c_str(),
            GatherOpenGLContextErrorInfo(m_sharedContext.get()).c_str());

        return RenderBufferOpenGL::Create(this, contextOpenGL, _deleter);
    }

    //////////////////////////////////////////
    void RenderSystemOpenGL::setDefaultRenderContext(const ContextOpenGLPtr& _context)
    {
        if (m_defaultRenderContext == _context) 
            return;

        m_defaultRenderContext = _context; 

        eventDefaultContextSet(m_defaultRenderContext);
    }

    //////////////////////////////////////////
    bool RenderSystemOpenGL::isTextureFormatSupported(PixelFormat::Enum _pixelFormat)
    {
        // #TODO:
        return true;
    }

    //////////////////////////////////////////
    S32 RenderSystemOpenGL::getWindowMaxAntialiasingLevelSupport()
    {
        if (m_defaultRenderContext)
            return m_defaultRenderContext->getWindowMaxAntialiasingLevelSupport();

        if (m_dummyContext)
            return m_dummyContext->getWindowMaxAntialiasingLevelSupport();

        return 0;
    }

    //////////////////////////////////////////
    S32 RenderSystemOpenGL::getWindowCurrentAntialiasingLevelSupport()
    {
        if (m_defaultRenderContext)
            return m_defaultRenderContext->getStateMachine()->getAntialiasingLevelSupport();

        if (m_dummyContext)
            return m_dummyContext->getStateMachine()->getAntialiasingLevelSupport();

        return 0;
    }
    

} // namespace Maze
//////////////////////////////////////////
