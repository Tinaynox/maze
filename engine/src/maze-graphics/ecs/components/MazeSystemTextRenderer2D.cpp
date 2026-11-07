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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/events/MazeEcsGraphicsEvents.hpp"
#include "maze-graphics/helpers/MazeSystemFontHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(TextOverflow2D);


    //////////////////////////////////////////
    // Class SystemTextRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SystemTextRenderer2D, AbstractTextRenderer2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SystemFontPtr, systemFont, SystemFontPtr(), getSystemFont, setSystemFont),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(TextOverflow2D, overflow, TextOverflow2D::None, getOverflow, setOverflow));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SystemTextRenderer2D);


    //////////////////////////////////////////
    SystemTextRenderer2D::SystemTextRenderer2D()
    {

    }

    //////////////////////////////////////////
    SystemTextRenderer2D::~SystemTextRenderer2D()
    {

    }

    //////////////////////////////////////////
    SystemTextRenderer2DPtr SystemTextRenderer2D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        SystemTextRenderer2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SystemTextRenderer2D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SystemTextRenderer2D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool SystemTextRenderer2D::init(
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
    void SystemTextRenderer2D::setSystemFont(SystemFontPtr const& _systemFont)
    {
        if (m_systemFont == _systemFont)
            return;

        m_systemFont = _systemFont;

        enableFlag(Flags::MeshDataDirty);
        enableFlag(Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::setOverflow(TextOverflow2D _value)
    {
        if (m_overflow == _value)
            return;

        m_overflow = _value;

        enableFlag(Flags::MeshDataDirty);
    }

    //////////////////////////////////////////
    Vec2F SystemTextRenderer2D::measureText(CString _text, F32 _fontScale) const
    {
        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        SystemFontHelper::CalculateSystemTextData(_text, rowsCount, maxColumnsCount, charsCount);

        return SystemFontHelper::CalculateSystemTextSize(*m_systemFont, m_lineSpacingScale, rowsCount, maxColumnsCount) * _fontScale;
    }

    //////////////////////////////////////////
    bool SystemTextRenderer2D::isWrapConstrained() const
    {
        return m_overflow == TextOverflow2D::Wrap && !isAutoSizedWidth() && m_transform && m_transform->getSize().x > 0.0f;
    }

    //////////////////////////////////////////
    S32 SystemTextRenderer2D::calculateWrapColumns(F32 _fontScale) const
    {
        return Math::Max(1, SystemFontHelper::CalculateSystemTextColumnsFit(*m_systemFont, _fontScale, m_transform->getSize().x));
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::updateDisplayText()
    {
        m_displayFontScale = m_systemFont ? SystemFontHelper::CalculateSystemFontScale(*m_systemFont, m_fontSize) : 0.0f;

        if (!m_systemFont || !m_transform)
        {
            m_displayText = m_text;
            return;
        }

        Vec2F const& size = m_transform->getSize();

        // Axes driven by autoSize grow with the text instead of constraining it
        bool const constrainWidth = !isAutoSizedWidth();
        bool const constrainHeight = !isAutoSizedHeight();

        S32 const c_unlimited = 0x7FFFFFFF;

        switch (m_overflow)
        {
            case TextOverflow2D::Wrap:
            {
                if (isWrapConstrained())
                    SystemFontHelper::ApplySystemTextWordWrap(m_text.c_str(), calculateWrapColumns(m_displayFontScale), m_displayText);
                else
                    m_displayText = m_text;
                break;
            }

            case TextOverflow2D::Truncate:
            case TextOverflow2D::Ellipsis:
            {
                S32 maxColumns = constrainWidth
                    ? SystemFontHelper::CalculateSystemTextColumnsFit(*m_systemFont, m_displayFontScale, size.x)
                    : c_unlimited;

                // At least one row is kept, a fully hidden label is never what is wanted
                S32 maxRows = constrainHeight
                    ? Math::Max(1, SystemFontHelper::CalculateSystemTextRowsFit(*m_systemFont, m_displayFontScale, m_lineSpacingScale, size.y))
                    : c_unlimited;

                SystemFontHelper::ApplySystemTextTruncate(
                    m_text.c_str(),
                    maxColumns,
                    maxRows,
                    m_overflow == TextOverflow2D::Ellipsis,
                    m_displayText);
                break;
            }

            case TextOverflow2D::Shrink:
            {
                m_displayText = m_text;

                Vec2F naturalSize = measureText(m_text.c_str(), m_displayFontScale);
                F32 shrink = 1.0f;
                if (constrainWidth && naturalSize.x > size.x && naturalSize.x > 0.0f)
                    shrink = Math::Min(shrink, size.x / naturalSize.x);
                if (constrainHeight && naturalSize.y > size.y && naturalSize.y > 0.0f)
                    shrink = Math::Min(shrink, size.y / naturalSize.y);

                m_displayFontScale *= Math::Max(0.0f, shrink);
                break;
            }

            default:
            {
                m_displayText = m_text;
                break;
            }
        }
    }

    //////////////////////////////////////////
    Vec2F SystemTextRenderer2D::calculatePreferredSize()
    {
        if (!m_systemFont)
            return Vec2F::c_zero;

        F32 fontScale = SystemFontHelper::CalculateSystemFontScale(*m_systemFont, m_fontSize);

        if (isWrapConstrained())
        {
            SystemFontHelper::ApplySystemTextWordWrap(m_text.c_str(), calculateWrapColumns(fontScale), m_measureScratch);
            return measureText(m_measureScratch.c_str(), fontScale);
        }

        return measureText(m_text.c_str(), fontScale);
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::updateMeshDataNow()
    {
        if (!m_canvasRenderer || !m_meshRenderer || !m_transform)
            return;

        m_boundingSize = Vec2F::c_zero;

        updateDisplayText();

        Texture2DPtr const& systemFontTexture = m_systemFont ? m_systemFont->texture : nullptr;
        if (!systemFontTexture || m_displayText.empty())
        {
            m_meshRenderer->resize(0);
            m_localMatrices.clear();
            m_localColors.clear();

            disableFlag(Flags::MeshDataDirty);
            disableFlag(Flags::GlyphColorsDirty);
            return;
        }

        Vec2F const& size = m_transform->getSize();
        F32 const fontScale = m_displayFontScale;

        F32 const rowSize = (F32)m_systemFont->charSize.y;
        F32 const rowAdvance = SystemFontHelper::CalculateSystemTextRowAdvance(*m_systemFont, m_lineSpacingScale);
        F32 const columnAdvance = SystemFontHelper::CalculateSystemTextColumnAdvance(*m_systemFont);

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        SystemFontHelper::CalculateSystemTextData(
            m_displayText.c_str(),
            rowsCount,
            maxColumnsCount,
            charsCount);

        m_meshRenderer->resize(charsCount);
        m_localMatrices.resize(charsCount);
        m_localColors.assign(charsCount, Vec4F::c_one);

        S32 columnsCount = SystemFontHelper::CalculateSystemTextRowColumns(m_displayText.c_str());

        F32 sx = 0.0f;
        F32 sy = -rowSize;
        Size charIndex = 0;

        Vec2F positionShift = SystemFontHelper::CalculateSystemTextShift(
            m_systemFont,
            m_horizontalAlignment,
            m_verticalAlignment,
            size,
            fontScale,
            m_lineSpacingScale,
            rowsCount,
            columnsCount);

        Vec2F const glyphSize = (Vec2F)m_systemFont->charSize * fontScale;

        Char const* p = m_displayText.c_str();
        while (*p != 0)
        {
            Char c = *p;

            if (c == '\n')
            {
                sx = 0.0f;
                sy -= rowAdvance;

                columnsCount = SystemFontHelper::CalculateSystemTextRowColumns(p + 1);

                positionShift = SystemFontHelper::CalculateSystemTextShift(
                    m_systemFont,
                    m_horizontalAlignment,
                    m_verticalAlignment,
                    size,
                    fontScale,
                    m_lineSpacingScale,
                    rowsCount,
                    columnsCount);
            }
            else
            {
                S32 ox = (c - 32) % 16;
                S32 oy = 5 - ((c - 32) / 16);

                F32 border = 0.00001f;
                Vec4F uv(
                    (F32(ox * m_systemFont->stroke.x + m_systemFont->offset.x) / (F32)systemFontTexture->getWidth()) + border,
                    (F32(oy * m_systemFont->stroke.y + m_systemFont->offset.y) / (F32)systemFontTexture->getHeight()) + border,
                    (F32(ox * m_systemFont->stroke.x + m_systemFont->charSize.x + m_systemFont->offset.x) / (F32)systemFontTexture->getWidth()) - border,
                    (F32(oy * m_systemFont->stroke.y + m_systemFont->charSize.y + m_systemFont->offset.y) / (F32)systemFontTexture->getHeight()) - border
                );

                Vec2F glyphPosition = Vec2F(sx, sy) * fontScale + positionShift;

                TMat localTransform = TMat::CreateTranslation(glyphPosition).transform(
                    TMat::CreateScale(glyphSize));
                m_localMatrices[charIndex] = localTransform;

                m_meshRenderer->setUV0(charIndex, uv);

                ++charIndex;

                sx += columnAdvance;
            }

            ++p;
        }

        m_boundingSize = SystemFontHelper::CalculateSystemTextSize(*m_systemFont, m_lineSpacingScale, rowsCount, maxColumnsCount) * fontScale;

        disableFlag(Flags::MeshDataDirty);
        disableFlag(Flags::GlyphColorsDirty);
        enableFlag(Flags::ModelMatricesDirty);
        enableFlag(Flags::ColorDirty);
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::updateMaterial()
    {
        if (!m_meshRenderer)
            return;

        if (m_systemFont)
            m_meshRenderer->setMaterial(m_systemFont->material);
        else
            m_meshRenderer->setMaterial(MaterialPtr());

        disableFlag(Flags::MaterialDirty);
    }

    //////////////////////////////////////////
    Vec4F SystemTextRenderer2D::calculateMeshColor() const
    {
        Vec4F meshColor = m_color.toVec4F32();
        if (m_canvasRenderer)
            meshColor.w *= m_canvasRenderer->getAlpha();
        return meshColor;
    }

    //////////////////////////////////////////
    Vec2F SystemTextRenderer2D::calculateRowShift(S32 _rowsCount, S32 _rowColumnsCount) const
    {
        return SystemFontHelper::CalculateSystemTextShift(
            m_systemFont,
            m_horizontalAlignment,
            m_verticalAlignment,
            m_transform->getSize(),
            m_displayFontScale,
            m_lineSpacingScale,
            _rowsCount,
            _rowColumnsCount);
    }

    //////////////////////////////////////////
    Vec2F SystemTextRenderer2D::getTextEnd(Size _rowIndex)
    {
        // Apply pending changes so the result reflects the actual state
        prepareForRender();

        if (!m_systemFont || !m_transform)
            return Vec2F::c_zero;

        F32 const rowAdvance = SystemFontHelper::CalculateSystemTextRowAdvance(*m_systemFont, m_lineSpacingScale);
        F32 const columnAdvance = SystemFontHelper::CalculateSystemTextColumnAdvance(*m_systemFont);

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        SystemFontHelper::CalculateSystemTextData(m_displayText.c_str(), rowsCount, maxColumnsCount, charsCount);

        F32 sy = -(F32)m_systemFont->charSize.y;
        Char const* rowStart = m_displayText.c_str();
        for (Char const* p = rowStart; *p != 0 && _rowIndex > 0; ++p)
        {
            if (*p == '\n')
            {
                --_rowIndex;
                sy -= rowAdvance;
                rowStart = p + 1;
            }
        }

        S32 columnsCount = SystemFontHelper::CalculateSystemTextRowColumns(rowStart);
        Vec2F shift = calculateRowShift(rowsCount, columnsCount);

        return Vec2F(F32(columnsCount) * columnAdvance, sy) * m_displayFontScale + shift;
    }

    //////////////////////////////////////////
    Vec2F SystemTextRenderer2D::getSymbolPosition(Size _symbolIndex)
    {
        // Apply pending changes so the result reflects the actual state
        prepareForRender();

        if (!m_systemFont || !m_transform)
            return Vec2F::c_zero;

        F32 const rowAdvance = SystemFontHelper::CalculateSystemTextRowAdvance(*m_systemFont, m_lineSpacingScale);
        F32 const columnAdvance = SystemFontHelper::CalculateSystemTextColumnAdvance(*m_systemFont);

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        SystemFontHelper::CalculateSystemTextData(m_displayText.c_str(), rowsCount, maxColumnsCount, charsCount);

        S32 caretRow = 0;
        S32 caretColumn = 0;
        Char const* rowStart = m_displayText.c_str();

        Char const* p = m_displayText.c_str();
        for (Size i = 0; *p != 0 && i < _symbolIndex; ++p, ++i)
        {
            if (*p == '\n')
            {
                ++caretRow;
                caretColumn = 0;
                rowStart = p + 1;
            }
            else
                ++caretColumn;
        }

        S32 rowColumnsCount = SystemFontHelper::CalculateSystemTextRowColumns(rowStart);
        Vec2F shift = calculateRowShift(rowsCount, rowColumnsCount);

        F32 sy = -(F32)m_systemFont->charSize.y - (F32)caretRow * rowAdvance;
        return Vec2F(F32(caretColumn) * columnAdvance, sy) * m_displayFontScale + shift;
    }

    //////////////////////////////////////////
    Size SystemTextRenderer2D::getSymbolIndexAtPosition(Vec2F const& _position)
    {
        // Apply pending changes so the result reflects the actual state
        prepareForRender();

        if (!m_systemFont || !m_transform || m_displayText.empty())
            return m_displayText.size();

        F32 const rowAdvance = SystemFontHelper::CalculateSystemTextRowAdvance(*m_systemFont, m_lineSpacingScale) * m_displayFontScale;
        F32 const columnAdvance = SystemFontHelper::CalculateSystemTextColumnAdvance(*m_systemFont) * m_displayFontScale;
        if (rowAdvance <= 0.0f || columnAdvance <= 0.0f)
            return m_displayText.size();

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        SystemFontHelper::CalculateSystemTextData(m_displayText.c_str(), rowsCount, maxColumnsCount, charsCount);

        // The vertical part of the shift is independent from the columns count
        F32 shiftY = calculateRowShift(rowsCount, 0).y;

        S32 row = (S32)Math::Floor((shiftY - _position.y) / rowAdvance);
        row = Math::Clamp(row, 0, rowsCount - 1);

        Size rowStartIndex = 0;
        S32 rowsToSkip = row;
        Char const* p = m_displayText.c_str();
        while (*p != 0 && rowsToSkip > 0)
        {
            if (*p == '\n')
                --rowsToSkip;

            ++p;
            ++rowStartIndex;
        }

        S32 rowColumnsCount = SystemFontHelper::CalculateSystemTextRowColumns(p);
        F32 shiftX = calculateRowShift(rowsCount, rowColumnsCount).x;

        S32 column = (S32)Math::Round((_position.x - shiftX) / columnAdvance);
        column = Math::Clamp(column, 0, rowColumnsCount);

        return rowStartIndex + (Size)column;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SystemTextRenderer2DAdded,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        SystemTextRenderer2D* _textRenderer)
    {
        if (!_textRenderer->getSystemFont())
        {
            _textRenderer->setSystemFont(
                SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault());
        }
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SystemTextRenderer2DPreferredSize2DQuery,
        {},
        {},
        PreferredSize2DQueryEvent& _event,
        Entity* _entity,
        SystemTextRenderer2D* _textRenderer2D)
    {
        _event.answer(_textRenderer2D->calculatePreferredSize());
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SystemTextRenderer2DSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        Render2DPostUpdateEvent& _event,
        Entity* _entity,
        SystemTextRenderer2D* _textRenderer2D)
    {
        if (_textRenderer2D->getTransform()->isSizeChanged())
            _textRenderer2D->enableFlag(AbstractTextRenderer2D::Flags::MeshDataDirty);

        if (_textRenderer2D->getTransform()->isWorldTransformChanged())
            _textRenderer2D->enableFlag(AbstractTextRenderer2D::Flags::ModelMatricesDirty);

        if (_textRenderer2D->getCanvasRenderer()->isAlphaDirty())
            _textRenderer2D->enableFlag(AbstractTextRenderer2D::Flags::ColorDirty);

        _textRenderer2D->applyAutoSize();
        _textRenderer2D->prepareForRender();
    }

} // namespace Maze
//////////////////////////////////////////
