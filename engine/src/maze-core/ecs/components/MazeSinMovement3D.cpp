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
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
// SinMovement3Dspace Maze
namespace Maze
{
    //////////////////////////////////////////
    // Class SinMovement3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SinMovement3D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec3F, axis, Vec3F::c_unitY, getAxis, setAxis),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, frequency, 2.0f, getFrequency, setFrequency),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, amplitude, 1.0f, getAmplitude, setAmplitude));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SinMovement3D);

    //////////////////////////////////////////
    SinMovement3D::SinMovement3D()
        : m_axis(Vec3F::c_unitY)
        , m_frequency(2.0f)
        , m_amplitude(1.0f)
        , m_startPosition(Vec3F::c_zero)
        , m_timer(0.0f)
    {
    }

    //////////////////////////////////////////
    SinMovement3D::~SinMovement3D()
    {
    }

    //////////////////////////////////////////
    SinMovement3DPtr SinMovement3D::Create(
        Vec3F const& _axis,
        F32 _frequency,
        F32 _amplitude)
    {
        SinMovement3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SinMovement3D, object, init(_axis, _frequency, _amplitude));
        return object;
    }

    //////////////////////////////////////////
    bool SinMovement3D::init(
        Vec3F const& _axis,
        F32 _frequency,
        F32 _amplitude)
    {
        setAxis(_axis);
        setFrequency(_frequency);
        setAmplitude(_amplitude);

        return true;
    }

    //////////////////////////////////////////
    void SinMovement3D::update(F32 _dt)
    {
        m_timer += _dt;

        F32 value = m_amplitude * Math::Sin(m_frequency * m_timer);
        m_transform->setLocalPosition(m_startPosition + m_axis * value);
    }

    //////////////////////////////////////////
    void SinMovement3D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->getComponent<Transform3D>();

        m_startPosition = m_transform->getLocalPosition();
    }



    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM_EVENT_HANDLER(SinMovement3DSystem, {},
        UpdateEvent const& _event,
        Entity* _entity,
        SinMovement3D* _sinMovement)
    {
        _sinMovement->update(_event.getDt());
    }
    
} // namespace Maze
//////////////////////////////////////////
