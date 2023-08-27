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
#include "maze-render-system-opengl-core/MazeTextureCubeOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTextureOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TextureCubeOpenGLScopeBind
    //
    //////////////////////////////////////////
    TextureCubeOpenGLScopeBind::TextureCubeOpenGLScopeBind(TextureCubeOpenGL* _newTexture)
        : m_context(nullptr)
    {
        RenderSystemOpenGL* rs = _newTexture->getRenderSystemOpenGLRaw();
        ContextOpenGL* context = rs->ensureCurrentContext();
        if (context)
        {
            
            m_activeTextureIndex = context->getActiveTexture();
            context->activeTexture(0);
            m_prevTexture0 = context->getBindedTexture();

            context->bindTextureCube(_newTexture);

            m_context = context;
        }
    }

    //////////////////////////////////////////
    TextureCubeOpenGLScopeBind::TextureCubeOpenGLScopeBind(TextureCubeOpenGLPtr const& _newTexture)
        : TextureCubeOpenGLScopeBind(_newTexture.get())
    {
    }

    //////////////////////////////////////////
    TextureCubeOpenGLScopeBind::~TextureCubeOpenGLScopeBind()
    {
        if (m_context)
        {
            m_context->activeTexture(0);
            m_context->bindTexture(m_prevTexture0);
            m_context->activeTexture(m_activeTextureIndex);

        }
    }


    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TextureCubeOpenGL, TextureCube);


    //////////////////////////////////////////
    // Class TextureCubeOpenGL
    //
    //////////////////////////////////////////
    TextureCubeOpenGL::TextureCubeOpenGL()
        : m_context(nullptr)
        , m_glTexture(0)
        , m_hasPresetMipmaps(false)
    {
    }

    //////////////////////////////////////////
    TextureCubeOpenGL::~TextureCubeOpenGL()
    {
        setContextOpenGL(nullptr);
    }

    //////////////////////////////////////////
    TextureCubeOpenGLPtr TextureCubeOpenGL::Create(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        TextureCubeOpenGLPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TextureCubeOpenGL, object, init(_renderSystem, _contextOpenGL));
        return object;
    }

    //////////////////////////////////////////
    bool TextureCubeOpenGL::init(
        RenderSystem* _renderSystem,
        ContextOpenGL* _contextOpenGL)
    {
        if (!TextureCube::init(_renderSystem))
            return false;

        setContextOpenGL(_contextOpenGL);

        generateGLObjects();

        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeOpenGL::isValid()
    {
        return (m_glTexture != 0);
    }

    //////////////////////////////////////////
    void TextureCubeOpenGL::generateGLObjects()
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
    void TextureCubeOpenGL::deleteGLObjects()
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
    bool TextureCubeOpenGL::loadTexture(
        Vector<PixelSheet2D> const _pixelSheets[6],
        PixelFormat::Enum _internalPixelFormat)
    {
        MAZE_PROFILE_EVENT("TextureCubeOpenGL::loadTexture");

        if (    _pixelSheets[0].empty()
            &&  _pixelSheets[1].empty()
            &&  _pixelSheets[2].empty()
            &&  _pixelSheets[3].empty()
            &&  _pixelSheets[4].empty()
            &&  _pixelSheets[5].empty())
            return false;

        for (S32 i = 0; i < 6; ++i)
            m_pixelSheetsTEMP[i] = _pixelSheets[i];

        PixelSheet2D const& pixelSheet0 = _pixelSheets[0][0];

        if (_internalPixelFormat == PixelFormat::None)
            _internalPixelFormat = pixelSheet0.getFormat();

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
        TextureCubeOpenGLScopeBind textureScopedBind(this);
                
        m_size = pixelSheet0.getSize();
        m_hasPresetMipmaps = (_pixelSheets[0].size() > 1);


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

        for (Size side = 0; side < 6; ++side)
        {
            Vec2DU size = m_size;
            for (Size mipmapLevel = 0, in = _pixelSheets[side].size(); mipmapLevel < in; ++mipmapLevel)
            {
                PixelSheet2D pixelSheet = _pixelSheets[side][mipmapLevel];
                pixelSheet.flipY();
                PixelFormat::Enum mipmapPixelFormat = pixelSheet.getFormat();

                MZGLint originFormat = GetOpenGLOriginFormat(mipmapPixelFormat);
                MZGLint dataType = GetOpenGLDataType(mipmapPixelFormat);

                MZGLsizei dataSize = (MZGLsizei)pixelSheet.getTotalBytesCount();
                U8 const* data = pixelSheet.getDataPointer();

                MZGLenum const targets[6] =
                    {
                        MAZE_GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                        MAZE_GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                        MAZE_GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                        MAZE_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                        MAZE_GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                        MAZE_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                    };
                MZGLenum target = targets[side];

                if (PixelFormat::IsCompressed(mipmapPixelFormat))
                {
                    MAZE_GL_CALL(
                        mzglCompressedTexImage2D(
                            target,
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
                        MAZE_GL_CALL(mzglGetTexLevelParameteriv(target, (MZGLint)mipmapLevel, MAZE_GL_TEXTURE_COMPRESSED_ARB, &param));
                        MAZE_ERROR_IF(param == 0, "Texture compression failed");
                    }
                }
                else
                {
                    if (   originFormat == MAZE_GL_DEPTH_COMPONENT
                        || originFormat == MAZE_GL_DEPTH_STENCIL)
                    {
                        MAZE_GL_CALL(mzglTexImage2D(target, (MZGLint)mipmapLevel, internalFormat, size.x, size.y, 0, originFormat, dataType, 0));
                    }
                    else
                    {
                        // #TODO:
                        MAZE_GL_CALL(mzglPixelStorei(MAZE_GL_UNPACK_ALIGNMENT, 1));
                        MAZE_GL_CALL(mzglTexImage2D(target, (MZGLint)mipmapLevel, internalFormat, size.x, size.y, 0, originFormat, dataType, data));
                    }
                }

                size /= 2;
                size.x = Math::Max(size.x, 1u);
                size.y = Math::Max(size.y, 1u);
            }
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

        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_MAG_FILTER, GetOpenGLFilter(m_magFilter)));
        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_MIN_FILTER, GetOpenGLFilter(m_minFilter)));
        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_WRAP_S, GetOpenGLWrap(m_wrapS)));
        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_WRAP_T, GetOpenGLWrap(m_wrapT)));
        MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_WRAP_R, GetOpenGLWrap(m_wrapR)));
    
        generateMipmaps();

        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeOpenGL::setMagFilter(TextureFilter _value)
    {
        if (m_magFilter == _value)
            return true;

        m_magFilter = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            TextureCubeOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_MAG_FILTER, GetOpenGLFilter(m_magFilter)));
        }

        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeOpenGL::setMinFilter(TextureFilter _value)
    {
        if (m_minFilter == _value)
            return true;

        m_minFilter = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            TextureCubeOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_MIN_FILTER, GetOpenGLFilter(m_minFilter)));
        }

        generateMipmaps();

        return true;
    }
    //////////////////////////////////////////
    bool TextureCubeOpenGL::setWrapS(TextureWrap _value)
    {
        if (m_wrapS == _value)
            return true;

        m_wrapS = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            TextureCubeOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_WRAP_S, GetOpenGLWrap(m_wrapS)));
        }


        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeOpenGL::setWrapT(TextureWrap _value)
    {
        if (m_wrapT == _value)
            return true;

        m_wrapT = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            TextureCubeOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_WRAP_T, GetOpenGLWrap(m_wrapT)));
        }

        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeOpenGL::setWrapR(TextureWrap _value)
    {
        if (m_wrapR == _value)
            return true;

        m_wrapR = _value;

        {
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            TextureCubeOpenGLScopeBind textureScopedBind(this);

            MAZE_GL_CALL(mzglTexParameteri(MAZE_GL_TEXTURE_CUBE_MAP, MAZE_GL_TEXTURE_WRAP_R, GetOpenGLWrap(m_wrapR)));
        }

        return true;
    }

    //////////////////////////////////////////
    void TextureCubeOpenGL::notifyContextOpenGLDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_context = nullptr;

        m_glTexture = 0;
    }

    //////////////////////////////////////////
    void TextureCubeOpenGL::notifyContextOpenGLContextWillBeDestroyed(ContextOpenGL* _contextOpenGL)
    {
        m_glTexture = 0;
    }

    //////////////////////////////////////////
    void TextureCubeOpenGL::notifyContextOpenGLContextSetup(ContextOpenGL* _contextOpenGL)
    {
        if (m_glTexture == 0)
        {
            generateGLObjects();

            reload();
        }
    }

    //////////////////////////////////////////
    void TextureCubeOpenGL::setContextOpenGL(ContextOpenGL* _contextOpenGL)
    {
        if (m_context == _contextOpenGL)
            return;

        if (m_context)
        {
            deleteGLObjects();

            m_context->eventDestroyed.unsubscribe(this, &TextureCubeOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.unsubscribe(this, &TextureCubeOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.unsubscribe(this, &TextureCubeOpenGL::notifyContextOpenGLContextSetup);
        }

        m_context = _contextOpenGL;

        if (m_context)
        {
            m_context->eventDestroyed.subscribe(this, &TextureCubeOpenGL::notifyContextOpenGLDestroyed);
            m_context->eventGLContextWillBeDestroyed.subscribe(this, &TextureCubeOpenGL::notifyContextOpenGLContextWillBeDestroyed);
            m_context->eventGLContextSetup.subscribe(this, &TextureCubeOpenGL::notifyContextOpenGLContextSetup);
        }
    }

    //////////////////////////////////////////
    void TextureCubeOpenGL::generateMipmaps()
    {
        if (m_glTexture == 0)
            return;

        if (   m_minFilter == TextureFilter::LinearMipmapLinear
            || m_minFilter == TextureFilter::LinearMipmapNearest
            || m_minFilter == TextureFilter::NearestMipmapLinear
            || m_minFilter == TextureFilter::NearestMipmapNearest)
        {            
            ContextOpenGLScopeBind contextScopedBind(m_context);
            MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());
            TextureCubeOpenGLScopeBind textureScopedBind(this);           

            if (mzglGenerateMipmap)
            {
                Debug::log << getName() << ": mzglGenerateMipmap started..." << endl;
                MAZE_GL_CALL(mzglGenerateMipmap(MAZE_GL_TEXTURE_CUBE_MAP));
                Debug::log << getName() << ": mzglGenerateMipmap finished." << endl;
            }
            else
            {
                MAZE_WARNING("Autogenerated mipmaps is not available!");
            }            
        }
    }

    //////////////////////////////////////////
    void TextureCubeOpenGL::reload()
    {
        if (m_assetFile)
        {
            loadFromAssetFile(m_assetFile);
        }
        else
        if (    !m_pixelSheetsTEMP[0].empty()
            &&  !m_pixelSheetsTEMP[1].empty()
            &&  !m_pixelSheetsTEMP[2].empty()
            &&  !m_pixelSheetsTEMP[3].empty()
            &&  !m_pixelSheetsTEMP[4].empty()
            &&  !m_pixelSheetsTEMP[5].empty())
        {
            loadTexture(m_pixelSheetsTEMP, m_internalPixelFormat);
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
