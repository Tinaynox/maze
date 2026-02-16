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
#if (!defined(_EntitySerializationManager_hpp_))
#define _EntitySerializationManager_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/assets/MazeAssetUnitId.hpp"
#include <tinyxml2/tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitySerializationManager);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(PrefabInstance);
    

    //////////////////////////////////////////
    struct MAZE_CORE_API EntitySerializationData
    {
        EntityPtr entity;
        Vector<ComponentPtr> components;
    };

    //////////////////////////////////////////
    struct MAZE_CORE_API PrefabSerializationData
    {
        PrefabInstance* prefabInstance = nullptr;
    };

    //////////////////////////////////////////
    struct MAZE_CORE_API EntitiesToDataBlockContext
    {
        //////////////////////////////////////////
        EcsWorld* ecsWorld = nullptr;
        Vector<EntitySerializationData> entityComponents;
        Vector<PrefabSerializationData> prefabs;
        Map<void*, EcsSerializationId> pointerIndices;
        Map<EntityId, EcsSerializationId> entityIndices;
        Map<AssetUnitId, EntityPtr> identityPrefabs;
    };

    //////////////////////////////////////////
    struct MAZE_CORE_API EntitiesFromDataBlockContext
    {
        //////////////////////////////////////////
        EcsWorld* world = nullptr;
        EcsScene* scene = nullptr;
        UnorderedMap<EntityId, EntityPtr> entitiesPerEntityId;
        Map<EcsSerializationId, EntityPtr> outEntities;
        Map<EcsSerializationId, ComponentPtr> outComponents;
    };


    //////////////////////////////////////////
    using SerializeComponentToDataBlockFunc = std::function<
        void(EntitiesToDataBlockContext& _context, ComponentPtr const& _component, DataBlock& _parentDataBlock)>;
    using SerializeComponentModificationsToDataBlockFunc = std::function<
        void(EntitiesToDataBlockContext& _context, ComponentPtr const& _component, ComponentPtr const& _identityComponent, DataBlock& _parentDataBlock)>;
    using DeserializeComponentFromDataBlockFunc = std::function<
        void(DataBlock const& _componentBlock, ComponentPtr const& _component, EntitiesFromDataBlockContext& _context)>;
    using DeserializeComponentModificaionFromDataBlockFunc = std::function<
        void(DataBlock const& _modificationBlock, ComponentPtr const& _component, CString _propertyName, EntitiesFromDataBlockContext& _context)>;


    //////////////////////////////////////////
    struct MAZE_CORE_API ComponentSerializationFunctions
    {
        SerializeComponentToDataBlockFunc toDataBlockFunc;
        SerializeComponentModificationsToDataBlockFunc modificationsToDataBlockFunc;
        DeserializeComponentFromDataBlockFunc fromDataBlockFunc;
        DeserializeComponentModificaionFromDataBlockFunc modificationFromDataBlockFunc;
    };


    //////////////////////////////////////////
    // Class EntitySerializationManager
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntitySerializationManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ~EntitySerializationManager();

        //////////////////////////////////////////
        static void Initialize(EntitySerializationManagerPtr& _manager);
        

        //////////////////////////////////////////
        static inline EntitySerializationManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline EntitySerializationManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        bool saveEntitiesToDataBlock(Set<Entity*> const&, DataBlock& _dataBlock) const;

        //////////////////////////////////////////
        bool savePrefabToDataBlock(EntityPtr const& _entity, DataBlock& _dataBlock) const;

        //////////////////////////////////////////
        bool saveSceneToDataBlock(EcsScenePtr const& _scene, DataBlock& _dataBlock) const;

        //////////////////////////////////////////
        bool savePrefabToDataBlockFile(EntityPtr const& _entity, Path const& _fileFullPath) const;

        //////////////////////////////////////////
        bool saveSceneToDataBlockFile(EcsScenePtr const& _scene, Path const& _fileFullPath) const;

        //////////////////////////////////////////
        bool loadSceneFromDataBlock(EcsScenePtr const& _scene, DataBlock& _dataBlock) const;

        //////////////////////////////////////////
        bool loadSceneFromDataBlockFile(EcsScenePtr const& _scene, Path const& _fileFullPath) const;


        //////////////////////////////////////////
        EntityPtr loadPrefab(
            AssetFilePtr const& _assetFile,
            EcsWorld* _world = nullptr,
            EcsScene* _scene = nullptr) const;

        //////////////////////////////////////////
        EntityPtr loadPrefab(
            Path const& _assetFileName,
            EcsWorld* _world = nullptr,
            EcsScene* _scene = nullptr) const;

        //////////////////////////////////////////
        EntityPtr loadPrefab(
            DataBlock& _dataBlock,
            EcsWorld* _world = nullptr,
            EcsScene* _scene = nullptr) const;



        //////////////////////////////////////////
        void collectAllChildrenEntity(
            Entity* _entity,
            Vector<EntityPtr>& _entities,
            Vector<PrefabSerializationData>& _prefabInstances) const;

        //////////////////////////////////////////
        void collectEntitiesComponentsMap(
            Set<Entity*> const& _entities,
            Vector<EntitySerializationData>& _entityComponents,
            Vector<PrefabSerializationData>& _prefabs) const;

        //////////////////////////////////////////
        void collectEntityComponentsMap(
            EntityPtr const& _entity,
            Vector<EntitySerializationData>& _entityComponents,
            Vector<PrefabSerializationData>& _prefabs) const;


        //////////////////////////////////////////
        void registerComponentSerializationByClassUID(ClassUID _classUID, ComponentSerializationFunctions _funcs);


    protected:

        //////////////////////////////////////////
        EntitySerializationManager();

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        void saveComponentToDataBlockDefault(
            EntitiesToDataBlockContext& _context,
            ComponentPtr const& _component,
            DataBlock& _parentBlock) const;

        //////////////////////////////////////////
        void saveComponentModificationsToDataBlockDefault(
            EntitiesToDataBlockContext& _context,
            ComponentPtr const& _component,
            ComponentPtr const& _identityComponent,
            DataBlock& _parentBlock) const;

        //////////////////////////////////////////
        void saveEntitiesToDataBlock(
            EntitiesToDataBlockContext& _context,
            DataBlock& _dataBlock) const;

        //////////////////////////////////////////
        void replaceDataBlockEcsIds(
            DataBlock& _dataBlock,
            Map<EntityId, S32> const& _entityIndices,
            Vector<EntitySerializationData> const& _entityComponents,
            Map<void*, S32>& _pointerIndices,
            EcsWorld* _ecsWorld) const;

        //////////////////////////////////////////
        void restoreDataBlockEcsIds(
            DataBlock& _dataBlock,
            Map<S32, EntityPtr>& _outEntities,
            Map<S32, ComponentPtr>& _outComponents) const;

        //////////////////////////////////////////
        void loadComponentFromDataBlockDefault(
            DataBlock const& _componentBlock,
            ComponentPtr const& _component,
            EntitiesFromDataBlockContext& _context) const;

        //////////////////////////////////////////
        void loadComponentModificationFromDataBlockDefault(
            DataBlock const& _modificationBlock,
            ComponentPtr const& _component,
            CString _propertyName,
            EntitiesFromDataBlockContext& _context) const;

        //////////////////////////////////////////
        void loadEntities(
            DataBlock& _dataBlock,
            EntitiesFromDataBlockContext& _context) const;

    protected:
        static EntitySerializationManager* s_instance;

        Set<ClassUID> m_componentsToIgnore;
        Map<ClassUID, ComponentSerializationFunctions> m_componentCustomSerializationByClassUID;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _EntitySerializationManager_hpp_
//////////////////////////////////////////
