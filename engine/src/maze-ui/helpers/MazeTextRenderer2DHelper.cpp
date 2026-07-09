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
#include "MazeUIHeader.hpp"
#include "maze-ui/helpers/MazeTextRenderer2DHelper.hpp"
#include "maze-graphics/MazeColorU32.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace TextRenderer2DHelper
    {
        //////////////////////////////////////////
        MAZE_UI_API bool ExtractColorTags(
            String& _text,
            ColorF128 const& _baseColor,
            Deque<Pair<Size, ColorF128>>& _outColorTags)
        {
            bool tagsFound = false;

            Size startTagPosition = _text.find("#{");
            Size endTagPosition = _text.find('}', startTagPosition);
            while (startTagPosition != String::npos && endTagPosition != String::npos)
            {
                String colorString = _text.substr(startTagPosition + 2, endTagPosition - startTagPosition - 2);
                ColorF128 color;

                if (colorString == "-")
                    color = _baseColor;
                else
                    color = ColorU32::FromString(colorString, ',');

                _text.erase(startTagPosition, endTagPosition - startTagPosition + 1);
                _outColorTags.push_back(Pair<Size, ColorF128>(startTagPosition, color));
                tagsFound = true;

                startTagPosition = _text.find("#{");
                endTagPosition = _text.find('}', startTagPosition);
            }

            return tagsFound;
        }

        //////////////////////////////////////////
        MAZE_UI_API void ApplySymbolsLimitPolicy(
            String& _text,
            U32 _symbolsLimit,
            TextRenderer2DSymbolsLimitPolicy _policy)
        {
            if (_symbolsLimit == 0 || _policy == TextRenderer2DSymbolsLimitPolicy::None)
                return;

            bool textCropped = false;

            while (TextHelper::GetSymbolsCountUTF8(_text) > _symbolsLimit)
            {
                _text = TextHelper::PopOneSymbolUTF8(_text);
                textCropped = true;
            }

            if (!textCropped)
                return;

            switch (_policy.toEnum())
            {
                case TextRenderer2DSymbolsLimitPolicy::Dot:
                {
                    _text = TextHelper::PopOneSymbolUTF8(_text);
                    _text += ".";
                    break;
                }
                case TextRenderer2DSymbolsLimitPolicy::ThreeDots:
                {
                    _text = TextHelper::PopOneSymbolUTF8(_text);
                    _text = TextHelper::PopOneSymbolUTF8(_text);
                    _text = TextHelper::PopOneSymbolUTF8(_text);
                    _text += "...";
                    break;
                }
                default:
                    break;
            }
        }

        //////////////////////////////////////////
        MAZE_UI_API F32 CalculateXAlignOffset(
            TextRenderer2DWidthPolicy _widthPolicy,
            HorizontalAlignment2D _horizontalAlignment,
            F32 _areaWidth,
            F32 _rowLength)
        {
            if (_widthPolicy == TextRenderer2DWidthPolicy::Abut)
            {
                switch (_horizontalAlignment)
                {
                    case HorizontalAlignment2D::Left: return Math::Min(0.0f, _areaWidth - _rowLength);
                    case HorizontalAlignment2D::Right: return Math::Max(0.0f, _areaWidth - _rowLength);
                    case HorizontalAlignment2D::Center: return (_areaWidth - _rowLength) * 0.5f;
                    default:
                        MAZE_NOT_IMPLEMENTED;
                        break;
                }
            }
            else
            {
                switch (_horizontalAlignment)
                {
                    case HorizontalAlignment2D::Left: return 0.0f;
                    case HorizontalAlignment2D::Right: return _areaWidth - _rowLength;
                    case HorizontalAlignment2D::Center: return (_areaWidth - _rowLength) * 0.5f;
                    default:
                        MAZE_NOT_IMPLEMENTED;
                        break;
                }
            }

            return 0.0f;
        }

        //////////////////////////////////////////
        MAZE_UI_API F32 CalculateTextPositionY(
            VerticalAlignment2D _verticalAlignment,
            F32 _areaHeight,
            F32 _ascent,
            F32 _descent,
            F32 _linespace,
            F32 _totalTextHeight,
            Size _actualRowsCount)
        {
            switch (_verticalAlignment)
            {
                case VerticalAlignment2D::Top:
                    return _areaHeight - _ascent;
                case VerticalAlignment2D::Middle:
                    return (_areaHeight - _totalTextHeight) * 0.5f - _descent + F32(_actualRowsCount - 1) * _linespace;
                case VerticalAlignment2D::Bottom:
                    return -_descent + F32(_actualRowsCount - 1) * _linespace;
                default:
                    MAZE_NOT_IMPLEMENTED;
                    break;
            }

            return 0.0f;
        }

    } // namespace TextRenderer2DHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
