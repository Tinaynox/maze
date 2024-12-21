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
#pragma once
#if (!defined(_MazeEntityPrefabManager_hpp_))
#define _MazeEntityPrefabManager_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntityPrefabManager);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(EcsScene);


    //////////////////////////////////////////
    // Struct EntityPrefabLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API EntityPrefabLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct EntityPrefabLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API EntityPrefabLibraryData
    {
        //////////////////////////////////////////
        EntityPrefabLibraryData(
            EntityPtr const& _prefab = nullptr,
            EntityPrefabLibraryDataCallbacks const& _callbacks = EntityPrefabLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : prefab(_prefab)
            , callbacks(_callbacks)
            , data(_data)
        {}

        EntityPtr prefab;
        EntityPrefabLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Class EntityPrefabManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityPrefabManager
    {
    public:

        //////////////////////////////////////////
        virtual ~EntityPrefabManager();

        //////////////////////////////////////////
        static void Initialize(
            EntityPrefabManagerPtr& _prefabManager,
            DataBlock const& _config = DataBlock::c_empty);


        //////////////////////////////////////////
        static inline EntityPrefabManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EntityPrefabManager& GetInstance() { return *s_instance; }


        //////////////////////////////////////////
        EntityPrefabLibraryData const* getEntityPrefabLibraryData(HashedCString _name);

        //////////////////////////////////////////
        EntityPrefabLibraryData const* getEntityPrefabLibraryData(String const& _assetFileName) { return getEntityPrefabLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        EntityPrefabLibraryData const* getEntityPrefabLibraryData(CString _assetFileName) { return getEntityPrefabLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        EntityPtr const& getOrLoadEntityPrefab(HashedCString _name);


        //////////////////////////////////////////
        EntityPrefabLibraryData* addEntityPrefabToLibrary(
            HashedCString _name,
            EntityPtr const& _entity,
            EntityPrefabLibraryDataCallbacks const& _callbacks = EntityPrefabLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        void removeEntityPrefabFromLibrary(HashedCString _name);

        //////////////////////////////////////////
        inline void removeEntityPrefabFromLibrary(CString _name) { removeEntityPrefabFromLibrary(HashedCString(_name)); }

        //////////////////////////////////////////
        inline void removeEntityPrefabFromLibrary(String const& _name) { removeEntityPrefabFromLibrary(HashedCString(_name.c_str())); }
        


        //////////////////////////////////////////
        EntityPtr instantiatePrefab(
            HashedCString _name,
            EcsWorld* _world,
            EcsScene* _scene);

    protected:

        //////////////////////////////////////////
        EntityPrefabManager();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config);

    
    protected:
        static EntityPrefabManager* s_instance;

        StringKeyMap<EntityPrefabLibraryData> m_entityPrefabsLibrary;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntityPrefabManager_hpp_
//////////////////////////////////////////
