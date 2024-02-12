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
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeFunctionsOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ExtensionsOpenGL
    //
    //////////////////////////////////////////
    ExtensionsOpenGL::ExtensionsOpenGL()
    {

    }

    //////////////////////////////////////////
    ExtensionsOpenGL::~ExtensionsOpenGL()
    {
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL::init(ContextOpenGL* _context)
    {
        m_context = _context;

        return true;
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL::loadGLExtensionsImpl()
    {
        Debug::Log("loadGLExtensions started...");
        m_extensions.clear();

        MAZE_GL_MUTEX_SCOPED_LOCK(m_context->getRenderSystemRaw());

        m_context->makeCurrentContext(true);


        if (m_context->getMajorVersion() >= 3
            && mzglGetIntegerv != nullptr
            && mzglGetStringi != nullptr)
        {
            MZGLint n = 0;
            MZGLint i = 0;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_NUM_EXTENSIONS, &n));
            for (i = 0; i < n; i++)
            {
                CString extension;
                MAZE_GL_CALL(extension = (CString)mzglGetStringi(MAZE_GL_EXTENSIONS, i));
                if (extension)
                    m_extensions.insert(extension);
            }
        }
        else
        if (mzglGetString != nullptr)
        {
            CString extList = nullptr;
            MAZE_GL_CALL(extList = (CString)mzglGetString(MAZE_GL_EXTENSIONS));

            if (extList)
            {
                Vector<String> words;
                StringHelper::SplitWords(String(extList), words);
                for (Size i = 0, in = words.size(); i < in; ++i)
                    if (!words[i].empty())
                        m_extensions.insert(std::move(words[i]));
            }
        }

        saveCommonChecks();

        Debug::Log("loadGLExtensions finished.");

        return true;
    }

    //////////////////////////////////////////
    void ExtensionsOpenGL::printGLExtensions()
    {
        Vector<String> sortedExtensions(m_extensions.begin(), m_extensions.end());
        std::sort(sortedExtensions.begin(), sortedExtensions.end());

        MAZE_LOG("GL_EXTENSIONS: ");
        for (String const& glExtension : sortedExtensions)
            MAZE_LOG("\t- %s", glExtension.c_str());
    }

    //////////////////////////////////////////
    void ExtensionsOpenGL::printSupportedCompressedTextureFormats()
    {
        MAZE_LOG("GL_COMPRESSED_TEXTURE_FORMATS: ");
        for (MZGLint compressedTextureFormat : m_supportedCompressedTextureFormats)
        {
            CString name = "";

            switch (compressedTextureFormat)
            {
                case MAZE_GL_COMPRESSED_RGBA_BPTC_UNORM_ARB: name = "GL_COMPRESSED_RGBA_BPTC_UNORM_ARB"; break;
                case MAZE_GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB: name = "GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB"; break;
                case MAZE_GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB: name = "GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB"; break;
                case MAZE_GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB: name = "GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB"; break;
                case MAZE_GL_COMPRESSED_RED_RGTC1: name = "GL_COMPRESSED_RED_RGTC1"; break;
                case MAZE_GL_COMPRESSED_SIGNED_RED_RGTC1: name = "GL_COMPRESSED_SIGNED_RED_RGTC1"; break;
                case MAZE_GL_COMPRESSED_RG_RGTC2: name = "GL_COMPRESSED_RG_RGTC2"; break;
                case MAZE_GL_COMPRESSED_SIGNED_RG_RGTC2: name = "GL_COMPRESSED_SIGNED_RG_RGTC2"; break;
                case MAZE_GL_COMPRESSED_RGB_S3TC_DXT1_EXT: name = "GL_COMPRESSED_RGB_S3TC_DXT1_EXT"; break;
                case MAZE_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: name = "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT"; break;
                case MAZE_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: name = "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT"; break;
                case MAZE_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: name = "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT"; break;
                case MAZE_GL_PALETTE4_RGB8_OES: name = "GL_PALETTE4_RGB8_OES"; break;
                case MAZE_GL_PALETTE4_RGBA8_OES: name = "GL_PALETTE4_RGBA8_OES"; break;
                case MAZE_GL_PALETTE4_R5_G6_B5_OES: name = "GL_PALETTE4_R5_G6_B5_OES"; break;
                case MAZE_GL_PALETTE4_RGBA4_OES: name = "GL_PALETTE4_RGBA4_OES"; break;
                case MAZE_GL_PALETTE4_RGB5_A1_OES: name = "GL_PALETTE4_RGB5_A1_OES"; break;
                case MAZE_GL_PALETTE8_RGB8_OES: name = "GL_PALETTE8_RGB8_OES"; break;
                case MAZE_GL_PALETTE8_RGBA8_OES: name = "GL_PALETTE8_RGBA8_OES"; break;
                case MAZE_GL_PALETTE8_R5_G6_B5_OES: name = "GL_PALETTE8_R5_G6_B5_OES"; break;
                case MAZE_GL_PALETTE8_RGBA4_OES: name = "GL_PALETTE8_RGBA4_OES"; break;
                case MAZE_GL_PALETTE8_RGB5_A1_OES: name = "GL_PALETTE8_RGB5_A1_OES"; break;

                case MAZE_GL_COMPRESSED_R11_EAC: name = "GL_COMPRESSED_R11_EAC"; break;
                case MAZE_GL_COMPRESSED_SIGNED_R11_EAC: name = "GL_COMPRESSED_SIGNED_R11_EAC"; break;
                case MAZE_GL_COMPRESSED_RG11_EAC: name = "GL_COMPRESSED_RG11_EAC"; break;
                case MAZE_GL_COMPRESSED_SIGNED_RG11_EAC: name = "GL_COMPRESSED_SIGNED_RG11_EAC"; break;
                case MAZE_GL_COMPRESSED_RGB8_ETC2: name = "GL_COMPRESSED_RGB8_ETC2"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ETC2: name = "GL_COMPRESSED_SRGB8_ETC2"; break;
                case MAZE_GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2: name = "GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2"; break;
                case MAZE_GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2: name = "GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2"; break;
                case MAZE_GL_COMPRESSED_RGBA8_ETC2_EAC: name = "GL_COMPRESSED_RGBA8_ETC2_EAC"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC: name = "GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC"; break;

                case MAZE_GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG: name = "GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG"; break;
                case MAZE_GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG: name = "GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG"; break;
                case MAZE_GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG: name = "GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG"; break;
                case MAZE_GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: name = "GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_4x4_KHR: name = "GL_COMPRESSED_RGBA_ASTC_4x4_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_5x4_KHR: name = "GL_COMPRESSED_RGBA_ASTC_5x4_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_5x5_KHR: name = "GL_COMPRESSED_RGBA_ASTC_5x5_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_6x5_KHR: name = "GL_COMPRESSED_RGBA_ASTC_6x5_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_6x6_KHR: name = "GL_COMPRESSED_RGBA_ASTC_6x6_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_8x5_KHR: name = "GL_COMPRESSED_RGBA_ASTC_8x5_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_8x6_KHR: name = "GL_COMPRESSED_RGBA_ASTC_8x6_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_8x8_KHR: name = "GL_COMPRESSED_RGBA_ASTC_8x8_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_10x5_KHR: name = "GL_COMPRESSED_RGBA_ASTC_10x5_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_10x6_KHR: name = "GL_COMPRESSED_RGBA_ASTC_10x6_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_10x8_KHR: name = "GL_COMPRESSED_RGBA_ASTC_10x8_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_10x10_KHR: name = "GL_COMPRESSED_RGBA_ASTC_10x10_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_12x10_KHR: name = "GL_COMPRESSED_RGBA_ASTC_12x10_KHR"; break;
                case MAZE_GL_COMPRESSED_RGBA_ASTC_12x12_KHR: name = "GL_COMPRESSED_RGBA_ASTC_12x12_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR"; break;
                case MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR: name = "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR"; break;

                case MAZE_GL_COMPRESSED_SRGB_EXT: name = "GL_COMPRESSED_SRGB_EXT"; break;
                case MAZE_GL_COMPRESSED_SRGB_ALPHA_EXT: name = "GL_COMPRESSED_SRGB_ALPHA_EXT"; break;
                case MAZE_GL_COMPRESSED_SLUMINANCE_EXT: name = "GL_COMPRESSED_SLUMINANCE_EXT"; break;
                case MAZE_GL_COMPRESSED_SLUMINANCE_ALPHA_EXT: name = "GL_COMPRESSED_SLUMINANCE_ALPHA_EXT"; break;
                case MAZE_GL_COMPRESSED_SRGB_S3TC_DXT1_EXT: name = "GL_COMPRESSED_SRGB_S3TC_DXT1_EXT"; break;
                case MAZE_GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT: name = "GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT"; break;
                case MAZE_GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT: name = "GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT"; break;
                case MAZE_GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT: name = "GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT"; break;

                case MAZE_GL_COMPRESSED_RGB_FXT1_3DFX: name = "GL_COMPRESSED_RGB_FXT1_3DFX"; break;
                case MAZE_GL_COMPRESSED_RGBA_FXT1_3DFX: name = "GL_COMPRESSED_RGBA_FXT1_3DFX"; break;

                default:
                    break;
            }

            MAZE_LOG("\t- 0x%04X %s", compressedTextureFormat, name);
        }
    }

    //////////////////////////////////////////
    bool ExtensionsOpenGL::loadGLExtensions()
    {
        bool result = loadGLExtensionsImpl();

        if (result)
        {
            m_extensionsLoaded = true;
            eventLoaded();
        }

        return result;
    }

    //////////////////////////////////////////
    void ExtensionsOpenGL::saveCommonChecks()
    {
        bool isGLES = m_context->getConfig().flags & ContextOpenGLFlags::EmbeddedSystems;

        m_supportArbBufferStorage = m_context->hasMinVersion(4, 4) || hasGLExtension("GL_ARB_buffer_storage");
        m_supportMultisample = m_context->hasMinVersion(3, 3) || hasGLExtension("EXT_multisample_compatibility");
        m_supportClipDistance = hasGLExtension("GL_ARB_cull_distance") || hasGLExtension("GL_APPLE_clip_distance");
        m_supportFrameBufferObject = isGLES || hasGLExtension("GL_EXT_framebuffer_object");
        m_supportFrameBufferBlit = isGLES || hasGLExtension("GL_EXT_framebuffer_blit");

        if (mzglGetIntegerv)
        {
            MZGLint supportedCompressedTextureFormatsCount = 0;
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_NUM_COMPRESSED_TEXTURE_FORMATS, &supportedCompressedTextureFormatsCount));
            m_supportedCompressedTextureFormats.resize(supportedCompressedTextureFormatsCount);
            MAZE_GL_CALL(mzglGetIntegerv(MAZE_GL_COMPRESSED_TEXTURE_FORMATS, m_supportedCompressedTextureFormats.data()));
        }
        else
        {
            m_supportedCompressedTextureFormats.clear();
        }
    }


} // namespace Maze
//////////////////////////////////////////
