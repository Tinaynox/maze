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
#if (!defined(_ViewTriggerParticleSystems_hpp_))
#define _ViewTriggerParticleSystems_hpp_


//////////////////////////////////////////
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "game/DamageData.hpp"
#include "game/ViewTriggerBehaviour.hpp"
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ViewTriggerParticleSystems);
    MAZE_USING_SHARED_PTR(ViewTrigger);


    //////////////////////////////////////////
    // Class ViewTriggerBehaviour
    //
    //////////////////////////////////////////
    class ViewTriggerParticleSystems
        : public ViewTriggerBehaviour
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ViewTriggerParticleSystems, ViewTriggerBehaviour);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ViewTriggerParticleSystems);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ViewTriggerParticleSystems();

        //////////////////////////////////////////
        static ViewTriggerParticleSystemsPtr Create();

        //////////////////////////////////////////
        virtual void processTrigger(ViewTriggerPtr const& _trigger) MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        inline Vector<ParticleSystem3DPtr> const& getParticleSystems() const { return m_particleSystems; }

        //////////////////////////////////////////
        inline void setParticleSystems(Vector<ParticleSystem3DPtr> const& _particleSystems) { m_particleSystems = _particleSystems; }

        //////////////////////////////////////////
        inline Vector<ComponentPtr> getParticleSystemsComponents() const 
        {
            Vector<ComponentPtr> value;
            std::for_each(m_particleSystems.begin(), m_particleSystems.end(), [&value](ParticleSystem3DPtr const& _value) { value.push_back(_value); });
            return value;
        }

        //////////////////////////////////////////
        inline void setParticleSystems(Vector<ComponentPtr> _particleSystems)
        {
            Vector<ParticleSystem3DPtr> value;
            std::for_each(_particleSystems.begin(), _particleSystems.end(), [&value](ComponentPtr const& _value) { value.push_back(_value->cast<ParticleSystem3D>()); });
            setParticleSystems(value);
        }

        //////////////////////////////////////////
        inline ViewTriggerPtr const& getTrigger() const { return m_trigger; }

        //////////////////////////////////////////
        inline void setTrigger(ViewTriggerPtr const& _trigger) { m_trigger = _trigger; }

        //////////////////////////////////////////
        inline bool getRecursive() const { return m_recursive; }

        //////////////////////////////////////////
        inline void setRecursive(bool _recursive) { m_recursive = _recursive; }


    protected:

        //////////////////////////////////////////
        ViewTriggerParticleSystems();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        virtual bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened();

    protected:
        Vector<ParticleSystem3DPtr> m_particleSystems;
        ViewTriggerPtr m_trigger;
        bool m_recursive = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _ViewTriggerParticleSystems_hpp_
//////////////////////////////////////////
