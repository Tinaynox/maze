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
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemTextRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SystemTextRenderer2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(String, text, String(), getText, setText),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MaterialPtr, material, MaterialPtr(), getMaterial, setMaterial),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorU32, color, ColorU32::c_white, getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SystemFontPtr, systemFont, SystemFontPtr(), getSystemFont, setSystemFont),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U32, fontSize, 32, getFontSize, setFontSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(HorizontalAlignment2D, horizontalAlignment, HorizontalAlignment2D::Left, getHorizontalAlignment, setHorizontalAlignment),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(VerticalAlignment2D, verticalAlignment, VerticalAlignment2D::Top, getVerticalAlignment, setVerticalAlignment));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SystemTextRenderer2D);


    //////////////////////////////////////////
    SystemTextRenderer2D::SystemTextRenderer2D()
        : m_renderSystem(nullptr)
        , m_transform(nullptr)
        , m_meshRenderer(nullptr)
        , m_canvasRenderer(nullptr)
        , m_color(ColorU32::c_white)
        , m_fontSize(32)
        , m_horizontalAlignment(HorizontalAlignment2D::Left)
        , m_verticalAlignment(VerticalAlignment2D::Top)
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
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<SystemTextRenderer2D>()->m_renderSystem;

        if (!Component::init(
            _component,
            _world,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::setText(String const& _text)
    {
        if (m_text == _text)
            return;

        m_text = _text;

        updateMesh();
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::setMaterial(MaterialPtr const& _material)
    {
        if (m_material == _material)
            return;

        m_material = _material->createCopy();        
        m_colorUniform = m_material->ensureUniform("u_color");
        m_baseMapUniform = m_material->ensureUniform("u_baseMap");
        m_baseMapTexelSizeUniform = m_material->ensureUniform("u_baseMapTexelSize");
            
        updateMaterial();
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::setColor(ColorU32 _color)
    {
        if (m_color == _color)
            return;

        m_color = _color;

        updateMaterial();
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::setSystemFont(SystemFontPtr const& _systemFont)
    {
        if (m_systemFont == _systemFont)
            return;

        m_systemFont = _systemFont;

        updateMaterial();
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::updateMesh()
    {
        if (!m_canvasRenderer)
            return;

        if (!m_meshRenderer)
            return;

        if (!m_systemFont)
            return;

        Vec4DF vertexColor = Vec4DF(1.0f, 1.0f, 1.0f, m_canvasRenderer->getAlpha());

        Vec2DF const& size = m_transform->getSize();

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

        S32 columnsCount = 0;
        Char const* p0 = &m_text[0];
        while (*p0 != 0 && *p0 != '\n')
        {
            ++columnsCount;
            ++p0;
        }

            
        Maze::Size verticesCount = (Size)charsCount * 4;
        Maze::Size indicesCount = (Size)charsCount * 6;
        Maze::Vector<Maze::Vec3DF> positions(verticesCount);
        Maze::Vector<Maze::Vec4DF> colors(verticesCount);
        Maze::Vector<Maze::Vec4DF> uvs0(verticesCount);
        Maze::Vector<Maze::U32> indices(indicesCount);


        S32 sx = 0;
        S32 sy = -rowSize;

        Size charVerticesIndex = 0;
        Size charIndicesIndex = 0;

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
                Vec4DF uv(
                    (F32(ox * m_systemFont->stroke.x + m_systemFont->offset.x) / (F32)systemFontTexture->getWidth()) + border,
                    (F32(oy * m_systemFont->stroke.y + m_systemFont->offset.y) / (F32)systemFontTexture->getHeight()) + border,
                    (F32(ox * m_systemFont->stroke.x + m_systemFont->charSize.x + m_systemFont->offset.x) / (F32)systemFontTexture->getWidth()) - border,
                    (F32(oy * m_systemFont->stroke.y + m_systemFont->charSize.y + m_systemFont->offset.y) / (F32)systemFontTexture->getHeight()) - border
                );

                Vec2DF positionShift;
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
                    case VerticalAlignment2D::Bottom: positionShift.y = (rowsCount * (m_systemFont->charSize.y - m_systemFont->outline) + m_systemFont->outline) * fontScale; break;
                    default: break;
                }
                    
                positionShift += 0.5f * m_systemFont->charSize.y * fontScale;

                Vec2DF sizeV = m_systemFont->charSize * fontScale;
                Vec2DF positionShiftV = Vec2DF((F32)sx, (F32)sy) * fontScale + positionShift;


                positions[charVerticesIndex + 0] = Maze::Vec3DF(+0.5f, +0.5f, +0.0f) * sizeV + positionShiftV;    // Top right
                positions[charVerticesIndex + 1] = Maze::Vec3DF(+0.5f, -0.5f, +0.0f) * sizeV + positionShiftV;    // Bottom right
                positions[charVerticesIndex + 2] = Maze::Vec3DF(-0.5f, -0.5f, +0.0f) * sizeV + positionShiftV;    // Bottom left
                positions[charVerticesIndex + 3] = Maze::Vec3DF(-0.5f, +0.5f, +0.0f) * sizeV + positionShiftV;    // Top left

                colors[charVerticesIndex + 0] = vertexColor;
                colors[charVerticesIndex + 1] = vertexColor;
                colors[charVerticesIndex + 2] = vertexColor;
                colors[charVerticesIndex + 3] = vertexColor;

                uvs0[charVerticesIndex + 0] = Maze::Vec4DF(uv.z, uv.w, 0.0f, 0.0f);    // Top right
                uvs0[charVerticesIndex + 1] = Maze::Vec4DF(uv.z, uv.y, 0.0f, 0.0f);    // Bottom right
                uvs0[charVerticesIndex + 2] = Maze::Vec4DF(uv.x, uv.y, 0.0f, 0.0f);    // Bottom left
                uvs0[charVerticesIndex + 3] = Maze::Vec4DF(uv.x, uv.w, 0.0f, 0.0f);    // Top left

                indices[charIndicesIndex + 0] = (U32)charVerticesIndex + 0;
                indices[charIndicesIndex + 1] = (U32)charVerticesIndex + 1;
                indices[charIndicesIndex + 2] = (U32)charVerticesIndex + 3;
                indices[charIndicesIndex + 3] = (U32)charVerticesIndex + 1;
                indices[charIndicesIndex + 4] = (U32)charVerticesIndex + 2;
                indices[charIndicesIndex + 5] = (U32)charVerticesIndex + 3;

                charVerticesIndex += 4;
                charIndicesIndex += 6;

                sx += m_systemFont->charSize.x - m_systemFont->outline;
            }

            ++p;
        }

        if (charsCount > 0)
        {
            m_subMesh->setPositions(&positions[0], positions.size());
            m_subMesh->setColors(&colors[0], colors.size());
            m_subMesh->setTexCoords(0, &uvs0[0], uvs0.size());
            m_subMesh->setIndices(&indices[0], indices.size());

            m_meshRenderer->setMesh(m_mesh);
        }
        else
        {
            m_meshRenderer->clearMesh();
        }
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::updateMaterial()
    {
        if (!m_meshRenderer)
            return;

        m_meshRenderer->setMaterial(m_material);

        if (m_material && m_systemFont)
        {
            Texture2DPtr texture = m_systemFont->texture;

            m_colorUniform->set(m_color.toVec4DF());
            m_baseMapUniform->set(texture);
            m_baseMapTexelSizeUniform->set(1.0f / (Vec2DF)texture->getSize());
        }
    }

    //////////////////////////////////////////
    Vec2DF SystemTextRenderer2D::getTextEnd(Size _rowIndex)
    {
        Vec2DF const& size = m_transform->getSize();
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

        Vec2DF positionShift;
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
            case VerticalAlignment2D::Bottom: positionShift.y = (rowsCount * (m_systemFont->charSize.y - m_systemFont->outline) + m_systemFont->outline) * fontScale; break;
            default: break;
        }

        positionShift += 0.5f * m_systemFont->charSize.x * fontScale;

        Vec2DF sizeV = m_systemFont->charSize * fontScale;
        Vec2DF positionShiftV = Vec2DF((F32)sx, (F32)sy) * fontScale + positionShift;

        return Vec2DF(-0.5f + columnsCount, -0.5f) * sizeV + positionShiftV;
    }

    //////////////////////////////////////////
    void SystemTextRenderer2D::calculateTextData(
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
    void SystemTextRenderer2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();
        m_canvasRenderer = getEntityRaw()->ensureComponent<CanvasRenderer>();

        m_meshRenderer->setRenderMesh(RenderMeshPtr());
        m_mesh = Mesh::Create();
        m_subMesh = Maze::SubMesh::Create();             
        m_subMesh->setRenderDrawTopology(RenderDrawTopology::Triangles);
        m_mesh->addSubMesh(m_subMesh);

        updateMaterial();
        updateMesh();
    }
            
    
} // namespace Maze
//////////////////////////////////////////
