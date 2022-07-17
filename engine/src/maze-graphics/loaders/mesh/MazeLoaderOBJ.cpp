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
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadOBJ(
        AssetFilePtr const& _file,
        MeshPtr const& _mesh,
        RenderMeshLoaderProperties const& _props)
    {
        ByteBufferPtr fileData = _file->readAsByteBuffer();
        return LoadOBJ(fileData, _mesh, _props);
    }

    //////////////////////////////////////////
    static String GetFirstToken(String const& _line)
    {
        if (!_line.empty())
        {
            Size tokenStart = _line.find_first_not_of(" \t");
            Size tokenEnd = _line.find_first_of(" \t", tokenStart);

            if (    tokenStart != String::npos
                &&  tokenEnd != String::npos)
            {
                return _line.substr(
                    tokenStart,
                    tokenEnd - tokenStart);
            }
            else
            if (tokenStart != String::npos)
            {
                return _line.substr(tokenStart);
            }
        }

        return String();
    }

    //////////////////////////////////////////
    inline static String GetTail(String const& _line)
    {
        Size tokenStart = _line.find_first_not_of(" \t");
        Size spaceStart = _line.find_first_of(" \t", tokenStart);
        Size tailStart = _line.find_first_not_of(" \t", spaceStart);
        Size tailEnd = _line.find_last_not_of(" \t");

        if (   tailStart != String::npos
            && tailEnd != String::npos)
        {
            return _line.substr(
                tailStart,
                tailEnd - tailStart + 1);
        }
        else
        if (tailStart != std::string::npos)
        {
            return _line.substr(tailStart);
        }
        return String();
    }

    //////////////////////////////////////////
    template <class T>
    inline const T& GetElement(
        Vector<T> const& _elements,
        String& _index)
    {
        S32 idx = StringHelper::StringToS32(_index);
        if (idx < 0)
            idx = S32(_elements.size()) + idx;
        else
            idx--;
        return _elements[idx];
    }

    //////////////////////////////////////////
    static Vec2DF NormalizeUV(Vec2DF const& _uv)
    {
        Vec2DF uv = _uv;
        while (uv.x > 1.0f) uv.x -= 1.0f;
        while (uv.x < 0.0f) uv.x += 1.0f;
        while (uv.y > 1.0f) uv.y -= 1.0f;
        while (uv.y < 0.0f) uv.y += 1.0f;
        return uv;
    }

    //////////////////////////////////////////
    void MAZE_GRAPHICS_API GenVerticesFromRawOBJ(
        Vector<Vec3DF>& _outPositions,
        Vector<Vec2DF>& _outUVs,
        Vector<Vec3DF>& _outNormals,
        Vector<Vec3DF> const& _positions,
        Vector<Vec2DF> const& _uvs,
        Vector<Vec3DF> const& _normals,
        String _line)
    {
        Vector<String> sface;
        Vector<String> svert;

        String tail = GetTail(_line);

        StringHelper::SplitWords(tail, sface, ' ');

        bool noNormal = false;

        // For every given vertex do this
        for (S32 i = 0; i < S32(sface.size()); ++i)
        {
            // See What type the vertex is.
            S32 vertexType;

            StringHelper::SplitWords(sface[i], svert, '/');

            // Check for just position - v1
            if (svert.size() == 1)
            {
                // Only position
                vertexType = 1;
            }

            // Check for position & texture - v1/vt1
            if (svert.size() == 2)
            {
                // Position & Texture
                vertexType = 2;
            }

            // Check for Position, Texture and Normal - v1/vt1/vn1
            // or if Position and Normal - v1//vn1
            if (svert.size() == 3)
            {
                if (svert[1] != "")
                {
                    // Position, Texture, and Normal
                    vertexType = 4;
                }
                else
                {
                    // Position & Normal
                    vertexType = 3;
                }
            }

            // Calculate and store the vertex
            switch (vertexType)
            {
                // P
                case 1:
                {
                    Vec3DF finalPosition = GetElement(_positions, svert[0]);
                    if (std::find(_outPositions.begin(), _outPositions.end(), finalPosition) == _outPositions.end())
                    {
                        Vec2DF finalUV = Vec2DF::c_zero;
                        noNormal = true;
                        _outPositions.push_back(finalPosition);
                        _outUVs.push_back(finalUV);
                        _outNormals.push_back(Vec3DF::c_zero);
                    }

                    break;
                }
                // P/T
                case 2:
                {
                    Vec3DF finalPosition = GetElement(_positions, svert[0]);
                    if (std::find(_outPositions.begin(), _outPositions.end(), finalPosition) == _outPositions.end())
                    {
                        Vec2DF finalUV = NormalizeUV(GetElement(_uvs, svert[1]));
                        noNormal = true;
                        _outPositions.push_back(finalPosition);
                        _outUVs.push_back(finalUV);
                        _outNormals.push_back(Vec3DF::c_zero);
                    }

                    break;
                }
                // P//N
                case 3: 
                {
                    Vec3DF finalPosition = GetElement(_positions, svert[0]);
                    if (std::find(_outPositions.begin(), _outPositions.end(), finalPosition) == _outPositions.end())
                    {
                        Vec2DF finalUV = Vec2DF::c_zero;
                        Vec3DF finalNormal = GetElement(_normals, svert[2]);

                        _outPositions.push_back(finalPosition);
                        _outUVs.push_back(finalUV);
                        _outNormals.push_back(finalNormal);
                    }

                    break;
                }
                // P/T/N
                case 4:
                {
                    Vec3DF finalPosition = GetElement(_positions, svert[0]);
                    if (std::find(_outPositions.begin(), _outPositions.end(), finalPosition) == _outPositions.end())
                    {
                        Vec2DF finalUV = NormalizeUV(GetElement(_uvs, svert[1]));
                        Vec3DF finalNormal = GetElement(_normals, svert[2]);

                        _outPositions.push_back(finalPosition);
                        _outUVs.push_back(finalUV);
                        _outNormals.push_back(finalNormal);
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }
        }

        if (noNormal)
        {
            Vec3DF a = _outPositions[0] - _outPositions[1];
            Vec3DF b = _outPositions[2] - _outPositions[1];

            Vec3DF normal = a.crossProduct(b);

            for (S32 i = 0; i < S32(_outPositions.size()); ++i)
            {
                _outNormals[i] = normal;
            }
        }
    }

    //////////////////////////////////////////
    void ProcessVertexTriangluation(
        Vector<U32>& _outIndices,
        Vector<Vec3DF> const& _positions,
        Vector<Vec2DF> const& _uvs,
        Vector<Vec3DF> const& _normals)
    {
        // If there are 2 or less verts,
        // no triangle can be created,
        // so exit
        if (_positions.size() < 3)
        {
            return;
        }
        // If it is a triangle no need to calculate it
        if (_positions.size() == 3)
        {
            _outIndices.push_back(0);
            _outIndices.push_back(1);
            _outIndices.push_back(2);
            return;
        }

        // Create a list of vertices
        Vector<Vec3DF> positions = _positions;
        Vector<Vec2DF> uvs = _uvs;
        Vector<Vec3DF> normals = _normals;

        while (true)
        {
            // For every vertex
            for (S32 i = 0; i < S32(positions.size()); ++i)
            {
                Vec3DF prevPosition;
                Vec2DF prevUV;
                Vec3DF prevNormal;

                if (i == 0)
                {
                    prevPosition = positions[positions.size() - 1];
                }
                else
                {
                    prevPosition = positions[i - 1];
                }

                
                Vec3DF currentPosition = positions[i];

                // nextPosition = the next vertex in the list
                Vec3DF nextPosition;
                if (i == positions.size() - 1)
                {
                    nextPosition = positions[0];
                }
                else
                {
                    nextPosition = positions[i + 1];
                }

                // Check to see if there are only 3 verts left
                // if so this is the last triangle
                if (positions.size() == 3)
                {
                    for (S32 j = 0; j < S32(_positions.size()); ++j)
                    {
                        if (_positions[j] == currentPosition)
                            _outIndices.push_back(j);

                        if (_positions[j] == prevPosition)
                            _outIndices.push_back(j);

                        if (_positions[j] == nextPosition)
                            _outIndices.push_back(j);
                    }

                    positions.clear();
                    break;
                }
                if (positions.size() == 4)
                {
                    Vec3DF fourthPosition;
                    for (S32 j = 0; j < S32(positions.size()); ++j)
                    {
                        if (   positions[j] != currentPosition
                            && positions[j] != prevPosition
                            && positions[j] != nextPosition)
                        {
                            fourthPosition = positions[j];
                            break;
                        }
                    }

                    // Degenerate triangle 
                    if (Math::IsPointOnSegment(prevPosition, currentPosition, nextPosition) ||
                        Math::IsPointOnSegment(currentPosition, nextPosition, prevPosition) ||
                        Math::IsPointOnSegment(nextPosition, prevPosition, currentPosition))
                    {
                        std::swap(currentPosition, fourthPosition);
                    }


                    // Cur-Prev-Next
                    for (S32 j = 0; j < S32(_positions.size()); ++j)
                    {
                        if (_positions[j] == currentPosition)
                            _outIndices.push_back(j);

                        if (_positions[j] == prevPosition)
                            _outIndices.push_back(j);

                        if (_positions[j] == nextPosition)
                            _outIndices.push_back(j);
                    }

                    // Included triangle 
                    if (Math::IsPointOnSegment(prevPosition, currentPosition, fourthPosition) ||
                        Math::IsPointOnSegment(currentPosition, nextPosition, fourthPosition) ||
                        Math::IsPointOnSegment(nextPosition, prevPosition, fourthPosition))
                    {
                        positions.clear();
                        break;
                    }

                    // Prev-Next-Temp
                    for (S32 j = 0; j < S32(_positions.size()); ++j)
                    {
                        if (_positions[j] == prevPosition)
                            _outIndices.push_back(j);

                        if (_positions[j] == nextPosition)
                            _outIndices.push_back(j);

                        if (_positions[j] == fourthPosition)
                            _outIndices.push_back(j);
                    }

                    positions.clear();
                    break;
                }

                // If Vertex is not an interior vertex
                F32 angle = Vec3DF::GetAngleBetween(
                    prevPosition - currentPosition,
                    nextPosition - currentPosition);

                if (angle <= 0 && angle >= Math::c_pi)
                    continue;

                // If any vertices are within this triangle
                bool inTriangle = false;
                for (S32 j = 0; j < S32(positions.size()); ++j)
                {
                    if (Math::IsPointInTriangle(
                            positions[j], 
                            prevPosition,
                            currentPosition,
                            nextPosition)
                        && positions[j] != prevPosition
                        && positions[j] != currentPosition
                        && positions[j] != nextPosition)
                    {
                        inTriangle = true;
                        break;
                    }
                }

                if (inTriangle)
                    continue;

                // Create a triangle from currentPosition, prevPosition, nextPosition
                for (S32 j = 0; j < S32(_positions.size()); ++j)
                {
                    if (_positions[j] == currentPosition)
                        _outIndices.push_back(j);

                    if (_positions[j] == prevPosition)
                        _outIndices.push_back(j);

                    if (_positions[j] == nextPosition)
                        _outIndices.push_back(j);
                }

                // Delete currentPosition from the list
                for (S32 j = 0; j < S32(positions.size()); ++j)
                {
                    if (positions[j] == currentPosition)
                    {
                        positions.erase(positions.begin() + j);
                        break;
                    }
                }

                // Reset i to the start
                // -1 since loop will add 1 to it
                i = -1;
            }

            // If no triangles were created
            if (_outIndices.size() == 0)
                break;

            // If no more vertices
            if (positions.size() == 0)
                break;
        }
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadOBJ(
        ByteBufferPtr const& _fileData,
        MeshPtr const& _mesh,
        RenderMeshLoaderProperties const& _props)
    {
        _mesh->clear();

        Vector<Vec3DF> positions;
        Vector<Vec2DF> uvs;
        Vector<Vec3DF> normals;

        Vector<U32> indices;
        Vector<Vec3DF> finalPositions;
        Vector<Vec2DF> finalUVs;
        Vector<Vec3DF> finalNormals;

        bool listening = false;
        String meshName;

        auto processCreateSubMesh =
            [&]()
            {
                // Create Mesh
                SubMeshPtr subMesh = SubMesh::Create();
                subMesh->setName(meshName);

                // Flip X for LHCS
                for (S32 i = 0, in = (S32)indices.size(); i < in; i += 3)
                    std::swap(indices[i], indices[i + 2]);
                for (Vec3DF& finalPosition : finalPositions)
                {
                    finalPosition.x = -finalPosition.x;
                    finalPosition *= _props.scale;
                }
                for (Vec3DF& finalNormal : finalNormals)
                    finalNormal.x = -finalNormal.x;

                subMesh->setIndices(&indices[0], indices.size());
                subMesh->setPositions(&finalPositions[0], finalPositions.size());
                subMesh->setNormals(&finalNormals[0], finalNormals.size());
                subMesh->setTexCoords(0, &finalUVs[0], finalUVs.size());

                // Generate tangents and bitangents
                Vector<Vec3DF> tangents;
                Vector<Vec3DF> bitangents;
                if (SubMeshHelper::GenerateTangentsAndBitangents(
                    &indices[0],
                    indices.size(),
                    &finalPositions[0],
                    &finalUVs[0],
                    &finalNormals[0],
                    finalPositions.size(),
                    tangents,
                    bitangents))
                {
                    subMesh->setTangents(&tangents[0], tangents.size());
                    subMesh->setBitangents(&bitangents[0], bitangents.size());
                }

                _mesh->addSubMesh(subMesh);

                // Cleanup
                finalPositions.clear();
                finalUVs.clear();
                finalNormals.clear();
                indices.clear();
            };

        auto processOBJLine =
            [&](String& _line)
            {
                String firstToken = GetFirstToken(_line);
                String tail = GetTail(_line);

                if (   firstToken == "o" 
                    || firstToken == "g"
                    || _line[0] == 'g')
                {
                    if (!listening)
                    {
                        listening = true;

                        if (   firstToken == "o" 
                            || firstToken == "g")
                        {
                            meshName = tail;
                        }
                        else
                        {
                            meshName = "unnamed";
                        }
                    }
                    else
                    {
                        if (!indices.empty() && !finalPositions.empty())
                        {
                            // Create Mesh
                            processCreateSubMesh();

                            meshName = tail;
                        }
                        else
                        {
                            if (   firstToken == "o" 
                                || firstToken == "g")
                            {
                                meshName = tail;
                            }
                            else
                            {
                                meshName = "unnamed";
                            }
                        }
                    }
                }

                // Vertex Position
                if (firstToken == "v")
                {
                    Vector<String> words;
                    StringHelper::SplitWords(tail, words, ' ');

                    Vec3DF vertexPosition;
                    if (words.size() >= 3)
                    {
                        vertexPosition.x = StringHelper::StringToF32Safe(words[0]);
                        vertexPosition.y = StringHelper::StringToF32Safe(words[1]);
                        vertexPosition.z = StringHelper::StringToF32Safe(words[2]);

                        positions.push_back(vertexPosition);
                    }
                }
                else
                // Vertex UV
                if (firstToken == "vt")
                {
                    Vector<String> words;
                    StringHelper::SplitWords(tail, words, ' ');

                    Vec2DF uv;
                    if (words.size() >= 2)
                    {
                        uv.x = StringHelper::StringToF32Safe(words[0]);
                        uv.y = StringHelper::StringToF32Safe(words[1]);

                        uvs.push_back(uv);
                    }
                }
                else
                // Vertex Normal
                if (firstToken == "vn")
                {
                    Vector<String> words;
                    StringHelper::SplitWords(tail, words, ' ');

                    Vec3DF vertexNormal;
                    if (words.size() >= 3)
                    {
                        vertexNormal.x = StringHelper::StringToF32Safe(words[0]);
                        vertexNormal.y = StringHelper::StringToF32Safe(words[1]);
                        vertexNormal.z = StringHelper::StringToF32Safe(words[2]);

                        normals.push_back(vertexNormal);
                    }
                }
                else
                // Face
                if (firstToken == "f")
                {
                    Vector<Vec3DF> tempPositions;
                    Vector<Vec2DF> tempUVs;
                    Vector<Vec3DF> tempNormals;
                    GenVerticesFromRawOBJ(
                        tempPositions,
                        tempUVs,
                        tempNormals,
                        positions,
                        uvs,
                        normals,
                        _line);

                    for (Vec3DF const& tempPosition : tempPositions)
                        finalPositions.push_back(tempPosition);

                    for (Vec2DF const& tempUV : tempUVs)
                        finalUVs.push_back(tempUV);

                    for (Vec3DF const& tempNormal : tempNormals)
                        finalNormals.push_back(tempNormal);

                    Vector<U32> tempIndices;
                    ProcessVertexTriangluation(
                        tempIndices,
                        tempPositions,
                        tempUVs,
                        tempNormals);

                    for (S32 i = 0; i < S32(tempIndices.size()); i++)
                    {
                        U32 indnum = (U32)((finalPositions.size()) - tempPositions.size()) + tempIndices[i];
                        indices.push_back(indnum);
                    }
                }
                else
                // Material Name
                if (firstToken == "usemtl")
                {
                    if (!indices.empty() && !finalPositions.empty())
                    {
                        // Create Mesh
                        SubMeshPtr subMesh = SubMesh::Create();
                        subMesh->setName(meshName);

                        subMesh->setIndices(&indices[0], indices.size());
                        subMesh->setPositions(&finalPositions[0], finalPositions.size());
                        subMesh->setNormals(&finalNormals[0], finalNormals.size());
                        subMesh->setTexCoords(0, &finalUVs[0], finalUVs.size());

                        // Generate tangents and bitangents
                        Vector<Vec3DF> tangents;
                        Vector<Vec3DF> bitangents;
                        if (SubMeshHelper::GenerateTangentsAndBitangents(
                            &indices[0],
                            indices.size(),
                            &finalPositions[0],
                            &finalUVs[0],
                            &finalNormals[0],
                            finalPositions.size(),
                            tangents,
                            bitangents))
                        {
                            subMesh->setTangents(&tangents[0], tangents.size());
                            subMesh->setBitangents(&bitangents[0], bitangents.size());
                        }

                        _mesh->addSubMesh(subMesh);

                        S32 i = 2;
                        while (1) 
                        {
                            subMesh->setName(meshName + "_" + StringHelper::ToString(i));

                            for (Size subMeshIndex = 0; subMeshIndex < _mesh->getSubMeshesCount(); ++subMeshIndex)
                            {
                                SubMeshPtr const& subMesh2 = _mesh->getSubMesh(subMeshIndex);
                                if (subMesh2->getName() == subMesh->getName())
                                    continue;
                            }
                            break;
                        }

                        // Cleanup
                        finalPositions.clear();
                        finalUVs.clear();
                        finalNormals.clear();
                        indices.clear();
                    }
                }
                else
                // Material Name
                if (firstToken == "mtllib")
                {
                    // Skip materials
                }
            };

        String line;
        Size i = 0;
        while (i < _fileData->getSize())
        {
            Char c = _fileData->getByte(i);

            if (c == '\n')
            {
                processOBJLine(line);
                line.clear();
            }
            else
            if (c != '\r')
            {
                line += c;
            }

            ++i;
        }
        processOBJLine(line);

        if (!indices.empty() && !finalPositions.empty())
        {
            // Create Mesh
            processCreateSubMesh();
        }

        return true;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsOBJFile(AssetFilePtr const& _file)
    {
        // #TODO:
        return _file->getExtension() == "obj";
        // ByteBufferPtr fileData = _file->readAsByteBuffer();
        // return IsOBJFile(fileData);        
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsOBJFile(ByteBufferPtr const& _fileData)
    {
        // #TODO:
        MAZE_NOT_IMPLEMENTED;
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
