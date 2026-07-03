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
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemTextRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SystemTextRenderer2D, AbstractTextRenderer2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SystemFontPtr, systemFont, SystemFontPtr(), getSystemFont, setSystemFont));

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
    void SystemTextRenderer2D::updateMeshDataNow()
    {
        if (!m_canvasRenderer || !m_meshRenderer || !m_transform)
            return;

        m_boundingSize = Vec2F::c_zero;

        Texture2DPtr const& systemFontTexture = m_systemFont ? m_systemFont->texture : nullptr;
        if (!systemFontTexture || m_text.empty())
        {
            m_meshRenderer->resize(0);
            m_localMatrices.clear();
            m_localColors.clear();

            disableFlag(Flags::MeshDataDirty);
            disableFlag(Flags::GlyphColorsDirty);
            return;
        }

        Vec2F const& size = m_transform->getSize();

        F32 fontScale = (F32)m_fontSize / (F32)(m_systemFont->charSize.x - m_systemFont->outline * 2);

        F32 const rowSize = (F32)m_systemFont->charSize.y;
        F32 const rowAdvance = rowSize * m_lineSpacingScale;

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        SystemFontHelper::CalculateSystemTextData(
            m_text.c_str(),
            rowsCount,
            maxColumnsCount,
            charsCount);

        m_meshRenderer->resize(charsCount);
        m_localMatrices.resize(charsCount);
        m_localColors.assign(charsCount, Vec4F::c_one);

        S32 columnsCount = SystemFontHelper::CalculateSystemTextRowColumns(m_text.c_str());

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

        Char const* p = m_text.c_str();
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

                Vec2F sizeV = (Vec2F)m_systemFont->charSize * fontScale;
                Vec2F positionShiftV = Vec2F(sx, sy) * fontScale + positionShift;

                TMat localTransform = TMat::CreateTranslation(positionShiftV).transform(
                    TMat::CreateScale(sizeV));
                m_localMatrices[charIndex] = localTransform;

                m_meshRenderer->setUV0(charIndex, uv);

                ++charIndex;

                sx += F32(m_systemFont->charSize.x - m_systemFont->outline);
            }

            ++p;
        }

        m_boundingSize.x = (F32(maxColumnsCount) * (m_systemFont->charSize.x - m_systemFont->outline) + m_systemFont->outline) * fontScale;
        m_boundingSize.y = (F32(rowsCount - 1) * (m_systemFont->charSize.y - m_systemFont->outline) * m_lineSpacingScale
            + F32(m_systemFont->charSize.y - m_systemFont->outline) + F32(m_systemFont->outline)) * fontScale;

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
    Vec2F SystemTextRenderer2D::getTextEnd(Size _rowIndex)
    {
        // Apply pending changes so the result reflects the actual state
        prepareForRender();

        if (!m_systemFont || !m_transform)
            return Vec2F::c_zero;

        Vec2F const& size = m_transform->getSize();
        F32 fontScale = (F32)m_fontSize / (F32)(m_systemFont->charSize.x - m_systemFont->outline * 2);

        F32 const rowSize = (F32)m_systemFont->charSize.y;
        F32 const rowAdvance = rowSize * m_lineSpacingScale;

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        SystemFontHelper::CalculateSystemTextData(
            m_text.c_str(),
            rowsCount,
            maxColumnsCount,
            charsCount);

        S32 columnsCount = SystemFontHelper::CalculateSystemTextRowColumns(m_text.c_str());

        F32 sx = 0.0f;
        F32 sy = -rowSize;

        Char const* p = m_text.c_str();
        while (*p != 0 && _rowIndex > 0)
        {
            if (*p == '\n')
            {
                --_rowIndex;

                sx = 0.0f;
                sy -= rowAdvance;

                columnsCount = SystemFontHelper::CalculateSystemTextRowColumns(p + 1);
            }

            ++p;
        }

        Vec2F positionShift = SystemFontHelper::CalculateSystemTextShift(
            m_systemFont,
            m_horizontalAlignment,
            m_verticalAlignment,
            size,
            fontScale,
            m_lineSpacingScale,
            rowsCount,
            columnsCount);

        Vec2F sizeV = (Vec2F)m_systemFont->charSize * fontScale;
        Vec2F positionShiftV = Vec2F(sx, sy) * fontScale + positionShift;

        return Vec2F((F32)columnsCount, 0.0f) * sizeV + positionShiftV;
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

        _textRenderer2D->prepareForRender();
    }

} // namespace Maze
//////////////////////////////////////////
