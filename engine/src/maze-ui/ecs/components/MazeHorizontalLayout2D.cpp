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
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
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
    // Class HorizontalLayout2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(HorizontalLayout2D, Layout2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, spacing, 0.0f, getSpacing, setSpacing),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, controlChildWidth, false, getControlChildWidth, setControlChildWidth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, controlChildHeight, false, getControlChildHeight, setControlChildHeight),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, expand, false, getExpand, setExpand));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(HorizontalLayout2D);

    //////////////////////////////////////////
    HorizontalLayout2D::HorizontalLayout2D()
        : m_spacing(0.0f)
        , m_controlChildWidth(false)
        , m_controlChildHeight(false)
        , m_expand(false)
    {
        setAutoWidth(true);
    }

    //////////////////////////////////////////
    HorizontalLayout2D::~HorizontalLayout2D()
    {

    }

    //////////////////////////////////////////
    HorizontalLayout2DPtr HorizontalLayout2D::Create()
    {
        HorizontalLayout2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(HorizontalLayout2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool HorizontalLayout2D::init()
    {

        return true;
    }


    //////////////////////////////////////////
    void HorizontalLayout2D::alignChildren()
    {
        Size childrenCount = getActiveChildrenCount();

        if (childrenCount == 0)
        {
            Vec2F targetLayoutSize = m_transform->getSize();

            if (m_autoWidth)
                targetLayoutSize.x = m_paddingLeft + m_paddingRight;

            if (m_autoHeight)
                targetLayoutSize.y = m_paddingBottom + m_paddingTop;

            m_transform->setSize(targetLayoutSize);

            m_dirty = false;

            return;
        }

        if (m_autoWidth || m_autoHeight)
        {
            Vec2F targetLayoutSize = m_transform->getSize();

            F32 totalWidth = 0.0f;
            F32 maxHeight = 0.0f;
            processChildren(
                [&](Transform2D* _childTransform)
                {
                    Vec2F const& childSize = _childTransform->getSize();
                    totalWidth += childSize.x;
                    maxHeight = Math::Max(childSize.y, maxHeight);
                });

            totalWidth += m_spacing * (childrenCount - 1);

            if (m_autoWidth)
                targetLayoutSize.x = totalWidth + m_paddingLeft + m_paddingRight;

            if (m_autoHeight)
                targetLayoutSize.y = maxHeight + m_paddingBottom + m_paddingTop;

            m_transform->setSize(targetLayoutSize);
        }

        Vec2F const& layoutSize = m_transform->getSize();
        F32 paddedWidth = layoutSize.x - m_paddingLeft - m_paddingRight;
        F32 paddedHeight = layoutSize.y - m_paddingBottom - m_paddingTop;

        F32 x;
        F32 spacing = m_spacing;


        F32 totalChildrenWidth = 0.0f;
        processChildren(
            [&](Transform2D* _childTransform)
            {
                Vec2F targetChildSize = _childTransform->getSize();

                if (m_controlChildWidth)
                    targetChildSize.x = (paddedWidth - spacing * (childrenCount - 1)) / childrenCount;

                if (m_controlChildHeight)
                    targetChildSize.y = paddedHeight;

                _childTransform->setSize(targetChildSize);

                totalChildrenWidth += _childTransform->getWidth();
            });


        F32 activeWidth = 0.0f;
        if (m_expand)
        {
            F32 extraSpacing = Math::Max(0.0f, paddedWidth - totalChildrenWidth) / (childrenCount - 1);
            spacing += extraSpacing;
        }

        activeWidth = totalChildrenWidth + spacing * (childrenCount - 1);

        switch (m_horizontalAlignment)
        {
            case HorizontalAlignment2D::Left:
                x = 0.0f;
                break;
            case HorizontalAlignment2D::Center:
                x = paddedWidth * 0.5f - activeWidth * 0.5f;
                break;
            case HorizontalAlignment2D::Right:
                x = paddedWidth - activeWidth;
                break;

            default:
                MAZE_NOT_IMPLEMENTED_RETURN;
                break;
        }


        processChildren(
            [&](Transform2D* _childTransform)
            {
                _childTransform->setPivot(0.0f, 0.0f);
                _childTransform->setAnchor(0.0f, 0.0f);

                Vec2F localPosition;

                localPosition.x = x;
                x += _childTransform->getWidth() + spacing;

                switch (m_verticalAlignment)
                {
                    case VerticalAlignment2D::Bottom:
                    {
                        localPosition.y = 0.0f;
                        break;
                    }

                    case VerticalAlignment2D::Middle:
                    {
                        localPosition.y = paddedHeight * 0.5f - _childTransform->getHeight() * 0.5f;
                        break;
                    }

                    case VerticalAlignment2D::Top:
                    {
                        localPosition.y = paddedHeight - _childTransform->getHeight();
                        break;
                    }

                    default:
                    {
                        MAZE_WARNING("Not implemented");
                        break;
                    }
                }

                _childTransform->setLocalPosition(Vec2F(m_paddingLeft, m_paddingBottom) + localPosition);
            });

        m_dirty = false;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(HorizontalLayout2DActiveChanged,
        {},
        {},
        EntityActiveChangedEvent const& _event,
        Entity* _entity,
        HorizontalLayout2D* _horizontalLayout2D)
    {
        _horizontalLayout2D->dirty();
    }
    
} // namespace Maze
//////////////////////////////////////////
