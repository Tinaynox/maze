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
#if (!defined(_MazeEcsCoreEvents_hpp_))
#define _MazeEcsCoreEvents_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeRotation2D.hpp"
#include "maze-core/ecs/MazeEcsScene.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct EcsEventParams
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API EcsEventParams
    {
        //////////////////////////////////////////
        EcsEventParams(
            bool _ignoreRemovingEntity = true,
            bool _ignoreNullWorldEntity = true)
            : ignoreRemovingEntity(_ignoreRemovingEntity)
            , ignoreNullWorldEntity(_ignoreNullWorldEntity)
        {

        }

        bool ignoreRemovingEntity = true;
        bool ignoreNullWorldEntity = true;
    };


    //////////////////////////////////////////
    // Class PreUpdateEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PreUpdateEvent
        : public GenericEvent<PreUpdateEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PreUpdateEvent, Event);

    public:

        //////////////////////////////////////////
        inline PreUpdateEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    // Class UpdateEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API UpdateEvent
        : public GenericEvent<UpdateEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(UpdateEvent, Event);

    public:

        //////////////////////////////////////////
        inline UpdateEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    // Class PostUpdateEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PostUpdateEvent
        : public GenericEvent<PostUpdateEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PostUpdateEvent, Event);

    public:

        //////////////////////////////////////////
        inline PostUpdateEvent(F32 _dt = 0.0f)
            : m_dt(_dt)
        {}

        //////////////////////////////////////////
        inline F32 getDt() const { return m_dt; }

    private:
        F32 m_dt = 0.0f;
    };


    //////////////////////////////////////////
    // Class EntityAddedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityAddedEvent
        : public GenericEvent<EntityAddedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EntityAddedEvent, Event);
    };


    //////////////////////////////////////////
    // Class EntityRemovedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityRemovedEvent
        : public GenericEvent<EntityRemovedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EntityRemovedEvent, Event);
    };


    //////////////////////////////////////////
    // Class EntityAddedToSampleEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityAddedToSampleEvent
        : public GenericEvent<EntityAddedToSampleEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EntityAddedToSampleEvent, Event);
    };


    //////////////////////////////////////////
    // Class EntityRemovedFromSampleEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityRemovedFromSampleEvent
        : public GenericEvent<EntityRemovedFromSampleEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EntityRemovedFromSampleEvent, Event);
    };


    //////////////////////////////////////////
    // Class EntityActiveChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EntityActiveChangedEvent
        : public GenericEvent<EntityActiveChangedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EntityActiveChangedEvent, Event);

    public:

        //////////////////////////////////////////
        inline EntityActiveChangedEvent(bool _value = false)
            : m_activeInHierarchy(_value)
        {}

        //////////////////////////////////////////
        inline bool getActiveInHierarchy() const { return m_activeInHierarchy; }

    private:
        bool m_activeInHierarchy = false;
    };


    //////////////////////////////////////////
    // Class EcsEntityActiveChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsEntityActiveChangedEvent
        : public GenericEvent<EcsEntityActiveChangedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EcsEntityActiveChangedEvent, Event);

    public:

        //////////////////////////////////////////
        inline EcsEntityActiveChangedEvent(
            EcsWorld* _world = nullptr,
            EntityId _entityId = c_invalidEntityId,
            bool _value = false)
            : m_world(_world)
            , m_entityId(_entityId)
            , m_activeInHierarchy(_value)
        {}

        //////////////////////////////////////////
        inline EcsWorld* getWorld() const { return m_world; }

        //////////////////////////////////////////
        inline EntityId getEntityId() const { return m_entityId; }

        //////////////////////////////////////////
        inline bool getActiveInHierarchy() const { return m_activeInHierarchy; }

    private:
        EcsWorld* m_world = nullptr;
        EntityId m_entityId = c_invalidEntityId;
        bool m_activeInHierarchy = false;
    };


    //////////////////////////////////////////
    // Class EcsSceneStateChangedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsSceneStateChangedEvent
        : public GenericEvent<EcsSceneStateChangedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EcsSceneStateChangedEvent, Event);

    public:

        //////////////////////////////////////////
        inline EcsSceneStateChangedEvent(
            EcsSceneId _sceneId = EcsSceneId(),
            EcsSceneState _state = EcsSceneState())
            : m_sceneId(_sceneId)
            , m_state(_state)
        {}

        //////////////////////////////////////////
        inline EcsSceneId getSceneId() const { return m_sceneId; }

        //////////////////////////////////////////
        inline EcsSceneState getState() const { return m_state; }

    private:
        EcsSceneId m_sceneId;
        EcsSceneState m_state = EcsSceneState::None;
    };


    //////////////////////////////////////////
    // Class EcsChildRemovedEvent
    //
    //////////////////////////////////////////
    class MAZE_CORE_API EcsChildRemovedEvent
        : public GenericEvent<EcsChildRemovedEvent>
    {
    public:
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(EcsChildRemovedEvent, Event);

    public:

        //////////////////////////////////////////
        inline EcsChildRemovedEvent(
            EntityId _childEid = c_invalidEntityId)
            : m_childEid(_childEid)
        {}

        //////////////////////////////////////////
        inline EntityId getChildEid() const { return m_childEid; }


    private:
        EntityId m_childEid = c_invalidEntityId;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeEcsCoreEvents_hpp_
//////////////////////////////////////////
