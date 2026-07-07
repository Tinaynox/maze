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
#include "maze-graphics/MazeMeshSkeleton.hpp"
#include "maze-graphics/MazeMeshSkeletonAnimation.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #pragma warning(push)
    #pragma warning(disable:4307)
    MAZE_CONSTEXPR U32 const c_mzMeshHeaderMagic = Hash::CalculateFNV1("MZMESH");
    MAZE_CONSTEXPR S32 const c_mzMeshHeaderVersion = 2;
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
        SkeletonStart       = 5,
        SkeletonEnd         = 6,
    };


    //////////////////////////////////////////
    inline MeshSkeletonAnimationCurve LoadMZMESHAnimationCurve(
        ByteBuffer const& _fileData,
        U32& _bytesRead)
    {
        U32 keysCount = 0u;
        _bytesRead += _fileData.read(_bytesRead, &keysCount, sizeof(keysCount));

        FastVector<F32> times((Size)keysCount, 0.0f);
        FastVector<F32> values((Size)keysCount, 0.0f);

        if (keysCount > 0u)
        {
            _bytesRead += _fileData.read(_bytesRead, times.begin(), keysCount * sizeof(F32));
            _bytesRead += _fileData.read(_bytesRead, values.begin(), keysCount * sizeof(F32));
        }

        return MeshSkeletonAnimationCurve(std::move(times), std::move(values));
    }

    //////////////////////////////////////////
    inline MeshSkeletonRotationCurve LoadMZMESHRotationCurve(
        ByteBuffer const& _fileData,
        U32& _bytesRead)
    {
        U32 keysCount = 0u;
        _bytesRead += _fileData.read(_bytesRead, &keysCount, sizeof(keysCount));

        FastVector<F32> times((Size)keysCount, 0.0f);
        FastVector<Quaternion> values((Size)keysCount, Quaternion::c_identity);

        if (keysCount > 0u)
        {
            _bytesRead += _fileData.read(_bytesRead, times.begin(), keysCount * sizeof(F32));
            _bytesRead += _fileData.read(_bytesRead, values.begin(), keysCount * sizeof(Quaternion));
        }

        return MeshSkeletonRotationCurve(std::move(times), std::move(values));
    }


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

        // Skeleton (optional, appended after submeshes)
        if (header.version >= 2 && bytesRead < _fileData.getSize())
        {
            MZMESHTag tag = MZMESHTag::None;
            bytesRead += _fileData.read(bytesRead, &tag, sizeof(tag));
            MAZE_ERROR_RETURN_VALUE_IF(tag != MZMESHTag::SkeletonStart, false, "MZMESH structure broken");

            MeshSkeletonPtr const& skeleton = _mesh.ensureSkeleton();

            S32 bonesCount = 0;
            bytesRead += _fileData.read(bytesRead, &bonesCount, sizeof(bonesCount));
            for (S32 i = 0; i < bonesCount; ++i)
            {
                U16 nameLength = 0u;
                bytesRead += _fileData.read(bytesRead, &nameLength, sizeof(nameLength));
                String boneName;
                boneName.resize((Size)nameLength);
                bytesRead += _fileData.read(bytesRead, (void*)boneName.data(), (Size)nameLength);

                MeshSkeleton::BoneIndex parentBoneIndex = -1;
                bytesRead += _fileData.read(bytesRead, &parentBoneIndex, sizeof(parentBoneIndex));

                TMat inversedBindPoseTransformMS = TMat::c_identity;
                bytesRead += _fileData.read(bytesRead, &inversedBindPoseTransformMS, sizeof(TMat));

                skeleton->addBone(boneName, inversedBindPoseTransformMS, parentBoneIndex);
            }

            TMat rootTransform = TMat::c_identity;
            bytesRead += _fileData.read(bytesRead, &rootTransform, sizeof(TMat));
            skeleton->setRootTransform(rootTransform);

            S32 animationsCount = 0;
            bytesRead += _fileData.read(bytesRead, &animationsCount, sizeof(animationsCount));
            for (S32 i = 0; i < animationsCount; ++i)
            {
                U16 nameLength = 0u;
                bytesRead += _fileData.read(bytesRead, &nameLength, sizeof(nameLength));
                String animationName;
                animationName.resize((Size)nameLength);
                bytesRead += _fileData.read(bytesRead, (void*)animationName.data(), (Size)nameLength);

                F32 animationTime = 0.0f;
                bytesRead += _fileData.read(bytesRead, &animationTime, sizeof(animationTime));

                MeshSkeletonAnimationPtr const& animation = skeleton->ensureAnimation(animationName);
                animation->setAnimationTime(animationTime);

                Vector<MeshSkeletonAnimationBone> boneAnimations((Size)bonesCount);
                for (S32 b = 0; b < bonesCount; ++b)
                {
                    MeshSkeletonAnimationBone& boneAnimation = boneAnimations[(Size)b];

                    for (S32 axis = 0; axis < 3; ++axis)
                        boneAnimation.translation[axis] = LoadMZMESHAnimationCurve(_fileData, bytesRead);

                    boneAnimation.rotation = LoadMZMESHRotationCurve(_fileData, bytesRead);

                    for (S32 axis = 0; axis < 3; ++axis)
                        boneAnimation.scale[axis] = LoadMZMESHAnimationCurve(_fileData, bytesRead);
                }

                animation->setBoneAnimations(std::move(boneAnimations));
            }

            // End tag
            bytesRead += _fileData.read(bytesRead, &tag, sizeof(tag));
            MAZE_ERROR_RETURN_VALUE_IF(tag != MZMESHTag::SkeletonEnd, false, "MZMESH structure broken");
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
    inline void SaveMZMESHAnimationCurve(
        std::ofstream& _outputFile,
        MeshSkeletonAnimationCurve const& _curve)
    {
        U32 keysCount = (U32)_curve.getValues().size();
        _outputFile.write((S8 const*)&keysCount, sizeof(keysCount));

        if (keysCount > 0u)
        {
            _outputFile.write((S8 const*)_curve.getTimes().begin(), keysCount * sizeof(F32));
            _outputFile.write((S8 const*)_curve.getValues().begin(), keysCount * sizeof(F32));
        }
    }

    //////////////////////////////////////////
    inline void SaveMZMESHRotationCurve(
        std::ofstream& _outputFile,
        MeshSkeletonRotationCurve const& _curve)
    {
        U32 keysCount = (U32)_curve.getValues().size();
        _outputFile.write((S8 const*)&keysCount, sizeof(keysCount));

        if (keysCount > 0u)
        {
            _outputFile.write((S8 const*)_curve.getTimes().begin(), keysCount * sizeof(F32));
            _outputFile.write((S8 const*)_curve.getValues().begin(), keysCount * sizeof(Quaternion));
        }
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

        // Skeleton (optional, appended after submeshes)
        MeshSkeletonPtr const& skeleton = _mesh.getSkeleton();
        if (skeleton && skeleton->getBonesCount() > 0)
        {
            tag = MZMESHTag::SkeletonStart;
            outputFile.write((S8 const*)&tag, sizeof(tag));

            S32 bonesCount = (S32)skeleton->getBonesCount();
            outputFile.write((S8 const*)&bonesCount, sizeof(bonesCount));
            for (S32 i = 0; i < bonesCount; ++i)
            {
                MeshSkeleton::Bone const& bone = skeleton->getBone(i);

                U16 nameLength = (U16)bone.name.size();
                outputFile.write((S8 const*)&nameLength, sizeof(nameLength));
                outputFile.write(bone.name.c_str(), (Size)nameLength);

                outputFile.write((S8 const*)&bone.parentBoneIndex, sizeof(bone.parentBoneIndex));
                outputFile.write((S8 const*)&bone.inversedBindPoseTransformMS, sizeof(TMat));
            }

            TMat rootTransform = skeleton->getRootTransform();
            outputFile.write((S8 const*)&rootTransform, sizeof(TMat));

            StringKeyMap<MeshSkeletonAnimationPtr> const& animations = skeleton->getAnimations();
            S32 animationsCount = (S32)animations.size();
            outputFile.write((S8 const*)&animationsCount, sizeof(animationsCount));
            for (auto const& animationEntry : animations)
            {
                MeshSkeletonAnimationPtr const& animation = animationEntry.second;

                U16 nameLength = (U16)animation->getName().size();
                outputFile.write((S8 const*)&nameLength, sizeof(nameLength));
                outputFile.write(animation->getName().c_str(), (Size)nameLength);

                F32 animationTime = animation->getAnimationTime();
                outputFile.write((S8 const*)&animationTime, sizeof(animationTime));

                Vector<MeshSkeletonAnimationBone> const& boneAnimations = animation->getBoneAnimations();
                static MeshSkeletonAnimationBone const c_emptyBoneAnimation;
                for (S32 b = 0; b < bonesCount; ++b)
                {
                    MeshSkeletonAnimationBone const& boneAnimation =
                        (b < (S32)boneAnimations.size()) ? boneAnimations[(Size)b] : c_emptyBoneAnimation;

                    for (S32 axis = 0; axis < 3; ++axis)
                        SaveMZMESHAnimationCurve(outputFile, boneAnimation.translation[axis]);

                    SaveMZMESHRotationCurve(outputFile, boneAnimation.rotation);

                    for (S32 axis = 0; axis < 3; ++axis)
                        SaveMZMESHAnimationCurve(outputFile, boneAnimation.scale[axis]);
                }
            }

            // End tag
            tag = MZMESHTag::SkeletonEnd;
            outputFile.write((S8 const*)&tag, sizeof(tag));
        }

        outputFile.close();

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
