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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
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
    // Class Layout2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Layout2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingLeft, 0.0f, getPaddingLeft, setPaddingLeft),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingRight, 0.0f, getPaddingRight, setPaddingRight),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingTop, 0.0f, getPaddingTop, setPaddingTop),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, paddingBottom, 0.0f, getPaddingBottom, setPaddingBottom),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(HorizontalAlignment2D, horizontalAlignment, HorizontalAlignment2D::Left, getHorizontalAlignment, setHorizontalAlignment),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(VerticalAlignment2D, verticalAlignment, VerticalAlignment2D::Top, getVerticalAlignment, setVerticalAlignment),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, reverseOrder, false, getReverseOrder, setReverseOrder),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, autoWidth, false, getAutoWidth, setAutoWidth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, autoHeight, false, getAutoHeight, setAutoHeight));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(Layout2D);

    //////////////////////////////////////////
    Layout2D::Layout2D()
        : m_dirty(true)
        , m_paddingLeft(0.0f)
        , m_paddingRight(0.0f)
        , m_paddingTop(0.0f)
        , m_paddingBottom(0.0f)
        , m_horizontalAlignment(HorizontalAlignment2D::Left)
        , m_verticalAlignment(VerticalAlignment2D::Top)
        , m_reverseOrder(false)
        , m_autoWidth(false)
        , m_autoHeight(false)
    {
    }

    //////////////////////////////////////////
    Layout2D::~Layout2D()
    {

    }

    //////////////////////////////////////////
    bool Layout2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void Layout2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

    }

    //////////////////////////////////////////
    void Layout2D::update()
    {
        bool alignmentDirty = m_dirty || m_transform->isSizeChanged();

        if (!alignmentDirty)
        {
            for (Transform2D* childTransform : m_transform->getChildren())
            {
                if (childTransform->isLocalTransformChanged() || childTransform->isEntityActiveChanged())
                {
                    alignmentDirty = true;
                    break;
                }
            }
        }

        if (alignmentDirty)
            alignChildren();
    }

    //////////////////////////////////////////
    void Layout2D::processChildren(std::function<void(Transform2D*)> _func)
    {
        if (m_reverseOrder)
        {
            auto it = m_transform->getChildren().rbegin();
            auto end = m_transform->getChildren().rend();
            for (; it != end; ++it)
            {
                if (!(*it)->getEntityRaw()->getActiveInHierarchy())
                    continue;

                _func(*it);
            }
        }
        else
        {
            auto it = m_transform->getChildren().begin();
            auto end = m_transform->getChildren().end();
            for (; it != end; ++it)
            {
                if (!(*it)->getEntityRaw()->getActiveInHierarchy())
                    continue;

                _func(*it);
            }
        }
    }

    //////////////////////////////////////////
    Size Layout2D::getActiveChildrenCount()
    {
        Size result = 0;

        auto it = m_transform->getChildren().begin();
        auto end = m_transform->getChildren().end();
        for (; it != end; ++it)
        {
            if (!(*it)->getEntityRaw()->getActiveInHierarchy())
                continue;

            ++result;
        }

        return result;
    }

    
} // namespace Maze
//////////////////////////////////////////
