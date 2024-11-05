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
#define MAZE_MONO_SERIALIZATION_TYPE(DTypeName, DType)                                                      \
        registerPropertyAndFieldDataBlockSerialization(MAZE_HCS(DTypeName),                                 \
            [](ScriptInstance const& _instance, ScriptPropertyPtr const& _prop, DataBlock& _dataBlock)      \
            {                                                                                               \
                DType value;                                                                                \
                _instance.getPropertyValue(_prop, value);                                                   \
                _dataBlock.set ## DType(_prop->getName(), value);                                           \
            },                                                                                              \
            [](ScriptInstance& _instance, ScriptPropertyPtr const& _prop, DataBlock const& _dataBlock)      \
            {                                                                                               \
                DataBlock::ParamIndex idx = _dataBlock.findParamIndex(_prop->getName());                    \
                if (idx >= 0)                                                                               \
                    _instance.setPropertyValue(_prop, _dataBlock.get ## DType(idx));                        \
            },                                                                                              \
            [](ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)        \
            {                                                                                               \
                DType value;                                                                                \
                _instance.getFieldValue(_field, value);                                                     \
                _dataBlock.set ## DType(_field->getName(), value);                                          \
            },                                                                                              \
            [](ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)        \
            {                                                                                               \
                DataBlock::ParamIndex idx = _dataBlock.findParamIndex(_field->getName());                   \
                if (idx >= 0)                                                                               \
                    _instance.setFieldValue(_field, _dataBlock.get ## DType(idx));                          \
            });

        // Core
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

        // Graphics
        MAZE_MONO_SERIALIZATION_TYPE("Maze.Graphics.ColorU32", Vec4U8);
        MAZE_MONO_SERIALIZATION_TYPE("Maze.Graphics.ColorF128", Vec4F);

#undef MAZE_MONO_SERIALIZATION_TYPE

        return true;
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerPropertyDataBlockSerialization(
        HashedCString _typeName,
        std::function<void(ScriptInstance const&, ScriptPropertyPtr const&, DataBlock&)> const& _propToDataBlockCb,
        std::function<void(ScriptInstance&, ScriptPropertyPtr const&, DataBlock const&)> const& _propFromDataBlockCb)
    {
        ScriptPropertyDataBlockSerializationData& data = m_propertyDataBlockSerializationData[_typeName];
        data.propToDataBlockCb = _propToDataBlockCb;
        data.propFromDataBlockCb = _propFromDataBlockCb;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::savePropertyToDataBlock(ScriptInstance const& _instance, ScriptPropertyPtr const& _property, DataBlock& _dataBlock)
    {
        auto it = m_propertyDataBlockSerializationData.find(_property->getTypeName());
        if (it != m_propertyDataBlockSerializationData.end())
        {
            it->second.propToDataBlockCb(_instance, _property, _dataBlock);
            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::loadPropertyFromDataBlock(ScriptInstance& _instance, ScriptPropertyPtr const& _property, DataBlock const& _dataBlock)
    {
        auto it = m_propertyDataBlockSerializationData.find(_property->getTypeName());
        if (it != m_propertyDataBlockSerializationData.end())
        {
            it->second.propFromDataBlockCb(_instance, _property, _dataBlock);
            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    void MonoSerializationManager::registerFieldDataBlockSerialization(
        HashedCString _typeName,
        std::function<void(ScriptInstance const&, ScriptFieldPtr const&, DataBlock&)> const& _propToDataBlockCb,
        std::function<void(ScriptInstance&, ScriptFieldPtr const&, DataBlock const&)> const& _propFromDataBlockCb)
    {
        ScriptFieldDataBlockSerializationData& data = m_fieldDataBlockSerializationData[_typeName];
        data.propToDataBlockCb = _propToDataBlockCb;
        data.propFromDataBlockCb = _propFromDataBlockCb;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::saveFieldToDataBlock(ScriptInstance const& _instance, ScriptFieldPtr const& _field, DataBlock& _dataBlock)
    {
        auto it = m_fieldDataBlockSerializationData.find(_field->getTypeName());
        if (it != m_fieldDataBlockSerializationData.end())
        {
            it->second.propToDataBlockCb(_instance, _field, _dataBlock);
            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    bool MonoSerializationManager::loadFieldFromDataBlock(ScriptInstance& _instance, ScriptFieldPtr const& _field, DataBlock const& _dataBlock)
    {
        auto it = m_fieldDataBlockSerializationData.find(_field->getTypeName());
        if (it != m_fieldDataBlockSerializationData.end())
        {
            it->second.propFromDataBlockCb(_instance, _field, _dataBlock);
            return true;
        }

        return false;
    }


} // namespace Maze
//////////////////////////////////////////
