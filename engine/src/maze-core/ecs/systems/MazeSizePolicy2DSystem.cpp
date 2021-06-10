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
#include "maze-core/ecs/systems/MazeSizePolicy2DSystem.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SizePolicy2DSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SizePolicy2DSystem, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SizePolicy2DSystem);

    //////////////////////////////////////////
    SizePolicy2DSystem::SizePolicy2DSystem()
    {
    }

    //////////////////////////////////////////
    SizePolicy2DSystem::~SizePolicy2DSystem()
    {
    }

    //////////////////////////////////////////
    SizePolicy2DSystemPtr SizePolicy2DSystem::Create()
    {
        SizePolicy2DSystemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SizePolicy2DSystem, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SizePolicy2DSystem::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void SizePolicy2DSystem::processSystemAdded()
    {
        m_sizePolicy2D = m_worldRaw->requestInclusiveSample<SizePolicy2D>();
    }
        
    //////////////////////////////////////////
    void SizePolicy2DSystem::processUpdate(F32 _dt)
    {
        m_sizePolicy2D->process(
            [](Entity* entity, SizePolicy2D* _sizePolicy)
            {
                if (_sizePolicy->getTransform()->isWorldTransformChanged())
                    _sizePolicy->updateSize();
            });
    }
    
    
} // namespace Maze
//////////////////////////////////////////
