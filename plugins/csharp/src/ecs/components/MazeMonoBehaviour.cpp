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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/MazeCustomComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MonoBehaviour
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MonoBehaviour, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(DataBlock, classData, DataBlock(), getMonoClassData, setMonoClassData),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(DataBlock, data, DataBlock(), getData, setData));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MonoBehaviour);

    //////////////////////////////////////////
    MonoBehaviour::MonoBehaviour()
    {
    }

    //////////////////////////////////////////
    MonoBehaviour::~MonoBehaviour()
    {
    }

    //////////////////////////////////////////
    MonoBehaviourPtr MonoBehaviour::Create(HashedCString _scriptClass)
    {
        MonoBehaviourPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MonoBehaviour, object, init(_scriptClass));
        return object;
    }

    //////////////////////////////////////////
    bool MonoBehaviour::init(HashedCString _scriptClass)
    {
        if (!_scriptClass.empty())
            setMonoClass(_scriptClass);
        
        return true;
    }

    //////////////////////////////////////////
    void MonoBehaviour::destroyMonoInstance()
    {
        if (!m_monoInstance.isValid())
            return;

        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    void MonoBehaviour::createMonoInstance()
    {
        destroyMonoInstance();

        MAZE_ERROR_RETURN_IF(!m_monoClass, "MonoClass is empty!");
        MAZE_ERROR_RETURN_IF(!m_monoClass->isValid(), "MonoClass is invalid!");

        m_monoInstance = m_monoClass->instantiate();
        m_monoInstance.setProperty("nativeComponentPtr", castRaw<Component>());
    }

    //////////////////////////////////////////
    void MonoBehaviour::setMonoClass(ScriptClassPtr const& _scriptClass)
    {
        if (m_monoClass == _scriptClass)
            return;

        m_monoClass = _scriptClass;

        m_monoClass->getMethod("OnCreate");
        m_monoClass->getMethod("OnUpdate", 1);

        m_componentId = GetComponentIdByName(m_monoClass->getFullname().c_str());

        createMonoInstance();
    }

    //////////////////////////////////////////
    void MonoBehaviour::setMonoClass(HashedCString _scriptClass)
    {
        ScriptClassPtr const& scriptClass = MonoEngine::GetMonoBehaviourSubClass(_scriptClass);
        MAZE_ERROR_RETURN_IF(!scriptClass, "MonoBehaviour class is not found: %s!", _scriptClass.str);
        setMonoClass(scriptClass);
    }

    //////////////////////////////////////////
    void MonoBehaviour::setMonoClassData(DataBlock _dataBlock)
    {
        Debug::LogError("4");
    }

    //////////////////////////////////////////
    DataBlock MonoBehaviour::getMonoClassData() const
    {
        DataBlock db;
        Debug::LogError("3");

        return std::move(db);
    }

    //////////////////////////////////////////
    void MonoBehaviour::setData(DataBlock _dataBlock)
    {
        Debug::LogError("1");
    }

    //////////////////////////////////////////
    DataBlock MonoBehaviour::getData() const
    {
        DataBlock db;
        Debug::LogError("2");

        return std::move(db);
    }


    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnCreate(
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
        ScriptInstance& scriptInstance = _monoBehaviour->getMonoInstance();

        if (!scriptClass || !scriptInstance.isValid())
            return;

        if (scriptClass->getOnCreateMethod())
            scriptInstance.invokeMethod(
                scriptClass->getOnCreateMethod());
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnUpdate(
        UpdateEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
        ScriptInstance& scriptInstance = _monoBehaviour->getMonoInstance();

        if (!scriptClass || !scriptInstance.isValid())
            return;

        if (scriptClass->getOnUpdateMethod())
            scriptInstance.invokeMethod(
                scriptClass->getOnUpdateMethod(),
                _event.getDt());
    }

    /*
    CustomComponentSystemHolder holder0(
        MAZE_HCS("123"),
        ClassInfo<EntityAddedToSampleEvent>::UID(),
        [](EcsWorld* _world) { return _world->requestDynamicIdSample<MonoBehaviour>(GetComponentIdByName("Sandbox.Player")); },
        (ComponentSystemEventHandler::Func)&MonoBehaviourOnCreate);

    CustomComponentSystemHolder holder1(
        MAZE_HCS("456"),
        ClassInfo<UpdateEvent>::UID(),
        [](EcsWorld* _world) { return _world->requestDynamicIdSample<MonoBehaviour>(GetComponentIdByName("Sandbox.Player")); },
        (ComponentSystemEventHandler::Func)&MonoBehaviourOnUpdate);

    CustomComponentSystemHolder holder2(
        MAZE_HCS("789"),
        ClassInfo<EntityAddedToSampleEvent>::UID(),
        [](EcsWorld* _world) { return _world->requestDynamicIdSample<MonoBehaviour>(GetComponentIdByName("Sandbox.Player2")); },
        (ComponentSystemEventHandler::Func)&MonoBehaviourOnCreate);

    CustomComponentSystemHolder holder3(
        MAZE_HCS("000"),
        ClassInfo<UpdateEvent>::UID(),
        [](EcsWorld* _world) { return _world->requestDynamicIdSample<MonoBehaviour>(GetComponentIdByName("Sandbox.Player2")); },
        (ComponentSystemEventHandler::Func)&MonoBehaviourOnUpdate);
    */



    /*
    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(MonoBehaviourSystemAppear,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        MonoBehaviour*)
    {
        _entity->findMultiComponent<MonoBehaviour>(
            [&_event](MonoBehaviour* _monoBehaviour)
            {
                ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
                ScriptInstance& scriptInstance = _monoBehaviour->getMonoInstance();

                if (!scriptClass || !scriptInstance.isValid())
                    return false;

                if (scriptClass->getOnCreateMethod())
                    scriptInstance.invokeMethod(
                        scriptClass->getOnCreateMethod());

                return false;
            });
    }

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(MonoBehaviourSystemUpdate,
        {},
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        MonoBehaviour*)
    {
        _entity->findMultiComponent<MonoBehaviour>(
            [&_event](MonoBehaviour* _monoBehaviour)
            {
                ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
                ScriptInstance& scriptInstance = _monoBehaviour->getMonoInstance();

                if (!scriptClass || !scriptInstance.isValid())
                    return false;

                if (scriptClass->getOnUpdateMethod())
                    scriptInstance.invokeMethod(
                        scriptClass->getOnUpdateMethod(),
                        _event.getDt());
                return false;
            });
        
    }
    */

} // namespace Maze
//////////////////////////////////////////
