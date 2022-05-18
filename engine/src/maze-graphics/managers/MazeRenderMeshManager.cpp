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
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinRenderMeshType);


    //////////////////////////////////////////
    // Class RenderMeshManager
    //
    //////////////////////////////////////////
    RenderMeshManager::RenderMeshManager()
        : m_renderSystemRaw(nullptr)
    {
        
    }

    //////////////////////////////////////////
    RenderMeshManager::~RenderMeshManager()
    {
        if (m_renderSystemRaw)
            m_renderSystemRaw->eventSystemInited.unsubscribe(this);
    }

    //////////////////////////////////////////
    void RenderMeshManager::Initialize(RenderMeshManagerPtr& _renderMeshManager, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderMeshManager, _renderMeshManager, init(_renderSystem));
    }

    //////////////////////////////////////////
    RenderMeshManagerPtr const& RenderMeshManager::GetCurrentInstancePtr()
    {
        return RenderSystem::GetCurrentInstancePtr()->getRenderMeshManager();
    }

    //////////////////////////////////////////
    bool RenderMeshManager::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        m_renderSystemRaw->eventSystemInited.subscribe(this, &RenderMeshManager::notifyRenderSystemInited);

        registerRenderMeshLoader(
            MAZE_HASHED_CSTRING("obj"),
            RenderMeshLoaderData(
                (LoadRenderMeshAssetFileFunction)&LoadOBJ,
                (LoadRenderMeshByteBufferFunction)&LoadOBJ,
                (IsRenderMeshAssetFileFunction)&IsOBJFile,
                (IsRenderMeshByteBufferFunction)&IsOBJFile));

        return true;
    }

    //////////////////////////////////////////
    void RenderMeshManager::notifyRenderSystemInited()
    {
        // createBuiltinRenderMeshes();
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::createBuiltinRenderMesh(BuiltinRenderMeshType _renderMeshType)
    {
        RenderMeshPtr& renderMesh = m_builtinRenderMeshes[_renderMeshType];

        switch (_renderMeshType)
        {
            case BuiltinRenderMeshType::Quad:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetCurrentInstancePtr()->getBuiltinMesh(BuiltinMeshType::Quad),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cube:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetCurrentInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cube),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Sphere:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetCurrentInstancePtr()->getBuiltinMesh(BuiltinMeshType::Sphere),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cone:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetCurrentInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cone),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cylinder:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetCurrentInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cylinder),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Torus:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetCurrentInstancePtr()->getBuiltinMesh(BuiltinMeshType::Torus),
                    m_renderSystemRaw);
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        if (renderMesh)
        {
            renderMesh->setName(_renderMeshType.toCString());
            addRenderMesh(renderMesh);
        }

        return renderMesh;
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::ensureBuiltinRenderMesh(BuiltinRenderMeshType _renderMeshType)
    {
        RenderMeshPtr const& renderMesh = getBuiltinRenderMesh(_renderMeshType);
        if (renderMesh)
            return renderMesh;

        return createBuiltinRenderMesh(_renderMeshType);
    }

    //////////////////////////////////////////
    void RenderMeshManager::createBuiltinRenderMeshes()
    {
        for (BuiltinRenderMeshType t = BuiltinRenderMeshType(1); t < BuiltinRenderMeshType::MAX; ++t)
            ensureBuiltinRenderMesh(t);
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::getRenderMesh(HashedCString _renderMeshName)
    {
        static RenderMeshPtr nullPointer;

        StringKeyMap<RenderMeshPtr>::const_iterator it = m_renderMeshesByName.find(_renderMeshName);
        if (it != m_renderMeshesByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_renderMeshName);
        if (!assetFile)
            return nullPointer;

        RenderMeshPtr renderMesh = RenderMesh::Create(assetFile);
        renderMesh->setName(_renderMeshName.str);
        return addRenderMesh(renderMesh);
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::getRenderMesh(AssetFilePtr const& _assetFile)
    {
        StringKeyMap<RenderMeshPtr>::const_iterator it = m_renderMeshesByName.find(_assetFile->getFileName());
        if (it != m_renderMeshesByName.end())
            return it->second;

        RenderMeshPtr renderMesh = RenderMesh::Create(_assetFile);
        renderMesh->setName(_assetFile->getFileName());
        return addRenderMesh(renderMesh);
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::addRenderMesh(RenderMeshPtr const& _renderMesh)
    {
        auto it2 = m_renderMeshesByName.insert(
            _renderMesh->getName(),
            _renderMesh);
        return *it2;
    }

    //////////////////////////////////////////
    Vector<RenderMeshPtr> RenderMeshManager::getRenderMeshesSorted()
    {
        Vector<RenderMeshPtr> result;

        for (auto const& value : m_renderMeshesByName)
            result.emplace_back(value.second);

        std::sort(
            result.begin(),
            result.end(),
            [](RenderMeshPtr const& _a, RenderMeshPtr const& _b)
            {
                return _a->getName() < _b->getName();
            });

        return result;
    }

    //////////////////////////////////////////
    MeshPtr RenderMeshManager::loadMesh(AssetFilePtr const& _assetFile)
    {
        MeshPtr mesh;

        if (!_assetFile)
            return mesh;

        mesh = Mesh::Create(m_renderSystemRaw);

        Debug::Log("Loading render mesh: %s...", _assetFile->getFileName().c_str());

        StringKeyMap<String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);

        if (metaData.empty() || !metaData.contains("ext"))
        {
            bool loaderFound = false;
            for (auto const& renderMeshLoaderData : m_renderMeshLoaders)
            {
                RenderMeshLoaderData const& loaderData = renderMeshLoaderData.second;
                if (loaderData.isRenderMeshAssetFileFunc(_assetFile))
                {
                    loaderFound = true;
                    MAZE_ERROR_IF(!loaderData.loadRenderMeshAssetFileFunc(_assetFile, mesh), "Mesh is not loaded - '%s'", _assetFile->getFileName().c_str());
                    break;
                }
            }

            MAZE_ERROR_IF(!loaderFound, "Unsupported texture format - %s!", _assetFile->getFileName().c_str());
        }
        else
        {
            HashedString fileExtension = StringHelper::ToLower(metaData["ext"]);

            auto it = m_renderMeshLoaders.find(fileExtension);
            if (it != m_renderMeshLoaders.end())
            {
                RenderMeshLoaderData const& loaderData = it->second;
                MAZE_ERROR_IF(!loaderData.loadRenderMeshAssetFileFunc(_assetFile, mesh), "Mesh is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format - %s!", _assetFile->getFileName().c_str());
            }
        }

        Debug::Log("Loaded.", _assetFile->getFileName().c_str());

        return mesh;
    }

    //////////////////////////////////////////
    void RenderMeshManager::loadAllAssetRenderMeshes()
    {
        Vector<String> loaderExtensions = getRenderMeshLoaderExtensions();

        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(
            Set<String>(loaderExtensions.begin(), loaderExtensions.end()));
        for (AssetFilePtr const& assetFile : assetFiles)
        {
            if (m_renderMeshesByName.find(assetFile->getFileName()) != m_renderMeshesByName.end())
                continue;

            RenderMeshPtr renderMesh = RenderMesh::Create(assetFile);
            renderMesh->setName(assetFile->getFileName());
            addRenderMesh(renderMesh);
        }
    }

    //////////////////////////////////////////
    Vector<String> RenderMeshManager::getRenderMeshLoaderExtensions()
    {
        Vector<String> result;
        for (auto const& renderMeshLoaderData : m_renderMeshLoaders)
            result.push_back(renderMeshLoaderData.first);

        return result;
    }
    
} // namespace Maze
//////////////////////////////////////////
