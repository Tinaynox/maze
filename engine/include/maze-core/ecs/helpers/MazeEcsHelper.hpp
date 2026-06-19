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

    } // namespace EcsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsHelper_hpp_
//////////////////////////////////////////
