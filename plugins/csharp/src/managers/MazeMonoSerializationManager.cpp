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
#include "maze-core/ecs/MazeEcsTypes.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/serialization/MazeDataBlockBinarySerialization.hpp"
#include "maze-core/serialization/MazeDataBlockTextSerialization.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/managers/MazeScriptableObjectManager.hpp"
#include "maze-plugin-csharp/mono/MazeScriptableObject.hpp"


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
            ScriptClassPtr monoByteBufferClass = ScriptClass::Create("Maze.Core", "ByteBuffer", MonoEngine::GetCoreAssemblyImage());
            MonoProperty* monoByteBufferSizeProperty = monoByteBufferClass->getProperty("Size");
            MonoProperty* monoByteBufferDataProperty = monoByteBufferClass->getProperty("Data");

            ScriptClassPtr monoDataBlockClass = ScriptClass::Create("Maze.Core", "DataBlock", MonoEngine::GetCoreAssemblyImage());
            MonoMethod* monoDataBlockLoadBytesMethod = monoDataBlockClass->getMethod("LoadBytes", 2);
            MonoMethod* monoDataBlockToByteBufferMethod = monoDataBlockClass->getMethod("ToByteBuffer", 0);

            auto dataBlockToMonoDataBlock =
                [monoDataBlockLoadBytesMethod](DataBlock const& _dataBlock)
                {
                    ByteBuffer buffer;
                    DataBlockBinarySerialization::SaveBinary(_dataBlock, buffer);
                    U32 bufferSize = buffer.getSize();
                    void* args[2] = { (void*)buffer.getDataRO(), (void*)&bufferSize };
                    return MonoHelper::InvokeMethod(nullptr, monoDataBlockLoadBytesMethod, args);
                };

            auto monoDataBlockToDataBlock =
                [monoDataBlockToByteBufferMethod,
                monoByteBufferSizeProperty,
                monoByteBufferDataProperty](MonoObject* _monoDataBlock, DataBlock& _dataBlock)
                {
                    _dataBlock.clear();

                    if (!_monoDataBlock)
                        return true;

                    MonoObject* monoByteBuffer = MonoHelper::InvokeMethod(_monoDataBlock, monoDataBlockToByteBufferMethod);
                    MonoObject* monoByteBufferSizeObject = mono_property_get_value(monoByteBufferSizeProperty, monoByteBuffer, nullptr, nullptr);
                    MAZE_ERROR_RETURN_VALUE_IF(!monoByteBufferSizeObject || !MonoHelper::IsValueType(monoByteBufferSizeObject), false, "Failed to get Size property");

                    U32 monoByteBufferSize = *(U32*)mono_object_unbox(monoByteBufferSizeObject);
                    if (monoByteBufferSize == 0)
                        return true;
                        
                    MonoArray* monoByteBufferDataObject = (MonoArray*)mono_property_get_value(monoByteBufferDataProperty, monoByteBuffer, nullptr, nullptr);
                    U8 const* arrayData = (U8 const*)mono_array_addr_with_size(monoByteBufferDataObject, sizeof(U8), 0);
                    ByteBuffer buffer(arrayData, monoByteBufferSize);
                    DataBlockBinarySerialization::LoadBinary(_dataBlock, buffer);
                    return true;
                };


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
                    MonoObject* listMonoObject = nullptr;
                    _instance.getPropertyValue(_prop, listMonoObject);
                    if (listMonoObject)
                    {
                        DataBlock* listDataBlock = _dataBlock.addNewDataBlock(_prop->getName());
                        MonoHelper::SerializeMonoObjectListToDataBlock(listMonoObject, *listDataBlock);
                    }
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    DataBlock const* listDataBlock = _dataBlock.getDataBlock(_prop->getName().asHashedCString());
                    if (listDataBlock)
                    {
                        MonoObject* listMonoObject = nullptr;
                        _instance.getPropertyValue(_prop, listMonoObject);
                        if (listMonoObject)
                            MonoHelper::DeserializeDataBlockToMonoObjectList(*listDataBlock, listMonoObject);
                    }
                    else
                    {
                        Debug::LogWarning("Not implemented - empty list object!");
                    }
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MonoObject* listMonoObject = nullptr;
                    _instance.getFieldValue(_field, listMonoObject);
                    if (listMonoObject)
                    {
                        DataBlock* listDataBlock = _dataBlock.addNewDataBlock(_field->getName());
                        MonoHelper::SerializeMonoObjectListToDataBlock(listMonoObject, *listDataBlock);
                    }
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    DataBlock const* listDataBlock = _dataBlock.getDataBlock(_field->getName().asHashedCString());
                    if (listDataBlock)
                    {
                        MonoObject* listMonoObject = nullptr;
                        _instance.getFieldValue(_field, listMonoObject);
                        if (listMonoObject)
                            MonoHelper::DeserializeDataBlockToMonoObjectList(*listDataBlock, listMonoObject);
                    }
                    else
                    {
                        Debug::LogWarning("Not implemented - empty list object!");
                    }
                });


            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("System.String[]"),
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    MAZE_ERROR("NOT IMPLEMENTED");
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    Vector<CString> strings = _dataBlock.getDataBlockAsVectorCString(_prop->getName());
                    MonoDomain* domain = mono_domain_get();
                    MonoArray* arr = mono_array_new(domain, mono_get_string_class(), strings.size());

                    for (Size i = 0, in = strings.size(); i != in; ++i)
                        mono_array_set(arr, MonoString*, i, mono_string_new(domain, strings[i]));

                    _instance.setPropertyValue(_prop, arr);
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MAZE_ERROR("NOT IMPLEMENTED");
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    Vector<CString> strings = _dataBlock.getDataBlockAsVectorCString(_field->getName());
                    MonoDomain* domain = mono_domain_get();
                    MonoArray* arr = mono_array_new(domain, mono_get_string_class(), strings.size());

                    for (Size i = 0, in = strings.size(); i != in; ++i)
                        mono_array_set(arr, MonoString*, i, mono_string_new(domain, strings[i]));

                    _instance.setFieldValue(_field, arr);
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
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Core.AssetFileId", U32);

            // AABB2D
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("Maze.Core.AABB2D"),
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    AABB2D value;
                    _instance.getPropertyValue(_prop, value);
                    ValueToDataBlock(value, *_dataBlock.ensureDataBlock(_prop->getName()));
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    AABB2D value;

                    if (DataBlock const* dataBlock = _dataBlock.getDataBlock(_prop->getName()))
                        ValueFromDataBlock(value, *dataBlock);
                   
                    _instance.setPropertyValue(_prop, value);
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    AABB2D value;
                    _instance.getFieldValue(_field, value);
                    ValueToDataBlock(value, *_dataBlock.ensureDataBlock(_field->getName()));
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    AABB2D value;

                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_field->getName());
                    if (dataBlock)
                        ValueFromDataBlock(value, *dataBlock);

                    _instance.setFieldValue(_field, value);
                });

            // AABB3D
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("Maze.Core.AABB3D"),
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    AABB3D value;
                    _instance.getPropertyValue(_prop, value);
                    ValueToDataBlock(value, *_dataBlock.ensureDataBlock(_prop->getName()));
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    AABB3D value;

                    if (DataBlock const* dataBlock = _dataBlock.getDataBlock(_prop->getName()))
                        ValueFromDataBlock(value, *dataBlock);

                    _instance.setPropertyValue(_prop, value);
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    AABB3D value;
                    _instance.getFieldValue(_field, value);
                    ValueToDataBlock(value, *_dataBlock.ensureDataBlock(_field->getName()));
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    AABB3D value;

                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_field->getName());
                    if (dataBlock)
                        ValueFromDataBlock(value, *dataBlock);

                    _instance.setFieldValue(_field, value);
                });


            // DataBlock
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("Maze.Core.DataBlock"),
                [monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    MonoObject* monoDataBlock = nullptr;
                    _instance.getPropertyValue(_prop, monoDataBlock);
                    monoDataBlockToDataBlock(monoDataBlock, *_dataBlock.ensureDataBlock(_prop->getName()));
                },
                [dataBlockToMonoDataBlock](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    MonoObject* monoDataBlock = nullptr;
                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_prop->getName());
                    if (dataBlock)
                        monoDataBlock = dataBlockToMonoDataBlock(*dataBlock);
                    _instance.setPropertyValue(_prop, monoDataBlock);
                },
                [monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MonoObject* monoDataBlock = nullptr;
                    _instance.getFieldValue(_field, monoDataBlock);
                    monoDataBlockToDataBlock(monoDataBlock, *_dataBlock.ensureDataBlock(_field->getName()));
                },
                [dataBlockToMonoDataBlock, monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    MonoObject* monoDataBlock = nullptr;
                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_field->getName());
                    if (dataBlock)
                        monoDataBlock = dataBlockToMonoDataBlock(*dataBlock);
                    _instance.setFieldValue(_field, monoDataBlock);
                });


            // SciptableObjects
            ScriptClassPtr const& scriptableObjectClass = MonoEngine::GetScriptableObjectClass();
            registerPropertyAndFieldDataBlockSubClassSerialization(
                scriptableObjectClass->getMonoClass(),
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    MonoObject* scriptableObjectInstance = nullptr;
                    _instance.getPropertyValue(_prop, scriptableObjectInstance);
                    MonoHelper::SerializeScriptableObjectToDataBlock(_dataBlock, _prop->getName(), scriptableObjectInstance);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(_prop->getName());
                    if (paramIndex >= 0)
                    {
                        DataBlock::Param const& param = _dataBlock.getParam(paramIndex);
                        if (param.type == U8(DataBlockParamType::ParamU32))
                        {
                            U32 auid = _dataBlock.getU32(paramIndex);
                            ScriptableObjectPtr const& scriptableObject = ScriptableObjectManager::GetInstancePtr()->getOrLoadScriptableObject(auid, true);
                            if (scriptableObject && scriptableObject->getScriptInstance())
                            {
                                MAZE_ERROR_IF(!scriptableObject->getScriptInstance()->getInstance(), "ScriptableObject is not loaded - %s!", scriptableObject->getName().c_str());
                                _instance.setPropertyValue(_prop, scriptableObject->getScriptInstance()->getInstance());
                            }
                            else
                                _instance.resetPropertyValue(_prop);
                        }
                        else
                        {
                            MAZE_ERROR("Unsupported type: %d", U8(param.type));
                        }
                    }
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MonoObject* scriptableObjectInstance = nullptr;
                    _instance.getFieldValue(_field, scriptableObjectInstance);
                    MonoHelper::SerializeScriptableObjectToDataBlock(_dataBlock, _field->getName(), scriptableObjectInstance);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(_field->getName());
                    if (paramIndex >= 0)
                    {
                        DataBlock::Param const& param = _dataBlock.getParam(paramIndex);
                        if (param.type == U8(DataBlockParamType::ParamU32))
                        {
                            U32 auid = _dataBlock.getU32(paramIndex);
                            ScriptableObjectPtr const& scriptableObject = ScriptableObjectManager::GetInstancePtr()->getOrLoadScriptableObject(auid, true);
                            if (scriptableObject && scriptableObject->getScriptInstance())
                            {
                                MAZE_ERROR_IF(!scriptableObject->getScriptInstance()->getInstance(), "ScriptableObject is not loaded - %s!", scriptableObject->getName().c_str());
                                _instance.setFieldValue(_field, scriptableObject->getScriptInstance()->getInstance());
                            }
                            else
                                _instance.resetFieldValue(_field);
                        }
                        else
                        {
                            MAZE_ERROR("Unsupported type: %d", U8(param.type));
                        }
                    }
                });


            // Graphics
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Graphics.ColorU32", Vec4U8);
            MAZE_MONO_SERIALIZATION_TYPE("Maze.Graphics.ColorF128", Vec4F);

            // Material
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("Maze.Graphics.Material"),
                [monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    MonoObject* indexedResourceInstance = nullptr;
                    _instance.getPropertyValue(_prop, indexedResourceInstance);
                    ResourceId resourceId = MonoHelper::GetIndexedResourceId(indexedResourceInstance);
                    if (Material* material = Material::GetResource(resourceId))
                        MaterialAssetRef(material).toDataBlock(*_dataBlock.ensureDataBlock(_prop->getName()));
                    
                },
                [dataBlockToMonoDataBlock](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_prop->getName());
                    if (dataBlock)
                    {
                        MaterialAssetRef materialAssetRef;
                        materialAssetRef.loadFromDataBlock(*dataBlock);
                        ResourceId resourceId = materialAssetRef.getMaterial() ? materialAssetRef.getMaterial()->getResourceId() : c_invalidResourceId;
                        MonoObject* indexedResourceObj = MonoHelper::CreateIndexedResource(MAZE_HCS("Maze.Graphics.Material"), resourceId);
                        _instance.setPropertyValue(_prop, indexedResourceObj);
                    }
                    else
                        _instance.resetPropertyValue(_prop);
                },
                [monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MonoObject* indexedResourceInstance = nullptr;
                    _instance.getFieldValue(_field, indexedResourceInstance);
                    ResourceId resourceId = MonoHelper::GetIndexedResourceId(indexedResourceInstance);
                    if (Material* material = Material::GetResource(resourceId))
                        MaterialAssetRef(material).toDataBlock(*_dataBlock.ensureDataBlock(_field->getName()));
                },
                [dataBlockToMonoDataBlock, monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_field->getName());
                    if (dataBlock)
                    {
                        MaterialAssetRef materialAssetRef;
                        materialAssetRef.loadFromDataBlock(*dataBlock);
                        ResourceId resourceId = materialAssetRef.getMaterial() ? materialAssetRef.getMaterial()->getResourceId() : c_invalidResourceId;
                        MonoObject* indexedResourceObj = MonoHelper::CreateIndexedResource(MAZE_HCS("Maze.Graphics.Material"), resourceId);
                        _instance.setFieldValue(_field, indexedResourceObj);
                    }
                    else
                        _instance.resetFieldValue(_field);
                });

            // Texture2D
            registerPropertyAndFieldDataBlockSerialization(MAZE_HCS("Maze.Graphics.Texture2D"),
                [monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    MonoObject* indexedResourceInstance = nullptr;
                    _instance.getPropertyValue(_prop, indexedResourceInstance);
                    ResourceId resourceId = MonoHelper::GetIndexedResourceId(indexedResourceInstance);
                    if (Texture2D* texture = Texture2D::GetResource(resourceId))
                        Texture2DAssetRef(texture).toDataBlock(*_dataBlock.ensureDataBlock(_prop->getName()));

                },
                [dataBlockToMonoDataBlock](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_prop->getName());
                    if (dataBlock)
                    {
                        Texture2DAssetRef textureAssetRef;
                        textureAssetRef.loadFromDataBlock(*dataBlock);
                        ResourceId resourceId = textureAssetRef.getTexture2D() ? textureAssetRef.getTexture2D()->getResourceId() : c_invalidResourceId;
                        MonoObject* indexedResourceObj = MonoHelper::CreateIndexedResource(MAZE_HCS("Maze.Graphics.Texture2D"), resourceId);
                        _instance.setPropertyValue(_prop, indexedResourceObj);
                    }
                    else
                        _instance.resetPropertyValue(_prop);
                },
                [monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    MonoObject* indexedResourceInstance = nullptr;
                    _instance.getFieldValue(_field, indexedResourceInstance);
                    ResourceId resourceId = MonoHelper::GetIndexedResourceId(indexedResourceInstance);
                    if (Texture2D* texture = Texture2D::GetResource(resourceId))
                        Texture2DAssetRef(texture).toDataBlock(*_dataBlock.ensureDataBlock(_field->getName()));
                },
                [dataBlockToMonoDataBlock, monoDataBlockToDataBlock](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    DataBlock const* dataBlock = _dataBlock.getDataBlock(_field->getName());
                    if (dataBlock)
                    {
                        Texture2DAssetRef textureAssetRef;
                        textureAssetRef.loadFromDataBlock(*dataBlock);
                        ResourceId resourceId = textureAssetRef.getTexture2D() ? textureAssetRef.getTexture2D()->getResourceId() : c_invalidResourceId;
                        MonoObject* indexedResourceObj = MonoHelper::CreateIndexedResource(MAZE_HCS("Maze.Graphics.Texture2D"), resourceId);
                        _instance.setFieldValue(_field, indexedResourceObj);
                    }
                    else
                        _instance.resetFieldValue(_field);
                });

