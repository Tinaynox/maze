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
#include "maze-plugin-csharp/managers/MazeMonoSerializationManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MonoSerializationManager
    //
    //////////////////////////////////////////
    MonoSerializationManager* MonoSerializationManager::s_instance = nullptr;

    //////////////////////////////////////////
    MonoSerializationManager::MonoSerializationManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    MonoSerializationManager::~MonoSerializationManager()
    {
        s_instance = nullptr;

        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->unsubscribeEvent<CSharpCoreAssemblyLoadedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
        }
    }

    //////////////////////////////////////////
    void MonoSerializationManager::Initialize(
        MonoSerializationManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(MonoSerializationManager, _manager, init());
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::init()
    {
        EventManager::GetInstancePtr()->subscribeEvent<CSharpCoreAssemblyLoadedEvent>(this, &MonoSerializationManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &MonoSerializationManager::notifyEvent);


        registerWriteMetaPropertyToMonoClassFieldFunctions();

        return true;
    }

    //////////////////////////////////////////
    void MonoSerializationManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<CSharpCoreAssemblyLoadedEvent>::UID())
        {
#define MAZE_MONO_SERIALIZATION_TYPE(DTypeName, DType)                                                                     \
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS(DTypeName),                                            \
                [](EcsWorld*, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)      \
                {                                                                                                          \
                    DType value;                                                                                           \
                    _instance.getPropertyValue(_prop, value);                                                              \
                    _dataBlock.set ## DType(_prop->getName(), value);                                                      \
                },                                                                                                         \
                [](EcsWorld*, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)      \
                {                                                                                                          \
                    DataBlock::ParamIndex idx = _dataBlock.findParamIndex(_prop->getName());                               \
                    if (idx >= 0)                                                                                          \
                        _instance.setPropertyValue(_prop, _dataBlock.get ## DType(idx));                                   \
                },                                                                                                         \
                [](EcsWorld*, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)        \
                {                                                                                                          \
                    DType value;                                                                                           \
                    _instance.getFieldValue(_field, value);                                                                \
                    _dataBlock.set ## DType(_field->getName(), value);                                                     \
                },                                                                                                         \
                [](EcsWorld*, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)        \
                {                                                                                                          \
                    DataBlock::ParamIndex idx = _dataBlock.findParamIndex(_field->getName());                              \
                    if (idx >= 0)                                                                                          \
                        _instance.setFieldValue(_field, _dataBlock.get ## DType(idx));                                     \
                });

            // System
            MAZE_MONO_SERIALIZATION_TYPE("System.String", String);
            MAZE_MONO_SERIALIZATION_TYPE("System.Char", U16); // Wide char
            MAZE_MONO_SERIALIZATION_TYPE("System.Boolean", Bool);
            MAZE_MONO_SERIALIZATION_TYPE("System.SByte", S8);
            MAZE_MONO_SERIALIZATION_TYPE("System.Int16", S16);
            MAZE_MONO_SERIALIZATION_TYPE("System.Int32", S32);
            MAZE_MONO_SERIALIZATION_TYPE("System.Int64", S64);
            MAZE_MONO_SERIALIZATION_TYPE("System.Byte", U8);
            MAZE_MONO_SERIALIZATION_TYPE("System.UInt16", U16);
            MAZE_MONO_SERIALIZATION_TYPE("System.UInt32", U32);
            MAZE_MONO_SERIALIZATION_TYPE("System.UInt64", U64);
            MAZE_MONO_SERIALIZATION_TYPE("System.Single", F32);
            MAZE_MONO_SERIALIZATION_TYPE("System.Double", F64);


            // System.Collections
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("System.Collections.Generic.List"),
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    MAZE_WARNING("NOT IMPLEMENTED");
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    MAZE_WARNING("NOT IMPLEMENTED");
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MAZE_WARNING("NOT IMPLEMENTED");

                    MonoObject* listMonoObject = nullptr;
                    _instance.getFieldValue(_field, listMonoObject);
                    if (listMonoObject)
                    {
                        MonoClass* listMonoClass = mono_object_get_class(listMonoObject);
                        MonoMethod* getEnumeratorMethod = mono_class_get_method_from_name(listMonoClass, "GetEnumerator", 0);
                        MonoObject* enumerator = MonoHelper::InvokeMethod(listMonoObject, getEnumeratorMethod, nullptr);
                        if (enumerator)
                        {
                            MonoClass* enumeratorClass = mono_object_get_class(enumerator);
                            MonoMethod* moveNextMethod = mono_class_get_method_from_name(enumeratorClass, "MoveNext", 0);
                            MonoMethod* getCurrentMethod = mono_class_get_method_from_name(enumeratorClass, "get_Current", 0);

                            // #TODO:
                            /*
                            while (MonoHelper::InvokeMethod(enumerator, moveNextMethod, nullptr))
                            {
                                MonoObject* current = MonoHelper::InvokeMethod(enumerator, getCurrentMethod, nullptr);
                                if (current)
                                {
                                    
                                }
                            }
                            */
                        }
                    }
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    MAZE_WARNING("NOT IMPLEMENTED");
                });

            // Core
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("Maze.Core.Entity"),
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    Entity* value = nullptr;
                    _instance.getPropertyValue(_prop, value);
                    EcsHelper::SerializeEntityIdToDataBlock(_dataBlock, _prop->getName().c_str(), value ? value->getId() : c_invalidEntityId);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    EntityId entityId = EcsHelper::DeserializeEntityIdFromDataBlock(_dataBlock, _prop->getName().c_str());
                    EntityPtr const& entity = _world->getEntity(entityId);
                    if (entity)
                        _instance.setPropertyValue(_prop, entity.get());
                    else
                        _instance.setPropertyValue(_prop, nullptr);
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    Entity* value = nullptr;
                    _instance.getFieldValue(_field, value);
                    EcsHelper::SerializeEntityIdToDataBlock(_dataBlock, _field->getName().c_str(), value ? value->getId() : c_invalidEntityId);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    EntityId entityId = EcsHelper::DeserializeEntityIdFromDataBlock(_dataBlock, _field->getName().c_str());
                    EntityPtr const& entity = _world->getEntity(entityId);
                    if (entity)
                        _instance.setFieldValue(_field, entity.get());
                    else
                        _instance.setFieldValue(_field, nullptr);
                });

            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec2S", Vec2S);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec3S", Vec3S);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec4S", Vec4S);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec2U", Vec2U);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec3U", Vec3U);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec4U", Vec4U);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec2F", Vec2F);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec3F", Vec3F);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Vec4F", Vec4F);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Mat3F", Mat3F);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Mat4F", Mat4F);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.TMat", TMat);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.Rect2F", Vec4F);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.AssetUnitId", U32);

            // Graphics
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Graphics.ColorU32", Vec4U8);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Graphics.ColorF128", Vec4F);

