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
#if (!defined(_MazeEcsHelper_hpp_))
#define _MazeEcsHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    namespace EcsHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API CString GetName(Entity* _entity);

        //////////////////////////////////////////
        MAZE_CORE_API void SetName(Entity* _entity, String const& _name);

        //////////////////////////////////////////
        MAZE_CORE_API EntityId GetEntityParentId(Entity const* _entity);


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureEntityIdBlock(DataBlock& _data, CString _name);

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeEntityIdToDataBlock(DataBlock& _data, CString _name, EntityId _id);

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetEntityIdBlock(DataBlock const& _data, CString _name);

        //////////////////////////////////////////
        MAZE_CORE_API EntityId DeserializeEntityIdFromDataBlock(DataBlock const& _data, CString _name);


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureEntityIdArrayBlock(DataBlock& _data, CString _name);

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetEntityIdArrayBlock(DataBlock const& _data, CString _name);


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureComponentBlock(DataBlock& _parentBlock, CString _name);

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeComponentToDataBlock(DataBlock& _cmpBlock, Component* _component);

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeComponentToDataBlock(DataBlock& _parentBlock, CString _name, Component* _component);

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetComponentBlock(DataBlock const& _parentBlock, CString _name);

        //////////////////////////////////////////
        MAZE_CORE_API Component* DeserializeComponentFromDataBlock(EcsWorld* _ecsWorld, DataBlock const& _cmpBlock);

        //////////////////////////////////////////
        MAZE_CORE_API Component* DeserializeComponentFromDataBlock(EcsWorld* _ecsWorld, DataBlock const& _parentBlock, CString _name);


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureComponentArrayBlock(DataBlock& _data, CString _name);

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetComponentArrayBlock(DataBlock const& _data, CString _name);


        //////////////////////////////////////////
        // Note: Defined out-of-line in MazeEcsHelper.inl, since the bodies need
        // the complete definitions of Transform2D/Transform3D, which would otherwise
        // create a header circular-include trap (Transform2D.hpp includes this header
        // before its own class body is complete). Include MazeEcsHelper.inl explicitly
        // (after Transform2D.hpp/Transform3D.hpp are fully visible) wherever these are used.
        //////////////////////////////////////////
        template <typename TComponent>
        TComponent* GetFirstTrunkComponent(Entity* _entity);

        //////////////////////////////////////////
        template <typename TComponent>
        TComponent* FindComponentRawRecursive(Entity* _entity);

        //////////////////////////////////////////
        MAZE_CORE_API Entity* FindEntityWithSerializationId(Entity* _entity, EcsSerializationId _sid);

        //////////////////////////////////////////
        MAZE_CORE_API Entity* FindEntityWithSerializationId(Transform2D* _transform2D, EcsSerializationId _sid);

        //////////////////////////////////////////
        MAZE_CORE_API Entity* FindEntityWithSerializationId(Transform3D* _transform3D, EcsSerializationId _sid);

        //////////////////////////////////////////
        // Searches _entity's subtree for _sid within _entity's prefab sid space only:
        // subtrees of nested prefab instances are skipped (their internals belong to
        // foreign sid spaces), but nested instance root entities themselves are
        // matchable - their sids belong to the current space.
        MAZE_CORE_API Entity* FindEntityWithSerializationIdInPrefabSpace(Entity* _entity, EcsSerializationId _sid);


        //////////////////////////////////////////
        // Prefab sid chain refs: a serialized reference to an entity/component inside
        // a prefab instance of any nesting depth. Stored as repeated 'prefabSid' params
        // (one per prefab space, outermost first) + 'prefabEntitySid' for the target
        // entity (+ 'prefabComponentSid' for component refs).
        //////////////////////////////////////////
        static MAZE_CONSTEXPR S32 c_maxPrefabRefDepth = 16;

        //////////////////////////////////////////
        // Returns the root entity of the nearest prefab instance strictly above _entity
        MAZE_CORE_API Entity* GetEnclosingPrefabInstanceRootEntity(Entity* _entity);

        //////////////////////////////////////////
        // Builds the chain of prefab instance sids leading from the serialized document
        // down to _entity's prefab space (outermost first). Each hop sid belongs to the
        // sid space of the previous hop's prefab. Returns the hops count, or 0 if _entity
        // is not reachable through a prefab instance registered in _entityIndices.
        MAZE_CORE_API S32 BuildPrefabSidChain(
            Entity* _entity,
            FlatHashMap<EntityId, EcsSerializationId> const& _entityIndices,
            EcsSerializationId (&_outChain)[c_maxPrefabRefDepth]);

        //////////////////////////////////////////
        MAZE_CORE_API void WritePrefabRefParams(
            DataBlock* _block,
            EcsSerializationId const* _chain,
            S32 _chainCount,
            EcsSerializationId _entitySid);

        //////////////////////////////////////////
        MAZE_CORE_API void RemovePrefabRefParams(DataBlock* _block);

        //////////////////////////////////////////
        // Resolves a sid chain written by WritePrefabRefParams back to the target entity.
        // Returns nullptr if _block has no prefabSid params or the chain is broken.
        MAZE_CORE_API Entity* ResolvePrefabSidChainTarget(
            DataBlock const& _block,
            FlatHashMap<EcsSerializationId, EntityPtr> const& _outEntities);

        //////////////////////////////////////////
        MAZE_CORE_API ComponentPtr const& ResolvePrefabComponentRef(
            DataBlock const& _block,
            FlatHashMap<EcsSerializationId, EntityPtr> const& _outEntities);


        //////////////////////////////////////////
        // Returns true if _ancestor is a transform ancestor of _entity (or the entity itself)
        MAZE_CORE_API bool IsEntityAncestor(Entity* _ancestor, Entity* _entity);

        //////////////////////////////////////////
        // Builds the sid path from _root's prefab space down to _target: sids of nested
        // prefab instance roots (outermost first) followed by _target's own sid.
        // Returns the path length, 0 if _target == _root, or -1 if _target is not
        // inside _root's subtree.
        MAZE_CORE_API S32 BuildPrefabSidPathToRoot(
            Entity* _target,
            Entity* _root,
            EcsSerializationId (&_outPath)[c_maxPrefabRefDepth]);

        //////////////////////////////////////////
        // Rewrites every ':EntityId' block's runtime eid in _dataBlock (recursively) into
        // a _root-relative form ('self' flag / 'relSid' path / 'external' eid) resolved in
        // _world. Used to compare serialized data of a prefab instance against its identity
        // prefab independently of runtime eids.
        MAZE_CORE_API void NormalizeEntityIdBlocksToPrefabSpace(
            DataBlock& _dataBlock,
            Entity* _root,
            EcsWorld* _world);

    } // namespace EcsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsHelper_hpp_
//////////////////////////////////////////
