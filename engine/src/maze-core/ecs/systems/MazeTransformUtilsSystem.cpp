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
#include "MazeCoreHeader.hpp"
#include "maze-core/ecs/systems/MazeTransformUtilsSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TransformUtilsSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TransformUtilsSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TransformUtilsSystem);

    //////////////////////////////////////////
    TransformUtilsSystem::TransformUtilsSystem()
    {
    }

    //////////////////////////////////////////
    TransformUtilsSystem::~TransformUtilsSystem()
    {
    }

    //////////////////////////////////////////
    TransformUtilsSystemPtr TransformUtilsSystem::Create()
    {
        TransformUtilsSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TransformUtilsSystem, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool TransformUtilsSystem::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void TransformUtilsSystem::processSystemAdded()
    {
        m_rotors3D = m_worldRaw->requestInclusiveSample<Rotor3D>();
        m_sinMovements3D = m_worldRaw->requestInclusiveSample<SinMovement3D>();
    }
        
    //////////////////////////////////////////
    void TransformUtilsSystem::processUpdate(F32 _dt)
    {
        m_rotors3D->process(
            [_dt](Entity* entity, Rotor3D* _rotor)
            {
                _rotor->update(_dt);
            });

        m_sinMovements3D->process(
            [_dt](Entity* entity, SinMovement3D* _sinMovement)
            {
                _sinMovement->update(_dt);
            });
    }
    
    
} // namespace Maze
//////////////////////////////////////////
