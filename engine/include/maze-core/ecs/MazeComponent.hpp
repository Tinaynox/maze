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
#if (!defined(_Maze_hpp_))
#define _Maze_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/managers/MazeEventManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Component);
    MAZE_USING_SHARED_PTR(Entity);
    MAZE_USING_SHARED_PTR(EcsWorld);
    MAZE_USING_SHARED_PTR(ComponentEntityLinker);


    //////////////////////////////////////////
    // Struct EntityCopyData
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityCopyData
    {
    public:

        //////////////////////////////////////////
        struct MAZE_CORE_API ComponentPropertyData
        {
            MetaProperty* metaProperty;
            MetaInstance metaInstance;
            MetaInstance objMetaInstance;
        };

        //////////////////////////////////////////
        struct MAZE_CORE_API EntityPropertyData
        {
            MetaProperty* metaProperty;
            MetaInstance metaInstance;
            MetaInstance objMetaInstance;
        };

    public:

        //////////////////////////////////////////
        inline EntityCopyData()
            : m_stackDepth(0)
        {
            // #TODO: OPTIMIZE
            m_entities.reset(new Map<Entity*, Entity*>());
            m_components.reset(new Map<Component*, Component*>());
            m_componentProperties.reset(new Vector<ComponentPropertyData>());
            m_entityProperties.reset(new Vector<EntityPropertyData>());
        }

        //////////////////////////////////////////
        inline Map<Entity*, Entity*>& getEntities()
        {
            return *m_entities.get();
        }

        //////////////////////////////////////////
        inline Map<Component*, Component*>& getComponents()
        {
            return *m_components.get();
        }

        //////////////////////////////////////////
        inline Vector<ComponentPropertyData>& getComponentProperties()
        {
            return *m_componentProperties.get();
        }

        //////////////////////////////////////////
        inline Vector<EntityPropertyData>& getEntityProperties()
        {
            return *m_entityProperties.get();
        }

        //////////////////////////////////////////
        inline S32 getStackDepth() const { return m_stackDepth; }

        //////////////////////////////////////////
        inline void pushStackDepth() { ++m_stackDepth; }

        //////////////////////////////////////////
        inline void popStackDepth() { --m_stackDepth; }

    private:
        SharedPtr<Map<Entity*, Entity*>> m_entities;
        SharedPtr<Map<Component*, Component*>> m_components;
        SharedPtr<Vector<ComponentPropertyData>> m_componentProperties;
        SharedPtr<Vector<EntityPropertyData>> m_entityProperties;
        S32 m_stackDepth;
    };


    //////////////////////////////////////////
    // Class Component
    //
    //////////////////////////////////////////
    class MAZE_CORE_API Component
        : public SharedObject<Component>
        , public IEventReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(Component);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~Component();


        //////////////////////////////////////////
        ComponentPtr Create(
            Component* _component,
            EcsWorld* _world = nullptr,
            EntityCopyData _copyData = EntityCopyData());

        //////////////////////////////////////////
        inline ComponentPtr createCopy(
            EcsWorld* _world = nullptr,
            EntityCopyData _copyData = EntityCopyData())
        {
            return Create(this, _world, _copyData);
        }


        //////////////////////////////////////////
        inline Entity* getEntityRaw() const { return m_entityRaw; }

        //////////////////////////////////////////
        EntityPtr getEntity() const;

        //////////////////////////////////////////
        EntityId getEntityId() const;

        //////////////////////////////////////////
        bool removeFromEntity();

        //////////////////////////////////////////
        S32 getPriority() const;

        //////////////////////////////////////////
        inline S64 getMaskFlag() const { return (S64)1 << U32(getClassUID() % 64); }


        //////////////////////////////////////////
        template <typename UComponent>
        inline SharedPtr<UComponent> safeCast()
        {
            MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
                !getMetaClass()->isInheritedFrom<UComponent>(),
                SharedPtr<UComponent>(),
                "Incompatible cast '%s' to '%s'!",
                getClassName(),
                ClassInfo<UComponent>::Name());

            return cast<UComponent>();
        }

        //////////////////////////////////////////
        template <typename UComponent>
        inline UComponent* safeCastRaw()
        {
            MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
                !getMetaClass()->isInheritedFrom<UComponent>(),
                nullptr,
                "Incompatible cast '%s' to '%s'!",
                getClassName(),
                ClassInfo<UComponent>::Name());

            return static_cast<UComponent*>(this);
        }

        //////////////////////////////////////////
        virtual bool isMetaPropertyCopyable(MetaProperty* _metaProperty) { return true; }

    protected:

        //////////////////////////////////////////
        Component();

        //////////////////////////////////////////
        Component(Component const&) = delete;

        //////////////////////////////////////////
        Component(Component&&) = delete;

        //////////////////////////////////////////
        Component& operator=(Component const&) = delete;

        //////////////////////////////////////////
        Component& operator=(Component&&) = delete;


        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EcsWorld* _world,
            EntityCopyData _copyData = EntityCopyData());

        //////////////////////////////////////////
        void setEntity(Entity* _entity);


        //////////////////////////////////////////
        virtual void processComponentAdded() {}

        //////////////////////////////////////////
        virtual void processComponentRemoved() {}


        //////////////////////////////////////////
        virtual void processSceneSet() {}

        //////////////////////////////////////////
        virtual void processSceneReset() {}

        //////////////////////////////////////////
        virtual void processEntityEnabled() {}

        //////////////////////////////////////////
        virtual void processEntityDisabled() {}
        
        //////////////////////////////////////////
        virtual void processEntityAwakened() {}


    protected:
        Entity* m_entityRaw = nullptr;
    };

    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_COMPONENT_SERIALIZATION(DComponent) \
        inline void SerializeValue(DComponent const& _value, U8* _data) \
        { \
            memcpy(_data, (U8 const*)(&_value), sizeof(DComponent)); \
        } \
        inline void DeserializeValue(DComponent& _value, U8 const* _data) \
        { \
            memcpy((U8*)&_value, _data, sizeof(DComponent)); \
        }


    //////////////////////////////////////////
    MAZE_IMPLEMENT_COMPONENT_SERIALIZATION(Component);


    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_COMPONENT_PROPERTY_SETTER_GETTER(DComponentClass, DMemberName, DMethodName) \
        inline void set##DMethodName(DComponentClass##Ptr const& _value) { m_##DMemberName = _value; } \
        inline DComponentClass##Ptr const& get##DMethodName() const { return m_##DMemberName; } \
        inline void set##DMethodName(ComponentPtr _value) \
        { \
            if (_value) \
            { \
                MAZE_DEBUG_BP_RETURN_IF(_value->getClassUID() != ClassInfo<DComponentClass>::UID()); \
                set##DMethodName(_value->cast<DComponentClass>()); \
            } \
            else \
            { \
                set##DMethodName(DComponentClass##Ptr()); \
            } \
        }\
        inline ComponentPtr get##DMethodName##Component() const { return m_##DMemberName; }

    //////////////////////////////////////////
    #define MAZE_IMPLEMENT_METACLASS_COMPONENT_PROPERTY(DMemberName, DMethodName) \
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, DMemberName, ComponentPtr(), get##DMethodName##Component, set##DMethodName)

} // namespace Maze
//////////////////////////////////////////


#endif // _Maze_hpp_
//////////////////////////////////////////
