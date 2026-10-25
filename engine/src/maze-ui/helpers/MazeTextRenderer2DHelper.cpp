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
#include "maze-ui/fonts/MazeFont.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-ui/fonts/MazeFontGlyph.hpp"
#include <cstring>


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
        MAZE_UI_API void ApplyWordWrap(
            CString _text,
            Font* _font,
            U32 _fontSize,
            bool _bold,
            bool _colorTagsEnabled,
            F32 _maxWidth,
            String& _outResult)
        {
            _outResult.clear();

            if (!_text || !*_text || !_font || _maxWidth <= 0.0f)
            {
                if (_text)
                    _outResult = _text;
                return;
            }

            TrueTypeFontPtr const& defaultFont = _font->getDefaultFont();
            if (!defaultFont)
            {
                _outResult = _text;
                return;
            }

            auto ttfPage = &defaultFont->ensureTTFPage(_fontSize, _bold);
            F32 hSpace = static_cast<F32>(defaultFont->ensureGlyph(L' ', _fontSize, _bold, *ttfPage).advance);

            Size textLen = strlen(_text);

            String& result = _outResult;
            result.reserve(textLen + 8);

            F32 lineWidth = 0.0f;
            bool lineHasWord = false;

            // Pending space/tab run since the last word - dropped if the next word breaks
            Size pendingSpaceStart = String::npos;
            F32 pendingSpaceWidth = 0.0f;

            String word;
            F32 wordWidth = 0.0f;
            U32 prevChar = 0;

            auto flushWord = [&]()
            {
                if (word.empty())
                    return;

                if (lineHasWord && (lineWidth + pendingSpaceWidth + wordWidth > _maxWidth))
                {
                    result.resize(pendingSpaceStart);
                    result += '\n';
                    lineWidth = wordWidth;
                }
                else
                {
                    lineWidth += pendingSpaceWidth + wordWidth;
                }

                result.append(word);
                lineHasWord = true;

                word.clear();
                wordWidth = 0.0f;
                pendingSpaceWidth = 0.0f;
                pendingSpaceStart = String::npos;
            };

            CString it = _text;
            CString end = _text + textLen;
            while (it != end)
            {
                CString charStart = it;
                U32 curChar = utf8::next(it, end);

                // Color tag - zero width, kept verbatim, never a break point
                if (_colorTagsEnabled && curChar == '#' && it != end && *it == '{')
                {
                    while (it != end && *it != '}')
                        ++it;
                    if (it != end)
                        ++it;

                    word.append(charStart, it);
                    prevChar = 0;
                    continue;
                }

                if (curChar == '\n')
                {
                    flushWord();
                    result += '\n';
                    lineWidth = 0.0f;
                    lineHasWord = false;
                    // flushWord() only resets these when word is non-empty - do it here too
                    pendingSpaceStart = String::npos;
                    pendingSpaceWidth = 0.0f;
                    prevChar = 0;
                    continue;
                }

                if (curChar == ' ' || curChar == '\t')
                {
                    flushWord();

                    if (pendingSpaceStart == String::npos)
                        pendingSpaceStart = result.size();

                    result += (char)curChar;
                    pendingSpaceWidth += (curChar == ' ') ? hSpace : hSpace * 4.0f;
                    prevChar = 0;
                    continue;
                }

                F32 kerning = _font->getKerning(prevChar, curChar, _fontSize);
                prevChar = curChar;

                FontGlyphStorageData* glyphStorageData = _font->getGlyphStorageData(curChar);
                if (glyphStorageData && glyphStorageData->getTrueTypeFont())
                    ttfPage = &glyphStorageData->getTrueTypeFont()->ensureTTFPage(_fontSize, _bold);
                else
                    ttfPage = &defaultFont->ensureTTFPage(_fontSize, _bold);

                FontGlyph const& glyph = _font->ensureGlyphFromStorage(glyphStorageData, curChar, _fontSize, _bold, *ttfPage);

                wordWidth += kerning + glyph.advance;
                if (glyphStorageData &&
                    (glyphStorageData->type == FontGlyphStorageType::Sprite ||
                     glyphStorageData->type == FontGlyphStorageType::Entity))
                    wordWidth = Math::Round(wordWidth);

                word.append(charStart, it);
            }

            flushWord();
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
