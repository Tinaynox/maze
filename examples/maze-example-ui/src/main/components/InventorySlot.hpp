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
#pragma once
#if (!defined(_InventorySlot_hpp_))
#define _InventorySlot_hpp_


//////////////////////////////////////////
#include "maze-core/ecs/MazeComponent.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(InventorySlot);
    MAZE_USING_SHARED_PTR(InventoryItem);
    MAZE_USING_SHARED_PTR(DragAndDropZone);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    

    //////////////////////////////////////////
    // Class InventorySlot
    //
    //////////////////////////////////////////
    class InventorySlot
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InventorySlot, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(InventorySlot);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~InventorySlot();

        //////////////////////////////////////////
        static InventorySlotPtr Create();


        //////////////////////////////////////////
        void setItem(InventoryItemPtr const& _item);

        //////////////////////////////////////////
        inline InventoryItemPtr const& getItem() const { return m_item; }


        //////////////////////////////////////////
        inline SpriteRenderer2DPtr const& getGlow() const { return m_glow; }

        //////////////////////////////////////////
        inline void setGlow(SpriteRenderer2DPtr const& _glow) { m_glow = _glow; }

    protected:

        //////////////////////////////////////////
        InventorySlot();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

    protected:
        InventoryItemPtr m_item;
        DragAndDropZonePtr m_dragAndDropZone;

        SpriteRenderer2DPtr m_glow;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _InventorySlot_hpp_
//////////////////////////////////////////
