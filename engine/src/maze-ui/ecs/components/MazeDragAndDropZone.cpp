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
#include "maze-ui/ecs/components/MazeDragAndDropZone.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeBounds2D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/events/MazeUIEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DragAndDropZone
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(DragAndDropZone, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(DragAndDropZone);

    //////////////////////////////////////////
    DragAndDropZone::DragAndDropZone()
    {
    }

    //////////////////////////////////////////
    DragAndDropZone::~DragAndDropZone()
    {

    }

    //////////////////////////////////////////
    DragAndDropZonePtr DragAndDropZone::Create()
    {
        DragAndDropZonePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(DragAndDropZone, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool DragAndDropZone::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void DragAndDropZone::processEntityAwakened()
    {
        m_element = getEntityRaw()->ensureComponent<UIElement2D>();
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(DragAndDropZoneOnDragAndDropValidateEvent,
        {},
        {},
        DragAndDropValidateEvent& _event,
        Entity* _entity,
        DragAndDropZone* _zone)
    {
        _zone->eventDragAndDropValidate(_event.data, _event.viewEid, _event.dropAllowed);
    }
    

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(DragAndDropZoneOnDragAndDropEvent,
        {},
        {},
        DragAndDropEvent const& _event,
        Entity* _entity,
        DragAndDropZone* _zone)
    {
        _zone->eventDragAndDrop(_event.data, _event.viewEid);
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(DragAndDropZoneOnDragAndDropCurrentZoneChangedEvent,
        {},
        {},
        DragAndDropCurrentZoneChangedEvent const& _event,
        Entity* _entity,
        DragAndDropZone* _zone)
    {
        _zone->eventDragAndDropZoneOnDragAndDropCurrentZoneChanged(
            _entity->getId() == _event.zoneEid);
    }


} // namespace Maze
//////////////////////////////////////////
