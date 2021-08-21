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
#include "SpecialEffectPool.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"
#include "game/LevelAdapter.hpp"
#include "managers/SpecialEffectManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT_TEMPLATE(ComponentPoolObject<SpecialEffect>, Component);


    //////////////////////////////////////////
    // Class SpecialEffectPool
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpecialEffectPool, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpecialEffectPool);

    //////////////////////////////////////////
    SpecialEffectPool::SpecialEffectPool()        
    {
    }

    //////////////////////////////////////////
    SpecialEffectPool::~SpecialEffectPool()
    {

    }

    //////////////////////////////////////////
    SpecialEffectPoolPtr SpecialEffectPool::Create(LevelAdapter* _levelAdapter)
    {
        SpecialEffectPoolPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpecialEffectPool, object, init(_levelAdapter));
        return object; 
    }

    //////////////////////////////////////////
    bool SpecialEffectPool::init(LevelAdapter* _levelAdapter)
    {
        m_levelAdapter = _levelAdapter;

        for (SpecialEffectType avatarType = SpecialEffectType::None; avatarType < SpecialEffectType::MAX; ++avatarType)
        {
            m_specialEffectPools[avatarType] = SharedPtr<SharedObjectPool<SpecialEffect>>(
                new SharedObjectPool<SpecialEffect>(
                    [this,  avatarType]() -> SpecialEffectPtr 
                    {
                        EntityPtr entity = SpecialEffectManager::GetInstancePtr()->createSpecialEffect(avatarType, getEntityRaw()->getECSScene());
                        entity->ensureComponent<ComponentPoolObject<SpecialEffect>>(m_specialEffectPools[avatarType]);
                        SpecialEffectPtr effect = entity->getComponent<SpecialEffect>();
                        effect->getTransform()->setParent(m_transform);

                        effect->setup();

                        return effect;
                    },
                    [](SpecialEffectPtr const& _effect)
                    {
                        _effect->getEntityRaw()->setActiveSelf(true);
                        _effect->prepare();
                    },
                    [](SpecialEffectPtr const& _effect)
                    {
                        _effect->getEntityRaw()->setActiveSelf(false);
                    },
                    [](SpecialEffectPtr const& _effect)
                    {
                        _effect->getEntityRaw()->removeFromECSWorld();
                    }));
        }

        return true;
    }

    //////////////////////////////////////////
    SpecialEffectPtr SpecialEffectPool::createSpecialEffect(SpecialEffectType _type)
    {
        return m_specialEffectPools[_type]->fetch();
    }
    
    //////////////////////////////////////////
    void SpecialEffectPool::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }


} // namespace Maze
//////////////////////////////////////////
