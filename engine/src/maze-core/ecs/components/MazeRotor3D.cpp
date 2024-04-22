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
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
// Rotor3Dspace Maze
namespace Maze
{
    //////////////////////////////////////////
    // Class Rotor3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Rotor3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, active, true, getActive, setActive),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3F, axis, Vec3F::c_unitY, getAxis, setAxis),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, speed, 5.0f, getSpeed, setSpeed));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Rotor3D);

    //////////////////////////////////////////
    Rotor3D::Rotor3D()
    {
    }

    //////////////////////////////////////////
    Rotor3D::~Rotor3D()
    {
    }

    //////////////////////////////////////////
    Rotor3DPtr Rotor3D::Create(Vec3F const& _axis, F32 _speed)
    {
        Rotor3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Rotor3D, object, init(_axis, _speed));
        return object;
    }

    //////////////////////////////////////////
    bool Rotor3D::init(Vec3F const& _axis, F32 _speed)
    {
        setAxis(_axis);
        setSpeed(_speed);

        return true;
    }

    //////////////////////////////////////////
    void Rotor3D::update(F32 _dt)
    {
        if (m_active)
            m_transform->rotate(m_axis.normalizedCopy(), m_speed * _dt);
    }

    //////////////////////////////////////////
    void Rotor3D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->getComponent<Transform3D>();
    }



    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM(Rotor3DSystem, -200,
        UpdateEvent const& _event,
        Entity* _entity,
        Rotor3D* _rotor)
    {
        _rotor->update(_event.getDt());
    }
    
} // namespace Maze
//////////////////////////////////////////
