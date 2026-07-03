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
#include "maze-core/ecs/MazeEcsArchetype.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EcsArchetype
    //
    //////////////////////////////////////////
    EcsArchetype::EcsArchetype(
        ArchetypeId _id,
        Vector<ComponentId> const& _sortedComponentIds,
        U64 _hash)
        : m_id(_id)
        , m_componentIds(_sortedComponentIds)
        , m_hash(_hash)
    {
        for (ComponentId componentId : m_componentIds)
            m_componentsMask |= (S64)1 << U32(componentId % 64);
    }

    //////////////////////////////////////////
    EcsArchetypePtr EcsArchetype::Create(
        ArchetypeId _id,
        Vector<ComponentId> const& _sortedComponentIds,
        U64 _hash)
    {
        return EcsArchetypePtr(new EcsArchetype(_id, _sortedComponentIds, _hash));
    }

    //////////////////////////////////////////
    U64 EcsArchetype::CalculateHash(Vector<ComponentId> const& _sortedComponentIds)
    {
        // FNV-1a
        U64 hash = 14695981039346656037ULL;
        for (ComponentId componentId : _sortedComponentIds)
        {
            hash ^= (U64)componentId;
            hash *= 1099511628211ULL;
        }
        return hash;
    }


} // namespace Maze
//////////////////////////////////////////
