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
#if (!defined(_MazeColorHelper_hpp_))
#define _MazeColorHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);
    MAZE_USING_MANAGED_SHARED_PTR(SubMesh);


    //////////////////////////////////////////
    namespace ColorHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec3F ConvertRGBToHSV(F32 _r, F32 _g, F32 _b);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API inline Vec3F ConvertRGBToHSV(Vec3F const& _color)
        {
            return ConvertRGBToHSV(_color.x, _color.y, _color.z);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API inline Vec3F ConvertRGBToHSV(U8 _r, U8 _g, U8 _b)
        {
            return ConvertRGBToHSV((F32)_r/255.0f, (F32)_g/255.0f, (F32)_b/255.0f);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API inline Vec3F ConvertRGBToHSV(S32 _r, S32 _g, S32 _b)
        {
            return ConvertRGBToHSV((F32)_r/255.0f, (F32)_g/255.0f, (F32)_b/255.0f);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API inline Vec3F ConvertRGBToHSV(ColorU32 const& _color)
        {
            return ConvertRGBToHSV(_color.r, _color.g, _color.b);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec3F ConvertHSVToRGB(F32 _h, F32 _s, F32 _v);

        //////////////////////////////////////////
        inline MAZE_GRAPHICS_API Vec3F ConvertHSVToRGB(Vec3F const& _color)
        {
            return ConvertHSVToRGB(_color.x, _color.y, _color.z);
        }

    } // namespace ColorHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeColorHelper_hpp_
//////////////////////////////////////////
