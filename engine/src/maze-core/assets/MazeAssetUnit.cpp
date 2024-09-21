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
#include "maze-core/assets/MazeAssetUnit.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitIdChangedEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitNameChangedEvent, Event);


    //////////////////////////////////////////
    // Class AssetUnit
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnit, Object);
    
    //////////////////////////////////////////
    AssetUnit::AssetUnit()
    {

    }

    //////////////////////////////////////////
    AssetUnit::~AssetUnit()
    {
    }
    
    //////////////////////////////////////////
    bool AssetUnit::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        m_assetFile = _assetFile;
        m_data = _data;

        m_auid = m_data.getU32(MAZE_HCS("auid"), c_invalidAssetUnitId);
        m_name = HashedString(m_data.getString(MAZE_HCS("name"), _assetFile->getFileName()));
            
        return true;
    }

    //////////////////////////////////////////
    void AssetUnit::setAssetUnitId(AssetUnitId _auid)
    {
        if (m_auid == _auid)
            return;

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEvent<AssetUnitIdChangedEvent>(this, m_auid, _auid);
        
        m_auid = _auid;
    }

    //////////////////////////////////////////
    void AssetUnit::setName(HashedString const& _name)
    {
        if (m_name == _name)
            return;

        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->broadcastEvent<AssetUnitNameChangedEvent>(this, m_name, _name);

        m_name = _name;
    }

    //////////////////////////////////////////
    void AssetUnit::updateDataFromAssetUnit(DataBlock& _metaData) const
    {
        if (m_auid != c_invalidAssetUnitId)
            _metaData.setU32(MAZE_HCS("auid"), m_auid);
        else
            _metaData.removeParam(MAZE_HCS("auid")); 

        if (!m_name.empty())
            _metaData.setString(MAZE_HCS("name"), m_name);
        else
            _metaData.removeParam(MAZE_HCS("name")); 
    }

    //////////////////////////////////////////
    void AssetUnit::load()
    {
        if (isLoaded() || isLoading())
            return;

        m_loadingState = AssetUnitLoadingState::Loading;

#if (1)
        // #TODO: async load test
        if (TaskManager::GetInstancePtr())
        {
            TaskManager::GetInstancePtr()->addDelayedMainThreadTask(
                100,
                [weakPtr = (AssetUnitWPtr)cast<AssetUnit>()]()
            {
                AssetUnitPtr assetUnit = weakPtr.lock();
                if (!assetUnit)
                    return;

                assetUnit->loadNow();
            });
        }
        else
            loadNow();
#else
        loadNow();
#endif
    }

    //////////////////////////////////////////
    void AssetUnit::loadNow()
    {
        if (isLoaded())
            return;

        if (isUnloading())
        {
            m_loadingState = AssetUnitLoadingState::None;
            return;
        }

        if (loadNowImpl())
            m_loadingState = AssetUnitLoadingState::Loaded;
        else
            m_loadingState = AssetUnitLoadingState::Error;
    }

    //////////////////////////////////////////
    void AssetUnit::unload()
    {
        if (!isLoaded() || isUnloading())
            return;

        m_loadingState = AssetUnitLoadingState::Unloading;

#if (1)
        // #TODO: async load test
        if (TaskManager::GetInstancePtr())
        {
            TaskManager::GetInstancePtr()->addDelayedMainThreadTask(
                10,
                [weakPtr = (AssetUnitWPtr)cast<AssetUnit>()]()
            {
                AssetUnitPtr assetUnit = weakPtr.lock();
                if (!assetUnit)
                    return;

                assetUnit->unloadNow();
            });
        }
        else
            unloadNow();
#else
        unloadNow();
#endif
    }

    //////////////////////////////////////////
    void AssetUnit::unloadNow()
    {
        if (!isLoaded())
            return;

        if (unloadNowImpl())
            m_loadingState = AssetUnitLoadingState::None;
    }

} // namespace Maze
//////////////////////////////////////////
