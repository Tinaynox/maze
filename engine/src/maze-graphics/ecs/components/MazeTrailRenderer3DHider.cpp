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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3DHider.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/math/MazeRandom.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TrailRenderer3DHider
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TrailRenderer3DHider, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorGradient, color, ColorGradient(), getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, duration, Vec2F(1.0f, 2.0f), getDuration, setDuration));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TrailRenderer3DHider);

    //////////////////////////////////////////
    TrailRenderer3DHider::TrailRenderer3DHider()
    {
    }

    //////////////////////////////////////////
    TrailRenderer3DHider::~TrailRenderer3DHider()
    {

    }

    //////////////////////////////////////////
    TrailRenderer3DHiderPtr TrailRenderer3DHider::Create()
    {
        TrailRenderer3DHiderPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TrailRenderer3DHider, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool TrailRenderer3DHider::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void TrailRenderer3DHider::update(F32 _dt)
    {
        if (m_timer > 0.0f)
        {
            m_timer -= _dt;
            updateColor();
        }
    }
    
    //////////////////////////////////////////
    void TrailRenderer3DHider::processEntityAwakened()
    {
        m_trailRenderer = getEntityRaw()->getComponent<TrailRenderer3D>();

        startHiding();
    }

    //////////////////////////////////////////
    void TrailRenderer3DHider::startHiding()
    {
        m_time = Random::RangeRandom(m_duration.x, m_duration.y);
        m_timer = m_time;

        updateColor();
    }

    //////////////////////////////////////////
    void TrailRenderer3DHider::updateColor()
    {
        F32 progress = Math::Clamp01(m_timer / m_time);

        ColorGradient color = m_color;
        color.multiplyAlpha(progress);
        m_trailRenderer->setColor(color);
    }



    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM(TrailRenderer3DHiderSystem, 0,
        UpdateEvent const& _event,
        Entity* _entity,
        TrailRenderer3DHider* _trailRendererHider)
    {
        _trailRendererHider->update(_event.getDt());
    }
    
} // namespace Maze
//////////////////////////////////////////
