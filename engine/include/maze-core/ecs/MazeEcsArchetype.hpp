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
#if (!defined(_MazeEcsArchetype_hpp_))
#define _MazeEcsArchetype_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/containers/MazeFastVector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EcsArchetype);
    class IEntitiesSample;


    //////////////////////////////////////////
    // Class EcsArchetype
    //
    // Shadow archetype - a unique combination of components existing in the world.
    // Every entity with an assigned archetype is listed in m_entities, and the
    // archetype lazily caches which samples can be affected by entities of this
    // combination, so entity changes only touch the relevant samples.
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsArchetype
    {
    public:

        //////////////////////////////////////////
        friend class EcsWorld;

        //////////////////////////////////////////
        struct SampleEntry
        {
            IEntitiesSample* sample = nullptr;
            U32 order = 0u;
        };

    public:

        //////////////////////////////////////////
        static EcsArchetypePtr Create(
            ArchetypeId _id,
            Vector<ComponentId> const& _sortedComponentIds,
            U64 _hash);

        //////////////////////////////////////////
        inline ArchetypeId getId() const { return m_id; }

        //////////////////////////////////////////
        inline Vector<ComponentId> const& getComponentIds() const { return m_componentIds; }

        //////////////////////////////////////////
        inline U64 getHash() const { return m_hash; }

        //////////////////////////////////////////
        inline S64 getComponentsMask() const { return m_componentsMask; }

        //////////////////////////////////////////
        inline FastVector<EntityId> const& getEntities() const { return m_entities; }


        //////////////////////////////////////////
        static U64 CalculateHash(Vector<ComponentId> const& _sortedComponentIds);

    protected:

        //////////////////////////////////////////
        EcsArchetype(
            ArchetypeId _id,
            Vector<ComponentId> const& _sortedComponentIds,
            U64 _hash);

    protected:
        ArchetypeId m_id = c_invalidArchetypeId;
        Vector<ComponentId> m_componentIds;
        U64 m_hash = 0u;
        S64 m_componentsMask = 0;

        FastVector<EntityId> m_entities;

        Vector<SampleEntry> m_samplesCache;
        U32 m_samplesCacheVersion = U32(-1);
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsArchetype_hpp_
//////////////////////////////////////////
