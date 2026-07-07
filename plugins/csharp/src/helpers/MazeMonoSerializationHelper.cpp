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
#include "maze-plugin-csharp/helpers/MazeMonoSerializationHelper.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-plugin-csharp/mono/MazeScriptField.hpp"
#include "maze-plugin-csharp/mono/MazeScriptableObject.hpp"
#include "maze-plugin-csharp/managers/MazeScriptableObjectManager.hpp"
#include "maze-plugin-csharp/managers/MazeMonoSerializationManager.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-core/serialization/MazeDataBlockTextSerialization.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/ecs/events/MazeEcsInputEvents.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MonoSerializationHelper
    {
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeMonoBehaviourToDataBlock(EntitiesToDataBlockContext& _context, ComponentPtr const& _component, DataBlock& _componentBlock)
        {
            _componentBlock.setS32(MAZE_HCS("_sid"), _component->getSerializationId());

            MonoBehaviour* monoBehaviour = _component->castRaw<MonoBehaviour>();
            auto const& monoClass = monoBehaviour->getMonoClass();
            if (!monoClass)
                return;

            auto const& monoInstance = monoBehaviour->getMonoInstance();
            if (!monoInstance)
                return;

            EcsWorld* ecsWorld = _component->getEntityRaw()->getEcsWorld();

            MonoHelper::IterateSerializableFields(monoClass,
                [&](ScriptFieldPtr const& _field)
                {
                    if (_field->isStatic())
                        return;

                    MonoSerializationManager::GetInstancePtr()->saveFieldToDataBlock(
                        ecsWorld,
                        *monoInstance,
                        _field,
                        _componentBlock);
                });

            MonoHelper::IterateSerializableProperties(monoClass,
                [&](ScriptPropertyPtr const& _prop)
                {
                    if (_prop->isStaticGetter())
                        return;

                    MonoSerializationManager::GetInstancePtr()->savePropertyToDataBlock(
                        ecsWorld,
                        *monoInstance,
                        _prop,
                        _componentBlock);
                });
        }

        //////////////////////////////////////////
        // Serialized values contain runtime eids that never match across the live and
        // identity prefab worlds - when the raw compare fails, compare again with eids
        // normalized to prefab-root-relative sid paths
        static bool IsModificationValueEqual(
            DataBlock const& _value,
            DataBlock const& _identityValue,
            Entity* _rootEntity,
            Entity* _identityRootEntity)
        {
            if (_value.isEqual(_identityValue))
                return true;

            DataBlock normalizedValue;
            normalizedValue.copyFrom(&_value);
            EcsHelper::NormalizeEntityIdBlocksToPrefabSpace(
                normalizedValue,
                _rootEntity,
                _rootEntity->getEcsWorld());

            DataBlock normalizedIdentityValue;
            normalizedIdentityValue.copyFrom(&_identityValue);
            EcsHelper::NormalizeEntityIdBlocksToPrefabSpace(
                normalizedIdentityValue,
                _identityRootEntity,
                _identityRootEntity->getEcsWorld());

            return normalizedValue.isEqual(normalizedIdentityValue);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeMonoBehaviourModificationsToDataBlock(EntitiesToDataBlockContext& _context, ComponentPtr const& _component, ComponentPtr const& _identityComponent, DataBlock& _prefabBlock)
        {
            MonoBehaviour* monoBehaviour = _component->castRaw<MonoBehaviour>();
            auto const& monoClass = monoBehaviour->getMonoClass();
            if (!monoClass)
                return;

            auto const& monoInstance = monoBehaviour->getMonoInstance();
            if (!monoInstance)
                return;

            MonoBehaviour* identityMonoBehaviour = _identityComponent->castRaw<MonoBehaviour>();
            auto const& identityMonoClass = identityMonoBehaviour->getMonoClass();
            if (!identityMonoClass)
                return;

            auto const& identityMonoInstance = identityMonoBehaviour->getMonoInstance();
            if (!identityMonoInstance)
                return;

            MAZE_ERROR_RETURN_IF(monoClass != identityMonoClass, "Different mono classes!");

            EcsWorld* ecsWorld = _component->getEntityRaw()->getEcsWorld();

            // Fields
            MonoHelper::IterateSerializableFields(monoClass,
                [&](ScriptFieldPtr const& _field)
                {
                    if (_field->isStatic())
                        return;

                    // #TODO: for debug (remove later)
                    /*
                    MonoClass* fieldMonoClass = mono_class_from_mono_type(_field->getMonoType());
                    if (fieldMonoClass)
                    {
                        if (fieldMonoClass == MonoEngine::GetEntityClass()->getMonoClass())
                        {
                            MonoObject* entityInstance0 = nullptr;
                            monoInstance->getFieldValue(_field, entityInstance0);

                            MonoObject* entityInstance1 = nullptr;
                            identityMonoInstance->getFieldValue(_field, entityInstance1);

                            // #TODO: Ignore entity eid modifications for now
                            if (entityInstance0 && entityInstance1)
                            {
                                int a = 0;
                                // return;
                            }
                        }
                        else
                        if (mono_class_is_subclass_of(fieldMonoClass, MonoEngine::GetComponentClass()->getMonoClass(), false))
                        {
                            MonoObject* componentInstance0 = nullptr;
                            monoInstance->getFieldValue(_field, componentInstance0);

                            MonoObject* componentInstance1 = nullptr;
                            identityMonoInstance->getFieldValue(_field, componentInstance1);

                            // #TODO: Ignore component eid modifications for now
                            if (componentInstance0 && componentInstance1)
                            {
                                int a = 0;
                                // return;
                            }
                        }
                        else
                        if (mono_type_get_type(_field->getMonoType()) == MONO_TYPE_GENERICINST)
                        {
                            CString monoTypeName = mono_type_get_name(_field->getMonoType());

                            Char monoTypeBaseNameBuffer[256];
                            if (!MonoHelper::GetMonoGenericClassBaseName(
                                monoTypeName,
                                monoTypeBaseNameBuffer,
                                sizeof(monoTypeBaseNameBuffer)))
                            {
                                return;
                            }

                            if (strcmp(monoTypeBaseNameBuffer, "System.Collections.Generic.List") == 0)
                            {
                                CStringSpan monoTypeArgBaseName = MonoHelper::GetMonoGenericClassFirstGenericArgumentName(monoTypeName);
                                MAZE_ERROR_RETURN_IF(monoTypeArgBaseName.size() == 0, "Invalid generic class - %s", monoTypeName);

                                Char monoTypeArgBaseNameBuffer[256];
                                memcpy_s(monoTypeArgBaseNameBuffer, sizeof(monoTypeArgBaseNameBuffer), monoTypeArgBaseName.ptr(), monoTypeArgBaseName.size());
                                monoTypeArgBaseNameBuffer[monoTypeArgBaseName.size()] = 0;
                                HashedCString monoTypeArgBaseNameHCS(monoTypeArgBaseNameBuffer);

                                // #TODO: Ignore component eid modifications for now
                                if (MonoEngine::GetMonoBehaviourSubClass(monoTypeArgBaseNameHCS))
                                {
                                    int a = 0;
                                    // return;
                                }
                            }
                        }
                    }
                    */

                    DataBlock value;
                    MonoSerializationManager::GetInstancePtr()->saveFieldToDataBlock(
                        ecsWorld,
                        *monoInstance,
                        _field,
                        value);

                    DataBlock identityValue;
                    MonoSerializationManager::GetInstancePtr()->saveFieldToDataBlock(
                        ecsWorld,
                        *identityMonoInstance,
                        _field,
                        identityValue);

                    if (IsModificationValueEqual(
                        value,
                        identityValue,
                        monoBehaviour->getEntityRaw(),
                        identityMonoBehaviour->getEntityRaw()))
                        return;

                    DataBlock* modificationBlock = _prefabBlock.addNewDataBlock(MAZE_HCS("modification"));
                    modificationBlock->setCString(MAZE_HCS("component"), MonoBehaviour::GetMetaClass()->getName().str);
                    modificationBlock->setCString(MAZE_HCS("_ct"), _component->getComponentClassName());

                    modificationBlock->setCString(MAZE_HCS("property"), _field->getName().c_str());
                    modificationBlock->addNewDataBlock(&value, MAZE_HCS("value"));
                });

            // Properties
            MonoHelper::IterateSerializableProperties(monoClass,
                [&](ScriptPropertyPtr const& _property)
                {
                    if (_property->isStaticGetter() || _property->isStaticSetter())
                        return;

                    // #TODO: for debug (remove later)
                    /*
                    MonoClass* fieldMonoClass = mono_class_from_mono_type(_property->getMonoType());
                    if (fieldMonoClass)
                    {
                        if (fieldMonoClass == MonoEngine::GetEntityClass()->getMonoClass())
                        {
                            MonoObject* entityInstance0 = nullptr;
                            monoInstance->getPropertyValue(_property, entityInstance0);

                            MonoObject* entityInstance1 = nullptr;
                            identityMonoInstance->getPropertyValue(_property, entityInstance1);

                            // #TODO: Ignore entity eid modifications for now
                            if (entityInstance0 && entityInstance1)
                            {
                                int a = 0;
                                // return;
                            }
                        }
                        else
                        if (mono_class_is_subclass_of(fieldMonoClass, MonoEngine::GetComponentClass()->getMonoClass(), false))
                        {
                            MonoObject* componentInstance0 = nullptr;
                            monoInstance->getPropertyValue(_property, componentInstance0);

                            MonoObject* componentInstance1 = nullptr;
                            identityMonoInstance->getPropertyValue(_property, componentInstance1);

                            // #TODO: Ignore component eid modifications for now
                            if (componentInstance0 && componentInstance1)
                            {
                                int a = 0;
                                // return;
                            }
                        }
                        else
                        if (mono_type_get_type(_property->getMonoType()) == MONO_TYPE_GENERICINST)
                        {
                            CString monoTypeName = mono_type_get_name(_property->getMonoType());

                            Char monoTypeBaseNameBuffer[256];
                            if (!MonoHelper::GetMonoGenericClassBaseName(
                                monoTypeName,
                                monoTypeBaseNameBuffer,
                                sizeof(monoTypeBaseNameBuffer)))
                            {
                                return;
                            }

                            if (strcmp(monoTypeBaseNameBuffer, "System.Collections.Generic.List") == 0)
                            {
                                CStringSpan monoTypeArgBaseName = MonoHelper::GetMonoGenericClassFirstGenericArgumentName(monoTypeName);
                                MAZE_ERROR_RETURN_IF(monoTypeArgBaseName.size() == 0, "Invalid generic class - %s", monoTypeName);

                                Char monoTypeArgBaseNameBuffer[256];
                                memcpy_s(monoTypeArgBaseNameBuffer, sizeof(monoTypeArgBaseNameBuffer), monoTypeArgBaseName.ptr(), monoTypeArgBaseName.size());
                                monoTypeArgBaseNameBuffer[monoTypeArgBaseName.size()] = 0;
                                HashedCString monoTypeArgBaseNameHCS(monoTypeArgBaseNameBuffer);

                                // #TODO: Ignore component eid modifications for now
                                if (MonoEngine::GetMonoBehaviourSubClass(monoTypeArgBaseNameHCS))
                                {
                                    int a = 0;
                                    // return;
                                }
                            }
                        }
                    }
                    */

                    DataBlock value;
                    MonoSerializationManager::GetInstancePtr()->savePropertyToDataBlock(
                        ecsWorld,
                        *monoInstance,
                        _property,
                        value);

                    DataBlock identityValue;
                    MonoSerializationManager::GetInstancePtr()->savePropertyToDataBlock(
                        ecsWorld,
                        *identityMonoInstance,
                        _property,
                        identityValue);

                    if (IsModificationValueEqual(
                        value,
                        identityValue,
                        monoBehaviour->getEntityRaw(),
                        identityMonoBehaviour->getEntityRaw()))
                        return;

                    DataBlock* modificationBlock = _prefabBlock.addNewDataBlock(MAZE_HCS("modification"));
                    modificationBlock->setCString(MAZE_HCS("component"), MonoBehaviour::GetMetaClass()->getName().str);
                    modificationBlock->setCString(MAZE_HCS("_ct"), _component->getComponentClassName());

                    modificationBlock->setCString(MAZE_HCS("property"), _property->getName().c_str());
                    modificationBlock->addNewDataBlock(&value, MAZE_HCS("value"));
                });
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void DeserializeMonoBehaviourFromDataBlock(DataBlock const& _componentBlock, ComponentPtr const& _component, EntitiesFromDataBlockContext& _context)
        {
            _component->setSerializationId(_componentBlock.getS32(MAZE_HCS("_sid"), c_invalidSerializationId));

            MonoBehaviour* monoBehaviour = _component->castRaw<MonoBehaviour>();
            auto const& monoClass = monoBehaviour->getMonoClass();
            if (!monoClass)
                return;

            auto const& monoInstance = monoBehaviour->getMonoInstance();
            if (!monoInstance)
                return;

            EcsWorld* ecsWorld = _component->getEntityRaw()->getEcsWorld();

            MonoHelper::IterateSerializableFields(monoClass,
                [&](ScriptFieldPtr const& _field)
                {
                    if (_field->isStatic())
                        return;

                    MonoSerializationManager::GetInstancePtr()->loadFieldFromDataBlock(
                        ecsWorld,
                        *monoInstance,
                        _field,
                        _componentBlock);
                });

            MonoHelper::IterateSerializableProperties(monoClass,
                [&](ScriptPropertyPtr const& _prop)
                {
                    if (_prop->isStaticGetter())
                        return;

                    MonoSerializationManager::GetInstancePtr()->loadPropertyFromDataBlock(
                        ecsWorld,
                        *monoInstance,
                        _prop,
                        _componentBlock);
                });
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void DeserializeMonoBehaviourModificaionFromDataBlock(DataBlock const& _modificationBlock, ComponentPtr const& _component, CString _propertyName, EntitiesFromDataBlockContext& _context)
        {
            MonoBehaviour* monoBehaviour = _component->castRaw<MonoBehaviour>();
            auto const& monoClass = monoBehaviour->getMonoClass();
            if (!monoClass)
                return;

            auto const& monoInstance = monoBehaviour->getMonoInstance();
            if (!monoInstance)
                return;

            EcsWorld* ecsWorld = _component->getEntityRaw()->getEcsWorld();

            DataBlock const* valueBlock = _modificationBlock.getDataBlock(MAZE_HCS("value"));
            MAZE_WARNING_RETURN_IF(!valueBlock, "MonoBehaviour modification without value block! property=%s", _propertyName);

            HashedCString propertyName(_propertyName);

            if (ScriptFieldPtr const& field = monoClass->getField(propertyName))
            {
                if (!field->isStatic() && field->isSerializable())
                {
                    MonoSerializationManager::GetInstancePtr()->loadFieldFromDataBlock(
                        ecsWorld,
                        *monoInstance,
                        field,
                        *valueBlock);
                }
            }
            else
            if (ScriptPropertyPtr const& prop = monoClass->getProperty(propertyName))
            {
                if (!prop->isStaticGetter() && prop->isSerializable())
                {
                    MonoSerializationManager::GetInstancePtr()->loadPropertyFromDataBlock(
                        ecsWorld,
                        *monoInstance,
                        prop,
                        *valueBlock);
                }
            }
        }


        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void ClearMonoListObject(MonoObject* _listMonoObject)
        {
            if (!_listMonoObject)
                return;

            MonoClass* listMonoClass = mono_object_get_class(_listMonoObject);
            MonoMethod* clearMethod = mono_class_get_method_from_name(listMonoClass, "Clear", 0);
            if (clearMethod)
                MonoHelper::InvokeMethod(_listMonoObject, clearMethod, nullptr);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* CreateMonoListObject(MonoType* _listMonoType)
        {
            if (!_listMonoType)
                return nullptr;

            MonoClass* listMonoClass = mono_class_from_mono_type(_listMonoType);
            if (!listMonoClass)
                return nullptr;

            MonoObject* listMonoObject = mono_object_new(mono_domain_get(), listMonoClass);
            if (!listMonoObject)
                return nullptr;

            // Invoke the default ctor
            mono_runtime_object_init(listMonoObject);
            return listMonoObject;
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeMonoStringArrayToDataBlock(
            MonoObject* _arrayMonoObject,
            HashedCString _name,
            DataBlock& _dataBlock)
        {
            if (!_arrayMonoObject)
                return;

            MonoArray* monoArray = (MonoArray*)_arrayMonoObject;
            Size arrayLength = (Size)mono_array_length(monoArray);

            // Own the utf8 copies for the duration of the write
            Vector<String> strings;
            strings.reserve(arrayLength);
            for (Size i = 0; i < arrayLength; ++i)
            {
                MonoString* monoString = mono_array_get(monoArray, MonoString*, i);
                if (monoString)
                {
                    Char* cstr = mono_string_to_utf8(monoString);
                    strings.push_back(String(cstr));
                    mono_free(cstr);
                }
                else
                    strings.push_back(String());
            }

            Vector<CString> cstrings;
            cstrings.reserve(strings.size());
            for (String const& text : strings)
                cstrings.push_back(text.c_str());

            _dataBlock.setDataBlockAsVectorCString(_name, cstrings);
        }


    } // namespace MonoSerializationHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
