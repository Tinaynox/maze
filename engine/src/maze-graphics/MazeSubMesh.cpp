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
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SubMesh
    //
    //////////////////////////////////////////
    SubMesh::MeshVertexAttributeDescription::MeshVertexAttributeDescription()
        : count(0)
    {
    }

    //////////////////////////////////////////
    SubMesh::SubMesh()
        : m_renderDrawTopology(RenderDrawTopology::Triangles)
        , m_indicesType(VertexAttributeType::U32)
        , m_indicesCount(0)
    {
    }

    //////////////////////////////////////////
    SubMesh::~SubMesh()
    {
    }

    //////////////////////////////////////////
    SubMeshPtr SubMesh::Create()
    {
        SubMeshPtr subMesh;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SubMesh, subMesh, init());
        return subMesh;
    }

    //////////////////////////////////////////
    SubMeshPtr SubMesh::Create(SubMeshPtr const& _subMesh)
    {
        SubMeshPtr subMesh;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SubMesh, subMesh, init(_subMesh));
        return subMesh;
    }

    //////////////////////////////////////////
    bool SubMesh::init()
    {
        return true;
    }

    //////////////////////////////////////////
    bool SubMesh::init(SubMeshPtr const& _subMesh)
    {
        loadFromSubMesh(_subMesh);
        return true;
    }

    //////////////////////////////////////////
    void SubMesh::loadFromSubMesh(SubMeshPtr const& _subMesh)
    {
        setName(_subMesh->getName());
        setRenderDrawTopology(_subMesh->getRenderDrawTopology());

        for (VertexAttributeSemantic    attribSemantic = VertexAttributeSemantic(0);
                                        attribSemantic < VertexAttributeSemantic::MAX;
                                        attribSemantic = VertexAttributeSemantic((S32)attribSemantic + 1))
        {
            MeshVertexAttributeDescription const& srcVertexData = _subMesh->m_vertexData[(Size)attribSemantic];

            MeshVertexAttributeDescription vertexData;
            vertexData.description = srcVertexData.description;
            vertexData.byteBuffer = srcVertexData.byteBuffer ? srcVertexData.byteBuffer->createCopy() : nullptr;
            vertexData.count = srcVertexData.count;

            m_vertexData[(Size)attribSemantic] = vertexData;
        }

        m_indicesBuffer = _subMesh->m_indicesBuffer ? _subMesh->m_indicesBuffer->createCopy() : nullptr;
        m_indicesType = _subMesh->m_indicesType;
        m_indicesCount = _subMesh->m_indicesCount;
    }

    //////////////////////////////////////////
    SubMeshPtr SubMesh::createCopy()
    {
        SubMeshPtr result = Create(getSharedPtr());
        return result;
    }

    //////////////////////////////////////////
    void SubMesh::setIndices(U32 const* _indices, Size _count)
    {
        m_indicesType = VertexAttributeType::U32;

        Size bytesPerIndex = GetVertexAttributeTypeSize(m_indicesType);
        Size bytesPerIndices = bytesPerIndex * _count;
        m_indicesBuffer = ByteBuffer::Create();
        m_indicesBuffer->upload((U8 const*)_indices, bytesPerIndices);
        m_indicesCount = _count;
    }

    //////////////////////////////////////////
    void SubMesh::setPositions(Vec3DF const* _positions, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = VertexAttributeSemantic::Position;

        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = vertexSemantic;
        vertexDescription.count = 3;
        vertexDescription.type = VertexAttributeType::F32;
        vertexDescription.normalized = false;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->upload((U8 const*)_positions, vertexDescription.stride * _count);
        vertexData.count = _count;
    }

    //////////////////////////////////////////
    void SubMesh::setNormals(Vec3DF const* _normals, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = VertexAttributeSemantic::Normal;

        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = vertexSemantic;
        vertexDescription.count = 3;
        vertexDescription.type = VertexAttributeType::F32;
        vertexDescription.normalized = false;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->upload((U8 const*)_normals, vertexDescription.stride * _count);
        vertexData.count = _count;
    }

    //////////////////////////////////////////
    void SubMesh::setTangents(Vec3DF const* _tangents, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = VertexAttributeSemantic::Tangent;

        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = vertexSemantic;
        vertexDescription.count = 3;
        vertexDescription.type = VertexAttributeType::F32;
        vertexDescription.normalized = false;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->upload((U8 const*)_tangents, vertexDescription.stride * _count);
        vertexData.count = _count;
    }

    //////////////////////////////////////////
    void SubMesh::setBitangents(Vec3DF const* _bitangents, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = VertexAttributeSemantic::Bitangent;

        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = vertexSemantic;
        vertexDescription.count = 3;
        vertexDescription.type = VertexAttributeType::F32;
        vertexDescription.normalized = false;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->upload((U8 const*)_bitangents, vertexDescription.stride * _count);
        vertexData.count = _count;
    }

    //////////////////////////////////////////
    void SubMesh::setColors(Vec4DF const* _colors, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = VertexAttributeSemantic::Color;

        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = vertexSemantic;
        vertexDescription.count = 4;
        vertexDescription.type = VertexAttributeType::F32;
        vertexDescription.normalized = false;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->upload((U8 const*)_colors, vertexDescription.stride * _count);
        vertexData.count = _count;
    }

    //////////////////////////////////////////
    void SubMesh::setTexCoords(S32 _uvIndex, Vec2DF const* _data, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = (VertexAttributeSemantic)((Size)VertexAttributeSemantic::TexCoords0 + _uvIndex);

        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = vertexSemantic;
        vertexDescription.count = 2;
        vertexDescription.type = VertexAttributeType::F32;
        vertexDescription.normalized = false;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->upload((U8 const*)_data, vertexDescription.stride * _count);
        vertexData.count = _count;
    }

    //////////////////////////////////////////
    void SubMesh::setTexCoords(S32 _uvIndex, Vec4DF const* _data, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = (VertexAttributeSemantic)((Size)VertexAttributeSemantic::TexCoords0 + _uvIndex);

        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = vertexSemantic;
        vertexDescription.count = 4;
        vertexDescription.type = VertexAttributeType::F32;
        vertexDescription.normalized = false;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->upload((U8 const*)_data, vertexDescription.stride * _count);
        vertexData.count = _count;
    }

} // namespace Maze
//////////////////////////////////////////
