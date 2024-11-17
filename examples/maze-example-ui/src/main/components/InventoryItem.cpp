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
#include "InventoryItem.hpp"
#include "main/components/InventorySlot.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class InventoryItem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InventoryItem, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(InventoryItem);

    //////////////////////////////////////////
    InventoryItem::InventoryItem()
    {
    }

    //////////////////////////////////////////
    InventoryItem::~InventoryItem()
    {
    }

    //////////////////////////////////////////
    InventoryItemPtr InventoryItem::Create()
    {
        InventoryItemPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InventoryItem, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool InventoryItem::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void InventoryItem::setSlot(InventorySlotPtr const& _slot)
    {
        InventorySlotPtr prevSlot = getSlot();

        if (prevSlot == _slot)
            return;

        if (prevSlot && prevSlot->getItem().get() == this)
            prevSlot->setItem(nullptr);

        m_slot = _slot;

        if (_slot)
            _slot->setItem(cast<InventoryItem>());
    }

} // namespace Maze
//////////////////////////////////////////
