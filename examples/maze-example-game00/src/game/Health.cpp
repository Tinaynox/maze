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
#include "Health.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Health
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Health, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, health, 100.0f, getHealth, setHealth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, healthMax, 100.0f, getHealthMax, setHealthMax));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Health);

    //////////////////////////////////////////
    Health::Health()
        : m_healthMax(100.0f)
        , m_health(100.0f)
    {
    }

    //////////////////////////////////////////
    Health::~Health()
    {

    }

    //////////////////////////////////////////
    HealthPtr Health::Create()
    {
        HealthPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Health, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Health::init()
    {

        return true;
    }    

    //////////////////////////////////////////
    void Health::processComponentAdded()
    {
        
    }

    //////////////////////////////////////////
    void Health::setHealth(F32 _health)
    {
        if (m_health == _health)
            return;

        _health = Math::Clamp(_health, 0.0f, m_healthMax);

        m_health = _health;

        eventHealthChanged(m_health);
    }

    //////////////////////////////////////////
    void Health::setHealthMax(F32 _healthMax)
    {
        if (m_healthMax == _healthMax)
            return;

        m_healthMax = _healthMax;

        eventHealthChanged(m_healthMax);
    }

    //////////////////////////////////////////
    bool Health::doDamage(DamageData const& _damageData)
    {
        if (!isAlive())
            return false;

        F32 damageValue = _damageData.damageValue;

        setHealth(m_health - damageValue);

        eventDamage(_damageData);

        if (m_health <= 0.0f)
        {
            eventDeath();
        }

        return true;
    }

    //////////////////////////////////////////
    bool Health::doDamage(F32 _damageValue)
    {
        return doDamage(DamageData(_damageValue));
    }


} // namespace Maze
//////////////////////////////////////////
