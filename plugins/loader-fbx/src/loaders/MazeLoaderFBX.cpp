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
#include "MazeLoaderFBXHeader.hpp"
#include "maze-plugin-loader-fbx/loaders/MazeLoaderFBX.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"

#undef VOID
#include <ofbx.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    inline ofbx::u16 ConstuctLoadFlags()
    {
        ofbx::LoadFlags flags =
            ofbx::LoadFlags::TRIANGULATE |
            // ofbx::LoadFlags::IGNORE_MODELS |
            ofbx::LoadFlags::IGNORE_BLEND_SHAPES |
            ofbx::LoadFlags::IGNORE_CAMERAS |
            ofbx::LoadFlags::IGNORE_LIGHTS |
            ofbx::LoadFlags::IGNORE_TEXTURES |
            ofbx::LoadFlags::IGNORE_SKIN |
            ofbx::LoadFlags::IGNORE_BONES |
            ofbx::LoadFlags::IGNORE_PIVOTS |
            ofbx::LoadFlags::IGNORE_MATERIALS |
            ofbx::LoadFlags::IGNORE_POSES |
            ofbx::LoadFlags::IGNORE_VIDEOS |
            ofbx::LoadFlags::IGNORE_LIMBS |
            // ofbx::LoadFlags::IGNORE_MESHES |
            ofbx::LoadFlags::IGNORE_ANIMATIONS;
        return (ofbx::u16)flags;
    }

    //////////////////////////////////////////
    inline Mat4DF ConvertOpenFBXMatrixToMat4DF(ofbx::Matrix const& _mat)
    {
        return Mat4DF(
            (F32)_mat.m[0], (F32)_mat.m[4], (F32)_mat.m[8], (F32)_mat.m[12],
            (F32)_mat.m[1], (F32)_mat.m[5], (F32)_mat.m[9], (F32)_mat.m[13],
            (F32)_mat.m[2], (F32)_mat.m[6], (F32)_mat.m[10], (F32)_mat.m[14],
            (F32)_mat.m[3], (F32)_mat.m[7], (F32)_mat.m[11], (F32)_mat.m[15]);
    }

    //////////////////////////////////////////
    static bool LoadFBX(
        ofbx::IScene* _scene,
        Mesh& _mesh,
        MeshLoaderProperties const& _props)
    {
        if (!_scene)
            return false;

        _mesh.clear();

        ofbx::GlobalSettings const* sceneGlobalSettings = _scene->getGlobalSettings();

        Vector<U32> indices;
        Vector<Vec3DF> positions;
        Vector<Vec2DF> uvs;
        Vector<Vec4DF> colors;
        Vector<Vec3DF> normals;
        Vector<Vec3DF> tangents;
        String meshName;

        auto processCreateSubMesh =
            [&]()
        {
            if (indices.empty() || positions.empty())
                return;

            // Create Mesh
            SubMeshPtr subMesh = SubMesh::Create();
            subMesh->setName(meshName);


            Vector<Vec3DF> bitangents;

            // Flip X for LHCS
            SubMeshHelper::FlipX(
                subMesh->getRenderDrawTopology(),
                indices,
                &positions,
                &normals,
                &tangents);
         
            
            for (Vec3DF& position : positions)
                position *= _props.scale * 0.01f;

            subMesh->setIndices(&indices[0], indices.size());
            subMesh->setPositions(&positions[0], positions.size());

            if (!normals.empty())
                subMesh->setNormals(&normals[0], normals.size());
            if (!uvs.empty())
                subMesh->setTexCoords(0, &uvs[0], uvs.size());
            if (!colors.empty())
                subMesh->setColors(&colors[0], colors.size());

            if (!tangents.empty())
            {
                subMesh->setTangents(&tangents[0], tangents.size());

                bitangents.resize(tangents.size());
                for (S32 i = 0, in = bitangents.size(); i != in; ++i)
                    bitangents[i] = normals[i].crossProduct(tangents[i]);

                subMesh->setBitangents(&tangents[0], tangents.size());
            }
            else
            if (!normals.empty() && !uvs.empty())
            {
                // Generate tangents and bitangents
                if (SubMeshHelper::GenerateTangentsAndBitangents(
                    &indices[0],
                    indices.size(),
                    &positions[0],
                    &uvs[0],
                    &normals[0],
                    positions.size(),
                    tangents,
                    bitangents))
                {
                    subMesh->setTangents(&tangents[0], tangents.size());
                    subMesh->setBitangents(&bitangents[0], bitangents.size());
                }
            }

            _mesh.addSubMesh(subMesh);

            // Cleanup
            positions.clear();
            uvs.clear();
            normals.clear();
            tangents.clear();
            colors.clear();
            indices.clear();
        };

        Mat4DF fixOrientationMat = Mat4DF::c_identity;

        // Fix orientation for LHCS
        if (sceneGlobalSettings->CoordAxis == ofbx::CoordSystem::CoordSystem_RightHanded)
        {
            if (sceneGlobalSettings->UpAxis == ofbx::UpVector::UpVector_AxisY && sceneGlobalSettings->CoordAxisSign < 0)
                fixOrientationMat = Mat4DF::CreateRotationYMatrix(Math::c_pi);
            else
            if (sceneGlobalSettings->UpAxis == ofbx::UpVector::UpVector_AxisZ && sceneGlobalSettings->CoordAxisSign > 0)
                fixOrientationMat = Mat4DF::CreateRotationXMatrix(-Math::c_halfPi);

        }

        S32 indicesOffset = 0;

        S32 meshCount = _scene->getMeshCount();
        for (S32 i = 0; i < meshCount; ++i)
        {
            ofbx::Mesh const& mesh = *_scene->getMesh(i);
            ofbx::Geometry const& geom = *mesh.getGeometry();

            ofbx::Matrix meshGeometricTransform = mesh.getGeometricMatrix();
            ofbx::Matrix meshGlobalTransform = mesh.getGlobalTransform();

            Mat4DF meshGeometricTransformMat = ConvertOpenFBXMatrixToMat4DF(meshGeometricTransform);
            Mat4DF meshGlobalTransformMat = ConvertOpenFBXMatrixToMat4DF(meshGlobalTransform);

            Mat4DF transformMat = fixOrientationMat * meshGeometricTransformMat * meshGlobalTransformMat;


            

            meshName = mesh.name;

            // Indices
            S32 indexCount = geom.getIndexCount();
            S32 const* indicesPtr = (S32 const*)geom.getFaceIndices();
            indices.reserve(indices.capacity() + indexCount);
            for (S32 i = 0; i < indexCount; ++i)
            {
                S32 idx = indicesPtr[i];
                idx = (idx < 0 ? -idx - 1 : idx) + indicesOffset;
                indices.push_back((U32)idx);
            }

            // Positions
            S32 vertexCount = geom.getVertexCount();
            ofbx::Vec3 const* verticesPtr = geom.getVertices();
            positions.reserve(positions.capacity() + vertexCount);
            for (S32 i = 0; i < vertexCount; ++i)
            {
                Vec3DF pos = Vec3DF((F32)verticesPtr[i].x, (F32)verticesPtr[i].y, (F32)verticesPtr[i].z);
                pos = transformMat.transformAffine(pos);
                positions.push_back(pos);
            }
            transformMat.setTranslation(Vec3DF::c_zero);

            // Normals
            bool hasNormals = geom.getNormals() != nullptr;
            if (hasNormals)
            {
                ofbx::Vec3 const* normalsPtr = geom.getNormals();
                normals.reserve(normals.capacity() + vertexCount);
                for (S32 i = 0; i < vertexCount; ++i)
                {
                    Vec3DF normal = Vec3DF((F32)normalsPtr[i].x, (F32)normalsPtr[i].y, (F32)normalsPtr[i].z);
                    normal = transformMat.transformAffine(normal);
                    normals.push_back(normal);
                }
            }

            // Tangents
            bool hasTangents = geom.getTangents() != nullptr;
            if (hasTangents)
            {
                ofbx::Vec3 const* tangentsPtr = geom.getTangents();
                tangents.reserve(tangents.capacity() + vertexCount);
                for (S32 i = 0; i < vertexCount; ++i)
                {
                    Vec3DF tangent = Vec3DF((F32)tangentsPtr[i].x, (F32)tangentsPtr[i].y, (F32)tangentsPtr[i].z);
                    tangent = transformMat.transformAffine(tangent);
                    tangents.push_back(tangent);
                }
            }

            // UVs
            bool hasUVs = geom.getUVs() != nullptr;
            if (hasUVs)
            {
                ofbx::Vec2 const* uvsPtr = geom.getUVs();
                uvs.reserve(uvs.capacity() + vertexCount);
                for (S32 i = 0; i < vertexCount; ++i)
                    uvs.push_back(Vec2DF((F32)uvsPtr[i].x, (F32)uvsPtr[i].y));
            }

            // Colors
            bool hasColors = geom.getColors() != nullptr;
            if (hasColors)
            {
                ofbx::Vec4 const* colorsPtr = geom.getColors();
                colors.reserve(colors.capacity() + vertexCount);
                for (S32 i = 0; i < vertexCount; ++i)
                    colors.push_back(Vec4DF((F32)colorsPtr[i].x, (F32)colorsPtr[i].y, (F32)colorsPtr[i].z, (F32)colorsPtr[i].w));
            }

            if (!_props.mergeSubMeshes)
                processCreateSubMesh();
            else
                indicesOffset += indexCount;
        }

        if (_props.mergeSubMeshes)
            processCreateSubMesh();

        return true;
    }

    
    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_FBX_API bool LoadFBX(
        AssetFile const& _file,
        Mesh& _mesh,
        MeshLoaderProperties const& _props)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadFBX(fileData, _mesh, _props);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_FBX_API bool LoadFBX(
        ByteBuffer const& _fileData,
        Mesh& _mesh,
        MeshLoaderProperties const& _props)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");

        ofbx::u16 flags = ConstuctLoadFlags();
        ofbx::IScene* scene = ofbx::load(_fileData.getData(), _fileData.getSize(), flags);
        bool result = LoadFBX(scene, _mesh, _props);
        if (scene)
            scene->destroy();

        return result;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_FBX_API bool IsFBXFile(
        AssetFile const& _file)
    {
        // #TODO:
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return IsFBXFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_FBX_API bool IsFBXFile(
        ByteBuffer const& _fileData)
    {
        // #TODO: Optimize?
        ofbx::u16 flags = ConstuctLoadFlags();
        ofbx::IScene* scene = ofbx::load(_fileData.getData(), _fileData.getSize(), flags);
        bool result = (scene != nullptr);
        if (scene)
            scene->destroy();
        return result;
    }

} // namespace Maze
//////////////////////////////////////////