#undef MAZE_MONO_SERIALIZATION_TYPE



            // Components
            ScriptClassPtr const& componentClass = MonoEngine::GetComponentClass();
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
                        _instance.setPropertyValue(_prop, result);
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
                        _instance.setFieldValue(_field, result);
                    else
                        _instance.resetFieldValue(_field);
                });

            // System.Enum
            registerPropertyAndFieldDataBlockEnumSerialization(
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)
                {
                    S32 value;
                    _instance.getPropertyValue(_prop, value);
                    _dataBlock.setS32(_prop->getName(), value);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)
                {
                    DataBlock::ParamIndex idx = _dataBlock.findParamIndex(_prop->getName());
                    if (idx >= 0)
                        _instance.setPropertyValue(_prop, _dataBlock.getS32(idx));
                },
                [](EcsWorld* _world, ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
                {
                    S32 value;
                    _instance.getFieldValue(_field, value);
                    _dataBlock.setS32(_field->getName(), value);
                },
                [](EcsWorld* _world, ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
                {
                    DataBlock::ParamIndex idx = _dataBlock.findParamIndex(_field->getName());
                    if (idx >= 0)
                        _instance.setFieldValue(_field, _dataBlock.getS32(idx));
                });


            registerMonoObjectSerializationFunctions();
        }
        else
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            m_propertyDataBlockSerializationData.clear();
            m_fieldDataBlockSerializationData.clear();
            m_propertyDataBlockSubClassSerializationData.clear();
            m_fieldDataBlockSubClassSerializationData.clear();

            m_serializeMonoObjectToDataBlockFunctions.clear();
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
        if (_property->isGenericType())
        {
            CStringSpan monoTypeBaseName = MonoHelper::GetMonoGenericClassBaseName(_property->getTypeName().c_str());
            if (monoTypeBaseName.empty())
                return false;

            Char monoTypeBaseNameBuffer[256];
            memcpy_s(monoTypeBaseNameBuffer, sizeof(monoTypeBaseNameBuffer), monoTypeBaseName.ptr(), monoTypeBaseName.size());
            monoTypeBaseNameBuffer[monoTypeBaseName.size()] = 0;
            HashedCString monoTypeBaseNameHCS(monoTypeBaseNameBuffer);

            auto it = m_propertyDataBlockSerializationData.find(monoTypeBaseNameHCS);
            if (it == m_propertyDataBlockSerializationData.end())
                return false;

            it->second.propToDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
            return true;
        }
        else
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

                    if (mono_class_is_enum(monoClass))
                    {
                        m_propertyDataBlockEnumSerializationData.propToDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
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
        if (_property->isGenericType())
        {
            CStringSpan monoTypeBaseName = MonoHelper::GetMonoGenericClassBaseName(_property->getTypeName().c_str());
            if (monoTypeBaseName.empty())
                return false;

            Char monoTypeBaseNameBuffer[256];
            memcpy_s(monoTypeBaseNameBuffer, sizeof(monoTypeBaseNameBuffer), monoTypeBaseName.ptr(), monoTypeBaseName.size());
            monoTypeBaseNameBuffer[monoTypeBaseName.size()] = 0;
            HashedCString monoTypeBaseNameHCS(monoTypeBaseNameBuffer);

            auto it = m_propertyDataBlockSerializationData.find(monoTypeBaseNameHCS);
            if (it == m_propertyDataBlockSerializationData.end())
                return false;

            it->second.propFromDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
            return true;
        }
        else
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

                    if (mono_class_is_enum(monoClass))
                    {
                        m_propertyDataBlockEnumSerializationData.propFromDataBlockCb(_ecsWorld, _instance, _property, _dataBlock);
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
            CStringSpan monoTypeBaseName = MonoHelper::GetMonoGenericClassBaseName(_field->getTypeName().c_str());
            if (monoTypeBaseName.empty())
                return false;

            Char monoTypeBaseNameBuffer[256];
            memcpy_s(monoTypeBaseNameBuffer, sizeof(monoTypeBaseNameBuffer), monoTypeBaseName.ptr(), monoTypeBaseName.size());
            monoTypeBaseNameBuffer[monoTypeBaseName.size()] = 0;
            HashedCString monoTypeBaseNameHCS(monoTypeBaseNameBuffer);

            auto it = m_fieldDataBlockSerializationData.find(monoTypeBaseNameHCS);
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

                    if (mono_class_is_enum(monoClass))
                    {
                        m_fieldDataBlockEnumSerializationData.propToDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
                        return true;
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
        if (_field->isGenericType())
        {
            CStringSpan monoTypeBaseName = MonoHelper::GetMonoGenericClassBaseName(_field->getTypeName().c_str());
            if (monoTypeBaseName.empty())
                return false;

            Char monoTypeBaseNameBuffer[256];
            memcpy_s(monoTypeBaseNameBuffer, sizeof(monoTypeBaseNameBuffer), monoTypeBaseName.ptr(), monoTypeBaseName.size());
            monoTypeBaseNameBuffer[monoTypeBaseName.size()] = 0;
            HashedCString monoTypeBaseNameHCS(monoTypeBaseNameBuffer);

            auto it = m_fieldDataBlockSerializationData.find(monoTypeBaseNameHCS);
            if (it == m_fieldDataBlockSerializationData.end())
                return false;

            it->second.propFromDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
            return true;
        }
        else
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

                    if (mono_class_is_enum(monoClass))
                    {
                        m_fieldDataBlockEnumSerializationData.propFromDataBlockCb(_ecsWorld, _instance, _field, _dataBlock);
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
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(EntityId);

        // Graphics
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(ColorU32);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(ColorF128);

        // UI
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(CursorInputEvent);
        REGISTER_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD_FUNCTION(CursorWheelInputEvent);
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerMonoObjectSerializationFunctions()
    {
        auto registerMonoObjectSerializationFunction = 
            [&](
                MonoImage* _image,
                CString _namespaceName,
                CString _typeName,
                SerializeMonoObjectToDataBlockFunc _func)
            {
                MonoClass* monoClass = mono_class_from_name(_image, _namespaceName, _typeName);
                MAZE_ERROR_RETURN_IF(!monoClass, "Undefined type - %s.%s", _namespaceName, _typeName);
                m_serializeMonoObjectToDataBlockFunctions[monoClass] = _func;
            };

        auto registerGenericMonoObjectSerializationFunction =
            [&](
                HashedCString _genericFullName,
                SerializeMonoObjectToDataBlockFunc _func)
            {
                m_serializeGenericMonoObjectToDataBlockFunctions.insert(_genericFullName, _func);
            };

        // System
        MonoImage* systemCoreImage = mono_get_corlib();
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "String", MonoHelper::SerializeMonoObjectStringToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Char", MonoHelper::SerializeMonoObjectU16ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Boolean", MonoHelper::SerializeMonoObjectBoolToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "SByte", MonoHelper::SerializeMonoObjectS8ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Int16", MonoHelper::SerializeMonoObjectS16ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Int32", MonoHelper::SerializeMonoObjectS32ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Int64", MonoHelper::SerializeMonoObjectS64ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Byte", MonoHelper::SerializeMonoObjectU8ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "UInt16", MonoHelper::SerializeMonoObjectU16ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "UInt32", MonoHelper::SerializeMonoObjectU32ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "UInt64", MonoHelper::SerializeMonoObjectU64ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Single", MonoHelper::SerializeMonoObjectF32ToDataBlock);
        registerMonoObjectSerializationFunction(systemCoreImage, "System", "Double", MonoHelper::SerializeMonoObjectF64ToDataBlock);

        registerGenericMonoObjectSerializationFunction(MAZE_HCS("System.Collections.Generic.List"), MonoHelper::SerializeMonoObjectListToDataBlock);


        // Maze.Core
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec2F", MonoHelper::SerializeMonoObjectVec2FToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec3F", MonoHelper::SerializeMonoObjectVec3FToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec4F", MonoHelper::SerializeMonoObjectVec4FToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec2S", MonoHelper::SerializeMonoObjectVec2SToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec3S", MonoHelper::SerializeMonoObjectVec3SToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec4S", MonoHelper::SerializeMonoObjectVec4SToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec2U", MonoHelper::SerializeMonoObjectVec2UToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec3U", MonoHelper::SerializeMonoObjectVec3UToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec4U", MonoHelper::SerializeMonoObjectVec4UToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Mat3F", MonoHelper::SerializeMonoObjectMat3FToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Mat4F", MonoHelper::SerializeMonoObjectMat4FToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "TMat", MonoHelper::SerializeMonoObjectTMatToDataBlock);
        registerMonoObjectSerializationFunction(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Rect2F", MonoHelper::SerializeMonoObjectRect2FToDataBlock);
    }

    //////////////////////////////////////////
    void MonoSerializationManager::serializeMonoObjectToDataBlock(
        MonoObject* _object,
        DataBlock& _dataBlock)
    {
        if (!_object)
            return;

        MonoClass* objectClass = mono_object_get_class(_object);
        auto it = m_serializeMonoObjectToDataBlockFunctions.find(objectClass);
        if (it != m_serializeMonoObjectToDataBlockFunctions.end())
        {
            it->second(_object, _dataBlock);
        }
        else
        {
            MonoType* monoType = mono_class_get_byref_type(objectClass);
            S32 monoTypeType = mono_type_get_type(monoType);

            if (monoTypeType == MONO_TYPE_GENERICINST)
            {
                // CString monoClassName = mono_class_get_name(objectClass);
                CString monoTypeName = mono_type_get_name(monoType);

                CStringSpan monoTypeBaseName = MonoHelper::GetMonoGenericClassBaseName(monoTypeName);
                MAZE_ERROR_RETURN_IF(monoTypeBaseName.size() == 0, "Invalid generic class - %s", monoTypeName);

                Char monoTypeBaseNameBuffer[256];
                memcpy_s(monoTypeBaseNameBuffer, sizeof(monoTypeBaseNameBuffer), monoTypeBaseName.ptr(), monoTypeBaseName.size());
                monoTypeBaseNameBuffer[monoTypeBaseName.size()] = 0;
                HashedCString monoTypeBaseNameHCS(monoTypeBaseNameBuffer);
                
                auto it2 = m_serializeGenericMonoObjectToDataBlockFunctions.find(monoTypeBaseNameHCS);
                if (it2 != m_serializeGenericMonoObjectToDataBlockFunctions.end())
                {
                    it2->second(_object, _dataBlock);
                }
                else
                {
                    MAZE_WARNING("Unsupported generic type serialization - %s!", monoTypeBaseNameBuffer);
                }
            }
        }
    }

    //////////////////////////////////////////
    void MonoSerializationManager::deserializeDataBlockToMonoObject(
        DataBlock const& _dataBlock,
        MonoObject* _object)
    {

    }

    //////////////////////////////////////////
    MonoObject* MonoSerializationManager::createMonoObjectFromDataBlock(DataBlock const& _dataBlock)
    {
        if (_dataBlock.getParamsCount() == 1)
        {
            DataBlock::Param const& paramData = _dataBlock.getParam(0);
            switch ((DataBlockParamType)paramData.type)
            {
                case DataBlockParamType::ParamVec2F32:
                {
                    MonoClass* monoClass = mono_class_from_name(MonoEngine::GetCoreAssemblyImage(), "Maze.Core", "Vec2F");
                    return mono_value_box(mono_get_root_domain(), monoClass, &_dataBlock.getVec2F(0));
                }
                case DataBlockParamType::ParamString:
                {
                    CString str = _dataBlock.getCString(0);
                    return (MonoObject*)mono_string_new(mono_domain_get(), str);
                }
                default:
                {
                    MAZE_ERROR("Unsupported param type - %d", (S32)paramData.type);
                    break;
                }
            }
        }
        else
        if (_dataBlock.getDataBlocksCount() == 1)
        {
            MAZE_NOT_IMPLEMENTED;
        }
        else
        {
            MAZE_WARNING("Invalid data block! params count = %d, blocks count = %d", _dataBlock.getParamsCount(), _dataBlock.getDataBlocksCount());
        }

        return nullptr;
    }

} // namespace Maze
//////////////////////////////////////////
