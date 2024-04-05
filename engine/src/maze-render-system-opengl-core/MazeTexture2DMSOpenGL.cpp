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
#include "maze-render-system-opengl-core/MazeTexture2DMSOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTextureOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Texture2DMSOpenGLScopeBind
    //
    //////////////////////////////////////////
    Texture2DMSOpenGLScopeBind::Texture2DMSOpenGLScopeBind(Texture2DMSOpenGL* _newTexture)
        : m_context(nullptr)
    {
        RenderSystemOpenGL* rs = _newTexture->getRenderSystemOpenGLRaw();
        ContextOpenGL* context = rs->ensureCurrentContext();
        if (context)
        {
            
            m_activeTextureIndex = context->getActiveTexture();
            context->activeTexture(0);
            m_prevTexture0 = context->getBindedTexture();

            context->bindTexture2DMS(_newTexture);

            m_context = context;
        }
    }

    //////////////////////////////////////////
    Texture2DMSOpenGLScopeBind::Texture2DMSOpenGLScopeBind(Texture2DMSOpenGLPtr const& _newTexture)
        : Texture2DMSOpenGLScopeBind(_newTexture.get())
    {
    }

    //////////////////////////////////////////
    Texture2DMSOpenGLScopeBind::~Texture2DMSOpenGLScopeBind()
    {
        if (m_context)
        {
            m_context->activeTexture(0);
            m_context->bindTexture(m_prevTexture0);
            m_context->activeTexture(m_activeTextureIndex);

        }
    }


    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DMSOpenGL, Texture2DMS);


    //////////////////////////////////////////
    // Class Texture2DMSOpenGL
    //
    //////////////////////////////////////////
    Texture2DMSOpenGL::Texture2DMSOpenGL()
        : m_context(nullptr)
        , m_glTexture(0)
    {
    }

    //////////////////////////////////////////
    Texture2DMSOpenGL::~Texture2DMSOpenGL()
    {
        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    Texture2DMSOpenGLPtr Texture2DMSOpenGL::Create(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        Texture2DMSOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Texture2DMSOpenGL, object, init(_renderSystem, _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DMSOpenGL::init(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        if (!Texture2DMS::init(_renderSystem))
            return false;

        setContextOpenGL(_contextOpenGL);

        generateGLObjects();

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DMSOpenGL::isValid()
    {
        return (m_glTexture != 0);
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::generateGLObjects()
    {
        if (m_glTexture)
        {
            deleteGLObjects();
        }

        ContextOpenGLScopeBind contextScopedBind(m_context);

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        MAZE_GL_CALL(mzglGenTextures(1, &m_glTexture));
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::deleteGLObjects()
    {
        if (!m_context || !m_context->isValid())
        {
            m_glTexture = 0;
            return;
        }

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        MAZE_GL_CALL(mzglDeleteTextures(1, &m_glTexture));
        m_glTexture = 0;
    }

    //////////////////////////////////////////
    bool Texture2DMSOpenGL::loadEmpty(
        Vec2U const& _size,
        PixelFormat::Enum _internalPixelFormat,
        S32 _samples)
    {
        m_samples = _samples;

        MZGLint internalFormat = GetOpenGLInternalFormat(_internalPixelFormat);
        if (0 == internalFormat)
        {
            m_internalPixelFormat = PixelFormat::None;
            return false;
        }

        m_internalPixelFormat = _internalPixelFormat;

        if (m_glTexture == 0)
        {
            generateGLObjects();
        }

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        Texture2DMSOpenGLScopeBind textureScopedBind(this);       

        m_size = _size;


#if (MAZE_DEBUG_GL)
        {
            MZGLboolean isTexture = MAZE_GL_FALSE;
            MAZE_GL_CALL(isTexture = mzglIsTexture(m_glTexture));
            if (isTexture == MAZE_GL_FALSE)
            {
                MAZE_ERROR("Texture %u is invalid texture Id!", (U32)m_glTexture);
                return false;
            }
        }
#endif        
        if (mzglTexImage2DMultisample)
        {
            MAZE_GL_CALL(mzglTexImage2DMultisample(MAZE_GL_TEXTURE_2D_MULTISAMPLE, (MZGLsizei)m_samples, internalFormat, (MZGLsizei)_size.x, (MZGLsizei)_size.y, MAZE_GL_TRUE));
        }
        else
        if (mzglTexStorage2DMultisample)
        {
            MAZE_GL_CALL(mzglTexStorage2DMultisample(MAZE_GL_TEXTURE_2D_MULTISAMPLE, (MZGLsizei)m_samples, internalFormat, (MZGLsizei)_size.x, (MZGLsizei)_size.y, MAZE_GL_TRUE));
        }
        else
        {
            MAZE_ERROR_RETURN_VALUE(false, "There is no way to create Texture2DMS!");
        }
    
#if (MAZE_DEBUG_GL)
        {
            MZGLboolean isTexture = MAZE_GL_FALSE;
            MAZE_GL_CALL(isTexture = mzglIsTexture(m_glTexture));
            if (isTexture == MAZE_GL_FALSE)
            {
                MAZE_ERROR("Texture %u is invalid texture Id!", (U32)m_glTexture);
                return false;
            }
        }
#endif

        return true;
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::saveToFileAsTGA(String const& _fileName, Vec2U _size)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    PixelSheet2D Texture2DMSOpenGL::readAsPixelSheet(PixelFormat::Enum _outputFormat)
    {   
        if (_outputFormat == PixelFormat::None)
            _outputFormat = m_internalPixelFormat;

        if (mzglGetTexImage)
        {
            PixelSheet2D result;

            result.setFormat(_outputFormat);
            result.setSize(m_size);

            MZGLint originFormat = GetOpenGLOriginFormat(_outputFormat);
            MZGLint dataType = GetOpenGLDataType(_outputFormat);

            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DMSOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglGetTexImage(MAZE_GL_TEXTURE_2D_MULTISAMPLE, 0, originFormat, dataType, result.getDataPointer()));

            return result;
        }

        if (mzglFramebufferTexture2D)
        {
            MAZE_WARNING_IF(_outputFormat != PixelFormat::RGBA_U8, "Unsupported format - %d", _outputFormat);

            MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_PACK_ALIGNMENT, 1));


            MZGLint currentFBO = 0;
            MZGLuint tempFBO = 0;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_FRAMEBUFFER_BINDING, &currentFBO));
            MAZE_GL_CALL(mzglGenFramebuffers(1, &tempFBO));
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_FRAMEBUFFER, tempFBO));

            PixelSheet2D result;
            result.setFormat(PixelFormat::RGBA_U8);
            result.setSize(m_size);

            MAZE_GL_CALL(mzglFramebufferTexture2D(MAZE_GL_FRAMEBUFFER, MAZE_GL_COLOR_ATTACHMENT0, MAZE_GL_TEXTURE_2D_MULTISAMPLE, m_glTexture, 0));
            MAZE_GL_CALL(mzglCheckFramebufferStatus(MAZE_GL_FRAMEBUFFER));
            MAZE_GL_CALL(
                mzglReadPixels(
                    0,
                    0,
                    result.getWidth(),
                    result.getHeight(),
                    MAZE_GL_RGBA,
                    MAZE_GL_UNSIGNED_BYTE,
                    result.getDataPointer()));

            MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_PACK_ALIGNMENT, 4));
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_FRAMEBUFFER, currentFBO));
            MAZE_GL_CALL(mzglDeleteFramebuffers(1, &tempFBO));

            return result;
        }

        /*
        if (mzglMapBufferRange)
        {
            PixelSheet2D result;

            PixelFormat::Enum pixelFormat = m_internalPixelFormat;
            result.setFormat(pixelFormat);
            result.setSize(m_size);

            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

            MZGLuint bufferId;
            MAZE_GL_CALL(mzglGenBuffers(1, &bufferId));
            MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_PIXEL_PACK_BUFFER, bufferId));

            MAZE_GL_CALL(
                mzglBufferData(
                    MAZE_GL_PIXEL_PACK_BUFFER,
                    result.getTotalBytesCount(),
                    NULL,
                    MAZE_GL_STREAM_DRAW));
            
            Texture2DMSOpenGLScopeBind textureScopedBind(this);
            
            void* buffer;
            Size offsetInBytes = 0;
            MAZE_GL_CALL(buffer = mzglMapBufferRange(MAZE_GL_PIXEL_PACK_BUFFER, offsetInBytes, result.getTotalBytesCount(), MAZE_GL_MAP_READ_BIT));

            MAZE_ERROR_RETURN_VALUE_IF(buffer == 0, result, "Out of memory!");

            memcpy(result.getDataPointer(), buffer, result.getTotalBytesCount());

            MZGLboolean mapped;
            MAZE_GL_CALL(mapped = mzglUnmapBuffer(MAZE_GL_PIXEL_PACK_BUFFER));
            MAZE_ERROR_RETURN_VALUE_IF(!mapped, result, "Buffer data corrupted, please reload");

            MAZE_GL_CALL(mzglBindBuffer(MAZE_GL_PIXEL_PACK_BUFFER, 0));
            MAZE_GL_CALL(mzglDeleteBuffers(1, &bufferId));
        }
        */


        MAZE_WARNING("Not supported!");

        return PixelSheet2D();
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::copyImageFrom(
        U8 const* _pixels,
        PixelFormat::Enum _pixelFormat,
        U32 _width,
        U32 _height,
        U32 _x,
        U32 _y)
    {
        MAZE_ERROR_RETURN_IF((S32)_x + (S32)_width > m_size.x, "Can't copy image!");
        MAZE_ERROR_RETURN_IF((S32)_y + (S32)_height > m_size.y, "Can't copy image!");

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        Texture2DMSOpenGLScopeBind textureScopedBind(this);

        MZGLint originFormat = GetOpenGLOriginFormat(_pixelFormat);
        MZGLint dataType = GetOpenGLDataType(_pixelFormat);

        MAZE_GL_CALL(mzglTexSubImage2D(MAZE_GL_TEXTURE_2D_MULTISAMPLE, 0, _x, _y, _width, _height, originFormat, dataType, _pixels));

        // Brute GPU synchronization
        MAZE_GL_CALL(mzglFinish());
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;

        m_glTexture = 0;
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_glTexture = 0;
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        Debug::log << "Texture2DMSOpenGL<" << getName() << ">: notifyContextOpenGLContextSetup started..." << endl;

        if (m_glTexture == 0)
            reload();

        Debug::log << "Texture2DMSOpenGL<" << getName() << ">: notifyContextOpenGLContextSetup finished." << endl;
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            deleteGLObjects();

            m_context->eventDestroyed.unsubscribe(this, &Texture2DMSOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this, &Texture2DMSOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.unsubscribe(this, &Texture2DMSOpenGL::notifyContextOpenGLContextSetup);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &Texture2DMSOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &Texture2DMSOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.subscribe(this, &Texture2DMSOpenGL::notifyContextOpenGLContextSetup);
        }
    }

    //////////////////////////////////////////
    void Texture2DMSOpenGL::reload()
    {
        if (m_glTexture == 0)
            generateGLObjects();

        Debug::log << "Texture2DMSOpenGL<" << getName() << ">: reloading empty..." << endl;
        loadEmpty(m_size, m_internalPixelFormat, m_samples);
        Debug::log << "Texture2DMSOpenGL<" << getName() << ">: reloaded with id=" << m_glTexture << "." << endl;
    }
    
} // namespace Maze
//////////////////////////////////////////
