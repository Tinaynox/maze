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
#if (!defined(_MazeFontGlyph_hpp_))
#define _MazeFontGlyph_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(TrueTypeFont);
    

    //////////////////////////////////////////
    // Class Font
    //
    //////////////////////////////////////////
    struct MAZE_UI_API FontGlyph
    {
        // Offset to move horizontally to the next character
        F32 advance = 0.0f;
        // Bounding rectangle of the glyph, in coordinates relative to the baseline
        Rect2DF	bounds;
        // Font's texture
        Texture2DPtr texture;
        // Texture coordinates of the glyph inside the font's texture
        Rect2DS textureRect;
        // Texture coordinates of the glyph inside the font's texture
        Rect2DF textureCoords;
    };


    //////////////////////////////////////////
    // Enum FontGlyphStorageType
    //
    //////////////////////////////////////////
    enum class MAZE_UI_API FontGlyphStorageType
    {
        None = 0,
        TrueTypeFont,
        Sprite,
        Entity
    };


    //////////////////////////////////////////
    // Struct SpriteFontGlyphData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API SpriteFontGlyphData
    {
        //////////////////////////////////////////
        FontGlyph const& getGlyph(U32 _fontSize);

        //////////////////////////////////////////
        void collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result) const;

        U32 spriteGlyphFontSize;
        FontGlyph spriteGlyph;
        Map<U32, FontGlyph> glyphs;
    };


    //////////////////////////////////////////
    // Struct EntityFontGlyphData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API EntityFontGlyphData
    {
        //////////////////////////////////////////
        FontGlyph const& getGlyph(U32 _fontSize);

        //////////////////////////////////////////
        EntityPtr const& getPrefab();

        EntityPtr prefab;
        String prefabName;
        U32 prefabGlyphFontSize;
        FontGlyph prefabGlyph;
        Map<U32, FontGlyph> glyphs;
    };


    //////////////////////////////////////////
    // Struct FontGlyphStorageData
    //
    //////////////////////////////////////////
    struct MAZE_UI_API FontGlyphStorageData
        : public MultiDelegateCallbackReceiver
    {
        //////////////////////////////////////////
        inline FontGlyphStorageData(FontGlyphStorageType _type = FontGlyphStorageType::None)
            : fromCodePoints(0)
            , toCodePoints(0)
            , type(_type)
        {
        }

        //////////////////////////////////////////
        virtual ~FontGlyphStorageData()
        {
            setTrueTypeFont(nullptr);
        }

        //////////////////////////////////////////
        inline FontGlyphStorageData(FontGlyphStorageData const& _other)
        {
            fromCodePoints = _other.fromCodePoints;
            toCodePoints = _other.toCodePoints;
            type = _other.type;

            spriteData = _other.spriteData;
            entityData = _other.entityData;

            setTrueTypeFont(_other.trueTypeFont);
        }

        //////////////////////////////////////////
        inline bool contains(U32 _codePoint) const
        {
            return (_codePoint >= fromCodePoints) && (_codePoint <= toCodePoints);
        }

        //////////////////////////////////////////
        void collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result) const;


        //////////////////////////////////////////
        void setTrueTypeFont(TrueTypeFontPtr const& _trueTypeFont);

        //////////////////////////////////////////
        inline TrueTypeFontPtr const& getTrueTypeFont() const { return trueTypeFont; }

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventTexturesChanged;

    private:

        //////////////////////////////////////////
        void notifyTexturesChanged()
        {
            eventTexturesChanged();
        }

    public:
        U32 fromCodePoints;
        U32 toCodePoints;
        FontGlyphStorageType type;

        SpriteFontGlyphData spriteData;
        EntityFontGlyphData entityData;

    private:
        TrueTypeFontPtr trueTypeFont;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFont_hpp_
//////////////////////////////////////////
