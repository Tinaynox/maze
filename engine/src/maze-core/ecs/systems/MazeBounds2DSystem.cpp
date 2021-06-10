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
#include "maze-core/ecs/systems/MazeBounds2DSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Bounds2DSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Bounds2DSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Bounds2DSystem);

    //////////////////////////////////////////
    Bounds2DSystem::Bounds2DSystem()
    {
    }

    //////////////////////////////////////////
    Bounds2DSystem::~Bounds2DSystem()
    {
    }

    //////////////////////////////////////////
    Bounds2DSystemPtr Bounds2DSystem::Create()
    {
        Bounds2DSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Bounds2DSystem, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool Bounds2DSystem::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void Bounds2DSystem::processSystemAdded()
    {
        m_bounds2D = m_worldRaw->requestInclusiveSample<Bounds2D>();
    }
            
    //////////////////////////////////////////
    void Bounds2DSystem::processUpdate(F32 _dt)
    {
        m_bounds2D->process(
            [](Entity* entity, Bounds2D* _bounds)
            {
                if (_bounds->getTransform()->isWorldTransformChanged())
                    _bounds->dirtyBounds();
            });
    }
    
    
} // namespace Maze
//////////////////////////////////////////
