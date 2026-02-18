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

            MonoHelper::IterateSerializableFields(monoClass,
                [&](ScriptFieldPtr const& _field)
                {
                    if (_field->isStatic())
                        return;

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

                    if (value.isEqual(identityValue))
                        return;

                    DataBlock* modificationBlock = _prefabBlock.addNewDataBlock(MAZE_HCS("modification"));
                    modificationBlock->setCString(MAZE_HCS("component"), MonoBehaviour::GetMetaClass()->getName().str);
                    modificationBlock->setCString(MAZE_HCS("_ct"), _component->getComponentClassName());

                    modificationBlock->setCString(MAZE_HCS("property"), _field->getName().c_str());
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



    } // namespace MonoSerializationHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
