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
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SizePolicy2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SizePolicy2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, flags, (Flags::Width | Flags::Height), getFlags, setFlags),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2DF, sizeDelta, Vec2DF::c_zero, getSizeDelta, setSizeDelta));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SizePolicy2D);

    //////////////////////////////////////////
    SizePolicy2D::SizePolicy2D()
        : m_flags(Flags::Width | Flags::Height)
        , m_sizeDelta(Vec2DF::c_zero)
    {
    }

    //////////////////////////////////////////
    SizePolicy2D::~SizePolicy2D()
    {

    }

    //////////////////////////////////////////
    SizePolicy2DPtr SizePolicy2D::Create()
    {
        SizePolicy2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SizePolicy2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SizePolicy2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void SizePolicy2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
    }

    //////////////////////////////////////////
    void SizePolicy2D::updateSize()
    {
        if (!m_transform->getParent())
            return;

        bool affectWidth = m_flags & Flags::Width;
        bool affectHeight = m_flags & Flags::Height;

        if (affectWidth && affectHeight)
            m_transform->setSize(m_transform->getParent()->getSize() + m_sizeDelta);
        else
        if (affectWidth)
            m_transform->setWidth(m_transform->getParent()->getSize().x + m_sizeDelta.x);
        else
        if (affectHeight)
            m_transform->setHeight(m_transform->getParent()->getSize().y + m_sizeDelta.y);
        
    }

    //////////////////////////////////////////
    void SizePolicy2D::setSizeDelta(Vec2DF const& _sizeDelta)
    {
        if (m_sizeDelta == _sizeDelta)
            return;

        m_sizeDelta = _sizeDelta;

        if (m_transform)
            m_transform->dirtyWorldTransform();
    }
    
    
} // namespace Maze
//////////////////////////////////////////
