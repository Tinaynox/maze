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
#include "ProjectilePool.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "managers/ProjectileManager.hpp"
#include "game/LevelAdapter.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(ComponentPoolObject<Projectile>, Component);


    //////////////////////////////////////////
    // Class ProjectilePool
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ProjectilePool, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ProjectilePool);

    //////////////////////////////////////////
    ProjectilePool::ProjectilePool()        
    {
    }

    //////////////////////////////////////////
    ProjectilePool::~ProjectilePool()
    {

    }

    //////////////////////////////////////////
    ProjectilePoolPtr ProjectilePool::Create(LevelAdapter* _levelAdapter)
    {
        ProjectilePoolPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ProjectilePool, object, init(_levelAdapter));
        return object; 
    }

    //////////////////////////////////////////
    bool ProjectilePool::init(LevelAdapter* _levelAdapter)
    {
        m_levelAdapter = _levelAdapter;

        for (ProjectileAvatarType avatarType = ProjectileAvatarType::None; avatarType < ProjectileAvatarType::MAX; ++avatarType)
        {
            m_projectilePools[avatarType] = SharedPtr<SharedObjectPool<Projectile>>(
                new SharedObjectPool<Projectile>(
                    [this,  avatarType]() -> ProjectilePtr 
                    {
                        EntityPtr entity = ProjectileManager::GetInstancePtr()->createProjectile(avatarType, getEntityRaw()->getECSScene());
                        entity->ensureComponent<ComponentPoolObject<Projectile>>(m_projectilePools[avatarType]);
                        ProjectilePtr projectile = entity->getComponent<Projectile>();
                        projectile->getTransform()->setParent(m_transform);

                        projectile->setup();

                        return projectile;
                    },
                    [](ProjectilePtr const& _projectile)
                    {
                        _projectile->getEntityRaw()->setActiveSelf(true);
                        _projectile->prepare();
                    },
                    [](ProjectilePtr const& _projectile)
                    {
                        _projectile->getEntityRaw()->setActiveSelf(false);
                    },
                    [](ProjectilePtr const& _projectile)
                    {
                        _projectile->getEntityRaw()->removeFromECSWorld();
                    }));
        }

        return true;
    }

    //////////////////////////////////////////
    ProjectilePtr ProjectilePool::createProjectile(ProjectileAvatarType _type)
    {
        return m_projectilePools[_type]->fetch();
    }
    
    //////////////////////////////////////////
    void ProjectilePool::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }


} // namespace Maze
//////////////////////////////////////////
