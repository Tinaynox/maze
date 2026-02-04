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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_GRAPHICS_API CString PixelFormat::ToCString(PixelFormat::Enum _value)
    {
        switch (_value)
        {
            case R_S8:          return "R_S8";
            case R_U8:          return "R_U8";
            case R_S16:         return "R_S16";
            case R_U16:         return "R_U16";
            case R_F16:         return "R_F16";
            case R_S32:         return "R_S32";
            case R_U32:         return "R_U32";
            case R_F32:         return "R_F32";

            case RG_S8:         return "RG_S8";
            case RG_U8:         return "RG_U8";
            case RG_S16:        return "RG_S16";
            case RG_U16:        return "RG_U16";
            case RG_F16:        return "RG_F16";
            case RG_S32:        return "RG_S32";
            case RG_U32:        return "RG_U32";
            case RG_F32:        return "RG_F32";

            case RGB_U3_3_2:    return "RGB_U3_3_2";
            case RGB_U5_6_5:    return "RGB_U5_6_5";
            case RGB_S8:        return "RGB_S8";
            case RGB_U8:        return "RGB_U8";
            case RGB_F32:       return "RGB_F32";

            case BGR_U5_6_5:    return "BGR_U5_6_5";
            case BGR_U8:        return "BGR_U8";

            case RGBA_U5_5_5_1: return "RGBA_U5_5_5_1";
            case RGBA_S8:       return "RGBA_S8";
            case RGBA_U8:       return "RGBA_U8";
            case RGBA_S16:      return "RGBA_S16";
            case RGBA_U16:      return "RGBA_U16";
            case RGBA_F16:      return "RGBA_F16";
            case RGBA_F32:      return "RGBA_F32";

            case BGRA_U8:       return "BGRA_U8";

            case DXT1_RGB:      return "DXT1_RGB";
            // case DXT2_RGBA:     return "DXT2_RGBA";
            case DXT3_RGBA:     return "DXT3_RGBA";
            // case DXT4_RGBA:     return "DXT4_RGBA";
            case DXT5_RGBA:     return "DXT5_RGBA";
                
            case PVRTC1_RGBA4:  return "PVRTC1_RGBA4";
            case PVRTC1_RGB4:   return "PVRTC1_RGB4";
            case PVRTC1_RGBA2:  return "PVRTC1_RGBA2";
            case PVRTC1_RGB2:   return "PVRTC1_RGB2";
            case PVRTC2_RGBA2:  return "PVRTC2_RGBA2";
            case PVRTC2_RGBA4:  return "PVRTC2_RGBA4";

            case ETC2_RGB:      return "ETC2_RGB";
            case ETC2_RGBA:     return "ETC2_RGBA";
            case ETC2_RGB_A1:   return "ETC2_RGB_A1";
                
            case ASTC_RGBA_4x4: return "ASTC_RGBA_4x4";
            case ASTC_RGBA_5x4: return "ASTC_RGBA_5x4";
            case ASTC_RGBA_5x5: return "ASTC_RGBA_5x5";
            case ASTC_RGBA_6x5: return "ASTC_RGBA_6x5";
            case ASTC_RGBA_6x6: return "ASTC_RGBA_6x6";
            case ASTC_RGBA_8x5: return "ASTC_RGBA_8x5";
            case ASTC_RGBA_8x6: return "ASTC_RGBA_8x6";
            case ASTC_RGBA_8x8: return "ASTC_RGBA_8x8";
            case ASTC_RGBA_10x5: return "ASTC_RGBA_10x5";
            case ASTC_RGBA_10x6: return "ASTC_RGBA_10x6";
            case ASTC_RGBA_10x8: return "ASTC_RGBA_10x8";
            case ASTC_RGBA_10x10: return "ASTC_RGBA_10x10";
            case ASTC_RGBA_12x10: return "ASTC_RGBA_12x10";
            case ASTC_RGBA_12x12: return "ASTC_RGBA_12x12";
            case ASTC_SRGBA_4x4: return "ASTC_SRGBA_4x4";
            case ASTC_SRGBA_5x4: return "ASTC_SRGBA_5x4";
            case ASTC_SRGBA_5x5: return "ASTC_SRGBA_5x5";
            case ASTC_SRGBA_6x5: return "ASTC_SRGBA_6x5";
            case ASTC_SRGBA_6x6: return "ASTC_SRGBA_6x6";
            case ASTC_SRGBA_8x5: return "ASTC_SRGBA_8x5";
            case ASTC_SRGBA_8x6: return "ASTC_SRGBA_8x6";
            case ASTC_SRGBA_8x8: return "ASTC_SRGBA_8x8";
            case ASTC_SRGBA_10x5: return "ASTC_SRGBA_10x5";
            case ASTC_SRGBA_10x6: return "ASTC_SRGBA_10x6";
            case ASTC_SRGBA_10x8: return "ASTC_SRGBA_10x8";
            case ASTC_SRGBA_10x10: return "ASTC_SRGBA_10x10";
            case ASTC_SRGBA_12x10: return "ASTC_SRGBA_12x10";
            case ASTC_SRGBA_12x12: return "ASTC_SRGBA_12x12";
                
            case DEPTH_U16: return "DEPTH_U16";
            case DEPTH_U24: return "DEPTH_U24";
            case DEPTH_U32: return "DEPTH_U32";
            case DEPTH_F32: return "DEPTH_F32";
            case DEPTH_STENCIL_U24_8: return "DEPTH_STENCIL_U24_8";
            case DEPTH_STENCIL_F32_U8: return "DEPTH_STENCIL_F32_U8";

            default:
                break;
        }

        return "";
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API String PixelFormat::ToString(PixelFormat::Enum _value)
    {
        return PixelFormat::ToCString(_value);
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API PixelFormat::Enum PixelFormat::FromString(CString _value)
    {
        if (strcmp(_value, "R_S8") == 0) return R_S8;
        if (strcmp(_value, "R_U8") == 0) return R_U8;
        if (strcmp(_value, "R_S16") == 0) return R_S16;
        if (strcmp(_value, "R_U16") == 0) return R_U16;
        if (strcmp(_value, "R_F16") == 0) return R_F16;
        if (strcmp(_value, "R_S32") == 0) return R_S32;
        if (strcmp(_value, "R_U32") == 0) return R_U32;
        if (strcmp(_value, "R_F32") == 0) return R_F32;
        
        if (strcmp(_value, "RG_S8") == 0) return RG_S8;
        if (strcmp(_value, "RG_U8") == 0) return RG_U8;
        if (strcmp(_value, "RG_S16") == 0) return RG_S16;
        if (strcmp(_value, "RG_U16") == 0) return RG_U16;
        if (strcmp(_value, "RG_F16") == 0) return RG_F16;
        if (strcmp(_value, "RG_S32") == 0) return RG_S32;
        if (strcmp(_value, "RG_U32") == 0) return RG_U32;
        if (strcmp(_value, "RG_F32") == 0) return RG_F32;
        
        if (strcmp(_value, "RGB_U3_3_2") == 0) return RGB_U3_3_2;
        if (strcmp(_value, "RGB_U5_6_5") == 0) return RGB_U5_6_5;
        if (strcmp(_value, "RGB_S8") == 0) return RGB_S8;
        if (strcmp(_value, "RGB_U8") == 0) return RGB_U8;
        if (strcmp(_value, "RGB_F32") == 0) return RGB_F32;
        
        if (strcmp(_value, "BGR_U5_6_5") == 0) return BGR_U5_6_5;
        if (strcmp(_value, "BGR_U8") == 0) return BGR_U8;
        
        if (strcmp(_value, "RGBA_U5_5_5_1") == 0) return RGBA_U5_5_5_1;
        if (strcmp(_value, "RGBA_S8") == 0) return RGBA_S8;
        if (strcmp(_value, "RGBA_U8") == 0) return RGBA_U8;
        if (strcmp(_value, "RGBA_S16") == 0) return RGBA_S16;
        if (strcmp(_value, "RGBA_U16") == 0) return RGBA_U16;
        if (strcmp(_value, "RGBA_F16") == 0) return RGBA_F16;
        if (strcmp(_value, "RGBA_F32") == 0) return RGBA_F32;
        
        if (strcmp(_value, "BGRA_U8") == 0) return BGRA_U8;

        if (strcmp(_value, "DXT1_RGB") == 0) return DXT1_RGB;
        // if (strcmp(_value, "DXT2_RGBA") == 0) return DXT2_RGBA;
        if (strcmp(_value, "DXT3_RGBA") == 0) return DXT3_RGBA;
        // if (strcmp(_value, "DXT4_RGBA") == 0) return DXT4_RGBA;
        if (strcmp(_value, "DXT5_RGBA") == 0) return DXT5_RGBA;
        
        if (strcmp(_value, "PVRTC1_RGBA4") == 0) return PVRTC1_RGBA4;
        if (strcmp(_value, "PVRTC1_RGB4") == 0) return PVRTC1_RGB4;
        if (strcmp(_value, "PVRTC1_RGBA2") == 0) return PVRTC1_RGBA2;
        if (strcmp(_value, "PVRTC1_RGB2") == 0) return PVRTC1_RGB2;
        if (strcmp(_value, "PVRTC2_RGBA2") == 0) return PVRTC2_RGBA2;
        if (strcmp(_value, "PVRTC2_RGBA4") == 0) return PVRTC2_RGBA4;
        
        if (strcmp(_value, "ETC2_RGB") == 0) return ETC2_RGB;
        if (strcmp(_value, "ETC2_RGBA") == 0) return ETC2_RGBA;
        if (strcmp(_value, "ETC2_RGB_A1") == 0) return ETC2_RGB_A1;
        
        if (strcmp(_value, "ASTC_RGBA_4x4") == 0) return ASTC_RGBA_4x4;
        if (strcmp(_value, "ASTC_RGBA_5x4") == 0) return ASTC_RGBA_5x4;
        if (strcmp(_value, "ASTC_RGBA_5x5") == 0) return ASTC_RGBA_5x5;
        if (strcmp(_value, "ASTC_RGBA_6x5") == 0) return ASTC_RGBA_6x5;
        if (strcmp(_value, "ASTC_RGBA_6x6") == 0) return ASTC_RGBA_6x6;
        if (strcmp(_value, "ASTC_RGBA_8x5") == 0) return ASTC_RGBA_8x5;
        if (strcmp(_value, "ASTC_RGBA_8x6") == 0) return ASTC_RGBA_8x6;
        if (strcmp(_value, "ASTC_RGBA_8x8") == 0) return ASTC_RGBA_8x8;
        if (strcmp(_value, "ASTC_RGBA_10x5") == 0) return ASTC_RGBA_10x5;
        if (strcmp(_value, "ASTC_RGBA_10x6") == 0) return ASTC_RGBA_10x6;
        if (strcmp(_value, "ASTC_RGBA_10x8") == 0) return ASTC_RGBA_10x8;
        if (strcmp(_value, "ASTC_RGBA_10x10") == 0) return ASTC_RGBA_10x10;
        if (strcmp(_value, "ASTC_RGBA_12x10") == 0) return ASTC_RGBA_12x10;
        if (strcmp(_value, "ASTC_RGBA_12x12") == 0) return ASTC_RGBA_12x12;
        if (strcmp(_value, "ASTC_SRGBA_4x4") == 0) return ASTC_SRGBA_4x4;
        if (strcmp(_value, "ASTC_SRGBA_5x4") == 0) return ASTC_SRGBA_5x4;
        if (strcmp(_value, "ASTC_SRGBA_5x5") == 0) return ASTC_SRGBA_5x5;
        if (strcmp(_value, "ASTC_SRGBA_6x5") == 0) return ASTC_SRGBA_6x5;
        if (strcmp(_value, "ASTC_SRGBA_6x6") == 0) return ASTC_SRGBA_6x6;
        if (strcmp(_value, "ASTC_SRGBA_8x5") == 0) return ASTC_SRGBA_8x5;
        if (strcmp(_value, "ASTC_SRGBA_8x6") == 0) return ASTC_SRGBA_8x6;
        if (strcmp(_value, "ASTC_SRGBA_8x8") == 0) return ASTC_SRGBA_8x8;
        if (strcmp(_value, "ASTC_SRGBA_10x5") == 0) return ASTC_SRGBA_10x5;
        if (strcmp(_value, "ASTC_SRGBA_10x6") == 0) return ASTC_SRGBA_10x6;
        if (strcmp(_value, "ASTC_SRGBA_10x8") == 0) return ASTC_SRGBA_10x8;
        if (strcmp(_value, "ASTC_SRGBA_10x10") == 0) return ASTC_SRGBA_10x10;
        if (strcmp(_value, "ASTC_SRGBA_12x10") == 0) return ASTC_SRGBA_12x10;
        if (strcmp(_value, "ASTC_SRGBA_12x12") == 0) return ASTC_SRGBA_12x12;
        
        if (strcmp(_value, "DEPTH_U16") == 0) return DEPTH_U16;
        if (strcmp(_value, "DEPTH_U24") == 0) return DEPTH_U24;
        if (strcmp(_value, "DEPTH_U32") == 0) return DEPTH_U32;
        if (strcmp(_value, "DEPTH_F32") == 0) return DEPTH_F32;
        if (strcmp(_value, "DEPTH_STENCIL_U24_8") == 0) return DEPTH_STENCIL_U24_8;
        if (strcmp(_value, "DEPTH_STENCIL_F32_U8") == 0) return DEPTH_STENCIL_F32_U8;

        return PixelFormat::None;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API PixelFormat::Enum PixelFormat::FromString(String const& _value)
    {
        return PixelFormat::FromString(_value.c_str());
    }
    
    //////////////////////////////////////////
    MAZE_GRAPHICS_API U32 PixelFormat::GetBytesPerPixel(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8: return 1;
            case PixelFormat::R_U8: return 1;
            case PixelFormat::R_S16: return 2;
            case PixelFormat::R_U16: return 2;
            case PixelFormat::R_F16: return 2;
            case PixelFormat::R_S32: return 4;
            case PixelFormat::R_U32: return 4;
            case PixelFormat::R_F32: return 4;
            
            case PixelFormat::RG_S8: return 2;
            case PixelFormat::RG_U8: return 2;
            case PixelFormat::RG_S16: return 4;
            case PixelFormat::RG_U16: return 4;
            case PixelFormat::RG_F16: return 4;
            case PixelFormat::RG_S32: return 8;
            case PixelFormat::RG_U32: return 8;
            case PixelFormat::RG_F32: return 8;
            
            case PixelFormat::RGB_U3_3_2: return 1;
            case PixelFormat::RGB_U5_6_5: return 2;
            case PixelFormat::RGB_S8: return 3;
            case PixelFormat::RGB_U8: return 3;
            case PixelFormat::RGB_F32: return 12;

            case PixelFormat::BGR_U8: return 3;
        
            case PixelFormat::RGBA_U5_5_5_1: return 2;
            case PixelFormat::RGBA_S8: return 4;
            case PixelFormat::RGBA_U8: return 4;
            case PixelFormat::RGBA_S16: return 8;
            case PixelFormat::RGBA_U16: return 8;
            case PixelFormat::RGBA_F16: return 8;
            case PixelFormat::RGBA_F32: return 16;

            case PixelFormat::BGRA_U8: return 4;
            
            case PixelFormat::DXT1_RGB: return 0;
            // case PixelFormat::DXT2_RGBA: return 0;
            case PixelFormat::DXT3_RGBA: return 0;
            // case PixelFormat::DXT4_RGBA: return 0;
            case PixelFormat::DXT5_RGBA: return 0;
            
            case PixelFormat::PVRTC1_RGBA4: return 0;
            case PixelFormat::PVRTC1_RGB4: return 0;
            case PixelFormat::PVRTC1_RGBA2: return 0;
            case PixelFormat::PVRTC1_RGB2: return 0;
            case PixelFormat::PVRTC2_RGBA2: return 0;
            case PixelFormat::PVRTC2_RGBA4: return 0;

            case PixelFormat::ETC2_RGB: return 0;
            case PixelFormat::ETC2_RGBA: return 0;
            case PixelFormat::ETC2_RGB_A1: return 0;
            
            case PixelFormat::DEPTH_U16: return 2;
            case PixelFormat::DEPTH_U24: return 3;
            case PixelFormat::DEPTH_U32: return 4;
            case PixelFormat::DEPTH_F32: return 4;
            case PixelFormat::DEPTH_STENCIL_U24_8: return 4;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return 8;

            default:
                break;
        }
        
        MAZE_ERROR_RETURN_VALUE(0, "Unsupported PixelFormat: %d", (S32)_pixelFormat);
        return 0;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API U32 PixelFormat::GetChannelsPerPixel(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8:
            case PixelFormat::R_U8:
            case PixelFormat::R_S16:
            case PixelFormat::R_U16:
            case PixelFormat::R_F16:
            case PixelFormat::R_S32:
            case PixelFormat::R_U32:
            case PixelFormat::R_F32:
                return 1;
            
            case PixelFormat::RG_S8:
            case PixelFormat::RG_U8:
            case PixelFormat::RG_S16:
            case PixelFormat::RG_U16:
            case PixelFormat::RG_F16:
            case PixelFormat::RG_S32:
            case PixelFormat::RG_U32:
            case PixelFormat::RG_F32:
                return 2;
            
            case PixelFormat::RGB_U3_3_2:
            case PixelFormat::RGB_U5_6_5:
            case PixelFormat::RGB_S8:
            case PixelFormat::RGB_U8:
            case PixelFormat::RGB_F32:
                return 3;
        
            case PixelFormat::RGBA_U5_5_5_1:
            case PixelFormat::RGBA_S8:
            case PixelFormat::RGBA_U8:
            case PixelFormat::RGBA_S16:
            case PixelFormat::RGBA_U16:
            case PixelFormat::RGBA_F16:
            case PixelFormat::RGBA_F32:
                return 4;

            case PixelFormat::BGRA_U8:
                return 4;
            
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
            
            case PixelFormat::DEPTH_U16:
            case PixelFormat::DEPTH_U24:
            case PixelFormat::DEPTH_U32:
            case PixelFormat::DEPTH_F32:
            case PixelFormat::DEPTH_STENCIL_U24_8:
            case PixelFormat::DEPTH_STENCIL_F32_U8:
                return 0;

            default:
                break;
        }

         return 0;
    }
    
    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool PixelFormat::IsCompressed(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::DXT1_RGB:
            // case PixelFormat::DXT2_RGBA:
            case PixelFormat::DXT3_RGBA:
            // case PixelFormat::DXT4_RGBA:
            case PixelFormat::DXT5_RGBA:
                
            case PixelFormat::PVRTC1_RGBA4:
            case PixelFormat::PVRTC1_RGB4:
            case PixelFormat::PVRTC1_RGBA2:
            case PixelFormat::PVRTC1_RGB2:
            case PixelFormat::PVRTC2_RGBA2:
            case PixelFormat::PVRTC2_RGBA4:

            case PixelFormat::ETC2_RGB:
            case PixelFormat::ETC2_RGBA:
            case PixelFormat::ETC2_RGB_A1:
                
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
                return true;
                
            default:
                break;
        }
        
        return false;
    }
    
    //////////////////////////////////////////
    static inline Size CalculateRequiredBytesASTCSlice(
        U32 _width,
        U32 _height,
        U32 _blockWidth,
        U32 _blockHeight)
    {
        return  ((_width + _blockWidth - 1) / _blockWidth) *
                ((_height + _blockHeight - 1) / _blockHeight) * 16;
    }
    
    //////////////////////////////////////////
    MAZE_GRAPHICS_API Size PixelFormat::CalculateRequiredBytes(
        U32 _width,
        U32 _height,
        U32 _depth,
        PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::DXT1_RGB:
                return ((_width + 3) / 4) * ((_height + 3) / 4) * 8 * _depth;
            // case PixelFormat::DXT2_RGBA:
            case PixelFormat::DXT3_RGBA:
            // case PixelFormat::DXT4_RGBA:
            case PixelFormat::DXT5_RGBA:
                return ((_width + 3) / 4) * ((_height + 3) / 4) * 16 * _depth;
            
            case PixelFormat::PVRTC1_RGBA2:
            case PixelFormat::PVRTC1_RGB2:
            case PixelFormat::PVRTC2_RGBA2:
                return (Math::Max((S32)_width, 16) * Math::Max((S32)_height, 8) * 2 + 7) / 8;
                
                
            case PixelFormat::PVRTC1_RGBA4:
            case PixelFormat::PVRTC1_RGB4:
            case PixelFormat::PVRTC2_RGBA4:
                return (Math::Max((S32)_width, 8) * Math::Max((S32)_height, 8) * 4 + 7) / 8;

            case PixelFormat::ETC2_RGB:
                return Math::Ceil(_width / 4) * Math::Ceil(_height / 4) * 8 * _depth;
            case PixelFormat::ETC2_RGBA:
                return Math::Ceil(_width / 4) * Math::Ceil(_height / 4) * 16 * _depth;
            case PixelFormat::ETC2_RGB_A1:
                return Math::Ceil(_width / 4) * Math::Ceil(_height / 4) * 8 * _depth;
                
            case PixelFormat::ASTC_RGBA_4x4: 
                return CalculateRequiredBytesASTCSlice(_width, _height, 4, 4) * _depth;
            case PixelFormat::ASTC_RGBA_5x4:
                return CalculateRequiredBytesASTCSlice(_width, _height, 5, 4) * _depth;
            case PixelFormat::ASTC_RGBA_5x5:
                return CalculateRequiredBytesASTCSlice(_width, _height, 5, 5) * _depth;
            case PixelFormat::ASTC_RGBA_6x5:
                return CalculateRequiredBytesASTCSlice(_width, _height, 6, 5) * _depth;
            case PixelFormat::ASTC_RGBA_6x6:
                return CalculateRequiredBytesASTCSlice(_width, _height, 6, 6) * _depth;
            case PixelFormat::ASTC_RGBA_8x5:
                return CalculateRequiredBytesASTCSlice(_width, _height, 8, 5) * _depth;
            case PixelFormat::ASTC_RGBA_8x6:   
                return CalculateRequiredBytesASTCSlice(_width, _height, 8, 6) * _depth;
            case PixelFormat::ASTC_RGBA_8x8:
                return CalculateRequiredBytesASTCSlice(_width, _height, 8, 8) * _depth;
            case PixelFormat::ASTC_RGBA_10x5:
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 5) * _depth;
            case PixelFormat::ASTC_RGBA_10x6:
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 6) * _depth;
            case PixelFormat::ASTC_RGBA_10x8:
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 8) * _depth;
            case PixelFormat::ASTC_RGBA_10x10:
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 10) * _depth;
            case PixelFormat::ASTC_RGBA_12x10:
                return CalculateRequiredBytesASTCSlice(_width, _height, 12, 10) * _depth;
            case PixelFormat::ASTC_RGBA_12x12:
                return CalculateRequiredBytesASTCSlice(_width, _height, 12, 12) * _depth;
            case PixelFormat::ASTC_SRGBA_4x4:  
                return CalculateRequiredBytesASTCSlice(_width, _height, 4, 4) * _depth;
            case PixelFormat::ASTC_SRGBA_5x4:
                return CalculateRequiredBytesASTCSlice(_width, _height, 5, 4) * _depth;
            case PixelFormat::ASTC_SRGBA_5x5:  
                return CalculateRequiredBytesASTCSlice(_width, _height, 5, 5) * _depth;
            case PixelFormat::ASTC_SRGBA_6x5:  
                return CalculateRequiredBytesASTCSlice(_width, _height, 6, 5) * _depth;
            case PixelFormat::ASTC_SRGBA_6x6:  
                return CalculateRequiredBytesASTCSlice(_width, _height, 6, 6) * _depth;
            case PixelFormat::ASTC_SRGBA_8x5:  
                return CalculateRequiredBytesASTCSlice(_width, _height, 8, 5) * _depth;
            case PixelFormat::ASTC_SRGBA_8x6:  
                return CalculateRequiredBytesASTCSlice(_width, _height, 8, 6) * _depth;
            case PixelFormat::ASTC_SRGBA_8x8:  
                return CalculateRequiredBytesASTCSlice(_width, _height, 8, 8) * _depth;
            case PixelFormat::ASTC_SRGBA_10x5: 
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 5) * _depth;
            case PixelFormat::ASTC_SRGBA_10x6: 
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 6) * _depth;
            case PixelFormat::ASTC_SRGBA_10x8: 
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 8) * _depth;
            case PixelFormat::ASTC_SRGBA_10x10:
                return CalculateRequiredBytesASTCSlice(_width, _height, 10, 10) * _depth;
            case PixelFormat::ASTC_SRGBA_12x10:
                return CalculateRequiredBytesASTCSlice(_width, _height, 12, 10) * _depth;
            case PixelFormat::ASTC_SRGBA_12x12:
                return CalculateRequiredBytesASTCSlice(_width, _height, 12, 12) * _depth;
                
            default:
                return _width * _height * _depth * GetBytesPerPixel(_pixelFormat);
        }

    } // namespace PixelFormat
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
