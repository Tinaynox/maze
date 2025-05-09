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
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderMZMESH.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinMeshType);


    //////////////////////////////////////////
    // Class MeshManager
    //
    //////////////////////////////////////////
    MeshManager* MeshManager::s_instance = nullptr;

    //////////////////////////////////////////
    MeshManager::MeshManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    MeshManager::~MeshManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void MeshManager::Initialize(MeshManagerPtr& _meshManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshManager, _meshManager, init());
    }


    //////////////////////////////////////////
    bool MeshManager::init()
    {

        registerMeshLoader(
            MAZE_HASHED_CSTRING("obj"),
            MeshLoaderData(
                (LoadMeshAssetFileFunction)&LoadOBJ,
                (LoadMeshByteBufferFunction)&LoadOBJ,
                (IsMeshAssetFileFunction)&IsOBJFile,
                (IsMeshByteBufferFunction)&IsOBJFile));

        registerMeshLoader(
            MAZE_HASHED_CSTRING("mzmesh"),
            MeshLoaderData(
                (LoadMeshAssetFileFunction)&LoadMZMESH,
                (LoadMeshByteBufferFunction)&LoadMZMESH,
                (IsMeshAssetFileFunction)&IsMZMESHFile,
                (IsMeshByteBufferFunction)&IsMZMESHFile));

        return true;
    }

    //////////////////////////////////////////
    MeshPtr const& MeshManager::createBuiltinMesh(BuiltinMeshType _meshType)
    {
        MeshPtr& mesh = m_builtinMeshes[_meshType];

        switch (_meshType)
        {
            case BuiltinMeshType::Quad:
            {
                mesh = MeshHelper::CreateQuadMesh();
                break;
            }
            case BuiltinMeshType::QuadNullPivot:
            {
                mesh = MeshHelper::CreateQuadMesh(Vec2F::c_one, Vec2F(0.5f));
                break;
            }
            case BuiltinMeshType::Cube:
            {
                mesh = MeshHelper::CreateCubeMesh();
                break;
            }
            case BuiltinMeshType::Sphere:
            {
                mesh = MeshHelper::CreateSpherifiedCubeMesh();
                break;
            }
            case BuiltinMeshType::Cone:
            {
                mesh = MeshHelper::CreateConeMesh();
                break;
            }
            case BuiltinMeshType::Cylinder:
            {
                mesh = MeshHelper::CreateCylinderMesh();
                break;
            }
            case BuiltinMeshType::Torus:
            {
                mesh = MeshHelper::CreateTorusMesh();
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return mesh;
    }

    //////////////////////////////////////////
    MeshPtr const& MeshManager::ensureBuiltinMesh(BuiltinMeshType _meshType)
    {
        MeshPtr const& mesh = getBuiltinMesh(_meshType);
        if (mesh)
            return mesh;

        return createBuiltinMesh(_meshType);
    }

    //////////////////////////////////////////
    void MeshManager::createBuiltinMeshes()
    {
        MAZE_PROFILE_EVENT("MeshManager::createBuiltinMeshes");

        for (BuiltinMeshType t = BuiltinMeshType(1); t < BuiltinMeshType::MAX; ++t)
            ensureBuiltinMesh(t);
    }

    //////////////////////////////////////////
    Vector<HashedString> MeshManager::getMeshLoaderExtensions()
    {
        Vector<HashedString> result;
        for (StringKeyMap<MeshLoaderData>::iterator it = m_meshLoaders.begin(),
                                                    end = m_meshLoaders.end();
                                                    it != end;
                                                    ++it)
            result.push_back(HashedString(it.key()));

        return result;
    }

    //////////////////////////////////////////
    MeshPtr MeshManager::loadMesh(AssetFilePtr const& _assetFile)
    {
        MAZE_PROFILE_EVENT("MeshManager::loadMesh");

        MeshPtr mesh;

        if (!_assetFile)
            return mesh;

        mesh = Mesh::Create();

        Debug::Log("Loading render mesh: %s...", _assetFile->getFileName().toUTF8().c_str());
        Timer timer;

        DataBlock metaData;
        if (AssetManager::GetInstancePtr())
            AssetManager::GetInstancePtr()->loadMetaData(_assetFile, metaData);

        MeshLoaderProperties loaderProps;
        if (metaData.isParamExists(MAZE_HCS("scale")))
            loaderProps.scale = metaData.getF32(MAZE_HCS("scale"));

        if (metaData.isParamExists(MAZE_HCS("mergeSubMeshes")))
            loaderProps.mergeSubMeshes = metaData.getBool(MAZE_HCS("mergeSubMeshes"));

        Path tangentsFilePath = _assetFile->getFullPath() + ".mztangents";
        AssetFilePtr tangentsAsset = AssetManager::GetInstancePtr()->getAssetFileByFullPath(tangentsFilePath);
        if (tangentsAsset)
            loaderProps.tangentsData = tangentsAsset->readAsByteBuffer();

        if (metaData.isEmpty() || !metaData.isParamExists(MAZE_HCS("ext")))
        {
            String assetFileExtension = _assetFile->getExtension().toUTF8();
            bool loaderFound = false;

            if (!assetFileExtension.empty())
            {
                auto it = m_meshLoaders.find(assetFileExtension);
                if (it != m_meshLoaders.end())
                {
                    loaderFound = true;
                    MAZE_ERROR_IF(!it->second.loadMeshAssetFileFunc(*_assetFile.get(), *mesh.get(), loaderProps), "Mesh is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
                }
            }
            else
            {
                for (auto const& renderMeshLoaderData : m_meshLoaders)
                {
                    MeshLoaderData const& loaderData = renderMeshLoaderData.second;
                    if (loaderData.isMeshAssetFileFunc(*_assetFile.get()))
                    {
                        loaderFound = true;
                        MAZE_ERROR_IF(!loaderData.loadMeshAssetFileFunc(*_assetFile.get(), *mesh.get(), loaderProps), "Mesh is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
                        break;
                    }
                }
            }

            MAZE_ERROR_IF(!loaderFound, "Unsupported mesh format - %s!", _assetFile->getFileName().toUTF8().c_str());
        }
        else
        {
            HashedString fileExtension = HashedString(
                StringHelper::ToLower(metaData.getString(MAZE_HCS("ext"))));

            auto it = m_meshLoaders.find(fileExtension);
            if (it != m_meshLoaders.end())
            {
                MeshLoaderData const& loaderData = it->second;
                MAZE_ERROR_IF(!loaderData.loadMeshAssetFileFunc(*_assetFile.get(), *mesh.get(), loaderProps), "Mesh is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format - %s!", _assetFile->getFileName().toUTF8().c_str());
            }
        }

        F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
        Debug::Log("Render mesh %s loaded for %.1fms.", _assetFile->getFileName().toUTF8().c_str(), msTime);

        return mesh;
    }
    
} // namespace Maze
//////////////////////////////////////////
