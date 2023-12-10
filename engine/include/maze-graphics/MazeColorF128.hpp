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
#if (!defined(_MazeColorF128_hpp_))
#define _MazeColorF128_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-core/math/MazeRandom.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-graphics/MazeColorU32.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ColorF128
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API ColorF128 
    {
    public:
        static const ColorF128 c_zero;
        static const ColorF128 c_white;
        static const ColorF128 c_whiteSoft;
        static const ColorF128 c_gray;
        static const ColorF128 c_lightGray;
        static const ColorF128 c_darkGray;
        static const ColorF128 c_black;
        static const ColorF128 c_blackSoft;
        static const ColorF128 c_red;
        static const ColorF128 c_green;
        static const ColorF128 c_blue;
        static const ColorF128 c_cyan;
        static const ColorF128 c_magenta;
        static const ColorF128 c_yellow;
        static const ColorF128 c_transparent;

    public:

        //////////////////////////////////////////
        inline ColorF128(
            F32 _r = 1.0f,
            F32 _g = 1.0f,
            F32 _b = 1.0f,
            F32 _a = 1.0f)
            : r(_r)
            , g(_g)
            , b(_b)
            , a(_a)
        {
        }

        //////////////////////////////////////////
        inline ColorF128(
            ColorF128 const& _color,
            F32 _a)
            : r(_color.r)
            , g(_color.g)
            , b(_color.b)
            , a(_a) 
        {
        }

        //////////////////////////////////////////
        inline ColorF128(ColorF128 const& _color)
            : value(_color.value)
        {
        }

        //////////////////////////////////////////
        inline ColorF128(Vec3F const& _colorF32)
            : r(_colorF32.x)
            , g(_colorF32.y)
            , b(_colorF32.z)
            , a(1.0f)
        {
        }

        //////////////////////////////////////////
        inline ColorF128(
            ColorU32 const& _color)
            : r(_color.r / 255.0f)
            , g(_color.g / 255.0f)
            , b(_color.b / 255.0f)
            , a(_color.a / 255.0f)
        {
        }

        //////////////////////////////////////////
        inline ColorF128(Vec4F const& _value)
            : value(_value)
        {
        }

        //////////////////////////////////////////
        static ColorF128 Random();

        //////////////////////////////////////////
        static ColorF128 Lerp(ColorF128 const& _color0, ColorF128 const& _color1, F32 _scalar);

        //////////////////////////////////////////
        inline bool operator==(ColorF128 const& _color) const
        {
            return ( r == _color.r
                &&   g == _color.g
                &&   b == _color.b
                &&   a == _color.a);
        }

        //////////////////////////////////////////
        inline bool operator!=(ColorF128 const& _color) const
        {
            return !operator==(_color);
        }

        //////////////////////////////////////////
        inline ColorF128& operator=(ColorF128 const& _vec)
        {
            value = _vec.value;
            return *this;
        }

        //////////////////////////////////////////
        inline ColorF128& operator=(ColorF128&& _vec) noexcept
        {
            value = std::move(_vec.value);
            return *this;
        }

        //////////////////////////////////////////
        inline ColorF128 operator+(ColorF128 const& _color) const
        {
            return ColorF128(value + _color.value);
        }

        //////////////////////////////////////////
        inline ColorF128 operator-(ColorF128 const& _color) const
        {
            return ColorF128(value - _color.value);
        }

        //////////////////////////////////////////
        inline ColorF128 operator*(ColorF128 const& _color) const
        {
            ColorF128 newColor = *this;
            newColor.blend(_color);
            return newColor;
        }

        //////////////////////////////////////////
        inline ColorF128 operator*(F32 _scalar) const
        {
            ColorF128 newColor = *this;
            newColor.value *= _scalar;
            return newColor;
        }

        //////////////////////////////////////////
        inline void lerp(ColorF128 const& _color, F32 _scalar)
        {
            r = Math::ClampLerp(r, _color.r, _scalar);
            g = Math::ClampLerp(g, _color.g, _scalar);
            b = Math::ClampLerp(b, _color.b, _scalar);
            a = Math::ClampLerp(a, _color.a, _scalar);
        }

        //////////////////////////////////////////
        inline void blend(ColorF128 const& _color)
        {
            r *=_color.r;
            g *=_color.g;
            b *=_color.b;
            a *=_color.a;
        }

        //////////////////////////////////////////
        inline ColorF128 blendedCopy(ColorF128 const& _color) const
        {
            ColorF128 copy;

            copy.r = r * _color.r;
            copy.g = g * _color.g;
            copy.b = b * _color.b;
            copy.a = a * _color.a;

            return copy;
        }

        //////////////////////////////////////////
        inline void setRGB(F32 _r, F32 _g, F32 _b)
        {
            this->r = _r;
            this->g = _g;
            this->b = _b;
        }

        //////////////////////////////////////////
        inline Vec3F getRGB() const { return { r, g, b }; }

        //////////////////////////////////////////
        inline void setRGBA(F32 _r, F32 _g, F32 _b, F32 _a)
        {
            this->r = _r;
            this->g = _g;
            this->b = _b;
            this->a = _a;
        }

        //////////////////////////////////////////
        static ColorF128 FromString(String const& _colorString, S8 _separator = ';');

        //////////////////////////////////////////
        static ColorF128 FromStringHex(String const& _colorString);
    
        //////////////////////////////////////////
        String toStringHex() const;

        //////////////////////////////////////////
        void setString(String const& _colorString, S8 _separator = ';');


        //////////////////////////////////////////
        inline String toString(S8 separator = ';') const
        {
            String result;
            result += StringHelper::F32ToString(r);
            result += separator + StringHelper::F32ToString(g);
            result += separator + StringHelper::F32ToString(b);
            result += separator + StringHelper::F32ToString(a);
            return result;
        }

        //////////////////////////////////////////
        inline String toStringTag(S8 separator = ';') const
        {
            String result = "#{";
            result += StringHelper::F32ToString(r);
            result += separator + StringHelper::F32ToString(g);
            result += separator + StringHelper::F32ToString(b);
            result += separator + StringHelper::F32ToString(a);
            result += "}";

            return result;
        }

        //////////////////////////////////////////
        inline operator Vec3F() const { return { value.x, value.y, value.z }; }

        //////////////////////////////////////////
        inline operator Vec4F() const { return value; }


        //////////////////////////////////////////
        inline Vec3F toVec3F32() const { return { value.x, value.y, value.z }; }

        //////////////////////////////////////////
        inline Vec4F toVec4F32() const { return value; }


        //////////////////////////////////////////
        inline void toColorU32AndIntensity(
            ColorU32& _color,
            F32& _intensity) const
        {
            if (r > 1.0f || g > 1.0f || b > 1.0f)
            {
                F32 max = Math::Max(r, g, b);
                Vec4F normalizedColor(
                    r / max,
                    g / max,
                    b / max,
                    a);

                _color = ColorU32::FromVec4F32(normalizedColor);
                _intensity = max - 1.0f;
            }
            else
            {
                _color = ColorU32::FromVec4F32(value);
                _intensity = 0.0f;
            }
        }

        //////////////////////////////////////////
        inline ColorU32 getColorU32() const
        {
            ColorU32 color;
            F32 unused;
            toColorU32AndIntensity(color, unused);
            return color;
        }

        //////////////////////////////////////////
        static inline ColorF128 FromVec4F32AndIntensity(
            Vec4F const& _color,
            F32 _intensity)
        {
            F32 multiplier = 1.0f + _intensity;

            return ColorF128(
                _color.x * multiplier,
                _color.y * multiplier,
                _color.z * multiplier,
                _color.w);
        }

        //////////////////////////////////////////
        static inline ColorF128 FromColorU32AndIntensity(
            ColorU32 const& _color,
            F32 _intensity)
        {
            Vec4F vec4 = _color.toVec4F32();
            F32 multiplier = 1.0f + _intensity;

            return ColorF128(
                vec4.x * multiplier,
                vec4.y * multiplier,
                vec4.z * multiplier,
                vec4.w);
        }

        //////////////////////////////////////////
        inline Vec4F const* getDataPointer() const { return &value; }

    public:
        union
        {
            struct
            {
                F32 r;
                F32 g;
                F32 b;
                F32 a;
            };
            Vec4F value;
        };
    };


    //////////////////////////////////////////
    template <>
    inline ColorF128 Random::RangeRandom(ColorF128 const& _low, ColorF128 const& _high)
    {
        return ColorF128::Lerp(_low, _high, UnitRandom());
    }


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    inline void ValueToString(ColorF128 const& _value, String& _data)
    {
        _data = _value.toString(';');
    }

    //////////////////////////////////////////
    inline void ValueFromString(ColorF128& _value, CString _data, Size _count)
    {
        _value = ColorF128::FromString(String(_data, _count), ';');
    }

    //////////////////////////////////////////
    inline U32 GetValueSerializationSize(ColorF128 const& _value)
    {
        return sizeof(ColorF128);
    }

    //////////////////////////////////////////
    inline void SerializeValue(ColorF128 const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(ColorF128));
    }

    //////////////////////////////////////////
    inline void DeserializeValue(ColorF128& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(ColorF128));
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorF128_hpp_
//////////////////////////////////////////
