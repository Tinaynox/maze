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
#if (!defined(_MazeColorU32_hpp_))
#define _MazeColorU32_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3D.hpp"
#include "maze-core/math/MazeVec4D.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ColorU32
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ColorU32 
    {
    public:
        static const ColorU32 c_zero;
        static const ColorU32 c_white;
        static const ColorU32 c_whiteSoft;
        static const ColorU32 c_gray;
        static const ColorU32 c_lightGray;
        static const ColorU32 c_darkGray;
        static const ColorU32 c_black;
        static const ColorU32 c_blackSoft;
        static const ColorU32 c_red;
        static const ColorU32 c_green;
        static const ColorU32 c_blue;
        static const ColorU32 c_cyan;
        static const ColorU32 c_magenta;
        static const ColorU32 c_yellow;
        static const ColorU32 c_transparent;

    public:

        //////////////////////////////////////////
        inline ColorU32(U8 _r = 255, U8 _g = 255, U8 _b = 255, U8 _a = 255)
            : r(_r)
            , g(_g)
            , b(_b)
            , a(_a)
        {
        }

        //////////////////////////////////////////
        inline ColorU32(ColorU32 const& _color, U8 _a)
            : r(_color.r)
            , g(_color.g)
            , b(_color.b)
            , a(_a) 
        {
        }

        //////////////////////////////////////////
        inline ColorU32(ColorU32 const& _color)
            : r(_color.r)
            , g(_color.g)
            , b(_color.b)
            , a(_color.a)
        {
        }

        //////////////////////////////////////////
        inline ColorU32(Vec3DF const& _colorF32)
            : r(U8(_colorF32.x * 255.0f))
            , g(U8(_colorF32.y * 255.0f))
            , b(U8(_colorF32.z * 255.0f))
            , a(255)
        {
        }

        //////////////////////////////////////////
        static ColorU32 Random();

        //////////////////////////////////////////
        static ColorU32 Lerp(ColorU32 const& _color0, ColorU32 const& _color1, float _scalar);

        //////////////////////////////////////////
        inline bool operator==(ColorU32 const& _color) const
        {
            return (r == _color.r
                &&   g == _color.g
                &&   b == _color.b
                &&   a == _color.a);
        }

        //////////////////////////////////////////
        inline bool operator!=(ColorU32 const& _color) const
        {
            return !operator==(_color);
        }

        //////////////////////////////////////////
        inline ColorU32 operator*(ColorU32 const& _color) const
        {
            ColorU32 newColor = *this;
            newColor.blend(_color);
            return newColor;
        }

        //////////////////////////////////////////
        inline U8& operator[](Size _idx)
        {
            return *((U8*)&r + _idx);
        }

        //////////////////////////////////////////
        inline U8 operator[](Size _idx) const
        {
            return *((U8*)&r + _idx);
        }

        //////////////////////////////////////////
        inline void lerp(ColorU32 const& _color, F32 _scalar)
        {
            r = Math::ClampLerp(r, _color.r, _scalar);
            g = Math::ClampLerp(g, _color.g, _scalar);
            b = Math::ClampLerp(b, _color.b, _scalar);
            a = Math::ClampLerp(a, _color.a, _scalar);
        }

        //////////////////////////////////////////
        inline void blend(ColorU32 const& _color)
        {
            r = U8((S32(r) * S32(_color.r)) / 255);
            g = U8((S32(g) * S32(_color.g)) / 255);
            b = U8((S32(b) * S32(_color.b)) / 255);
            a = U8((S32(a) * S32(_color.a)) / 255);
        }

        //////////////////////////////////////////
        inline ColorU32 blendedCopy(ColorU32 const& _color) const
        {
            ColorU32 copy;

            copy.r = U8((S32(r) * S32(_color.r)) / 255);
            copy.g = U8((S32(g) * S32(_color.g)) / 255);
            copy.b = U8((S32(b) * S32(_color.b)) / 255);
            copy.a = U8((S32(a) * S32(_color.a)) / 255);

            return copy;
        }

        //////////////////////////////////////////
        inline void setRGB(U8 _r, U8 _g, U8 _b)
        {
            this->r = _r;
            this->g = _g;
            this->b = _b;
        }

        //////////////////////////////////////////
        inline void setRGBA(U8 _r, U8 _g, U8 _b, U8 _a)
        {
            this->r = _r;
            this->g = _g;
            this->b = _b;
            this->a = _a;
        }

        //////////////////////////////////////////
        static ColorU32 FromString(String const& _colorString, S8 _separator = ';');

        //////////////////////////////////////////
        static ColorU32 FromStringHex(String const& _colorString);
    
        //////////////////////////////////////////
        String toStringHex() const;

        //////////////////////////////////////////
        void setString(String const& _colorString, S8 _separator = ';');

        //////////////////////////////////////////
        U32 toRGB_U8() const;

        //////////////////////////////////////////
        U32 toRGBA_U8() const;

        //////////////////////////////////////////
        void setRGBA_U8(U32 _value);

        //////////////////////////////////////////
        Vec3DF toVec3DF() const;

        //////////////////////////////////////////
        void setVec3DF(Vec3DF const& _vec);

        //////////////////////////////////////////
        inline static ColorU32 FromVec3DF(Vec3DF const& _vec)
        {
            ColorU32 color;
            color.setVec3DF(_vec);
            return color;
        }

        //////////////////////////////////////////
        Vec4DF toVec4DF() const;

        //////////////////////////////////////////
        void setVec4DF(Vec4DF const& _vec);

        //////////////////////////////////////////
        inline static ColorU32 FromVec4DF(Vec4DF const& _vec)
        {
            ColorU32 color;
            color.setVec4DF(_vec);
            return color;
        }

        //////////////////////////////////////////
        inline String toString(S8 _separator = ';') const
        {
            String result = StringHelper::S32ToString(r) + _separator + StringHelper::S32ToString(g) + _separator + StringHelper::S32ToString(b);
            if (a != 255)
            {
                result += _separator + StringHelper::S32ToString(a);
            }
            return result;
        }

        //////////////////////////////////////////
        inline String toStringTag(S8 _separator = ';') const
        {
            String result = "#{" + StringHelper::S32ToString(r) + _separator + StringHelper::S32ToString(g) + _separator + StringHelper::S32ToString(b);
            if (a != 255)
                result += _separator + StringHelper::S32ToString(a);
            result += "}";

            return result;
        }

    public:
        U8 r;
        U8 g;
        U8 b;
        U8 a;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(ColorU32 const& _value, String& _data)
    {
        _data = _value.toString(';');
    }

    //////////////////////////////////////////
    inline void ValueFromString(ColorU32& _value, CString _data, Size _count)
    {
        _value = ColorU32::FromString(String(_data, _count), ';');
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(ColorU32 const& _value)
    {
        return sizeof(ColorU32);
    }

    //////////////////////////////////////////
    inline void SerializeValue(ColorU32 const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(ColorU32));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(ColorU32& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(ColorU32));
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorU32_hpp_
//////////////////////////////////////////
