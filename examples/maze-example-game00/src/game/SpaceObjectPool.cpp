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
#include "SpaceObjectPool.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "game/SpaceObject.hpp"
#include "game/LevelAdapter.hpp"
#include "managers/UnitManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(ComponentPoolObject<SpaceObject>, Component);


    //////////////////////////////////////////
    // Class SpaceObjectPool
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpaceObjectPool, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpaceObjectPool);

    //////////////////////////////////////////
    SpaceObjectPool::SpaceObjectPool()        
    {
    }

    //////////////////////////////////////////
    SpaceObjectPool::~SpaceObjectPool()
    {

    }

    //////////////////////////////////////////
    SpaceObjectPoolPtr SpaceObjectPool::Create(LevelAdapter* _levelAdapter)
    {
        SpaceObjectPoolPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpaceObjectPool, object, init(_levelAdapter));
        return object; 
    }

    //////////////////////////////////////////
    bool SpaceObjectPool::init(LevelAdapter* _levelAdapter)
    {
        m_levelAdapter = _levelAdapter;

        for (SpaceObjectAvatarType avatarType = SpaceObjectAvatarType::None; avatarType < SpaceObjectAvatarType::MAX; ++avatarType)
        {
            m_spaceObjectPools[avatarType] = SharedPtr<SharedObjectPool<SpaceObject>>(
                new SharedObjectPool<SpaceObject>(
                    [this,  avatarType]() -> SpaceObjectPtr 
                    {
                        EntityPtr entity = UnitManager::GetInstancePtr()->createSpaceObject(avatarType, getEntityRaw()->getECSScene());
                        entity->ensureComponent<ComponentPoolObject<SpaceObject>>(m_spaceObjectPools[avatarType]);
                        SpaceObjectPtr spaceship = entity->getComponent<SpaceObject>();
                        spaceship->setLevelAdapter(m_levelAdapter->cast<LevelAdapter>());
                        spaceship->getTransform()->setParent(m_transform);

                        spaceship->setup();

                        return spaceship;
                    },
                    [](SpaceObjectPtr const& _spaceship)
                    {
                        _spaceship->getEntityRaw()->setActiveSelf(true);
                        _spaceship->prepare();
                    },
                    [](SpaceObjectPtr const& _spaceship)
                    {
                        _spaceship->getEntityRaw()->setActiveSelf(false);
                    },
                    [](SpaceObjectPtr const& _spaceship)
                    {
                        _spaceship->getEntityRaw()->removeFromECSWorld();
                    }));
        }

        return true;
    }

    //////////////////////////////////////////
    SpaceObjectPtr SpaceObjectPool::createSpaceObject(SpaceObjectAvatarType _type)
    {
        return m_spaceObjectPools[_type]->fetch();
    }

    //////////////////////////////////////////
    Size SpaceObjectPool::getCountActive()
    {
        Size result = 0;
        
        for (SpaceObjectAvatarType avatarType = SpaceObjectAvatarType::None; avatarType < SpaceObjectAvatarType::MAX; ++avatarType)
        {
            result += m_spaceObjectPools[avatarType]->getCountActive();
        }

        return result;
    }
    
    //////////////////////////////////////////
    void SpaceObjectPool::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }


} // namespace Maze
//////////////////////////////////////////
