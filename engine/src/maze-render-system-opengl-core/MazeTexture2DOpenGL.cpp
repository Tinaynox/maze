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
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTextureOpenGL.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/system/MazeTimer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    U8 ConvertPixelChannelDataToU8(const U8* _channelPointer, PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8:
            case PixelFormat::RG_S8:
            case PixelFormat::RGB_S8:
            case PixelFormat::RGBA_S8:
            case PixelFormat::R_U8:
            case PixelFormat::RG_U8:
            case PixelFormat::RGB_U8:
            case PixelFormat::RGBA_U8:
            {
                U8 sourcePixel = *_channelPointer;
                return sourcePixel;
            }

            case PixelFormat::R_S16:
            case PixelFormat::RG_S16:
            case PixelFormat::RGBA_S16:
            case PixelFormat::R_U16:
            case PixelFormat::RG_U16:
            case PixelFormat::RGBA_U16:
            {
                U16 sourcePixel = *(U16*)_channelPointer;
                return static_cast<U8>(sourcePixel);
            }

            case PixelFormat::R_S32:
            case PixelFormat::RG_S32:
            case PixelFormat::R_U32:
            case PixelFormat::RG_U32:
            {
                U32 sourcePixel = *(U32*)_channelPointer;
                return static_cast<U8>(sourcePixel);
            }

            case PixelFormat::R_F16:
            case PixelFormat::RG_F16:
            case PixelFormat::RGBA_F16:
            {
                F32 sourcePixel = *(F32*)_channelPointer;
                return static_cast<U8>(sourcePixel);
            }

            case PixelFormat::R_F32:
            case PixelFormat::RG_F32:
            case PixelFormat::RGB_F32:
            case PixelFormat::RGBA_F32:
            {
                F32 sourcePixel = *(F32*)_channelPointer;
                return static_cast<U8>(sourcePixel);
            }
            default:
            {
                MAZE_ERROR("Unsupported PixelFormat!");
            }
        }

        return 0;
    }

    //////////////////////////////////////////
    // Class Texture2DOpenGLScopeBind
    //
    //////////////////////////////////////////
    Texture2DOpenGLScopeBind::Texture2DOpenGLScopeBind(Texture2DOpenGL* _newTexture)
        : m_context(nullptr)
    {
        RenderSystemOpenGL* rs = _newTexture->getRenderSystemOpenGLRaw();
        ContextOpenGL* context = rs->ensureCurrentContext();
        if (context)
        {
            
            m_activeTextureIndex = context->getActiveTexture();
            context->activeTexture(0);
            m_prevTexture0 = context->getBindedTexture();

            context->bindTexture2D(_newTexture);

            m_context = context;
        }
    }

    //////////////////////////////////////////
    Texture2DOpenGLScopeBind::Texture2DOpenGLScopeBind(Texture2DOpenGLPtr const& _newTexture)
        : Texture2DOpenGLScopeBind(_newTexture.get())
    {
    }

    //////////////////////////////////////////
    Texture2DOpenGLScopeBind::~Texture2DOpenGLScopeBind()
    {
        if (m_context)
        {
            m_context->activeTexture(0);
            m_context->bindTexture(m_prevTexture0);
            m_context->activeTexture(m_activeTextureIndex);

        }
    }


    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DOpenGL, Texture2D);


    //////////////////////////////////////////
    // Class Texture2DOpenGL
    //
    //////////////////////////////////////////
    Texture2DOpenGL::Texture2DOpenGL()
        : m_context(nullptr)
        , m_glTexture(0)
        , m_hasPresetMipmaps(false)
    {
    }

    //////////////////////////////////////////
    Texture2DOpenGL::~Texture2DOpenGL()
    {
        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    Texture2DOpenGLPtr Texture2DOpenGL::Create(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        Texture2DOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Texture2DOpenGL, object, init(_renderSystem, _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DOpenGL::init(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        if (!Texture2D::init(_renderSystem))
            return false;

        setContextOpenGL(_contextOpenGL);

        generateGLObjects();

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DOpenGL::isValid()
    {
        return (m_glTexture != 0);
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::generateGLObjects()
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
    void Texture2DOpenGL::deleteGLObjects()
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
    bool Texture2DOpenGL::loadTexture(
        Vector<PixelSheet2D> const& _pixelSheets,
        PixelFormat::Enum _internalPixelFormat)
    {
        MAZE_PROFILE_EVENT("Texture2DOpenGL::loadTexture");

        if (_pixelSheets.empty())
            return false;

        PixelSheet2D const& pixelSheet0 = _pixelSheets[0];

        if (_internalPixelFormat == PixelFormat::None)
            _internalPixelFormat = pixelSheet0.getFormat();

        MZGLint internalFormat = GetOpenGLInternalFormat(_internalPixelFormat);
        if (0 == internalFormat)
        {
            m_internalPixelFormat = PixelFormat::None;
            return false;
        }

        if (!m_context || !m_context->isValid())
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
        Texture2DOpenGLScopeBind textureScopedBind(this);

        

        m_size = pixelSheet0.getSize();
        m_hasPresetMipmaps = (_pixelSheets.size() > 1);


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

        Vec2U size = m_size;
        for (Size mipmapLevel = 0, in = _pixelSheets.size(); mipmapLevel < in; ++mipmapLevel)
        {
            PixelSheet2D const& pixelSheet = _pixelSheets[mipmapLevel];
            PixelFormat::Enum mipmapPixelFormat = pixelSheet.getFormat();

            MZGLint originFormat = GetOpenGLOriginFormat(mipmapPixelFormat);
            MZGLint dataType = GetOpenGLDataType(mipmapPixelFormat);

            MZGLsizei dataSize = (MZGLsizei)pixelSheet.getTotalBytesCount();
            U8 const* data = pixelSheet.getDataRW();

            if (PixelFormat::IsCompressed(mipmapPixelFormat))
            {
                MAZE_GL_CALL(
                    mzglCompressedTexImage2D(
                        MAZE_GL_TEXTURE_2D,
                        (MZGLint)mipmapLevel,
                        internalFormat,
                        size.x,
                        size.y,
                        0,
                        dataSize,
                        data));

                if (mzglGetTexLevelParameteriv)
                {
                    MZGLint param = 0;
                    MAZE_GL_CALL(mzglGetTexLevelParameteriv(MAZE_GL_TEXTURE_2D, (MZGLint)mipmapLevel, MAZE_GL_TEXTURE_COMPRESSED_ARB, &param));
                    MAZE_ERROR_IF(param == 0, "Texture2DOpenGL<%s>: Texture compressed format loading failed! mipmapLevel=%d (%ux%u) mipmapPixelFormat=%s, _internalPixelFormat=%s",
                        getName().c_str(),
                        (S32)mipmapLevel,
                        size.x,
                        size.y,
                        PixelFormat::ToString(mipmapPixelFormat).c_str(),
                        PixelFormat::ToString(_internalPixelFormat).c_str());
                }
            }
            else
            {
                if (originFormat == MAZE_GL_DEPTH_COMPONENT ||
                    originFormat == MAZE_GL_DEPTH_STENCIL)
                {
                    MAZE_GL_CALL(mzglTexImage2D(MAZE_GL_TEXTURE_2D, (MZGLint)mipmapLevel, internalFormat, size.x, size.y, 0, originFormat, dataType, 0));
                }
                else
                {
                    // #TODO:
                    MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_UNPACK_ALIGNMENT, 1));
                    MAZE_GL_CALL(mzglTexImage2D(MAZE_GL_TEXTURE_2D, (MZGLint)mipmapLevel, internalFormat, size.x, size.y, 0, originFormat, dataType, data));
                }
            }

            size /= 2;
            size.x = Math::Max(size.x, 1u);
            size.y = Math::Max(size.y, 1u);
        }

        MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_UNPACK_ALIGNMENT, 4));
    
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

        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_MAG_FILTER, GetOpenGLFilter(m_magFilter)));
        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_MIN_FILTER, GetOpenGLFilter(m_minFilter)));
        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_WRAP_S, GetOpenGLWrap(m_wrapS)));
        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_WRAP_T, GetOpenGLWrap(m_wrapT)));
        Vec4F borderColorVec4F = m_borderColor.toVec4F32();
        MAZE_GL_CALL(mzglTexParameterfv(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_BORDER_COLOR, (F32 const*)&borderColorVec4F));
    
        generateMipmaps();

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DOpenGL::setMagFilter(TextureFilter _value)
    {
        if (m_magFilter == _value)
            return true;

        m_magFilter = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_MAG_FILTER, GetOpenGLFilter(m_magFilter)));
        }

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DOpenGL::setMinFilter(TextureFilter _value)
    {
        if (m_minFilter == _value)
            return true;

        m_minFilter = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_MIN_FILTER, GetOpenGLFilter(m_minFilter)));
        }

        generateMipmaps();

        return true;
    }
    //////////////////////////////////////////
    bool Texture2DOpenGL::setWrapS(TextureWrap _value)
    {
        if (m_wrapS == _value)
            return true;

        m_wrapS = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_WRAP_S, GetOpenGLWrap(m_wrapS)));
        }


        return true;
    }

    //////////////////////////////////////////
    bool Texture2DOpenGL::setWrapT(TextureWrap _value)
    {
        if (m_wrapT == _value)
            return true;

        m_wrapT = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_WRAP_T, GetOpenGLWrap(m_wrapT)));
        }

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DOpenGL::setBorderColor(ColorU32 _value)
    {
        if (m_borderColor == _value)
            return true;

        m_borderColor = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(this);

            Vec4F borderColorVec4F = m_borderColor.toVec4F32();
            MAZE_GL_CALL(mzglTexParameterfv(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_BORDER_COLOR, (F32 const*)&borderColorVec4F));
        }

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DOpenGL::setAnisotropyLevel(F32 _value)
    {
        if (m_anisotropyLevel == _value)
            return true;

        m_anisotropyLevel = Math::Clamp(_value, 1.0f, m_renderSystem->getTextureMaxAnisotropyLevel());

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameterf(MAZE_GL_TEXTURE_2D, MAZE_GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisotropyLevel));
        }

        return true;
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::saveToFileAsTGA(
        String const& _fileName,
        Vec2U _size,
        bool _resetAlpha)
    {
        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
        Texture2DOpenGLScopeBind textureScopedBind(this);

        if (Vec2U::c_zero == _size)
        {
            _size = m_size;
        }

#if (MAZE_DEBUG_GL)
        {
            MZGLint value;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_ACTIVE_TEXTURE, &value));
            MAZE_DEBUG_BP_IF(value != MAZE_GL_TEXTURE0);
        }
        {
            MZGLint value;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_TEXTURE_BINDING_2D, &value));
            MAZE_DEBUG_BP_IF(value != m_glTexture);
        }
