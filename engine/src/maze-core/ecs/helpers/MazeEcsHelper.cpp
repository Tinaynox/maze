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
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EcsHelper
    {

        //////////////////////////////////////////
        MAZE_CORE_API CString GetName(Entity* _entity)
        {
            StaticName* staticName = _entity->getComponentRaw<StaticName>();
            if (staticName)
                return staticName->getName();

            Name* name = _entity->getComponentRaw<Name>();
            if (name)
                return name->getName().c_str();

            return "";
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SetName(Entity* _entity, String const& _name)
        {
            _entity->removeComponent<StaticName>();
            _entity->ensureComponent<Name>()->setName(_name);
        }

        //////////////////////////////////////////
        MAZE_CORE_API EntityId GetEntityParentId(Entity const* _entity)
        {
            EntityId parentId = c_invalidEntityId;

            Transform2D* transform2D = _entity->getComponentRaw<Transform2D>();
            if (transform2D && transform2D->getParent())
                parentId = transform2D->getParent()->getEntityId();
            else
            {
                Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
                if (transform3D && transform3D->getParent())
                    parentId = transform3D->getParent()->getEntityId();
            }

            return parentId;
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureEntityIdBlock(DataBlock& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:EntityId", _name);
            return _data.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeEntityIdToDataBlock(DataBlock& _data, CString _name, EntityId _id)
        {
            EnsureEntityIdBlock(_data, _name)->setS32(MAZE_HCS("value"), (S32)_id);
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetEntityIdBlock(DataBlock const& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:EntityId", _name);

            return _data.getDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API EntityId DeserializeEntityIdFromDataBlock(DataBlock const& _data, CString _name)
        {
            DataBlock const* dataBlock = GetEntityIdBlock(_data, _name);
            if (!dataBlock)
                return c_invalidEntityId;

            return EntityId(dataBlock->getS32(MAZE_HCS("value"), (S32)c_invalidEntityId));
        }


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureEntityIdArrayBlock(DataBlock& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<EntityId>", _name);
            return _data.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetEntityIdArrayBlock(DataBlock const& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<EntityId>", _name);

            return _data.getDataBlock(HashedCString(buffer));
        }


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureComponentBlock(DataBlock& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Component", _name);
            return _data.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SerializeComponentToDataBlock(DataBlock& _data, CString _name, Component* _component)
        {
            if (!_component || !_component->getEntity())
                return;

            DataBlock* cmpBlock = EnsureComponentBlock(_data, _name);
            SerializeEntityIdToDataBlock(*cmpBlock, "eid", _component->getEntityId());
            cmpBlock->setString(MAZE_HCS("class"), _component->getComponentClassName());
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetComponentBlock(DataBlock const& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Component", _name);

            return _data.getDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API Component* DeserializeComponentFromDataBlock(EcsWorld* _ecsWorld, DataBlock const& _data, CString _name)
        {
            DataBlock const* cmpBlock = GetComponentBlock(_data, _name);
            if (!cmpBlock)
                return nullptr;

            EntityId eid = DeserializeEntityIdFromDataBlock(*cmpBlock, "eid");
            EntityPtr const& entity = _ecsWorld->getEntity(eid);
            if (!entity)
                return nullptr;

            CString className = cmpBlock->getCString(MAZE_HCS("class"));
            if (!className)
                return nullptr;

            ComponentId componentId = GetComponentIdByName(className);
            return entity->getComponentById(componentId).get();
        }


        //////////////////////////////////////////
        MAZE_CORE_API DataBlock* EnsureComponentArrayBlock(DataBlock& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<Component>", _name);
            return _data.ensureDataBlock(HashedCString(buffer));
        }

        //////////////////////////////////////////
        MAZE_CORE_API DataBlock const* GetComponentArrayBlock(DataBlock const& _data, CString _name)
        {
            Char buffer[128];
            StringHelper::FormatString(buffer, sizeof(buffer), "%s:Array<Component>", _name);

            return _data.getDataBlock(HashedCString(buffer));
        }

    } // namespace EcsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
