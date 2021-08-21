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
#include "SpecialEffect.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-physics2d/helpers/MazePhysics2DHelper.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "game/UnitBounds.hpp"
#include "game/PlayerOwner.hpp"
#include "game/SpaceObject.hpp"
#include "game/Health.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-physics2d/helpers/MazePhysics2DHelper.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SpecialEffect
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpecialEffect, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpecialEffect);

    //////////////////////////////////////////
    SpecialEffect::SpecialEffect()
    {
    }

    //////////////////////////////////////////
    SpecialEffect::~SpecialEffect()
    {

    }

    //////////////////////////////////////////
    SpecialEffectPtr SpecialEffect::Create()
    {
        SpecialEffectPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpecialEffect, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SpecialEffect::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void SpecialEffect::update(F32 _dt)
    {
        bool isFinished = true;
        for (ParticleSystem3DPtr const& particleSystem : m_particleSystems)
        {
            if (particleSystem->getState() == ParticleSystemState::Playing ||
                particleSystem->getState() == ParticleSystemState::Pause)
            {
                isFinished = false;
                break;
            }
        }

        if (isFinished)
        {
            release();
        }
        
    }

    //////////////////////////////////////////
    void SpecialEffect::release()
    {
        m_poolObject->release(cast<SpecialEffect>());
    }

    //////////////////////////////////////////
    void SpecialEffect::setup()
    {
        if (!m_poolObject)
            m_poolObject = getEntityRaw()->getComponent<ComponentPoolObject<SpecialEffect>>();

        m_particleSystems = getEntityRaw()->getComponent<Transform3D>()->getAllComponents<ParticleSystem3D>();
    }

    //////////////////////////////////////////
    void SpecialEffect::prepare()
    {
        eventPrepare();

        for (ParticleSystem3DPtr const& particleSystem : m_particleSystems)
        {
            particleSystem->restart();
        }
    }

    //////////////////////////////////////////
    void SpecialEffect::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }


} // namespace Maze
//////////////////////////////////////////
