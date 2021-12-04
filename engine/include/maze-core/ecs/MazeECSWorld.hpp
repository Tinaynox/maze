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
#if (!defined(_MazeECSWorld_hpp_))
#define _MazeECSWorld_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeECSTypes.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeEntityAspect.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/utils/MazeSwitchableContainer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(ECSWorld);
    MAZE_USING_SHARED_PTR(ComponentEntityLinker);
    MAZE_USING_SHARED_PTR(IEntitiesSample);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(ComponentSystem);


    //////////////////////////////////////////
    // Class ECSWorld
    //
    //////////////////////////////////////////
    class MAZE_CORE_API ECSWorld
        : public SharedObject<ECSWorld>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(ECSWorld);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(ECSWorld);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~ECSWorld();

        //////////////////////////////////////////
        static ECSWorldPtr Create(EntityId _entitiesIdCounter = 0);


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        EntityPtr createEntity();

        //////////////////////////////////////////
        bool addEntity(EntityPtr const& _entity);

        //////////////////////////////////////////
        bool removeEntity(EntityPtr const& _entity);

        //////////////////////////////////////////
        EntityPtr const& getEntityById(EntityId _id) const;

        //////////////////////////////////////////
        Size getEntitiesCount();


        //////////////////////////////////////////
        void processEntityComponentsChanged(Entity* _entity);



        //////////////////////////////////////////
        template <typename TSystem, typename ...TArgs>
        SharedPtr<TSystem> createAndAddSystem(TArgs... _args)
        {
            SharedPtr<TSystem> system = TSystem::Create(this->getSharedPtr(), _args...);
            addSystem(system);
            return system;
        }

        //////////////////////////////////////////
        template <typename TSystem>
        SharedPtr<TSystem> getSystem()
        {
            ClassUID uid = ClassInfo<TSystem>::UID();
            for (Size i = 0, in = m_systems.size(); i < in; ++i)
            {
                if (m_systems[i]->getClassUID() == uid)
                {
                    return m_systems[i]->cast<TSystem>();
                }
            }

            return SharedPtr<TSystem>();
        }

        //////////////////////////////////////////
        template <typename TSystem>
        void removeSystem()
        {
            ClassUID uid = ClassInfo<TSystem>::UID();
            for (Size i = 0, in = m_systems.size(); i < in; ++i)
            {
                if (m_systems[i]->getClassUID() == uid)
                {
                    m_systems.erase(m_systems.begin() + i);
                    return;
                }
            }
        }

        //////////////////////////////////////////
        void addSystem(ComponentSystemPtr const& _system);

        //////////////////////////////////////////
        void removeSystem(ComponentSystemPtr const& _system);


        //////////////////////////////////////////
        EntitiesSamplePtr requestCommonSample(EntityAspect const& _aspect);

        //////////////////////////////////////////
        template<typename ...TComponents>
        SharedPtr<GenericInclusiveEntitiesSample<TComponents...>> requestInclusiveSample()
        {
            for (Size i = 0, in = m_samples.size(); i < in; ++i)
            {
                if (m_samples[i]->getType() != EntitiesSampleType::GenericInclusive)
                    continue;

                if (m_samples[i]->getAspect().isAspect<TComponents...>(EntityAspectType::HaveAllOfComponents))
                    return m_samples[i]->cast<GenericInclusiveEntitiesSample<TComponents...>>();
            }

            SharedPtr<GenericInclusiveEntitiesSample<TComponents...>> sample = GenericInclusiveEntitiesSample<TComponents...>::Create(getSharedPtr());
            m_samples.push_back(sample);

            for (Size i = 0, in = m_activeEntities.size(); i < in; ++i)
                sample->processEntity(m_activeEntities[i]);

            for (Size i = 0, in = m_addingEntities.size(); i < in; ++i)
                sample->processEntity(m_addingEntities[i]);

            return sample;
        }

        //////////////////////////////////////////
        template<typename ...TComponents>
        SimpleComponentSystemPtr addSystem(
            void (*_func)(F32, Entity*, TComponents* ...),
            S32 _order = 0)
        {
            SimpleComponentSystemPtr system = SimpleComponentSystem::Create(
                requestInclusiveSample<TComponents...>(),
                (SimpleComponentSystem::Func)_func,
                _order);
            addSystem(system);
            return system;
        }

    public:

        //////////////////////////////////////////
        MultiDelegate<EntityPtr const&> eventEntityAdded;
        MultiDelegate<Entity*> eventEntityChanged;
        MultiDelegate<EntityPtr const&> eventEntityRemoved;
        MultiDelegate<ComponentSystemPtr const&> eventComponentSystemAdded;

    protected:

        //////////////////////////////////////////
        ECSWorld();

        //////////////////////////////////////////
        bool init(EntityId _entitiesIdCounter);

        //////////////////////////////////////////
        void processStartedEntities();

        //////////////////////////////////////////
        void processAddingEntities();

        //////////////////////////////////////////
        void processRemovingEntities();

        //////////////////////////////////////////
        void processChangedEntities();

        
        //////////////////////////////////////////
        void processEntityActiveChanged(Entity* _entity);

    protected:
        UnorderedMap<EntityId, EntityPtr> m_entitiesMap;

        Deque<Entity*> m_addingEntities;
        Deque<Entity*> m_removingEntities;
        Deque<Entity*> m_componentsChangedEntities;
        Deque<Entity*> m_activeChangedEntities;

        FastVector<Entity*> m_activeEntities;

        SwitchableContainer<FastVector<Entity*>> m_startedEntities;

        EntityId m_entitiesIdCounter;

        Vector<IEntitiesSamplePtr> m_samples;
        Vector<ComponentSystemPtr> m_systems;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeECSWorld_hpp_
//////////////////////////////////////////
