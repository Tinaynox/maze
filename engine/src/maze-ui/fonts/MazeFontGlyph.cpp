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
#include "maze-ui/fonts/MazeFontGlyph.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct SpriteFontGlyphData
    //
    //////////////////////////////////////////
    FontGlyph const& SpriteFontGlyphData::ensureGlyph(U32 _fontSize)
    {
        Map<U32, FontGlyph>::iterator it = glyphs.find(_fontSize);
        if (it != glyphs.end())
            return it->second;

        F32 p = (F32)_fontSize / (F32)spriteGlyphFontSize;
        FontGlyph& glyph = glyphs[_fontSize];
        glyph.advance = spriteGlyph.advance * p;
        glyph.bounds.position = spriteGlyph.bounds.position * p;
        glyph.bounds.size = spriteGlyph.bounds.size * p;
        glyph.texture = spriteGlyph.texture;
        glyph.textureCoords = spriteGlyph.textureCoords;
        glyph.textureRect = spriteGlyph.textureRect;

        return glyph;
    }

    //////////////////////////////////////////
    void SpriteFontGlyphData::collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result) const
    {
        if (spriteGlyph.texture)
        {
            if (std::find(_result.begin(), _result.end(), spriteGlyph.texture) == _result.end())
                _result.push_back(spriteGlyph.texture);
        }
    }


    //////////////////////////////////////////
    // Struct EntityFontGlyphData
    //
    //////////////////////////////////////////
    FontGlyph const& EntityFontGlyphData::ensureGlyph(U32 _fontSize)
    {
        Map<U32, FontGlyph>::iterator it = glyphs.find(_fontSize);
        if (it != glyphs.end())
            return it->second;

        F32 p = (F32)_fontSize / (F32)prefabGlyphFontSize;
        FontGlyph& glyph = glyphs[_fontSize];
        glyph.advance = prefabGlyph.advance * p;
        glyph.bounds.position = prefabGlyph.bounds.position * p;
        glyph.bounds.size = prefabGlyph.bounds.size * p;

        return glyph;
    }

    //////////////////////////////////////////
    EntityPtr const& EntityFontGlyphData::getPrefab()
    {
        static EntityPtr const nullPointer;
        return nullPointer;
    }


    //////////////////////////////////////////
    // Struct FontGlyphStorageData
    //
    //////////////////////////////////////////
    void FontGlyphStorageData::collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result) const
    {
        if (trueTypeFont)
            trueTypeFont->collectAllTextures(_fontSize, _result);
        spriteData.collectAllTextures(_fontSize, _result);
    }

    //////////////////////////////////////////
    void FontGlyphStorageData::setTrueTypeFont(TrueTypeFontPtr const& _trueTypeFont)
    {
        if (trueTypeFont)
        {
            trueTypeFont->eventTexturesChanged.unsubscribe(this, &FontGlyphStorageData::notifyTexturesChanged);
        }

        trueTypeFont = _trueTypeFont;

        if (trueTypeFont)
        {
            trueTypeFont->eventTexturesChanged.subscribe(this, &FontGlyphStorageData::notifyTexturesChanged);
        }
    }
    

} // namespace Maze
//////////////////////////////////////////
