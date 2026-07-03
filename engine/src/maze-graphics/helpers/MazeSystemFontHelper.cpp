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
#include "maze-graphics/helpers/MazeSystemFontHelper.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemFontHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API void CalculateSystemTextData(
            CString _text,
            S32& _rowsCount,
            S32& _maxColumnsCount,
            S32& _charsCount)
        {
            if (!_text)
                return;

            S32 columnsCounter = 0;

            CString p = _text;
            while (*p != 0)
            {
                if (*p == '\n')
                {
                    ++_rowsCount;

                    _maxColumnsCount = Math::Max(_maxColumnsCount, columnsCounter);
                    columnsCounter = 0;
                }
                else
                {
                    ++columnsCounter;
                    ++_charsCount;
                }

                ++p;
            }
            _maxColumnsCount = Math::Max(_maxColumnsCount, columnsCounter);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API S32 CalculateSystemTextRowColumns(CString _rowStart)
        {
            S32 columnsCount = 0;

            if (!_rowStart)
                return columnsCount;

            while (*_rowStart != 0 && *_rowStart != '\n')
            {
                ++columnsCount;
                ++_rowStart;
            }

            return columnsCount;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec2F CalculateSystemTextShift(
            SystemFontPtr const& _systemFont,
            HorizontalAlignment2D _horizontalAlignment,
            VerticalAlignment2D _verticalAlignment,
            Vec2F const& _areaSize,
            F32 _fontScale,
            F32 _lineSpacingScale,
            S32 _rowsCount,
            S32 _columnsCount)
        {
            Vec2F shift = Vec2F::c_zero;

            if (!_systemFont)
                return shift;

            F32 const rowWidth = F32(_columnsCount * (_systemFont->charSize.x - _systemFont->outline) + _systemFont->outline);
            F32 const rowHeightUnit = F32(_systemFont->charSize.y - _systemFont->outline);
            F32 const totalTextHeight = (F32(_rowsCount - 1) * rowHeightUnit * _lineSpacingScale + rowHeightUnit + F32(_systemFont->outline));
            F32 const rowSize = (F32)_systemFont->charSize.y;

            switch (_horizontalAlignment)
            {
                case HorizontalAlignment2D::Left: shift.x = 0.0f; break;
                case HorizontalAlignment2D::Center: shift.x = (_areaSize.x - rowWidth * _fontScale) * 0.5f; break;
                case HorizontalAlignment2D::Right: shift.x = _areaSize.x - rowWidth * _fontScale; break;
                default: break;
            }

            switch (_verticalAlignment)
            {
                case VerticalAlignment2D::Top: shift.y = _areaSize.y; break;
                case VerticalAlignment2D::Middle: shift.y = (_areaSize.y + totalTextHeight * _fontScale) * 0.5f; break;
                case VerticalAlignment2D::Bottom: shift.y = (F32(_rowsCount - 1) * rowSize * _lineSpacingScale + rowSize) * _fontScale; break;
                default: break;
            }

            return shift;
        }

    } // namespace SystemFontHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
