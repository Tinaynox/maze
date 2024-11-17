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
#include "InventorySlot.hpp"
#include "ecs/MazeEntity.hpp"
#include "ecs/components/MazeTransform2D.hpp"
#include "main/components/InventoryItem.hpp"
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-ui/events/MazeUIEvents.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class InventorySlot
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InventorySlot, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(InventorySlot);

    //////////////////////////////////////////
    InventorySlot::InventorySlot()
    {
    }

    //////////////////////////////////////////
    InventorySlot::~InventorySlot()
    {
    }

    //////////////////////////////////////////
    InventorySlotPtr InventorySlot::Create()
    {
        InventorySlotPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InventorySlot, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool InventorySlot::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void InventorySlot::setItem(InventoryItemPtr const& _item)
    {
        if (m_item == _item)
            return;

        m_item = _item;

        if (m_item)
        {
            Transform2DPtr const& transform2d = m_item->getEntityRaw()->getComponent<Transform2D>();

            transform2d->setParent(getEntityRaw()->getComponent<Transform2D>());

            m_item->setSlot(cast<InventorySlot>());
        }
    }

    //////////////////////////////////////////
    void InventorySlot::processEntityAwakened()
    {
        m_dragAndDropZone = getEntityRaw()->ensureComponent<DragAndDropZone>();
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(InventorySlotOnDragAndDropEvent,
        {},
        {},
        DragAndDropEvent const& _event,
        Entity* _entity,
        InventorySlot* _slot)
    {
        EntityPtr const& entity = _entity->getEcsWorld()->getEntity(_event.viewEid);
        if (entity)
        {
            InventoryItemPtr item = entity->getComponent<InventoryItem>();
            if (item)
                _slot->setItem(item);
        }
    }

} // namespace Maze
//////////////////////////////////////////
