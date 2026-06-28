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
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"


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

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void DrawLine(
            PixelSheet2D& _pixelSheet,
            S32 _x0, S32 _y0,
            S32 _x1, S32 _y1,
            ColorU32 const& _color)
        {
            U32 color = _color.toRGBA_U8();

            S32 x;
            S32 y;
            S32 dx = _x1 - _x0;
            S32 dy = _y1 - _y0;
            S32 xe;
            S32 ye;
            S32 i;

            // Line is vertical
            if (dx == 0)
            {
                if (_y1 < _y0)
                    std::swap(_y0, _y1);

                for (y = _y0; y <= _y1; ++y)
                    _pixelSheet.setPixelSafe(_x0, y, color);

                return;
            }

            // Line is horizontal
            if (dy == 0)
            {
                if (_x1 < _x0)
                    std::swap(_x0, _x1);

                for (x = _x0; x <= _x1; ++x)
                    _pixelSheet.setPixelSafe(x, _y0, color);

                return;
            }

            // Line is Funk-aye
            S32 dx1 = abs(dx);
            S32 dy1 = abs(dy);

            S32 px = 2 * dy1 - dx1;
            S32 py = 2 * dx1 - dy1;

            if (dy1 <= dx1)
            {
                if (dx >= 0)
                {
                    x = _x0;
                    y = _y0;
                    xe = _x1;
                }
                else
                {
                    x = _x1;
                    y = _y1;
                    xe = _x0;
                }

                _pixelSheet.setPixelSafe(x, y, color);

                for (i = 0; x < xe; ++i)
                {
                    x = x + 1;
                    if (px < 0)
                        px = px + 2 * dy1;
                    else
                    {
                        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                            y = y + 1;
                        else
                            y = y - 1;

                        px = px + 2 * (dy1 - dx1);
                    }

                    _pixelSheet.setPixelSafe(x, y, color);
                }
            }
            else
            {
                if (dy >= 0)
                {
                    x = _x0;
                    y = _y0;
                    ye = _y1;
                }
                else
                {
                    x = _x1;
                    y = _y1;
                    ye = _y0;
                }

                _pixelSheet.setPixelSafe(x, y, color);

                for (i = 0; y < ye; ++i)
                {
                    y = y + 1;

                    if (py <= 0)
                        py = py + 2 * dx1;
                    else
                    {
                        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
                            x = x + 1;
                        else
                            x = x - 1;

                        py = py + 2 * (dx1 - dy1);
                    }

                    _pixelSheet.setPixelSafe(x, y, color);
                }
            }
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void DrawCircle(
            PixelSheet2D& _pixelSheet,
            S32 _x, S32 _y,
            S32 _radius,
            ColorU32 const& _color)
        {
            if (_radius <= 0)
                return;

            U32 color = _color.toRGBA_U8();

            S32 x0 = 0;
            S32 y0 = _radius;
            S32 d = 3 - 2 * _radius;

            while (y0 >= x0)
            {
                _pixelSheet.setPixelSafe(_x + x0, _y - y0, color);
                _pixelSheet.setPixelSafe(_x + y0, _y - x0, color);
                _pixelSheet.setPixelSafe(_x + y0, _y + x0, color);
                _pixelSheet.setPixelSafe(_x + x0, _y + y0, color);
                _pixelSheet.setPixelSafe(_x - x0, _y + y0, color);
                _pixelSheet.setPixelSafe(_x - y0, _y + x0, color);
                _pixelSheet.setPixelSafe(_x - y0, _y - x0, color);
                _pixelSheet.setPixelSafe(_x - x0, _y - y0, color);
                if (d < 0)
                    d += (4 * x0++ + 6);
                else
                    d += (4 * (x0++ - y0--) + 10);
            }
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void DrawFilledCircle(
            PixelSheet2D& _pixelSheet,
            S32 _x, S32 _y,
            S32 _radius,
            ColorU32 const& _color)
        {
            if (_radius <= 0)
                return;

            U32 color = _color.toRGBA_U8();

            S32 x0 = 0;
            S32 y0 = _radius;
            S32 d = 3 - 2 * _radius;

            auto _drawline =
                [&](S32 _sx, S32 _ex, S32 _ny)
                {
                    for (S32 i = _sx; i <= _ex; i++)
                        _pixelSheet.setPixelSafe(i, _ny, color);
                };

            while (y0 >= x0)
            {
                _drawline(_x - x0, _x + x0, _y - y0);
                _drawline(_x - y0, _x + y0, _y - x0);
                _drawline(_x - x0, _x + x0, _y + y0);
                _drawline(_x - y0, _x + y0, _y + x0);

                if (d < 0)
                    d += (4 * x0++ + 6);
                else
                    d += (4 * (x0++ - y0--) + 10);
            }
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void DrawFilledRect(
            PixelSheet2D& _pixelSheet,
            S32 _x, S32 _y,
            S32 _w, S32 _h,
            ColorU32 const& _color)
        {
            U32 color = _color.toRGBA_U8();

            Vec2S size = _pixelSheet.getSize();

            S32 x1 = _x + _w;
            S32 y1 = _y + _h;

            if (_x < 0)
                _x = 0;
            if (_x >= size.x)
                _x = size.x;
            if (_y < 0)
                _y = 0;
            if (_y >= size.y)
                _y = size.y;

            if (x1 < 0)
                x1 = 0;
            if (x1 >= size.x)
                x1 = size.x;
            if (y1 < 0)
                y1 = 0;
            if (y1 >= size.y)
                y1 = size.y;

            for (S32 y = _y; y < y1; ++y)
                for (S32 x = _x; x < x1; ++x)
                    _pixelSheet.setPixelSafe(x, y, color);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void DrawFilledTriangle(
            PixelSheet2D& _pixelSheet,
            S32 _x0, S32 _y0,
            S32 _x1, S32 _y1,
            S32 _x2, S32 _y2,
            ColorU32 const& _color)
        {
            // _y2 is top
            if (_y2 >= _y1 && _y2 >= _y0)
            {
            }
            else
            // _y1 is top
            if (_y1 >= _y0 && _y1 >= _y2)
            {
                // Swap 1 & 2
                std::swap(_x1, _x2);
                std::swap(_y1, _y2);
            }
            else
            // _y0 is top
            {
                // Swap 1 & 2
                std::swap(_x0, _x2);
                std::swap(_y0, _y2);
            }

            if (_y1 <= _y0)
            {
                // Swap 0 & 1
                std::swap(_x0, _x1);
                std::swap(_y0, _y1);
            }

            auto fillBottomFlatTriangle =
                [&](S32 _tx0, S32 _ty0,
                    S32 _tx1, S32 _ty1,
                    S32 _tx2, S32 _ty2)
                {
                    F32 invslope1 = F32(_tx1 - _tx0) / F32(_ty1 - _ty0);
                    F32 invslope2 = F32(_tx2 - _tx0) / F32(_ty2 - _ty0);

                    F32 curx1 = (F32)_tx0;
                    F32 curx2 = (F32)_tx0;

                    for (S32 scanlineY = _ty0; scanlineY > _ty2; --scanlineY)
                    {
                        DrawLine(_pixelSheet, (S32)round(curx1), scanlineY, (S32)round(curx2), scanlineY, _color);
                        curx1 -= invslope1;
                        curx2 -= invslope2;
                    }

                };

            auto fillTopFlatTriangle =
                [&](S32 _tx0, S32 _ty0,
                    S32 _tx1, S32 _ty1,
                    S32 _tx2, S32 _ty2)
            {
                F32 invslope1 = F32(_tx1 - _tx0) / F32(_ty1 - _ty0);
                F32 invslope2 = F32(_tx2 - _tx0) / F32(_ty2 - _ty0);

                F32 curx1 = (F32)_tx0;
                F32 curx2 = (F32)_tx0;

                for (S32 scanlineY = _ty0; scanlineY <= _ty1; ++scanlineY)
                {
                    DrawLine(_pixelSheet, (S32)round(curx1), scanlineY, (S32)round(curx2), scanlineY, _color);
                    curx1 += invslope1;
                    curx2 += invslope2;
                }
            };

            if (_y1 == _y2)
            {
                fillTopFlatTriangle(
                    _x0, _y0,
                    _x1, _y1,
                    _x2, _y2);
            }
            else
            if (_y0 == _y1)
            {
                fillBottomFlatTriangle(
                    _x2, _y2,
                    _x0, _y0,
                    _x1, _y1);
            }
            else
            {
                S32 x3 = (S32)(_x0 + ((F32)(_y1 - _y0) / (F32)(_y2 - _y0)) * (_x2 - _x0));
                S32 y3 = _y1;

                fillTopFlatTriangle(
                    _x0, _y0,
                    _x1, _y1,
                    x3, y3);

                fillBottomFlatTriangle(
                    _x2, _y2,
                    x3, y3,
                    _x1, _y1);

            }
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void DrawText(
            PixelSheet2D& _pixelSheet,
            S32 _x, S32 _y,
            CString _text,
            ColorU32 const& _color,
            U32 _scale,
            S32 _horizontalAlignment,
            S32 _verticalAlignment)
        {
            PixelSheet2D const* asciiSymbols = GraphicsUtilsHelper::GetAsciiSymbolsSheet8x8();

            S32 rowSize = 8 * (S32)_scale;
            S32 columnSize = 8 * (S32)_scale;

            S32 rowsCount = 1;
            S32 columnsCount = 0;

            S32 columnsCounter = 0;

            Char const* p = _text;
            while (*p != 0)
            {
                if (*p == '\n')
                {
                    ++rowsCount;

                    columnsCount = Math::Max(columnsCount, columnsCounter);
                    columnsCounter = 0;
                }
                else
                {
                    ++columnsCounter;
                }

                ++p;
            }
            columnsCount = Math::Max(columnsCount, columnsCounter);


            S32 sx = 0;
            S32 sy = -rowSize;

            S32 xShift = 0;
            switch (_horizontalAlignment)
            {
            case 0:
                break;
            case 1:
                xShift = -(columnSize * columnsCount) >> 1;
                break;
            case 2:
                xShift = -columnSize * columnsCount;
                break;
            }
            sx = xShift;

            switch (_verticalAlignment)
            {
                case 0:
                    break;
                case 1:
                    sy += (rowSize * rowsCount) >> 1;
                    break;
                case 2:
                    sy += rowSize * rowsCount;
                    break;
            }

            p = _text;
            while (*p != 0)
            {
                Char c = *p;

                if (c == '\n')
                {
                    sx = xShift;
                    sy -= rowSize;
                }
                else
                {
                    S32 ox = (c - 32) % 16;
                    S32 oy = 5 - ((c - 32) / 16);

                    if (_scale > 1)
                    {
                        for (U32 i = 0; i < 8; i++)
                            for (U32 j = 0; j < 8; j++)
                                if (asciiSymbols->getPixelRGBA_U8(i + ox * 8, j + oy * 8).r > 0)
                                    for (U32 is = 0; is < _scale; is++)
                                        for (U32 js = 0; js < _scale; js++)
                                            _pixelSheet.setPixelSafe(_x + sx + (i * _scale) + is, _y + sy + (j * _scale) + js, _color);
                    }
                    else
                    {
                        for (U32 i = 0; i < 8; i++)
                            for (U32 j = 0; j < 8; j++)
                                if (asciiSymbols->getPixelRGBA_U8(i + ox * 8, j + oy * 8).r > 0)
                                    _pixelSheet.setPixelSafe(_x + sx + i, _y + sy + j, _color);
                    }
                    sx += 8 * _scale;
                }

                ++p;
            }
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D UpscaledCopy(
            PixelSheet2D const& _pixelSheet,
            S32 _scale)
        {
            if (_scale <= 1)
                return PixelSheet2D(_pixelSheet);

            Vec2S size = _pixelSheet.getSize();

            PixelSheet2D newSheet(size * _scale, _pixelSheet.getFormat());

            for (S32 _r = 0; _r < size.y; ++_r)
            {
                for (S32 _y = 0; _y < _scale; ++_y)
                {
                    for (S32 _c = 0; _c < size.x; ++_c)
                    {
                        for (S32 _x = 0; _x < _scale; ++_x)
                            newSheet.setPixel(_c * _scale + _x, _r * _scale + _y, _pixelSheet.getPixel(_c, _r));
                    }
                }
            }

            return newSheet;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D DownscaledCopy(
            PixelSheet2D const& _pixelSheet,
            S32 _scale,
            bool _roundAlpha)
        {
            if (_scale <= 1)
                return PixelSheet2D(_pixelSheet);

            MAZE_ERROR_RETURN_VALUE_IF(_pixelSheet.getFormat() != PixelFormat::RGBA_U8, PixelSheet2D(_pixelSheet), "Unsupported pixel format!");

            PixelSheet2D newSheet(_pixelSheet.getSize() / _scale, _pixelSheet.getFormat());

            Vec2S newSize = newSheet.getSize();

            for (S32 _r = 0; _r < newSize.y; ++_r)
            {
                for (S32 _c = 0; _c < newSize.x; ++_c)
                {
                    U32 r = 0;
                    U32 g = 0;
                    U32 b = 0;
                    U32 a = 0;
                    U32 count = _scale * _scale;
                    for (S32 _y = 0; _y < _scale; ++_y)
                        for (S32 _x = 0; _x < _scale; ++_x)
                        {
                            ColorU32 srcPixel = _pixelSheet.getPixelRGBA_U8(_c * _scale + _x, _r * _scale + _y);
                            r += srcPixel.r;
                            g += srcPixel.g;
                            b += srcPixel.b;
                            a += srcPixel.a;
                        }
                    ColorU32 newPixel(r / count, g / count, b / count, a / count);
                    if (_roundAlpha)
                        newPixel.a = newPixel.a >= 128 ? 255 : 0;
                    newSheet.setPixel(_c, _r, newPixel);
                }
            }

            return newSheet;
        }

    } // namespace PixelSheet2DHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
