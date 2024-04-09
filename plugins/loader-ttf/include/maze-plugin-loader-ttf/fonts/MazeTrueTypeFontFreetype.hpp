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
#if (!defined(_MazeTrueTypeFontFreetype_hpp_))
#define _MazeTrueTypeFontFreetype_hpp_


//////////////////////////////////////////
#include "maze-plugin-loader-ttf/MazeLoaderTTFHeader.hpp"
#include "maze-ui/fonts/MazeTrueTypeFont.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-plugin-loader-ttf/MazeFreetypeInclude.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TrueTypeFontFreetype);
    

    //////////////////////////////////////////
    // Class TrueTypeFontFreetype
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_LOADER_TTF_API TrueTypeFontFreetype
        : public TrueTypeFont
    {
    public:

        //////////////////////////////////////////
        static TrueTypeFontFreetypePtr Create();

        //////////////////////////////////////////
        static TrueTypeFontFreetypePtr Create(
            AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        static TrueTypeFontFreetypePtr Create(
            ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        virtual ~TrueTypeFontFreetype();


        //////////////////////////////////////////
        virtual bool loadFromAssetFile(
            AssetFilePtr const& _assetFile) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadFromByteBuffer(
            ByteBuffer const& _byteBuffer) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual TTFPagePtr const& ensureTTFPage(U32 _fontSize) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual TTFPagePtr const& ensureTTFOutlineThicknessPage(U32 _fontSize, F32 _outlineThickness) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FontGlyph const& ensureGlyph(U32 _codePoint, U32 _fontSize, TTFPagePtr const& _page) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FontGlyph const& ensureGlyph(U32 _codePoint, U32 _fontSize) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FontGlyph const& ensureOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness, TTFPagePtr const& _page) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FontGlyph const& ensureOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getLineSpacing(U32 _fontSize) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getAscender(U32 _fontSize) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getDescender(U32 _fontSize) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getKerning(U32 _first, U32 _second, U32 _fontSize) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getUnderlinePosition(U32 _fontSize) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getUnderlineThickness(U32 _fontSize) MAZE_OVERRIDE;

        



        //////////////////////////////////////////
        virtual void collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        TrueTypeFontFreetype();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;


        //////////////////////////////////////////
        bool selectFTPixelSize(U32 _characterSize);

        //////////////////////////////////////////
        void cleanupFreeType();

        //////////////////////////////////////////
        inline FontGlyph loadGlyph(
            TTFPagePtr const& _page,
            U32 _codePoint,
            U32 _fontSize,
            F32 _outlineThickness = 0.0f);

        //////////////////////////////////////////
        inline Rect2DS findGlyphRect(TTFPagePtr const& _page, U32 _width, U32 _height);

        //////////////////////////////////////////
        inline U64 getTTFOutlineGlyphKey(U32 _fontSize, F32 _outlineThickness) const
        {
            return (static_cast<U64>(*reinterpret_cast<U32*>(&_outlineThickness)) << 32) | (static_cast<U64>(_fontSize));
        }

    protected:

        FT_Library m_library = nullptr;
        FT_StreamRec* m_streamRec = nullptr;
        FT_Face m_face = nullptr;
        FT_Stroker m_stroker = nullptr;

        TTFPageTable<U32> m_pages;
        TTFPageTable<U64> m_outlineThicknessPages;

    private:
        ByteBuffer m_memoryBuffer;
    };


    //////////////////////////////////////////
    FontGlyph TrueTypeFontFreetype::loadGlyph(
        TTFPagePtr const& _page,
        U32 _codePoint,
        U32 _fontSize,
        F32 _outlineThickness)
    {
        S32 scale = 1;

        // The glyph to return
        FontGlyph glyph;

        if (!m_face)
            return glyph;

        // Set the character size
        if (!selectFTPixelSize(_fontSize * scale))
            return glyph;

        // Load the glyph corresponding to the code point
        FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
        if (_outlineThickness != 0)
            flags |= FT_LOAD_NO_BITMAP;

        if (FT_Load_Char(m_face, _codePoint, flags) != 0)
            return glyph;

        // Retrieve the glyph
        FT_Glyph glyphDesc;
        if (FT_Get_Glyph(m_face->glyph, &glyphDesc) != 0)
        {
            MAZE_ERROR("FT_Get_Glyph error!");
            return glyph;
        }

        // Apply outline (there is no fallback for outline) if necessary -- first technique using outline (highest quality)
        // FT_Pos weight = 1 << 6;
        bool outline = (glyphDesc->format == FT_GLYPH_FORMAT_OUTLINE);
        if (outline)
        {
            if (_outlineThickness != 0)
            {
                FT_Stroker_Set(
                    m_stroker,
                    static_cast<FT_Fixed>(_outlineThickness * scale * static_cast<F32>(1 << 6)),
                    FT_STROKER_LINECAP_ROUND,
                    FT_STROKER_LINEJOIN_ROUND,
                    0);
                FT_Glyph_Stroke(&glyphDesc, m_stroker, false);
            }
        }

        // Convert the glyph to a bitmap (i.e. rasterize it)
        FT_Glyph_To_Bitmap(&glyphDesc, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyphDesc)->bitmap;

        // Compute the glyph's advance offset
        F32 invGlyphMetricsDiv = 1.0f / static_cast<F32>(scale * (1 << 6));
        glyph.advance = static_cast<F32>(m_face->glyph->metrics.horiAdvance) * invGlyphMetricsDiv;

        S32 width = bitmap.width;
        S32 height = bitmap.rows;

        if ((width > 0) && (height > 0))
        {
            // Leave a small padding around characters, so that filtering doesn't
            // pollute them with pixels from neighbors
            const U32 padding = 1;

            width += 2 * padding;
            height += 2 * padding;

            glyph.texture = _page->texture;

            // Find a good position for the new glyph into the texture
            glyph.textureRect = findGlyphRect(_page, width, height);

            // Make sure the texture data is positioned in the center
            // of the allocated texture rectangle
            glyph.textureRect.position.x += padding;
            glyph.textureRect.position.y += padding;
            glyph.textureRect.size.x -= 2 * padding;
            glyph.textureRect.size.y -= 2 * padding;

            glyph.textureCoords.position = (Vec2F)glyph.textureRect.position / glyph.texture->getSize();
            glyph.textureCoords.size = (Vec2F)glyph.textureRect.size / glyph.texture->getSize();

            // Compute the glyph's bounding box
            // The metrics found in face->glyph->metrics are normally expressed in 26.6 pixel format (i.e., 1/64th of pixels),
            // unless you use the FT_LOAD_NO_SCALE flag when calling FT_Load_Glyph or FT_Load_Char
            glyph.bounds.position.x = static_cast<F32>(m_face->glyph->metrics.horiBearingX) * invGlyphMetricsDiv - _outlineThickness;
            glyph.bounds.position.y = static_cast<F32>(m_face->glyph->metrics.horiBearingY) * invGlyphMetricsDiv + _outlineThickness;
            glyph.bounds.size.x = static_cast<F32>(m_face->glyph->metrics.width) * invGlyphMetricsDiv + _outlineThickness * 2;
            glyph.bounds.size.y = static_cast<F32>(m_face->glyph->metrics.height) * invGlyphMetricsDiv + _outlineThickness * 2;
            glyph.bounds.position.y -= glyph.bounds.size.y;

            // Resize the pixel buffer to the new size and fill it with transparent white pixels
            Vector<U8> pixelBuffer;
            pixelBuffer.resize(width * height * 4);

            U8* current = &pixelBuffer[0];
            U8* end = current + width * height * 4;

            while (current != end)
            {
                (*current++) = 255;
                (*current++) = 255;
                (*current++) = 255;
                (*current++) = 0;
            }

            // Extract the glyph's pixels from the bitmap
            const U8* pixels = bitmap.buffer;
            if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
            {
                // Pixels are 1 bit monochrome values
                for (S32 y = (S32)padding; y < height - (S32)padding; ++y)
                {
                    for (S32 x = (S32)padding; x < width - (S32)padding; ++x)
                    {
                        // The color channels remain white, just fill the alpha channel
                        S32 flippedY = height - y - 1;
                        Size index = x + flippedY * width;
                        pixelBuffer[index * 4 + 3] = ((pixels[(x - padding) / 8]) & (1 << (7 - ((x - padding) % 8)))) ? 255 : 0;
                    }
                    pixels += bitmap.pitch;
                }
            }
            else
            {
                // Pixels are 8 bits gray levels
                for (S32 y = (S32)padding; y < height - (S32)padding; ++y)
                {
                    for (S32 x = (S32)padding; x < width - (S32)padding; ++x)
                    {
                        // The color channels remain white, just fill the alpha channel
                        S32 flippedY = height - y - 1;
                        Size index = x + flippedY * width;
                        pixelBuffer[index * 4 + 3] = pixels[x - padding];
                    }
                    pixels += bitmap.pitch;
                }
            }

            // Write the pixels to the texture
            U32 x = glyph.textureRect.position.x - padding;
            U32 y = glyph.textureRect.position.y - padding;
            U32 w = glyph.textureRect.size.x + 2 * padding;
            U32 h = glyph.textureRect.size.y + 2 * padding;
            _page->texture->copyImageFrom(&pixelBuffer[0], PixelFormat::RGBA_U8, w, h, x, y);
        }

        // Delete the FT glyph
        FT_Done_Glyph(glyphDesc);

        return glyph;
    }


    //////////////////////////////////////////
    inline Rect2DS TrueTypeFontFreetype::findGlyphRect(TTFPagePtr const& _page, U32 _width, U32 _height)
    {
        // Find the line that fits well the glyph
        TTFRow* row = nullptr;
        F32 bestRatio = 0;
        for (Vector<TTFRow>::iterator it = _page->rows.begin(); it != _page->rows.end() && !row; ++it)
        {
            F32 ratio = static_cast<F32>(_height) / it->height;

            // Ignore rows that are either too small or too high
            if ((ratio < 0.7f) || (ratio > 1.0f))
                continue;

            // Check if there's enough horizontal space left in the row
            if (_width > _page->texture->getSize().x - it->width)
                continue;

            // Make sure that this new row is the best found so far
            if (ratio < bestRatio)
                continue;

            // The current row passed all the tests: we can select it
            row = &*it;
            bestRatio = ratio;
        }

        bool texturesChanged = false;

        // If we didn't find a matching row, create a new one (10% taller than the glyph)
        if (!row)
        {
            S32 rowHeight = _height + _height / 10;
            while (((S32)_page->nextRow + rowHeight >= _page->texture->getSize().y) || ((S32)_width >= _page->texture->getSize().x))
            {
                // Not enough space: resize the texture if possible
                U32 textureWidth = _page->texture->getSize().x;
                U32 textureHeight = _page->texture->getSize().y;
                U32 textureMaximumSize = RenderSystem::GetCurrentInstancePtr()->getTextureMaxSize();

                if ((textureWidth * 2 <= textureMaximumSize) && (textureHeight * 2 <= textureMaximumSize))
                {
                    // Make the texture 2 times bigger
                    Texture2DPtr newTexture = Texture2D::Create(textureWidth * 2, textureHeight * 2, PixelFormat::RGBA_U8);
                    newTexture->setName("ttf_texture");
                    newTexture->copyImageFrom(_page->texture);
                    newTexture->setMinFilter(_page->texture->getMinFilter());
                    newTexture->setMagFilter(_page->texture->getMagFilter());
                    _page->texture.swap(newTexture);

                    for (auto glyph : _page->glyphs)
                    {
                        glyph.second.textureCoords.position = (Vec2F)glyph.second.textureRect.position / _page->texture->getSize();
                        glyph.second.textureCoords.size = (Vec2F)glyph.second.textureRect.size / _page->texture->getSize();
                    }

                    texturesChanged = true;
                }
                else
                {
                    // Oops, we've reached the maximum texture size...
                    MAZE_ERROR("Failed to add a new character to the font: the maximum texture size has been reached");
                    return Rect2DS(0, 0, 2, 2);
                }
            }

            // We can now create the new row
            _page->rows.push_back(TTFRow(_page->nextRow, rowHeight));
            _page->nextRow += rowHeight;
            row = &_page->rows.back();
        }

        // Find the glyph's rectangle on the selected row
        Rect2DS rect(row->width, row->top, _width, _height);

        // Update the row informations
        row->width += _width;

        if (texturesChanged)
            eventTexturesChanged();

        return rect;
    }
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTrueTypeFontFreetype_hpp_
//////////////////////////////////////////
