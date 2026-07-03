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
#if (!defined(_MazeSystemFontHelper_hpp_))
#define _MazeSystemFontHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/MazeSystemFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemFontHelper
    {
        //////////////////////////////////////////
        // Counts rows, the longest row and total drawable chars of a system text
        MAZE_GRAPHICS_API void CalculateSystemTextData(
            CString _text,
            S32& _rowsCount,
            S32& _maxColumnsCount,
            S32& _charsCount);

        //////////////////////////////////////////
        // Counts chars of a single row starting at _rowStart (until '\n' or end)
        MAZE_GRAPHICS_API S32 CalculateSystemTextRowColumns(CString _rowStart);

        //////////////////////////////////////////
        // Alignment shift of a system text row inside the area
        MAZE_GRAPHICS_API Vec2F CalculateSystemTextShift(
            SystemFontPtr const& _systemFont,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _areaSize,
            F32 _fontScale,
            F32 _lineSpacingScale,
            S32 _rowsCount,
            S32 _columnsCount);

    } // namespace SystemFontHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSystemFontHelper_hpp_
//////////////////////////////////////////
