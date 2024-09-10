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
#include "maze-core/managers/MazeAssetUnitManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitManager
    //
    //////////////////////////////////////////
    AssetUnitManager* AssetUnitManager::s_instance = nullptr;

    //////////////////////////////////////////
    AssetUnitManager::AssetUnitManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    AssetUnitManager::~AssetUnitManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void AssetUnitManager::Initialize(AssetUnitManagerPtr& _assetManager, DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetUnitManager, _assetManager, init(_config));
    }

    //////////////////////////////////////////
    bool AssetUnitManager::init(DataBlock const& _config)
    {    

        return true;
    }

    //////////////////////////////////////////
    AssetUnitPtr AssetUnitManager::createAssetUnit(AssetFilePtr const& _assetFile, DataBlock const& _data)
    {
        auto it = m_assetUnitProcessors.find(HashedString(_data.getName()));
        if (it != m_assetUnitProcessors.end())
            return it->second(_assetFile, _data);        

        return nullptr;
    }

    //////////////////////////////////////////
    void AssetUnitManager::registerAssetUnitProcessor(HashedString const& _name, AssetUnitProcessor const& _processor)
    {
        m_assetUnitProcessors[_name] = _processor;
    }

    //////////////////////////////////////////
    void AssetUnitManager::clearAssetUnitProcessor(HashedString const& _name)
    {
        m_assetUnitProcessors.erase(_name);
    }

} // namespace Maze
//////////////////////////////////////////
