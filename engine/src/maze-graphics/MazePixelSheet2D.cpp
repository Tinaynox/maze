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
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PixelSheet2D
    //
    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D()
        : m_size(Vec2DU::c_zero)
        , m_format(PixelFormat::None)
        , m_bytesPerPixel(0)
        , m_bytesPerRow(0)
    {
    }

    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D(
        Vec2DS const& _size,
        PixelFormat::Enum _pixelFormat)
        : PixelSheet2D()
    {
        setSize(_size);
        setFormat(_pixelFormat);
    }

    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D(
        Vec2DS const& _size,
        ColorU32 const& _color)
        : PixelSheet2D(_size, PixelFormat::RGBA_U8)
    {
        fill(_color);
    }

    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D(PixelSheet2D const& _other)
        : m_size(_other.m_size)
        , m_data(_other.m_data)
        , m_format(_other.m_format)
        , m_bytesPerPixel(_other.m_bytesPerPixel)
        , m_bytesPerRow(_other.m_bytesPerRow)
    {
    }

    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D(PixelSheet2D&& _other) noexcept
        : m_size(std::move(_other.m_size))
        , m_data(std::move(_other.m_data))
        , m_format(std::move(_other.m_format))
        , m_bytesPerPixel(std::move(_other.m_bytesPerPixel))
        , m_bytesPerRow(std::move(_other.m_bytesPerRow))
    {
    }

    //////////////////////////////////////////
    PixelSheet2D& PixelSheet2D::operator=(PixelSheet2D const& _copy)
    {
        m_size = _copy.m_size;
        m_data = _copy.m_data;
        m_format = _copy.m_format;
        m_bytesPerPixel = _copy.m_bytesPerPixel;
        m_bytesPerRow = _copy.m_bytesPerRow;
        
        return *this;
    }

    //////////////////////////////////////////
    PixelSheet2D& PixelSheet2D::operator==(PixelSheet2D&& _copy) noexcept
    {
        m_size = std::move(_copy.m_size);
        m_data = std::move(_copy.m_data);
        m_format = std::move(_copy.m_format);
        m_bytesPerPixel = std::move(_copy.m_bytesPerPixel);
        m_bytesPerRow = std::move(_copy.m_bytesPerRow);

        return *this;
    }

    //////////////////////////////////////////
    void PixelSheet2D::setSize(Vec2DS const& _size)
    {
        MAZE_DEBUG_ERROR_RETURN_IF(_size.x < 0 || _size.y < 0, "Invalid pixel sheet size!");

        if (m_size == _size)
            return;

        m_size = _size;

        m_bytesPerRow = getColumnsCount() * m_bytesPerPixel;

        updateDataSize();
    }

    //////////////////////////////////////////
    void PixelSheet2D::updateDataSize()
    {
        S32 pixelsCount = getRowsCount() * getColumnsCount();
        S32 dataSize = pixelsCount * m_bytesPerPixel;

        m_data.resize(dataSize);
    }

    //////////////////////////////////////////
    void PixelSheet2D::setFormat(PixelFormat::Enum _format)
    {
        if (m_format == _format)
            return;

        m_format = _format;

        m_bytesPerPixel = (S32)PixelFormat::GetBytesPerPixel(m_format);
        m_bytesPerRow = getColumnsCount() * m_bytesPerPixel;

        updateDataSize();
    }

    //////////////////////////////////////////
    ColorU32 PixelSheet2D::getPixelRGBA_U8(S32 _x, S32 _y) const
    {
        ColorU32 color;

        Size offset = (Size)_y * (Size)m_bytesPerRow + (Size)_x * (Size)m_bytesPerPixel;
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(offset >= m_data.getSize(), ColorU32::c_zero, "Offset is out of bounds!");
        void const* pixelData = &m_data[offset];
        color.setRGBA_U8(*((U32*)pixelData));
        return color;
    }

    //////////////////////////////////////////
    void PixelSheet2D::setPixel(S32 _x, S32 _y, U8 _pixelData)
    {
        Size offset = (Size)_y * (Size)m_bytesPerRow + (Size)_x * (Size)m_bytesPerPixel;
        MAZE_DEBUG_ERROR_RETURN_IF(offset >= m_data.getSize(), "Offset is out of bounds!");
        void* pixelData = &m_data[offset];
        *((U8*)pixelData) = _pixelData;
    }

    //////////////////////////////////////////
    void PixelSheet2D::setPixel(S32 _x, S32 _y, U16 _pixelData)
    {
        Size offset = (Size)_y * (Size)m_bytesPerRow + (Size)_x * (Size)m_bytesPerPixel;
        MAZE_DEBUG_ERROR_RETURN_IF(offset >= m_data.getSize(), "Offset is out of bounds!");
        void* pixelData = &m_data[offset];
        *((U16*)pixelData) = _pixelData;
    }

    //////////////////////////////////////////
    void PixelSheet2D::setPixel(S32 _x, S32 _y, U32 _pixelData)
    {
        Size offset = (Size)_y * (Size)m_bytesPerRow + (Size)_x * (Size)m_bytesPerPixel;
        MAZE_DEBUG_ERROR_RETURN_IF(offset >= m_data.getSize(), "Offset is out of bounds!");
        void* pixelData = &m_data[offset];
        *((U32*)pixelData) = _pixelData;
    }

    //////////////////////////////////////////
    void PixelSheet2D::setPixelSafe(S32 _x, S32 _y, U32 _pixelData)
    {
        if (_x < 0 || _x >= m_size.x || _y < 0 || _y >= m_size.y)
            return;

        Size offset = (Size)_y * (Size)m_bytesPerRow + (Size)_x * (Size)m_bytesPerPixel;
        void* pixelData = &m_data[offset];
        *((U32*)pixelData) = _pixelData;
    }

    //////////////////////////////////////////
    void PixelSheet2D::fill(ColorU32 const& _color)
    {
        MAZE_DEBUG_ERROR_RETURN_IF(m_bytesPerPixel != sizeof(U32), "Unsupported format!");

        U32 color = _color.toRGBA_U8();

        for (S32 _r = 0; _r < m_size.y; ++_r)
        {
            Size offset = (Size)_r * (Size)m_bytesPerRow;

            for (S32 _c = 0; _c < m_size.x; ++_c)
            {
                void* pixelData = &m_data[offset + (Size)_c * (Size)m_bytesPerPixel];

                for (S32 b = 0; b < m_bytesPerPixel; ++b)
                {
                    *((U8*)pixelData + b) = (color >> b * 8) & 0xFF;
                }
            }
        }
    }

    //////////////////////////////////////////
    void PixelSheet2D::fill(U8 _byte)
    {
        m_data.fill(_byte);
    }

    //////////////////////////////////////////
    void PixelSheet2D::fill(F32* _value, Size _channelsCount)
    {
        MAZE_DEBUG_ERROR_RETURN_IF(m_bytesPerPixel != sizeof(F32) * _channelsCount, "Unsupported format!");

        for (S32 _r = 0; _r < m_size.y; ++_r)
        {
            Size offset = (Size)_r * (Size)m_bytesPerRow;

            for (S32 _c = 0; _c < m_size.x; ++_c)
            {
                void* pixelData = &m_data[offset + (Size)_c * (Size)m_bytesPerPixel];

                for (Size b = 0; b < _channelsCount; ++b)
                {
                    *((F32*)pixelData + b) = _value[b];
                }
            }
        }
    }

    //////////////////////////////////////////
    void PixelSheet2D::flipY()
    {
        for (S32 r = 0; r < m_size.y / 2; ++r)
        {
            Size offset0 = (Size)r * (Size)m_bytesPerRow;
            Size offset1 = (Size)((S32)m_size.y - r - 1) * (Size)m_bytesPerRow;

            for (S32 x = 0; x < m_bytesPerRow; ++x)
            {
                void* pixelData0 = &m_data[offset0 + x];
                void* pixelData1 = &m_data[offset1 + x];

                std::swap(*((U8*)pixelData0), *((U8*)pixelData1));
            }
        }
    }

    //////////////////////////////////////////
    void PixelSheet2D::drawLine(
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
                setPixelSafe(_x0, y, color);

            return;
        }

        // Line is horizontal
        if (dy == 0)
        {
            if (_x1 < _x0)
                std::swap(_x0, _x1);

            for (x = _x0; x <= _x1; ++x)
                setPixelSafe(x, _y0, color);

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

            setPixelSafe(x, y, color);

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

                setPixelSafe(x, y, color);
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

            setPixelSafe(x, y, color);

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

                setPixelSafe(x, y, color);
            }
        }
    }

    //////////////////////////////////////////
    void PixelSheet2D::drawCircle(
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
            setPixelSafe(_x + x0, _y - y0, color);
            setPixelSafe(_x + y0, _y - x0, color);
            setPixelSafe(_x + y0, _y + x0, color);
            setPixelSafe(_x + x0, _y + y0, color);
            setPixelSafe(_x - x0, _y + y0, color);
            setPixelSafe(_x - y0, _y + x0, color);
            setPixelSafe(_x - y0, _y - x0, color);
            setPixelSafe(_x - x0, _y - y0, color);
            if (d < 0)
                d += (4 * x0++ + 6);
            else 
                d += (4 * (x0++ - y0--) + 10);
        }
    }


    //////////////////////////////////////////
    void PixelSheet2D::drawFilledCircle(
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
                    setPixelSafe(i, _ny, color);
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
    void PixelSheet2D::drawRect(
        S32 _x, S32 _y,
        S32 _w, S32 _h,
        ColorU32 const& _color)
    {
        drawLine(_x, _y, _x + _w, _y, _color);
        drawLine(_x + _w, _y, _x + _w, _y + _h, _color);
        drawLine(_x + _w, _y + _h, _x, _y + _h, _color);
        drawLine(_x, _y + _h, _x, _y, _color);
    }

    //////////////////////////////////////////
    void PixelSheet2D::drawFilledRect(
        S32 _x, S32 _y,
        S32 _w, S32 _h,
        ColorU32 const& _color)
    {
        U32 color = _color.toRGBA_U8();

        S32 x1 = _x + _w;
        S32 y1 = _y + _h;

        if (_x < 0)
            _x = 0;
        if (_x >= m_size.x)
            _x = m_size.x;
        if (_y < 0)
            _y = 0;
        if (_y >= m_size.y)
            _y = m_size.y;

        if (x1 < 0)
            x1 = 0;
        if (x1 >= m_size.x)
            x1 = m_size.x;
        if (y1 < 0)
            y1 = 0;
        if (y1 >= m_size.y)
            y1 = m_size.y;

        for (S32 y = _y; y < y1; ++y)
            for (S32 x = _x; x < x1; ++x)
                setPixelSafe(x, y, color);
    }

    //////////////////////////////////////////
    void PixelSheet2D::drawTriangle(
        S32 _x0, S32 _y0,
        S32 _x1, S32 _y1,
        S32 _x2, S32 _y2,
        ColorU32 const& _color)
    {
        drawLine(_x0, _y0, _x1, _y1, _color);
        drawLine(_x1, _y1, _x2, _y2, _color);
        drawLine(_x2, _y2, _x0, _y0, _color);
    }

    //////////////////////////////////////////
    void PixelSheet2D::drawFilledTriangle(
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
                    drawLine((S32)round(curx1), scanlineY, (S32)round(curx2), scanlineY, _color);
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
                drawLine((S32)round(curx1), scanlineY, (S32)round(curx2), scanlineY, _color);
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
    void PixelSheet2D::drawText(
        S32 _x, S32 _y,
        CString _text,
        ColorU32 const& _color,
        U32 _scale,
        S32 _horizontalAlignment,
        S32 _verticalAlignment)
    {
        PixelSheet2D const& systemFontSHeet = GraphicsUtilsHelper::GetSystemFontSheet();

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
                            if (systemFontSHeet.getPixelRGBA_U8(i + ox * 8, j + oy * 8).r > 0)
                                for (U32 is = 0; is < _scale; is++)
                                    for (U32 js = 0; js < _scale; js++)
                                        setPixelSafe(_x + sx + (i * _scale) + is, _y + sy + (j * _scale) + js, _color);
                }
                else
                {
                    for (U32 i = 0; i < 8; i++)
                        for (U32 j = 0; j < 8; j++)
                            if (systemFontSHeet.getPixelRGBA_U8(i + ox * 8, j + oy * 8).r > 0)
                                setPixelSafe(_x + sx + i,_y + sy + j, _color);
                }
                sx += 8 * _scale;
            }

            ++p;
        }
    }

    //////////////////////////////////////////
    U8* PixelSheet2D::getDataPointer()
    {
        return m_data.getDataPointer();
    }

    //////////////////////////////////////////
    U8 const* PixelSheet2D::getDataPointer() const
    {
        return m_data.getDataPointer();
    }

} // namespace Maze
//////////////////////////////////////////
