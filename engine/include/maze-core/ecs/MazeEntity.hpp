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
#if (!defined(_MazeEntity_hpp_))
#define _MazeEntity_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/containers/MazeFastVector.hpp"
#include "maze-core/managers/MazeEventManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(EcsScene);
    MAZE_USING_SHARED_PTR(ComponentEntityLinker);


    //////////////////////////////////////////
    // Class Entity
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Entity
        : public SharedObject<Entity>
        , public IEventReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(Entity);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Entity);

        //////////////////////////////////////////
        using ComponentsContainer = Map<ComponentId, ComponentPtr>;

        //////////////////////////////////////////
        friend class EcsWorld;
        friend class EcsWorldEventsQueue;
        friend class Component;

        //////////////////////////////////////////
        enum class Flags
        {
            None = 0,
            
            ActiveSelf = MAZE_BIT(0),
            DisabledByHierarchy = MAZE_BIT(1),
            ActiveInHierarchyPrevFrame = MAZE_BIT(2),
            ComponentsMaskDirty = MAZE_BIT(3),
        };

        //////////////////////////////////////////
        enum class TransitionFlags
        {
            None = 0,
            
            Adding = MAZE_BIT(0),
            Removing = MAZE_BIT(1),
            ComponentsChanged = MAZE_BIT(2),
            ActiveChanged = MAZE_BIT(3),
            AwakeForbidden = MAZE_BIT(4),
            Awakened = MAZE_BIT(5),
        };

    public:

        //////////////////////////////////////////
        virtual ~Entity();

        //////////////////////////////////////////
        static EntityPtr Create();

        //////////////////////////////////////////
        static EntityPtr Create(
            Entity* _entity,
            EcsWorld* _world = nullptr,
            EntityCopyData _copyData = EntityCopyData());

        //////////////////////////////////////////
        inline static EntityPtr Create(
            EntityPtr const& _entity,
            EcsWorld* _world = nullptr,
            EntityCopyData _copyData = EntityCopyData())
        {
            return Create(_entity.get(), _world, _copyData);
        }

        //////////////////////////////////////////
        static EntityPtr const& GetEntity(EntityId _eid);

        //////////////////////////////////////////
        inline EntityPtr createCopy(
            EcsWorld* _world = nullptr,
            EntityCopyData _copyData = EntityCopyData())
        {
            return Create(this, _world, _copyData);
        }

        //////////////////////////////////////////
        virtual void processEvent(Event* _event) MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline EntityId getId() const { return m_id; }
        
       
        //////////////////////////////////////////
        inline EcsWorld* getEcsWorld() const { return m_world; }

        //////////////////////////////////////////
        void setEcsScene(EcsScene* _scene);

        //////////////////////////////////////////
        inline EcsScene* getEcsScene() { return m_scene; }

        //////////////////////////////////////////
        void removeFromEcsWorld();


        //////////////////////////////////////////
        inline ComponentsContainer const& getComponents() const { return m_components; }

        //////////////////////////////////////////
        inline Size getComponentsCount() const { return m_components.size(); }

        //////////////////////////////////////////
        inline ComponentPtr const& getComponentById(ComponentId _id) const
        {
            ComponentsContainer::const_iterator it = m_components.find(_id);
            
            static ComponentPtr const nullPointer;
            if (it == m_components.end())
            {
                return nullPointer;
            }
            
            return it->second;
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline SharedPtr<TComponent> getComponent() const
        {
            auto& component = getComponentById(GetStaticComponentId<TComponent>());
                
            if (!component)
                return nullptr;

            return component->template cast<TComponent>();
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline TComponent* getComponentRaw() const
        {
            auto& component = getComponentById(GetStaticComponentId<TComponent>());

            if (!component)
                return nullptr;

            return component->template castRaw<TComponent>();
        }


        //////////////////////////////////////////
        /*
        template <typename TComponent>
        inline bool findMultiComponent(std::function<bool(TComponent*)> _pred) const
        {
            auto range = m_components.equal_range(GetStaticComponentId<TComponent>());

            bool found = false;
            for (auto it = range.first; it != range.second; ++it)
            {
                if (_pred(it->second->template castRaw<TComponent>()))
                {
                    found = true;
                    break;
                }
            }

            return found;
        }
        */


        //////////////////////////////////////////
        template <typename TComponent>
        inline SharedPtr<TComponent> getComponentInheritedFrom() const
        {
            for (auto const& componentData : m_components)
                if (componentData.second->getMetaClass()->template isInheritedFrom<TComponent>())
                    return componentData.second->cast<TComponent>();

            return SharedPtr<TComponent>();
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline TComponent* getComponentRawInheritedFrom() const
        {
            for (auto const& componentData : m_components)
                if (componentData.second->getMetaClass()->template isInheritedFrom<TComponent>())
                    return componentData.second->castRaw<TComponent>();

            return nullptr;
        }

        //////////////////////////////////////////
        template <typename TComponent, typename ...TArgs>
        inline SharedPtr<TComponent> ensureComponent(TArgs... _args)
        {
            ComponentPtr const& component = getComponentById(GetStaticComponentId<TComponent>());
            if (component)
                return component->template cast<TComponent>();

            return createComponent<TComponent, TArgs...>(_args...);
        }

        
        //////////////////////////////////////////
        ComponentPtr const& addComponent(ComponentPtr const& component);

        //////////////////////////////////////////
        bool removeComponent(ComponentId _componentUID);

        //////////////////////////////////////////
        inline bool removeComponent(ComponentPtr const& _component)
        {
            return removeComponent(_component->getClassUID());
        }

        //////////////////////////////////////////
        template <typename TComponent>
        inline bool removeComponent()
        {
            return removeComponent(ClassInfo<TComponent>::UID());
        }

        //////////////////////////////////////////
        template <typename TComponent, typename ...TArgs>
        SharedPtr<TComponent> createComponent(TArgs... _args)
        {            
            return addComponent(TComponent::Create(_args...))->template cast<TComponent>();
        }

        
        //////////////////////////////////////////
        inline bool getTransitionFlag(TransitionFlags flag) const { return (m_transitionFlags & static_cast<U8>(flag)) != 0; }

        //////////////////////////////////////////
        bool getRemoving() const { return getTransitionFlag(TransitionFlags::Removing); }
        
        //////////////////////////////////////////
        bool getAdding() const { return getTransitionFlag(TransitionFlags::Adding); }

        //////////////////////////////////////////
        bool getComponentsChanged() const { return getTransitionFlag(TransitionFlags::ComponentsChanged); }

        //////////////////////////////////////////
        bool getActiveChanged() const { return getTransitionFlag(TransitionFlags::ActiveChanged); }

        //////////////////////////////////////////
        void setAwakeForbidden(bool _value) { setTransitionFlag(TransitionFlags::AwakeForbidden, _value); }

        //////////////////////////////////////////
        bool getAwakeForbidden() const { return getTransitionFlag(TransitionFlags::AwakeForbidden); }


        //////////////////////////////////////////
        inline U8 getFlags() const { return m_flags; }
        
        //////////////////////////////////////////
        void setFlags(U8 flags);
        
        //////////////////////////////////////////
        inline void setFlag(Flags flag, bool value)
        {
            if (value)
                setFlags(m_flags | static_cast<U8>(flag));
            else
                setFlags(m_flags & ~static_cast<U8>(flag));
        }

        //////////////////////////////////////////
        inline bool getFlag(Flags flag) const { return (m_flags & static_cast<U8>(flag)) != 0; }

        //////////////////////////////////////////
        inline void setActiveSelf(bool _active) { setFlag(Flags::ActiveSelf, _active); }
        
        //////////////////////////////////////////
        inline bool getActiveSelf() const { return getFlag(Flags::ActiveSelf); }

        //////////////////////////////////////////
        inline bool getActiveInHierarchy() const { return getFlag(Flags::ActiveSelf) && !getFlag(Flags::DisabledByHierarchy); }


        //////////////////////////////////////////
        inline void setDisabledByHierarchy(bool _value) { setFlag(Flags::DisabledByHierarchy, _value); }

        //////////////////////////////////////////
        inline bool getDisabledByHierarchy() const { return getFlag(Flags::DisabledByHierarchy); }


        //////////////////////////////////////////
        Set<ComponentId> const& getComponentIds() const { return m_componentIds; }

        //////////////////////////////////////////
        inline S64 getComponentsMask()
        {
            if (m_flags & (U8)Flags::ComponentsMaskDirty)
            {
                m_componentsMask = 0;
                for (ComponentId componentId : m_componentIds)
                {
                    m_componentsMask |= (S64)1 << U32(componentId % 64);
                }

                m_flags &= ~(U8)Flags::ComponentsMaskDirty;
            }

            return m_componentsMask;
        }
        
        //////////////////////////////////////////
        void tryAwake();

    protected:

        //////////////////////////////////////////
        Entity();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        bool init(
            Entity* _entity,
            EcsWorld* _world,
            EntityCopyData _copyData = EntityCopyData());

        
        //////////////////////////////////////////
        inline void setId(EntityId _id) { m_id = _id; }


        //////////////////////////////////////////
        inline void setTransitionFlag(TransitionFlags _flag, bool _value)
        {
            if (_value)
                m_transitionFlags |= static_cast<U8>(_flag);
            else
                m_transitionFlags &= ~static_cast<U8>(_flag);
        }
                
        //////////////////////////////////////////
        void setRemoving(bool _value) { setTransitionFlag(TransitionFlags::Removing, _value); }
        
        //////////////////////////////////////////
        void setAdding(bool _value) { setTransitionFlag(TransitionFlags::Adding, _value); }
        


        //////////////////////////////////////////
        void setComponentsChanged(bool _value) { setTransitionFlag(TransitionFlags::ComponentsChanged, _value); }

        //////////////////////////////////////////
        void setActiveChanged(bool _value) { setTransitionFlag(TransitionFlags::ActiveChanged, _value); }
        
        
        //////////////////////////////////////////
        void setEcsWorld(EcsWorld* _world);


        //////////////////////////////////////////
        inline void setActiveInHierarchyPrevFrame(bool _active) { setFlag(Flags::ActiveInHierarchyPrevFrame, _active); }

        //////////////////////////////////////////
        inline bool getActiveInHierarchyPrevFrame() const { return getFlag(Flags::ActiveInHierarchyPrevFrame); }

    protected:
        EcsWorld* m_world = nullptr;
        EcsScene* m_scene = nullptr;
        EntityId m_id;
        ComponentsContainer m_components;
        Set<ComponentId> m_componentIds;
        U8 m_flags = (U8(Flags::ActiveSelf) | U8(Flags::ActiveInHierarchyPrevFrame) | U8(Flags::ComponentsMaskDirty));

        S64 m_componentsMask = 0;

    protected:
        U8 m_transitionFlags = 0;
    };


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<EntityPtr>::value), void>::type
        ValueToString(EntityPtr const& _value, String& _data)
    {
        _data = _value ? StringHelper::S32ToString(_value->getId()) : StringHelper::S32ToString(c_invalidEntityId);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<EntityPtr>::value), void>::type
        ValueFromString(EntityPtr& _value, CString _data, Size _count)
    {
        EntityId eid = EntityId(StringHelper::StringToS32(_data));
        _value = Entity::GetEntity(eid);
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEntity_hpp_
//////////////////////////////////////////
