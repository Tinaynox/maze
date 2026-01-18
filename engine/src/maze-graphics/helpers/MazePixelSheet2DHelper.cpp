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
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace PixelSheet2DHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool ConvertRGBToBGR(PixelSheet2D& _pixelSheet)
        {
            S32 bpp = _pixelSheet.getBytesPerPixel();
            MAZE_ERROR_RETURN_VALUE_IF(bpp < 3, false, "Invalid PixelSheet2D!");

            S32 imageSize = (S32)_pixelSheet.getDataSize();
            for (S32 i = 0; i < imageSize; i += bpp)
                _pixelSheet.swapBytes(i, i + 2);

            return true;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool ConvertA1RGB5ToRGB5A1(PixelSheet2D& _pixelSheet)
        {
            S32 bpp = _pixelSheet.getBytesPerPixel();
            MAZE_ERROR_RETURN_VALUE_IF(bpp != 2, false, "Invalid PixelSheet2D!");

            U16* it = (U16*)_pixelSheet.getDataRW();
            U16* end = it + _pixelSheet.getRowsCount() * _pixelSheet.getColumnsCount();
            for (; it != end; ++it)
            {
                U16 red = (*it & 0x7C00) >> 10;
                U16 green = (*it & 0x03E0) >> 5;
                U16 blue = *it & 0x001F;
                U16 alpha = 1;
                *it = (red << 11) | (green << 6) | (blue << 1) | (alpha);
            }

            return true;
        }

        //////////////////////////////////////////
        inline U8 ConvertPixelChannelDataToU8(
            const U8* _channelPointer, PixelFormat::Enum _pixelFormat)
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
                    return static_cast<U8>(sourcePixel * 255.0f);
                }

                case PixelFormat::R_F32:
                case PixelFormat::RG_F32:
                case PixelFormat::RGB_F32:
                case PixelFormat::RGBA_F32:
                {
                    F32 sourcePixel = *(F32*)_channelPointer;
                    return static_cast<U8>(sourcePixel * 255.0f);
                }
                default:
                {
                    MAZE_ERROR("Unsupported PixelFormat!");
                }
            }

            return 0;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool SaveTGA(
            PixelSheet2D const& _pixelSheet,
            CString _fileName,
            bool _resetAlpha)
        {
            PixelFormat::Enum pixelFormat = _pixelSheet.getFormat();
            U32 bytesPerPixel = (U32)_pixelSheet.getBytesPerPixel();
            if (bytesPerPixel == 0u)
                return false;

            U32 channelsPerPixel = PixelFormat::GetChannelsPerPixel(pixelFormat);
            U32 bytesPerChannel = bytesPerPixel / channelsPerPixel;

            U32 bytesPerRow = _pixelSheet.getBytesPerRow();

            S32 weight = _pixelSheet.getWidth();
            S32 height = _pixelSheet.getHeight();
            if (weight == 0 || height == 0)
                return false;

            Debug::Log("Saving tga file - %s...", _fileName);
            
            ByteBuffer tgaData(weight * height * 4);
            U8* tgaDataPointer = tgaData.getDataRW();

            for (Size r = 0; r < height; ++r)
            {
                for (Size c = 0; c < weight; ++c)
                {
                    U32 offset = bytesPerRow * (U32)r + bytesPerPixel * (U32)c;
                    const U8* pixel = _pixelSheet.getDataRO() + offset;

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

            U32 xa = weight % 256;
            U32 xb = (weight - xa) / 256;
            U32 ya = height % 256;
            U32 yb = (height - ya) / 256;

            U8 header[18] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, (U8)xa, (U8)xb, (U8)ya, (U8)yb, 32, 0 };

            std::fstream file(_fileName, std::ios::out | std::ios::binary);
            file.write(reinterpret_cast<CString>(header), sizeof(S8) * 18);
            file.write(reinterpret_cast<CString>(tgaData.getDataRO()), sizeof(S8) * tgaData.getSize());
            file.close();

            Debug::Log("%s saved.", _fileName);

            return true;
        }

    } // namespace PixelSheet2DHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
