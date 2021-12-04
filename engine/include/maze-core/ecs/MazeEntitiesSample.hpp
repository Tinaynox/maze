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
#if (!defined(_MazeEntitiesSample_hpp_))
#define _MazeEntitiesSample_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSTypes.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/MazeEntityAspect.hpp"
#include "maze-core/utils/MazeIndexTupleBuilder.hpp"
#include <functional>
#include <tuple>
#include <utility>
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(IEntitiesSample);
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(ECSWorld);


    //////////////////////////////////////////
    // Enum class EntitiesSampleType
    //
    //////////////////////////////////////////
    enum class EntitiesSampleType
    {
        Common,
        GenericInclusive,
    };


    //////////////////////////////////////////
    // Class IEntitiesSample
    //
    //////////////////////////////////////////
    class MAZE_CORE_API IEntitiesSample
        : public SharedObject<IEntitiesSample>
    {
    public:

        //////////////////////////////////////////
        virtual ~IEntitiesSample();

        //////////////////////////////////////////
        virtual EntitiesSampleType getType() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void clear() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processEntity(Entity* _entity) MAZE_ABSTRACT;


        //////////////////////////////////////////
        EntityAspect const& getAspect() const { return m_aspect; }


        //////////////////////////////////////////
        virtual void process(void (*)()) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt, void (*)()) MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        IEntitiesSample();

        //////////////////////////////////////////
        virtual bool init(
            ECSWorldPtr const& _ecs,
            EntityAspect const& _aspect);

    protected:
        ECSWorldWPtr m_world;
        ECSWorld* m_worldRaw;
        EntityAspect m_aspect;
    };


    //////////////////////////////////////////
    // Class EntitiesSample
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntitiesSample
        : public IEntitiesSample
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(EntitiesSample)

    public:

        //////////////////////////////////////////
        virtual ~EntitiesSample();

        //////////////////////////////////////////
        static EntitiesSamplePtr Create(
            ECSWorldPtr const& _ecs,
            EntityAspect const& _aspect)
        {
            EntitiesSamplePtr object;
            MAZE_CREATE_AND_INIT_SHARED_PTR(EntitiesSample, object, init(_ecs, _aspect));
            return object;
        }

        //////////////////////////////////////////
        virtual EntitiesSampleType getType() MAZE_OVERRIDE { return EntitiesSampleType::Common; }

        //////////////////////////////////////////
        Vector<Entity*> const& getEntities() const { return m_entities; }

        //////////////////////////////////////////
        Vector<EntityId> getEntityIds() const;

        //////////////////////////////////////////
        virtual void clear() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntity(Entity* _entity) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void process(void (*_func)()) MAZE_OVERRIDE
        { }

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt, void (*_func)()) MAZE_OVERRIDE
        { }

    public:
    
        //////////////////////////////////////////
        MultiDelegate<Entity*> eventEntityAdded;
        MultiDelegate<Entity*> eventEntityRemoved;

    protected:

        //////////////////////////////////////////
        EntitiesSample();

        //////////////////////////////////////////
        virtual bool init(
            ECSWorldPtr const& _ecs,
            EntityAspect const& _aspect) MAZE_OVERRIDE;

    protected:
        Vector<Entity*> m_entities;
    };


    //////////////////////////////////////////
    // Class GenericInclusiveEntitiesSample
    //
    //////////////////////////////////////////
    template<typename ...TComponents>
    class GenericInclusiveEntitiesSample
        : public IEntitiesSample
    {
    protected:

        //////////////////////////////////////////
        using Indices = IndexTupleBuilder<sizeof ...(TComponents)>;

        //////////////////////////////////////////
        using ProcessFunc = std::function<void(Entity*, TComponents* ..._components)>;

        //////////////////////////////////////////
        using ProcessFuncRaw = void(*)(Entity*, TComponents* ..._components);

        //////////////////////////////////////////
        using ProcessUpdateFunc = std::function<void(F32, Entity*, TComponents* ..._components)>;

        //////////////////////////////////////////
        using ProcessUpdateFuncRaw = void(*)(F32, Entity*, TComponents* ..._components);

        //////////////////////////////////////////
        struct EntityData
        {
            Entity* entity = nullptr;
            std::tuple<TComponents*...> components;
        };

    public:

        //////////////////////////////////////////
        virtual ~GenericInclusiveEntitiesSample()
        {

        }

        //////////////////////////////////////////
        static inline SharedPtr<GenericInclusiveEntitiesSample> Create(
            ECSWorldPtr const& _ecs)
        {
            SharedPtr<GenericInclusiveEntitiesSample> object;
            EntityAspect aspect = EntityAspect::HaveAllOfComponents<TComponents...>();

            object = MAZE_CREATE_SHARED_PTR(GenericInclusiveEntitiesSample);
            if (!object->init(_ecs, aspect))
                object.reset();
            return object;
        }

        //////////////////////////////////////////
        virtual EntitiesSampleType getType() MAZE_OVERRIDE { return EntitiesSampleType::GenericInclusive; }

        //////////////////////////////////////////
        Vector<EntityData> const& getEntitiesData() const { return m_entitiesData; }

        //////////////////////////////////////////
        Vector<EntityId> getEntityIds() const
        {
            Vector<EntityId> result;
            for (EntityData entityData : m_entitiesData)
                result.push_back(entityData.entity->getId());

            return result;
        }

        //////////////////////////////////////////
        virtual void clear() MAZE_OVERRIDE
        {
            for (EntityData const& entityData : m_entitiesData)
                invokeEntityRemoved(entityData, typename Indices::Indexes());

            m_entitiesData.clear();
        }

        //////////////////////////////////////////
        virtual void processEntity(Entity* _entity) MAZE_OVERRIDE
        {
            typename Vector<EntityData>::iterator it = std::find_if(
                m_entitiesData.begin(),
                m_entitiesData.end(),
                [_entity](EntityData const& _data) -> bool { return _data.entity == _entity; });

            bool intersects;
            if (!_entity->getActiveInHierarchy() || !_entity->getECSWorld() || _entity->getRemoving())
                intersects = false;
            else
                intersects = m_aspect.hasIntersection(_entity);
            
            if (it == m_entitiesData.end() && intersects)
            {
                EntityData entityData;
                entityData.entity = _entity;
                extractComponents(_entity, entityData.components);

                m_entitiesData.push_back(entityData);
                invokeEntityAdded(entityData, typename Indices::Indexes());
            }
            else
            if (it != m_entitiesData.end() && !intersects)
            {
                EntityData entityData = (*it);
                m_entitiesData.erase(it);
                invokeEntityRemoved(entityData, typename Indices::Indexes());
            }
        }

        //////////////////////////////////////////
        void process(ProcessFunc _func)
        {
            for (EntityData entityData : m_entitiesData)
            {
                callProcess(
                    _func,
                    entityData.entity,
                    entityData.components,
                    typename Indices::Indexes());
            }
        }

        //////////////////////////////////////////
        virtual void process(void (*_func)()) MAZE_OVERRIDE
        {
            ProcessFuncRaw rawFunc = (ProcessFuncRaw)_func;
            process((ProcessFunc)(rawFunc));
        }

        //////////////////////////////////////////
        void processUpdate(F32 _dt, ProcessUpdateFunc _func)
        {
            for (EntityData entityData : m_entitiesData)
            {
                callProcessUpdate(
                    _dt,
                    _func,
                    entityData.entity,
                    entityData.components,
                    typename Indices::Indexes());
            }
        }

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt, void (*_func)()) MAZE_OVERRIDE
        {
            ProcessUpdateFuncRaw rawFunc = (ProcessUpdateFuncRaw)_func;
            processUpdate(_dt, (ProcessUpdateFunc)(rawFunc));
        }

    protected:

        template<std::size_t TIndex = 0>
        inline
        typename std::enable_if<TIndex == sizeof...(TComponents), void>::type
        extractComponents(Entity* entity, std::tuple<TComponents*...>& _components)
        {}

        template<std::size_t TIndex = 0>
        inline
        typename std::enable_if<TIndex < sizeof...(TComponents), void>::type
        extractComponents(Entity* entity, std::tuple<TComponents*...>& _components)
        {
            using TComponentType = typename std::tuple_element<TIndex, std::tuple<TComponents...> >::type;
            std::get<TIndex>(_components) = entity->getComponentRaw<TComponentType>();

            this->extractComponents<TIndex + 1>(entity, _components);
        }

        template<S32 ...Idxs> 
        inline void callProcess(
            ProcessFunc _func,
            Entity* _entity,
            std::tuple<TComponents*...>& _components,
            IndexesTuple<Idxs...> const&)
        {
            _func(_entity, std::get<Idxs>(_components)...);
        }

        template<S32 ...Idxs>
        inline void callProcessUpdate(
            F32 _dt,
            ProcessUpdateFunc _func,
            Entity* _entity,
            std::tuple<TComponents*...>& _components,
            IndexesTuple<Idxs...> const&)
        {
            _func(_dt, _entity, std::get<Idxs>(_components)...);
        }


    public:
        MultiDelegate<Entity*, TComponents* ...> eventEntityAdded;
        MultiDelegate<Entity*, TComponents* ...> eventEntityRemoved;

    protected:

        //////////////////////////////////////////
        GenericInclusiveEntitiesSample()
        {

        }

        //////////////////////////////////////////
        virtual bool init(
            ECSWorldPtr const& _ecs,
            EntityAspect const& _aspect) MAZE_OVERRIDE
        {
            if (!IEntitiesSample::init(_ecs, _aspect))
                return false;

            return true;
        }

        //////////////////////////////////////////
        template<S32 ...Idxs> 
        inline void invokeEntityAdded(
            EntityData const& _entityData,
            IndexesTuple<Idxs...> const&)
        {
            eventEntityAdded(_entityData.entity, std::get<Idxs>(_entityData.components)...);
        }

        //////////////////////////////////////////
        template<S32 ...Idxs> 
        inline void invokeEntityRemoved(
            EntityData const& _entityData,
            IndexesTuple<Idxs...> const&)
        {
            eventEntityRemoved(_entityData.entity, std::get<Idxs>(_entityData.components)...);
        }

    protected:
        Vector<EntityData> m_entitiesData;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntitiesSample_hpp_
//////////////////////////////////////////
