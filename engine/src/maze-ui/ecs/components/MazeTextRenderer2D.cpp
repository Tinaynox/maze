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
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
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
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
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
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, text, String(), getText, setText),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, color, ColorU32::c_white, getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(FontMaterialAssetRef, fontMaterial, FontMaterialAssetRef(), getFontMaterialRef, setFontMaterialRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U32, fontSize, 32, getFontSize, setFontSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(HorizontalAlignment2D, horizontalAlignment, HorizontalAlignment2D::Left, getHorizontalAlignment, setHorizontalAlignment),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(VerticalAlignment2D, verticalAlignment, VerticalAlignment2D::Top, getVerticalAlignment, setVerticalAlignment),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextRenderer2DWidthPolicy, widthPolicy, TextRenderer2DWidthPolicy::None, getWidthPolicy, setWidthPolicy),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, outlineThickness, 0.0f, getOutlineThickness, setOutlineThickness),
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
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        if (!AbstractTextRenderer2D::init(
            _component,
            _world,
            _copyData))
            return false;

        updateMeshData();

        return true;
    }

    //////////////////////////////////////////
    void TextRenderer2D::setText(String const& _text)
    {
        if (m_text == _text)
            return;

        m_text = _text;

        updateMeshData();
    }

    //////////////////////////////////////////
    void TextRenderer2D::removeSymbol()
    {
        if (getSymbolsCount() == 0)
            return;

        m_text = TextHelper::PopOneSymbolUTF8(m_text);

        updateMeshData();
    }

    //////////////////////////////////////////
    Size TextRenderer2D::getSymbolsCount()
    {
        return TextHelper::GetSymbolsCountUTF8(m_text);
    }
    
    //////////////////////////////////////////
    void TextRenderer2D::setColor(ColorU32 _color)
    {
        if (m_color == _color)
            return;

        m_color = _color;

        updateMeshData(); // #TODO: Rework
    }

    //////////////////////////////////////////
    void TextRenderer2D::setStyles(U32 _style)
    {
        if (m_styles == _style)
            return;

        m_styles = _style;

        updateMeshData();
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

        updateMeshData();
        updateMaterial();
    }

    //////////////////////////////////////////
    Vec2F TextRenderer2D::getTextEnd(Size _rowIndex)
    {
        if (!getFontMaterial())
            m_lastGlyphOffset = Vec2F(0.0f, 0.0f);

        if (m_text.empty())
        {
            F32 linespace = getFontMaterial()->getFont()->getLineSpacing(m_fontSize) * getLineSpacingScale();

            F32 y = calculateY(linespace, 1);
            m_lastGlyphOffset = Vec2F(0.0f, y);
        }

        return m_lastGlyphOffset;
    }

    //////////////////////////////////////////
    F32 TextRenderer2D::calculateY(
        F32 _totalTextHeight,
        Size _actualRowsCount)
    {
        F32 y = 0.0f;

        if (!m_transform || !getFontMaterial())
            return y;

        Vec2F const& size = m_transform->getSize();

        F32 ascent = getFontMaterial()->getFont()->getDefaultFont()->getAscender(m_fontSize);
        F32 descent = getFontMaterial()->getFont()->getDefaultFont()->getDescender(m_fontSize);
        F32 linespace = getFontMaterial()->getFont()->getLineSpacing(m_fontSize) * getLineSpacingScale();

        switch (m_verticalAlignment)
        {
        case VerticalAlignment2D::Top:
            y = size.y - ascent;
            break;
        case VerticalAlignment2D::Middle:
            y = (size.y - _totalTextHeight) * 0.5f - descent + F32(_actualRowsCount - 1) * linespace;
            break;
        case VerticalAlignment2D::Bottom:
            y = -descent + F32(_actualRowsCount - 1) * linespace;
            break;
        default:
            MAZE_NOT_IMPLEMENTED;
            break;
        }
        
        return m_pixelPerfect ? Math::Round(y) : y;
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateMeshData()
    {
        if (m_debugUpdatingMeshData)
            return;

        m_debugUpdatingMeshData = true;
        ensureAllGlyphs();
        updateMaterial();
        updateMeshDataNow();
        m_debugUpdatingMeshData = false;
    }

    //////////////////////////////////////////
    void TextRenderer2D::ensureAllGlyphs()
    {
        if (!m_meshRenderer)
            return;

        if (!getFontMaterial())
            return;

        if (!getFontMaterial()->getFont()->getDefaultFont())
            return;

        auto ttfPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFPage(m_fontSize);
        auto ttfOutlineThicknessPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_outlineThickness);

        getFontMaterial()->getFont()->getDefaultFont()->ensureGlyph(L'x', m_fontSize, *ttfPage);
        getFontMaterial()->getFont()->getDefaultFont()->ensureGlyph(L' ', m_fontSize, *ttfPage);
        getFontMaterial()->getFont()->getDefaultFont()->ensureGlyph(L'.', m_fontSize, *ttfPage);

        U32 prevChar = 0;
        U32 curChar = 0;
        String::iterator it = m_text.begin();
        String::iterator end = m_text.end();

        while ((it != end) && (curChar = utf8::next(it, end)))
        {
            if (curChar == ' ' || curChar == '\t' || curChar == '\n')
                continue;

            auto glyphStorageData = getFontMaterial()->getFont()->getGlyphStorageData(curChar);
            if (glyphStorageData)
            {
                if (glyphStorageData->getTrueTypeFont())
                {
                    ttfPage = &glyphStorageData->getTrueTypeFont()->ensureTTFPage(m_fontSize);

                    if (m_outlineThickness)
                        ttfOutlineThicknessPage = &glyphStorageData->getTrueTypeFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_outlineThickness);
                }
            }
            else
            {
                ttfPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFPage(m_fontSize);

                if (m_outlineThickness)
                    ttfOutlineThicknessPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_outlineThickness);
            }
           
            getFontMaterial()->getFont()->ensureGlyphFromStorage(glyphStorageData, curChar, m_fontSize, *ttfPage);

            if (m_outlineThickness)
                getFontMaterial()->getFont()->ensureOutlinedGlyphFromStorage(glyphStorageData, curChar, m_fontSize, m_outlineThickness, *ttfOutlineThicknessPage);
        }
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateMeshDataNow()
    {
        m_boundingSize = Vec2F::c_zero;

        if (!m_canvasRenderer)
            return;
        
        if (!m_meshRenderer)
            return;

        Vec2F const& size = m_transform->getSize();

        if (!getFontMaterial() || !getFontMaterial()->getFont() || !getFontMaterial()->getFont()->getDefaultFont() || m_text.empty())
        {
            m_meshRenderer->resize(0);
            m_localMatrices.clear();
            m_localColors.clear();
            return;
        }

        String finalText = m_text;

        // Process color tags
        Deque<Pair<Size, ColorF128>> colorTags;
        if (getColorTags())
        {
            Size startTagPosition = finalText.find("#{");
            Size endTagPosition = finalText.find('}', startTagPosition);
            while (startTagPosition != String::npos && endTagPosition != String::npos)
            {
                String colorString = finalText.substr(startTagPosition + 2, endTagPosition - startTagPosition - 2);
                ColorF128 color;

                if (colorString == "-")
                    color = getColor();
                else
                    color = ColorU32::FromString(colorString, ',');

                finalText.erase(startTagPosition, endTagPosition - startTagPosition + 1);
                colorTags.push_back(std::make_pair(startTagPosition, color));

                startTagPosition = finalText.find("#{");
                endTagPosition = finalText.find('}', startTagPosition);
            }
        }

        // Process symbols limit policy
        if ((m_symbolsLimit > 0) && (m_symbolsLimitPolicy != TextRenderer2DSymbolsLimitPolicy::None))
        {
            bool textCropped = false;

            while (TextHelper::GetSymbolsCountUTF8(finalText) > m_symbolsLimit)
            {
                finalText = TextHelper::PopOneSymbolUTF8(finalText);
                textCropped = true;
            }

            switch (m_symbolsLimitPolicy.toEnum())
            {
                case TextRenderer2DSymbolsLimitPolicy::Dot:
                {
                    if (textCropped)
                    {
                        finalText = TextHelper::PopOneSymbolUTF8(finalText);
                        finalText += ".";
                    }

                    break;
                }
                case TextRenderer2DSymbolsLimitPolicy::ThreeDots:
                {
                    if (textCropped)
                    {
                        finalText = TextHelper::PopOneSymbolUTF8(finalText);
                        finalText = TextHelper::PopOneSymbolUTF8(finalText);
                        finalText = TextHelper::PopOneSymbolUTF8(finalText);
                        finalText += "...";
                    }

                    break;
                }

                default:
                    break;
            }
        }

        FontGlyphStorageData* glyphStorageData = nullptr;
        auto ttfPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFPage(m_fontSize);
        auto ttfOutlineThicknessPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_outlineThickness);

        F32 ascent = getFontMaterial()->getFont()->getDefaultFont()->getAscender(m_fontSize);
        F32 descent = getFontMaterial()->getFont()->getDefaultFont()->getDescender(m_fontSize);
        F32 linespace = getFontMaterial()->getFont()->getLineSpacing(m_fontSize) * getLineSpacingScale();

        Rect2DF xBounds = getFontMaterial()->getFont()->getDefaultFont()->ensureGlyph(L'x', m_fontSize, *ttfPage).bounds;

        F32 hSpace = static_cast<F32>(getFontMaterial()->getFont()->getDefaultFont()->ensureGlyph(L' ', m_fontSize, *ttfPage).advance);
        F32 vSpace = linespace;

        

        Vector<TempGlyphData> glyphs;
        glyphs.reserve(getSymbolsCount());

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
                F32 kerning = getFontMaterial()->getFont()->getKerning(prevChar, curChar, m_fontSize);
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

                glyphData.glyphStorageData = getFontMaterial()->getFont()->getGlyphStorageData(curChar);
                if (glyphStorageData != glyphData.glyphStorageData)
                {
                    glyphStorageData = glyphData.glyphStorageData;

                    if (glyphStorageData)
                    {
                        if (glyphStorageData->getTrueTypeFont())
                        {
                            ttfPage = &glyphStorageData->getTrueTypeFont()->ensureTTFPage(m_fontSize);

                            if (m_outlineThickness)
                            {
                                ttfOutlineThicknessPage = &glyphStorageData->getTrueTypeFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_outlineThickness);
                            }
                        }
                    }
                    else
                    {
                        ttfPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFPage(m_fontSize);

                        if (m_outlineThickness)
                            ttfOutlineThicknessPage = &getFontMaterial()->getFont()->getDefaultFont()->ensureTTFOutlineThicknessPage(m_fontSize, m_outlineThickness);
                    }
                }
                glyphData.glyph = &(getFontMaterial()->getFont()->ensureGlyphFromStorage(glyphStorageData, curChar, m_fontSize, *ttfPage));
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
                        glyphData.outlineThicknessGlyph = &(getFontMaterial()->getFont()->ensureOutlinedGlyphFromStorage(
                            glyphStorageData, curChar, m_fontSize, m_outlineThickness, *ttfOutlineThicknessPage));

                
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
        F32 xAlignOffset = calculateXAlignOffset(rowLengths, currentRow);
        if (m_pixelPerfect)
            xAlignOffset = Math::Round(xAlignOffset);

        S32 glyphIndex = 0;

        Size totalQuadsCount = quadsCount + outlineQuadsCount;
        m_meshRenderer->resize((S32)totalQuadsCount);
        m_localMatrices.resize(totalQuadsCount);
        m_localColors.resize(totalQuadsCount);

        // Build mesh
        while ((it != end) && (curChar = utf8::next(it, end)))
        {
            // #TODO: Rework - take kerning from the current glyph storage
            F32 kerning = getFontMaterial()->getFont()->getKerning(prevChar, curChar, m_fontSize);
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
                xAlignOffset = calculateXAlignOffset(rowLengths, currentRow);
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

        updateMeshRendererModelMatrices();
        updateMeshRendererColors();
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateMaterial()
    {
        if (!m_meshRenderer)
            return;

        if (getFontMaterial())
            m_meshRenderer->setMaterial(getFontMaterial()->fetchMaterial(m_fontSize));
        else
            m_meshRenderer->setMaterial(MaterialPtr());
    }

    //////////////////////////////////////////
    void TextRenderer2D::processEntityAwakened()
    {
        AbstractTextRenderer2D::processEntityAwakened();

        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRendererInstanced>();
        m_meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultQuadNullPivotMesh());

        updateMeshData();
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateMeshRendererColors()
    {
        if (!m_meshRenderer)
            return;

        Vec4F const vertexColor = Vec4F(1.0f, 1.0f, 1.0f, m_canvasRenderer ? m_canvasRenderer->getAlpha() : 1.0f);

        Size colorsCount = m_meshRenderer->getColors().size();
        for (Size i = 0; i < colorsCount; ++i)
            m_meshRenderer->setColor(i, m_localColors[i] * vertexColor);
    }

    //////////////////////////////////////////
    void TextRenderer2D::updateMeshRendererModelMatrices()
    {
        if (!m_meshRenderer)
            return;

        Size transformCount = m_meshRenderer->getModelMatrices().size();
        MAZE_DEBUG_ERROR_IF(transformCount != m_localMatrices.size(), "Invalid characters count!");
        for (Size i = 0; i < transformCount; ++i)
        {
            Mat4F tm = m_transform->getWorldTransform().concatenatedAffineCopy(m_localMatrices[i]);
            if (m_pixelPerfect)
                tm.setTranslation(Math::Round(tm.getAffineTranslation2D()));
            m_meshRenderer->setModelMatrix(i, tm);
        }
    }

    //////////////////////////////////////////
    F32 TextRenderer2D::calculateXAlignOffset(FastVector<F32> const& _rowLengths, U32 _currentRow)
    {
        Vec2F const& size = m_transform->getSize();

        if (m_widthPolicy == TextRenderer2DWidthPolicy::Abut)
        {
            switch (m_horizontalAlignment)
            {
                case HorizontalAlignment2D::Left: return Math::Min(0.0f, size.x - _rowLengths[_currentRow]);
                case HorizontalAlignment2D::Right: return Math::Max(0.0f, size.x - _rowLengths[_currentRow]);
                case HorizontalAlignment2D::Center: return (size.x - _rowLengths[_currentRow]) / 2.0f;
                default:
                    MAZE_NOT_IMPLEMENTED;
                    break;
            }
        }
        else
        {
            switch (m_horizontalAlignment)
            {
                case HorizontalAlignment2D::Left: return 0.0f;
                case HorizontalAlignment2D::Right: return size.x - _rowLengths[_currentRow];
                case HorizontalAlignment2D::Center: return (size.x - _rowLengths[_currentRow]) / 2.0f;
                default:
                    MAZE_NOT_IMPLEMENTED;
                    break;
            }
        }

        return 0.0f;
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

        S32 textureIndex = getFontMaterial()->getTextureIndex(m_fontSize, _glyph.texture.get());
        MAZE_ERROR_RETURN_IF(textureIndex == -1, "Texture index is -1!");

        MAZE_ERROR_RETURN_IF(_quadIndex >= m_localMatrices.size(), "Out of bounds!");
        MAZE_ERROR_RETURN_IF(_quadIndex >= m_localColors.size(), "Out of bounds!");
        
        Vec2F sizeV = (Vec2F)_glyph.bounds.size;
        Vec2F positionShiftV = _position + _glyph.bounds.position;

        Mat4F localTransform = Mat4F::CreateTranslationMatrix(positionShiftV) * Mat4F::CreateScaleMatrix(sizeV);
        m_localMatrices[_quadIndex] = localTransform;
        m_localColors[_quadIndex] = _color.toVec4F32();

        Vec4F const vertexColor = Vec4F(1.0f, 1.0f, 1.0f, m_canvasRenderer ? m_canvasRenderer->getAlpha() : 1.0f);

        m_meshRenderer->setColor(_quadIndex, (Vec4F)_color * vertexColor);
        m_meshRenderer->setUV0(
            _quadIndex,
            Vec4F(
                _glyph.textureCoords.position.x,
                _glyph.textureCoords.position.x + _glyph.textureCoords.size.x,
                _glyph.textureCoords.position.y,
                _glyph.textureCoords.position.y + _glyph.textureCoords.size.y));

        m_meshRenderer->setUV1(
            _quadIndex,
            Vec4F((F32)textureIndex, 0.0f, 0.0f, 0.0f));
    }

    //////////////////////////////////////////
    void TextRenderer2D::notifyFontMaterialTexturesChanged()
    {
        // #TODO: dirty flag here
        updateMeshData();
    }

    //////////////////////////////////////////
    void TextRenderer2D::notifyFontMaterialMaterialChanged()
    {
        updateMaterial();
    }
            
    
} // namespace Maze
//////////////////////////////////////////
