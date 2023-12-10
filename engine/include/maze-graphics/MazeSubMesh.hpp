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
#pragma once
#if (!defined(_MazeSubMesh_hpp_))
#define _MazeSubMesh_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SubMesh);
    MAZE_USING_SHARED_PTR(VertexArrayObject);
    MAZE_USING_SHARED_PTR(Mesh);
    

    //////////////////////////////////////////
    // Class SubMesh
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API SubMesh
        : public SharedObject<SubMesh>
        , public ISharedCopyable<SubMesh>
    {
    public:

        //////////////////////////////////////////
        struct MeshVertexAttributeDescription
        {
        public:

            //////////////////////////////////////////
            MeshVertexAttributeDescription();

            VertexAttributeDescription description;
            ByteBufferPtr byteBuffer;
            Size count;
        };

    public:

        //////////////////////////////////////////
        virtual ~SubMesh();

        //////////////////////////////////////////
        static SubMeshPtr Create();

        //////////////////////////////////////////
        static SubMeshPtr Create(SubMeshPtr const& _subMesh);


        //////////////////////////////////////////
        virtual SubMeshPtr createCopy() MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline void setRenderDrawTopology(RenderDrawTopology _renderDrawTopology) { m_renderDrawTopology = _renderDrawTopology; }

        //////////////////////////////////////////
        inline RenderDrawTopology getRenderDrawTopology() const { return m_renderDrawTopology; }


        //////////////////////////////////////////
        void loadFromSubMesh(SubMeshPtr const& _subMesh);


        //////////////////////////////////////////
        void setName(String const& _name) { m_name = _name; }

        //////////////////////////////////////////
        String const& getName() const { return m_name; }


        //////////////////////////////////////////
        void setIndices(U32 const* _indices, Size _count);

        //////////////////////////////////////////
        ByteBufferPtr const& allocateIndices(VertexAttributeType _type, Size _count);

        //////////////////////////////////////////
        inline void setIndices(Vector<U32> const& _indices)
        {
            setIndices(&_indices[0], _indices.size());
        }



        //////////////////////////////////////////
        ByteBufferPtr const& allocateVertexAttributes(
            VertexAttributeSemantic _semantic,
            VertexAttributeType _type,
            U8 _attributesCount,
            Size _count,
            bool _normalized = false);

        //////////////////////////////////////////
        void setPositions(Vec3F const* _positions, Size _count);

        //////////////////////////////////////////
        inline void setPositions(Vector<Vec3F> const& _positions)
        {
            setPositions(&_positions[0], _positions.size());
        }

        //////////////////////////////////////////
        void setNormals(Vec3F const* _normals, Size _count);

        //////////////////////////////////////////
        inline void setNormals(Vector<Vec3F> const& _normals)
        {
            setNormals(&_normals[0], _normals.size());
        }

        //////////////////////////////////////////
        void setTangents(Vec3F const* _tangents, Size _count);

        //////////////////////////////////////////
        inline void setTangents(Vector<Vec3F> const& _tangents)
        {
            setTangents(&_tangents[0], _tangents.size());
        }

        //////////////////////////////////////////
        void setBitangents(Vec3F const* _bitangents, Size _count);

        //////////////////////////////////////////
        inline void setBitangents(Vector<Vec3F> const& _bitangents)
        {
            setBitangents(&_bitangents[0], _bitangents.size());
        }

        //////////////////////////////////////////
        void setColors(Vec4F const* _colors, Size _count);

        //////////////////////////////////////////
        inline void setColors(Vector<Vec4F> const& _colors)
        {
            setColors(&_colors[0], _colors.size());
        }

        //////////////////////////////////////////
        void setTexCoords(S32 _uvIndex, Vec2F const* _data, Size _count);

        //////////////////////////////////////////
        inline void setTexCoords(S32 _uvIndex, Vector<Vec2F> const& _data)
        {
            setTexCoords(_uvIndex, &_data[0], _data.size());
        }

        //////////////////////////////////////////
        void setTexCoords(S32 _uvIndex, Vec4F const* _data, Size _count);

        //////////////////////////////////////////
        inline void setTexCoords(S32 _uvIndex, Vector<Vec4F> const& _data)
        {
            setTexCoords(_uvIndex, &_data[0], _data.size());
        }


        //////////////////////////////////////////
        U8 const* getIndicesData() const { return m_indicesBuffer->getDataPointer(); }

        //////////////////////////////////////////
        VertexAttributeType getIndicesType() const { return m_indicesType; }

        //////////////////////////////////////////
        Size getIndicesCount() const { return m_indicesCount; }


        //////////////////////////////////////////
        inline U8 const* getVertexData(VertexAttributeSemantic _semantic) const
        {
            ByteBufferPtr const& byteBuffer = m_vertexData[(Size)_semantic].byteBuffer;
            if (!byteBuffer)
                return nullptr;
            return byteBuffer->getDataPointer();
        }

        //////////////////////////////////////////
        inline VertexAttributeDescription const& getVertexDescription(VertexAttributeSemantic _semantic)
        {
            return m_vertexData[(Size)_semantic].description;
        }

        //////////////////////////////////////////
        inline Size getVerticesCount(VertexAttributeSemantic _semantic)
        {
            return m_vertexData[(Size)_semantic].count;
        }

        //////////////////////////////////////////
        inline bool getVertexData(
            VertexAttributeSemantic _semantic,
            U8 const*& _data,
            VertexAttributeDescription& _description,
            Size& _count) const
        {
            MeshVertexAttributeDescription const& data = m_vertexData[(Size)_semantic];
            ByteBufferPtr const& byteBuffer = data.byteBuffer;
            if (!byteBuffer)
                return false;

            _data = byteBuffer->getData();
            _description = data.description;
            _count = data.count;

            return true;
        }

        //////////////////////////////////////////
        void scale(F32 _scale);

        //////////////////////////////////////////
        bool mergeWith(SubMeshPtr const& _otherSubMesh);

    protected:

        //////////////////////////////////////////
        SubMesh();

        //////////////////////////////////////////
        virtual bool init();

        //////////////////////////////////////////
        virtual bool init(SubMeshPtr const& _subMesh);

    protected:
        
        String m_name;

        RenderDrawTopology m_renderDrawTopology;

        MeshVertexAttributeDescription m_vertexData[(Size)VertexAttributeSemantic::MAX];

        ByteBufferPtr m_indicesBuffer;
        VertexAttributeType m_indicesType;
        Size m_indicesCount;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSubMesh_hpp_
//////////////////////////////////////////
