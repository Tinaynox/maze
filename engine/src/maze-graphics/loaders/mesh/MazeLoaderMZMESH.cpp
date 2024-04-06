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
#include "maze-core/hash/MazeHashFNV1.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderMZMESH.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #pragma warning(push)
    #pragma warning(disable:4307)
    MAZE_CONSTEXPR U32 const c_mzMeshHeaderMagic = Hash::CalculateFNV1("MZMESH");
    MAZE_CONSTEXPR S32 const c_mzMeshHeaderVersion = 1;
    #pragma warning(pop)


    //////////////////////////////////////////
    #pragma pack(push,1)
    struct MAZE_GRAPHICS_API MZMESHHeader
    {
        U32 magic = 0u;
        S32 version = 0;
        S32 subMeshCount = 0;
    };
    #pragma pack(pop)


    //////////////////////////////////////////
    enum class MAZE_GRAPHICS_API MZMESHTag : U8
    {
        None                = 0,
        SubMeshStart        = 1,
        SubMeshEnd          = 2,
        VertexIndices       = 3,
        VertexAttributes    = 4,
    };
    

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadMZMESH(
        AssetFile const& _file,
        Mesh& _mesh,
        MeshLoaderProperties const& _props)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadMZMESH(fileData, _mesh, _props);
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadMZMESH(
        ByteBuffer const& _fileData,
        Mesh& _mesh,
        MeshLoaderProperties const& _props)
    {
        MAZE_PROFILE_EVENT("LoadMZMESH");

        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");
        
        MZMESHHeader header;
        U32 bytesRead = _fileData.read(0, &header, sizeof(header));
        if (bytesRead < sizeof(header))
            return false;
        if (header.magic != c_mzMeshHeaderMagic)
            return false;

        _mesh.clear();

        for (S32 i = 0; i < header.subMeshCount; ++i)
        {
            // Start tag
            MZMESHTag tag = MZMESHTag::None;
            bytesRead += _fileData.read(bytesRead, &tag, sizeof(tag));
            MAZE_ERROR_RETURN_VALUE_IF(tag != MZMESHTag::SubMeshStart, false, "MZMESH structure broken");

            // Create Mesh
            SubMeshPtr subMesh = SubMesh::Create();

            // Name
            U16 nameLength = 0u;
            bytesRead += _fileData.read(bytesRead, &nameLength, sizeof(nameLength));
            String subMeshName;
            subMeshName.resize((Size)nameLength);
            bytesRead += _fileData.read(bytesRead, (void*)subMeshName.data(), (Size)nameLength);
            subMesh->setName(subMeshName);

            // RenderDrawTopology
            RenderDrawTopology::Enum subMeshRenderDrawTopology = RenderDrawTopology::None;
            bytesRead += _fileData.read(bytesRead, &subMeshRenderDrawTopology, sizeof(subMeshRenderDrawTopology));
            subMesh->setRenderDrawTopology(subMeshRenderDrawTopology);

            // Vertex blocks
            bool subMeshFinished = false;
            U32 indicesCount = 0u;
            do
            {
                bytesRead += _fileData.read(bytesRead, &tag, sizeof(tag));
                switch (tag)
                {
                    case MZMESHTag::VertexIndices:
                    {
                        VertexAttributeType type = VertexAttributeType::S8;
                        bytesRead += _fileData.read(bytesRead, &type, sizeof(type));
                        U32 elementsCount = 0u;
                        bytesRead += _fileData.read(bytesRead, &elementsCount, sizeof(elementsCount));
                        
                        ByteBufferPtr const& indicesBuffer = subMesh->allocateIndices(
                            type, 
                            elementsCount);
                        bytesRead += _fileData.read(bytesRead, indicesBuffer->getDataRW(), U32(elementsCount * GetVertexAttributeTypeSize(type)));
                        break;
                    }
                    case MZMESHTag::VertexAttributes:
                    {
                        VertexAttributeSemantic semantic = VertexAttributeSemantic::Position;
                        bytesRead += _fileData.read(bytesRead, &semantic, sizeof(semantic));
                        VertexAttributeType type = VertexAttributeType::S8;
                        bytesRead += _fileData.read(bytesRead, &type, sizeof(type));
                        U8 attributesCount = 0u;
                        bytesRead += _fileData.read(bytesRead, &attributesCount, sizeof(attributesCount));
                        U32 elementsCount = 0u;
                        bytesRead += _fileData.read(bytesRead, &elementsCount, sizeof(elementsCount));

                        ByteBufferPtr const& buffer = subMesh->allocateVertexAttributes(
                            semantic,
                            type,
                            attributesCount,
                            elementsCount,
                            false);
                        bytesRead += _fileData.read(bytesRead, buffer->getDataRW(), U32(elementsCount * attributesCount * GetVertexAttributeTypeSize(type)));

                        break;
                    }
                    case MZMESHTag::SubMeshEnd:
                    {
                        subMeshFinished = true;
                        break;
                    }
                    default:
                    {
                        MAZE_ERROR_RETURN_VALUE(false, "MZMESH structure broken");
                    }
                }
            }
            while(!subMeshFinished);

            _mesh.addSubMesh(subMesh);
        }

        if (_props.mergeSubMeshes)
            _mesh.mergeSubMeshes();

        if (_props.scale != 1.0f)
            _mesh.scale(_props.scale);

        return true;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsMZMESHFile(AssetFile const& _file)
    {
        ByteBuffer fileData;
        _file.readHeaderToByteBuffer(fileData, sizeof(U32));
        return IsMZMESHFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsMZMESHFile(ByteBuffer const& _fileData)
    {
        if (_fileData.getSize() < sizeof(U32))
            return false;
        
        U32 magic = 0u;
        _fileData.read(0, &magic, sizeof(magic));

        return magic == c_mzMeshHeaderMagic;
    }

    //////////////////////////////////////////
    inline bool SaveMZMESHSubMeshVertexIndices(
        std::ofstream& _outputFile,
        VertexAttributeType _type,
        U8 const* _elementsData,
        U32 _elementsCount)
    {
        if (_elementsCount == 0)
            return true;

        MZMESHTag tag = MZMESHTag::VertexIndices;
        _outputFile.write((S8 const*)&tag, sizeof(tag));
        _outputFile.write((S8 const*)&_type, sizeof(_type));
        _outputFile.write((S8 const*)&_elementsCount, sizeof(_elementsCount));
        _outputFile.write((S8 const*)_elementsData, _elementsCount * GetVertexAttributeTypeSize(_type));

        return true;
    }

    //////////////////////////////////////////
    inline bool SaveMZMESHSubMeshVertexAttributes(
        std::ofstream& _outputFile,
        VertexAttributeSemantic _semantic,
        VertexAttributeType _type,
        U8 _count,
        U8 const* _elementsData,
        U32 _elementsCount)
    {
        if (_elementsCount == 0)
            return true;

        MZMESHTag tag = MZMESHTag::VertexAttributes;
        _outputFile.write((S8 const*)&tag, sizeof(tag));
        _outputFile.write((S8 const*)&_semantic, sizeof(_semantic));
        _outputFile.write((S8 const*)&_type, sizeof(_type));
        _outputFile.write((S8 const*)&_count, sizeof(_count));
        _outputFile.write((S8 const*)&_elementsCount, sizeof(_elementsCount));
        _outputFile.write((S8 const*)_elementsData, _elementsCount * _count * GetVertexAttributeTypeSize(_type));

        return true;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool SaveMZMESH(
        Mesh const& _mesh,
        Path _filePath)
    {
        MAZE_PROFILE_EVENT("SaveMZMESH");

        std::ofstream outputFile(_filePath.c_str(), std::ios::binary);

        MAZE_ERROR_RETURN_VALUE_IF(!outputFile, false, "Failed to open file: %s", _filePath.toUTF8().c_str());

        MZMESHHeader header;
        header.magic = c_mzMeshHeaderMagic;
        header.version = c_mzMeshHeaderVersion;
        header.subMeshCount = (S32)_mesh.getSubMeshesCount();
        outputFile.write((S8 const*)&header, sizeof(header));

        MZMESHTag tag = MZMESHTag::None;


        for (S32 i = 0; i < header.subMeshCount; ++i)
        {
            SubMeshPtr const& subMesh = _mesh.getSubMesh((Size)i);

            // Start tag
            tag = MZMESHTag::SubMeshStart;
            outputFile.write((S8 const*)&tag, sizeof(tag));

            // Name
            U16 nameLength = (U16)subMesh->getName().size();
            outputFile.write((S8 const*)&nameLength, sizeof(nameLength));
            outputFile.write(subMesh->getName().c_str(), (Size)nameLength);

            // RenderDrawTopology
            RenderDrawTopology::Enum subMeshRenderDrawTopology = subMesh->getRenderDrawTopology();
            outputFile.write((S8 const*)&subMeshRenderDrawTopology, sizeof(subMeshRenderDrawTopology));

            // Indices
            SaveMZMESHSubMeshVertexIndices(
                outputFile,
                subMesh->getIndicesType(),
                subMesh->getIndicesData(),
                (U32)subMesh->getIndicesCount());

            // Attributes
            for (VertexAttributeSemantic s = VertexAttributeSemantic(0);
                 s < VertexAttributeSemantic::MAX;
                 s = VertexAttributeSemantic((S32)s + 1))
            {
                VertexAttributeDescription const& desc = subMesh->getVertexDescription(s);
                U32 elementSize = U32(GetVertexAttributeTypeSize(desc.type) * desc.count);

                SaveMZMESHSubMeshVertexAttributes(
                    outputFile,
                    s,
                    desc.type,
                    (U8)desc.count,
                    subMesh->getVertexData(s),
                    (U32)subMesh->getVerticesCount(s));
            }

            // End tag
            tag = MZMESHTag::SubMeshEnd;
            outputFile.write((S8 const*)&tag, sizeof(tag));
        }

        outputFile.close();

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
