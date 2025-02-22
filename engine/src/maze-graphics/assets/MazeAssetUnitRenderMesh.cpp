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
#include "maze-graphics/assets/MazeAssetUnitRenderMesh.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitRenderMesh
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitRenderMesh, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitRenderMesh::AssetUnitRenderMesh()
    {

    }

    //////////////////////////////////////////
    AssetUnitRenderMesh::~AssetUnitRenderMesh()
    {
    }

    //////////////////////////////////////////
    AssetUnitRenderMeshPtr AssetUnitRenderMesh::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitRenderMeshPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitRenderMesh,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitRenderMesh::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    RenderMeshPtr const& AssetUnitRenderMesh::loadRenderMesh(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initRenderMesh();
            _syncLoad ? loadNow() : load();
        }

        return m_renderMesh;
    }

    //////////////////////////////////////////
    bool AssetUnitRenderMesh::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initRenderMesh();
        if (!m_renderMesh)
            return false;

        m_renderMesh->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitRenderMesh::unloadNowImpl()
    {
        if (m_renderMesh)
        {
            m_renderMesh->clear();
        }

        return true;
    }

    //////////////////////////////////////////
    RenderMeshPtr const& AssetUnitRenderMesh::initRenderMesh()
    {
        if (m_renderMesh)
            return m_renderMesh;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_renderMesh;

        m_renderMesh = RenderMesh::Create();
        m_renderMesh->setName(HashedString(m_data.getString(MAZE_HCS("name"), assetFile->getFileName().toUTF8())));

        if (RenderMeshManager::GetCurrentInstancePtr())
        {
            RenderMeshLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitRenderMeshWPtr)cast<AssetUnitRenderMesh>()](bool _syncLoad)
                {
                    if (AssetUnitRenderMeshPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitRenderMeshWPtr)cast<AssetUnitRenderMesh>()] (bool _syncLoad)
                {
                    if (AssetUnitRenderMeshPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitRenderMeshWPtr)cast<AssetUnitRenderMesh>()](bool _syncLoad)
                {
                    if (AssetUnitRenderMeshPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitRenderMeshWPtr)cast<AssetUnitRenderMesh>()](Set<String> const& _tags)
                {
                    if (AssetUnitRenderMeshPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            RenderMeshManager::GetCurrentInstancePtr()->addRenderMeshToLibrary(
                m_renderMesh,
                callbacks,
                info);
        }

        return m_renderMesh;
    }


} // namespace Maze
//////////////////////////////////////////
