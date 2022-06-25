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
#if (!defined(_MazeFont_hpp_))
#define _MazeFont_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-ui/fonts/MazeFontGlyph.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Font);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Material);
    

    //////////////////////////////////////////
    // Class Font
    //
    //////////////////////////////////////////
    class MAZE_UI_API Font
        : public SharedObject<Font>
    {
    public:

        //////////////////////////////////////////
        ~Font();

        //////////////////////////////////////////
        static FontPtr Create();

        //////////////////////////////////////////
        static FontPtr Create(
            AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }


        //////////////////////////////////////////
        bool loadFromAssetFile(
            AssetFilePtr const& _assetFile);




        //////////////////////////////////////////
        FontGlyphStorageData* getGlyphStorageData(U32 _codePoint);

        //////////////////////////////////////////
        FontGlyph const& getGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize);

        //////////////////////////////////////////
        FontGlyph const& getGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, TTFPagePtr<U32>& _ttfPage);

        //////////////////////////////////////////
        FontGlyph const& getGlyph(U32 _codePoint, U32 _fontSize);

        //////////////////////////////////////////
        FontGlyph const& getOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, F32 _outlineThickness);

        //////////////////////////////////////////
        FontGlyph const& getOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, F32 _outlineThickness, TTFPagePtr<U64>& _ttfPage);

        //////////////////////////////////////////
        FontGlyph const& getOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness);

        //////////////////////////////////////////
        F32 getLineSpacing(U32 _fontSize) const;

        //////////////////////////////////////////
        F32 getKerning(U32 _first, U32 _second, U32 _fontSize) const;

        //////////////////////////////////////////
        F32 getUnderlinePosition(U32 _fontSize) const;

        //////////////////////////////////////////
        F32 getUnderlineThickness(U32 _fontSize) const;

        //////////////////////////////////////////
        TrueTypeFontPtr const& getDefaultFont() const { return m_defaultGlyphsData.trueTypeFont; }


    public:

        //////////////////////////////////////////
        static void FromString(FontPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(Font const* _value, String& _data);

    protected:

        //////////////////////////////////////////
        Font();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void updateMaterialTextures();

    protected:
        String m_name;

        Vector<FontGlyphStorageData> m_glyphsData;
        FontGlyphStorageData m_defaultGlyphsData;
        Map<U32, FontGlyphStorageData*> m_glyphsMap;
    };


    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(Font);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontPtr>::value), void>::type
        ValueToString(FontPtr const& _value, String& _data)
    {
        Font::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontPtr>::value), void>::type
        ValueFromString(FontPtr& _value, CString _data, Size _count)
    {
        Font::FromString(_value, _data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontPtr>::value), U32>::type
        GetValueSerializationSize(FontPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontPtr>::value), void>::type
        SerializeValue(FontPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<FontPtr>::value), void>::type
        DeserializeValue(FontPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFont_hpp_
//////////////////////////////////////////
