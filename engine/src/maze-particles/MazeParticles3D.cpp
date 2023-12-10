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
#include "MazeParticlesHeader.hpp"
#include "maze-particles/MazeParticles3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Particles3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(Particles3D);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Particles3D);


    //////////////////////////////////////////
    Particles3D::Particles3D()
        : m_capacity(0)
        , m_aliveCount(0)
    {
        
    }

    //////////////////////////////////////////
    void Particles3D::update()
    {
        S32 aliveCount = m_aliveCount;

        if (aliveCount == 0)
            return;

        Vec3F pos = m_positions[0];
        F32 size = m_sizes[0].current;
        F32 halfSize = size * 0.5f;

        AABB3D aabb(
            pos.x - halfSize,
            pos.y - halfSize,
            pos.z - halfSize,
            pos.x + halfSize,
            pos.y + halfSize,
            pos.z + halfSize);

        for (S32 i = 0; i < aliveCount; )
        {
            // Alive
            if (m_lifes[i].current >= 0.0f)
            {
                pos = m_positions[i];
                size = m_sizes[i].current;
                halfSize = size * 0.5f;

                aabb.applyUnion(
                    pos.x - halfSize,
                    pos.y - halfSize,
                    pos.z - halfSize,
                    pos.x + halfSize,
                    pos.y + halfSize,
                    pos.z + halfSize);

                ++i;
            }
            // Dead
            else
            {
                aliveCount--;

                // Swap last alive particle with
                resetData(i, aliveCount);
            }
        }

        m_aliveCount = aliveCount;

        m_bounds = aabb;
    }

    //////////////////////////////////////////
    void Particles3D::setCapacity(S32 _capacity)
    {
        if (m_capacity == _capacity)
            return;

        m_capacity = _capacity;

        m_seeds.resize(_capacity);
        m_positions.resize(_capacity);
        m_directions.resize(_capacity);
        m_rotations.resize(_capacity);
        m_lifes.resize(_capacity);
        m_sizes.resize(_capacity);
        m_colorsInitial.resize(_capacity);
        m_colorsCurrent.resize(_capacity);
        m_movements.resize(_capacity);
        m_animationFrames.resize(_capacity);

        m_sqrDistanceToCamera.resize(_capacity);

        m_renderTransforms.resize(_capacity);
        m_renderColors.resize(_capacity);
        m_renderUVs.resize(_capacity);
    }

    
} // namespace Maze
//////////////////////////////////////////
