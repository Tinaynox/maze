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
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLint GetOpenGLOriginFormat(PixelFormat::Enum _pixelFormat, bool _normalized)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8:      return _normalized ? MAZE_GL_RED : MAZE_GL_RED_INTEGER;
            case PixelFormat::R_U8:      return _normalized ? MAZE_GL_RED : MAZE_GL_RED_INTEGER;
            case PixelFormat::R_S16:     return _normalized ? MAZE_GL_RED : MAZE_GL_RED_INTEGER;
            case PixelFormat::R_U16:     return /*_normalized ? MAZE_GL_RED : */MAZE_GL_RED_INTEGER;
            case PixelFormat::R_F16:     return MAZE_GL_RED;
            case PixelFormat::R_S32:     return MAZE_GL_RED_INTEGER;
            case PixelFormat::R_U32:     return MAZE_GL_RED_INTEGER;
            case PixelFormat::R_F32:     return MAZE_GL_RED;


            case PixelFormat::RG_S8:     return _normalized ? MAZE_GL_RG : MAZE_GL_RG_INTEGER;
            case PixelFormat::RG_U8:     return _normalized ? MAZE_GL_RG : MAZE_GL_RG_INTEGER;
            case PixelFormat::RG_S16:    return _normalized ? MAZE_GL_RG : MAZE_GL_RG_INTEGER;
            case PixelFormat::RG_U16:    return /*normalized ? MAZE_GL_RG : */MAZE_GL_RG_INTEGER;
            case PixelFormat::RG_F16:    return MAZE_GL_RG;
            case PixelFormat::RG_S32:    return MAZE_GL_RG_INTEGER;
            case PixelFormat::RG_U32:    return MAZE_GL_RG_INTEGER;
            case PixelFormat::RG_F32:    return MAZE_GL_RG;

            case PixelFormat::RGB_U3_3_2:
            case PixelFormat::RGB_U5_6_5:
            case PixelFormat::RGB_U8:
            case PixelFormat::RGB_F32:
                return MAZE_GL_RGB;

            case PixelFormat::BGR_U5_6_5:
            case PixelFormat::BGR_U8:
                return MAZE_GL_BGR;

            case PixelFormat::RGBA_U5_5_5_1:
            case PixelFormat::RGBA_U8:
            case PixelFormat::RGBA_U16:
            case PixelFormat::RGBA_F16:
            case PixelFormat::RGBA_F32:
                return MAZE_GL_RGBA;

            case PixelFormat::BGRA_U8:
                return MAZE_GL_BGRA;

            case PixelFormat::DXT1_RGB:
            // case PixelFormat::DXT2_RGBA:
            case PixelFormat::DXT3_RGBA:
            // case PixelFormat::DXT4_RGBA:
            case PixelFormat::DXT5_RGBA:
                return 0;

            case PixelFormat::PVRTC1_RGBA4:
            case PixelFormat::PVRTC1_RGB4:
            case PixelFormat::PVRTC1_RGBA2:
            case PixelFormat::PVRTC1_RGB2:
            case PixelFormat::PVRTC2_RGBA2:
            case PixelFormat::PVRTC2_RGBA4:
                return 0;

            case PixelFormat::ETC2_RGB:
                return MAZE_GL_RGB;
            case PixelFormat::ETC2_RGBA:
            case PixelFormat::ETC2_RGB_A1:
                return MAZE_GL_RGBA;


            case PixelFormat::ASTC_RGBA_4x4:
            case PixelFormat::ASTC_RGBA_5x4:
            case PixelFormat::ASTC_RGBA_5x5:
            case PixelFormat::ASTC_RGBA_6x5:
            case PixelFormat::ASTC_RGBA_6x6:
            case PixelFormat::ASTC_RGBA_8x5:
            case PixelFormat::ASTC_RGBA_8x6:
            case PixelFormat::ASTC_RGBA_8x8:
            case PixelFormat::ASTC_RGBA_10x5:
            case PixelFormat::ASTC_RGBA_10x6:
            case PixelFormat::ASTC_RGBA_10x8:
            case PixelFormat::ASTC_RGBA_10x10:
            case PixelFormat::ASTC_RGBA_12x10:
            case PixelFormat::ASTC_RGBA_12x12:
            case PixelFormat::ASTC_SRGBA_4x4:
            case PixelFormat::ASTC_SRGBA_5x4:
            case PixelFormat::ASTC_SRGBA_5x5:
            case PixelFormat::ASTC_SRGBA_6x5:
            case PixelFormat::ASTC_SRGBA_6x6:
            case PixelFormat::ASTC_SRGBA_8x5:
            case PixelFormat::ASTC_SRGBA_8x6:
            case PixelFormat::ASTC_SRGBA_8x8:
            case PixelFormat::ASTC_SRGBA_10x5:
            case PixelFormat::ASTC_SRGBA_10x6:
            case PixelFormat::ASTC_SRGBA_10x8:
            case PixelFormat::ASTC_SRGBA_10x10:
            case PixelFormat::ASTC_SRGBA_12x10:
            case PixelFormat::ASTC_SRGBA_12x12:
                return 0;

            case PixelFormat::DEPTH_U16: return MAZE_GL_DEPTH_COMPONENT;
            case PixelFormat::DEPTH_U24: return MAZE_GL_DEPTH_COMPONENT;
            case PixelFormat::DEPTH_U32: return MAZE_GL_DEPTH_COMPONENT;
            case PixelFormat::DEPTH_F32: return MAZE_GL_DEPTH_COMPONENT;
            case PixelFormat::DEPTH_STENCIL_U24_8: return MAZE_GL_DEPTH_STENCIL;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return MAZE_GL_DEPTH_STENCIL;

            default:
                break;
        }

        MAZE_ERROR("Unsupported origin format: %s", PixelFormat::ToString(_pixelFormat).c_str());
        return 0;
    }

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLint GetOpenGLInternalFormat(PixelFormat::Enum _pixelFormat, bool _normalized)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8:     return _normalized ? MAZE_GL_R8_SNORM : MAZE_GL_R8I;
            case PixelFormat::R_U8:     return _normalized ? MAZE_GL_R8 : MAZE_GL_R8UI;
            case PixelFormat::R_S16:    return _normalized ? MAZE_GL_R16_SNORM : MAZE_GL_R16I;
            case PixelFormat::R_U16:    return /*_normalized ? MAZE_GL_R16 : */MAZE_GL_R16UI;
            case PixelFormat::R_F16:    return MAZE_GL_R16F;
            case PixelFormat::R_S32:    return MAZE_GL_R32I;
            case PixelFormat::R_U32:    return MAZE_GL_R32UI;
            case PixelFormat::R_F32:    return MAZE_GL_R32F;

            case PixelFormat::RG_S8:    return _normalized ? MAZE_GL_RG8_SNORM : MAZE_GL_RG8I;
            case PixelFormat::RG_U8:    return _normalized ? MAZE_GL_RG8 : MAZE_GL_RG8UI;
            case PixelFormat::RG_S16:   return _normalized ? MAZE_GL_RG16_SNORM : MAZE_GL_RG16I;
            case PixelFormat::RG_U16:   return /*_normalized ? MAZE_GL_RG16 : */MAZE_GL_RG16UI;
            case PixelFormat::RG_F16:   return MAZE_GL_RG16F;
            case PixelFormat::RG_S32:   return MAZE_GL_RG32I;
            case PixelFormat::RG_U32:   return MAZE_GL_RG32UI;
            case PixelFormat::RG_F32:   return MAZE_GL_RG32F;

            case PixelFormat::RGB_U3_3_2:    return MAZE_GL_R3_G3_B2;
            case PixelFormat::RGB_U5_6_5:    return MAZE_GL_RGB565;
            case PixelFormat::RGB_S8:        return _normalized ? MAZE_GL_RGB8_SNORM : MAZE_GL_RGB8I;
            case PixelFormat::RGB_U8:        return _normalized ? MAZE_GL_RGB8 : MAZE_GL_RGB8UI;
            case PixelFormat::RGB_F32:       return MAZE_GL_RGB32F;

            case PixelFormat::BGR_U5_6_5:    return MAZE_GL_RGB565;
            case PixelFormat::BGR_U8:        return _normalized ? MAZE_GL_RGB8 : MAZE_GL_RGB8UI;

            case PixelFormat::RGBA_U5_5_5_1: return MAZE_GL_RGB5_A1;
            case PixelFormat::RGBA_S8:       return _normalized ? MAZE_GL_RGBA8_SNORM : MAZE_GL_RGBA8I;
            case PixelFormat::RGBA_U8:       return _normalized ? MAZE_GL_RGBA8 : MAZE_GL_RGBA8UI;
            case PixelFormat::RGBA_S16:      return _normalized ? MAZE_GL_RGBA16_SNORM : MAZE_GL_RGBA16I;
            case PixelFormat::RGBA_U16:      return /*_normalized ? MAZE_GL_RGBA16 : */MAZE_GL_RGBA16UI;
            case PixelFormat::RGBA_F16:      return MAZE_GL_RGBA16F;
            case PixelFormat::RGBA_F32:      return MAZE_GL_RGBA32F;

            case PixelFormat::BGRA_U8:       return MAZE_GL_RGBA8;

            case PixelFormat::DXT1_RGB:     return MAZE_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            // case PixelFormat::DXT2_RGBA:     MAZE_ERROR("Not supported"); return 0;
            case PixelFormat::DXT3_RGBA:     return MAZE_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            // case PixelFormat::DXT4_RGBA:     MAZE_ERROR("Not supported"); return 0;
            case PixelFormat::DXT5_RGBA:     return MAZE_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

            case PixelFormat::PVRTC1_RGBA4:
                return MAZE_GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;

            case PixelFormat::PVRTC1_RGB4:
                return MAZE_GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;

            case PixelFormat::PVRTC1_RGBA2:
                return MAZE_GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;

            case PixelFormat::PVRTC1_RGB2:
                return MAZE_GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;

            case PixelFormat::PVRTC2_RGBA2:
                return 0;

            case PixelFormat::PVRTC2_RGBA4:
                return 0;

            case PixelFormat::ETC2_RGB:
                return MAZE_GL_COMPRESSED_RGB8_ETC2;

            case PixelFormat::ETC2_RGBA:
                return MAZE_GL_COMPRESSED_RGBA8_ETC2_EAC;

            case PixelFormat::ETC2_RGB_A1:
                return MAZE_GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2;

            case PixelFormat::ASTC_RGBA_4x4:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
            case PixelFormat::ASTC_RGBA_5x4:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_5x4_KHR;
            case PixelFormat::ASTC_RGBA_5x5:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_5x5_KHR;
            case PixelFormat::ASTC_RGBA_6x5:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_6x5_KHR;
            case PixelFormat::ASTC_RGBA_6x6:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_6x6_KHR;
            case PixelFormat::ASTC_RGBA_8x5:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_8x5_KHR;
            case PixelFormat::ASTC_RGBA_8x6:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_8x6_KHR;
            case PixelFormat::ASTC_RGBA_8x8:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
            case PixelFormat::ASTC_RGBA_10x5:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_10x5_KHR;
            case PixelFormat::ASTC_RGBA_10x6:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_10x6_KHR;
            case PixelFormat::ASTC_RGBA_10x8:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_10x8_KHR;
            case PixelFormat::ASTC_RGBA_10x10:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_10x10_KHR;
            case PixelFormat::ASTC_RGBA_12x10:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_12x10_KHR;
            case PixelFormat::ASTC_RGBA_12x12:
                return MAZE_GL_COMPRESSED_RGBA_ASTC_12x12_KHR;
            case PixelFormat::ASTC_SRGBA_4x4:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR;
            case PixelFormat::ASTC_SRGBA_5x4:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR;
            case PixelFormat::ASTC_SRGBA_5x5:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR;
            case PixelFormat::ASTC_SRGBA_6x5:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR;
            case PixelFormat::ASTC_SRGBA_6x6:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR;
            case PixelFormat::ASTC_SRGBA_8x5:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR;
            case PixelFormat::ASTC_SRGBA_8x6:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR;
            case PixelFormat::ASTC_SRGBA_8x8:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR;
            case PixelFormat::ASTC_SRGBA_10x5:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR;
            case PixelFormat::ASTC_SRGBA_10x6:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR;
            case PixelFormat::ASTC_SRGBA_10x8:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR;
            case PixelFormat::ASTC_SRGBA_10x10:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR;
            case PixelFormat::ASTC_SRGBA_12x10:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR;
            case PixelFormat::ASTC_SRGBA_12x12:
                return MAZE_GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR;

            case PixelFormat::DEPTH_U16: return MAZE_GL_DEPTH_COMPONENT16;
            case PixelFormat::DEPTH_U24: return MAZE_GL_DEPTH_COMPONENT24;
            case PixelFormat::DEPTH_U32: return MAZE_GL_DEPTH_COMPONENT32;
            case PixelFormat::DEPTH_F32: return MAZE_GL_DEPTH_COMPONENT32F;
            case PixelFormat::DEPTH_STENCIL_U24_8: return MAZE_GL_DEPTH24_STENCIL8;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return MAZE_GL_DEPTH32F_STENCIL8;

            default:
                break;
        }

        MAZE_ERROR("Unsupported Pixel Format: %s", PixelFormat::ToString(_pixelFormat).c_str());
        
        return 0;
    }

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_OPENGL_CORE_API MZGLint GetOpenGLDataType(PixelFormat::Enum _pixelFormat, bool _normalized)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8:             return MAZE_GL_BYTE;
            case PixelFormat::R_U8:             return MAZE_GL_UNSIGNED_BYTE;
            case PixelFormat::R_S16:            return MAZE_GL_SHORT;
            case PixelFormat::R_U16:            return MAZE_GL_UNSIGNED_SHORT;
            case PixelFormat::R_F16:            return MAZE_GL_HALF_FLOAT;
            case PixelFormat::R_S32:            return MAZE_GL_INT;
            case PixelFormat::R_U32:            return MAZE_GL_UNSIGNED_INT;
            case PixelFormat::R_F32:            return MAZE_GL_FLOAT;

            case PixelFormat::RG_S8:            return MAZE_GL_BYTE;
            case PixelFormat::RG_U8:            return MAZE_GL_UNSIGNED_BYTE;
            case PixelFormat::RG_S16:           return MAZE_GL_SHORT;
            case PixelFormat::RG_U16:           return MAZE_GL_UNSIGNED_SHORT;
            case PixelFormat::RG_F16:           return MAZE_GL_HALF_FLOAT;
            case PixelFormat::RG_S32:           return MAZE_GL_INT;
            case PixelFormat::RG_U32:           return MAZE_GL_UNSIGNED_INT;
            case PixelFormat::RG_F32:           return MAZE_GL_FLOAT;

            case PixelFormat::RGB_U3_3_2:       return MAZE_GL_UNSIGNED_BYTE_3_3_2;
            case PixelFormat::RGB_U5_6_5:       return MAZE_GL_UNSIGNED_SHORT_5_6_5;
            case PixelFormat::RGB_S8:           return MAZE_GL_BYTE;
            case PixelFormat::RGB_U8:           return MAZE_GL_UNSIGNED_BYTE;
            case PixelFormat::RGB_F32:          return MAZE_GL_FLOAT;

            case PixelFormat::BGR_U5_6_5:       return MAZE_GL_UNSIGNED_SHORT_5_6_5;
            case PixelFormat::BGR_U8:           return MAZE_GL_UNSIGNED_BYTE;

            case PixelFormat::RGBA_U5_5_5_1:    return MAZE_GL_UNSIGNED_SHORT_5_5_5_1;
            case PixelFormat::RGBA_S8:          return MAZE_GL_BYTE;
            case PixelFormat::RGBA_U8:          return MAZE_GL_UNSIGNED_BYTE;
            case PixelFormat::RGBA_S16:         return MAZE_GL_SHORT;
            case PixelFormat::RGBA_U16:         return MAZE_GL_UNSIGNED_SHORT;
            case PixelFormat::RGBA_F16:         return MAZE_GL_HALF_FLOAT;
            case PixelFormat::RGBA_F32:         return MAZE_GL_FLOAT;

            case PixelFormat::BGRA_U8:          return MAZE_GL_UNSIGNED_BYTE;

            case PixelFormat::DXT1_RGB:
            // case PixelFormat::DXT2_RGBA:
            case PixelFormat::DXT3_RGBA:
            // case PixelFormat::DXT4_RGBA:
            case PixelFormat::DXT5_RGBA:
                return 0;

            case PixelFormat::PVRTC1_RGBA4:
            case PixelFormat::PVRTC1_RGB4:
            case PixelFormat::PVRTC1_RGBA2:
            case PixelFormat::PVRTC1_RGB2:
            case PixelFormat::PVRTC2_RGBA2:
            case PixelFormat::PVRTC2_RGBA4:
                return 0;

            case PixelFormat::ETC2_RGB:
            case PixelFormat::ETC2_RGBA:
            case PixelFormat::ETC2_RGB_A1:
                return 0;

            case PixelFormat::ASTC_RGBA_4x4:
            case PixelFormat::ASTC_RGBA_5x4:
            case PixelFormat::ASTC_RGBA_5x5:
            case PixelFormat::ASTC_RGBA_6x5:
            case PixelFormat::ASTC_RGBA_6x6:
            case PixelFormat::ASTC_RGBA_8x5:
            case PixelFormat::ASTC_RGBA_8x6:
            case PixelFormat::ASTC_RGBA_8x8:
            case PixelFormat::ASTC_RGBA_10x5:
            case PixelFormat::ASTC_RGBA_10x6:
            case PixelFormat::ASTC_RGBA_10x8:
            case PixelFormat::ASTC_RGBA_10x10:
            case PixelFormat::ASTC_RGBA_12x10:
            case PixelFormat::ASTC_RGBA_12x12:
            case PixelFormat::ASTC_SRGBA_4x4:
            case PixelFormat::ASTC_SRGBA_5x4:
            case PixelFormat::ASTC_SRGBA_5x5:
            case PixelFormat::ASTC_SRGBA_6x5:
            case PixelFormat::ASTC_SRGBA_6x6:
            case PixelFormat::ASTC_SRGBA_8x5:
            case PixelFormat::ASTC_SRGBA_8x6:
            case PixelFormat::ASTC_SRGBA_8x8:
            case PixelFormat::ASTC_SRGBA_10x5:
            case PixelFormat::ASTC_SRGBA_10x6:
            case PixelFormat::ASTC_SRGBA_10x8:
            case PixelFormat::ASTC_SRGBA_10x10:
            case PixelFormat::ASTC_SRGBA_12x10:
            case PixelFormat::ASTC_SRGBA_12x12:
                return 0;

            case PixelFormat::DEPTH_U16: return MAZE_GL_UNSIGNED_SHORT;
            case PixelFormat::DEPTH_U24: return MAZE_GL_UNSIGNED_INT;
            case PixelFormat::DEPTH_U32: return MAZE_GL_UNSIGNED_INT;
            case PixelFormat::DEPTH_F32: return MAZE_GL_FLOAT;
            case PixelFormat::DEPTH_STENCIL_U24_8: return MAZE_GL_UNSIGNED_INT_24_8;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return MAZE_GL_FLOAT_32_UNSIGNED_INT_24_8_REV;

            default:
                break;
        }

        MAZE_ERROR("Unsupported Data Type: %s", PixelFormat::ToString(_pixelFormat).c_str());
        return 0;
    }

    
} // namespace Maze
//////////////////////////////////////////
