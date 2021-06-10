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
#if (!defined(_MazeContextOpenGL_hpp_))
#define _MazeContextOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGLConfig.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeModelMatricesArchitectureOpenGL.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/utils/MazeDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    MAZE_USING_SHARED_PTR(Shader);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL);
    MAZE_USING_SHARED_PTR(ExtensionsOpenGL);
    MAZE_USING_SHARED_PTR(RenderBuffer);


    //////////////////////////////////////////
    // Class ContextOpenGLScopeBind
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API ContextOpenGLScopeBind
    {
    public:

        //////////////////////////////////////////
        ContextOpenGLScopeBind(ContextOpenGL* _newContext, bool _includeSurface = false);

        //////////////////////////////////////////
        ContextOpenGLScopeBind(ContextOpenGLPtr const& _newContext, bool _includeSurface = false);

        //////////////////////////////////////////
        ~ContextOpenGLScopeBind();

    protected:

        //////////////////////////////////////////
        ContextOpenGLScopeBind(ContextOpenGLScopeBind const&) = delete;

        //////////////////////////////////////////
        ContextOpenGLScopeBind(ContextOpenGLScopeBind&&) = delete;

        //////////////////////////////////////////
        ContextOpenGLScopeBind& operator=(ContextOpenGLScopeBind const&) = delete;

        //////////////////////////////////////////
        ContextOpenGLScopeBind& operator=(ContextOpenGLScopeBind&&) = delete;

    private:
        bool m_bind;
        bool m_includeSurface;
        RenderSystemOpenGL* m_renderSystem;
        RenderTarget* m_prevRenderTarget;
        ContextOpenGL* m_prevContext;
    };


    //////////////////////////////////////////
    // Class ContextOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API ContextOpenGL
        : public SharedObject<ContextOpenGL>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        friend class RenderSystemOpenGL;

        //////////////////////////////////////////
        using GlFunctionPointer = void (*)();

    public:

        //////////////////////////////////////////
        virtual ~ContextOpenGL();

        //////////////////////////////////////////
        static ContextOpenGLPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            WindowPtr const& _window,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        static ContextOpenGLPtr Create(
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 _width = 1, U32 _height = 1,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        inline bool getIsCreated() const { return m_isCreated; }

        //////////////////////////////////////////
        inline bool getIsDestroyed() const { return m_isDestroyed; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }

        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        StateMachineOpenGLPtr const& getStateMachine() const { return m_stateMachine; }

        //////////////////////////////////////////
        inline RenderSystemOpenGLPtr getRenderSystem() const { return m_renderSystemWeak.lock(); }

        //////////////////////////////////////////
        inline RenderSystemOpenGL* getRenderSystemRaw() const { return m_renderSystemRaw; }

        //////////////////////////////////////////
        bool makeCurrentContext(bool _isCurrent, bool _includeSurface = false);

        //////////////////////////////////////////
        virtual void setVSync(S32 _vsync) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isValid() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool _validateIsCurrentGLContext() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool _validateIsCurrentGLContextAndSurface() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isSameGLContext(ContextOpenGL* _context) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isSameGLSurface(ContextOpenGL* _context) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual GlFunctionPointer getFunction(CString _name) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void swapBuffers() MAZE_ABSTRACT;


        //////////////////////////////////////////
        inline U32 getMajorVersion() const { return m_config.majorVersion; }

        //////////////////////////////////////////
        inline U32 getMinorVersion() const { return m_config.minorVersion; }

        //////////////////////////////////////////
        inline bool hasMinVersion(U32 _minMajor, U32 _minMinor) const
        {
            return getMajorVersion() > _minMajor || (getMajorVersion() == _minMajor && getMinorVersion() >= _minMinor);
        }


        //////////////////////////////////////////
        inline ExtensionsOpenGLPtr const& getExtensions() const { return m_extensions; }

        //////////////////////////////////////////
        inline ExtensionsOpenGL* getExtensionsRaw() const { return m_extensions.get(); }


        //////////////////////////////////////////
        void syncGLStates();


        ////////////////////////////////////
        virtual void processContextWillReset() {}

        ////////////////////////////////////
        virtual void processContextSet() {}


        //////////////////////////////////////////
        bool createGLContext();

        //////////////////////////////////////////
        bool createGLContext(WindowPtr const& _window);

        //////////////////////////////////////////
        virtual void flushConfig() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void destroyGLContext() MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual void setupDefaultStates();

        //////////////////////////////////////////
        inline Vec4DF const& getClearColor() const { return m_stateMachine->getClearColor(); }

        //////////////////////////////////////////
        inline void setClearColor(Vec4DF const& _clearColor) { m_stateMachine->setClearColor(_clearColor); }

        //////////////////////////////////////////
        inline F32 getClearDepth() const { return m_stateMachine->getClearDepth(); }

        //////////////////////////////////////////
        inline void setClearDepth(F32 _clearDepth) { m_stateMachine->setClearDepth(_clearDepth); }


        //////////////////////////////////////////
        void setViewportRect(Rect2DS const& _viewportRect);

        //////////////////////////////////////////
        inline Rect2DS const& getViewportRect() const { return m_stateMachine->getViewportRect(); }


        //////////////////////////////////////////
        inline Shader* getCurrentShader() const { return m_stateMachine->getCurrentShader(); }

        //////////////////////////////////////////
        inline void setCurrentShader(Shader* _shader) { m_stateMachine->setCurrentShader(_shader); }

        //////////////////////////////////////////
        inline void setCurrentShader(ShaderPtr const& _shader)
        {
            setCurrentShader(_shader.get());
        }


        //////////////////////////////////////////
        inline RenderBuffer* getCurrentRenderBuffer() const { return m_stateMachine->getCurrentRenderBuffer(); }

        //////////////////////////////////////////
        inline void setCurrentRenderBuffer(RenderBuffer* _renderBuffer) { m_stateMachine->setCurrentRenderBuffer(_renderBuffer); }

        //////////////////////////////////////////
        inline void setCurrentRenderBuffer(RenderBufferPtr const& _renderBuffer)
        {
            setCurrentRenderBuffer(_renderBuffer.get());
        }


        //////////////////////////////////////////
        inline MZGLuint getBindedFrameBuffer() const { return m_stateMachine->getBindedFrameBuffer(); }

        //////////////////////////////////////////
        inline void bindVertexArrayObject(MZGLuint _vertexArrayObject) { m_stateMachine->bindVertexArrayObject(_vertexArrayObject); }

        //////////////////////////////////////////
        inline MZGLuint getBindedVertexArrayObject() const { return m_stateMachine->getBindedVertexArrayObject(); }

        //////////////////////////////////////////
        inline void bindArrayBuffer(MZGLuint _arrayBuffer) { m_stateMachine->bindArrayBuffer(_arrayBuffer); }

        //////////////////////////////////////////
        inline MZGLuint getBindedArrayBuffer() const { return m_stateMachine->getBindedArrayBuffer(); }

        //////////////////////////////////////////
        inline void bindElementArrayBuffer(MZGLuint _elementArrayBuffer) { m_stateMachine->bindElementArrayBuffer(_elementArrayBuffer); }

        //////////////////////////////////////////
        inline MZGLuint getBindedElementArrayBuffer() const { return m_stateMachine->getBindedElementArrayBuffer(); }

        //////////////////////////////////////////
        void activeTexture(S32 _index);

        //////////////////////////////////////////
        inline S32 getActiveTexture() const { return m_stateMachine->getActiveTexture() - MAZE_GL_TEXTURE0; }

        //////////////////////////////////////////
        inline void bindTexture2D(Texture2D* _texture2D) { m_stateMachine->bindTexture2D(_texture2D); }

        //////////////////////////////////////////
        inline void bindTexture2D(Texture2DPtr _texture2D) { bindTexture2D(_texture2D.get()); }

        //////////////////////////////////////////
        inline Texture2D* getBindedTexture2D() const { return m_stateMachine->getCurrentTexture2D(getActiveTexture()); }


        //////////////////////////////////////////
        void setBlendFactors(BlendFactor _blendSrcFactor, BlendFactor _blendDestFactor);

        //////////////////////////////////////////
        void setBlendEnabled(bool _value);


        //////////////////////////////////////////
        void setDepthTestEnabled(bool _depthTestEnabled);

        //////////////////////////////////////////
        inline bool getDepthTestEnabled() const { return m_stateMachine->getDepthTestEnabled(); }

        //////////////////////////////////////////
        void setDepthTestCompareFunction(CompareFunction _depthTestCompareFunction);

        //////////////////////////////////////////
        inline CompareFunction getDepthTestCompareFunction() const { return m_stateMachine->getDepthTestCompareFunction(); }


        //////////////////////////////////////////
        void setDepthWriteEnabled(bool _depthWriteEnabled);

        //////////////////////////////////////////
        inline bool getDepthWriteEnabled() const { return m_stateMachine->getDepthWriteEnabled(); }

        //////////////////////////////////////////
        void setCullEnabled(bool _cullEnabled);

        //////////////////////////////////////////
        inline bool getCullEnabled() const { return m_stateMachine->getCullEnabled(); }

        //////////////////////////////////////////
        void setCullMode(CullMode _cullMode);

        //////////////////////////////////////////
        inline CullMode getCullMode() const { return m_stateMachine->getCullMode(); }

        //////////////////////////////////////////
        void setWireframeRender(bool _value);

        //////////////////////////////////////////
        inline bool getWireframeRender() const { return m_stateMachine->getWireframeRender(); }


        //////////////////////////////////////////
        void setScissorRect(Rect2DS const& _scissorRect);

        //////////////////////////////////////////
        inline void setScissorRect(S32 _x, S32 _y, S32 _w, S32 _h)
        {
            setScissorRect(Rect2DS(_x, _y, _w, _h));
        }

        //////////////////////////////////////////
        inline Rect2DS const& getScissorRect() const { return m_stateMachine->getScissorRect(); }

        //////////////////////////////////////////
        void setScissorTestEnabled(bool _value);

        //////////////////////////////////////////
        inline bool getScissorTestEnabled() const { return m_stateMachine->getScissorTestEnabled(); }

        //////////////////////////////////////////
        inline ContextOpenGLConfig const& getConfig() const { return m_config; }


        //////////////////////////////////////////
        inline ModelMatricesArchitectureOpenGL getModelMatricesArchitecture() const { return m_modelMatricesArchitecture; }

    public:

        //////////////////////////////////////////
        MultiDelegate<ContextOpenGL*> eventGLContextCreated;

        //////////////////////////////////////////
        MultiDelegate<ContextOpenGL*> eventGLContextSetup;

        //////////////////////////////////////////
        MultiDelegate<ContextOpenGL*> eventGLContextWillBeDestroyed;

        //////////////////////////////////////////
        MultiDelegate<ContextOpenGL*> eventGLContextDestroyed;

        //////////////////////////////////////////
        MultiDelegate<ContextOpenGL*> eventDestroyed;

    public:

        //////////////////////////////////////////
        static S32 EvaluateFormat(
            U32 _bitsPerPixel,
            ContextOpenGLConfig const& _config,
            S32 _colorBits,
            S32 _depthBits,
            S32 _stencilBits,
            S32 _antialiasing,
            bool _accelerated,
            bool _sRgb);


    protected:

        //////////////////////////////////////////
        ContextOpenGL();

        //////////////////////////////////////////
        virtual bool init(
            RenderSystemOpenGLPtr const& _renderSystem,
            WindowPtr const& _window,
            ContextOpenGLConfig* _config = nullptr);

        //////////////////////////////////////////
        virtual bool init(
            RenderSystemOpenGLPtr const& _renderSystem,
            U32 _width = 1, U32 _height = 1,
            ContextOpenGLConfig* _config = nullptr);


        //////////////////////////////////////////
        void notifyWindowWillClose(Window* window);

        //////////////////////////////////////////
        void notifyWindowDestroyed(Window* window);


        //////////////////////////////////////////
        void processContextCreated();

        //////////////////////////////////////////
        void processContextWillBeDestroyed();

        //////////////////////////////////////////
        void processContextDestroyed();


        //////////////////////////////////////////
        virtual bool createGLContextWindow() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool createGLContextPixelBuffer() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool makeCurrent() MAZE_ABSTRACT;


        //////////////////////////////////////////
        void setWindow(WindowPtr const& _window);

        //////////////////////////////////////////
        void resetWindow();


    protected:
        bool m_isCreated;
        bool m_isDestroyed;

        String m_name;

        StateMachineOpenGLPtr m_stateMachine;

        ExtensionsOpenGLPtr m_extensions;

        RenderSystemOpenGLWPtr m_renderSystemWeak;
        RenderSystemOpenGL* m_renderSystemRaw;
        WindowWPtr m_window;
        Window* m_windowRaw;
        ContextOpenGLConfig m_config;

        // Do we own the OpenGL Context?
        bool m_ownsOpenGLContext;

        ModelMatricesArchitectureOpenGL m_modelMatricesArchitecture;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenGL_hpp_
//////////////////////////////////////////
