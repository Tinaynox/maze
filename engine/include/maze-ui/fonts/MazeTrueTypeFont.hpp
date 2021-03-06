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
#if (!defined(_MazeTrueTypeFont_hpp_))
#define _MazeTrueTypeFont_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-ui/fonts/MazeFontGlyph.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TrueTypeFont);
    MAZE_USING_SHARED_PTR(TrueTypeFontManager);
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Texture2D);
    

    //////////////////////////////////////////
    template <typename TGlyphKey>
    using TTFGlyphTable = Map<TGlyphKey, FontGlyph>;


    //////////////////////////////////////////
    // Struct TTFRow
    //
    //////////////////////////////////////////
    struct TTFRow
    {
    public:

        //////////////////////////////////////////
        inline TTFRow(U32 _rowTop, U32 _rowHeight)
            : width(0u)
            , top(_rowTop)
            , height(_rowHeight)
        {}

    public:
        // Current width of the row
        U32 width;

        // Y position of the row into the texture
        U32 top;

        // Height of the row
        U32 height;
    };


    //////////////////////////////////////////
    using GlyphKeyType = U32;


    //////////////////////////////////////////
    // Struct TTFPage
    //
    //////////////////////////////////////////
    struct TTFPage
    {
        //////////////////////////////////////////
        inline TTFPage();


        // Table mapping code points to their corresponding glyph
        TTFGlyphTable<GlyphKeyType> glyphs;

        // Texture containing the pixels of the glyphs
        Texture2DPtr texture;

        // Y position of the next new row in the texture
        U32 nextRow;

        // List containing the position of all the existing rows
        Vector<TTFRow> rows;
    };

    //////////////////////////////////////////
    TTFPage::TTFPage()
        : nextRow(3u)
    {
        Size size = 512u;
        ByteBufferPtr byteBuffer = ByteBuffer::Create(size * size * 4u, 0);

        texture = Texture2D::Create();
        texture->setName("ttf_texture");
        texture->setMinFilter(TextureFilter::Linear);
        texture->setMagFilter(TextureFilter::Linear);
        texture->loadFromBuffer(byteBuffer, PixelFormat::RGBA_U8, Vec2DU((U32)size, (U32)size), PixelFormat::RGBA_U8);
    }


    //////////////////////////////////////////
    using TTFPagePtr = SharedPtr<struct TTFPage>;

    template <typename TGlyphKey>
    using TTFPageTable = Map<TGlyphKey, TTFPagePtr>;


    //////////////////////////////////////////
    // Class TrueTypeFont
    //
    //////////////////////////////////////////
    class MAZE_UI_API TrueTypeFont
    {
    public:

        //////////////////////////////////////////
        static TrueTypeFontPtr Create(
            AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        virtual ~TrueTypeFont();


        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(String const& _value) { m_name = _value; }

        //////////////////////////////////////////
        inline String const& getFamily() const { return m_family; }

        //////////////////////////////////////////
        inline void setFamily(String const& _value) { m_family = _value; }


        //////////////////////////////////////////
        virtual bool loadFromAssetFile(
            AssetFilePtr const& _assetFile) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool loadFromByteBuffer(
            ByteBuffer const& _byteBuffer) MAZE_ABSTRACT;


        ////////////////////////////////////
        virtual TTFPagePtr& getTTFPage(U32 _fontSize) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual TTFPagePtr& getTTFOutlineThicknessPage(U32 _fontSize, F32 _outlineThickness) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual FontGlyph const& getGlyph(U32 _codePoint, U32 _fontSize, TTFPagePtr& _page) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual FontGlyph const& getGlyph(U32 _codePoint, U32 _fontSize) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual FontGlyph const& getOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness, TTFPagePtr& _page) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual FontGlyph const& getOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual F32 getLineSpacing(U32 _fontSize) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual F32 getAscender(U32 _fontSize) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual F32 getDescender(U32 _fontSize) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual F32 getKerning(U32 _first, U32 _second, U32 _fontSize) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual F32 getUnderlinePosition(U32 _fontSize) MAZE_ABSTRACT;

        ////////////////////////////////////
        virtual F32 getUnderlineThickness(U32 _fontSize) MAZE_ABSTRACT;

        


        //////////////////////////////////////////
        virtual void collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result) MAZE_ABSTRACT;

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventTexturesChanged;

    protected:

        //////////////////////////////////////////
        TrueTypeFont();

        //////////////////////////////////////////
        virtual bool init();

    protected:
        String m_name;
        String m_family;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTrueTypeFont_hpp_
//////////////////////////////////////////
