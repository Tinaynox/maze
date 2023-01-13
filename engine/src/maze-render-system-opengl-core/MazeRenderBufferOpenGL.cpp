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
#include "maze-render-system-opengl-core/MazeRenderBufferOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DMSOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeMetaClassHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderBufferOpenGLScopeBind
    //
    //////////////////////////////////////////
    RenderBufferOpenGLScopeBind::RenderBufferOpenGLScopeBind(RenderBuffer* _newRenderBuffer)
        : m_context(nullptr)
    {
        RenderBufferOpenGL* renderBufferOpenGL = _newRenderBuffer->castRaw<RenderBufferOpenGL>();
        RenderSystemOpenGL* rs = renderBufferOpenGL->getRenderSystemOpenGLRaw();
        ContextOpenGL* context = rs->ensureCurrentContext();
        if (context)
        {
            m_prevRenderBuffer = context->getCurrentRenderBuffer();

            if (m_prevRenderBuffer != _newRenderBuffer)
            {
                context->setCurrentRenderBuffer(_newRenderBuffer);
                m_context = context;
            }
        }
    }

    //////////////////////////////////////////
    RenderBufferOpenGLScopeBind::~RenderBufferOpenGLScopeBind()
    {
        if (m_context)
        {
            m_context->setCurrentRenderBuffer(m_prevRenderBuffer);
        }
    }

    //////////////////////////////////////////
    // Class RenderBufferOpenGL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderBufferOpenGL, RenderBuffer);

    //////////////////////////////////////////
    RenderBufferOpenGL::RenderBufferOpenGL()
        : m_context(nullptr)
        , m_frameBufferId(0)
    {

    }

    //////////////////////////////////////////
    RenderBufferOpenGL::~RenderBufferOpenGL()
    {
        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBufferOpenGL::Create(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL,
        RenderBufferDeleter const& _deleter)
    {
        RenderBufferOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR_EX(RenderBufferOpenGL, object, _deleter, init(_renderSystem, _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBufferOpenGL::Create(
        RenderBufferOpenGLPtr const& _renderBuffer,
        RenderBufferDeleter const& _deleter)
    {
        RenderBufferOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR_EX(RenderBufferOpenGL, object, _deleter, init(_renderBuffer));
        return object;
    }

    //////////////////////////////////////////
    bool RenderBufferOpenGL::init(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        if (!RenderBuffer::init(_renderSystem))
            return false;

        if (!_contextOpenGL)
            return false;

        if (!_contextOpenGL->isValid())
            return false;

        setContextOpenGL(_contextOpenGL);

        generateGLObjects();

        return true;
    }

    //////////////////////////////////////////
    bool RenderBufferOpenGL::init(
        RenderBufferOpenGLPtr const& _renderBuffer)
    {
        if (!RenderBuffer::init(_renderBuffer))
            return false;

        setContextOpenGL(_renderBuffer->m_context);

        generateGLObjects();

        return true;
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBufferOpenGL::createCopy()
    {
        return RenderBufferOpenGL::Create(cast<RenderBufferOpenGL>());
    }

    //////////////////////////////////////////
    RenderSystemOpenGL* RenderBufferOpenGL::getRenderSystemOpenGLRaw()
    {
        return m_renderSystem->castRaw<RenderSystemOpenGL>();
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            deleteGLObjects();

            m_context->eventDestroyed.unsubscribe(this);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this);
            m_context->eventGLContextSetup.unsubscribe(this);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &RenderBufferOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventDestroyed.updateDelegatesList();
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &RenderBufferOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextWillBeDestroyed.updateDelegatesList();
            m_context->eventGLContextSetup.subscribe(this, &RenderBufferOpenGL::notifyContextOpenGLContextSetup);
            m_context->eventGLContextSetup.updateDelegatesList();

            m_renderQueue = RenderQueueOpenGL::Create(this, m_context);
        }
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;
        m_frameBufferId = 0;
        
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_frameBufferId = 0;
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        generateGLObjects();

        for (Size i = 0; i < c_renderBufferColorTexturesMax; ++i)
        {
            reloadTexture(m_colorTextures[i]);
            setColorTexture((U32)i, m_colorTextures[i]);
        }

        reloadTexture(m_depthTexture);
        setDepthTexture(m_depthTexture);

        reloadTexture(m_stencilTexture);
        setStencilTexture(m_stencilTexture);
    }

    //////////////////////////////////////////
    bool RenderBufferOpenGL::processRenderTargetWillSet()
    {
        if (!m_context)
            return false;

        if (!m_context->makeCurrentContext(true, true))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::processRenderTargetSet()
    {
        m_context->setCurrentRenderBuffer(this);
    }
        
    //////////////////////////////////////////
    void RenderBufferOpenGL::processRenderTargetWillReset()
    {
    }

    //////////////////////////////////////////
    bool RenderBufferOpenGL::setSize(Vec2DU const& _size)
    {
        if (!RenderBuffer::setSize(_size))
            return false;

        for (Size i = 0; i < c_renderBufferColorTexturesMax; ++i)
            resizeTexture(m_colorTextures[i], _size);

        resizeTexture(m_depthTexture, _size);
        resizeTexture(m_stencilTexture, _size);

        eventRenderBufferSizeChanged(cast<RenderBuffer>());

        return true;
    }

    //////////////////////////////////////////
    bool RenderBufferOpenGL::beginDraw()
    {
        if (!m_context->isValid())
            return false;

        RenderBuffer::beginDraw();

        MZGLenum buffers[c_renderBufferColorTexturesMax] = {0};
        U32 buffersCount = 0;
        S32 i = 0;
        for (; i < c_renderBufferColorTexturesMax; ++i)
        {
            if (m_colorTextures[i])
            {
                buffers[buffersCount] = MAZE_GL_COLOR_ATTACHMENT0 + i;
                ++buffersCount;
            }
        }

        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

        if (m_renderSystem->getShaderSystem()->cast<ShaderSystemOpenGL>()->getGLSLVersion() > 120)
        {
            MAZE_GL_CALL(mzglDrawBuffers(buffersCount, buffers));
        }
        else
        {
            MZGLenum const buffer[c_renderBufferColorTexturesMax] = {MAZE_GL_COLOR_ATTACHMENT0};
            MAZE_GL_CALL(mzglDrawBuffers(1, buffer));
        }

        return true;
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::endDraw()
    {
        RenderBuffer::endDraw();

//#if ((MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS) || \
//     (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN))
        mzglFlush();
//#endif

        eventRenderBufferEndDraw(this);
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::blit(RenderBufferPtr const& _srcBuffer)
    {
        TexturePtr const& destColorTexture = getColorTexture(0);
        TexturePtr const& srcColorTexture = _srcBuffer->getColorTexture(0);
        MAZE_ERROR_RETURN_IF(!destColorTexture, "Color Texture is null");
        MAZE_ERROR_RETURN_IF(!srcColorTexture, "Color Texture in src buffer is null");

        Vec2DS destColorTextureSize;
        switch (destColorTexture->getType())
        {
            case TextureType::TwoDimensional:
            {
                destColorTextureSize = destColorTexture->castRaw<Texture2D>()->getSize();
                break;
            }
            case TextureType::TwoDimensionalMultisample:
            {
                destColorTextureSize = destColorTexture->castRaw<Texture2DMS>()->getSize();
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }

        Vec2DS srcColorTextureSize;
        switch (srcColorTexture->getType())
        {
            case TextureType::TwoDimensional:
            {
                srcColorTextureSize = srcColorTexture->castRaw<Texture2D>()->getSize();
                break;
            }
            case TextureType::TwoDimensionalMultisample:
            {
                srcColorTextureSize = srcColorTexture->castRaw<Texture2DMS>()->getSize();
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }

        MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_READ_FRAMEBUFFER, _srcBuffer->castRaw<RenderBufferOpenGL>()->m_frameBufferId));
        MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_DRAW_FRAMEBUFFER, m_frameBufferId));

        MAZE_GL_CALL(mzglBlitFramebuffer(
            0, 0, srcColorTextureSize.x, srcColorTextureSize.y,
            0, 0, destColorTextureSize.x, destColorTextureSize.y,
            MAZE_GL_COLOR_BUFFER_BIT, MAZE_GL_NEAREST));
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::generateGLObjects()
    {
        deleteGLObjects();

        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

        ContextOpenGLScopeBind contextScopeBind(m_context);
        MAZE_GL_CALL(mzglGenFramebuffers(1, &m_frameBufferId));    
    }
    
    //////////////////////////////////////////
    void RenderBufferOpenGL::deleteGLObjects()
    {
        if (m_frameBufferId == 0)
            return;

        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());

        ContextOpenGLScopeBind contextScopeBind(m_context);

        if (m_context->getBindedFrameBuffer() == m_frameBufferId)
            m_context->setCurrentRenderBuffer(nullptr);


        MAZE_GL_CALL(mzglDeleteFramebuffers(1, &m_frameBufferId));
        
#if (MAZE_PLATFORM == MAZE_PLATFORM_IOS)
        if (IsApplicationForeground())
#endif
        {
            MAZE_GL_CALL(mzglFinish());
        }    


        m_frameBufferId = 0;
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::setColorTexture(U32 _index, TexturePtr const& _texture)
    {
        RenderBuffer::setColorTexture(_index, _texture);

        {
            MZGLint textureId = 0;
            MZGLenum textureTarget = MAZE_GL_TEXTURE_2D;

            if (_texture)
            {
                switch (_texture->getType())
                {
                    case TextureType::TwoDimensional:
                    {
                        textureId = _texture->castRaw<Texture2DOpenGL>()->getGLTexture();
                        textureTarget = MAZE_GL_TEXTURE_2D;
                        break;
                    }
                    case TextureType::TwoDimensionalMultisample:
                    {
                        textureId = _texture->castRaw<Texture2DMSOpenGL>()->getGLTexture();
                        textureTarget = MAZE_GL_TEXTURE_2D_MULTISAMPLE;
                        break;
                    }
                    default:
                    {
                        MAZE_NOT_IMPLEMENTED;
                    }
                }
            }

            if (textureId)
            {
                
                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
                ContextOpenGLScopeBind contextScopeBind(m_context);
                RenderBufferOpenGLScopeBind renderBufferScopeBind(this);

                S32 activeTextureIndex = m_context->getActiveTexture();
                m_context->activeTexture(0);            

                MAZE_GL_CALL(
                    mzglFramebufferTexture2D(
                        MAZE_GL_FRAMEBUFFER,
                        MAZE_GL_COLOR_ATTACHMENT0 + _index,
                        textureTarget,
                        textureId, 0));

                m_context->activeTexture(activeTextureIndex);
            }
            
        }
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::setDepthTexture(TexturePtr const& _texture)
    {
        RenderBuffer::setDepthTexture(_texture);

        {
            MZGLint textureId = 0;
            MZGLenum textureTarget = MAZE_GL_TEXTURE_2D;

            if (_texture)
            {
                switch (_texture->getType())
                {
                    case TextureType::TwoDimensional:
                    {
                        textureId = _texture->castRaw<Texture2DOpenGL>()->getGLTexture();
                        textureTarget = MAZE_GL_TEXTURE_2D;
                        break;
                    }
                    case TextureType::TwoDimensionalMultisample:
                    {
                        textureId = _texture->castRaw<Texture2DMSOpenGL>()->getGLTexture();
                        textureTarget = MAZE_GL_TEXTURE_2D_MULTISAMPLE;
                        break;
                    }
                    default:
                    {
                        MAZE_NOT_IMPLEMENTED;
                    }
                }
            }

            if (textureId)
            {
                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
                ContextOpenGLScopeBind contextScopeBind(m_context);
                RenderBufferOpenGLScopeBind renderBufferScopeBind(this);

                S32 activeTextureIndex = m_context->getActiveTexture();
                m_context->activeTexture(0);            

                MAZE_GL_CALL(
                    mzglFramebufferTexture2D(
                        MAZE_GL_FRAMEBUFFER,
                        MAZE_GL_DEPTH_ATTACHMENT,
                        textureTarget,
                        textureId, 0));

                m_context->activeTexture(activeTextureIndex);
            }
            
        }
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::setStencilTexture(TexturePtr const& _texture)
    {
        RenderBuffer::setStencilTexture(_texture);

        {
            MZGLint textureId = 0;
            MZGLenum textureTarget = MAZE_GL_TEXTURE_2D;

            if (_texture)
            {
                switch (_texture->getType())
                {
                    case TextureType::TwoDimensional:
                    {
                        textureId = _texture->castRaw<Texture2DOpenGL>()->getGLTexture();
                        textureTarget = MAZE_GL_TEXTURE_2D;
                        break;
                    }
                    case TextureType::TwoDimensionalMultisample:
                    {
                        textureId = _texture->castRaw<Texture2DMSOpenGL>()->getGLTexture();
                        textureTarget = MAZE_GL_TEXTURE_2D_MULTISAMPLE;
                        break;
                    }
                    default:
                    {
                        MAZE_NOT_IMPLEMENTED;
                    }
                }
            }

            if (textureId)
            {
                MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
                ContextOpenGLScopeBind contextScopeBind(m_context);
                RenderBufferOpenGLScopeBind renderBufferScopeBind(this);

                S32 activeTextureIndex = m_context->getActiveTexture();
                m_context->activeTexture(0);

                MAZE_GL_CALL(
                    mzglFramebufferTexture2D(
                        MAZE_GL_FRAMEBUFFER,
                        MAZE_GL_STENCIL_ATTACHMENT,
                        textureTarget,
                        textureId, 0));

                m_context->activeTexture(activeTextureIndex);
            }
            
        }
    }

    //////////////////////////////////////////
    bool RenderBufferOpenGL::check()
    {
        MAZE_GL_MUTEX_SCOPED_LOCK(getRenderSystemOpenGLRaw());
        ContextOpenGLScopeBind contextScopeBind(m_context);
        RenderBufferOpenGLScopeBind renderBufferScopeBind(this);
        
        MZGLenum status = mzglCheckFramebufferStatus(MAZE_GL_FRAMEBUFFER);
        switch (status)
        {
            case MAZE_GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            {
                MAZE_ERROR("MAZE_GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
                break;
            }
            case MAZE_GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            {
                MAZE_ERROR("MAZE_GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
                break;
            }
            case MAZE_GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            {
                MAZE_ERROR("MAZE_GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
                break;
            }
            case MAZE_GL_FRAMEBUFFER_UNSUPPORTED:
            {
                MAZE_ERROR("MAZE_GL_FRAMEBUFFER_UNSUPPORTED");
                break;
            }
        }

        return status == MAZE_GL_FRAMEBUFFER_COMPLETE;   
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::reloadTexture(TexturePtr const& _texture)
    {
        if (!_texture)
            return;

        switch (_texture->getType())
        {
            case TextureType::TwoDimensional:
            {
                Texture2D* texture2D = _texture->castRaw<Texture2D>();
                if (texture2D->isValid())
                    texture2D->reload();
                break;
            }
            case TextureType::TwoDimensionalMultisample:
            {
                Texture2DMS* texture2D = _texture->castRaw<Texture2DMS>();
                if (texture2D->isValid())
                    texture2D->reload();
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }
    }

    //////////////////////////////////////////
    void RenderBufferOpenGL::resizeTexture(TexturePtr const& _texture, Vec2DU const& _size)
    {
        if (!_texture)
            return;

        switch (_texture->getType())
        {
            case TextureType::TwoDimensional:
            {
                Texture2D* texture2D = _texture->castRaw<Texture2D>();
                texture2D->loadEmpty(_size.x, _size.y, texture2D->getInternalPixelFormat());
                break;
            }
            case TextureType::TwoDimensionalMultisample:
            {
                Texture2DMS* texture2D = _texture->castRaw<Texture2DMS>();
                texture2D->loadEmpty({ _size.x, _size.y }, texture2D->getInternalPixelFormat(), texture2D->getSamples());
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
