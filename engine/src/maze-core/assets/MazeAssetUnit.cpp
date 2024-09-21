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


//////////////////////////////////////////
namespace Maze
{

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
            
        return true;
    }

    //////////////////////////////////////////
    void AssetUnit::updateDataFromAssetUnit(DataBlock& _metaData) const
    {
        _metaData.setU32(MAZE_HCS("auid"), m_auid);
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
