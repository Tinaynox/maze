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
#if (!defined(_MazeMonoSerializationManager_hpp_))
#define _MazeMonoSerializationManager_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/events/MazeEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MonoSerializationManager);
    MAZE_USING_SHARED_PTR(EcsWorld);

    //////////////////////////////////////////
    using WriteMetaPropertyToMonoClassFieldFunction = std::function<void(
        ConstMetaInstance const&,
        MetaProperty const*,
        MonoObject*,
        MonoClassField*)>;


    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API ScriptPropertyDataBlockSerializationData
    {
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> propToDataBlockCb;
        std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> propFromDataBlockCb;
    };

    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API ScriptFieldDataBlockSerializationData
    {
        std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> propToDataBlockCb;
        std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> propFromDataBlockCb;
    };


    //////////////////////////////////////////
    // Class MonoSerializationManager
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API MonoSerializationManager
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~MonoSerializationManager();

        //////////////////////////////////////////
        static void Initialize(
            MonoSerializationManagerPtr& _manager);


        //////////////////////////////////////////
        static inline MonoSerializationManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline MonoSerializationManager& GetInstance() { return *s_instance; }



        //////////////////////////////////////////
        void registerPropertyDataBlockSerialization(
            HashedCString _typeName,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propFromDataBlockCb);

        //////////////////////////////////////////
        void registerPropertyDataBlockSubClassSerialization(
            MonoClass* _monoClass,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propFromDataBlockCb);

        //////////////////////////////////////////
        bool savePropertyToDataBlock(
            EcsWorld* _ecsWorld,
            ScriptInstance const& _instance,
            ScriptPropertyPtr const& _property,
            DataBlock& _dataBlock);

        //////////////////////////////////////////
        bool loadPropertyFromDataBlock(
            EcsWorld* _ecsWorld,
            ScriptInstance& _instance,
            ScriptPropertyPtr const& _property,
            DataBlock const& _dataBlock);


        //////////////////////////////////////////
        void registerFieldDataBlockSerialization(
            HashedCString _typeName,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _propToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _propFromDataBlockCb);

        //////////////////////////////////////////
        void registerFieldDataBlockSubClassSerialization(
            MonoClass* _monoClass,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _propToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _propFromDataBlockCb);

        //////////////////////////////////////////
        bool saveFieldToDataBlock(
            EcsWorld* _ecsWorld,
            ScriptInstance const& _instance,
            ScriptFieldPtr const& _field,
            DataBlock& _dataBlock);

        //////////////////////////////////////////
        bool loadFieldFromDataBlock(
            EcsWorld* _ecsWorld,
            ScriptInstance& _instance,
            ScriptFieldPtr const& _field,
            DataBlock const& _dataBlock);



        //////////////////////////////////////////
        inline void registerPropertyAndFieldDataBlockSerialization(
            HashedCString _typeName,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propertyPropToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propertyPropFromDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _fieldPropToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _fieldPropFromDataBlockCb)
        {
            registerPropertyDataBlockSerialization(_typeName, _propertyPropToDataBlockCb, _propertyPropFromDataBlockCb);
            registerFieldDataBlockSerialization(_typeName, _fieldPropToDataBlockCb, _fieldPropFromDataBlockCb);
        }

        //////////////////////////////////////////
        inline void registerPropertyAndFieldDataBlockSubClassSerialization(
            MonoClass* _monoClass,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propertyPropToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propertyPropFromDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _fieldPropToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _fieldPropFromDataBlockCb)
        {
            registerPropertyDataBlockSubClassSerialization(_monoClass, _propertyPropToDataBlockCb, _propertyPropFromDataBlockCb);
            registerFieldDataBlockSubClassSerialization(_monoClass, _fieldPropToDataBlockCb, _fieldPropFromDataBlockCb);
        }

        //////////////////////////////////////////
        inline void registerPropertyAndFieldDataBlockEnumSerialization(
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propertyPropToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propertyPropFromDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _fieldPropToDataBlockCb,
            std::function<void(EcsWorld*, ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _fieldPropFromDataBlockCb)
        {
            m_propertyDataBlockEnumSerializationData.propToDataBlockCb = _propertyPropToDataBlockCb;
            m_propertyDataBlockEnumSerializationData.propFromDataBlockCb = _propertyPropFromDataBlockCb;
            m_fieldDataBlockEnumSerializationData.propToDataBlockCb = _fieldPropToDataBlockCb;
            m_fieldDataBlockEnumSerializationData.propFromDataBlockCb = _fieldPropFromDataBlockCb;
        }


        //////////////////////////////////////////
        inline WriteMetaPropertyToMonoClassFieldFunction const& getWriteMetaPropertyToMonoClassFieldFunction(ClassUID _typeUID)
        {
            static WriteMetaPropertyToMonoClassFieldFunction nullValue;

            auto it = m_writeMetaPropertyToMonoClassFieldFunctions.find(_typeUID);
            if (it != m_writeMetaPropertyToMonoClassFieldFunctions.end())
                return it->second;

            return nullValue;
        }

    protected:

        //////////////////////////////////////////
        MonoSerializationManager();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);


        //////////////////////////////////////////
        void registerWriteMetaPropertyToMonoClassFieldFunctions();

    protected:
        static MonoSerializationManager* s_instance;

        StringKeyMap<ScriptPropertyDataBlockSerializationData> m_propertyDataBlockSerializationData;
        StringKeyMap<ScriptFieldDataBlockSerializationData> m_fieldDataBlockSerializationData;

        UnorderedMap<MonoClass*, ScriptPropertyDataBlockSerializationData> m_propertyDataBlockSubClassSerializationData;
        UnorderedMap<MonoClass*, ScriptFieldDataBlockSerializationData> m_fieldDataBlockSubClassSerializationData;

        ScriptPropertyDataBlockSerializationData m_propertyDataBlockEnumSerializationData;
        ScriptFieldDataBlockSerializationData m_fieldDataBlockEnumSerializationData;

        UnorderedMap<ClassUID, WriteMetaPropertyToMonoClassFieldFunction> m_writeMetaPropertyToMonoClassFieldFunctions;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoSerializationManager_hpp_
//////////////////////////////////////////