#endif

        MZGLint w, h;
        MZGLint miplevel = 0;
        MAZE_GL_CALL(mzglGetTexLevelParameteriv(MAZE_GL_TEXTURE_2D, miplevel, MAZE_GL_TEXTURE_WIDTH, &w));
        MAZE_GL_CALL(mzglGetTexLevelParameteriv(MAZE_GL_TEXTURE_2D, miplevel, MAZE_GL_TEXTURE_HEIGHT, &h));
        MAZE_DEBUG_BP_IF(w != m_size.x);
        MAZE_DEBUG_BP_IF(h != m_size.y);


        PixelFormat::Enum pixelFormat = PixelFormat::RGBA_U8;

        U32 bytesPerPixel = PixelFormat::GetBytesPerPixel(pixelFormat);
        U32 channelsPerPixel = PixelFormat::GetChannelsPerPixel(pixelFormat);
        U32 bytesPerChannel = bytesPerPixel / channelsPerPixel;
        MZGLint originFormat = GetOpenGLOriginFormat(pixelFormat);


        MZGLint dataType = GetOpenGLDataType(pixelFormat);

        if (!mzglGetTexImage)
            return;

        ByteBuffer data(_size.x * _size.y * bytesPerPixel);
        MAZE_GL_CALL(mzglGetTexImage(MAZE_GL_TEXTURE_2D, 0, originFormat, dataType, data.getDataRW()));        
        U32 bytesPerRow = bytesPerPixel * _size.x;

        ByteBuffer tgaData(_size.x * _size.y * 4);

        U8* tgaDataPointer = tgaData.getDataRW();

        for (Size r = 0; r < _size.y; ++r)
        {
            for (Size c = 0; c < _size.x; ++c)
            {
                U32 offset = bytesPerRow * (U32)r + bytesPerPixel * (U32)c;
                const U8* pixel = data.getDataRW() + offset;

                U8 r = 0;
                U8 g = 0;
                U8 b = 0;
                U8 a = 0;

                if (channelsPerPixel > 0)
                    r = ConvertPixelChannelDataToU8(pixel + 0 * bytesPerChannel, pixelFormat);

                if (channelsPerPixel > 1)
                    g = ConvertPixelChannelDataToU8(pixel + 1 * bytesPerChannel, pixelFormat);

                if (channelsPerPixel > 2)
                    b = ConvertPixelChannelDataToU8(pixel + 2 * bytesPerChannel, pixelFormat);

                if (!_resetAlpha)
                {
                    if (channelsPerPixel > 3)
                        a = ConvertPixelChannelDataToU8(pixel + 3 * bytesPerChannel, pixelFormat);
                }
                else
                {
                    a = 255;
                }

                // BGRA
                *(tgaDataPointer++) = b;
                *(tgaDataPointer++) = g;
                *(tgaDataPointer++) = r;
                *(tgaDataPointer++) = a;
            }
        }

        U32 xa = _size.x % 256;
        U32 xb = (_size.x - xa) / 256;
        U32 ya = _size.y % 256;
        U32 yb = (_size.y - ya) / 256;

        U8 header[18] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, (U8)xa, (U8)xb, (U8)ya, (U8)yb, 32, 0 };

        std::fstream file(_fileName.c_str(), std::ios::out | std::ios::binary);
        file.write(reinterpret_cast<CString>(header), sizeof(S8) * 18);
        file.write(reinterpret_cast<CString>(tgaData.getDataRW()), sizeof(S8) * tgaData.getSize());
        file.close();
    }

    //////////////////////////////////////////
    PixelSheet2D Texture2DOpenGL::readAsPixelSheet(
        PixelFormat::Enum _outputFormat)
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
            Texture2DOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglGetTexImage(MAZE_GL_TEXTURE_2D, 0, originFormat, dataType, result.getDataRW()));

            return result;
        }

        if (mzglFramebufferTexture2D)
        {
            MAZE_WARNING_IF(_outputFormat != PixelFormat::RGBA_U8, "%s: Unsupported format - %s", getName().c_str(), PixelFormat::ToString(_outputFormat).c_str());

            MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_PACK_ALIGNMENT, 1));

            MZGLint currentFBO = 0;
            MZGLuint tempFBO = 0;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_FRAMEBUFFER_BINDING, &currentFBO));
            MAZE_GL_CALL(mzglGenFramebuffers(1, &tempFBO));
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_FRAMEBUFFER, tempFBO));

            PixelSheet2D result;
            result.setFormat(PixelFormat::RGBA_U8);
            result.setSize(m_size);

            MAZE_GL_CALL(mzglFramebufferTexture2D(MAZE_GL_FRAMEBUFFER, MAZE_GL_COLOR_ATTACHMENT0, MAZE_GL_TEXTURE_2D, m_glTexture, 0));
            MAZE_GL_CALL(mzglCheckFramebufferStatus(MAZE_GL_FRAMEBUFFER));
            MAZE_GL_CALL(
                mzglReadPixels(
                    0,
                    0,
                    result.getWidth(),
                    result.getHeight(),
                    MAZE_GL_RGBA,
                    MAZE_GL_UNSIGNED_BYTE,
                    result.getDataRW()));

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
            
            Texture2DOpenGLScopeBind textureScopedBind(this);
            
            void* buffer;
            Size offsetInBytes = 0;
            MAZE_GL_CALL(buffer = mzglMapBufferRange(MAZE_GL_PIXEL_PACK_BUFFER, offsetInBytes, result.getTotalBytesCount(), MAZE_GL_MAP_READ_BIT));

            MAZE_ERROR_RETURN_VALUE_IF(buffer == 0, result, "Out of memory!");

            memcpy(result.getDataRW(), buffer, result.getTotalBytesCount());

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
    void Texture2DOpenGL::copyImageFrom(
        Texture2DPtr const& _texture,
        U32 _x,
        U32 _y)
    {
        if (!_texture)
            return;

        MAZE_ERROR_RETURN_IF((S32)_x + _texture->getWidth() > m_size.x, "Can't copy image from texture!");
        MAZE_ERROR_RETURN_IF((S32)_y + _texture->getHeight() > m_size.y, "Can't copy image from texture!");

        Texture2DOpenGL* textureGL = _texture->castRaw<Texture2DOpenGL>();

        if (m_glTexture == 0)
            return;

        ContextOpenGLScopeBind contextScopedBind(m_context);
        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        if (m_context->getExtensionsRaw()->getSupportFrameBufferObject()
            && m_context->getExtensionsRaw()->getSupportFrameBufferBlit())
        {
            Texture2DOpenGLScopeBind textureScopedBind(this);

            // Save the current bindings so we can restore them after we are done
            MZGLint readFramebuffer = 0;
            MZGLint drawFramebuffer = 0;

            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer));
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer));

            // Create the framebuffers
            MZGLuint sourceFrameBuffer = 0;
            MZGLuint destFrameBuffer = 0;
            MAZE_GL_CALL(mzglGenFramebuffers(1, &sourceFrameBuffer));
            MAZE_GL_CALL(mzglGenFramebuffers(1, &destFrameBuffer));

            MAZE_ERROR_RETURN_IF(!sourceFrameBuffer || !destFrameBuffer, "Cannot copy texture, failed to create a frame buffer object");

            // Link the source texture to the source frame buffer
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_READ_FRAMEBUFFER, sourceFrameBuffer));
            MAZE_GL_CALL(mzglFramebufferTexture2D(MAZE_GL_READ_FRAMEBUFFER, MAZE_GL_COLOR_ATTACHMENT0, MAZE_GL_TEXTURE_2D, textureGL->m_glTexture, 0));

            // Link the destination texture to the destination frame buffer
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_DRAW_FRAMEBUFFER, destFrameBuffer));
            MAZE_GL_CALL(mzglFramebufferTexture2D(MAZE_GL_DRAW_FRAMEBUFFER, MAZE_GL_COLOR_ATTACHMENT0, MAZE_GL_TEXTURE_2D, m_glTexture, 0));

            // A final check, just to be sure...
            MZGLenum sourceStatus;
            MAZE_GL_CALL(sourceStatus = mzglCheckFramebufferStatus(MAZE_GL_READ_FRAMEBUFFER));

            MZGLenum destStatus;
            MAZE_GL_CALL(destStatus = mzglCheckFramebufferStatus(MAZE_GL_DRAW_FRAMEBUFFER));

            if ((sourceStatus == MAZE_GL_FRAMEBUFFER_COMPLETE) && (destStatus == MAZE_GL_FRAMEBUFFER_COMPLETE))
            {
                // Blit the texture contents from the source to the destination texture
                MAZE_GL_CALL(mzglBlitFramebuffer(0, 0, _texture->getWidth(), _texture->getHeight(), _x, _y, _x + _texture->getWidth(), _y + _texture->getHeight(), MAZE_GL_COLOR_BUFFER_BIT, MAZE_GL_NEAREST));
            }
            else
            {
                MAZE_ERROR("Cannot copy texture, failed to link texture to frame buffer");
            }

            // Restore previously bound framebuffers
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_READ_FRAMEBUFFER, readFramebuffer));
            MAZE_GL_CALL(mzglBindFramebuffer(MAZE_GL_DRAW_FRAMEBUFFER, drawFramebuffer));

            // Delete the framebuffers
            MAZE_GL_CALL(mzglDeleteFramebuffers(1, &sourceFrameBuffer));
            MAZE_GL_CALL(mzglDeleteFramebuffers(1, &destFrameBuffer));

            generateMipmaps();

            return;
        }

        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::copyImageFrom(
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
        Texture2DOpenGLScopeBind textureScopedBind(this);

        MZGLint originFormat = GetOpenGLOriginFormat(_pixelFormat);
        MZGLint dataType = GetOpenGLDataType(_pixelFormat);

        MAZE_GL_CALL(mzglTexSubImage2D(MAZE_GL_TEXTURE_2D, 0, _x, _y, _width, _height, originFormat, dataType, _pixels));

        // Brute GPU synchronization
        MAZE_GL_CALL(mzglFinish());

        generateMipmaps();
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;

        m_glTexture = 0;
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        Texture2DLibraryData const* libraryData = m_renderSystem->getTextureManager()->getTexture2DLibraryData(getName().asHashedCString());
        if (!libraryData || !libraryData->callbacks.requestReload)
        {
            if (m_internalPixelFormat == PixelFormat::RGBA_U8)
                m_cachedPixelSheet = readAsPixelSheet();
        }

        m_glTexture = 0;
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        Debug::log << "Texture2DOpenGL<" << getName() << ">: notifyContextOpenGLContextSetup started..." << endl;

        if (m_glTexture == 0)
            reload();

        Debug::log << "Texture2DOpenGL<" << getName() << ">: notifyContextOpenGLContextSetup finished." << endl;
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            deleteGLObjects();

            m_context->eventDestroyed.unsubscribe(this, &Texture2DOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this, &Texture2DOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.unsubscribe(this, &Texture2DOpenGL::notifyContextOpenGLContextSetup);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &Texture2DOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &Texture2DOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.subscribe(this, &Texture2DOpenGL::notifyContextOpenGLContextSetup);
        }
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::generateMipmaps()
    {
        if (m_glTexture == 0)
            return;

        //if (m_hasPresetMipmaps)
        //    return;

        if (   m_minFilter == TextureFilter::LinearMipmapLinear
            || m_minFilter == TextureFilter::LinearMipmapNearest
            || m_minFilter == TextureFilter::NearestMipmapLinear
            || m_minFilter == TextureFilter::NearestMipmapNearest)
        {            
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            Texture2DOpenGLScopeBind textureScopedBind(this);           

            if (mzglGenerateMipmap)
            {
                MAZE_PROFILE_EVENT("mzglGenerateMipmap");

                Debug::log << "Texture2DOpenGL<" << getName() << ">: mzglGenerateMipmap started..." << endl;
                Timer timer;

                MAZE_GL_CALL(mzglGenerateMipmap(MAZE_GL_TEXTURE_2D));

                F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
                Debug::log << "Texture2DOpenGL<" << getName() << ">: mzglGenerateMipmap finished (" <<
                    std::fixed << std::setprecision(1) << msTime << "ms)" << endl;
            }
            else
            {
                MAZE_WARNING("Autogenerated mipmaps is not available!");
            }            
        }
    }

    //////////////////////////////////////////
    void Texture2DOpenGL::reload()
    {
        if (m_glTexture == 0)
            generateGLObjects();

        Texture2DLibraryData const* libraryData = m_renderSystem->getTextureManager()->getTexture2DLibraryData(getName().asHashedCString());
        if (libraryData && libraryData->callbacks.requestReload)
        {
            Debug::log << "Texture2DOpenGL<" << getName() << ">: reloading from asset file..." << endl;
            libraryData->callbacks.requestReload(true);
            Debug::log << "Texture2DOpenGL<" << getName() << ">: reloaded with id=" << m_glTexture << "." << endl;
        }
        else
        if (m_cachedPixelSheet.getFormat() != PixelFormat::None)
        {
            Debug::log << "Texture2DOpenGL<" << getName() << ">: reloading from pixel sheet... (cachedPixelSheetFormat=" << PixelFormat::ToString(m_cachedPixelSheet.getFormat()) << ", internalPixelFormat=" << PixelFormat::ToString(m_internalPixelFormat) << ")" << endl;
            loadTexture(m_cachedPixelSheet, m_internalPixelFormat);
            Debug::log << "Texture2DOpenGL<" << getName() << ">: reloaded with id=" << m_glTexture << "." << endl;

            m_cachedPixelSheet.clear();
        }
        else
        {
            Debug::log << "Texture2DOpenGL<" << getName() << ">: reloading empty..." << endl;
            loadEmpty(m_size, m_internalPixelFormat);
            Debug::log << "Texture2DOpenGL<" << getName() << ">: reloaded with id=" << m_glTexture << "." << endl;
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
