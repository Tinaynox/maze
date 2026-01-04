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
#include "maze-plugin-csharp/ecs/events/MazeEcsCSharpEvents.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/MazeCSharpService.hpp"
#include "maze-plugin-csharp/managers/MazeMonoSerializationManager.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/MazeCustomComponentSystemHolder.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    void RemapDataBlockEcsIdsAfterCopy(
        DataBlock& _dataBlock,
        MonoBehaviour* _monoBehaviour,
        EntityPostCopyEvent const& _event)
    {
        auto convertEntityId =
            [&](EntityId _entityId)
            {
                if (_entityId == c_invalidEntityId)
                    return c_invalidEntityId;

                Entity* entity = _event.sourceEntity->getEcsWorld()->getEntity(_entityId).get();
                if (entity)
                {
                    auto it = _event.copyData.getEntities().find(entity);
                    if (it == _event.copyData.getEntities().end())
                        return c_invalidEntityId;

                    if (!it->second)
                        return c_invalidEntityId;

                    return it->second->getId();
                }
                else
                    return c_invalidEntityId;
            };

        for (DataBlock* subBlock : _dataBlock)
        {
            if (StringHelper::IsEndsWith(subBlock->getName().str, ":EntityId"))
            {
                EntityId entityId(subBlock->getS32(MAZE_HCS("value"), (S32)c_invalidEntityId));
                subBlock->setS32(MAZE_HCS("value"), (S32)convertEntityId(entityId));
            }
            else
            if (StringHelper::IsEndsWith(subBlock->getName().str, ":Array<EntityId>"))
            {
                for (DataBlock::ParamIndex i = 0; i < (DataBlock::ParamIndex)subBlock->getParamsCount(); ++i)
                {
                    DataBlockParamType paramType = subBlock->getParamType(i);
                    if (paramType == DataBlockParamType::ParamS32)
                    {
                        EntityId entityId(subBlock->getS32(i));
                        subBlock->setS32(i, (S32)convertEntityId(entityId));
                    }
                }
            }
            else
            {
                RemapDataBlockEcsIdsAfterCopy(*subBlock, _monoBehaviour, _event);
            }
        }
    }


    //////////////////////////////////////////
    // Class MonoBehaviour
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MonoBehaviour, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(DataBlock, data, DataBlock(), getSerializableData, setSerializableData));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MonoBehaviour);

    //////////////////////////////////////////
    MonoBehaviour::MonoBehaviour()
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoPreShutdownEvent>(this, &MonoBehaviour::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<MonoPreReloadEvent>(this, &MonoBehaviour::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<MonoReloadEvent>(this, &MonoBehaviour::notifyEvent);
    }

    //////////////////////////////////////////
    MonoBehaviour::~MonoBehaviour()
    {
        destroyMonoInstance();

        EventManager::GetInstancePtr()->unsubscribeEvent<MonoPreShutdownEvent>(this);
        EventManager::GetInstancePtr()->unsubscribeEvent<MonoPreReloadEvent>(this);
        EventManager::GetInstancePtr()->unsubscribeEvent<MonoReloadEvent>(this);
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
    bool MonoBehaviour::init(
        Component* _component,
        EntityCopyData _copyData)
    {
        MonoBehaviour* other = _component->castRaw<MonoBehaviour>();
        ScriptClassPtr const& scriptClass = other->getMonoClass();
        if (scriptClass)
            setMonoClass(scriptClass);

        if (!Component::init(_component, _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void MonoBehaviour::processEvent(Event* _event)
    {
        ClassUID eventUID = _event->getClassUID();
        if (eventUID == ClassInfo<EntityPostCopyEvent>::UID())
        {
            EntityPostCopyEvent* copyEvent = _event->castRaw<EntityPostCopyEvent>();
            processPostCopy(*copyEvent);
        }
    }

    //////////////////////////////////////////
    void MonoBehaviour::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoPreShutdownEvent>::UID())
        {
            if (getEntityRaw()->getEcsWorld())
                getEntityRaw()->getEcsWorld()->sendEventImmediate(getEntityId(), _event);

            m_cachedData = getData();
            // m_cachedData.saveTextFile(EcsHelper::GetName(getEntityRaw()));
            destroyMonoInstance();
        }
        else
        if (_eventUID == ClassInfo<MonoPreReloadEvent>::UID())
        {
            if (m_monoClass)
            {
                ScriptClassPtr const& scriptClass = MonoEngine::GetMonoBehaviourSubClass(m_monoClass->getFullName());

                if (scriptClass)
                {
                    setMonoClass(scriptClass);
                }
                else
                {
                    MAZE_ERROR_RETURN("Undefined MonoClass: %s", m_monoClass->getFullName().c_str());
                }
            }
        }
        else
        if (_eventUID == ClassInfo<MonoReloadEvent>::UID())
        {
            if (getEntityRaw()->getEcsWorld())
                setData(m_cachedData);

            m_cachedData.clear();

            if (getEntityRaw()->getEcsWorld())
                getEntityRaw()->getEcsWorld()->sendEventImmediate(getEntityId(), _event);
        }
    }

    //////////////////////////////////////////
    ScriptInstancePtr const& MonoBehaviour::ensureMonoInstance()
    {
        if (!m_monoInstance && m_monoClass && m_monoClass->isValid())
            createMonoInstance();

        return m_monoInstance;
    }

    //////////////////////////////////////////
    void MonoBehaviour::destroyMonoInstance()
    {
        if (!m_monoInstance)
            return;

        m_monoInstance->destroy();
        m_monoInstance.reset();
    }

    //////////////////////////////////////////
    void MonoBehaviour::createMonoInstance()
    {
        destroyMonoInstance();

        MAZE_ERROR_RETURN_IF(!m_monoClass, "MonoClass is empty!");
        MAZE_ERROR_RETURN_IF(!m_monoClass->isValid(), "MonoClass is invalid!");

        m_monoInstance = m_monoClass->instantiate();
        m_monoInstance->setPropertyValue(MAZE_HCS("NativeComponentPtr"), castRaw<Component>());
    }

    //////////////////////////////////////////
    void MonoBehaviour::setMonoClass(ScriptClassPtr const& _scriptClass)
    {
        if (m_monoClass == _scriptClass)
            return;

        m_monoClass = _scriptClass;

        m_monoClass->getMethod("OnCreate");
        m_monoClass->getMethod("OnUpdate", 1);

        m_componentId = GetComponentIdByName(m_monoClass->getFullName().c_str());

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
    void MonoBehaviour::setSerializableData(DataBlock const& _dataBlock)
    {
        if (!getEntityRaw())
            return;

        if (m_monoClass && m_monoInstance)
        {
            MAZE_BP_RETURN_IF(!getEntityRaw()->getEcsWorld());

            MonoHelper::IterateSerializableFields(m_monoClass,
                [&](ScriptFieldPtr const& _field)
                {
                    MonoSerializationManager::GetInstancePtr()->loadFieldFromDataBlock(
                        getEntityRaw()->getEcsWorld(),
                        *m_monoInstance,
                        _field,
                        _dataBlock);
                });

            MonoHelper::IterateSerializableProperties(m_monoClass,
                [&](ScriptPropertyPtr const& _prop)
                {
                    MonoSerializationManager::GetInstancePtr()->loadPropertyFromDataBlock(
                        getEntityRaw()->getEcsWorld(),
                        *m_monoInstance,
                        _prop,
                        _dataBlock);
                });
        }
    }

    //////////////////////////////////////////
    DataBlock MonoBehaviour::getSerializableData() const
    {
        DataBlock db;
        
        if (m_monoClass && m_monoInstance)
        {
            MonoHelper::IterateSerializableFields(m_monoClass,
                [&](ScriptFieldPtr const& _prop)
            {
                MonoSerializationManager::GetInstancePtr()->saveFieldToDataBlock(
                    getEntityRaw()->getEcsWorld(),
                    *m_monoInstance,
                    _prop,
                    db);
            });

            MonoHelper::IterateSerializableProperties(m_monoClass,
                [&](ScriptPropertyPtr const& _prop)
                {
                    MonoSerializationManager::GetInstancePtr()->savePropertyToDataBlock(
                        getEntityRaw()->getEcsWorld(),
                        *m_monoInstance,
                        _prop,
                        db);
                });
        }


        return std::move(db);
    }


    //////////////////////////////////////////
    void MonoBehaviour::setData(DataBlock const& _dataBlock)
    {
        if (m_monoClass && m_monoInstance)
        {
            MonoHelper::IterateAllFields(m_monoClass,
                [&](ScriptFieldPtr const& _field)
            {
                if (_field->isStatic())
                    return;

                MonoSerializationManager::GetInstancePtr()->loadFieldFromDataBlock(
                    getEntityRaw()->getEcsWorld(),
                    *m_monoInstance,
                    _field,
                    _dataBlock);
            });

            MonoHelper::IterateSerializableProperties(m_monoClass,
                [&](ScriptPropertyPtr const& _prop)
            {
                if (_prop->isStaticGetter())
                    return;

                MonoSerializationManager::GetInstancePtr()->loadPropertyFromDataBlock(
                    getEntityRaw()->getEcsWorld(),
                    *m_monoInstance,
                    _prop,
                    _dataBlock);
            });
        }
    }

    //////////////////////////////////////////
    DataBlock MonoBehaviour::getData() const
    {
        DataBlock db;

        if (m_monoClass && m_monoInstance)
        {
            MonoHelper::IterateAllFields(m_monoClass,
                [&](ScriptFieldPtr const& _field)
            {
                if (_field->isStatic())
                    return;

                MonoSerializationManager::GetInstancePtr()->saveFieldToDataBlock(
                    getEntityRaw()->getEcsWorld(),
                    *m_monoInstance,
                    _field,
                    db);
            });

            MonoHelper::IterateSerializableProperties(m_monoClass,
                [&](ScriptPropertyPtr const& _prop)
            {
                if (_prop->isStaticGetter())
                    return;

                MonoSerializationManager::GetInstancePtr()->savePropertyToDataBlock(
                    getEntityRaw()->getEcsWorld(),
                    *m_monoInstance,
                    _prop,
                    db);
            });
        }


        return std::move(db);
    }

    //////////////////////////////////////////
    void MonoBehaviour::processPostCopy(EntityPostCopyEvent const& _event)
    {
        // Copy fields and properties from other behaviour
        if (m_monoClass && m_monoInstance && _event.sourceEntity)
        {
            MonoBehaviour* other = (MonoBehaviour*)_event.sourceEntity->getComponentById(getComponentId()).get();
            if (other && other->m_monoClass && other->m_monoInstance)
            {
                DataBlock db;

                MonoHelper::IterateAllFields(other->m_monoClass,
                    [&](ScriptFieldPtr const& _prop)
                {
                    MonoSerializationManager::GetInstancePtr()->saveFieldToDataBlock(
                        _event.copyData.getWorld(),
                        *other->m_monoInstance,
                        _prop,
                        db);
                });

                MonoHelper::IterateSerializableProperties(other->m_monoClass,
                    [&](ScriptPropertyPtr const& _prop)
                {
                    MonoSerializationManager::GetInstancePtr()->savePropertyToDataBlock(
                        _event.copyData.getWorld(),
                        *other->m_monoInstance,
                        _prop,
                        db);
                });

                RemapDataBlockEcsIdsAfterCopy(db, this, _event);

                MonoHelper::IterateAllFields(m_monoClass,
                    [&](ScriptFieldPtr const& _field)
                {
                    if (_field->isStatic())
                        return;

                    MonoSerializationManager::GetInstancePtr()->loadFieldFromDataBlock(
                        _event.copyData.getWorld(),
                        *m_monoInstance,
                        _field,
                        db);
                });

                MonoHelper::IterateSerializableProperties(m_monoClass,
                    [&](ScriptPropertyPtr const& _prop)
                {
                    if (_prop->isStaticGetter())
                        return;

                    MonoSerializationManager::GetInstancePtr()->loadPropertyFromDataBlock(
                        _event.copyData.getWorld(),
                        *m_monoInstance,
                        _prop,
                        db);
                });
            }
        }
    }

    //////////////////////////////////////////
    /*
    COMPONENT_SYSTEM_EVENT_HANDLER(MonoBehaviourOnPostCopy,
        {},
        {},
        EntityPostCopyEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        _monoBehaviour->processPostCopy(_event);
    }
    */

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnCreate(
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
        if (!scriptClass)
            return;

        if (ScriptInstancePtr const& scriptInstance = _monoBehaviour->ensureMonoInstance())
        {
            if (scriptClass->getOnCreateMethod())
                scriptInstance->invokeMethod(
                    scriptClass->getOnCreateMethod());
        }
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnUpdate(
        UpdateEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
        ScriptInstancePtr const& scriptInstance = _monoBehaviour->getMonoInstance();

        if (!scriptClass || !scriptInstance || !scriptInstance->isValid())
            return;

        if (scriptClass->getOnUpdateMethod())
            scriptInstance->invokeMethod(
                scriptClass->getOnUpdateMethod(),
                _event.getDt());
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnDestroy(
        EntityRemovedFromSampleEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
        ScriptInstancePtr const& scriptInstance = _monoBehaviour->getMonoInstance();

        if (scriptClass && scriptInstance && scriptInstance->isValid())
        {
            if (scriptClass->getOnDestroyMethod())
                scriptInstance->invokeMethod(
                    scriptClass->getOnDestroyMethod());
        }

        _monoBehaviour->destroyMonoInstance();
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnMonoEvent(
        MonoEvent const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
        ScriptInstancePtr const& scriptInstance = _monoBehaviour->getMonoInstance();

        if (scriptClass && scriptInstance && scriptInstance->isValid())
        {
            MonoMethod* monoMethod = scriptClass->getOnMonoEventMethodUnsafe(_event.getEventUID());
            scriptInstance->invokeMethod(monoMethod, _event.getMonoEvent());
        }
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_CSHARP_API void MonoBehaviourOnNativeEvent(
        Event const& _event,
        Entity* _entity,
        MonoBehaviour* _monoBehaviour)
    {
        ScriptClassPtr const& scriptClass = _monoBehaviour->getMonoClass();
        ScriptInstancePtr const& scriptInstance = _monoBehaviour->getMonoInstance();

        if (scriptClass && scriptInstance && scriptInstance->isValid())
        {
            ClassUID eventUID = _event.getEventUID();

            ScriptClassPtr const& monoEventScriptClass = MonoEngine::GetNativeEventSubClass(eventUID);

            MonoObject* monoEventObj = mono_object_new(
                MonoEngine::GetMonoDomain(),
                monoEventScriptClass->getMonoClass());

            MonoMethod* monoMethod = scriptClass->getOnNativeEventMethodUnsafe(_event.getEventUID());

            MetaClass const* eventMetaClass = _event.getMetaClass();
            for (S32 i = 0; i < eventMetaClass->getPropertiesCount(); ++i)
            {
                MetaProperty const* eventMetaProperty = eventMetaClass->getProperty(i);
                ClassUID eventMetaPropertyClassUID = eventMetaProperty->getValueClassUID();

                MonoClassField* monoField = mono_class_get_field_from_name(
                    monoEventScriptClass->getMonoClass(),
                    eventMetaProperty->getName());
                MAZE_ERROR_CONTINUE_IF(
                    !monoField,
                    "Property is not found: %s",
                    eventMetaProperty->getName().str);

                auto writeFieldValueFunc = MonoSerializationManager::GetInstancePtr()->getWriteMetaPropertyToMonoClassFieldFunction(eventMetaPropertyClassUID);
                MAZE_ERROR_CONTINUE_IF(
                    !writeFieldValueFunc,
                    "WriteMetaPropertyToMonoClassFieldFunction is not found for property: %s",
                    eventMetaProperty->getName().str);

                writeFieldValueFunc(
                    _event.getMetaInstance(),
                    eventMetaProperty,
                    monoEventObj,
                    monoField);
            }
            scriptInstance->invokeMethod(monoMethod, (MonoObject*)monoEventObj);
        }
    }

} // namespace Maze
//////////////////////////////////////////
