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
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/assets/MazeAssetUnitRenderMesh.hpp"


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
        static RenderMeshManagerPtr nullPointer;
        RenderSystem* renderSystem = RenderSystem::GetCurrentInstancePtr();
        if (!renderSystem)
            return nullPointer;

        return renderSystem->getRenderMeshManager();
    }

    //////////////////////////////////////////
    bool RenderMeshManager::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        m_renderSystemRaw->eventSystemInited.subscribe(this, &RenderMeshManager::notifyRenderSystemInited);

        
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitRenderMesh::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
            {
                return AssetUnitRenderMesh::Create(_file, _data);
            });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
            {
                if (_assetUnit->getClassUID() == ClassInfo<AssetUnitRenderMesh>::UID())
                    _assetUnit->castRaw<AssetUnitRenderMesh>()->initRenderMesh();
            });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
                {
                    if (MeshManager::GetInstancePtr()->hasMeshLoader(_extension))
                    {
                        if (!_assetFile->getAssetUnit<AssetUnitRenderMesh>())
                            _assetFile->addAssetUnit(AssetUnitRenderMesh::Create(_assetFile));
                    }
                });

            AssetManager::GetInstancePtr()->eventAssetFileMoved.subscribe(
                [](AssetFilePtr const& _assetFile, Path const& _prevPath)
                {
                    if (MeshManager::GetInstancePtr()->hasMeshLoader(HashedString(_assetFile->getExtension().toUTF8())))
                    {
                        if (!RenderMeshManager::GetCurrentInstancePtr())
                            return;

                        StringKeyMap<RenderMeshLibraryData>& renderMeshesLibrary = RenderMeshManager::GetCurrentInstancePtr()->m_renderMeshesLibrary;
                        String prevMaterialName = FileHelper::GetFileNameInPath(_prevPath).toUTF8();
                        StringKeyMap<RenderMeshLibraryData>::iterator it = renderMeshesLibrary.find(prevMaterialName);
                        if (it != renderMeshesLibrary.end())
                        {
                            String newAssetName = _assetFile->getFileName().toUTF8();
                            it->second.renderMesh->setName(HashedString(newAssetName));
                            renderMeshesLibrary.insert(newAssetName, it->second);
                            renderMeshesLibrary.erase(it);
                        }
                    }
                });
        }

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
                    MeshManager::GetInstancePtr()->ensureBuiltinMesh(BuiltinMeshType::Quad),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::QuadNullPivot:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->ensureBuiltinMesh(BuiltinMeshType::QuadNullPivot),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cube:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->ensureBuiltinMesh(BuiltinMeshType::Cube),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Sphere:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->ensureBuiltinMesh(BuiltinMeshType::Sphere),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cone:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->ensureBuiltinMesh(BuiltinMeshType::Cone),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Cylinder:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->ensureBuiltinMesh(BuiltinMeshType::Cylinder),
                    m_renderSystemRaw);
                break;
            }
            case BuiltinRenderMeshType::Torus:
            {
                renderMesh = RenderMesh::Create(
                    MeshManager::GetInstancePtr()->ensureBuiltinMesh(BuiltinMeshType::Torus),
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
            renderMesh->setName(HashedString(_renderMeshType.toString()));
            renderMesh->lock();
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
    RenderMeshPtr const& RenderMeshManager::getOrLoadRenderMesh(
        HashedCString _renderMeshName,
        bool _syncLoad)
    {
        static RenderMeshPtr const nullPointer;

        if (_renderMeshName.empty())
            return nullPointer;

        RenderMeshLibraryData const* libraryData = getRenderMeshLibraryData(_renderMeshName);
        if (libraryData != nullptr)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->renderMesh;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_renderMeshName);
        if (!assetFile)
            return nullPointer;

        return getOrLoadRenderMesh(assetFile, _syncLoad);
    }

    //////////////////////////////////////////
    RenderMeshPtr const& RenderMeshManager::getOrLoadRenderMesh(
        AssetFilePtr const& _assetFile,
        bool _syncLoad)
    {
        static RenderMeshPtr const nullPointer;

        StringKeyMap<RenderMeshLibraryData>::const_iterator it = m_renderMeshesLibrary.find(_assetFile->getFileName());
        if (it != m_renderMeshesLibrary.end())
        {
            if (it->second.callbacks.requestLoad)
                it->second.callbacks.requestLoad(_syncLoad);

            return it->second.renderMesh;
        }

        Vector<HashedString> loaderExtensions = MeshManager::GetInstancePtr()->getMeshLoaderExtensions();
        if (std::find(loaderExtensions.begin(), loaderExtensions.end(), HashedString(_assetFile->getExtension())) == loaderExtensions.end())
            return nullPointer;

        RenderMeshPtr renderMesh = RenderMesh::Create(_assetFile);
        renderMesh->setName(HashedString(_assetFile->getFileName().toUTF8()));
        RenderMeshLibraryData* data = addRenderMeshToLibrary(renderMesh);

        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)_assetFile,
                    renderMeshWeak = (RenderMeshWPtr)renderMesh
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    RenderMeshPtr renderMesh = renderMeshWeak.lock();
                    if (assetFile && renderMesh)
                        renderMesh->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)_assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            return data->renderMesh;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    HashedCString RenderMeshManager::getRenderMeshName(RenderMesh const* _renderMesh)
    {
        for (StringKeyMap<RenderMeshLibraryData>::iterator it = m_renderMeshesLibrary.begin(),
                                                           end = m_renderMeshesLibrary.end();
                                                           it != end;
                                                           ++it)
        {
            if (it->second.renderMesh.get() == _renderMesh)
                return it.key();
        }

        return HashedCString();
    }

    //////////////////////////////////////////
    RenderMeshLibraryData* RenderMeshManager::addRenderMeshToLibrary(
        RenderMeshPtr const& _renderMesh,
        RenderMeshLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_renderMeshesLibrary.insert(
            _renderMesh->getName(),
            { _renderMesh, _callbacks, _info });
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

        Vector<HashedString> loaderExtensions = MeshManager::GetInstancePtr()->getMeshLoaderExtensions();

        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(
            Set<String>(loaderExtensions.begin(), loaderExtensions.end()));
        for (AssetFilePtr const& assetFile : assetFiles)
        {
            if (m_renderMeshesLibrary.find(assetFile->getFileName()) != m_renderMeshesLibrary.end())
                continue;

            RenderMeshPtr renderMesh = RenderMesh::Create(assetFile);
            if (renderMesh)
            {
                renderMesh->setName(HashedString(assetFile->getFileName().toUTF8()));
                addRenderMeshToLibrary(renderMesh);
            }
        }
    }

    //////////////////////////////////////////
    void RenderMeshManager::unloadAssetRenderMeshes(Set<String> const& _tags)
    {
        MAZE_PROFILE_EVENT("RenderMeshManager::unloadAssetRenderMeshes");

        Vector<std::function<void(bool)>> unloadCallbacks;

        m_renderMeshesLibrary.iterate(
            [&](HashedCString _name, RenderMeshLibraryData const& _data)
            {
                if (_data.callbacks.hasAnyOfTags && _data.callbacks.hasAnyOfTags(_tags) && _data.callbacks.requestUnload)
                    unloadCallbacks.push_back(_data.callbacks.requestUnload);

                return true;
            });

        for (std::function<void(bool)> const& unloadCallback : unloadCallbacks)
            unloadCallback(true);
    }

    
} // namespace Maze
//////////////////////////////////////////
