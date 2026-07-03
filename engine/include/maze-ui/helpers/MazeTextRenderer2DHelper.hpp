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
#if (!defined(_MazeTextRenderer2DHelper_hpp_))
#define _MazeTextRenderer2DHelper_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-graphics/MazeAlignment2D.hpp"
#include "maze-graphics/MazeColorF128.hpp"
#include "maze-ui/ecs/components/MazeTextRenderer2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace TextRenderer2DHelper
    {
        //////////////////////////////////////////
        // Cuts '#{r,g,b[,a]}' / '#{-}' tags out of _text and collects them as
        // (character offset -> color) pairs. '#{-}' resets to _baseColor.
        // Returns true if at least one tag was found
        MAZE_UI_API bool ExtractColorTags(
            String& _text,
            ColorF128 const& _baseColor,
            Deque<Pair<Size, ColorF128>>& _outColorTags);

        //////////////////////////////////////////
        // Crops _text to _symbolsLimit symbols according to _policy
        MAZE_UI_API void ApplySymbolsLimitPolicy(
            String& _text,
            U32 _symbolsLimit,
            TextRenderer2DSymbolsLimitPolicy _policy);

        //////////////////////////////////////////
        MAZE_UI_API F32 CalculateXAlignOffset(
            TextRenderer2DWidthPolicy _widthPolicy,
            HorizontalAlignment2D _horizontalAlignment,
            F32 _areaWidth,
            F32 _rowLength);

        //////////////////////////////////////////
        MAZE_UI_API F32 CalculateTextPositionY(
            VerticalAlignment2D _verticalAlignment,
            F32 _areaHeight,
            F32 _ascent,
            F32 _descent,
            F32 _linespace,
            F32 _totalTextHeight,
            Size _actualRowsCount);

    } // namespace TextRenderer2DHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextRenderer2DHelper_hpp_
//////////////////////////////////////////
