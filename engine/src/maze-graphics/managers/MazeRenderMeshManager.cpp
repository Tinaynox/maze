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

        

        return true;
    }

    //////////////////////////////////////////
    void RenderMeshManager::notifyRenderSystemInited()
    {
        // createBuiltinRenderMeshes();
    }

    //////////////////////////////////////////
    RenderMeshLibraryData const* RenderMeshManager::getRenderMeshLibraryData(HashedCString _renderMeshName)
    {
        StringKeyMap<RenderMeshLibraryData>::const_iterator it = m_renderMeshesLibrary.find(_renderMeshName);
        if (it != m_renderMeshesLibrary.end())
            return &it->second;
        return nullptr;
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
                    MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Quad),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cube:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cube),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Sphere:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Sphere),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cone:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cone),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cylinder:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Cylinder),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Torus:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->getBuiltinMesh(BuiltinMeshType::Torus),
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
            addRenderMeshToLibrary(renderMesh);
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
        MAZE_PROFILE_EVENT("RenderMeshManager::createBuiltinRenderMeshes");

        for (BuiltinRenderMeshType t = BuiltinRenderMeshType(1); t < BuiltinRenderMeshType::MAX; ++t)
            ensureBuiltinRenderMesh(t);
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::getRenderMesh(HashedCString _renderMeshName)
    {
        static RenderMeshPtr nullPointer;

        RenderMeshLibraryData const* libraryData = getRenderMeshLibraryData(_renderMeshName);
        if (libraryData != nullptr)
            return libraryData->renderMesh;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_renderMeshName);
        if (!assetFile)
            return nullPointer;

        RenderMeshPtr renderMesh = RenderMesh::Create(assetFile);
        renderMesh->setName(_renderMeshName.str);
        RenderMeshLibraryData* data = addRenderMeshToLibrary(renderMesh);
        data->assetFile = assetFile;
        return data->renderMesh;
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::getRenderMesh(AssetFilePtr const& _assetFile)
    {
        StringKeyMap<RenderMeshLibraryData>::const_iterator it = m_renderMeshesLibrary.find(_assetFile->getFileName());
        if (it != m_renderMeshesLibrary.end())
            return it->second.renderMesh;

        RenderMeshPtr renderMesh = RenderMesh::Create(_assetFile);
        renderMesh->setName(_assetFile->getFileName());
        RenderMeshLibraryData* data = addRenderMeshToLibrary(renderMesh);
        data->assetFile = _assetFile;
        return data->renderMesh;
    }

    //////////////////////////////////////////
    RenderMeshLibraryData* RenderMeshManager::addRenderMeshToLibrary(RenderMeshPtr const& _renderMesh)
    {
        auto it2 = m_renderMeshesLibrary.insert(
            _renderMesh->getName(),
            _renderMesh);
        return it2;
    }

    //////////////////////////////////////////
    void RenderMeshManager::removeRenderMeshFromLibrary(HashedCString _renderMeshName)
    {
        m_renderMeshesLibrary.erase(_renderMeshName);
    }

    //////////////////////////////////////////
    Vector<RenderMeshPtr> RenderMeshManager::getRenderMeshesSorted()
    {
        Vector<RenderMeshPtr> result;

        for (auto const& value : m_renderMeshesLibrary)
            result.emplace_back(value.second.renderMesh);

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
    void RenderMeshManager::loadAllAssetRenderMeshes()
    {
        MAZE_PROFILE_EVENT("RenderMeshManager::loadAllAssetRenderMeshes");

        Vector<String> loaderExtensions = MeshManager::GetInstancePtr()->getMeshLoaderExtensions();

        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(
            Set<String>(loaderExtensions.begin(), loaderExtensions.end()));
        for (AssetFilePtr const& assetFile : assetFiles)
        {
            if (m_renderMeshesLibrary.find(assetFile->getFileName()) != m_renderMeshesLibrary.end())
                continue;

            RenderMeshPtr renderMesh = RenderMesh::Create(assetFile);
            renderMesh->setName(assetFile->getFileName());
            addRenderMeshToLibrary(renderMesh);
        }
    }

    //////////////////////////////////////////
    void RenderMeshManager::unloadAssetRenderMeshes(Set<String> const& _tags)
    {
        MAZE_PROFILE_EVENT("RenderMeshManager::unloadAssetRenderMeshes");

        StringKeyMap<RenderMeshLibraryData>::iterator it = m_renderMeshesLibrary.begin();
        StringKeyMap<RenderMeshLibraryData>::iterator end = m_renderMeshesLibrary.end();
        for (; it != end; )
        {
            if (it->second.assetFile && it->second.assetFile->hasAnyOfTags(_tags))
            {
                it = m_renderMeshesLibrary.erase(it);
                end = m_renderMeshesLibrary.end();
            }
            else
            {
                ++it;
            }
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
