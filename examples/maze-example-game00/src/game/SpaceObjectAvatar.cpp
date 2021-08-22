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
#include "SpaceObjectAvatar.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "PhysicsBody.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "managers/GameManager.hpp"
#include "managers/ProjectileManager.hpp"
#include "game/Projectile.hpp"
#include "game/LevelAdapter.hpp"
#include "game/ProjectilePool.hpp"
#include "game/UnitBounds.hpp"
#include "game/PlayerOwner.hpp"
#include "game/Health.hpp"
#include "game/UnitMeshPartRenderer.hpp"
#include "game/TrailProjectile.hpp"
#include "game/UnitPartRenderer.hpp"
#include "game/ViewTriggerBehaviour.hpp"
#include "game/SpecialEffectPool.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SpaceObjectAvatar
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpaceObjectAvatar, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SpecialEffectType, destroyEffect, SpecialEffectType::None, getDestroyEffect, setDestroyEffect),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, destroyEffectScale, 1.0f, getDestroyEffectScale, setDestroyEffectScale));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpaceObjectAvatar);

    //////////////////////////////////////////
    SpaceObjectAvatar::SpaceObjectAvatar()        
    {
    }

    //////////////////////////////////////////
    SpaceObjectAvatar::~SpaceObjectAvatar()
    {

    }

    //////////////////////////////////////////
    SpaceObjectAvatarPtr SpaceObjectAvatar::Create()
    {
        SpaceObjectAvatarPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpaceObjectAvatar, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SpaceObjectAvatar::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void SpaceObjectAvatar::setup(LevelAdapter* _levelAdapter)
    {
        m_levelAdapter = _levelAdapter;
    }

    //////////////////////////////////////////
    void SpaceObjectAvatar::prepare()
    {
        m_viewTriggers = m_transform->getAllComponents<ViewTriggerBehaviour>();

        collectUnitPartRenderers();

        for (UnitPartRendererPtr const& unitPartRenderer : m_partRenderers)
            unitPartRenderer->prepare();
    }

    //////////////////////////////////////////
    void SpaceObjectAvatar::processDeath()
    {
        for (UnitPartRendererPtr const& unitPartRenderer : m_partRenderers)
            unitPartRenderer->setState(UnitPartRendererState::DeathProgress);

        if (m_destroyEffect != SpecialEffectType::None)
        {
            SpecialEffectPtr effect = m_levelAdapter->getSpecialEffectPool()->createSpecialEffect(m_destroyEffect);
            effect->getTransform()->setLocalPosition(getTransform()->getWorldPosition());
            effect->getTransform()->setLocalScale(m_destroyEffectScale);
        }
    }

    //////////////////////////////////////////
    void SpaceObjectAvatar::processDamage()
    {
        for (UnitPartRendererPtr const& unitPartRenderer : m_partRenderers)
            unitPartRenderer->increaseExtraMaterialTimer(UnitExtraMaterialType::DamageHit, 0.1f);
    }

    //////////////////////////////////////////
    void SpaceObjectAvatar::invokeViewTrigger(ViewTriggerPtr const& _viewTrigger)
    {
        for (ViewTriggerBehaviourPtr const& trigger : m_viewTriggers)
            trigger->processTrigger(_viewTrigger);
    }

    //////////////////////////////////////////
    void SpaceObjectAvatar::processComponentAdded()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
    }

    //////////////////////////////////////////
    void SpaceObjectAvatar::collectUnitPartRenderers()
    {
        m_partRenderers = m_transform->getAllComponents<UnitPartRenderer>();
    }

    //////////////////////////////////////////
    bool SpaceObjectAvatar::isDeathAnimationFinished()
    {
        for (UnitPartRendererPtr const& unitPartRenderer : m_partRenderers)
            if (unitPartRenderer->getState() != UnitPartRendererState::Death)
                return false;

        return true;
    }
    

} // namespace Maze
//////////////////////////////////////////
