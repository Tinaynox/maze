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
#include "TrailProjectile.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "game/Projectile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TrailProjectile
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TrailProjectile, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorF128, startColor, ColorF128(1.3f, 1.3f, 1.3f, 1.0f), getStartColor, setStartColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorF128, endColor, ColorF128(1.3f, 1.0f, 1.75f, 1.0f), getEndColor, setEndColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, endColorTime, 1.0f, getEndColorTime, setEndColorTime)
    );

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(TrailProjectile);

    //////////////////////////////////////////
    TrailProjectile::TrailProjectile()
        : m_startColor(1.3f, 1.3f, 1.3f, 1.0f)
        , m_endColor(1.3f, 1.0f, 1.75f, 1.0f)
        , m_endColorTime(1.0f)
    {
    }

    //////////////////////////////////////////
    TrailProjectile::~TrailProjectile()
    {
        if (m_projectile)
            m_projectile->eventPrepare.unsubscribe(this);
    }

    //////////////////////////////////////////
    TrailProjectilePtr TrailProjectile::Create()
    {
        TrailProjectilePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(TrailProjectile, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool TrailProjectile::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void TrailProjectile::update(F32 _dt)
    {
        ColorF128 color = Math::Lerp(
            m_startColor,
            m_endColor,
            Math::Clamp01(m_trailRenderer->getTimer() / m_endColorTime));

        m_trailRenderer->setColor(color);
    }

    //////////////////////////////////////////
    void TrailProjectile::processComponentAdded()
    {
        m_trailRenderer = m_entityRaw->ensureComponent<TrailRenderer3D>();
        m_projectile = m_entityRaw->ensureComponent<Projectile>();

        m_projectile->eventPrepare.subscribe(this, &TrailProjectile::notifyProjectilePrepare);
    }

    //////////////////////////////////////////
    void TrailProjectile::notifyProjectilePrepare()
    {
        if (m_trailRenderer)
        {
            m_trailRenderer->clear();
            m_trailRenderer->setColor(m_startColor);
        }
    }


} // namespace Maze
//////////////////////////////////////////
