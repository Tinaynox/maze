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
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScissorMask2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ScissorMask2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingLeft, 0.0f, getPaddingLeft, setPaddingLeft),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingRight, 0.0f, getPaddingRight, setPaddingRight),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingTop, 0.0f, getPaddingTop, setPaddingTop),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingBottom, 0.0f, getPaddingBottom, setPaddingBottom));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ScissorMask2D);

    //////////////////////////////////////////
    ScissorMask2D::ScissorMask2D()
        : m_paddingLeft(0.0f)
        , m_paddingRight(0.0f)
        , m_paddingTop(0.0f)
        , m_paddingBottom(0.0f)
        , m_scissorBounds(AABB2D::c_zero)
        , m_scissorBoundsDirty(true)
    {
    }

    //////////////////////////////////////////
    ScissorMask2D::~ScissorMask2D()
    {
        if (m_bounds)
            m_bounds->eventBoundsCalculated.unsubscribe(this);
    }

    //////////////////////////////////////////
    ScissorMask2DPtr ScissorMask2D::Create()
    {
        ScissorMask2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScissorMask2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ScissorMask2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ScissorMask2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_bounds = getEntityRaw()->ensureComponent<Bounds2D>();

        m_bounds->eventBoundsCalculated.subscribe(this, &ScissorMask2D::notifyBoundsCalculated);
    }
    
    //////////////////////////////////////////
    AABB2D const& ScissorMask2D::getScissorBounds()
    {
        if (m_scissorBoundsDirty || m_bounds->isDirty())
        {
            m_bounds->getBounds();

            Vec2DF const& size = m_transform->getSize();
            Mat4DF const& worldTransform = m_transform->getWorldTransform();
            Vec2DF lb = worldTransform.transformAffine(Vec2DF(m_paddingLeft, m_paddingBottom));
            Vec2DF rb = worldTransform.transformAffine(Vec2DF(size.x - m_paddingRight, m_paddingBottom));
            Vec2DF rt = worldTransform.transformAffine(Vec2DF(size.x - m_paddingRight, size.y - m_paddingTop));
            Vec2DF lt = worldTransform.transformAffine(Vec2DF(m_paddingLeft, size.y - m_paddingTop));

            F32 minX = Math::Min(lb.x, rb.x, rt.x, lt.x);
            F32 maxX = Math::Max(lb.x, rb.x, rt.x, lt.x);
            F32 minY = Math::Min(lb.y, rb.y, rt.y, lt.y);
            F32 maxY = Math::Max(lb.y, rb.y, rt.y, lt.y);

            m_scissorBounds = AABB2D(minX, minY, maxX, maxY);

            m_scissorBoundsDirty = false;
        }

        return m_scissorBounds;
    }

    //////////////////////////////////////////
    void ScissorMask2D::notifyBoundsCalculated(AABB2D const& _bounds)
    {
        m_scissorBoundsDirty = true;
    }
    
} // namespace Maze
//////////////////////////////////////////