#undef MAZE_MONO_SERIALIZATION_TYPE



            ScriptClassPtr const& componentClass = MonoEngine::GetComponentClass();

            // Core
            registerPropertyAndFieldDataBlockSubClassSerialization(
                componentClass->getMonoClass(),
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    MonoObject* componentInstance = nullptr;
                    _instance.getPropertyValue(_prop, componentInstance);
                    MonoHelper::SerializeComponentToDataBlock(_dataBlock, _prop->getName().c_str(), componentInstance);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    MonoObject* result = MonoHelper::DeserializeComponentFromDataBlock(
                        _world, _dataBlock, _prop->getName().c_str(), _prop->getMonoType());
                    if (result)
                        _instance.setPropertyValue(_prop, *result);
                    else
                        _instance.resetPropertyValue(_prop);
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MonoObject* componentInstance = nullptr;
                    _instance.getFieldValue(_field, componentInstance);
                    MonoHelper::SerializeComponentToDataBlock(_dataBlock, _field->getName().c_str(), componentInstance);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    MonoObject* result = MonoHelper::DeserializeComponentFromDataBlock(
                        _world, _dataBlock, _field->getName().c_str(), _field->getMonoType());
                    if (result)
                        _instance.setFieldValue(_field, *result);
                    else
                        _instance.resetFieldValue(_field);
                });
        }
        else
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            m_propertyDataBlockSerializationData.clear();
            m_fieldDataBlockSerializationData.clear();
            m_propertyDataBlockSubClassSerializationData.clear();
            m_fieldDataBlockSubClassSerializationData.clear();
        }
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerPropertyDataBlockSerialization(
        HashedCString _typeName,
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propToDataBlockCb,
        std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propFromDataBlockCb)
    {
        ScriptPropertyDataBlockSerializationData& data = m_propertyDataBlockSerializationData[_typeName];
        data.propToDataBlockCb = _propToDataBlockCb;
        data.propFromDataBlockCb = _propFromDataBlockCb;
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerPropertyDataBlockSubClassSerialization(
        MonoClass* _monoClass,
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propToDataBlockCb,
        std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propFromDataBlockCb)
    {
        ScriptPropertyDataBlockSerializationData& data = m_propertyDataBlockSubClassSerializationData[_monoClass];
        data.propToDataBlockCb = _propToDataBlockCb;
        data.propFromDataBlockCb = _propFromDataBlockCb;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::savePropertyToDataBlock(
        EcsWorld* _ecsWorld,
        ScriptInstance const& _instance,
        ScriptPropertyPtr const& _property,
        DataBlock& _dataBlock)
    {
        auto it = m_propertyDataBlockSerializationData.find(_property->getTypeName());
        if (it != m_propertyDataBlockSerializationData.end())
        {
            it->second.propToDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
            return true;
        }
        else
        {
            MonoClass* monoClass = mono_class_from_mono_type(_property->getMonoType());
            if (monoClass)
            {
                for (auto const& data : m_propertyDataBlockSubClassSerializationData)
                {
                    MonoClass* baseClass = data.first;
                    if (mono_class_is_subclass_of(monoClass, baseClass, false))
                    {
                        data.second.propToDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
                        return true;
                    }
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::loadPropertyFromDataBlock(
        EcsWorld* _ecsWorld,
        ScriptInstance& _instance,
        ScriptPropertyPtr const& _property,
        DataBlock const& _dataBlock)
    {
        auto it = m_propertyDataBlockSerializationData.find(_property->getTypeName());
        if (it != m_propertyDataBlockSerializationData.end())
        {
            it->second.propFromDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
            return true;
        }
        else
        {
            MonoClass* monoClass = mono_class_from_mono_type(_property->getMonoType());
            if (monoClass)
            {
                for (auto const& data : m_propertyDataBlockSubClassSerializationData)
                {
                    MonoClass* baseClass = data.first;
                    if (mono_class_is_subclass_of(monoClass, baseClass, false))
                    {
                        data.second.propFromDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
                        return true;
                    }
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerFieldDataBlockSerialization(
        HashedCString _typeName,
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _propToDataBlockCb,
        std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _propFromDataBlockCb)
    {
        ScriptFieldDataBlockSerializationData& data = m_fieldDataBlockSerializationData[_typeName];
        data.propToDataBlockCb = _propToDataBlockCb;
        data.propFromDataBlockCb = _propFromDataBlockCb;
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerFieldDataBlockSubClassSerialization(
        MonoClass* _monoClass,
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _propToDataBlockCb,
        std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _propFromDataBlockCb)
    {
        ScriptFieldDataBlockSerializationData& data = m_fieldDataBlockSubClassSerializationData[_monoClass];
        data.propToDataBlockCb = _propToDataBlockCb;
        data.propFromDataBlockCb = _propFromDataBlockCb;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::saveFieldToDataBlock(
        EcsWorld* _ecsWorld,
        ScriptInstance const& _instance,
        ScriptFieldPtr const& _field,
        DataBlock& _dataBlock)
    {
        if (_field->isGenericType())
        {
            Size genericSubTypeIndex = _field->getTypeName().getString().find('<');
            if (genericSubTypeIndex == String::npos)
                return false;

            HashedString genericBaseType = HashedString(_field->getTypeName().getString().substr(0, genericSubTypeIndex));

            auto it = m_fieldDataBlockSerializationData.find(genericBaseType);
            if (it == m_fieldDataBlockSerializationData.end())
                return false;
            
            it->second.propToDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
            return true;
        }
        else
        {
            auto it = m_fieldDataBlockSerializationData.find(_field->getTypeName());
            if (it != m_fieldDataBlockSerializationData.end())
            {
                it->second.propToDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
                return true;
            }
            else
            {
                MonoClass* monoClass = mono_class_from_mono_type(_field->getMonoType());
                if (monoClass)
                {
                    for (auto const& data : m_fieldDataBlockSubClassSerializationData)
                    {
                        MonoClass* baseClass = data.first;
                        if (mono_class_is_subclass_of(monoClass, baseClass, false))
                        {
                            data.second.propToDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
                            return true;
                        }
                    }
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::loadFieldFromDataBlock(
        EcsWorld* _ecsWorld,
        ScriptInstance& _instance,
        ScriptFieldPtr const& _field,
        DataBlock const& _dataBlock)
    {
        auto it = m_fieldDataBlockSerializationData.find(_field->getTypeName());
        if (it != m_fieldDataBlockSerializationData.end())
        {
            it->second.propFromDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
            return true;
        }
        else
        {
            MonoClass* monoClass = mono_class_from_mono_type(_field->getMonoType());
            if (monoClass)
            {
                for (auto const& data : m_fieldDataBlockSubClassSerializationData)
                {
                    MonoClass* baseClass = data.first;
                    if (mono_class_is_subclass_of(monoClass, baseClass, false))
                    {
                        data.second.propFromDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
                        return true;
                    }
                }
            }
        }

        return false;
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerWriteMetaPropertyToMonoClassFieldFunctions()
    {
        #define REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(DType) \
            m_writeMetaPropertyToMonoClassFieldFunctions[ClassInfo<DType>::UID()] = MonoHelper::WriteMetaPropertyToMonoClassField ## DType;
    
        // Core
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(String);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Bool);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(F32);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(F64);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(S8);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(S16);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(S32);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(S64);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(U8);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(U16);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(U32);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(U64);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec2F);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec3F);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec4F);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec2S);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec3S);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec4S);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec2U);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec3U);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Vec4U);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Mat3F);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Mat4F);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(TMat);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(Rect2F);

        // Graphics
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(ColorU32);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(ColorF128);

        // UI
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(CursorInputEvent);
    }

} // namespace Maze
//////////////////////////////////////////
