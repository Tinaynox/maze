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
#include "maze-core/serialization/MazeXMLSerializable.hpp"
#include "maze-core/serialization/MazeDataBlockSerializable.hpp"
#include "maze-core/data/MazeDataBlock.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Font);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(Material);
    

    //////////////////////////////////////////
    // Class Font
    //
    //////////////////////////////////////////
    class MAZE_UI_API Font
        : public SharedObject<Font>
        , public MultiDelegateCallbackReceiver
        , public IDataBlockSerializable
        , public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        virtual ~Font();

        //////////////////////////////////////////
        static FontPtr Create();

        //////////////////////////////////////////
        static FontPtr Create(
            AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(HashedString const& _name) { m_name = _name; }


        //////////////////////////////////////////
        bool loadFromAssetFile(
            AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        bool loadFromXMLDocument(tinyxml2::XMLDocument& _doc);




        //////////////////////////////////////////
        FontGlyphStorageData* getGlyphStorageData(U32 _codePoint);


        //////////////////////////////////////////
        FontGlyph const& ensureGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, bool _bold);

        //////////////////////////////////////////
        FontGlyph const& ensureGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, bool _bold, TTFPagePtr const& _ttfPage);

        //////////////////////////////////////////
        FontGlyph const& ensureGlyph(U32 _codePoint, U32 _fontSize, bool _bold);


        //////////////////////////////////////////
        FontGlyph const& ensureOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, bool _bold, F32 _outlineThickness);

        //////////////////////////////////////////
        FontGlyph const& ensureOutlinedGlyphFromStorage(FontGlyphStorageData* _storage, U32 _codePoint, U32 _fontSize, bool _bold, F32 _outlineThickness, TTFPagePtr const& _ttfPage);

        //////////////////////////////////////////
        FontGlyph const& ensureOutlinedGlyph(U32 _codePoint, U32 _fontSize, bool _bold, F32 _outlineThickness);



        //////////////////////////////////////////
        F32 getLineSpacing(U32 _fontSize) const;

        //////////////////////////////////////////
        F32 getKerning(U32 _first, U32 _second, U32 _fontSize) const;

        //////////////////////////////////////////
        F32 getUnderlinePosition(U32 _fontSize) const;

        //////////////////////////////////////////
        F32 getUnderlineThickness(U32 _fontSize) const;

        //////////////////////////////////////////
        inline TrueTypeFontPtr const& getDefaultFont() const { return m_defaultGlyphsData.getTrueTypeFont(); }


        //////////////////////////////////////////
        void collectAllTextures(U32 _fontSize, bool _bold, Vector<Texture2DPtr>& _result) const;

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventTexturesChanged;

    public:

        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    public:

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE;

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

        //////////////////////////////////////////
        void subscribeGlyphsData();

        //////////////////////////////////////////
        void unsubscribeGlyphsData();

        //////////////////////////////////////////
        void notifyTexturesChanged();

    protected:
        HashedString m_name;

        Vector<FontGlyphStorageData> m_glyphsData;
        FontGlyphStorageData m_defaultGlyphsData;
        Map<U32, FontGlyphStorageData*> m_glyphsMap;
    };
    

    //////////////////////////////////////////
    // Class FontAssetRef
    //
    //////////////////////////////////////////
    class MAZE_UI_API FontAssetRef
        : public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        FontAssetRef(FontPtr const& _value = nullptr)
            : m_font(_value)
        {}

        //////////////////////////////////////////
        void setFont(FontPtr const& _value) { m_font = _value; }

        //////////////////////////////////////////
        inline FontPtr const& getFont() const { return m_font; }


        //////////////////////////////////////////
        inline bool operator==(FontAssetRef const& _value) const { return m_font == _value.getFont(); }

        //////////////////////////////////////////
        inline bool operator!=(FontAssetRef const& _value) const { return m_font != _value.getFont(); }

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    private:
        FontPtr m_font;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeFont_hpp_
//////////////////////////////////////////
