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
#if (!defined(_MazePixelSheet2D_hpp_))
#define _MazePixelSheet2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PixelSheet2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API PixelSheet2D
    {
    public:

        //////////////////////////////////////////
        PixelSheet2D();

        //////////////////////////////////////////
        PixelSheet2D(
            Vec2DS const& _size,
            PixelFormat::Enum _pixelFormat = PixelFormat::RGBA_U8);

        //////////////////////////////////////////
        PixelSheet2D(
            Vec2DS const& _size,
            ColorU32 const& _color);

        //////////////////////////////////////////
        PixelSheet2D(PixelSheet2D const& _other);

        //////////////////////////////////////////
        PixelSheet2D(PixelSheet2D&& _other) noexcept;

        //////////////////////////////////////////
        void setSize(Vec2DS const& _size);

        //////////////////////////////////////////
        inline void setSize(S32 _columnsCount, S32 _rowsCount)
        {
            setSize(Vec2DS(_columnsCount, _rowsCount));
        }

        //////////////////////////////////////////
        inline Vec2DS const& getSize() const { return m_size; }

        //////////////////////////////////////////
        inline S32 const& getWidth() const { return m_size.x; }

        //////////////////////////////////////////
        inline S32 const& getHeight() const { return m_size.y; }

        //////////////////////////////////////////
        inline S32 getRowsCount() const { return m_size.y; }

        //////////////////////////////////////////
        inline S32 getColumnsCount() const { return m_size.x; }

        //////////////////////////////////////////
        inline Size getTotalBytesCount() const { return m_data.getSize(); }

        //////////////////////////////////////////
        void setFormat(PixelFormat::Enum _format);

        //////////////////////////////////////////
        inline PixelFormat::Enum getFormat() const { return m_format; }

        //////////////////////////////////////////
        void const* getPixel(S32 _x, S32 _y) const;

        //////////////////////////////////////////
        ColorU32 getPixelRGBA_U8(S32 _x, S32 _y) const;

        //////////////////////////////////////////
        void setPixel(S32 _x, S32 _y, U8 _pixelData);

        //////////////////////////////////////////
        void setPixel(S32 _x, S32 _y, U16 _pixelData);

        //////////////////////////////////////////
        void setPixel(S32 _x, S32 _y, U32 _pixelData);

        //////////////////////////////////////////
        void setPixel(S32 _x, S32 _y, void const* _pixelData);

        //////////////////////////////////////////
        void setPixelSafe(S32 _x, S32 _y, U32 _pixelData);

        //////////////////////////////////////////
        void fill(ColorU32 const& _color);

        //////////////////////////////////////////
        void fill(U8 _byte);

        //////////////////////////////////////////
        void fill(F32* _value, Size _channelsCount);

        //////////////////////////////////////////
        void flipY();

        //////////////////////////////////////////
        inline void swapBytes(Size _i, Size _j)
        {
            m_data.swapBytes(_i, _j);
        }

        //////////////////////////////////////////
        inline void setPixel(S32 _x, S32 _y, ColorU32 const& _color)
        {
            setPixel(_x, _y, _color.toRGBA_U8());
        }

        //////////////////////////////////////////
        inline void setPixel(Vec2DS const& _pos, ColorU32 const& _color)
        {
            setPixel(_pos.x, _pos.y, _color);
        }

        //////////////////////////////////////////
        void setPixelSafe(S32 _x, S32 _y, ColorU32 const& _color)
        {
            setPixelSafe(_x, _y, _color.toRGBA_U8());
        }

        //////////////////////////////////////////
        void copyFrom(U8 const* _data, Vec2DS const& _size, PixelFormat::Enum _pixelFormat);

        //////////////////////////////////////////
        void drawLine(
            S32 _x0, S32 _y0,
            S32 _x1, S32 _y1,
            ColorU32 const& _color);

        //////////////////////////////////////////
        inline void drawLine(
            Vec2DS const& _p0,
            Vec2DS const& _p1,
            ColorU32 const& _color)
        {
            drawLine(_p0.x, _p0.y, _p1.x, _p1.y, _color);
        }

        //////////////////////////////////////////
        void drawCircle(
            S32 _x, S32 _y,
            S32 _radius,
            ColorU32 const& _color);

        //////////////////////////////////////////
        inline void drawCircle(
            Vec2DS const& _pos,
            S32 _radius,
            ColorU32 const& _color)
        {
            drawCircle(_pos.x, _pos.y, _radius, _color);
        }

        //////////////////////////////////////////
        void drawFilledCircle(
            S32 _x, S32 _y,
            S32 _radius,
            ColorU32 const& _color);

        //////////////////////////////////////////
        inline void drawFilledCircle(
            Vec2DS const& _point,
            S32 _radius,
            ColorU32 const& _color)
        {
            drawFilledCircle(_point.x, _point.y, _radius, _color);
        }

        //////////////////////////////////////////
        void drawRect(
            S32 _x, S32 _y,
            S32 _w, S32 _h,
            ColorU32 const& _color);

        //////////////////////////////////////////
        void drawFilledRect(
            S32 _x, S32 _y,
            S32 _w, S32 _h,
            ColorU32 const& _color);

        //////////////////////////////////////////
        inline void drawFilledRect(
            Vec2DS _pos,
            Vec2DS _size,
            ColorU32 const& _color)
        {
            drawFilledRect(_pos.x, _pos.y, _size.x, _size.y, _color);
        }

        //////////////////////////////////////////
        void drawTriangle(
            S32 _x0, S32 _y0,
            S32 _x1, S32 _y1,
            S32 _x2, S32 _y2,
            ColorU32 const& _color);

        //////////////////////////////////////////
        void drawFilledTriangle(
            S32 _x0, S32 _y0,
            S32 _x1, S32 _y1,
            S32 _x2, S32 _y2,
            ColorU32 const& _color);

        //////////////////////////////////////////
        inline void drawFilledTriangle(
            Vec2DS const& _p0,
            Vec2DS const& _p1,
            Vec2DS const& _p2,
            ColorU32 const& _color)
        {
            drawFilledTriangle(_p0.x, _p0.y, _p1.x, _p1.y, _p2.x, _p2.y, _color);
        }

        //////////////////////////////////////////
        void drawText(
            S32 _x, S32 _y,
            CString _text,
            ColorU32 const& _color,
            U32 _scale = 1,
            S32 _horizontalAlignment = 0,
            S32 _verticalAlignment = 0);
        
        //////////////////////////////////////////
        U8* getDataPointer();

        //////////////////////////////////////////
        U8 const* getDataPointer() const;

        //////////////////////////////////////////
        inline Size getDataSize() { return m_data.getSize(); }

        //////////////////////////////////////////
        inline S32 getBytesPerPixel() const { return m_bytesPerPixel; }

        //////////////////////////////////////////
        inline S32 getBytesPerRow() const { return m_bytesPerRow; }

        //////////////////////////////////////////
        PixelSheet2D& operator=(PixelSheet2D const& _copy);

        //////////////////////////////////////////
        PixelSheet2D& operator==(PixelSheet2D&& _copy) noexcept;

        //////////////////////////////////////////
        PixelSheet2D upscaledCopy(S32 _scale);

        //////////////////////////////////////////
        void upscale(S32 _scale);

        //////////////////////////////////////////
        PixelSheet2D downscaledCopy(S32 _scale, bool _roundAlpha = false);

        //////////////////////////////////////////
        void downscale(S32 _scale);

    protected:

        //////////////////////////////////////////
        void updateDataSize();

    protected:
        Vec2DS m_size;
        ByteBuffer m_data;
        PixelFormat::Enum m_format;
        S32 m_bytesPerPixel;
        S32 m_bytesPerRow;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePixelSheet2D_hpp_
//////////////////////////////////////////
