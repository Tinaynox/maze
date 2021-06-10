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
#if (!defined(_Health_hpp_))
#define _Health_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "game/DamageData.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Health);


    //////////////////////////////////////////
    // Class Health
    //
    //////////////////////////////////////////
    class Health
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Health, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Health);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Health();

        //////////////////////////////////////////
        static HealthPtr Create();


        //////////////////////////////////////////
        void setHealth(F32 _health);

        //////////////////////////////////////////
        inline F32 getHealth() const { return m_health; }


        //////////////////////////////////////////
        void setHealthMax(F32 _healthMax);

        //////////////////////////////////////////
        inline F32 getHealthMax() const { return m_healthMax; }


        //////////////////////////////////////////
        inline bool isAlive() const { return m_health > 0.0f; }


        //////////////////////////////////////////
        bool doDamage(DamageData const& _damageData);

        //////////////////////////////////////////
        bool doDamage(F32 _damageValue);
        
    public:

        MultiDelegate<> eventDeath;
        MultiDelegate<DamageData const&> eventDamage;
        MultiDelegate<F32> eventHealthChanged;
        MultiDelegate<F32> eventHealthMaxChanged;

    protected:

        //////////////////////////////////////////
        Health();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processComponentAdded() MAZE_OVERRIDE;

    protected:
        F32 m_health;
        F32 m_healthMax;

    };


} // namespace Maze
//////////////////////////////////////////


#endif // _Health_hpp_
//////////////////////////////////////////
