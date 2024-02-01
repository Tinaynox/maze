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
#pragma once
#if (!defined(_MazeRenderSystemOpenGL_hpp_))
#define _MazeRenderSystemOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLConfig.hpp"
#include "maze-render-system-opengl-core/MazeRenderPassOpenGLPool.hpp"
#include "maze-render-system-opengl-core/MazeMaterialOpenGLPool.hpp"
#include "maze-core/system/MazeThreadLocalPointer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    struct ContextOpenGLConfig;


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    MAZE_USING_SHARED_PTR(ExtensionsOpenGL);
    MAZE_USING_SHARED_PTR(Window);
    MAZE_USING_SHARED_PTR(GPUBuffersControllerOpenGL);


    //////////////////////////////////////////
    // Class RenderSystemOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API RenderSystemOpenGL 
        : public RenderSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~RenderSystemOpenGL();


        //////////////////////////////////////////
        inline RenderSystemOpenGLConfig const& getConfig() const { return m_config; }


        //////////////////////////////////////////
        virtual bool setCurrentRenderTarget(RenderTarget* _renderTarget) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool makeCurrentContext(ContextOpenGL* _context, bool _includeSurface = false);


        //////////////////////////////////////////
        bool isNullContextCreated();

        //////////////////////////////////////////
        inline ContextOpenGLPtr const& getSharedContext() const { return m_sharedContext; }

        //////////////////////////////////////////
        inline ContextOpenGLPtr const& getDummyContext() const { return m_dummyContext; }

        //////////////////////////////////////////
        inline ContextOpenGLPtr const& getDefaultContext() const { return m_defaultRenderContext; }

        //////////////////////////////////////////
        ContextOpenGL* getNullContext();

        //////////////////////////////////////////
        ContextOpenGL* getCurrentContext() const;

        //////////////////////////////////////////
        ContextOpenGL* ensureCurrentContext();

        //////////////////////////////////////////
        ContextOpenGL* ensureDefaultContext(RenderTarget* _renderTarget = nullptr);

        //////////////////////////////////////////
        virtual bool isTextureFormatSupported(PixelFormat::Enum _pixelFormat) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getWindowMaxAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getWindowCurrentAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getTextureMaxSize() MAZE_OVERRIDE { return m_textureMaxSize; }

        //////////////////////////////////////////
        virtual S32 getTextureMaxAntialiasingLevelSupport() MAZE_OVERRIDE { return m_textureMaxAntialiasingLevelSupport; }

        //////////////////////////////////////////
        inline void lockGLMutex() { m_glMutex.lock(); }

        //////////////////////////////////////////
        inline void unlockGLMutex() { m_glMutex.unlock(); }


        //////////////////////////////////////////
        ContextOpenGLPtr createContext(
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        ContextOpenGLPtr createContext(
            U32 _width = 1, U32 _height = 1,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        virtual ShaderUniformPtr createShaderUniform(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderWindowPtr createRenderWindow(RenderWindowParams const& _params) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual VertexArrayObjectPtr createVertexArrayObject(RenderTarget* _renderTarget) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual VertexBufferObjectPtr createVertexBufferObject(
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false,
            RenderTarget * _renderTarget = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        VertexBufferObjectPtr createVertexBufferObject(
            ContextOpenGL* _context,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false);

        //////////////////////////////////////////
        virtual Texture2DPtr createTexture2D() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Texture2DMSPtr createTexture2DMS() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual TextureCubePtr createTextureCube() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual MaterialPtr createMaterial() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderPassPtr createRenderPass(
            RenderPassType _passType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual GPUVertexBufferPtr createGPUVertexBuffer(
            VertexDataDescription const& _vertexDataDescription,
            Size _vertexCount,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        GPUVertexBufferPtr createGPUVertexBuffer(
            ContextOpenGL* _context,
            VertexDataDescription const& _vertexDataDescription,
            Size _vertexCount,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr);

        //////////////////////////////////////////
        virtual GPUTextureBufferPtr createGPUTextureBuffer(
            Vec2U const& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        GPUTextureBufferPtr createGPUTextureBuffer(
            ContextOpenGL* _context,
            Vec2U const& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr);

        //////////////////////////////////////////
        virtual RenderBufferPtr createRenderBuffer(
            RenderBuffer::RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>(),
            RenderTarget* _renderTarget = nullptr) MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderPassOpenGLPool& getRenderPassOpenGLPool() { return m_renderPassOpenGLPool; }

        //////////////////////////////////////////
        MaterialOpenGLPool& getMaterialOpenGLPool() { return m_materialOpenGLPool; }


        //////////////////////////////////////////
        virtual void setDefaultRenderContext(const ContextOpenGLPtr& _context);
        
    public:

        MultiDelegate<ContextOpenGLPtr const&> eventDefaultContextSet;

    protected:

        //////////////////////////////////////////
        RenderSystemOpenGL();

        //////////////////////////////////////////
        using RenderSystem::init;

        //////////////////////////////////////////
        virtual bool init(RenderSystemOpenGLConfig const& _config);


        //////////////////////////////////////////
        virtual ContextOpenGLPtr createContextImpl(
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual ContextOpenGLPtr createContextImpl(
            U32 _width = 1, U32 _height = 1,
            ContextOpenGLConfig* _config = nullptr) MAZE_ABSTRACT;
    
    protected:
        RenderSystemOpenGLConfig m_config;

        ContextOpenGLPtr m_dummyContext;
        ContextOpenGLPtr m_sharedContext;
        ThreadLocalPointer<ContextOpenGL> m_currentContext;
        ThreadLocalPointer<ContextOpenGL> m_nullContext;
        Set<ContextOpenGLPtr> m_nullContexts;
        Mutex m_nullContextsMutex;

        Mutex m_glMutex;

        ContextOpenGLPtr m_defaultRenderContext;

        S32 m_textureMaxSize = 0;
        S32 m_textureUniformsMaxCount = 0;
        S32 m_textureMaxAntialiasingLevelSupport = 0;

        GPUBuffersControllerOpenGLPtr m_GPUBuffersController;

        RenderPassOpenGLPool m_renderPassOpenGLPool;
        MaterialOpenGLPool m_materialOpenGLPool;
    };


    //////////////////////////////////////////
    // Class GLMutexScope
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API GLMutexScope
    {
    public:

        //////////////////////////////////////////
        inline GLMutexScope(RenderSystemOpenGL* _renderSystem)
            : m_renderSystem(_renderSystem)
        {
            m_renderSystem->lockGLMutex();
        }

        //////////////////////////////////////////
        inline ~GLMutexScope()
        {
            m_renderSystem->unlockGLMutex();
        }
    
        //////////////////////////////////////////
        GLMutexScope(GLMutexScope const&) = delete;

        //////////////////////////////////////////
        GLMutexScope(GLMutexScope&&) = delete;

        //////////////////////////////////////////
        GLMutexScope& operator=(GLMutexScope const&) = delete;

        //////////////////////////////////////////
        GLMutexScope& operator=(GLMutexScope&&) = delete;

    protected:
        RenderSystemOpenGL* m_renderSystem;
    };


    //////////////////////////////////////////
    #define MAZE_GL_MUTEX_SCOPED_LOCK(__renderSystem) GLMutexScope _glScopeLock(__renderSystem);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemOpenGL_hpp_
//////////////////////////////////////////
