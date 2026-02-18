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
#if (!defined(_MazeMonoSerializationHelper_hpp_))
#define _MazeMonoSerializationHelper_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MonoSerializationHelper
    {
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeMonoBehaviourToDataBlock(EntitiesToDataBlockContext& _context, ComponentPtr const& _component, DataBlock& _componentBlock);
        
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeMonoBehaviourModificationsToDataBlock(EntitiesToDataBlockContext& _context, ComponentPtr const& _component, ComponentPtr const& _identityComponent, DataBlock& _prefabBlock);
        
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void DeserializeMonoBehaviourFromDataBlock(DataBlock const& _componentBlock, ComponentPtr const& _component, EntitiesFromDataBlockContext& _context);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void DeserializeMonoBehaviourModificaionFromDataBlock(DataBlock const& _modificationBlock, ComponentPtr const& _component, CString _propertyName, EntitiesFromDataBlockContext& _context);

    } // namespace MonoSerializationHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoSerializationHelper_hpp_
//////////////////////////////////////////
