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
    MAZE_IMPLEMENT_INDEXED_RESOURCE(SubMesh);
    
    //////////////////////////////////////////
    SubMesh::MeshVertexAttributeDescription::MeshVertexAttributeDescription()
        : count(0)
    {
    }

    //////////////////////////////////////////
    SubMesh::SubMesh()
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
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(SubMesh, subMesh, init());
        return subMesh;
    }

    //////////////////////////////////////////
    SubMeshPtr SubMesh::Create(SubMeshPtr const& _subMesh)
    {
        SubMeshPtr subMesh;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(SubMesh, subMesh, init(_subMesh));
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
    ByteBufferPtr const& SubMesh::allocateIndices(VertexAttributeType _type, Size _count)
    {
        m_indicesType = _type;

        Size bytesPerIndex = GetVertexAttributeTypeSize(m_indicesType);
        Size bytesPerIndices = bytesPerIndex * _count;
        m_indicesBuffer = ByteBuffer::Create();
        m_indicesBuffer->resize(bytesPerIndices);
        m_indicesCount = _count;

        return m_indicesBuffer;
    }

    //////////////////////////////////////////
    void SubMesh::setIndices(U16 const* _indices, Size _count)
    {
        ByteBufferPtr const& indicesBuffer = allocateIndices(
            VertexAttributeType::U16,
            _count);
        indicesBuffer->copyFrom((U8 const*)_indices, indicesBuffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setIndices(U32 const* _indices, Size _count)
    {
        ByteBufferPtr const& indicesBuffer = allocateIndices(
            VertexAttributeType::U32,
            _count);
        indicesBuffer->copyFrom((U8 const*)_indices, indicesBuffer->getSize());
    }

    //////////////////////////////////////////
    ByteBufferPtr const& SubMesh::allocateVertexAttributes(
        VertexAttributeSemantic _vertexSemantic,
        VertexAttributeType _type,
        U8 _attributesCount,
        Size _count,
        bool _normalized)
    {
        MeshVertexAttributeDescription& vertexData = m_vertexData[(Size)_vertexSemantic];
        VertexAttributeDescription& vertexDescription = vertexData.description;
        vertexDescription.semantic = _vertexSemantic;
        vertexDescription.count = _attributesCount;
        vertexDescription.type = _type;
        vertexDescription.normalized = _normalized;
        vertexDescription.stride = vertexDescription.count * GetVertexAttributeTypeSize(vertexDescription.type);
        vertexDescription.offset = 0;
        vertexData.byteBuffer = ByteBuffer::Create();
        vertexData.byteBuffer->resize(vertexDescription.stride * _count);
        vertexData.count = _count;

        return vertexData.byteBuffer;
    }

    //////////////////////////////////////////
    void SubMesh::setPositions(Vec3F const* _positions, Size _count)
    {
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            VertexAttributeSemantic::Position,
            VertexAttributeType::F32,
            3,
            _count,
            false);
        buffer->copyFrom((U8 const*)_positions, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setNormals(Vec3F const* _normals, Size _count)
    {
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            VertexAttributeSemantic::Normal,
            VertexAttributeType::F32,
            3,
            _count,
            false);
        buffer->copyFrom((U8 const*)_normals, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setTangents(Vec3F const* _tangents, Size _count)
    {
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            VertexAttributeSemantic::Tangent,
            VertexAttributeType::F32,
            3,
            _count,
            false);
        buffer->copyFrom((U8 const*)_tangents, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setBitangents(Vec3F const* _bitangents, Size _count)
    {
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            VertexAttributeSemantic::Bitangent,
            VertexAttributeType::F32,
            3,
            _count,
            false);
        buffer->copyFrom((U8 const*)_bitangents, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setColors(Vec4F const* _colors, Size _count)
    {
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            VertexAttributeSemantic::Color,
            VertexAttributeType::F32,
            4,
            _count,
            false);
        buffer->copyFrom((U8 const*)_colors, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setTexCoords(S32 _uvIndex, Vec2F const* _data, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = (VertexAttributeSemantic)((Size)VertexAttributeSemantic::TexCoords0 + _uvIndex);
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            vertexSemantic,
            VertexAttributeType::F32,
            2,
            _count,
            false);
        buffer->copyFrom((U8 const*)_data, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setTexCoords(S32 _uvIndex, Vec4F const* _data, Size _count)
    {
        VertexAttributeSemantic vertexSemantic = (VertexAttributeSemantic)((Size)VertexAttributeSemantic::TexCoords0 + _uvIndex);
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            vertexSemantic,
            VertexAttributeType::F32,
            4,
            _count,
            false);
        buffer->copyFrom((U8 const*)_data, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setBlendWeights(Vec4F const* _blendWeights, Size _count)
    {
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            VertexAttributeSemantic::BlendWeights,
            VertexAttributeType::F32,
            4,
            _count,
            false);
        buffer->copyFrom((U8 const*)_blendWeights, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::setBlendIndices(Vec4F const* _blendIndices, Size _count)
    {
        ByteBufferPtr const& buffer = allocateVertexAttributes(
            VertexAttributeSemantic::BlendIndices,
            VertexAttributeType::F32,
            4,
            _count,
            false);
        buffer->copyFrom((U8 const*)_blendIndices, buffer->getSize());
    }

    //////////////////////////////////////////
    void SubMesh::scale(F32 _scale)
    {
        MeshVertexAttributeDescription const& data = m_vertexData[(Size)VertexAttributeSemantic::Position];
        ByteBufferPtr const& byteBuffer = data.byteBuffer;
        if (!byteBuffer)
            return;

        switch (data.description.type)
        {
            case VertexAttributeType::F32:
            {
                byteBuffer->iterateAs<F32>([_scale](F32& _value) { _value *= _scale; });
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }
    }

    //////////////////////////////////////////
    bool SubMesh::mergeWith(SubMeshPtr const& _otherSubMesh)
    {
        if (m_renderDrawTopology != _otherSubMesh->m_renderDrawTopology)
            return false;
        if (m_indicesType != _otherSubMesh->m_indicesType)
            return false;

        MeshVertexAttributeDescription const& positionData = m_vertexData[(Size)VertexAttributeSemantic::Position];
        ByteBufferPtr const& positionBuffer = positionData.byteBuffer;
        if (!positionBuffer)
            return false;

        for (VertexAttributeSemantic s = VertexAttributeSemantic(0);
            s < VertexAttributeSemantic::MAX;
            s = VertexAttributeSemantic((S32)s + 1))
        {
            MeshVertexAttributeDescription const& data0 = m_vertexData[(Size)s];
            MeshVertexAttributeDescription const& data1 = _otherSubMesh->m_vertexData[(Size)s];
            if ((bool)data0.byteBuffer != (bool)data1.byteBuffer)
                return false;

            if (data0.byteBuffer)
            {
                if (data0.description.type != data1.description.type ||
                    data0.description.count != data1.description.count ||
                    data0.description.normalized != data1.description.normalized ||
                    data0.description.stride != data1.description.stride ||
                    data0.description.offset != data1.description.offset)
                    return false;
            }
        }

        Size verticesCount = positionData.count;

        Size indicesBufferPrevSize = m_indicesBuffer->getSize();
        m_indicesBuffer->append(*_otherSubMesh->m_indicesBuffer.get());
        switch (m_indicesType)
        {
            #define CASE_SUBMESH_MERGE_INDEX_PROCESSOR(DType) \
                case VertexAttributeType:: DType :            \
            {                                                 \
                m_indicesBuffer->iterateAs<DType>(            \
                    [verticesCount](DType& _value)            \
                    {                                         \
                        _value += (DType)verticesCount;       \
                    },                                        \
                    indicesBufferPrevSize);                   \
                break;                                        \
            }
            CASE_SUBMESH_MERGE_INDEX_PROCESSOR(S8);
            CASE_SUBMESH_MERGE_INDEX_PROCESSOR(U8);
            CASE_SUBMESH_MERGE_INDEX_PROCESSOR(S16);
            CASE_SUBMESH_MERGE_INDEX_PROCESSOR(U16);
            CASE_SUBMESH_MERGE_INDEX_PROCESSOR(S32);
            CASE_SUBMESH_MERGE_INDEX_PROCESSOR(U32);
            #undef CASE_SUBMESH_MERGE_INDEX_PROCESSOR
            default:
            {
                MAZE_NOT_IMPLEMENTED
            }
        }
        m_indicesCount += _otherSubMesh->m_indicesCount;

        for (VertexAttributeSemantic s = VertexAttributeSemantic(0);
            s < VertexAttributeSemantic::MAX;
            s = VertexAttributeSemantic((S32)s + 1))
        {
            MeshVertexAttributeDescription& data0 = m_vertexData[(Size)s];
            if (!data0.byteBuffer)
                continue;

            MeshVertexAttributeDescription const& data1 = _otherSubMesh->m_vertexData[(Size)s];
            data0.byteBuffer->append(*data1.byteBuffer.get());
            data0.count += data1.count;
        }

        return true;
    }

    //////////////////////////////////////////
    F32 SubMesh::calculateBoundingSphereRadius() const
    {
        MeshVertexAttributeDescription const& positionData = m_vertexData[(Size)VertexAttributeSemantic::Position];
        ByteBufferPtr const& positionBuffer = positionData.byteBuffer;
        if (!positionBuffer)
            return 0.0f;

        F32 radiusSq = 0.0f;
        switch (positionData.description.type)
        {
            case VertexAttributeType::F32:
            {
                positionBuffer->iterateAs<Vec3F>([&radiusSq](Vec3F const& _value) { radiusSq = Math::Max(radiusSq, _value.squaredLength()); });
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }

        return Math::Sqrt(radiusSq);
    }

    //////////////////////////////////////////
    bool SubMesh::calculateAABB(AABB3D& _outAABB) const
    {
        MeshVertexAttributeDescription const& positionData = m_vertexData[(Size)VertexAttributeSemantic::Position];
        ByteBufferPtr const& positionBuffer = positionData.byteBuffer;
        if (!positionBuffer || positionData.count == 0)
            return false;

        switch (positionData.description.type)
        {
            case VertexAttributeType::F32:
            {
                Vec3F minPoint = Vec3F(F32_MAX, F32_MAX, F32_MAX);
                Vec3F maxPoint = Vec3F(-F32_MAX, -F32_MAX, -F32_MAX);
                positionBuffer->iterateAs<Vec3F>(
                    [&minPoint, &maxPoint](Vec3F const& _value)
                    {
                        minPoint.x = Math::Min(minPoint.x, _value.x);
                        minPoint.y = Math::Min(minPoint.y, _value.y);
                        minPoint.z = Math::Min(minPoint.z, _value.z);
                        maxPoint.x = Math::Max(maxPoint.x, _value.x);
                        maxPoint.y = Math::Max(maxPoint.y, _value.y);
                        maxPoint.z = Math::Max(maxPoint.z, _value.z);
                    });

                _outAABB.setMin(minPoint);
                _outAABB.setMax(maxPoint);
                return true;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                return false;
            }
        }
    }

    //////////////////////////////////////////
    bool SubMesh::traceRay(Vec3F const& _origin, Vec3F const& _direction, F32 &_t) const
    {
        if (m_renderDrawTopology != RenderDrawTopology::Triangles)
            return false;

        MeshVertexAttributeDescription const& positionData =
            m_vertexData[(Size)VertexAttributeSemantic::Position];
        ByteBufferPtr const& positionBuffer = positionData.byteBuffer;

        if (!positionBuffer || positionData.count == 0)
            return false;
        if (positionData.description.type != VertexAttributeType::F32)
        {
            MAZE_NOT_IMPLEMENTED;
            return false;
        }

        Vec3F const* positions = reinterpret_cast<Vec3F const*>(positionBuffer->getDataRO());
        Size vertexCount = positionData.count;

        if (!m_indicesBuffer || m_indicesCount == 0)
            return false;

        U8 const* indexData = m_indicesBuffer->getDataRO();

        auto getIndex = [&](Size _i) -> Size
        {
            switch (m_indicesType)
            {
                case VertexAttributeType::U16: return (Size)(reinterpret_cast<U16 const*>(indexData)[_i]);
                case VertexAttributeType::U32: return (Size)(reinterpret_cast<U32 const*>(indexData)[_i]);
                case VertexAttributeType::U8:  return (Size)(reinterpret_cast<U8  const*>(indexData)[_i]);
                case VertexAttributeType::S16: return (Size)(reinterpret_cast<S16 const*>(indexData)[_i]);
                case VertexAttributeType::S32: return (Size)(reinterpret_cast<S32 const*>(indexData)[_i]);
                case VertexAttributeType::S8:  return (Size)(reinterpret_cast<S8  const*>(indexData)[_i]);
                default: return 0;
            }
        };

        F32 bestT = _t;
        bool found = false;

        constexpr F32 c_epsilon = 1e-7f;

        auto testTriangle = 
            [&](Vec3F const& v0, Vec3F const& v1, Vec3F const& v2)
            {
                Vec3F edge1 = v1 - v0;
                Vec3F edge2 = v2 - v0;

                Vec3F h = _direction.crossProduct(edge2);
                F32   a = edge1.dotProduct(h);

                // Ray parallel to triangle
                if (a > -c_epsilon && a < c_epsilon)
                    return;

                F32   f = 1.0f / a;
                Vec3F s = _origin - v0;
                F32   u = f * s.dotProduct(h);

                if (u < 0.0f || u > 1.0f)
                    return;

                Vec3F q = s.crossProduct(edge1);
                F32   v = f * _direction.dotProduct(q);

                if (v < 0.0f || (u + v) > 1.0f)
                    return;

                F32 t = f * edge2.dotProduct(q);
                if (t < c_epsilon || t >= bestT)
                    return;

                bestT  = t;
                found  = true;
            };

        switch (m_renderDrawTopology)
        {
            case RenderDrawTopology::Triangles:
            {
                // Indices come in explicit triplets: (0,1,2), (3,4,5), …
                Size triCount = m_indicesCount / 3u;
                for (Size i = 0; i < triCount; ++i)
                {
                    Size i0 = getIndex(i * 3 + 0);
                    Size i1 = getIndex(i * 3 + 1);
                    Size i2 = getIndex(i * 3 + 2);
                    if (i0 >= vertexCount || i1 >= vertexCount || i2 >= vertexCount)
                        continue;
                    testTriangle(positions[i0], positions[i1], positions[i2]);
                }
                break;
            }

            case RenderDrawTopology::TrianglesFan:
            {
                // All triangles share index 0 as the hub vertex.
                Size hub = getIndex(0);
                if (hub >= vertexCount)
                    break;
                for (Size i = 1; i + 1 < m_indicesCount; ++i)
                {
                    Size i1 = getIndex(i);
                    Size i2 = getIndex(i + 1);
                    if (i1 >= vertexCount || i2 >= vertexCount)
                        continue;
                    testTriangle(positions[hub], positions[i1], positions[i2]);
                }
                break;
            }

            default:
                break;
        }

        if (found)
            _t = bestT;

        return found;
    }

} // namespace Maze
//////////////////////////////////////////
