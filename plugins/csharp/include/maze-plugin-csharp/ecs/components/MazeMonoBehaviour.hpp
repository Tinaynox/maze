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
#if (!defined(_MazeMonoBehaviour_hpp_))
#define _MazeMonoBehaviour_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-plugin-csharp/mono/MazeScriptClass.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MonoBehaviour);
    MAZE_USING_SHARED_PTR(MonoEvent);


    //////////////////////////////////////////
    // Class MonoBehaviour
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API MonoBehaviour
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MonoBehaviour, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MonoBehaviour);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~MonoBehaviour();

        //////////////////////////////////////////
        static MonoBehaviourPtr Create(HashedCString _scriptClass = HashedCString());


        //////////////////////////////////////////
        virtual ComponentId getComponentId() const MAZE_OVERRIDE { return m_componentId; }

        //////////////////////////////////////////
        virtual CString getComponentClassName() const { return m_monoClass ? m_monoClass->getFullName().c_str() : getClassQualifiedName(); }


        //////////////////////////////////////////
        void setMonoClass(ScriptClassPtr const& _scriptClass);

        //////////////////////////////////////////
        void setMonoClass(HashedCString _scriptClass);

        //////////////////////////////////////////
        ScriptClassPtr const& getMonoClass() const { return m_monoClass; }


        //////////////////////////////////////////
        void setSerializableData(DataBlock const& _dataBlock);

        //////////////////////////////////////////
        DataBlock getSerializableData() const;

        //////////////////////////////////////////
        void setData(DataBlock const& _dataBlock);

        //////////////////////////////////////////
        DataBlock getData() const;


        //////////////////////////////////////////
        ScriptInstancePtr const& getMonoInstance() const { return m_monoInstance; }

        //////////////////////////////////////////
        ScriptInstancePtr const& ensureMonoInstance();

        //////////////////////////////////////////
        void destroyMonoInstance();

    protected:

        //////////////////////////////////////////
        MonoBehaviour();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init(HashedCString _scriptClass);

        //////////////////////////////////////////
        virtual bool init(
            Component* _component,
            EntityCopyData _copyData);


        

        //////////////////////////////////////////
        void createMonoInstance();

        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    protected:
        ComponentId m_componentId = 0u;
        ScriptClassPtr m_monoClass;
        ScriptInstancePtr m_monoInstance;

        DataBlock m_cachedData;
    };


    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnCreate(
        EntityAddedEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour);

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnUpdate(
        UpdateEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour);

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnDestroy(
        EntityRemovedEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour);

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnMonoEvent(
        MonoEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour);

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnNativeEvent(
        Event const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoBehaviour_hpp_
//////////////////////////////////////////
