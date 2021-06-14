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
#include "maze-graphics/ecs/components/MazeTerrainMesh3D.hpp"
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
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include <cassert>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TerrainMesh3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TerrainMesh3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Texture2DPtr, heightMap, Texture2DPtr(), getHeightMap, setHeightMap),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2DU, cellsCount, Vec2DU(10, 10), getCellsCount, setCellsCount),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2DF, size, Vec2DF(10.0f, 10.0f), getSize, setSize),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, height, 5.0f, getHeight, setHeight));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TerrainMesh3D);


    //////////////////////////////////////////
    TerrainMesh3D::TerrainMesh3D()
    {
        
    }

    //////////////////////////////////////////
    TerrainMesh3D::~TerrainMesh3D()
    {
        
    }

    //////////////////////////////////////////
    TerrainMesh3DPtr TerrainMesh3D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        TerrainMesh3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TerrainMesh3D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool TerrainMesh3D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool TerrainMesh3D::init(
        Component* _component,
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<TerrainMesh3D>()->m_renderSystem;

        if (!Component::init(
            _component,
            _world,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void TerrainMesh3D::setHeightMap(Texture2DPtr const& _value)
    {
        if (m_heightMap == _value)
            return;

        m_heightMap = _value;

        buildMesh();
    }

    //////////////////////////////////////////
    void TerrainMesh3D::setCellsCount(Vec2DU const& _value)
    {
        if (m_cellsCount == _value)
            return;

        m_cellsCount = _value;

        buildMesh();
    }

    //////////////////////////////////////////
    void TerrainMesh3D::setSize(Vec2DF const& _value)
    {
        if (m_size == _value)
            return;

        m_size = _value;

        buildMesh();
    }

    //////////////////////////////////////////
    void TerrainMesh3D::setHeight(F32 _value)
    {
        if (m_height == _value)
            return;

        m_height = _value;
        
        buildMesh();
    }

    //////////////////////////////////////////
    void TerrainMesh3D::clearMesh()
    {
        if (!m_meshRenderer)
            return;

        m_meshRenderer->clearMesh();
    }

    //////////////////////////////////////////
    Vec2DF Random2(Vec2DF st)
    {
        st = Vec2DF(
            st.dotProduct(Vec2DF(127.1f, 311.7f)),
            st.dotProduct(Vec2DF(269.5f, 183.3f)));
        return -1.0f + 2.0f * (Vec2DF(Math::Fract(sin(st.x) * 43758.5453123f), Math::Fract(sin(st.y) * 43758.5453123f)));
    }

    //////////////////////////////////////////
    F32 GradientNoise(Vec2DF st)
    {
        Vec2DF i = { Math::Floor(st.x), Math::Floor(st.y) };
        Vec2DF f = { Math::Fract(st.x), Math::Fract(st.y) };

        Vec2DF u = f * f * (3.0f - 2.0f * f);

        Vec2DF a =
            Math::Lerp(
                Random2(i + Vec2DF(0.0, 0.0).dotProduct(f - Vec2DF(0.0, 0.0))),
                Random2(i + Vec2DF(1.0, 0.0).dotProduct(f - Vec2DF(1.0, 0.0))),
                u.x);

        Vec2DF b =
            Math::Lerp(
                Random2(i + Vec2DF(0.0, 1.0).dotProduct(f - Vec2DF(0.0, 1.0))),
                Random2(i + Vec2DF(1.0, 1.0).dotProduct(f - Vec2DF(1.0, 1.0))),
                u.x);

        return Math::Lerp(
            a,
            b,
            u.y).x;
    }

    //////////////////////////////////////////
    void TerrainMesh3D::buildMesh()
    {
        clearMesh();

        if (!m_meshRenderer)
            return;

        if (!m_heightMap)
            return;

        if (m_cellsCount.x <= 0 || m_cellsCount.y <= 0)
            return;

        if (m_size.x <= 0 || m_size.y <= 0)
            return;

        PixelSheet2D pixelSheet = m_heightMap->readAsPixelSheet();

        if (pixelSheet.getSize().x <= 0 || pixelSheet.getSize().y <= 0)
            return;

        std::function<F32(S32 _r, S32 _c)> fetchHeightCoef;

        switch (pixelSheet.getFormat())
        {
            case PixelFormat::RGBA_U8:
            case PixelFormat::RGB_U8:
            case PixelFormat::RG_U8:
            case PixelFormat::R_U8:
            {
                fetchHeightCoef = [&](S32 _r, S32 _c) -> F32
                {
                    F32 x = (F32)_c / ((F32)m_cellsCount.x + 1.0f);
                    F32 y = (F32)_r / ((F32)m_cellsCount.y + 1.0f);

                    x *= pixelSheet.getColumnsCount();
                    y *= pixelSheet.getRowsCount();

                    void const* pixelData = pixelSheet.getPixel((S32)x, (S32)y);
                    U8 red = *((U8*)pixelData);
                    return (F32)red / 255.0f;
                };
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                return;
            }
        }

        SubMeshPtr subMesh = SubMesh::Create();
        subMesh->setRenderDrawTopology(RenderDrawTopology::Triangles);

        std::function<S32(S32 _r, S32 _c)> fetchVertexIndex = [&](S32 _r, S32 _c) -> S32 { return _r * ((S32)m_cellsCount.x + 1) + _c; };

        Size verticesCount = (m_cellsCount.y + 1) * (m_cellsCount.x + 1);
        Size indicesCount = verticesCount * 6;

        Vector<Vec3DF> positions;
        Vector<Vec3DF> normals;
        Vector<Vec4DF> colors;
        Vector<Vec2DF> uvs0;
        Vector<U32> indices;

        positions.resize(verticesCount);
        normals.resize(verticesCount);
        colors.resize(verticesCount);
        uvs0.resize(verticesCount);
        indices.resize(indicesCount);

        F32 cellWidth = m_size.x / m_cellsCount.x;
        F32 cellHeight = m_size.y / m_cellsCount.y;

        for (S32 r = 0; r < (S32)m_cellsCount.y + 1; ++r)
        {
            for (S32 c = 0; c < (S32)m_cellsCount.x + 1; ++c)
            {
                S32 vertexIndex = fetchVertexIndex(r, c);

                F32 heightCoef = fetchHeightCoef(r, c);
                F32 height = m_height * heightCoef;

                positions[vertexIndex] = Vec3DF(c * cellWidth - m_size.x * 0.5f, height, r * cellHeight - m_size.y * 0.5f);
                colors[vertexIndex] = Vec4DF(1.0f, 1.0f, 1.0f, 1.0f);
                uvs0[vertexIndex] = Vec2DF((F32)c / m_cellsCount.x, (F32)r / m_cellsCount.y);
            }
        }

        // Calculate normals
        for (S32 r = 0; r < (S32)m_cellsCount.y + 1; ++r)
        {
            for (S32 c = 0; c < (S32)m_cellsCount.x + 1; ++c)
            {
                S32 vertexIndex = fetchVertexIndex(r, c);

                Vec3DF mPos = positions[vertexIndex];

                Vec3DF normalsSum = Vec3DF::c_zero;
                S32 normalsCount = 0;

                // Left
                if (c != 0)
                {
                    Vec3DF lPos = positions[fetchVertexIndex(r, c - 1)];
                    Vec3DF normal = (mPos - lPos).crossProduct(Vec3DF::c_negativeUnitZ);
                    normalsSum += normal;
                    ++normalsCount;
                }

                // Right
                if (c != (S32)m_cellsCount.x)
                {
                    Vec3DF rPos = positions[fetchVertexIndex(r, c + 1)];
                    Vec3DF normal = (rPos - mPos).crossProduct(Vec3DF::c_negativeUnitZ);
                    normalsSum += normal;
                    ++normalsCount;
                }

                // Down
                if (r != 0)
                {
                    Vec3DF dPos = positions[fetchVertexIndex(r - 1, c)];
                    Vec3DF normal = (mPos - dPos).crossProduct(Vec3DF::c_unitX);
                    normalsSum += normal;
                    ++normalsCount;
                }

                // Up
                if (r != (S32)m_cellsCount.y)
                {
                    Vec3DF uPos = positions[fetchVertexIndex(r + 1, c)];
                    Vec3DF normal = (uPos - mPos).crossProduct(Vec3DF::c_unitX);
                    normalsSum += normal;
                    ++normalsCount;
                }
                
                Vec3DF normal = normalsSum / (F32)normalsCount;
                normals[vertexIndex] = normal.normalizedCopy();
            }
        }

        Vector<Vec3DF> tangents;
        Vector<Vec3DF> bitangents;
        if (SubMeshHelper::GenerateTangentsAndBitangents(
            &indices[0],
            indicesCount,
            &positions[0],
            &uvs0[0],
            &normals[0],
            4,
            tangents,
            bitangents))
        {
            subMesh->setTangents(tangents);
            subMesh->setBitangents(bitangents);
        }

        S32 index = 0;
        
        for (S32 r = 0; r < (S32)m_cellsCount.y; ++r)
        {
            for (S32 c = 0; c < (S32)m_cellsCount.x; ++c)
            {
                S32 lb = fetchVertexIndex(r, c);
                S32 rb = fetchVertexIndex(r, c + 1);
                S32 rt = fetchVertexIndex(r + 1, c + 1);
                S32 lt = fetchVertexIndex(r + 1, c);

                indices[index++] = lb;
                indices[index++] = rb;
                indices[index++] = rt;

                indices[index++] = lb;
                indices[index++] = rt;
                indices[index++] = lt;
                
            }
        }

        subMesh->setPositions(positions);
        subMesh->setNormals(normals);
        subMesh->setColors(colors);
        subMesh->setTexCoords(0, uvs0);
        subMesh->setIndices(indices);

        MeshPtr mesh = Mesh::Create();
        mesh->addSubMesh(subMesh);

        m_meshRenderer->setMesh(mesh);
    }


    //////////////////////////////////////////
    void TerrainMesh3D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();

        m_meshRenderer->setRenderMesh(RenderMeshPtr());
        
        buildMesh();
    }
            
    
} // namespace Maze
//////////////////////////////////////////
