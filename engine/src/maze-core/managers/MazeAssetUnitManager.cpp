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
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/assets/MazeAssetUnit.hpp"


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

        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->unsubscribeEvent<AssetUnitIdChangedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<AssetUnitNameChangedEvent>(this);
        }
    }

    //////////////////////////////////////////
    void AssetUnitManager::Initialize(AssetUnitManagerPtr& _assetManager, DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetUnitManager, _assetManager, init(_config));
    }

    //////////////////////////////////////////
    bool AssetUnitManager::init(DataBlock const& _config)
    {    
        m_generateIdsForNewAssetUnits =
            _config.getBool(MAZE_HCS("generateIdsForNewAssetUnits"), m_generateIdsForNewAssetUnits);

        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->subscribeEvent<AssetUnitIdChangedEvent>(this, &AssetUnitManager::notifyEvent);
            EventManager::GetInstancePtr()->subscribeEvent<AssetUnitNameChangedEvent>(this, &AssetUnitManager::notifyEvent);
        }

        return true;
    }

    //////////////////////////////////////////
    AssetUnitPtr AssetUnitManager::createAssetUnit(AssetFilePtr const& _assetFile, DataBlock const& _data)
    {
        HashedString assetUnitType(_data.getName());

        auto it = m_assetUnitProcessors.find(assetUnitType);
        if (it != m_assetUnitProcessors.end())
            return it->second(_assetFile, _data);        

        MAZE_ERROR("Unsupported asset unit type - '%s'!", assetUnitType.c_str());

        return nullptr;
    }

    //////////////////////////////////////////
    void AssetUnitManager::registerAssetUnitProcessor(HashedCString _name, AssetUnitProcessor const& _processor)
    {
        m_assetUnitProcessors[_name] = _processor;
    }

    //////////////////////////////////////////
    void AssetUnitManager::clearAssetUnitProcessor(HashedCString _name)
    {
        m_assetUnitProcessors.erase(_name);
    }

    //////////////////////////////////////////
    AssetUnitId AssetUnitManager::generateAssetUnitId() const
    {
        AssetUnitId afid;

        do
        {
            afid = Random::RangeRandom(1u, U32_MAX);
        }
        while (m_assetUnitsById.find(afid) != m_assetUnitsById.end());

        return afid;
    }

    //////////////////////////////////////////
    void AssetUnitManager::addAssetUnit(AssetUnitPtr const& _assetUnit)
    {
        MAZE_ERROR_RETURN_IF(!_assetUnit, "AssetFile is null!");
        MAZE_ERROR_RETURN_IF(_assetUnit->getAssetUnitId() == c_invalidAssetUnitId, "AssetUnitId is invalid!");

        m_assetUnitsById[_assetUnit->getAssetUnitId()] = _assetUnit;

        if (!_assetUnit->getName().empty())
            m_assetUnitsByName[_assetUnit->getName()] = _assetUnit;

        eventAssetUnitAdded(_assetUnit);
    }

    //////////////////////////////////////////
    void AssetUnitManager::removeAssetUnit(AssetUnitId _assetUnitId)
    {
        auto it = m_assetUnitsById.find(_assetUnitId);
        if (it != m_assetUnitsById.end())
        {
            HashedString name = it->second->getName();

            eventAssetUnitWillBeRemoved(_assetUnitId, it->second);
            m_assetUnitsById.erase(it);
            m_assetUnitsByName.erase(name);
        }
    }

    //////////////////////////////////////////
    AssetUnitPtr const& AssetUnitManager::getAssetUnit(AssetUnitId _assetUnitId) const
    {
        static AssetUnitPtr nullPointer;

        auto it = m_assetUnitsById.find(_assetUnitId);
        if (it != m_assetUnitsById.end())
            return it->second;

        return nullPointer;
    }

    //////////////////////////////////////////
    AssetUnitPtr const& AssetUnitManager::getAssetUnit(HashedCString _name) const
    {
        static AssetUnitPtr nullPointer;

        auto it = m_assetUnitsByName.find(_name);
        if (it != m_assetUnitsByName.end())
            return it->second;

        return nullPointer;
    }

    //////////////////////////////////////////
    HashedString const& AssetUnitManager::getAssetUnitName(AssetUnitId _assetFileId) const
    {
        AssetUnitPtr const& assetUnit = getAssetUnit(_assetFileId);
        if (!assetUnit)
            return HashedString::c_empty;

        return assetUnit->getName();
    }

    //////////////////////////////////////////
    AssetUnitId AssetUnitManager::getAssetUnitId(HashedCString _name) const
    {
        AssetUnitPtr const& assetUnit = getAssetUnit(_name);
        if (!assetUnit)
            return c_invalidAssetUnitId;

        return assetUnit->getAssetUnitId();
    }

    //////////////////////////////////////////
    void AssetUnitManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<AssetUnitIdChangedEvent>::UID())
        {
            AssetUnitIdChangedEvent* evt = _event->castRaw<AssetUnitIdChangedEvent>();
            auto it = m_assetUnitsById.find(evt->getPrevAssetUnitId());
            if (it != m_assetUnitsById.end())
            {
                AssetUnitPtr assetUnit = std::move(it->second);
                
                m_assetUnitsById.erase(it);

                if (evt->getNewAssetUnitId() != c_invalidAssetUnitId)
                {
                    m_assetUnitsById.emplace(
                        std::piecewise_construct,
                        std::forward_as_tuple(evt->getNewAssetUnitId()),
                        std::forward_as_tuple(std::move(assetUnit)));
                }
                else
                {
                    MAZE_NOT_IMPLEMENTED
                }
            }
        }
        else
        if (_eventUID == ClassInfo<AssetUnitNameChangedEvent>::UID())
        {
            AssetUnitNameChangedEvent* evt = _event->castRaw<AssetUnitNameChangedEvent>();

            MAZE_NOT_IMPLEMENTED
        }
    }

} // namespace Maze
//////////////////////////////////////////
