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
#include "maze-graphics/ecs/components/MazeSystemTextRenderer3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeSystemFontManager.hpp"
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
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemTextRenderer3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SystemTextRenderer3D, AbstractTextRenderer3D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, text, String(), getText, setText),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, color, ColorU32::c_white, getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, size, Vec2F::c_zero, getSize, setSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SystemFontPtr, systemFont, SystemFontPtr(), getSystemFont, setSystemFont),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U32, fontSize, 32, getFontSize, setFontSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(HorizontalAlignment2D, horizontalAlignment, HorizontalAlignment2D::Left, getHorizontalAlignment, setHorizontalAlignment),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(VerticalAlignment2D, verticalAlignment, VerticalAlignment2D::Top, getVerticalAlignment, setVerticalAlignment));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SystemTextRenderer3D);


    //////////////////////////////////////////
    SystemTextRenderer3D::SystemTextRenderer3D()
        : m_color(ColorU32::c_white)
        , m_fontSize(32)
        , m_horizontalAlignment(HorizontalAlignment2D::Center)
        , m_verticalAlignment(VerticalAlignment2D::Middle)
    {
        
    }

    //////////////////////////////////////////
    SystemTextRenderer3D::~SystemTextRenderer3D()
    {
        
    }

    //////////////////////////////////////////
    SystemTextRenderer3DPtr SystemTextRenderer3D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        SystemTextRenderer3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SystemTextRenderer3D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SystemTextRenderer3D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool SystemTextRenderer3D::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        if (!AbstractTextRenderer::init(
            _component,
            _copyData))
            return false;

        updateMeshData();

        return true;
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::setText(String const& _text)
    {
        if (m_text == _text)
            return;

        m_text = _text;

        updateMeshData();
    }
    
    //////////////////////////////////////////
    void SystemTextRenderer3D::setColor(ColorU32 _color)
    {
        if (m_color == _color)
            return;

        m_color = _color;

        updateMeshRendererColors();
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::setSystemFont(SystemFontPtr const& _systemFont)
    {
        if (m_systemFont == _systemFont)
            return;

        m_systemFont = _systemFont;

        updateMaterial();
        updateMeshData();
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::updateMeshData()
    {
        if (!m_meshRenderer)
            return;

        if (!m_systemFont)
            return;

        Vec2F const& size = m_size;

        Texture2DPtr const& systemFontTexture = m_systemFont->texture;


        F32 fontScale = (F32)m_fontSize / (F32)(m_systemFont->charSize.x - m_systemFont->outline * 2);

        S32 const rowSize = m_systemFont->charSize.y;

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        calculateTextData(
            rowsCount,
            maxColumnsCount,
            charsCount);

        m_meshRenderer->resize(charsCount);
        m_localMatrices.resize(charsCount);

        S32 columnsCount = 0;
        Char const* p0 = &m_text[0];
        while (*p0 != 0 && *p0 != '\n')
        {
            ++columnsCount;
            ++p0;
        }

        S32 sx = 0;
        S32 sy = -rowSize;
        Size charIndex = 0;

        Char const* p = &m_text[0];
        while (*p != 0)
        {
            Char c = *p;

            if (c == '\n')
            {
                sx = 0;
                sy -= rowSize;

                columnsCount = 0;
                p0 = p + 1;
                while (*p0 != 0 && *p0 != '\n')
                {
                    ++columnsCount;
                    ++p0;
                }
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

                Vec2F positionShift;
                switch (m_horizontalAlignment)
                {
                    case HorizontalAlignment2D::Left: positionShift.x = 0.0f; break;
                    case HorizontalAlignment2D::Center: positionShift.x = (size.x - (columnsCount * (m_systemFont->charSize.x - m_systemFont->outline) + m_systemFont->outline) * fontScale) * 0.5f; break;
                    case HorizontalAlignment2D::Right: positionShift.x = size.x - (columnsCount * (m_systemFont->charSize.x - m_systemFont->outline) + m_systemFont->outline) * fontScale; break;
                    default: break;
                }
                switch (m_verticalAlignment)
                {
                    case VerticalAlignment2D::Top: positionShift.y = size.y; break;
                    case VerticalAlignment2D::Middle: positionShift.y = (size.y + (rowsCount * (m_systemFont->charSize.y - m_systemFont->outline) + m_systemFont->outline) * fontScale) * 0.5f; break;
                    case VerticalAlignment2D::Bottom: positionShift.y = (rowsCount * rowSize) * fontScale; break;
                    default: break;
                }
                    
                positionShift += 0.5f * m_systemFont->charSize.y * fontScale;

                Vec2F sizeV = (Vec2F)m_systemFont->charSize * fontScale;
                Vec2F positionShiftV = Vec2F((F32)sx, (F32)sy) * fontScale + positionShift;

                TMat localTransform = TMat::CreateScale(0.01f).transform(
                    TMat::CreateTranslation(positionShiftV)).transform(
                        TMat::CreateScale(sizeV));
                m_localMatrices[charIndex] = localTransform;

                m_meshRenderer->setUV0(charIndex, uv);

                ++charIndex;

                sx += m_systemFont->charSize.x - m_systemFont->outline;
            }

            ++p;
        }

        updateMeshRendererModelMatrices();
        updateMeshRendererColors();
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::updateMaterial()
    {
        if (!m_meshRenderer)
            return;

        if (m_systemFont)
            m_meshRenderer->setMaterial(m_systemFont->material);
        else
            m_meshRenderer->setMaterial(MaterialPtr());
    }

    //////////////////////////////////////////
    Vec2F SystemTextRenderer3D::getTextEnd(Size _rowIndex)
    {
        if (!m_systemFont)
            return Vec2F::c_zero;

        Vec2F const& size = m_size;
        F32 fontScale = (F32)m_fontSize / (F32)(m_systemFont->charSize.x - m_systemFont->outline * 2);

        S32 const rowSize = m_systemFont->charSize.y;

        S32 rowsCount = 1;
        S32 maxColumnsCount = 0;
        S32 charsCount = 0;
        calculateTextData(
            rowsCount,
            maxColumnsCount,
            charsCount);

        S32 columnsCount = 0;
        Char const* p0 = &m_text[0];
        while (*p0 != 0 && *p0 != '\n')
        {
            ++columnsCount;
            ++p0;
        }

        S32 sx = 0;
        S32 sy = -rowSize;

        Char const* p = &m_text[0];
        while (*p != 0 && _rowIndex > 0)
        {
            Char c = *p;

            if (c == '\n')
            {
                --_rowIndex;

                sx = 0;
                sy -= rowSize;

                columnsCount = 0;
                p0 = p + 1;
                while (*p0 != 0 && *p0 != '\n')
                {
                    ++columnsCount;
                    ++p0;
                }
            }
        }

        Vec2F positionShift;
        switch (m_horizontalAlignment)
        {
            case HorizontalAlignment2D::Left: positionShift.x = 0.0f; break;
            case HorizontalAlignment2D::Center: positionShift.x = (size.x - (columnsCount * (m_systemFont->charSize.x - m_systemFont->outline) + m_systemFont->outline) * fontScale) * 0.5f; break;
            case HorizontalAlignment2D::Right: positionShift.x = size.x - (columnsCount * (m_systemFont->charSize.x - m_systemFont->outline) + m_systemFont->outline) * fontScale; break;
            default: break;
        }
        switch (m_verticalAlignment)
        {
            case VerticalAlignment2D::Top: positionShift.y = size.y; break;
            case VerticalAlignment2D::Middle: positionShift.y = (size.y + (rowsCount * (m_systemFont->charSize.y - m_systemFont->outline) + m_systemFont->outline) * fontScale) * 0.5f; break;
            case VerticalAlignment2D::Bottom: positionShift.y = (rowsCount * rowSize) * fontScale; break;
            default: break;
        }

        positionShift += 0.5f * m_systemFont->charSize.x * fontScale;

        Vec2F sizeV = (Vec2F)m_systemFont->charSize * fontScale;
        Vec2F positionShiftV = Vec2F((F32)sx, (F32)sy) * fontScale + positionShift;

        return Vec2F(-0.5f + columnsCount, -0.5f) * sizeV + positionShiftV;
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::calculateTextData(
        S32& _rowsCount,
        S32& _maxColumnsCount,
        S32& _charsCount)
    {
        S32 columnsCounter = 0;

        Char const* p0 = &m_text[0];
        while (*p0 != 0)
        {
            if (*p0 == '\n')
            {
                ++_rowsCount;

                _maxColumnsCount = Math::Max(_maxColumnsCount, columnsCounter);
                columnsCounter = 0;
            }
            else
            {
                ++columnsCounter;
                ++_charsCount;
            }

            ++p0;
        }
        _maxColumnsCount = Math::Max(_maxColumnsCount, columnsCounter);
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::processEntityAwakened()
    {
        AbstractTextRenderer3D::processEntityAwakened();

        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRendererInstanced>();
        m_meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultQuadMesh());

        updateMaterial();
        updateMeshData();
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::updateMeshRendererColors()
    {
        if (!m_meshRenderer)
            return;

        Vec4F const vertexColor = Vec4F(1.0f, 1.0f, 1.0f, 1.0f);

        Size colorsCount = m_meshRenderer->getColors().size();
        for (Size i = 0; i < colorsCount; ++i)
            m_meshRenderer->setColor(i, vertexColor * m_color.toVec4F32());
    }

    //////////////////////////////////////////
    void SystemTextRenderer3D::updateMeshRendererModelMatrices()
    {
        if (!m_meshRenderer)
            return;

        Size transformCount = m_meshRenderer->getModelMatrices().size();
        MAZE_DEBUG_ERROR_IF(transformCount != m_localMatrices.size(), "Invalid characters count!");
        for (Size i = 0; i < transformCount; ++i)
            m_meshRenderer->setModelMatrix(
                i,
                m_transform->getWorldTransform().transform(m_localMatrices[i]));
    }



    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SystemTextRenderer3DSystem,
        MAZE_ECS_TAGS(MAZE_HS("render")),
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        SystemTextRenderer3D* _systemTextRenderer3D)
    {
        if (_systemTextRenderer3D->getTransform()->isWorldTransformChanged())
            _systemTextRenderer3D->updateMeshRendererModelMatrices();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(SystemTextRenderer3DAdded,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        SystemTextRenderer3D* _textRenderer)
    {
        if (!_textRenderer->getSystemFont())
        {
            _textRenderer->setSystemFont(
                SystemFontManager::GetCurrentInstancePtr()->getSystemFontDefault());
        }
    }
            
    
} // namespace Maze
//////////////////////////////////////////
