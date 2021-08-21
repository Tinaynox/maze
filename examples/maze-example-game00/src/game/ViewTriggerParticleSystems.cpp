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
#include "ViewTriggerParticleSystems.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ViewTriggerParticleSystems
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ViewTriggerParticleSystems, ViewTriggerBehaviour,
        // MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<ComponentPtr>, particleSystems, Vector<ComponentPtr>(), getParticleSystemsComponents, setParticleSystems),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ViewTriggerPtr, trigger, ViewTriggerPtr(), getTrigger, setTrigger),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, recursive, true, getRecursive, setRecursive));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ViewTriggerParticleSystems);

    //////////////////////////////////////////
    ViewTriggerParticleSystems::ViewTriggerParticleSystems()
    {
    }

    //////////////////////////////////////////
    ViewTriggerParticleSystems::~ViewTriggerParticleSystems()
    {

    }

    //////////////////////////////////////////
    ViewTriggerParticleSystemsPtr ViewTriggerParticleSystems::Create()
    {
        ViewTriggerParticleSystemsPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ViewTriggerParticleSystems, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ViewTriggerParticleSystems::init()
    {
        if (!ViewTriggerBehaviour::init())
            return false;

        return true;
    }    

    //////////////////////////////////////////
    void ViewTriggerParticleSystems::processTrigger(ViewTriggerPtr const& _trigger)
    {
        if (m_trigger != _trigger)
            return;

        for (ParticleSystem3DPtr const& particleSystem : m_particleSystems)
            particleSystem->play(m_recursive);
    }

    //////////////////////////////////////////
    void ViewTriggerParticleSystems::processEntityAwakened()
    {
        if (m_particleSystems.empty())
        {
            m_particleSystems = getEntityRaw()->getComponent<Transform3D>()->getAllComponents<ParticleSystem3D>();
        }
    }

} // namespace Maze
//////////////////////////////////////////
