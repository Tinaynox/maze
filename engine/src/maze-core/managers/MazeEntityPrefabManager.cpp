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
#include "maze-core/managers/MazeEntityPrefabManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetUnitEntityPrefab.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class EntityPrefabManager
    //
    //////////////////////////////////////////
    EntityPrefabManager* EntityPrefabManager::s_instance = nullptr;

    //////////////////////////////////////////
    EntityPrefabManager::EntityPrefabManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EntityPrefabManager::~EntityPrefabManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EntityPrefabManager::Initialize(
        EntityPrefabManagerPtr& inputManager,
        DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EntityPrefabManager, inputManager, init(_config));
    }

    //////////////////////////////////////////
    bool EntityPrefabManager::init(DataBlock const& _config)
    {
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitEntityPrefab::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
            {
                return AssetUnitEntityPrefab::Create(_file, _data);
            });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
            {
                if (_extension == MAZE_HCS("mzprefab"))
                {
                    if (!_assetFile->getAssetUnit<AssetUnitEntityPrefab>())
                        _assetFile->addAssetUnit(AssetUnitEntityPrefab::Create(_assetFile));
                }
            });

            AssetManager::GetInstancePtr()->eventAssetFileMoved.subscribe(
                [](AssetFilePtr const& _assetFile, Path const& _prevPath)
            {
                if (_assetFile->getExtension() == Path("mzprefab"))
                {
                    if (!EntityPrefabManager::GetInstancePtr())
                        return;

                    StringKeyMap<SharedPtr<EntityPrefabLibraryData>>& entityPrefabsLibrary = EntityPrefabManager::GetInstancePtr()->m_entityPrefabsLibrary;
                    String prevMaterialName = FileHelper::GetFileNameInPath(_prevPath).toUTF8();
                    StringKeyMap<SharedPtr<EntityPrefabLibraryData>>::iterator it = entityPrefabsLibrary.find(prevMaterialName);
                    if (it != entityPrefabsLibrary.end())
                    {
                        String newAssetName = _assetFile->getFileName().toUTF8();
                        entityPrefabsLibrary.insert(newAssetName, it->second);
                        entityPrefabsLibrary.erase(it);
                    }
                }
            });
        }

        return true;
    }

    //////////////////////////////////////////
    EntityPrefabLibraryData const* EntityPrefabManager::getEntityPrefabLibraryData(HashedCString _name)
    {
        StringKeyMap<SharedPtr<EntityPrefabLibraryData>>::const_iterator it = m_entityPrefabsLibrary.find(_name);
        if (it != m_entityPrefabsLibrary.end())
            return it->second.get();
        return nullptr;
    }

    //////////////////////////////////////////
    EntityPrefabLibraryData const* EntityPrefabManager::getEntityPrefabLibraryData(AssetUnitId _auid)
    {
        auto it = m_entityPrefabsByAssetUnitId.find(_auid);
        if (it != m_entityPrefabsByAssetUnitId.end())
            return it->second.get();
        return nullptr;
    }

    //////////////////////////////////////////
    EntityPtr const& EntityPrefabManager::getOrLoadEntityPrefab(HashedCString _name)
    {
        static EntityPtr const nullPointer;

        EntityPrefabLibraryData const* libraryData = getEntityPrefabLibraryData(_name);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(true);

            return libraryData->prefab;
        }

        AssetFilePtr assetFile = AssetManager::GetInstancePtr()->getAssetFile(_name);
        if (!assetFile)
            return nullPointer;

        EntityPtr prefab = EntitySerializationManager::GetInstancePtr()->loadPrefab(
            assetFile,
            EntityManager::GetInstancePtr()->getLibraryWorldRaw());

        EntityPrefabLibraryData* data = addEntityPrefabToLibrary(_name, prefab);
        if (data)
            return data->prefab;

        return nullPointer;
    }

    //////////////////////////////////////////
    EntityPtr const& EntityPrefabManager::getOrLoadEntityPrefab(AssetUnitId _auid)
    {
        static EntityPtr const nullPointer;

        EntityPrefabLibraryData const* libraryData = getEntityPrefabLibraryData(_auid);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(true);

            return libraryData->prefab;
        }

        AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(_auid);
        if (!assetUnit)
            return nullPointer;

        if (assetUnit->getClassUID() == ClassInfo<AssetUnitEntityPrefab>::UID())
            return assetUnit->castRaw<AssetUnitEntityPrefab>()->loadPrefab();

        return nullPointer;
    }

    //////////////////////////////////////////
    HashedCString EntityPrefabManager::getEntityPrefabName(Entity const* _entity)
    {
        for (StringKeyMap<SharedPtr<EntityPrefabLibraryData>>::iterator it = m_entityPrefabsLibrary.begin(),
                                                                        end = m_entityPrefabsLibrary.end();
                                                                        it != end;
                                                                        ++it)
        {
            if (it->second->prefab.get() == _entity)
                return it.key();
        }

        return HashedCString();
    }

    //////////////////////////////////////////
    EntityPrefabLibraryData* EntityPrefabManager::addEntityPrefabToLibrary(
        HashedCString _name,
        EntityPtr const& _entity,
        EntityPrefabLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        SharedPtr<EntityPrefabLibraryData> data = MakeShared<EntityPrefabLibraryData>(_entity, _callbacks, _info);

        auto it = m_entityPrefabsLibrary.insert(_name, data);

        AssetUnitId auid = _info.getU32(MAZE_HCS("auid"), c_invalidAssetUnitId);
        if (auid != c_invalidAssetUnitId)
            m_entityPrefabsByAssetUnitId.emplace(auid, data);

        return it->get();
    }

    //////////////////////////////////////////
    void EntityPrefabManager::removeEntityPrefabFromLibrary(HashedCString _name)
    {
        auto it = m_entityPrefabsLibrary.find(_name);
        if (it != m_entityPrefabsLibrary.end())
        {
            AssetUnitId auid = it->second->data.getU32(MAZE_HCS("auid"), c_invalidAssetUnitId);
            if (auid != c_invalidAssetUnitId)
                m_entityPrefabsByAssetUnitId.erase(auid);

            m_entityPrefabsLibrary.erase(it);
        }
    }

    //////////////////////////////////////////
    EntityPtr EntityPrefabManager::instantiatePrefab(
        EntityPtr const& _entity,
        EcsWorld* _world,
        EcsScene* _scene)
    {
        EntityCopyData copyData;
        copyData.setWorld(_world);
        copyData.setScene(_scene);

        return _entity->createCopy(copyData);
    }

    //////////////////////////////////////////
    EntityPtr EntityPrefabManager::instantiatePrefab(
        HashedCString _name,
        EcsWorld* _world,
        EcsScene* _scene)
    {
        EntityPtr const& prefab = getOrLoadEntityPrefab(_name);
        if (!prefab)
            return nullptr;

        return instantiatePrefab(prefab, _world, _scene);
    }

    //////////////////////////////////////////
    EntityPtr EntityPrefabManager::instantiatePrefab(
        AssetUnitId _auid,
        EcsWorld* _world,
        EcsScene* _scene)
    {
        EntityPtr const& prefab = getOrLoadEntityPrefab(_auid);
        if (!prefab)
            return nullptr;

        return instantiatePrefab(prefab, _world, _scene);
    }



} // namespace Maze
//////////////////////////////////////////
