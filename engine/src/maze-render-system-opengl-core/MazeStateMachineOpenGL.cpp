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
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeBlendModeOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeCullModeOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeCompareFunctionOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTextureCubeOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class StateMachineOpenGL
    //
    //////////////////////////////////////////
    StateMachineOpenGL::StateMachineOpenGL(ContextOpenGL* _context)
        : m_context(_context)
        , m_activeTexture(MAZE_GL_TEXTURE0)
        , m_bindTextureIds{ 0 }
        , m_bindTextureTargets{ 0 }
        , m_clipDistances{ 0 }
        , m_program(0)
        , m_frameBuffer(0)
        , m_vertexArrayObject(0)
        , m_viewportRect(0, 0, 0, 0)
        , m_scissorTest(false)        
        , m_scissorRect(0, 0, 0, 0)
        , m_clearColor(1.0f, 0.0f, 1.0f, 1.0f)
        , m_clearDepth(1.0f)
        , m_blendEnabled(false)
        , m_blendSrcFactor(BlendFactor::One)
        , m_blendDestFactor(BlendFactor::Zero)
        , m_depthTestEnabled(false)
        , m_depthTestCompareFunction(CompareFunction::None)
        , m_depthWriteEnabled(false)
        , m_cullEnabled(false)
        , m_cullMode(CullMode::None)
        , m_multiSampleEnabled(false)
        , m_wireframeRender(false)
        , m_antialiasingLevelSupport(0)
        , m_sRgbSupport(false)
        , m_currentShader(nullptr)
        , m_currentTextures{ nullptr }
        , m_currentRenderBuffer(nullptr)
        , m_pixelBufferSize(Vec2DU::c_zero)
    {
        m_context->eventGLContextCreated.subscribe(this, &StateMachineOpenGL::notifyGLContextCreated);
        m_context->eventGLContextWillBeDestroyed.subscribe(this, &StateMachineOpenGL::notifyGLContextWillBeDestroyed);
    }

    //////////////////////////////////////////
    StateMachineOpenGL::~StateMachineOpenGL()
    {
        m_context->eventGLContextCreated.unsubscribe(this);
        m_context->eventGLContextWillBeDestroyed.unsubscribe(this);
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setup()
    {
        ContextOpenGLScopeBind scopeBind(m_context);

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        syncStates();

        if (mzglFrontFace)
        {
            // LHCS (It's actually CCW, but because of inverted Z axis we should set CW)
            MAZE_GL_CALL(mzglFrontFace(MAZE_GL_CW));
        }

        MAZE_GL_CALL(mzglEnable(MAZE_GL_CLIP_DISTANCE0));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::syncStates()
    {
        RenderSystemOpenGL* renderSystem = m_context->getRenderSystemRaw();
        RenderTarget* prevRenderTarget = renderSystem->getCurrentRenderTarget();
        ContextOpenGL* prevRenderContext = renderSystem->getCurrentContext();

        if (prevRenderContext != m_context)
        {
            MAZE_ERROR_RETURN_IF(!m_context->makeCurrentContext(true), "Context cannot be set!");
        }

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif


        MAZE_GL_MUTEX_SCOPED_LOCK(renderSystem);

        for (U32 i = 0; i < MAZE_GL_MAX_TEXTURES_COUNT; ++i)
        {
            if (mzglActiveTexture && mzglBindTexture)
            {
                if (m_bindTextureTargets[i])
                {
                    MAZE_GL_CALL(mzglActiveTexture(MAZE_GL_TEXTURE0 + i));
                    MAZE_GL_CALL(mzglBindTexture(m_bindTextureTargets[i], m_bindTextureIds[i]));
                }
            }
        }

        if (mzglActiveTexture)
        {
            MAZE_GL_CALL(mzglActiveTexture(MAZE_GL_TEXTURE0));
        }
        m_activeTexture = MAZE_GL_TEXTURE0;


        if (m_program)
        {
            if (mzglUseProgram)
            {
                MAZE_GL_CALL(mzglUseProgram(m_program));
            }
            else
            {
                m_program = 0;
            }
        }
        else
        {
            m_program = 0;
        }

        for (S32 i = 0; i < MAZE_GL_MAX_CLIP_DISTANCES_COUNT; ++i)
        {
            if (m_clipDistances[i])
            {
                if (mzglEnable)
                    MAZE_GL_CALL(mzglEnable(MAZE_GL_CLIP_DISTANCE0 + i));
                else
                    m_clipDistances[i] = 0;
            }
            else
            {
                if (mzglDisable)
                    MAZE_GL_CALL(mzglDisable(MAZE_GL_CLIP_DISTANCE0 + i));
            }
        }
        
        if (m_blendEnabled)
        {
            if (mzglEnable)
            {
                MAZE_GL_CALL(mzglEnable(MAZE_GL_BLEND));
            }
            else
            {
                m_blendEnabled = false;
            }
        }
        else
        {
            if (mzglDisable)
            {
                MAZE_GL_CALL(mzglDisable(MAZE_GL_BLEND));
            }
        }

        if (m_depthTestEnabled)
        {
            if (mzglEnable)
            {
                MAZE_GL_CALL(mzglEnable(MAZE_GL_DEPTH_TEST));
            }
            else
            {
                m_depthTestEnabled = false;
            }
        }
        else
        {
            if (mzglDisable)
            {
                MAZE_GL_CALL(mzglDisable(MAZE_GL_DEPTH_TEST));
            }
        }
        
        if (m_depthWriteEnabled)
        {
            if (mzglDepthMask)
                MAZE_GL_CALL(mzglDepthMask(MAZE_GL_TRUE));
        }
        else
        {
            if (mzglDepthMask)
                MAZE_GL_CALL(mzglDepthMask(MAZE_GL_FALSE));
        }

        if (m_cullEnabled)
        {
            if (mzglEnable)
                MAZE_GL_CALL(mzglEnable(MAZE_GL_CULL_FACE));
        }
        else
        {
            if (mzglDisable)
                MAZE_GL_CALL(mzglDisable(MAZE_GL_CULL_FACE));
        }
        
        if (mzglCullFace)
        {
            if (m_cullMode != CullMode::None)
            {
                MZGLenum cullModeGL = GetOpenGLCullMode(m_cullMode);
                MAZE_GL_CALL(mzglCullFace(cullModeGL));
            }
        }


        if (mzglViewport)
        {
            MAZE_GL_CALL(
                mzglViewport(
                    m_viewportRect.position.x,
                    m_viewportRect.position.y,
                    m_viewportRect.size.x,
                    m_viewportRect.size.y));
        }

        if (m_scissorTest)
        {        
            if (mzglEnable)
            {
                MAZE_GL_CALL(mzglEnable(MAZE_GL_SCISSOR_TEST));
            }
            else
            {
                m_scissorTest = false;
            }
        }
        else
        {
            if (mzglDisable)
            {
                MAZE_GL_CALL(mzglDisable(MAZE_GL_SCISSOR_TEST));
            }
        }
        
        if (mzglScissor)
        {
            MAZE_GL_CALL(
                mzglScissor(
                    m_scissorRect.position.x,
                    m_scissorRect.position.y,
                    m_scissorRect.size.x,
                    m_scissorRect.size.y));
        }

        if (mzglBindFramebuffer)
        {
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_FRAMEBUFFER, m_frameBuffer));
        }

        if (mzglBindVertexArray)
        {
            MAZE_GL_CALL(mzglBindVertexArray(m_vertexArrayObject));
        }
               
        if (mzglBindBuffer)
        {
            //MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_ARRAY_BUFFER, m_arrayBuffer));
            //MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_ELEMENT_ARRAY_BUFFER, m_elementArrayBuffer));
        }


        if (mzglClearColor)
        {
            MAZE_GL_CALL(
                mzglClearColor(
                    m_clearColor.x,
                    m_clearColor.y,
                    m_clearColor.z,
                    m_clearColor.w));
        }

        if (mzglClearDepth)
        {
            MAZE_GL_CALL(
                mzglClearDepth(
                    m_clearDepth));
        }

        if (mzglPolygonMode)
        {
            if (m_wireframeRender)
                MAZE_GL_CALL(mzglPolygonMode(MAZE_GL_FRONT_AND_BACK, MAZE_GL_LINE));
            else
                MAZE_GL_CALL(mzglPolygonMode(MAZE_GL_FRONT_AND_BACK, MAZE_GL_FILL));
        }
        

        if (mzglBlendFunc)
        {
            MZGLenum srcFactor = GetOpenGLBlendFactor(m_blendSrcFactor);
            MZGLenum destFactor = GetOpenGLBlendFactor(m_blendDestFactor);
            MAZE_GL_CALL(mzglBlendFunc(srcFactor, destFactor));
        }


        if (   m_context->getExtensionsRaw()
            && m_context->getExtensionsRaw()->getSupportMultisample())
        {
            if (m_multiSampleEnabled)
            {
                if (mzglEnable)
                {
                    MAZE_GL_CALL(mzglEnable(MAZE_GL_MULTISAMPLE));
                }
                else
                {
                    m_multiSampleEnabled = false;
                }
            }
            else
            {
                if (mzglDisable)
                {
                    MAZE_GL_CALL(mzglDisable(MAZE_GL_MULTISAMPLE));
                }
            }
        }



        if (prevRenderContext)
        {
            renderSystem->makeCurrentContext(prevRenderContext);
            renderSystem->setCurrentRenderTarget(prevRenderTarget);
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::notifyGLContextCreated(ContextOpenGL* _contextOpenGL)
    {
        syncStates();
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::notifyGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_activeTexture = MAZE_GL_TEXTURE0;

        for (S32 i = 0; i < MAZE_GL_MAX_TEXTURES_COUNT; ++i)
        {
            m_bindTextureTargets[i] = 0;
            m_bindTextureIds[i] = 0;
        }

        for (S32 i = 0; i < MAZE_GL_MAX_CLIP_DISTANCES_COUNT; ++i)
        {
            m_clipDistances[i] = 0;
        }
            
        m_program = 0;
        m_frameBuffer = 0;
        m_vertexArrayObject = 0;
        m_viewportRect = Rect2DS(0, 0, 0, 0);
        m_scissorTest = false;
        m_scissorRect = Rect2DS(0, 0, 0, 0);
        m_clearColor = Vec4DF(1.0f, 0.0f, 1.0f, 1.0f);
        m_clearDepth = 1.0f;
        m_blendEnabled = false;
        m_blendSrcFactor = BlendFactor::One;
        m_blendDestFactor = BlendFactor::Zero;
        m_depthTestEnabled = false;
        m_depthTestCompareFunction = CompareFunction::None;
        m_depthWriteEnabled = false;
        m_cullEnabled = false;
        m_cullMode = CullMode::None;
        m_multiSampleEnabled = false;
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::activeTexture(MZGLenum _activeTexture)
    {
        if (m_activeTexture == _activeTexture)
            return;

        MAZE_DEBUG_ERROR_RETURN_IF(_activeTexture < MAZE_GL_TEXTURE0 || _activeTexture > MAZE_GL_TEXTURE15, "Invalid active texture index!");


        m_activeTexture = _activeTexture;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif


        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglActiveTexture(_activeTexture));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindTexture(MZGLenum _textureTarget, MZGLuint _textureId)
    {
        MAZE_DEBUG_ERROR_IF(_textureId == 0, "Texture is not created yet!");

        S32 activeTextureIndex = m_activeTexture - MAZE_GL_TEXTURE0;

        if (m_bindTextureIds[activeTextureIndex] == _textureId)
            return;

        m_bindTextureTargets[activeTextureIndex] = _textureTarget;
        m_bindTextureIds[activeTextureIndex] = _textureId;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglBindTexture(_textureTarget, _textureId));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setBlendEnabled(bool _value)
    {
        if (m_blendEnabled == _value)
            return;

        m_blendEnabled = _value;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif


        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        if (_value)
        {
            MAZE_GL_CALL(mzglEnable(MAZE_GL_BLEND));
        }
        else
        {
            MAZE_GL_CALL(mzglDisable(MAZE_GL_BLEND));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setClipDistanceEnabled(S32 _i, bool _value)
    {
        if (m_clipDistances[_i] == _value)
            return;

        m_clipDistances[_i] = _value;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        if (_value)
        {
            MAZE_GL_CALL(mzglEnable(MAZE_GL_CLIP_DISTANCE0 + _i));
        }
        else
        {
            MAZE_GL_CALL(mzglDisable(MAZE_GL_CLIP_DISTANCE0 + _i));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setBlendFactors(BlendFactor _blendSrcFactor, BlendFactor _blendDestFactor)
    {
        if (m_blendSrcFactor != _blendSrcFactor || m_blendDestFactor != _blendDestFactor)
        {
            m_blendSrcFactor = _blendSrcFactor;
            m_blendDestFactor = _blendDestFactor;

            MZGLenum srcFactor = GetOpenGLBlendFactor(_blendSrcFactor);
            MZGLenum destFactor = GetOpenGLBlendFactor(_blendDestFactor);

#if (MAZE_DEBUG_GL)
            m_context->_validateIsCurrentGLContext();
#endif

            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            MAZE_GL_CALL(mzglBlendFunc(srcFactor, destFactor));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setDepthTestEnabled(bool _depthTestEnabled)
    {
        if (m_depthTestEnabled == _depthTestEnabled)
            return;

        m_depthTestEnabled = _depthTestEnabled;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        if (_depthTestEnabled)
        {
            MAZE_GL_CALL(mzglEnable(MAZE_GL_DEPTH_TEST));
        }
        else
        {
            MAZE_GL_CALL(mzglDisable(MAZE_GL_DEPTH_TEST));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setDepthTestCompareFunction(CompareFunction _depthTestCompareFunction)
    {
        if (m_depthTestCompareFunction == _depthTestCompareFunction)
            return;

        m_depthTestCompareFunction = _depthTestCompareFunction;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MZGLenum depthTestCompareFunctionGL = GetOpenGLCompareFunction(_depthTestCompareFunction);

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglDepthFunc(depthTestCompareFunctionGL));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setDepthWriteEnabled(bool _depthWriteEnabled)
    {
        if (m_depthWriteEnabled == _depthWriteEnabled)
            return;

        m_depthWriteEnabled = _depthWriteEnabled;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        if (_depthWriteEnabled)
        {
            MAZE_GL_CALL(mzglDepthMask(MAZE_GL_TRUE));
        }
        else
        {
            MAZE_GL_CALL(mzglDepthMask(MAZE_GL_FALSE));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setCullEnabled(bool _cullEnabled)
    {
        if (m_cullEnabled == _cullEnabled)
            return;

        m_cullEnabled = _cullEnabled;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        if (_cullEnabled)
        {
            MAZE_GL_CALL(mzglEnable(MAZE_GL_CULL_FACE));
        }
        else
        {
            MAZE_GL_CALL(mzglDisable(MAZE_GL_CULL_FACE));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setCullMode(CullMode _cullMode)
    {
        if (m_cullMode == _cullMode)
            return;

        m_cullMode = _cullMode;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        MZGLenum cullModeGL = GetOpenGLCullMode(_cullMode);

        MAZE_GL_CALL(mzglCullFace(cullModeGL));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setViewportRect(Rect2DS const& _viewportRect)
    {
        if (m_viewportRect == _viewportRect)
            return;

        m_viewportRect = _viewportRect;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(
            mzglViewport(
                _viewportRect.position.x,
                _viewportRect.position.y,
                _viewportRect.size.x,
                _viewportRect.size.y));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setScissorTestEnabled(bool _value)
    {
        if (m_scissorTest == _value)
            return;

        m_scissorTest = _value;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        if (_value)
        {
            MAZE_GL_CALL(mzglEnable(MAZE_GL_SCISSOR_TEST));
        }
        else
        {
            MAZE_GL_CALL(mzglDisable(MAZE_GL_SCISSOR_TEST));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setScissorRect(Rect2DS const& _scissorRect)
    {
        if (m_scissorRect == _scissorRect)
            return;

        m_scissorRect = _scissorRect;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(
            mzglScissor(
                _scissorRect.position.x,
                _scissorRect.position.y,
                _scissorRect.size.x,
                _scissorRect.size.y));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindFrameBuffer(MZGLuint _frameBuffer)
    {
        if (m_frameBuffer == _frameBuffer)
        {

#if (MAZE_DEBUG_GL)

            MZGLint currentValue;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_DRAW_FRAMEBUFFER_BINDING, &currentValue));
            MAZE_DEBUG_BP_IF(currentValue != _frameBuffer);

#endif

            return;
        }

        m_frameBuffer = _frameBuffer;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_FRAMEBUFFER, m_frameBuffer));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindVertexArrayObject(MZGLuint _vertexArrayObject)
    {
        if (m_vertexArrayObject == _vertexArrayObject)
            return;

        m_vertexArrayObject = _vertexArrayObject;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglBindVertexArray(m_vertexArrayObject));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindArrayBuffer(MZGLuint _arrayBuffer)
    {
        
#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_ARRAY_BUFFER, _arrayBuffer));
    }

    //////////////////////////////////////////
    MZGLuint StateMachineOpenGL::getBindedArrayBuffer() const
    {
        MZGLint buffer;
        MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_ARRAY_BUFFER_BINDING, &buffer));
        return static_cast<MZGLuint>(buffer);
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindElementArrayBuffer(MZGLuint _elementArrayBuffer)
    {
        
#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_ELEMENT_ARRAY_BUFFER, _elementArrayBuffer));
    }

    //////////////////////////////////////////
    MZGLuint StateMachineOpenGL::getBindedElementArrayBuffer() const
    {
        MZGLint buffer;
        MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_ELEMENT_ARRAY_BUFFER_BINDING, &buffer));
        return static_cast<MZGLuint>(buffer);
    }

    
    //////////////////////////////////////////
    void StateMachineOpenGL::setClearColor(Vec4DF const& _clearColor)
    {
        if (m_clearColor == _clearColor)
            return;

        m_clearColor = _clearColor;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        MAZE_GL_CALL(mzglClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w));
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setClearDepth(F32 _clearDepth)
    {
        if (m_clearDepth == _clearDepth)
        {
            /*
#if (MAZE_DEBUG_GL)
            m_context->_validateIsCurrentGLContext();

            MZGLfloat value;
            MAZE_GL_CALL(mzglGetFloatv(MAZE_GL_DEPTH_CLEAR_VALUE, &value));
            MAZE_ERROR_RETURN_IF(value != _clearDepth, "State machine is out of sync!");
#endif
            */
            //return;
        }

        m_clearDepth = _clearDepth;

#if (MAZE_DEBUG_GL)
        m_context->_validateIsCurrentGLContext();
#endif

        if (mzglClearDepth)
        {
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

            MAZE_GL_CALL(mzglClearDepth(m_clearDepth));
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setWireframeRender(bool _value)
    {
        if (m_wireframeRender == _value)
            return;

        m_wireframeRender = _value;

        if (m_wireframeRender)
            mzglPolygonMode(MAZE_GL_FRONT_AND_BACK, MAZE_GL_LINE);
        else
            mzglPolygonMode(MAZE_GL_FRONT_AND_BACK, MAZE_GL_FILL);
    }


    //////////////////////////////////////////
    void StateMachineOpenGL::setMultiSampleEnabled(MZGLboolean _value)
    {
        if (m_multiSampleEnabled == _value)
            return;

        m_multiSampleEnabled = _value;

        if (m_context->getExtensions()->getSupportMultisample())
        {

#if (MAZE_DEBUG_GL)
            m_context->_validateIsCurrentGLContext();
#endif


            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

            if (_value)
            {
                MAZE_GL_CALL(mzglEnable(MAZE_GL_MULTISAMPLE));
            }
            else
            {
                MAZE_GL_CALL(mzglDisable(MAZE_GL_MULTISAMPLE));
            }
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::useProgram(MZGLuint _program)
    {
        if (m_program == _program)
            return;

        m_program = _program;

        if (m_program)
        {
            if (mzglUseProgram)
            {
                MAZE_GL_CALL(mzglUseProgram(m_program));
            }
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setCurrentShader(Shader* _shader)
    {
        if (m_currentShader == _shader)
            return;

#if (MAZE_DEBUG_GL)
        MAZE_RETURN_IF(!m_context->_validateIsCurrentGLContext());
#endif

        useProgram(_shader ? _shader->castRaw<ShaderOpenGL>()->getProgramId() : 0);

        m_currentShader = _shader;
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::setCurrentRenderBuffer(RenderBuffer* _renderBuffer)
    {
        if (m_currentRenderBuffer == _renderBuffer)
        {
#if (MAZE_DEBUG_GL)

            MZGLint targetValue = _renderBuffer ? _renderBuffer->castRaw<RenderBufferOpenGL>()->getFramebufferId() : 0;

            MZGLint currentValue;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_DRAW_FRAMEBUFFER_BINDING, &currentValue));
            MAZE_DEBUG_BP_IF(currentValue != targetValue);

#endif
            return;
        }

#if (MAZE_DEBUG_GL)
        MAZE_RETURN_IF(!m_context->_validateIsCurrentGLContext());
#endif

        bindFrameBuffer(_renderBuffer ? _renderBuffer->castRaw<RenderBufferOpenGL>()->getFramebufferId() : 0);

        m_currentRenderBuffer = _renderBuffer;
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindTexture(Texture* _texture)
    {
        if (!_texture)
        {
            MZGLenum activeTexture = getActiveTexture();
            Size activeTextureIndex = activeTexture - MAZE_GL_TEXTURE0;

            Texture* currentTexture = m_currentTextures[activeTextureIndex];
            if (currentTexture == _texture)
                return;

            m_currentTextures[activeTextureIndex] = _texture;

            return;
        }

        switch (_texture->getType())
        {
            case TextureType::TwoDimensional:
            {
                bindTexture2D(_texture->castRaw<Texture2D>());
                break;
            }
            case TextureType::Cube:
            {
                bindTextureCube(_texture->castRaw<TextureCube>());
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindTexture2D(Texture2D* _texture)
    {
        MZGLenum activeTexture = getActiveTexture();
        Size activeTextureIndex = activeTexture - MAZE_GL_TEXTURE0;

        Texture* currentTexture = m_currentTextures[activeTextureIndex];
        if (currentTexture == _texture)
            return;

        m_currentTextures[activeTextureIndex] = _texture;

        if (_texture)
            bindTexture2D(_texture->castRaw<Texture2DOpenGL>()->getGLTexture());
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::bindTextureCube(TextureCube* _texture)
    {
        MZGLenum activeTexture = getActiveTexture();
        Size activeTextureIndex = activeTexture - MAZE_GL_TEXTURE0;

        Texture* currentTexture = m_currentTextures[activeTextureIndex];
        if (currentTexture == _texture)
            return;

        m_currentTextures[activeTextureIndex] = _texture;

        if (_texture)
            bindTextureCube(_texture->castRaw<TextureCubeOpenGL>()->getGLTexture());
    }

    //////////////////////////////////////////
    void StateMachineOpenGL::processContextWillBeDestroyed()
    {
        m_antialiasingLevelSupport = 0;
        m_sRgbSupport = false;
        m_currentShader = nullptr;
        for (Size i = 0; i < MAZE_GL_MAX_TEXTURES_COUNT; ++i)
            m_currentTextures[i] = nullptr;
    }

} // namespace Maze
//////////////////////////////////////////
