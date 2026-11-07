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
        MAZE_GRAPHICS_API F32 CalculateSystemFontScale(
            SystemFont const& _systemFont,
            U32 _fontSize)
        {
            S32 glyphWidth = _systemFont.charSize.x - _systemFont.outline * 2;
            if (glyphWidth <= 0)
                return 0.0f;

            return (F32)_fontSize / (F32)glyphWidth;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API F32 CalculateSystemTextColumnAdvance(SystemFont const& _systemFont)
        {
            return F32(_systemFont.charSize.x - _systemFont.outline);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API F32 CalculateSystemTextRowAdvance(
            SystemFont const& _systemFont,
            F32 _lineSpacingScale)
        {
            return F32(_systemFont.charSize.y - _systemFont.outline) * _lineSpacingScale;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API Vec2F CalculateSystemTextSize(
            SystemFont const& _systemFont,
            F32 _lineSpacingScale,
            S32 _rowsCount,
            S32 _maxColumnsCount)
        {
            if (_rowsCount <= 0)
                return Vec2F::c_zero;

            F32 width = _maxColumnsCount > 0
                ? F32(_maxColumnsCount) * CalculateSystemTextColumnAdvance(_systemFont) + F32(_systemFont.outline)
                : 0.0f;
            F32 height = F32(_rowsCount - 1) * CalculateSystemTextRowAdvance(_systemFont, _lineSpacingScale) + F32(_systemFont.charSize.y);

            return Vec2F(width, height);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API S32 CalculateSystemTextColumnsFit(
            SystemFont const& _systemFont,
            F32 _fontScale,
            F32 _areaWidth)
        {
            F32 advance = CalculateSystemTextColumnAdvance(_systemFont) * _fontScale;
            if (advance <= 0.0f)
                return 0;

            F32 width = _areaWidth - F32(_systemFont.outline) * _fontScale;
            return Math::Max(0, (S32)Math::Floor(width / advance + 1e-4f));
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API S32 CalculateSystemTextRowsFit(
            SystemFont const& _systemFont,
            F32 _fontScale,
            F32 _lineSpacingScale,
            F32 _areaHeight)
        {
            F32 rowHeight = F32(_systemFont.charSize.y) * _fontScale;
            if (rowHeight <= 0.0f || _areaHeight + 1e-4f < rowHeight)
                return 0;

            F32 advance = CalculateSystemTextRowAdvance(_systemFont, _lineSpacingScale) * _fontScale;
            if (advance <= 0.0f)
                return 1;

            return 1 + (S32)Math::Floor((_areaHeight - rowHeight) / advance + 1e-4f);
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void ApplySystemTextWordWrap(
            CString _text,
            S32 _maxColumns,
            String& _outText)
        {
            _outText.clear();
            if (!_text)
                return;

            if (_maxColumns <= 0)
            {
                _outText = _text;
                return;
            }

            CString rowStart = _text;
            while (true)
            {
                CString rowEnd = rowStart + CalculateSystemTextRowColumns(rowStart);

                CString p = rowStart;
                while (rowEnd - p > _maxColumns)
                {
                    CString breakAt = nullptr;
                    for (CString s = p + _maxColumns; s > p; --s)
                    {
                        if (*s == ' ')
                        {
                            breakAt = s;
                            break;
                        }
                    }

                    if (breakAt)
                    {
                        _outText.append(p, breakAt);
                        p = breakAt + 1;
                    }
                    else
                    {
                        _outText.append(p, p + _maxColumns);
                        p += _maxColumns;
                    }

                    _outText.push_back('\n');
                }

                _outText.append(p, rowEnd);

                if (*rowEnd == 0)
                    break;

                _outText.push_back('\n');
                rowStart = rowEnd + 1;
            }
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void ApplySystemTextTruncate(
            CString _text,
            S32 _maxColumns,
            S32 _maxRows,
            bool _ellipsis,
            String& _outText)
        {
            _outText.clear();
            if (!_text || _maxRows <= 0)
                return;

            static CString const c_ellipsis = "...";
            S32 const ellipsisLength = 3;
            _maxColumns = Math::Max(0, _maxColumns);

            CString rowStart = _text;
            for (S32 row = 0; row < _maxRows; ++row)
            {
                S32 rowColumns = CalculateSystemTextRowColumns(rowStart);
                CString rowEnd = rowStart + rowColumns;
                bool lastRow = (*rowEnd == 0);
                bool rowsCut = !lastRow && (row == _maxRows - 1);
                bool columnsCut = rowColumns > _maxColumns;

                if (row > 0)
                    _outText.push_back('\n');

                if (_ellipsis && (columnsCut || rowsCut))
                {
                    S32 dots = Math::Min(ellipsisLength, _maxColumns);
                    S32 keep = Math::Min(rowColumns, _maxColumns - dots);
                    _outText.append(rowStart, rowStart + keep);
                    _outText.append(c_ellipsis, c_ellipsis + dots);
                }
                else
                {
                    _outText.append(rowStart, rowStart + Math::Min(rowColumns, _maxColumns));
                }

                if (lastRow)
                    break;

                rowStart = rowEnd + 1;
            }
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

            F32 const rowWidth = CalculateSystemTextSize(*_systemFont, _lineSpacingScale, 1, _columnsCount).x;
            F32 const totalTextHeight = CalculateSystemTextSize(*_systemFont, _lineSpacingScale, _rowsCount, 1).y;

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
                case VerticalAlignment2D::Bottom: shift.y = totalTextHeight * _fontScale; break;
                default: break;
            }

            return shift;
        }

    } // namespace SystemFontHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
