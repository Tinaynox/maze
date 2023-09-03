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
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderBufferOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    void MAZE_GL_FUNCPTR ProcessDebugOutput(
        MZGLenum _source,
        MZGLenum _type,
        MZGLuint _id,
        MZGLenum _severity,
        MZGLsizei _length,
        MZGLchar const* _message,
        void const* _userParam)
    {
        CString sourceStr = "";
        switch (_source)
        {
        case MAZE_GL_DEBUG_SOURCE_API:
            sourceStr = "MAZE_GL_DEBUG_SOURCE_API";
            break;
        case MAZE_GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceStr = "MAZE_GL_DEBUG_SOURCE_WINDOW_SYSTEM";
            break;
        case MAZE_GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceStr = "MAZE_GL_DEBUG_SOURCE_SHADER_COMPILER";
            break;
        case MAZE_GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceStr = "MAZE_GL_DEBUG_SOURCE_THIRD_PARTY";
            break;
        case MAZE_GL_DEBUG_SOURCE_APPLICATION:
            sourceStr = "MAZE_GL_DEBUG_SOURCE_APPLICATION";
            break;
        case MAZE_GL_DEBUG_SOURCE_OTHER:
            sourceStr = "MAZE_GL_DEBUG_SOURCE_OTHER";
            break;
        }

        CString severityStr = "";
        switch (_severity)
        {
        case MAZE_GL_DEBUG_SEVERITY_HIGH:
            severityStr = "MAZE_GL_DEBUG_SEVERITY_HIGH";
            break;
        case MAZE_GL_DEBUG_SEVERITY_MEDIUM:
            severityStr = "MAZE_GL_DEBUG_SEVERITY_MEDIUM";
            break;
        case MAZE_GL_DEBUG_SEVERITY_LOW:
            severityStr = "MAZE_GL_DEBUG_SEVERITY_LOW";
            break;
        case MAZE_GL_DEBUG_OUTPUT:
            severityStr = "MAZE_GL_DEBUG_SEVERITY_OUTPUT";
            break;
        }

        CString typeStr = "";
        switch (_type)
        {
        case MAZE_GL_DEBUG_TYPE_ERROR:
            typeStr = "MAZE_GL_DEBUG_TYPE_ERROR";
            break;
        case MAZE_GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeStr = "MAZE_GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            break;
        case MAZE_GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeStr = "MAZE_GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            break;
        case MAZE_GL_DEBUG_TYPE_PORTABILITY:
            typeStr = "MAZE_GL_DEBUG_TYPE_PORTABILITY";
            break;
        case MAZE_GL_DEBUG_TYPE_PERFORMANCE:
            typeStr = "MAZE_GL_DEBUG_TYPE_PERFORMANCE";
            break;
        case MAZE_GL_DEBUG_TYPE_OTHER:
            typeStr = "MAZE_GL_DEBUG_TYPE_OTHER";
            return;
            break;
        case MAZE_GL_DEBUG_TYPE_MARKER:
            typeStr = "MAZE_GL_DEBUG_TYPE_MARKER";
            break;
        case MAZE_GL_DEBUG_TYPE_PUSH_GROUP:
            typeStr = "MAZE_GL_DEBUG_TYPE_PUSH_GROUP";
            break;
        case MAZE_GL_DEBUG_TYPE_POP_GROUP:
            typeStr = "MAZE_GL_DEBUG_TYPE_POP_GROUP";
            break;
        }

        MAZE_LOG_WARNING("Source: %s; Severity: %s; Type: %s, id: %u, Message: %s", sourceStr, severityStr, typeStr, _id, _message);
    }


    //////////////////////////////////////////
    // Class ContextOpenGLScopeBind
    //
    //////////////////////////////////////////
    ContextOpenGLScopeBind::ContextOpenGLScopeBind(ContextOpenGL* _newContext, bool _includeSurface)
        : m_bind(false)
        , m_includeSurface(_includeSurface)
        , m_prevContext(nullptr)
    {

        RenderSystemOpenGL* rs =  _newContext->getRenderSystemRaw();
        m_renderSystem = rs;

        m_prevContext = rs->getCurrentContext();

        if (m_prevContext != _newContext)
        {
            m_prevRenderTarget = rs->getCurrentRenderTarget();
            _newContext->makeCurrentContext(true, _includeSurface);

            m_bind = true;
        }
    }

    //////////////////////////////////////////
    ContextOpenGLScopeBind::ContextOpenGLScopeBind(ContextOpenGLPtr const& _newContext, bool _includeSurface)
        : ContextOpenGLScopeBind(_newContext.get(), _includeSurface)
    {
    }

    //////////////////////////////////////////
    ContextOpenGLScopeBind::~ContextOpenGLScopeBind()
    {
        if (m_bind)
        {
            if (m_prevContext)
                m_prevContext->makeCurrentContext(true, m_includeSurface);

            m_renderSystem->setCurrentRenderTarget(m_prevRenderTarget);
        }
    }


    //////////////////////////////////////////
    // Class ContextOpenGL
    //
    //////////////////////////////////////////
    ContextOpenGL::ContextOpenGL()
        : m_isCreated(false)
        , m_isDestroyed(false)
        , m_renderSystemRaw(nullptr)
        , m_windowRaw(nullptr)
        , m_ownsOpenGLContext(true)
        , m_modelMatricesArchitecture(ModelMatricesArchitectureOpenGL::None)
    {
        
    }

    //////////////////////////////////////////
    ContextOpenGL::~ContextOpenGL()
    {
        m_isDestroyed = true;

        resetWindow();

        eventDestroyed(this);
    }

    //////////////////////////////////////////
    ContextOpenGLPtr ContextOpenGL::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        return _renderSystem->createContext(_window, _config);
    }

    //////////////////////////////////////////
    ContextOpenGLPtr ContextOpenGL::Create(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 _width, U32 _height,
        ContextOpenGLConfig* _config)
    {
        return _renderSystem->createContext(_width, _height, _config);
    }

    //////////////////////////////////////////
    bool ContextOpenGL::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        const WindowPtr& _window,
        ContextOpenGLConfig* _config)
    {
        m_renderSystemWeak = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        setWindow(_window);

        if (_config)
        {
            m_config = *_config;
        }

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL::init(
        RenderSystemOpenGLPtr const& _renderSystem,
        U32 width, U32 height,
        ContextOpenGLConfig* config)
    {
        m_renderSystemWeak = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        if (config)
            m_config = *config;

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenGL::createGLContext()
    {
        bool result = false;

        if (m_windowRaw)
            result = createGLContextWindow();
        else
            result = createGLContextPixelBuffer();

        if (result && m_renderSystemRaw->getSystemInited())
        {
            if (m_ownsOpenGLContext)
                setupDefaultStates();
        }

        return result;
    }

    //////////////////////////////////////////
    bool ContextOpenGL::createGLContext(WindowPtr const& _window)
    {
        setWindow(_window);
        return createGLContext();
    }

    //////////////////////////////////////////
    bool ContextOpenGL::makeCurrentContext(bool _isCurrent, bool _includeSurface)
    {
        if (_isCurrent)
            return m_renderSystemRaw->makeCurrentContext(this, _includeSurface);
        else
            return m_renderSystemRaw->makeCurrentContext(m_renderSystemRaw->getNullContext(), _includeSurface);
    }

    //////////////////////////////////////////
    void ContextOpenGL::notifyWindowWillClose(Window* _window)
    {
        destroyGLContext();
    }

    //////////////////////////////////////////
    void ContextOpenGL::notifyWindowDestroyed(Window* _window)
    {
        resetWindow();
    }

    ////////////////////////////////////
    void ContextOpenGL::processContextCreated()
    {
        flushConfig();

        m_isCreated = true;

        eventGLContextCreated(this);
    }

    //////////////////////////////////////////
    void ContextOpenGL::processContextWillBeDestroyed()
    {
        if (!m_ownsOpenGLContext)
            return;

        if (m_stateMachine)
            m_stateMachine->processContextWillBeDestroyed();

        m_modelMatricesArchitecture = ModelMatricesArchitectureOpenGL::None;

        eventGLContextWillBeDestroyed(this);
    }

    //////////////////////////////////////////
    void ContextOpenGL::processContextDestroyed()
    {
        if (!m_ownsOpenGLContext)
            return;

        eventGLContextDestroyed(this);
    }

    //////////////////////////////////////////
    S32 ContextOpenGL::EvaluateFormat(
        U32 _bitsPerPixel,
        ContextOpenGLConfig const& _config,
        S32 _colorBits,
        S32 _depthBits,
        S32 _stencilBits,
        S32 _antialiasing,
        bool _accelerated,
        bool _sRgb)
    {
        S32 colorDiff        = static_cast<S32>(_bitsPerPixel)             - _colorBits;
        S32 depthDiff        = static_cast<S32>(_config.depthBits)         - _depthBits;
        S32 stencilDiff      = static_cast<S32>(_config.stencilBits)       - _stencilBits;
        S32 antialiasingDiff = static_cast<S32>(_config.antialiasingLevel) - _antialiasing;

        colorDiff        *= ((colorDiff        > 0) ? 100000 : 1);
        depthDiff        *= ((depthDiff        > 0) ? 100000 : 1);
        stencilDiff      *= ((stencilDiff      > 0) ? 100000 : 1);
        antialiasingDiff *= ((antialiasingDiff > 0) ? 100000 : 1);

        S32 score = std::abs(colorDiff) + std::abs(depthDiff) + std::abs(stencilDiff) + std::abs(antialiasingDiff);

        if (_config.sRgbCapable && !_sRgb)
            score += 10000000;

        if (!_accelerated)
            score += 100000000;

        return score;
    }

    //////////////////////////////////////////
    void ContextOpenGL::setWindow(WindowPtr const& _window)
    {
        resetWindow();

        m_window = _window;
        m_windowRaw = _window.get();
        _window->eventWindowWillClose.subscribe(this, &ContextOpenGL::notifyWindowWillClose);
        _window->eventDestroyed.subscribe(this, &ContextOpenGL::notifyWindowDestroyed);
    }

    //////////////////////////////////////////
    void ContextOpenGL::resetWindow()
    {
        WindowPtr window = m_window.lock();
        if (window)
        {
            window->eventWindowWillClose.unsubscribe(this);
            window->eventDestroyed.unsubscribe(this);
        }

        m_window.reset();
        m_windowRaw = nullptr;
    }

    //////////////////////////////////////////
    void ContextOpenGL::syncGLStates()
    {
        m_stateMachine->syncStates();
    }

    //////////////////////////////////////////
    void ContextOpenGL::setupDefaultStates()
    {
        MAZE_PROFILE_EVENT("ContextOpenGL::setupDefaultStates");

#if (MAZE_DEBUG_GL)
        
        if (getExtensionsRaw()->hasGLExtension("GL_KHR_debug"))
        {
            Debug::Log("OpenGL KHR_debug log callback enabled");
            MAZE_GL_CALL(mzglEnable(MAZE_GL_DEBUG_OUTPUT_KHR));
            MAZE_GL_CALL(mzglEnable(MAZE_GL_DEBUG_OUTPUT_SYNCHRONOUS_KHR));
            MAZE_GL_CALL(mzglDebugMessageCallbackKHR(ProcessDebugOutput, 0));
            MAZE_GL_CALL(mzglDebugMessageControlKHR(MAZE_GL_DONT_CARE, MAZE_GL_DONT_CARE, MAZE_GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, MAZE_GL_FALSE));
        }
        else
        if (mzglDebugMessageCallback && mzglDebugMessageControl)
        {
            Debug::Log("OpenGL debug log callback enabled");
            MAZE_GL_CALL(mzglEnable(MAZE_GL_DEBUG_OUTPUT));
            MAZE_GL_CALL(mzglEnable(MAZE_GL_DEBUG_OUTPUT_SYNCHRONOUS));
            MAZE_GL_CALL(mzglDebugMessageCallback(ProcessDebugOutput, 0));
            MAZE_GL_CALL(mzglDebugMessageControl(MAZE_GL_DONT_CARE, MAZE_GL_DONT_CARE, MAZE_GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, MAZE_GL_FALSE));
        }
        else
        {
            Debug::Log("OpenGL debug log callback is not enabled");
        }
#endif

        if (getExtensionsRaw()->getSupportArbBufferStorage() && 0) // #TODO: UniformTexture streams
        {
            Debug::Log("Model Matrices Arch: Uniform Texture");
            m_modelMatricesArchitecture = ModelMatricesArchitectureOpenGL::UniformTexture;
        }
        else
        {
            Debug::Log("Model Matrices Arch: Uniform Array");
            m_modelMatricesArchitecture = ModelMatricesArchitectureOpenGL::UniformArray;
        }

        m_stateMachine->setup();

        eventGLContextSetup(this);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setViewportRect(Rect2DS const& _viewportRect)
    {
        m_stateMachine->setViewportRect(_viewportRect);
    }

    //////////////////////////////////////////
    void ContextOpenGL::activeTexture(S32 _index)
    {
        MZGLenum activeTexture = MAZE_GL_TEXTURE0 + _index;
        m_stateMachine->activeTexture(activeTexture);
    }    

    //////////////////////////////////////////
    void ContextOpenGL::setBlendFactors(BlendFactor _blendSrcFactor, BlendFactor _blendDestFactor)
    {
        m_stateMachine->setBlendFactors(_blendSrcFactor, _blendDestFactor);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setBlendEnabled(bool _value)
    {
        m_stateMachine->setBlendEnabled(_value);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setDepthTestEnabled(bool _depthTestEnabled)
    {
        m_stateMachine->setDepthTestEnabled(_depthTestEnabled);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setDepthTestCompareFunction(CompareFunction _depthTestCompareFunction)
    {
        m_stateMachine->setDepthTestCompareFunction(_depthTestCompareFunction);
    }
    
    //////////////////////////////////////////
    void ContextOpenGL::setDepthWriteEnabled(bool _depthWriteEnabled)
    {
        m_stateMachine->setDepthWriteEnabled(_depthWriteEnabled);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setCullEnabled(bool _cullEnabled)
    {
        m_stateMachine->setCullEnabled(_cullEnabled);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setCullMode(CullMode _cullMode)
    {
        m_stateMachine->setCullMode(_cullMode);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setWireframeRender(bool _value)
    {
        m_stateMachine->setWireframeRender(_value);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setScissorRect(Rect2DS const& _scissorRect)
    {
        m_stateMachine->setScissorRect(_scissorRect);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setScissorTestEnabled(bool _value)
    {
        m_stateMachine->setScissorTestEnabled(_value);
    }

    //////////////////////////////////////////
    void ContextOpenGL::setClipDistanceEnabled(S32 _i, bool _value)
    {
        m_stateMachine->setClipDistanceEnabled(_i, _value);
    }

} // namespace Maze
//////////////////////////////////////////
