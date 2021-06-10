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
#include "maze-graphics/helpers/MazeColorHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace ColorHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec3DF ConvertRGBToHSV(F32 _r, F32 _g, F32 _b)
        {
            F32 cmax = Math::Max(_r, Math::Max(_g, _b));
            F32 cmin = Math::Min(_r, Math::Min(_g, _b));
            F32 diff = cmax - cmin;
            F32 h = -1.0f;
            F32 s = -1.0f;

            if (cmax == cmin)
                h = 0.0f;
            else 
            if (cmax == _r)
                h = F32(S32(60 * ((_g - _b) / diff) + 360) % 360);
            else
            if (cmax == _g)
                h = F32(S32(60 * ((_b - _r) / diff) + 120) % 360);
            else
            if (cmax == _b)
                h = F32(S32(60 * ((_r - _g) / diff) + 240) % 360);

            if (cmax == 0)
                s = 0;
            else
                s = (diff / cmax);

            F32 v = cmax;

            return {h, s, v};
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec3DF ConvertHSVToRGB(F32 _h, F32 _s, F32 _v)
        {
            F32 hh;
            F32 p;
            F32 q;
            F32 t;
            F32 ff;
            S32 i;
            Vec3DF rgb;

            if (_s <= 0.0f)
            {
                rgb.x = _v;
                rgb.y = _v;
                rgb.z = _v;
                return rgb;
            }

            hh = _h;

            if (hh >= 360.0f)
                hh = F32(S32(hh) % 360);

            hh /= 60.0f;
            i = (S32)hh;
            ff = hh - i;
            p = _v * (1.0f - _s);
            q = _v * (1.0f - (_s * ff));
            t = _v * (1.0f - (_s * (1.0f - ff)));

            switch (i) 
            {
            case 0:
                rgb.x = _v;
                rgb.y = t;
                rgb.z = p;
                break;

            case 1:
                rgb.x = q;
                rgb.y = _v;
                rgb.z = p;
                break;

            case 2:
                rgb.x = p;
                rgb.y = _v;
                rgb.z = t;
                break;

            case 3:
                rgb.x = p;
                rgb.y = q;
                rgb.z = _v;
                break;

            case 4:
                rgb.x = t;
                rgb.y = p;
                rgb.z = _v;
                break;

            case 5:
            default:
                rgb.x = _v;
                rgb.y = p;
                rgb.z = q;
                break;
            }

            return rgb;
        }


    } // namespace ColorHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
