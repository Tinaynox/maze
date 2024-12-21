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

                    StringKeyMap<EntityPrefabLibraryData>& entityPrefabsLibrary = EntityPrefabManager::GetInstancePtr()->m_entityPrefabsLibrary;
                    String prevMaterialName = FileHelper::GetFileNameInPath(_prevPath).toUTF8();
                    StringKeyMap<EntityPrefabLibraryData>::iterator it = entityPrefabsLibrary.find(prevMaterialName);
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
        StringKeyMap<EntityPrefabLibraryData>::const_iterator it = m_entityPrefabsLibrary.find(_name);
        if (it != m_entityPrefabsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    EntityPtr const& EntityPrefabManager::getOrLoadEntityPrefab(
        HashedCString _name)
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
    EntityPrefabLibraryData* EntityPrefabManager::addEntityPrefabToLibrary(
        HashedCString _name,
        EntityPtr const& _entity,
        EntityPrefabLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_entityPrefabsLibrary.insert(
            _name,
            { _entity, _callbacks, _info });

        return it2;
    }

    //////////////////////////////////////////
    void EntityPrefabManager::removeEntityPrefabFromLibrary(HashedCString _name)
    {
        m_entityPrefabsLibrary.erase(_name);
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

        EntityCopyData copyData;
        copyData.setWorld(_world);
        copyData.setScene(_scene);

        return prefab->createCopy(copyData);
    }



} // namespace Maze
//////////////////////////////////////////
