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
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ColorU32
    //
    //////////////////////////////////////////
    const ColorU32 ColorU32::c_zero            = ColorU32(0, 0, 0, 0);
    const ColorU32 ColorU32::c_white           = ColorU32(255, 255, 255, 255);
    const ColorU32 ColorU32::c_whiteSoft       = ColorU32(252, 252, 252, 255);
    const ColorU32 ColorU32::c_gray            = ColorU32(128, 128, 128, 255);
    const ColorU32 ColorU32::c_lightGray       = ColorU32(192, 192, 192, 255);
    const ColorU32 ColorU32::c_darkGray        = ColorU32(64, 64, 64, 255);
    const ColorU32 ColorU32::c_black           = ColorU32(0, 0, 0, 255);
    const ColorU32 ColorU32::c_blackSoft       = ColorU32(7, 9, 9, 255);
    const ColorU32 ColorU32::c_red             = ColorU32(255, 0, 0, 255);
    const ColorU32 ColorU32::c_green           = ColorU32(0, 255, 0, 255);
    const ColorU32 ColorU32::c_blue            = ColorU32(0, 0, 255, 255);
    const ColorU32 ColorU32::c_cyan            = ColorU32(0, 255, 255, 255);
    const ColorU32 ColorU32::c_magenta         = ColorU32(255, 0, 255, 255);
    const ColorU32 ColorU32::c_yellow          = ColorU32(255, 255, 0, 255);
    const ColorU32 ColorU32::c_transparent     = c_zero;

    //////////////////////////////////////////
    ColorU32 ColorU32::Random()
    {
        return ColorU32(rand()%255, rand()%255, rand()%255);
    }

    //////////////////////////////////////////
    ColorU32 ColorU32::Lerp(ColorU32 const& _color0, ColorU32 const& _color1, float _scalar)
    {
        return ColorU32(
            Math::ClampLerp(_color0.r, _color1.r, _scalar),
            Math::ClampLerp(_color0.g, _color1.g, _scalar),
            Math::ClampLerp(_color0.b, _color1.b, _scalar),
            Math::ClampLerp(_color0.a, _color1.a, _scalar));
    }
    
    //////////////////////////////////////////
    ColorU32 ColorU32::FromString(String const& _colorString, S8 _separator)
    {
        ColorU32 color(0, 0, 0, 255);

        Vector<String> colorComponents;
        StringHelper::SplitWords(_colorString, colorComponents, _separator);
        Size colorComponentsSize = colorComponents.size();
        if (colorComponentsSize >= 3)
        {
            color.r = StringHelper::StringToU8(colorComponents[0].c_str());
            color.g = StringHelper::StringToU8(colorComponents[1].c_str());
            color.b = StringHelper::StringToU8(colorComponents[2].c_str());
        }

        if (colorComponentsSize >= 4)
        {
            color.a = StringHelper::StringToU8(colorComponents[3].c_str());
        }

        return color;
    }

    //////////////////////////////////////////
    ColorU32 ColorU32::FromStringHex(String const& _colorString)
    {
        ColorU32 color;
        
        if (_colorString.empty())
            return color;
        
        String colorString = _colorString;
        while (!colorString.empty() && colorString.front() == '#')
            colorString.erase(colorString.begin());

        if (colorString.size() >= 6)
        {
            color.r = static_cast<U8>(strtol(colorString.substr(0, 2).c_str(), NULL, 16));
            color.g = static_cast<U8>(strtol(colorString.substr(2, 2).c_str(), NULL, 16));
            color.b = static_cast<U8>(strtol(colorString.substr(4, 2).c_str(), NULL, 16));
        }
        else
        {
            color = ColorU32::c_white;
        }
        
        return color;
    }

    //////////////////////////////////////////
    String ColorU32::toStringHex() const
    {
        StringStream hexValue;
         
         hexValue << '#';

         hexValue << std::setfill('0')
                  << std::hex
                  << std::setw(2)
                  << static_cast<U32>(r)
                  << std::setw(2)
                  << static_cast<U32>(g)
                  << std::setw(2)
                  << static_cast<U32>(b);

         return hexValue.str();
    }

    //////////////////////////////////////////
    void ColorU32::setString(String const& _colorString, S8 _separator)
    {
        (*this) = ColorU32::FromString(_colorString, _separator);
    }

    //////////////////////////////////////////
    U32 ColorU32::toRGB_U8() const
    {
        return (b << 16 | g << 8 | r);
    }

    //////////////////////////////////////////
    U32 ColorU32::toRGBA_U8() const
    {
        return(a << 24 | b << 16 | g << 8 | r);
    }

    //////////////////////////////////////////
    void ColorU32::setRGBA_U8(U32 _value)
    {
        r = (_value & 0x000000FF);
        g = (_value & 0x0000FF00) >> 8;
        b = (_value & 0x00FF0000) >> 16;
        a = (_value & 0xFF000000) >> 24;
    }

    //////////////////////////////////////////
    Vec3F ColorU32::toVec3F32() const
    {
        return Vec3F(r/255.0f, g/255.0f, b/255.0f);
    }

    //////////////////////////////////////////
    void ColorU32::setVec3F32(Vec3F const& _vec)
    {
        r = (U8)(Math::Clamp(_vec.x*255.0f, 0.0f, 255.0f));
        g = (U8)(Math::Clamp(_vec.y*255.0f, 0.0f, 255.0f));
        b = (U8)(Math::Clamp(_vec.z*255.0f, 0.0f, 255.0f));
        a = 255;
    }

    //////////////////////////////////////////
    Vec4F ColorU32::toVec4F32() const
    {
        return Vec4F(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
    }

    //////////////////////////////////////////
    void ColorU32::setVec4F32(Vec4F const& _vec)
    {
        r = (U8)(Math::Clamp(_vec.x*255.0f, 0.0f, 255.0f));
        g = (U8)(Math::Clamp(_vec.y*255.0f, 0.0f, 255.0f));
        b = (U8)(Math::Clamp(_vec.z*255.0f, 0.0f, 255.0f));
        a = (U8)(Math::Clamp(_vec.w*255.0f, 0.0f, 255.0f));
    }

    //////////////////////////////////////////
    Vec4U ColorU32::toVec4U32() const
    {
        return { r, g, b, a };
    }

    //////////////////////////////////////////
    void ColorU32::setVec4U32(Vec4U const& _vec)
    {
        r = static_cast<U8>(_vec.x);
        g = static_cast<U8>(_vec.y);
        b = static_cast<U8>(_vec.z);
        a = static_cast<U8>(_vec.w);
    }

    //////////////////////////////////////////
    Vec4U8 ColorU32::toVec4U8() const
    {
        return { r, g, b, a };
    }

    //////////////////////////////////////////
    void ColorU32::setVec4U8(Vec4U8 const& _vec)
    {
        r = _vec.x;
        g = _vec.y;
        b = _vec.z;
        a = _vec.w;
    }

    //////////////////////////////////////////
    String ColorU32::toString() const
    {
        return toString(';');
    }

    //////////////////////////////////////////
    void ColorU32::setString(CString _data, Size _count)
    {
        (*this) = ColorU32::FromString(String(_data, _count), ';');
    }

    //////////////////////////////////////////
    bool ColorU32::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(MAZE_HS("value"));
        if (paramIndex >= 0)
        {
            DataBlockParamType paramType = _dataBlock.getParamType(paramIndex);
            switch (paramType)
            {
                case DataBlockParamType::ParamU32:
                {
                    setRGBA_U8(_dataBlock.getU32(MAZE_HS("value")));
                    break;
                }
                case DataBlockParamType::ParamVec4U8:
                {
                    setVec4U8(_dataBlock.getVec4U8(MAZE_HS("value")));
                    break;
                }
                default:
                {
                    MAZE_ERROR("Unsupported color value type - %d", (S32)paramType);
                    break;
                }
            }

        }

        return true;
    }

    //////////////////////////////////////////
    void ColorU32::toDataBlock(DataBlock& _dataBlock) const
    {
        _dataBlock.setVec4U8(MAZE_HS("value"), toVec4U8());
    }

} // namespace Maze
//////////////////////////////////////////
