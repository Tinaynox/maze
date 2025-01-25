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
#include "maze-graphics/MazeMeshSkeleton.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimation.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/system/MazeTimer.hpp"

#undef VOID
#include <ofbx.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    Size const c_fbxHeaderSize = 23;
    MAZE_CONSTEXPR CString const c_fbxHeaderTextBinary = "Kaydara FBX Binary";
    MAZE_CONSTEXPR CString const c_fbxHeaderTextASCII = "Kaydara FBX ASCII";


    //////////////////////////////////////////
    inline ofbx::u16 ConstructLoadFlags()
    {
        ofbx::LoadFlags flags =
            ofbx::LoadFlags::TRIANGULATE |
            // ofbx::LoadFlags::IGNORE_MODELS |
            // ofbx::LoadFlags::IGNORE_BLEND_SHAPES |
            ofbx::LoadFlags::IGNORE_CAMERAS |
            ofbx::LoadFlags::IGNORE_LIGHTS |
            ofbx::LoadFlags::IGNORE_TEXTURES |
            // ofbx::LoadFlags::IGNORE_SKIN |
            // ofbx::LoadFlags::IGNORE_BONES |
            ofbx::LoadFlags::IGNORE_PIVOTS |
            ofbx::LoadFlags::IGNORE_MATERIALS |
            // ofbx::LoadFlags::IGNORE_POSES |
            ofbx::LoadFlags::IGNORE_VIDEOS |
            ofbx::LoadFlags::IGNORE_LIMBS
            // ofbx::LoadFlags::IGNORE_MESHES
            // ofbx::LoadFlags::IGNORE_ANIMATIONS
            ;
        return (ofbx::u16)flags;
    }

    //////////////////////////////////////////
    inline Vec3F ConvertOpenFBXVec3ToVec3F(ofbx::Vec3 const& _value)
    {
        return Vec3F((F32)_value.x, (F32)_value.y, (F32)_value.z);
    }

    //////////////////////////////////////////
    inline TMat ConvertOpenFBXMatrixToTMat(ofbx::Matrix const& _mat)
    {
        return TMat(
            (F32)_mat.m[0], (F32)_mat.m[1], (F32)_mat.m[2],
            (F32)_mat.m[4], (F32)_mat.m[5], (F32)_mat.m[6],
            (F32)_mat.m[8], (F32)_mat.m[9], (F32)_mat.m[10],
            (F32)_mat.m[12], (F32)_mat.m[13], (F32)_mat.m[14]);
    }

    //////////////////////////////////////////
    inline MeshSkeletonAnimationCurve ConvertAnimationCurveToMeshSkeletonAnimationCurve(ofbx::AnimationCurve const* _curve)
    {
        if (!_curve || _curve->getKeyCount() == 0)
            return MeshSkeletonAnimationCurve();

        FastVector<F32> times;
        FastVector<F32> values;

        Size keyCount = Size(_curve->getKeyCount());
        times.resize(keyCount);
        values.resize(keyCount);

        memcpy(&values[0], _curve->getKeyValue(), sizeof(F32) * keyCount);
        for (Size i = 0; i < keyCount; ++i)
            times[i] = (F32)ofbx::fbxTimeToSeconds(_curve->getKeyTime()[i]);

        return MeshSkeletonAnimationCurve(std::move(times), std::move(values));
    }

    //////////////////////////////////////////
    inline MeshSkeletonRotationCurve ConvertRotationToMeshSkeletonRotationCurve(
        ofbx::AnimationCurveNode const* _rotation)
    {
        MeshSkeletonRotationCurve curve;

        Set<ofbx::i64> keyframes;
        for (S32 axis = 0; axis < 3; ++axis)
        {
            ofbx::AnimationCurve const* curve = _rotation->getCurve(axis);
            if (curve)
            {
                S32 count = curve->getKeyCount();
                for (S32 i = 0; i < count; ++i)
                    keyframes.insert(curve->getKeyTime()[i]);
            }
        }

        FastVector<F32> times;
        FastVector<Quaternion> values;

        for (ofbx::i64 keyTime : keyframes)
        {
            auto time = ofbx::fbxTimeToSeconds(keyTime);
            ofbx::Vec3 euler = _rotation->getNodeLocalTransform(time);
            
            Vec3F animationEuler = Vec3F(
                Math::DegreesToRadians((F32)euler.x),
                -(Math::DegreesToRadians((F32)euler.y)),
                -(Math::DegreesToRadians((F32)euler.z)));

            auto animationQuat =
                Quaternion::FromEuler(0.0f, 0.0f, animationEuler.z) *
                Quaternion::FromEuler(0.0f, animationEuler.y, 0.0f) *
                Quaternion::FromEuler(animationEuler.x, 0.0f, 0.0f);

            times.push_back((F32)time);
            values.push_back(animationQuat);
        }

        curve.setValues(
            std::move(times),
            std::move(values));

        return std::move(curve);
    }

    //////////////////////////////////////////
    inline void ProcessBoneForVertex(
        Vec4F& _blendWeights,
        Vec4F& _blendIndices,
        S32 _boneIndex,
        F32 _weight)
    {
        for (S32 i = 0; i < 4; ++i)
        {
            if (_blendWeights[i] == 0.0f)
            {
                _blendIndices[i] = F32(_boneIndex);
                _blendWeights[i] = _weight;
                return;
            }
        }

        // If more than 4 bones influence the vertex, find the smallest weight and replace it
        S32 smallestIndex = 0;
        for (S32 i = 1; i < 4; ++i)
            if (_blendWeights[i] < _blendWeights[smallestIndex])
                smallestIndex = i;

        if (_weight > _blendWeights[smallestIndex])
        {
            _blendIndices[smallestIndex] = F32(_boneIndex);
            _blendWeights[smallestIndex] = _weight;
        }
    }

    //////////////////////////////////////////
    inline void NormalizeBlendWeights(Vector<Vec4F>& _blendWeights)
    {
        for (auto& blendWeight : _blendWeights)
        {
            F32 totalWeight = blendWeight.x + blendWeight.y + blendWeight.z + blendWeight.w;

            if (totalWeight > 0.0f)
                blendWeight /= totalWeight;
        }
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

        Vector<U16> indices;
        Vector<Vec3F> positions;
        Vector<Vec2F> uvs;
        Vector<Vec4F> colors;
        Vector<Vec3F> normals;
        Vector<Vec3F> tangents;
        Vector<Vec4F> blendWeights;
        Vector<Vec4F> blendIndices;
        String meshName;

        struct BoneTempData
        {
            ofbx::Object const* object;
            TMat bindPoseMat;
        };

        Vector<BoneTempData> bonesData;

        auto processCreateSubMesh =
            [&]()
        {
            if (indices.empty() || positions.empty())
                return;

            // Create Mesh
            SubMeshPtr subMesh = SubMesh::Create();
            subMesh->setName(meshName);


            Vector<Vec3F> bitangents;

            // Flip X for LHCS
            SubMeshHelper::FlipX(
                subMesh->getRenderDrawTopology(),
                indices,
                &positions,
                &normals,
                &tangents);
         
            
            for (Vec3F& position : positions)
                position *= _props.scale * 0.01f;

            subMesh->setIndices(&indices[0], indices.size());
            subMesh->setPositions(&positions[0], positions.size());

            if (!normals.empty())
                subMesh->setNormals(&normals[0], normals.size());
            if (!uvs.empty())
                subMesh->setTexCoords(0, &uvs[0], uvs.size());
            if (!colors.empty())
                subMesh->setColors(&colors[0], colors.size());

#if 0
            if (!tangents.empty())
            {
                subMesh->setTangents(&tangents[0], tangents.size());

                bitangents.resize(tangents.size());
                for (S32 i = 0, in = (S32)bitangents.size(); i != in; ++i)
                    bitangents[i] = normals[i].crossProduct(tangents[i]);

                subMesh->setBitangents(&tangents[0], tangents.size());
            }
            else
#endif
            if (!normals.empty() && !uvs.empty())
            {
                Debug::LogWarning("SubMesh is not containing tangents info. Calculating...");
                Timer timer;

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

                F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
                Debug::LogWarning("Calculation is taken % .1fms", msTime);
            }

            if (!blendWeights.empty())
            {
                NormalizeBlendWeights(blendWeights);
                subMesh->setBlendWeights(&blendWeights[0], blendWeights.size());
            }

            if (!blendIndices.empty())
                subMesh->setBlendIndices(&blendIndices[0], blendIndices.size());

            _mesh.addSubMesh(subMesh);

            // Cleanup
            positions.clear();
            uvs.clear();
            normals.clear();
            tangents.clear();
            colors.clear();
            blendWeights.clear();
            blendIndices.clear();
            indices.clear();
        };

        TMat fixOrientationMat = TMat::c_identity;

        // Fix orientation for LHCS
        if (sceneGlobalSettings->CoordAxis == ofbx::CoordSystem::CoordSystem_RightHanded)
        {
            if (sceneGlobalSettings->UpAxis == ofbx::UpVector::UpVector_AxisY && sceneGlobalSettings->CoordAxisSign < 0)
                fixOrientationMat = TMat::CreateRotationY(Math::c_pi);
            else
            if (sceneGlobalSettings->UpAxis == ofbx::UpVector::UpVector_AxisZ && sceneGlobalSettings->CoordAxisSign > 0)
                fixOrientationMat = TMat::CreateRotationX(-Math::c_halfPi);

        }

        S32 indicesOffset = 0;

        S32 meshCount = _scene->getMeshCount();
        for (S32 meshI = 0; meshI < meshCount; ++meshI)
        {
            ofbx::Mesh const& mesh = *_scene->getMesh(meshI);
            meshName = mesh.name;

            ofbx::Geometry const& geom = *mesh.getGeometry();

            TMat meshGeometricTransformMat = ConvertOpenFBXMatrixToTMat(mesh.getGeometricMatrix());
            TMat meshLocalTransformMat = ConvertOpenFBXMatrixToTMat(mesh.getLocalTransform());
            TMat meshGlobalTransformMat = ConvertOpenFBXMatrixToTMat(mesh.getGlobalTransform());
            
            TMat transformMat = fixOrientationMat.transform(meshGeometricTransformMat).transform(meshGlobalTransformMat);
            

            

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
                Vec3F pos = Vec3F((F32)verticesPtr[i].x, (F32)verticesPtr[i].y, (F32)verticesPtr[i].z);
                pos = transformMat.transform(pos);
                positions.push_back(pos);
            }
            transformMat.setTranslation(Vec3F::c_zero);

            // Normals
            bool hasNormals = geom.getNormals() != nullptr;
            if (hasNormals)
            {
                ofbx::Vec3 const* normalsPtr = geom.getNormals();
                normals.reserve(normals.capacity() + vertexCount);
                for (S32 i = 0; i < vertexCount; ++i)
                {
                    Vec3F normal = Vec3F((F32)normalsPtr[i].x, (F32)normalsPtr[i].y, (F32)normalsPtr[i].z);
                    normal = transformMat.transform(normal);
                    normals.push_back(normal.normalizedCopy());
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
                    Vec3F tangent = Vec3F((F32)tangentsPtr[i].x, (F32)tangentsPtr[i].y, (F32)tangentsPtr[i].z);
                    tangent = transformMat.transform(tangent);
                    tangents.push_back(tangent.normalizedCopy());
                }
            }

            // UVs
            bool hasUVs = geom.getUVs() != nullptr;
            if (hasUVs)
            {
                ofbx::Vec2 const* uvsPtr = geom.getUVs();
                uvs.reserve(uvs.capacity() + vertexCount);
                for (S32 i = 0; i < vertexCount; ++i)
                {
                    Vec2F uv = SubMeshHelper::NormalizeUV(Vec2F((F32)uvsPtr[i].x, (F32)uvsPtr[i].y));
                    uvs.push_back(uv);
                }
            }

            // Colors
            bool hasColors = geom.getColors() != nullptr;
            if (hasColors)
            {
                ofbx::Vec4 const* colorsPtr = geom.getColors();
                colors.reserve(colors.capacity() + vertexCount);
                for (S32 i = 0; i < vertexCount; ++i)
                    colors.push_back(Vec4F((F32)colorsPtr[i].x, (F32)colorsPtr[i].y, (F32)colorsPtr[i].z, (F32)colorsPtr[i].w));
            }

            // Skin
            ofbx::Skin const* skin = geom.getSkin();
            if (skin)
            {
                MeshSkeletonPtr const& meshSkeleton = _mesh.ensureSkeleton();

                blendWeights.resize(blendWeights.size() + vertexCount, Vec4F(0.0f));
                blendIndices.resize(blendIndices.size() + vertexCount, Vec4F(0.0f));

                S32 const clusterCount = skin->getClusterCount();

                for (S32 clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
                {
                    ofbx::Cluster const* cluster = skin->getCluster(clusterIndex);
                    ofbx::Object const* bone = cluster->getLink(); // Bone corresponding to this cluster
                    if (!bone)
                        continue;

                    CString boneName = bone->name;

                    auto meshSkeletonBoneIndex = meshSkeleton->ensureBoneIndex(HashedCString(boneName));

                    MeshSkeleton::BoneIndex parentBoneIndex = -1;
                    if (bone->getParent() &&
                        bone->getParent()->getParent() &&
                        bone->getParent()->getParent()->getType() == ofbx::Object::Type::NULL_NODE)
                    {
                        parentBoneIndex = meshSkeleton->ensureBoneIndex(HashedCString(bone->getParent()->name));
                    }
                    
                    auto& meshSkeletonBone = meshSkeleton->getBone(meshSkeletonBoneIndex);
                    meshSkeletonBone.parentBoneIndex = parentBoneIndex;

                    //TMat boneLocalTransform = ConvertOpenFBXMatrixToTMat(bone->getLocalTransform());
                    TMat bindPoseTransformGlobal = ConvertOpenFBXMatrixToTMat(cluster->getTransformLinkMatrix());
                    meshSkeletonBone.inverseBindPoseTransform = bindPoseTransformGlobal.inversed();

                    if (meshSkeletonBoneIndex >= S32(bonesData.size()))
                        bonesData.resize((Size)meshSkeletonBoneIndex + 1);
                    bonesData[meshSkeletonBoneIndex].object = bone;
                    bonesData[meshSkeletonBoneIndex].bindPoseMat = bindPoseTransformGlobal;
                        

                    S32 const vertexCount = cluster->getIndicesCount();
                    if (vertexCount)
                    {
                        // Retrieve vertices influenced by this bone
                        S32 const* indices = cluster->getIndices(); // Indices of vertices affected by this bone
                        F64 const* weights = cluster->getWeights(); // Corresponding weights for each vertex

                        for (S32 i = 0; i < vertexCount; ++i)
                        {
                            S32 vertexIndex = indices[i]; // Vertex index in the mesh
                            MAZE_DEBUG_ASSERT(vertexIndex >= 0);
                            MAZE_DEBUG_ASSERT(vertexIndex < blendWeights.size());
                            MAZE_DEBUG_ASSERT(vertexIndex < blendIndices.size());

                            F32 weight = static_cast<F32>(weights[i]);

                            ProcessBoneForVertex(
                                blendWeights[vertexIndex],
                                blendIndices[vertexIndex],
                                S32(meshSkeletonBoneIndex),
                                weight);
                        }
                    }

                }
            }

            if (!_props.mergeSubMeshes)
                processCreateSubMesh();
            else
                indicesOffset += indexCount;
        }

        if(bonesData.size() > MAZE_SKELETON_BONES_MAX)
            Debug::LogError("Bones count overflow - %d/%d",
                (S32)bonesData.size(),
                MAZE_SKELETON_BONES_MAX);

        if (_mesh.getSkeleton())
        {
            Vec3F armatureEuler = Vec3F::c_zero;
            Vec3F armatureScale = Vec3F::c_one;

            TMat armatureInvScaleMat = TMat::c_identity;

            S32 bonesCount = S32(bonesData.size());
            for (MeshSkeleton::BoneIndex boneIndex = 0; boneIndex < bonesCount; ++boneIndex)
            {
                ofbx::Object const* bone = bonesData[boneIndex].object;

                // Find armature
                if (bone->getParent() &&
                    bone->getParent()->getParent() &&
                    bone->getParent()->getParent()->getType() == ofbx::Object::Type::ROOT)
                {
                    ofbx::Object const* armature = bone->getParent();

                    armatureScale = ConvertOpenFBXVec3ToVec3F(armature->getLocalScaling());
                    armatureEuler = ConvertOpenFBXVec3ToVec3F(armature->getLocalRotation());

                    armatureInvScaleMat = TMat::CreateScale(1.0f / armatureScale);

                    TMat armatureGlobalTransformMat = ConvertOpenFBXMatrixToTMat(armature->getGlobalTransform());
                    Quaternion armatureGlobalTransformQuat = Quaternion(armatureGlobalTransformMat);
                    armatureEuler = armatureGlobalTransformQuat.getEuler();
                    _mesh.getSkeleton()->setRootTransform(
                        armatureGlobalTransformMat * armatureInvScaleMat);
                }
            }

            // Fix bone bind pose scaling
            for (MeshSkeleton::BoneIndex boneIndex = 0; boneIndex < bonesCount; ++boneIndex)
            {
                MeshSkeleton::Bone& bone = _mesh.getSkeleton()->getBone(boneIndex);

                TMat bindPoseTransform = bonesData[boneIndex].bindPoseMat * armatureInvScaleMat;

                // Flip bind pose X
                Quaternion bindPoseQuaternion(bindPoseTransform);
                bindPoseQuaternion.y = -bindPoseQuaternion.y;
                bindPoseQuaternion.z = -bindPoseQuaternion.z;
                TMat newBindPoseTransformGlobal = bindPoseQuaternion.toRotationMatrix();
                newBindPoseTransformGlobal[3] = Vec3F(-bindPoseTransform[3].x, bindPoseTransform[3].y, bindPoseTransform[3].z);

                bone.inverseBindPoseTransform = newBindPoseTransformGlobal.inversed();
            }

            // Load animations
            S32 animationsCount = _scene->getAnimationStackCount();
            for (S32 i = 0; i < animationsCount; ++i)
            {
                ofbx::AnimationStack const* animStack = _scene->getAnimationStack(i);
                if (animStack)
                {
                    CString animName = animStack->name;

                    ofbx::TakeInfo const* takeInfo = _scene->getTakeInfo(animName);

                    // Take animations from layer 0 only
                    ofbx::AnimationLayer const* animLayer = animStack->getLayer(0);
                    if (animLayer && takeInfo)
                    {
                        F32 animationTime = F32(takeInfo->local_time_to - takeInfo->local_time_from);

                        String meshSkeletonAnimationName = animName;
                        Size meshSkeletonAnimationNamePrefixIndex = meshSkeletonAnimationName.find_last_of('|');
                        if (meshSkeletonAnimationNamePrefixIndex != String::npos)
                            meshSkeletonAnimationName = meshSkeletonAnimationName.substr(meshSkeletonAnimationNamePrefixIndex + 1);

                        MeshSkeletonAnimationPtr const& meshSkeletonAnimation = _mesh.getSkeleton()->ensureAnimation(meshSkeletonAnimationName);
                        meshSkeletonAnimation->setAnimationTime(animationTime);

                        Vector<MeshSkeletonAnimationBone> meshSkeletonAnimationBones;

                        for (MeshSkeleton::BoneIndex boneIndex = 0; boneIndex < bonesCount; ++boneIndex)
                        {
                            MeshSkeletonAnimationBone meshSkeletonAnimationBone;

                            ofbx::Object const* bone = bonesData[boneIndex].object;
                            auto& meshSkeletonBone = _mesh.getSkeleton()->getBone(boneIndex);

                            ofbx::AnimationCurveNode const* translation = animLayer->getCurveNode(*bone, "Lcl Translation");
                            ofbx::AnimationCurveNode const* rotation = animLayer->getCurveNode(*bone, "Lcl Rotation");
                            ofbx::AnimationCurveNode const* scaling = animLayer->getCurveNode(*bone, "Lcl Scaling");

                            for (S32 axis = 0; axis < 3; ++axis)
                            {
                                if (translation)
                                {
                                    meshSkeletonAnimationBone.translation[axis] =
                                        ConvertAnimationCurveToMeshSkeletonAnimationCurve(translation->getCurve(axis));

                                    // Flip X
                                    if (axis == 0)
                                        meshSkeletonAnimationBone.translation[axis].modifyValues(
                                            [](F32& _value) { _value = -_value; });
                                }


                                if (scaling)
                                {
                                    meshSkeletonAnimationBone.scale[axis] =
                                        ConvertAnimationCurveToMeshSkeletonAnimationCurve(scaling->getCurve(axis));
                                }
                            }

                            if (rotation)
                            {
                                meshSkeletonAnimationBone.rotation =
                                    ConvertRotationToMeshSkeletonRotationCurve(rotation);
                            }
                            
                            meshSkeletonAnimationBones.emplace_back(std::move(meshSkeletonAnimationBone));
                        }

                        meshSkeletonAnimation->setBoneAnimations(std::move(meshSkeletonAnimationBones));
                    }
                }
            }
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
        MAZE_PROFILE_EVENT("LoadFBX");

        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");

        ofbx::u16 flags = ConstructLoadFlags();        
        ofbx::IScene* scene = ofbx::load(_fileData.getDataRO(), (S32)_fileData.getSize(), flags);
        bool result = LoadFBX(scene, _mesh, _props);
        if (scene)
            scene->destroy();

        return result;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_FBX_API bool IsFBXFile(
        AssetFile const& _file)
    {
        ByteBuffer fileData;
        _file.readHeaderToByteBuffer(fileData, c_fbxHeaderSize);
        return IsFBXFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_FBX_API bool IsFBXFile(
        ByteBuffer const& _fileData)
    {
        if (_fileData.getSize() < c_fbxHeaderSize)
            return false;

        bool isFBX = (strncmp((CString)_fileData.getDataRO(), c_fbxHeaderTextBinary, strlen(c_fbxHeaderTextBinary)) == 0 ||
                      strncmp((CString)_fileData.getDataRO(), c_fbxHeaderTextASCII, strlen(c_fbxHeaderTextASCII)) == 0);
        return isFBX;
    }

} // namespace Maze
//////////////////////////////////////////
