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
#include "maze-core/ecs/components/MazeAwakeRandomRotation3D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"


//////////////////////////////////////////
// AwakeRandomRotation3Dspace Maze
namespace Maze
{
    //////////////////////////////////////////
    // Class AwakeRandomRotation3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AwakeRandomRotation3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3DB, axes, Vec3DB::c_true, getAxes, setAxes));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(AwakeRandomRotation3D);

    //////////////////////////////////////////
    AwakeRandomRotation3D::AwakeRandomRotation3D()
    {
    }

    //////////////////////////////////////////
    AwakeRandomRotation3D::~AwakeRandomRotation3D()
    {
    }

    //////////////////////////////////////////
    AwakeRandomRotation3DPtr AwakeRandomRotation3D::Create(Vec3DB const& _axes)
    {
        AwakeRandomRotation3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AwakeRandomRotation3D, object, init(_axes));
        return object;
    }

    //////////////////////////////////////////
    bool AwakeRandomRotation3D::init(Vec3DB const& _axes)
    {
        m_axes = _axes;
        return true;
    }

    //////////////////////////////////////////
    void AwakeRandomRotation3D::processEntityAwakened()
    {
        if (m_axes == Vec3DB::c_true)
        {
            getEntityRaw()->ensureComponent<Transform3D>()->setLocalRotation(
                Quaternion(
                    Vec3DF::c_unitZ,
                    Vec3DF::RandomDirection()));
        }
        else
        {
            Vec3DF angles = Vec3DF::c_zero;
            for (Size i = 0; i < 3; ++i)
                if (m_axes[i])
                    angles[i] = Math::RangeRandom(-Math::c_pi, Math::c_pi);
            getEntityRaw()->ensureComponent<Transform3D>()->setLocalRotation(angles);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
