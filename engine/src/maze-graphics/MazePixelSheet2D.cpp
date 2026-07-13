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
#include "maze-graphics/helpers/MazePixelSheet2DHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_INDEXED_RESOURCE(PixelSheet2D);

    //////////////////////////////////////////
    // Class PixelSheet2D
    //
    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D()
    {
    }

    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D(
        Vec2S const& _size,
        PixelFormat::Enum _pixelFormat)
        : PixelSheet2D()
    {
        setSize(_size);
        setFormat(_pixelFormat);
    }

    //////////////////////////////////////////
    PixelSheet2D::PixelSheet2D(
        Vec2S const& _size,
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
    PixelSheet2D::~PixelSheet2D()
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
    void PixelSheet2D::setSize(Vec2S const& _size)
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
        if (PixelFormat::IsCompressed(m_format))
        {
            S32 dataSize = (S32)PixelFormat::CalculateRequiredBytes(m_size.x, m_size.y, 1, m_format);
            m_data.resize(dataSize);
        }
        else
        {
            S32 pixelsCount = getRowsCount() * getColumnsCount();
            S32 dataSize = pixelsCount * m_bytesPerPixel;

            m_data.resize(dataSize);
        }
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
    void const* PixelSheet2D::getPixel(S32 _x, S32 _y) const
    {
        Size offset = (Size)_y * (Size)m_bytesPerRow + (Size)_x * (Size)m_bytesPerPixel;
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(offset >= m_data.getSize(), nullptr, "Offset is out of bounds!");
        void const* pixelData = &m_data[offset];

        return pixelData;
    }

    //////////////////////////////////////////
    ColorU32 PixelSheet2D::getPixelRGBA_U8(S32 _x, S32 _y) const
    {
        ColorU32 color;
        void const* pixelData = getPixel(_x, _y);
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
    void PixelSheet2D::setPixel(S32 _x, S32 _y, void const* _pixelData)
    {
        Size offset = (Size)_y * (Size)m_bytesPerRow + (Size)_x * (Size)m_bytesPerPixel;
        MAZE_DEBUG_ERROR_RETURN_IF(offset >= m_data.getSize(), "Offset is out of bounds!");
        void* pixelData = &m_data[offset];
        memcpy(pixelData, _pixelData, m_bytesPerPixel);
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

        S32 pixelCount = m_size.x * m_size.y;
        S32 bpp = m_bytesPerPixel;

        U32* __restrict pixels = reinterpret_cast<U32*>(m_data.getDataRW());
        for (Size i = 0; i < pixelCount; ++i)
            pixels[i] = color;

        //for (S32 _r = 0; _r < m_size.y; ++_r)
        //{
        //    Size offset = (Size)_r * (Size)m_bytesPerRow;

        //    for (S32 _c = 0; _c < m_size.x; ++_c)
        //    {
        //        void* pixelData = &m_data[offset + (Size)_c * (Size)m_bytesPerPixel];

        //        for (S32 b = 0; b < m_bytesPerPixel; ++b)
        //        {
        //            *((U8*)pixelData + b) = (color >> b * 8) & 0xFF;
        //        }
        //    }
        //}
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

                eastl::swap(*((U8*)pixelData0), *((U8*)pixelData1));
            }
        }
    }

    //////////////////////////////////////////
    void PixelSheet2D::copyFrom(U8 const* _data, Vec2S const& _size, PixelFormat::Enum _pixelFormat)
    {
        setFormat(_pixelFormat);
        setSize(_size);
        m_data.copyFrom(_data, getDataSize());
    }

    //////////////////////////////////////////
    void PixelSheet2D::drawLine(
        S32 _x0, S32 _y0,
        S32 _x1, S32 _y1,
        ColorU32 const& _color)
    {
        PixelSheet2DHelper::DrawLine(*this, _x0, _y0, _x1, _y1, _color);
    }

    //////////////////////////////////////////
    void PixelSheet2D::drawCircle(
        S32 _x, S32 _y,
        S32 _radius,
        ColorU32 const& _color)
    {
        PixelSheet2DHelper::DrawCircle(*this, _x, _y, _radius, _color);
    }


    //////////////////////////////////////////
    void PixelSheet2D::drawFilledCircle(
        S32 _x, S32 _y,
        S32 _radius,
        ColorU32 const& _color)
    {
        PixelSheet2DHelper::DrawFilledCircle(*this, _x, _y, _radius, _color);
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
        PixelSheet2DHelper::DrawFilledRect(*this, _x, _y, _w, _h, _color);
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
        PixelSheet2DHelper::DrawFilledTriangle(*this, _x0, _y0, _x1, _y1, _x2, _y2, _color);
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
        PixelSheet2DHelper::DrawText(*this, _x, _y, _text, _color, _scale, _horizontalAlignment, _verticalAlignment);
    }

    //////////////////////////////////////////
    U8* PixelSheet2D::getDataRW()
    {
        return m_data.getDataRW();
    }

    //////////////////////////////////////////
    U8 const* PixelSheet2D::getDataRO() const
    {
        return m_data.getDataRO();
    }

    //////////////////////////////////////////
    PixelSheet2D PixelSheet2D::upscaledCopy(S32 _scale)
    {
        return PixelSheet2DHelper::UpscaledCopy(*this, _scale);
    }

    //////////////////////////////////////////
    void PixelSheet2D::upscale(S32 _scale)
    {
        *this = upscaledCopy(_scale);
    }

    //////////////////////////////////////////
    PixelSheet2D PixelSheet2D::downscaledCopy(S32 _scale, bool _roundAlpha)
    {
        return PixelSheet2DHelper::DownscaledCopy(*this, _scale, _roundAlpha);
    }

    //////////////////////////////////////////
    void PixelSheet2D::downscale(S32 _scale)
    {
        *this = downscaledCopy(_scale);
    }

    //////////////////////////////////////////
    void PixelSheet2D::clear()
    {
        m_size = Vec2S::c_zero;
        m_data.clear();
        m_format = PixelFormat::None;
        m_bytesPerPixel = 0;
        m_bytesPerRow = 0;
    }

} // namespace Maze
//////////////////////////////////////////
