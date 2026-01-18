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
#if (!defined(_MazePixelSheet2DHelper_hpp_))
#define _MazePixelSheet2DHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace PixelSheet2DHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool ConvertRGBToBGR(PixelSheet2D& _pixelSheet);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool ConvertA1RGB5ToRGB5A1(PixelSheet2D& _pixelSheet);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool SaveTGA(
            PixelSheet2D const& _pixelSheet,
            CString _fileName,
            bool _resetAlpha = false);

    } // namespace PixelSheet2DHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePixelSheet2DHelper_hpp_
//////////////////////////////////////////
