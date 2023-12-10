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
#if (!defined(_MazePixelFormat_hpp_))
#define _MazePixelFormat_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace PixelFormat
    {
        //////////////////////////////////////////
        enum Enum
        {
            Unknown                 = -1,
            None                    = 0,
            
            // R
            R_S8                    = 1,    // 8-bit  = R(S8)
            R_U8                    = 2,    // 8-bit  = R(S8)
            R_S16                   = 3,    // 16-bit = R(S16)
            R_U16                   = 4,    // 16-bit = R(U16)
            R_F16                   = 5,    // 16-bit = R(F16)
            R_S32                   = 6,    // 32-bit = R(S32)
            R_U32                   = 7,    // 32-bit = R(U32)
            R_F32                   = 8,    // 32-bit = R(F32)

            // RG
            RG_S8                   = 50,    // 16-bit = R(S8) + G(S8)
            RG_U8                   = 51,    // 16-bit = R(U8) + G(U8)
            RG_S16                  = 52,    // 32-bit = R(S16) + G(S16)
            RG_U16                  = 53,    // 32-bit = R(U16) + G(U16)
            RG_F16                  = 54,    // 32-bit = R(F16) + G(F16)
            RG_S32                  = 55,    // 64-bit = R(S32) + G(S32)
            RG_U32                  = 56,    // 64-bit = R(U32) + G(U32)
            RG_F32                  = 57,    // 64-bit = R(F32) + G(F32)

            // RGB
            RGB_U3_3_2              = 100,    // 8-bit  = R(U3) + G(U3) + B(U2)
            RGB_U5_6_5              = 101,    // 16-bit = R(U5) + G(U6) + B(U5)
            RGB_S8                  = 102,    // 24-bit = R(S8) + G(S8) + B(S8)
            RGB_U8                  = 103,    // 24-bit = R(U8) + G(U8) + B(U8)
            RGB_F32                 = 104,    // 96-bit = R(F32) + G(F32) + B(F32)

            // BGR
            BGR_U5_6_5              = 120,    // 16-bit = B(U5) + G(U6) + R(U5)
            BGR_U8                  = 121,    // 24-bit = B(U8) + G(U8) + R(U8)

            // RGBA
            RGBA_U5_5_5_1           = 150,    // 16-bit = R(U5) + G(U5) + B(U5) + A(U1)
            RGBA_S8                 = 151,    // 32-bit = R(S8) + G(S8) + B(S8) + A(S8)
            RGBA_U8                 = 152,    // 32-bit = R(U8) + G(U8) + B(U8) + A(U8)
            RGBA_S16                = 153,    // 64-bit = R(S16) + G(S16) + B(S16) + A(S16)
            RGBA_U16                = 154,    // 64-bit = R(U16) + G(U16) + B(U16) + A(U16)
            RGBA_F16                = 155,    // 64-bit = R(F16) + G(F16) + B(F16) + A(F16)
            RGBA_F32                = 156,    // 128-bit = R(F32) + G(F32) + B(F32) + A(F32)

            // BGRA
            BGRA_U8                 = 170,    // 32-bit = B(U8) + G(U8) + R(U8) + A(U8)


            // DXT
            DXT1_RGB                = 200,
            // DXT2_RGBA            = 201,
            DXT3_RGBA               = 202,
            // DXT4_RGBA            = 203,
            DXT5_RGBA               = 204,

            // PVRTC
            PVRTC1_RGBA4            = 250,
            PVRTC1_RGB4             = 251,
            PVRTC1_RGBA2            = 252,
            PVRTC1_RGB2             = 253,
            PVRTC2_RGBA2            = 254,
            PVRTC2_RGBA4            = 255,


            // ETC 
            ETC2_RGB                = 300,
            ETC2_RGBA               = 301,
            ETC2_RGB_A1             = 302,
            
            // ASTC
            ASTC_RGBA_4x4           = 350,
            ASTC_RGBA_5x4           = 351,
            ASTC_RGBA_5x5           = 352,
            ASTC_RGBA_6x5           = 353,
            ASTC_RGBA_6x6           = 354,
            ASTC_RGBA_8x5           = 355,
            ASTC_RGBA_8x6           = 356,
            ASTC_RGBA_8x8           = 357,
            ASTC_RGBA_10x5          = 358,
            ASTC_RGBA_10x6          = 359,
            ASTC_RGBA_10x8          = 360,
            ASTC_RGBA_10x10         = 361,
            ASTC_RGBA_12x10         = 362,
            ASTC_RGBA_12x12         = 363,
            ASTC_SRGBA_4x4          = 364,
            ASTC_SRGBA_5x4          = 365,
            ASTC_SRGBA_5x5          = 366,
            ASTC_SRGBA_6x5          = 367,
            ASTC_SRGBA_6x6          = 368,
            ASTC_SRGBA_8x5          = 369,
            ASTC_SRGBA_8x6          = 370,
            ASTC_SRGBA_8x8          = 371,
            ASTC_SRGBA_10x5         = 372,
            ASTC_SRGBA_10x6         = 373,
            ASTC_SRGBA_10x8         = 374,
            ASTC_SRGBA_10x10        = 375,
            ASTC_SRGBA_12x10        = 376,
            ASTC_SRGBA_12x12        = 377,


            // Depth and Stencil
            DEPTH_U16               = 403,
            DEPTH_U24               = 404,
            DEPTH_U32               = 405,
            DEPTH_F32               = 406,
            DEPTH_STENCIL_U24_8     = 450,
            DEPTH_STENCIL_F32_U8    = 451,
        };
        

        //////////////////////////////////////////
        MAZE_GRAPHICS_API String ToString(Enum _value);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API Enum FromString(String const& _value);
        
        //////////////////////////////////////////
        MAZE_GRAPHICS_API U32 GetBytesPerPixel(PixelFormat::Enum _pixelFormat);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API U32 GetChannelsPerPixel(PixelFormat::Enum _pixelFormat);
        
        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool IsCompressed(PixelFormat::Enum _pixelFormat);
        
        //////////////////////////////////////////
        MAZE_GRAPHICS_API Size CalculateRequiredBytes(U32 _width, U32 _height, U32 _depth, PixelFormat::Enum _pixelFormat);
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePixelFormat_hpp_
//////////////////////////////////////////
