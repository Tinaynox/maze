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
#include "maze-ui/ecs/components/MazeOverlayLayout2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class OverlayLayout2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(OverlayLayout2D, Layout2D,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, controlChildWidth, false, getControlChildWidth, setControlChildWidth),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, controlChildHeight, false, getControlChildHeight, setControlChildHeight));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(OverlayLayout2D);


    //////////////////////////////////////////
    OverlayLayout2D::OverlayLayout2D()
    {
    }

    //////////////////////////////////////////
    OverlayLayout2D::~OverlayLayout2D()
    {
    }

    //////////////////////////////////////////
    OverlayLayout2DPtr OverlayLayout2D::Create()
    {
        OverlayLayout2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(OverlayLayout2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool OverlayLayout2D::init()
    {
        return true;
    }

    //////////////////////////////////////////
    Vec2F OverlayLayout2D::calculateContentSize()
    {
        Vec2F contentSize = Vec2F::c_zero;

        processChildren(
            [&](Transform2D* _childTransform)
            {
                Entity* childEntity = _childTransform->getEntityRaw();

                Vec2F childSize;
                bool hasContent = EcsHelper::CalculatePreferredSize2D(childEntity, childSize);
                SizePolicy2D* sizePolicy = childEntity->getComponentRaw<SizePolicy2D>();

                for (Size axis = 0; axis < 2; ++axis)
                {
                    bool controlled = (axis == 0) ? m_controlChildWidth : m_controlChildHeight;
                    bool stretched = sizePolicy && sizePolicy->getFlag(axis == 0 ? SizePolicy2D::Width : SizePolicy2D::Height);

                    // Without own content such a child only follows the layout size
                    if ((controlled || stretched) && !hasContent)
                        continue;

                    F32 size = childSize[axis];
                    if (stretched)
                        size -= sizePolicy->getSizeDelta()[axis];

                    contentSize[axis] = Math::Max(contentSize[axis], size);
                }
            });

        return contentSize;
    }

    //////////////////////////////////////////
    Vec2F OverlayLayout2D::calculatePreferredSize()
    {
        Vec2F size = m_transform->getSize();
        if (!m_autoWidth && !m_autoHeight)
            return size;

        Vec2F contentSize = calculateContentSize();

        if (m_autoWidth)
            size.x = contentSize.x + m_paddingLeft + m_paddingRight;

        if (m_autoHeight)
            size.y = contentSize.y + m_paddingBottom + m_paddingTop;

        return size;
    }

    //////////////////////////////////////////
    void OverlayLayout2D::alignChildren()
    {
        m_transform->setSize(calculatePreferredSize());

        Vec2F const& layoutSize = m_transform->getSize();
        F32 paddedWidth = layoutSize.x - m_paddingLeft - m_paddingRight;
        F32 paddedHeight = layoutSize.y - m_paddingBottom - m_paddingTop;

        processChildren(
            [&](Transform2D* _childTransform)
            {
                Vec2F childSize = getChildLayoutSize(_childTransform);

                if (m_controlChildWidth)
                    childSize.x = paddedWidth;

                if (m_controlChildHeight)
                    childSize.y = paddedHeight;

                _childTransform->setSize(childSize);
                _childTransform->setPivot(0.0f, 0.0f);
                _childTransform->setAnchor(0.0f, 0.0f);

                Vec2F localPosition = Vec2F::c_zero;

                switch (m_horizontalAlignment)
                {
                    case HorizontalAlignment2D::Left: localPosition.x = 0.0f; break;
                    case HorizontalAlignment2D::Center: localPosition.x = (paddedWidth - childSize.x) * 0.5f; break;
                    case HorizontalAlignment2D::Right: localPosition.x = paddedWidth - childSize.x; break;
                    default: break;
                }

                switch (m_verticalAlignment)
                {
                    case VerticalAlignment2D::Bottom: localPosition.y = 0.0f; break;
                    case VerticalAlignment2D::Middle: localPosition.y = (paddedHeight - childSize.y) * 0.5f; break;
                    case VerticalAlignment2D::Top: localPosition.y = paddedHeight - childSize.y; break;
                    default: break;
                }

                _childTransform->setLocalPosition(Vec2F(m_paddingLeft, m_paddingBottom) + localPosition);
            });

        m_dirty = false;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(OverlayLayout2DPreferredSize2DQuery,
        {},
        {},
        PreferredSize2DQueryEvent& _event,
        Entity* _entity,
        OverlayLayout2D* _layout)
    {
        if (_layout->getTransform() && (_layout->getAutoWidth() || _layout->getAutoHeight()))
            _event.answer(_layout->calculatePreferredSize());
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(OverlayLayout2DActiveChanged,
        {},
        {},
        EntityActiveChangedEvent const& _event,
        Entity* _entity,
        OverlayLayout2D* _layout)
    {
        _layout->dirty();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(OverlayLayout2DChildRemoved,
        {},
        {},
        EcsChildRemovedEvent const& _event,
        Entity* _entity,
        OverlayLayout2D* _layout)
    {
        _layout->dirty();
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(OverlayLayout2DChildAdded,
        {},
        {},
        EcsChildAddedEvent const& _event,
        Entity* _entity,
        OverlayLayout2D* _layout)
    {
        _layout->dirty();
    }

} // namespace Maze
//////////////////////////////////////////
