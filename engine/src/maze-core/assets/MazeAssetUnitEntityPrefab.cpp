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
#include "MazeCoreHeader.hpp"
#include "maze-core/assets/MazeAssetUnitEntityPrefab.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/ecs/components/MazePrefabInstance.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitEntityPrefab
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitEntityPrefab, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitEntityPrefab::AssetUnitEntityPrefab()
    {

    }

    //////////////////////////////////////////
    AssetUnitEntityPrefab::~AssetUnitEntityPrefab()
    {
    }

    //////////////////////////////////////////
    AssetUnitEntityPrefabPtr AssetUnitEntityPrefab::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitEntityPrefabPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitEntityPrefab,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitEntityPrefab::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    EntityPtr const& AssetUnitEntityPrefab::loadPrefab()
    {
        if (!isLoaded())
            loadNow();

        return m_prefab;
    }

    //////////////////////////////////////////
    bool AssetUnitEntityPrefab::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initPrefab();
        if (!m_prefab)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitEntityPrefab::unloadNowImpl()
    {
        if (m_prefab)
        {
            m_prefab->removeFromEcsWorld();
            m_prefab.reset();
        }

        return true;
    }

    //////////////////////////////////////////
    EntityPtr const& AssetUnitEntityPrefab::initPrefab()
    {
        if (m_prefab)
            return m_prefab;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_prefab;

        
        m_prefab = EntitySerializationManager::GetInstancePtr()->loadPrefab(
            assetFile,
            EntityManager::GetInstancePtr()->getLibraryWorldRaw());

        m_prefab->createComponent<PrefabInstance>(getAssetUnitId());
        
        if (EntityPrefabManager::GetInstancePtr())
        {
            EntityPrefabLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitEntityPrefabWPtr)cast<AssetUnitEntityPrefab>()](bool _syncLoad)
                {
                    if (AssetUnitEntityPrefabPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitEntityPrefabWPtr)cast<AssetUnitEntityPrefab>()] (bool _syncLoad)
                {
                    if (AssetUnitEntityPrefabPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitEntityPrefabWPtr)cast<AssetUnitEntityPrefab>()](bool _syncLoad)
                {
                    if (AssetUnitEntityPrefabPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitEntityPrefabWPtr)cast<AssetUnitEntityPrefab>()](Set<String> const& _tags)
                {
                    if (AssetUnitEntityPrefabPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            EntityPrefabManager::GetInstancePtr()->addEntityPrefabToLibrary(
                getName(),
                m_prefab,
                callbacks,
                info);
        }
        

        return m_prefab;
    }


} // namespace Maze
//////////////////////////////////////////
