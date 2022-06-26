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
#include "MazeLoaderTTFHeader.hpp"
#include "maze-plugin-loader-ttf/fonts/MazeTrueTypeFontFreetype.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TrueTypeFontFreetype
    //
    //////////////////////////////////////////
    TrueTypeFontFreetype::TrueTypeFontFreetype()
    {
        
    }

    //////////////////////////////////////////
    TrueTypeFontFreetype::~TrueTypeFontFreetype()
    {
        cleanupFreeType();
    }

    //////////////////////////////////////////
    TrueTypeFontFreetypePtr TrueTypeFontFreetype::Create()
    {
        TrueTypeFontFreetypePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TrueTypeFontFreetype, object, init());
        return object;
    }

    //////////////////////////////////////////
    TrueTypeFontFreetypePtr TrueTypeFontFreetype::Create(
        AssetFilePtr const& _assetFile)
    {
        TrueTypeFontFreetypePtr trueTypeFont = TrueTypeFontFreetype::Create();

        if (trueTypeFont)
            trueTypeFont->loadFromAssetFile(_assetFile);

        return trueTypeFont;
    }

    //////////////////////////////////////////
    TrueTypeFontFreetypePtr TrueTypeFontFreetype::Create(
        ByteBuffer const& _byteBuffer)
    {
        TrueTypeFontFreetypePtr trueTypeFont = TrueTypeFontFreetype::Create();

        if (trueTypeFont)
            trueTypeFont->loadFromByteBuffer(_byteBuffer);

        return trueTypeFont;
    }

    //////////////////////////////////////////
    bool TrueTypeFontFreetype::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void TrueTypeFontFreetype::cleanupFreeType()
    {
        // Destroy the stroker
        if (m_stroker)
            FT_Stroker_Done(m_stroker);

        // Destroy the font face
        if (m_face)
            FT_Done_Face(m_face);

        // Destroy the stream rec instance, if any (must be done after FT_Done_Face!)
        if (m_streamRec)
            delete m_streamRec;

        // Close the library
        if (m_library)
            FT_Done_FreeType(m_library);

        // Reset members
        m_library = nullptr;
        m_face = nullptr;
        m_stroker = nullptr;
        m_streamRec = nullptr;

        for (auto& page : m_pages)
            page.second->texture.reset();

        for (auto& page : m_outlineThicknessPages)        
            page.second->texture.reset();

        m_pages.clear();
    }

    //////////////////////////////////////////
    bool TrueTypeFontFreetype::loadFromAssetFile(
        AssetFilePtr const& _assetFile)
    {
        ByteBuffer byteBuffer;

        ClassUID classUID = _assetFile->getClassUID();
        _assetFile->readToByteBuffer(byteBuffer);

        return loadFromByteBuffer(byteBuffer);
    }

    //////////////////////////////////////////
    bool TrueTypeFontFreetype::loadFromByteBuffer(
        ByteBuffer const& _byteBuffer)
    {
        cleanupFreeType();

        m_memoryBuffer = _byteBuffer;
        MAZE_ERROR_RETURN_VALUE_IF(FT_Init_FreeType(&m_library) != 0, false, "Failed to load font! Failed to initialize FreeType!");
        MAZE_ERROR_RETURN_VALUE_IF(FT_New_Memory_Face(m_library, (const FT_Byte*)(m_memoryBuffer.getDataPointer()), (FT_Long)m_memoryBuffer.getSize(), 0, &m_face) != 0, false, "Failed to load font! Failed to create the font face!");

        if (FT_Stroker_New(m_library, &m_stroker) != 0)
        {
            MAZE_ERROR("Failed to load font! Failed to create the stroker!");
            FT_Done_Face(m_face);
            return false;
        }

        if (FT_Select_Charmap(m_face, FT_ENCODING_UNICODE) != 0)
        {
            MAZE_ERROR("Failed to load font! Failed to set the Unicode character set!");
            FT_Stroker_Done(m_stroker);
            FT_Done_Face(m_face);
            return false;
        }

        m_family = m_face->family_name ? m_face->family_name : String();

        return true;
    }

    ////////////////////////////////////
    TTFPagePtr<U32>& TrueTypeFontFreetype::getTTFPage(U32 _fontSize)
    {
        TTFPagePtr<U32>& page = m_pages[_fontSize];
        if (!page)
        {
            page = std::make_shared<TTFPage<U32>>();
            eventTexturesChanged();
        }
        return page;
    }

    ////////////////////////////////////
    TTFPagePtr<U64>& TrueTypeFontFreetype::getTTFOutlineThicknessPage(U32 _fontSize)
    {
        TTFPagePtr<U64>& page = m_outlineThicknessPages[_fontSize];
        if (!page)
        {
            page = std::make_shared<TTFPage<U64>>();
            eventTexturesChanged();
        }
        return page;
    }

    ////////////////////////////////////
    FontGlyph const& TrueTypeFontFreetype::getGlyph(U32 _codePoint, U32 _fontSize, TTFPagePtr<U32>& _page)
    {
        // Get the page corresponding to the character size
        TTFGlyphTable<U32>& glyphs = _page->glyphs;

         // Search the glyph into the cache
        auto it = glyphs.find(_codePoint);
        if (it != glyphs.end())
        {
            // Found: just return it
            return it->second;
        }
        else
        {
            // Not found: we have to load it
            FontGlyph glyph = loadGlyph(m_pages, _codePoint, _fontSize);
            return glyphs.emplace(_codePoint, glyph).first->second;
        }
    }

    ////////////////////////////////////
    FontGlyph const& TrueTypeFontFreetype::getGlyph(U32 _codePoint, U32 _fontSize)
    {
        return getGlyph(_codePoint, _fontSize, getTTFPage(_fontSize));
    }

    ////////////////////////////////////
    FontGlyph const& TrueTypeFontFreetype::getOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness, TTFPagePtr< U64 >& _page)
    {
        // Get the page corresponding to the character size
        TTFGlyphTable<U64>& glyphs = _page->glyphs;

        // Search the glyph into the cache
        auto it = glyphs.find(_codePoint);
        if (it != glyphs.end())
        {
            return it->second;
        }
        else
        {
            FontGlyph glyph = loadGlyph(m_outlineThicknessPages, _codePoint, _fontSize, _outlineThickness);
            return glyphs.insert(std::make_pair(_codePoint, glyph)).first->second;
        }
    }

    ////////////////////////////////////
    FontGlyph const& TrueTypeFontFreetype::getOutlinedGlyph(U32 _codePoint, U32 _fontSize, F32 _outlineThickness)
    {
        return getOutlinedGlyph(_codePoint, _fontSize, _outlineThickness, getTTFOutlineThicknessPage(_fontSize));
    }

    ////////////////////////////////////
    F32 TrueTypeFontFreetype::getLineSpacing(U32 _fontSize)
    {
        if (m_face && selectFTPixelSize(_fontSize))
        {
            return static_cast<F32>(m_face->size->metrics.height) / static_cast<F32>(1 << 6);
        }

        return 0.0f;
    }

    ////////////////////////////////////
    F32 TrueTypeFontFreetype::getKerning(U32 _first, U32 _second, U32 _fontSize)
    {
        // Special case where first or second is 0 (null character)
        if (_first == 0 || _second == 0)
            return 0.0f;

        FT_Face face = static_cast<FT_Face>(m_face);

        if (face && FT_HAS_KERNING(face) && selectFTPixelSize(_fontSize))
        {
            // Convert the characters to indices
            FT_UInt index1 = FT_Get_Char_Index(face, _first);
            FT_UInt index2 = FT_Get_Char_Index(face, _second);

            // Get the kerning vector
            FT_Vector kerning;
            FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

            // X advance is already in pixels for bitmap fonts
            if (!FT_IS_SCALABLE(face))
                return static_cast<F32>(kerning.x);

            // Return the X advance
            return static_cast<F32>(kerning.x) / static_cast<F32>(1 << 6);
        }

        return 0.0f;
    }

    ////////////////////////////////////
    F32 TrueTypeFontFreetype::getUnderlinePosition(U32 _fontSize)
    {
        if (m_face && selectFTPixelSize(_fontSize))
        {
            // Return a fixed position if font is a bitmap font
            if (!FT_IS_SCALABLE(m_face))
                return _fontSize / 10.0f;

            return -static_cast<F32>(FT_MulFix(m_face->underline_position, m_face->size->metrics.y_scale)) / static_cast<F32>(1 << 6);
        }

        return 0.0f;
    }

    ////////////////////////////////////
    F32 TrueTypeFontFreetype::getUnderlineThickness(U32 _fontSize)
    {
        if (m_face && selectFTPixelSize(_fontSize))
        {
            if (!FT_IS_SCALABLE(m_face))
                return _fontSize / 14.0f;

            return static_cast<F32>(FT_MulFix(m_face->underline_thickness, m_face->size->metrics.y_scale)) / static_cast<F32>(1 << 6);
        }

        return 0.0f;
    }

    //////////////////////////////////////////
    void TrueTypeFontFreetype::collectAllTextures(U32 _fontSize, Vector<Texture2DPtr>& _result)
    {
        {
            auto it = m_pages.find(_fontSize);
            if (it != m_pages.end())
            {
                TTFPagePtr<U32> const& page = it->second;
                if (page->texture)
                {
                    if (std::find(_result.begin(), _result.end(), page->texture) == _result.end())
                        _result.push_back(page->texture);
                }
            }
        }
        {
            auto it = m_outlineThicknessPages.find(_fontSize);
            if (it != m_outlineThicknessPages.end())
            {
                TTFPagePtr<U64> const& page = it->second;
                if (page->texture)
                {
                    if (std::find(_result.begin(), _result.end(), page->texture) == _result.end())
                        _result.push_back(page->texture);
                }
            }            
        }
    }

    ////////////////////////////////////
    bool TrueTypeFontFreetype::selectFTPixelSize(U32 _characterSize)
    {
        FT_UShort currentSize = m_face->size->metrics.x_ppem;

        if (currentSize != _characterSize)
        {
            FT_Error result = FT_Set_Pixel_Sizes(m_face, 0, _characterSize);

            if (result == FT_Err_Invalid_Pixel_Size)
            {
                // In the case of bitmap fonts, resizing can
                // fail if the requested size is not available
                if (!FT_IS_SCALABLE(m_face))
                {
                    MAZE_ERROR("Failed to set bitmap font size to %u!", _characterSize);
                    MAZE_ERROR("Available sizes are: ");
                    for (S32 i = 0; i < m_face->num_fixed_sizes; ++i)
                    {
                        MAZE_ERROR("%u", m_face->available_sizes[i].height);
                    }
                }
            }

            return result == FT_Err_Ok;
        }
        else
        {
            return true;
        }
    }

} // namespace Maze
//////////////////////////////////////////
