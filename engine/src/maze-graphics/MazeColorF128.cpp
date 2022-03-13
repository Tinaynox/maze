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
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ColorF128
    //
    //////////////////////////////////////////
    const ColorF128 ColorF128::c_zero            = ColorF128(0, 0, 0, 0);
    const ColorF128 ColorF128::c_white           = ColorF128(1.0f, 1.0f, 1.0f, 1.0f);
    const ColorF128 ColorF128::c_whiteSoft       = ColorF128(0.988f, 0.988f, 0.988f, 1.0f);
    const ColorF128 ColorF128::c_gray            = ColorF128(0.5f, 0.5f, 0.5f, 1.0f);
    const ColorF128 ColorF128::c_lightGray       = ColorF128(0.75f, 0.75f, 0.75f, 1.0f);
    const ColorF128 ColorF128::c_darkGray        = ColorF128(0.25f, 0.25f, 0.25f, 1.0f);
    const ColorF128 ColorF128::c_black           = ColorF128(0, 0, 0, 1.0f);
    const ColorF128 ColorF128::c_blackSoft       = ColorF128(0.027f, 0.035f, 0.035f, 1.0f);
    const ColorF128 ColorF128::c_red             = ColorF128(1.0f, 0, 0, 1.0f);
    const ColorF128 ColorF128::c_green           = ColorF128(0, 1.0f, 0, 1.0f);
    const ColorF128 ColorF128::c_blue            = ColorF128(0, 0, 1.0f, 1.0f);
    const ColorF128 ColorF128::c_cyan            = ColorF128(0, 1.0f, 1.0f, 1.0f);
    const ColorF128 ColorF128::c_magenta         = ColorF128(1.0f, 0, 1.0f, 1.0f);
    const ColorF128 ColorF128::c_yellow          = ColorF128(1.0f, 1.0f, 0, 1.0f);
    const ColorF128 ColorF128::c_transparent     = c_zero;

    //////////////////////////////////////////
    ColorF128 ColorF128::Random()
    {
        return ColorF128(
            (rand()%255) * 0.00390625f,
            (rand()%255) * 0.00390625f,
            (rand()%255) * 0.00390625f);
    }

    //////////////////////////////////////////
    ColorF128 ColorF128::Lerp(ColorF128 const& _color0, ColorF128 const& _color1, float _scalar)
    {
        return ColorF128(
            Math::ClampLerp(_color0.r, _color1.r, _scalar),
            Math::ClampLerp(_color0.g, _color1.g, _scalar),
            Math::ClampLerp(_color0.b, _color1.b, _scalar),
            Math::ClampLerp(_color0.a, _color1.a, _scalar));
    }
    
    //////////////////////////////////////////
    ColorF128 ColorF128::FromString(String const& _colorString, S8 _separator)
    {
        ColorF128 color(0, 0, 0, 1.0f);

        Vector<String> colorComponents;
        StringHelper::SplitWords(_colorString, colorComponents, _separator);
        Size colorComponentsSize = colorComponents.size();
        if (colorComponentsSize >= 3)
        {
            color.r = StringHelper::StringToF32(colorComponents[0].c_str());
            color.g = StringHelper::StringToF32(colorComponents[1].c_str());
            color.b = StringHelper::StringToF32(colorComponents[2].c_str());
        }

        if (colorComponentsSize >= 4)
        {
            color.a = StringHelper::StringToF32(colorComponents[3].c_str());
        }

        return color;
    }

    //////////////////////////////////////////
    ColorF128 ColorF128::FromStringHex(String const& _colorString)
    {
        ColorF128 color;
        
        if (_colorString.empty())
            return color;
        
        color.r = strtol(_colorString.substr(1, 2).c_str(), NULL, 16) * 0.00390625f;
        color.g = strtol(_colorString.substr(3, 2).c_str(), NULL, 16) * 0.00390625f;
        color.b = strtol(_colorString.substr(5, 2).c_str(), NULL, 16) * 0.00390625f;
        
        return color;
    }

    //////////////////////////////////////////
    String ColorF128::toStringHex() const
    {
        StringStream hexValue;
         
         hexValue << '#';

         hexValue << std::setfill('0')
                  << std::setw(2)
                  << std::hex
                  << Math::Clamp01(r) << Math::Clamp01(g) << Math::Clamp01(b) << Math::Clamp01(a);

         return hexValue.str();
    }

    //////////////////////////////////////////
    void ColorF128::setString(String const& _colorString, S8 _separator)
    {
        (*this) = ColorF128::FromString(_colorString, _separator);
    }


} // namespace Maze
//////////////////////////////////////////
