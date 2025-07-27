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
#if (!defined(_MazeEcsTypes_hpp_))
#define _MazeEcsTypes_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/utils/MazeIdWithGeneration.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using ComponentId = ClassUID;
    using EntityId = IdWithGeneration<S32, 8>;
    using EntityIds = Set<EntityId>;

    using EcsWorldId = IdWithGeneration<S8, 4>;

    //////////////////////////////////////////
    using EcsSerializationId = S32;

    //////////////////////////////////////////
    static const EntityId c_invalidEntityId = EntityId();
    static const ComponentId c_invalidComponentId = 0;
    static const EcsWorldId c_invalidWorldId = EcsWorldId(-1);
    static const EcsSerializationId c_invalidSerializationId = 0;
    

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
namespace std
{
    //////////////////////////////////////////
    template<>
    struct hash<Maze::EntityId>
    {
        inline size_t operator()(Maze::EntityId const& _id) const noexcept
        {
            return static_cast<size_t>(_id.getId());
        }
    };


} // namespace std
//////////////////////////////////////////


#endif // _MazeEcsTypes_hpp_
//////////////////////////////////////////
