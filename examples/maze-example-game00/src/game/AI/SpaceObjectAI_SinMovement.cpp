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
#include "SpaceObjectAI_SinMovement.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
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
#include "game/SpaceObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SpaceObjectAI_SinMovement
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SpaceObjectAI_SinMovement, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SpaceObjectAI_SinMovement);

    //////////////////////////////////////////
    SpaceObjectAI_SinMovement::SpaceObjectAI_SinMovement()
        : m_timer(Math::Random(3.0f))
    {
    }

    //////////////////////////////////////////
    SpaceObjectAI_SinMovement::~SpaceObjectAI_SinMovement()
    {
    
    }

    //////////////////////////////////////////
    SpaceObjectAI_SinMovementPtr SpaceObjectAI_SinMovement::Create()
    {
        SpaceObjectAI_SinMovementPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpaceObjectAI_SinMovement, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SpaceObjectAI_SinMovement::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void SpaceObjectAI_SinMovement::update(F32 _dt)
    {
        m_timer += _dt * 2.0f;

        F32 value = (Math::Sin(m_timer) + 1.0f) * 0.5f;

        F32 const sector = Math::c_halfPi * 0.25f;
        F32 const fromAngle = +Math::c_pi - sector * 0.5f;
        F32 const toAngle = +Math::c_pi + sector * 0.5f;
        
        F32 angle = Math::Lerp(fromAngle, toAngle, value);
        Vec2DF throttle = Vec2DF::FromAngle(angle);
        m_spaceObject->setThrottle(throttle);
    }

    //////////////////////////////////////////
    void SpaceObjectAI_SinMovement::processComponentAdded()
    {
        m_spaceObject = getEntityRaw()->ensureComponent<SpaceObject>();
    }


} // namespace Maze
//////////////////////////////////////////
