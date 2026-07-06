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
#include "maze-ui/ecs/components/MazeTextRenderer2D.hpp"
#include "maze-ui/helpers/MazeTextRenderer2DHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TextRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TextRenderer2D, AbstractTextRenderer2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(FontMaterialAssetRef, fontMaterial, FontMaterialAssetRef(), getFontMaterialRef, setFontMaterialRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextRenderer2DWidthPolicy, widthPolicy, TextRenderer2DWidthPolicy::None, getWidthPolicy, setWidthPolicy),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, outlineThickness, 0.0f, getOutlineThickness, setOutlineThickness),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, bold, false, getBold, setBold),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, outlineColor, ColorU32::c_white, getOutlineColor, setOutlineColor));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TextRenderer2D);


    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(TextRenderer2DSymbolsLimitPolicy);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(TextRenderer2DWidthPolicy);


    //////////////////////////////////////////
    // Struct TempGlyphData
    //
    //////////////////////////////////////////
    struct TempGlyphData
    {
        //////////////////////////////////////////
        TempGlyphData(
            FontGlyphStorageData* _glyphStorageData = nullptr,
            FontGlyph const* _glyph = nullptr,
            FontGlyph const* _outlineThicknessGlyph = nullptr)
            : glyphStorageData(_glyphStorageData)
            , glyph(_glyph)
            , outlineThicknessGlyph(_outlineThicknessGlyph)
        {

        }

        FontGlyphStorageData* glyphStorageData;
        FontGlyph const* glyph;
        FontGlyph const* outlineThicknessGlyph;
    };


    //////////////////////////////////////////
    TextRenderer2D::TextRenderer2D()
    {
        m_fontSize = 18u;
    }

    //////////////////////////////////////////
    TextRenderer2D::~TextRenderer2D()
    {
        setFontMaterial(FontMaterialPtr());
    }

    //////////////////////////////////////////
    TextRenderer2DPtr TextRenderer2D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        TextRenderer2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TextRenderer2D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool TextRenderer2D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool TextRenderer2D::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        if (!AbstractTextRenderer2D::init(
            _component,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void TextRenderer2D::removeSymbol()
    {
        if (getSymbolsCount() == 0)
            return;

        m_text = TextHelper::PopOneSymbolUTF8(m_text);

        enableFlag(Flags::MeshDataDirty);
    }

    //////////////////////////////////////////
    Size TextRenderer2D::getSymbolsCount()
    {
        return TextHelper::GetSymbolsCountUTF8(m_text);
    }

    //////////////////////////////////////////
    void TextRenderer2D::setStyles(U32 _style)
    {
        if (m_styles == _style)
            return;

        m_styles = _style;

        enableFlag(Flags::MeshDataDirty);
    }

    //////////////////////////////////////////
    void TextRenderer2D::setFontMaterialRef(FontMaterialAssetRef const& _fontMaterial)
    {
        if (m_fontMaterialRef.getFontMaterial() == _fontMaterial.getFontMaterial())
            return;

        if (getFontMaterial())
        {
            getFontMaterial()->eventTexturesChanged.unsubscribe(this, &TextRenderer2D::notifyFontMaterialTexturesChanged);
            getFontMaterial()->eventMaterialChanged.unsubscribe(this, &TextRenderer2D::notifyFontMaterialMaterialChanged);
        }

        m_fontMaterialRef.setFontMaterial(_fontMaterial.getFontMaterial());

        if (getFontMaterial())
        {
            getFontMaterial()->eventTexturesChanged.subscribe(this, &TextRenderer2D::notifyFontMaterialTexturesChanged);
            getFontMaterial()->eventMaterialChanged.subscribe(this, &TextRenderer2D::notifyFontMaterialMaterialChanged);
        }

        enableFlag(Flags::MeshDataDirty);
        enableFlag(Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    Vec2F TextRenderer2D::getTextEnd(Size _rowIndex)
    {
        // Apply pending changes so the result reflects the actual state
        prepareForRender();

        FontMaterialPtr const& fontMaterial = getFontMaterial();
        if (!fontMaterial || !fontMaterial->getFont() || !fontMaterial->getFont()->getDefaultFont())
            return Vec2F::c_zero;

        if (m_text.empty())
        {
            F32 linespace = fontMaterial->getFont()->getLineSpacing(m_fontSize) * getLineSpacingScale();

            F32 y = calculateY(linespace, 1);
            m_lastGlyphOffset = Vec2F(0.0f, y);
        }

        return m_lastGlyphOffset;
    }

    //////////////////////////////////////////
    void TextRenderer2D::calculateSymbolSlots(
        Vector<Pair<S32, F32>>& _outSlots,
        Vector<F32>& _outRowLengths)
    {
        _outSlots.clear();
        _outRowLengths.clear();

        FontMaterialPtr const& fontMaterial = getFontMaterial();
        Font* font = fontMaterial ? fontMaterial->getFont().get() : nullptr;
        if (!font || !font->getDefaultFont())
        {
            _outSlots.emplace_back(Pair<S32, F32>(0, 0.0f));
            _outRowLengths.push_back(0.0f);
            return;
        }

        ensureAllGlyphs();

        TrueTypeFontPtr const& defaultFont = font->getDefaultFont();

        String finalText = m_text;

        // Keep the text transforms in sync with updateMeshDataNow
        if (getColorTags())
        {
            Deque<Pair<Size, ColorF128>> colorTags;
            TextRenderer2DHelper::ExtractColorTags(
                finalText,
                ColorF128(m_color.toVec4F32()),
                colorTags);
        }
        TextRenderer2DHelper::ApplySymbolsLimitPolicy(finalText, m_symbolsLimit, m_symbolsLimitPolicy);

        FontGlyphStorageData* glyphStorageData = nullptr;
        auto ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);

        F32 hSpace = static_cast<F32>(defaultFont->ensureGlyph(L' ', m_fontSize, m_bold, *ttfPage).advance);

        S32 row = 0;
        F32 x = 0.0f;

        U32 prevChar = 0;
        U32 curChar = 0;
        String::iterator it = finalText.begin();
        String::iterator end = finalText.end();

        while ((it != end) && (curChar = utf8::next(it, end)))
        {
            // #TODO: Rework - take kerning from the current glyph storage
            F32 kerning = font->getKerning(prevChar, curChar, m_fontSize);
            x += kerning;
            prevChar = curChar;

            _outSlots.emplace_back(Pair<S32, F32>(row, x));

            if (curChar == ' ')
            {
                x += hSpace;
                continue;
            }
            if (curChar == '\t')
            {
                x += hSpace * 4;
                continue;
            }
            if (curChar == '\n')
            {
                _outRowLengths.push_back(x);

                ++row;
                x = 0.0f;
                continue;
            }

            FontGlyphStorageData* curGlyphStorageData = font->getGlyphStorageData(curChar);
            if (glyphStorageData != curGlyphStorageData)
            {
                glyphStorageData = curGlyphStorageData;

                if (glyphStorageData && glyphStorageData->getTrueTypeFont())
                    ttfPage = &glyphStorageData->getTrueTypeFont()->ensureTTFPage(m_fontSize, m_bold);
                else
                    ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);
            }

            FontGlyph const& glyph = font->ensureGlyphFromStorage(glyphStorageData, curChar, m_fontSize, m_bold, *ttfPage);
            x += glyph.advance;

            if (glyphStorageData &&
                (glyphStorageData->type == FontGlyphStorageType::Sprite ||
                 glyphStorageData->type == FontGlyphStorageType::Entity))
                x = Math::Round(x);
        }

        _outRowLengths.push_back(x);
        _outSlots.emplace_back(Pair<S32, F32>(row, x));
    }

    //////////////////////////////////////////
    Vec2F TextRenderer2D::getSymbolPosition(Size _symbolIndex)
    {
        // Apply pending changes so the result reflects the actual state
        prepareForRender();

        FontMaterialPtr const& fontMaterial = getFontMaterial();
        if (!fontMaterial || !fontMaterial->getFont() || !fontMaterial->getFont()->getDefaultFont() || !m_transform)
            return Vec2F::c_zero;

        Vector<Pair<S32, F32>> slots;
        Vector<F32> rowLengths;
        calculateSymbolSlots(slots, rowLengths);

        if (_symbolIndex >= slots.size())
            _symbolIndex = slots.size() - 1;

        S32 row = slots[_symbolIndex].first;
        F32 x = slots[_symbolIndex].second;

        Vec2F const& size = m_transform->getSize();
        F32 vSpace = fontMaterial->getFont()->getLineSpacing(m_fontSize) * getLineSpacingScale();
        Size rowsCount = rowLengths.size();

        F32 xAlignOffset = TextRenderer2DHelper::CalculateXAlignOffset(
            m_widthPolicy, m_horizontalAlignment, size.x, rowLengths[row]);
        if (m_pixelPerfect)
            xAlignOffset = Math::Round(xAlignOffset);

        F32 y = calculateY((F32)rowsCount * vSpace, rowsCount) - (F32)row * vSpace;

        return Vec2F(x + xAlignOffset + m_outlineThickness, y);
    }

    //////////////////////////////////////////
    Size TextRenderer2D::getSymbolIndexAtPosition(Vec2F const& _position)
    {
        // Apply pending changes so the result reflects the actual state
        prepareForRender();

        FontMaterialPtr const& fontMaterial = getFontMaterial();
        if (!fontMaterial || !fontMaterial->getFont() || !fontMaterial->getFont()->getDefaultFont() || !m_transform || m_text.empty())
            return m_text.size();

        Vector<Pair<S32, F32>> slots;
        Vector<F32> rowLengths;
        calculateSymbolSlots(slots, rowLengths);

        Vec2F const& size = m_transform->getSize();
        F32 vSpace = fontMaterial->getFont()->getLineSpacing(m_fontSize) * getLineSpacingScale();
        Size rowsCount = rowLengths.size();

        F32 y0 = calculateY((F32)rowsCount * vSpace, rowsCount);
        S32 row = (S32)Math::Round((y0 - _position.y) / vSpace);
        row = Math::Clamp(row, 0, (S32)rowsCount - 1);

        F32 xAlignOffset = TextRenderer2DHelper::CalculateXAlignOffset(
            m_widthPolicy, m_horizontalAlignment, size.x, rowLengths[row]);
        if (m_pixelPerfect)
            xAlignOffset = Math::Round(xAlignOffset);

        F32 localX = _position.x - xAlignOffset;

        Size bestIndex = slots.size() - 1;
        F32 bestDistance = 1e37f;
        for (Size i = 0, in = slots.size(); i < in; ++i)
        {
            if (slots[i].first != row)
                continue;

            F32 distance = Math::Abs(slots[i].second - localX);
            if (distance < bestDistance)
            {
                bestDistance = distance;
                bestIndex = i;
            }
        }

        return bestIndex;
    }

    //////////////////////////////////////////
    F32 TextRenderer2D::calculateY(
        F32 _totalTextHeight,
        Size _actualRowsCount)
    {
        FontMaterialPtr const& fontMaterial = getFontMaterial();
        if (!m_transform || !fontMaterial)
            return 0.0f;

        FontPtr const& font = fontMaterial->getFont();

        F32 ascent = font->getDefaultFont()->getAscender(m_fontSize);
        F32 descent = font->getDefaultFont()->getDescender(m_fontSize);
        F32 linespace = font->getLineSpacing(m_fontSize) * getLineSpacingScale();

        F32 y = TextRenderer2DHelper::CalculateTextPositionY(
            m_verticalAlignment,
            m_transform->getSize().y,
            ascent,
            descent,
            linespace,
            _totalTextHeight,
            _actualRowsCount);

        return m_pixelPerfect ? Math::Round(y) : y;
    }

    //////////////////////////////////////////
    void TextRenderer2D::ensureAllGlyphs()
    {
        if (!m_meshRenderer)
            return;

        FontMaterialPtr const& fontMaterial = getFontMaterial();
        if (!fontMaterial || !fontMaterial->getFont())
            return;

        FontPtr const& font = fontMaterial->getFont();
        TrueTypeFontPtr const& defaultFont = font->getDefaultFont();
        if (!defaultFont)
            return;

        auto ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);
        auto ttfOutlineThicknessPage = &defaultFont->ensureTTFOutlineThicknessPage(m_fontSize, m_bold, m_outlineThickness);

        defaultFont->ensureGlyph(L'x', m_fontSize, m_bold, *ttfPage);
        defaultFont->ensureGlyph(L' ', m_fontSize, m_bold, *ttfPage);
        defaultFont->ensureGlyph(L'.', m_fontSize, m_bold, *ttfPage);

        U32 curChar = 0;
        String::iterator it = m_text.begin();
        String::iterator end = m_text.end();

        while ((it != end) && (curChar = utf8::next(it, end)))
        {
            if (curChar == ' ' || curChar == '\t' || curChar == '\n')
                continue;

            auto glyphStorageData = font->getGlyphStorageData(curChar);
            if (glyphStorageData)
            {
                if (glyphStorageData->getTrueTypeFont())
                {
                    ttfPage = &glyphStorageData->getTrueTypeFont()->ensureTTFPage(m_fontSize, m_bold);

                    if (m_outlineThickness)
                        ttfOutlineThicknessPage = &glyphStorageData->getTrueTypeFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_bold, m_outlineThickness);
                }
            }
            else
            {
                ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);

                if (m_outlineThickness)
                    ttfOutlineThicknessPage = &defaultFont->ensureTTFOutlineThicknessPage(m_fontSize, m_bold, m_outlineThickness);
            }

            font->ensureGlyphFromStorage(glyphStorageData, curChar, m_fontSize, m_bold, *ttfPage);

            if (m_outlineThickness)
                font->ensureOutlinedGlyphFromStorage(glyphStorageData, curChar, m_fontSize, m_bold, m_outlineThickness, *ttfOutlineThicknessPage);
        }
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateMeshDataNow()
    {
        if (!m_canvasRenderer || !m_meshRenderer || !m_transform)
            return;

        ensureAllGlyphs();

        m_boundingSize = Vec2F::c_zero;

        FontMaterialPtr const& fontMaterial = getFontMaterial();
        Font* font = fontMaterial ? fontMaterial->getFont().get() : nullptr;

        if (!font || !font->getDefaultFont() || m_text.empty())
        {
            m_meshRenderer->resize(0);
            m_localMatrices.clear();
            m_localColors.clear();
            m_outlineQuadsCount = 0u;
            m_hasActiveColorTags = false;

            disableFlag(Flags::MeshDataDirty);
            disableFlag(Flags::GlyphColorsDirty);
            return;
        }

        TrueTypeFontPtr const& defaultFont = font->getDefaultFont();
        Vec2F const& size = m_transform->getSize();

        String finalText = m_text;

        // Process color tags
        Deque<Pair<Size, ColorF128>> colorTags;
        m_hasActiveColorTags = false;
        if (getColorTags())
        {
            m_hasActiveColorTags = TextRenderer2DHelper::ExtractColorTags(
                finalText,
                ColorF128(m_color.toVec4F32()),
                colorTags);
        }

        // Process symbols limit policy
        TextRenderer2DHelper::ApplySymbolsLimitPolicy(finalText, m_symbolsLimit, m_symbolsLimitPolicy);

        FontGlyphStorageData* glyphStorageData = nullptr;
        auto ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);
        auto ttfOutlineThicknessPage = &defaultFont->ensureTTFOutlineThicknessPage(m_fontSize, m_bold, m_outlineThickness);

        F32 linespace = font->getLineSpacing(m_fontSize) * getLineSpacingScale();

        F32 hSpace = static_cast<F32>(defaultFont->ensureGlyph(L' ', m_fontSize, m_bold, *ttfPage).advance);
        F32 vSpace = linespace;


        Vector<TempGlyphData> glyphs;
        glyphs.reserve(TextHelper::GetSymbolsCountUTF8(finalText));

        Size quadsCount = 0u;
        Size outlineQuadsCount = 0u;

        FastVector<F32> rowLengths;
        F32 rowLengthMax = 0.0f;
        {
            F32 x = 0.0f;

            U32 prevChar = 0;
            U32 curChar = 0;
            String::iterator it = finalText.begin();
            String::iterator end = finalText.end();

            while ((it != end) && (curChar = utf8::next(it, end)))
            {
                // #TODO: Rework - take kerning from the current glyph storage
                F32 kerning = font->getKerning(prevChar, curChar, m_fontSize);
                x += kerning;
                prevChar = curChar;

                if (curChar == ' ')
                {
                    x += hSpace;
                    continue;
                }
                if (curChar == '\t')
                {
                    x += hSpace * 4;
                    continue;
                }
                if (curChar == '\n')
                {
                    F32 totalLength = x;
                    rowLengths.push_back(totalLength);
                    rowLengthMax = Math::Max(rowLengthMax, totalLength);

                    x = 0;
                    continue;
                }

                TempGlyphData glyphData;

                glyphData.glyphStorageData = font->getGlyphStorageData(curChar);
                if (glyphStorageData != glyphData.glyphStorageData)
                {
                    glyphStorageData = glyphData.glyphStorageData;

                    if (glyphStorageData)
                    {
                        if (glyphStorageData->getTrueTypeFont())
                        {
                            ttfPage = &glyphStorageData->getTrueTypeFont()->ensureTTFPage(m_fontSize, m_bold);

                            if (m_outlineThickness)
                                ttfOutlineThicknessPage = &glyphStorageData->getTrueTypeFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_bold, m_outlineThickness);
                        }
                    }
                    else
                    {
                        ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);

                        if (m_outlineThickness)
                            ttfOutlineThicknessPage = &defaultFont->ensureTTFOutlineThicknessPage(m_fontSize, m_bold, m_outlineThickness);
                    }
                }
                glyphData.glyph = &(font->ensureGlyphFromStorage(glyphStorageData, curChar, m_fontSize, m_bold, *ttfPage));
                if (glyphData.glyph->bounds.size.x > 0.0 && glyphData.glyph->bounds.size.y > 0.0)
                {

                    // #TODO: Entities size here
                    /*
                    if (glyphStorageData &&
                        glyphStorageData->type == FontGlyphStorageType::Entity &&
                        scaleEntitiesToFit &&
                        (-glyphData.glyph->bounds.position.y > (F32)m_fontSize))
                    {
                        F32 scaleCoef = (F32)m_fontSize / (-glyphData.glyph->bounds.position.y);
                        glyphData.glyph = &(m_fontMaterial->getFont()->getGlyphFromStorage(glyphStorageData, curChar, U32(m_fontSize * scaleCoef)));
                    }
                    */

                    if (m_outlineThickness)
                        glyphData.outlineThicknessGlyph = &(font->ensureOutlinedGlyphFromStorage(
                            glyphStorageData, curChar, m_fontSize, m_bold, m_outlineThickness, *ttfOutlineThicknessPage));


                    if (glyphData.glyphStorageData)
                    {
                        switch (glyphData.glyphStorageData->type)
                        {
                            case FontGlyphStorageType::TrueTypeFont:
                                ++quadsCount;
                                if (m_outlineThickness)
                                    ++outlineQuadsCount;
                                break;
                            case FontGlyphStorageType::Sprite:
                                ++quadsCount;
                                break;
                            default:
                                break;
                        }
                    }
                    else
                    {
                        ++quadsCount;
                        if (m_outlineThickness)
                            ++outlineQuadsCount;
                    }
                }

                glyphs.emplace_back(glyphData);
                x += glyphData.glyph->advance;
            }
            F32 totalLength = x;
            rowLengths.push_back(totalLength);
            rowLengthMax = Math::Max(rowLengthMax, totalLength);
        }

        Size actualRowsCount = rowLengths.size();

        F32 totalTextHeight = (F32)actualRowsCount * vSpace;

        F32 x = 0.0f;
        F32 y = calculateY(totalTextHeight, actualRowsCount);

        F32 glyphX = x;
        F32 glyphY = y;

        U32 currentRow = 0;
        U32 prevChar = 0;
        U32 curChar = 0;
        U32 curQuadIndex = 0;
        U32 curOutlineQuadIndex = 0;

        String::iterator finalTextBegin = finalText.begin();
        String::iterator it = finalText.begin();
        String::iterator end = finalText.end();

        ColorF128 currentGlyphColor = ColorF128(m_color.toVec4F32());

        Size curCharOffset = 0;
        F32 xAlignOffset = TextRenderer2DHelper::CalculateXAlignOffset(
            m_widthPolicy, m_horizontalAlignment, size.x, rowLengths[currentRow]);
        if (m_pixelPerfect)
            xAlignOffset = Math::Round(xAlignOffset);

        S32 glyphIndex = 0;

        Size totalQuadsCount = quadsCount + outlineQuadsCount;
        m_meshRenderer->resize((S32)totalQuadsCount);
        m_localMatrices.resize(totalQuadsCount);
        m_localColors.resize(totalQuadsCount);
        m_outlineQuadsCount = outlineQuadsCount;

        // Build mesh
        while ((it != end) && (curChar = utf8::next(it, end)))
        {
            // #TODO: Rework - take kerning from the current glyph storage
            F32 kerning = font->getKerning(prevChar, curChar, m_fontSize);
            x += kerning;
            prevChar = curChar;

            if (!colorTags.empty())
            {
                Pair<Size, ColorF128> colorTag = colorTags.front();
                while (curCharOffset == colorTag.first)
                {
                    currentGlyphColor = colorTag.second;

                    colorTags.pop_front();
                    if (colorTags.empty())
                        break;

                    colorTag = colorTags.front();
                }
            }

            if (curChar == ' ')
            {
                x += hSpace;
                curCharOffset = it - finalTextBegin;
                continue;
            }
            if (curChar == '\t')
            {
                x += hSpace * 4;
                curCharOffset = it - finalTextBegin;
                continue;
            }
            if (curChar == '\n')
            {
                ++currentRow;
                xAlignOffset = TextRenderer2DHelper::CalculateXAlignOffset(
                    m_widthPolicy, m_horizontalAlignment, size.x, rowLengths[currentRow]);
                if (m_pixelPerfect)
                    xAlignOffset = Math::Round(xAlignOffset);

                y -= vSpace;
                x = 0;
                curCharOffset = it - finalTextBegin;
                continue;
            }

            glyphX = x + xAlignOffset;
            glyphY = y;

            TempGlyphData& glyphData = glyphs[glyphIndex];
            ++glyphIndex;


            if (glyphData.glyphStorageData && glyphData.glyphStorageData->type == FontGlyphStorageType::Entity)
            {
                EntityPtr const& prefab = glyphData.glyphStorageData->entityData.getPrefab();

                if (prefab)
                {
                    // #TODO: Entity prefabs
                    /*
                    SceneObject2DPtr prefabCopy = prefab->createCopy();
                    prefabCopy->setPosition(glyphX + glyphData.glyph->bounds.position.x, glyphY + glyphData.glyph->bounds.position.y);
                    prefabCopy->setSize(glyphData.glyph->bounds.size);
                    addChild(prefabCopy);
                    m_prefabGlyphs.push_back(prefabCopy);
                    */
                }
            }
            else
            {
                if (glyphData.glyph->bounds.size.x > 0.0 && glyphData.glyph->bounds.size.y > 0.0)
                {
                    if (m_outlineThickness)
                    {
                        if (!glyphData.glyphStorageData || glyphData.glyphStorageData->type == FontGlyphStorageType::TrueTypeFont)
                        {
                            setGlyphQuad(curOutlineQuadIndex, Vec2F(glyphX, glyphY), m_outlineColor, (*glyphData.outlineThicknessGlyph));
                            ++curOutlineQuadIndex;
                        }
                    }

                    setGlyphQuad(outlineQuadsCount + curQuadIndex, Vec2F(glyphX, glyphY), currentGlyphColor, (*glyphData.glyph));
                    ++curQuadIndex;
                }
            }

            x += glyphData.glyph->advance;

            if (glyphData.glyphStorageData &&
                (glyphData.glyphStorageData->type == FontGlyphStorageType::Sprite ||
                 glyphData.glyphStorageData->type == FontGlyphStorageType::Entity))
                x = Math::Round(x);

            curCharOffset = it - finalTextBegin;
        }

        m_lastGlyphOffset.x = x + xAlignOffset + m_outlineThickness;
        m_lastGlyphOffset.y = y;

        m_boundingSize.x = rowLengthMax;
        m_boundingSize.y = totalTextHeight;

        disableFlag(Flags::MeshDataDirty);
        disableFlag(Flags::GlyphColorsDirty);
        enableFlag(Flags::ModelMatricesDirty);
        enableFlag(Flags::ColorDirty);
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateMaterial()
    {
        if (!m_meshRenderer)
            return;

        if (getFontMaterial())
            m_meshRenderer->setMaterial(getFontMaterial()->fetchMaterial(m_fontSize, m_bold));
        else
            m_meshRenderer->setMaterial(MaterialPtr());

        disableFlag(Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateGlyphColors()
    {
        disableFlag(Flags::GlyphColorsDirty);

        Size quadsCount = m_localColors.size();
        Size outlineQuadsCount = Math::Min(m_outlineQuadsCount, quadsCount);

        Vec4F outlineColor = m_outlineColor.toVec4F32();
        for (Size i = 0; i < outlineQuadsCount; ++i)
            m_localColors[i] = outlineColor;

        // Tagged glyph colors can't be restored without a relayout,
        // so only rewrite them when no color tags are active
        if (!m_hasActiveColorTags)
        {
            Vec4F color = m_color.toVec4F32();
            for (Size i = outlineQuadsCount; i < quadsCount; ++i)
                m_localColors[i] = color;
        }

        enableFlag(Flags::ColorDirty);
    }

    //////////////////////////////////////////
    void TextRenderer2D::setGlyphQuad(
        Size _quadIndex,
        Vec2F const& _position,
        ColorF128 const& _color,
        FontGlyph const& _glyph)
    {

        if (!_glyph.texture)
            return;

        S32 textureIndex = getFontMaterial()->getTextureIndex(m_fontSize, m_bold, _glyph.texture.get());
        MAZE_ERROR_RETURN_IF(textureIndex == -1, "Texture index is -1!");

        MAZE_ERROR_RETURN_IF(_quadIndex >= m_localMatrices.size(), "Out of bounds!");
        MAZE_ERROR_RETURN_IF(_quadIndex >= m_localColors.size(), "Out of bounds!");

        Vec2F sizeV = (Vec2F)_glyph.bounds.size;
        Vec2F positionShiftV = _position + _glyph.bounds.position;

        TMat localTransform = TMat::CreateTranslation(positionShiftV).transform(
            TMat::CreateScale(sizeV));
        m_localMatrices[_quadIndex] = localTransform;
        m_localColors[_quadIndex] = _color.toVec4F32();

        // Final colors are pushed by updateMeshRendererColors (ColorDirty pass)

        m_meshRenderer->setUV0(
            _quadIndex,
            Vec4F(
                _glyph.textureCoords.position.x,
                _glyph.textureCoords.position.y,
                _glyph.textureCoords.position.x + _glyph.textureCoords.size.x,
                _glyph.textureCoords.position.y + _glyph.textureCoords.size.y));

        m_meshRenderer->setUV1(
            _quadIndex,
            Vec4F((F32)textureIndex, 0.0f, 0.0f, 0.0f));
    }

    //////////////////////////////////////////
    void TextRenderer2D::notifyFontMaterialTexturesChanged()
    {
        enableFlag(Flags::MeshDataDirty);
    }

    //////////////////////////////////////////
    void TextRenderer2D::notifyFontMaterialMaterialChanged()
    {
        enableFlag(Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    Vec2F TextRenderer2D::calculateRequiredSizeForText(CString _text)
    {
        FontMaterialPtr const& fontMaterial = getFontMaterial();
        if (!fontMaterial || !fontMaterial->getFont() || !fontMaterial->getFont()->getDefaultFont())
            return Vec2F::c_zero;

        if (!_text || !*_text)
            return Vec2F::c_zero;

        FontPtr const& font = fontMaterial->getFont();
        TrueTypeFontPtr const& defaultFont = font->getDefaultFont();

        auto ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);

        F32 hSpace = static_cast<F32>(defaultFont->ensureGlyph(L' ', m_fontSize, m_bold, *ttfPage).advance);
        F32 linespace = font->getLineSpacing(m_fontSize) * getLineSpacingScale();

        F32 rowLengthMax = 0.0f;
        F32 x = 0.0f;
        Size rowCount = 1;

        U32 prevChar = 0;
        U32 curChar = 0;
        CString it = _text;
        CString end = _text + strlen(_text);

        while ((it != end) && (curChar = utf8::next(it, end)))
        {
            x += font->getKerning(prevChar, curChar, m_fontSize);
            prevChar = curChar;

            if (getColorTags() && curChar == '#' && it != end && *it == '{')
            {
                while (it != end && *it != '}')
                    ++it;
                if (it != end)
                    ++it;
                prevChar = 0;
                continue;
            }

            if (curChar == ' ') { x += hSpace; continue; }
            if (curChar == '\t') { x += hSpace * 4; continue; }
            if (curChar == '\n')
            {
                rowLengthMax = Math::Max(rowLengthMax, x);
                x = 0.0f;
                ++rowCount;
                continue;
            }

            FontGlyphStorageData* glyphStorageData = font->getGlyphStorageData(curChar);
            if (glyphStorageData && glyphStorageData->getTrueTypeFont())
                ttfPage = &glyphStorageData->getTrueTypeFont()->ensureTTFPage(m_fontSize, m_bold);
            else
                ttfPage = &defaultFont->ensureTTFPage(m_fontSize, m_bold);

            FontGlyph const& glyph = font->ensureGlyphFromStorage(glyphStorageData, curChar, m_fontSize, m_bold, *ttfPage);
            x += glyph.advance;

            if (glyphStorageData &&
                (glyphStorageData->type == FontGlyphStorageType::Sprite ||
                 glyphStorageData->type == FontGlyphStorageType::Entity))
                x = Math::Round(x);
        }

        rowLengthMax = Math::Max(rowLengthMax, x);

        return Vec2F(rowLengthMax, (F32)rowCount * linespace);
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(TextRenderer2DSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render2DPostUpdateEvent& _event,
        Entity* _entity,
        TextRenderer2D* _textRenderer2D)
    {
        if (_textRenderer2D->getTransform()->isSizeChanged())
            _textRenderer2D->enableFlag(TextRenderer2D::Flags::MeshDataDirty);

        if (_textRenderer2D->getTransform()->isWorldTransformChanged())
            _textRenderer2D->enableFlag(TextRenderer2D::Flags::ModelMatricesDirty);

        if (_textRenderer2D->getCanvasRenderer()->isAlphaDirty())
            _textRenderer2D->enableFlag(TextRenderer2D::Flags::ColorDirty);

        _textRenderer2D->prepareForRender();
    }



} // namespace Maze
//////////////////////////////////////////
