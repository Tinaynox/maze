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


    } // namespace PixelSheet2DHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
