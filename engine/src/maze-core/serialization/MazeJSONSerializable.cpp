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
#include "MazeCoreHeader.hpp"
#include "maze-core/serialization/MazeJSONSerializable.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_CORE_API Json::Value SerializeMetaInstanceToJSONValue(
        Maze::MetaClass const* _metaClass,
        Maze::ConstMetaInstance _metaInstance)
    {
        Json::Value value;

        for (Maze::MetaClass* metaClass : _metaClass->getAllSuperMetaClasses())
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                Maze::MetaProperty* metaProperty = metaClass->getProperty(i);

                Maze::CString propertyName = metaProperty->getName();
                String properyStringValue = metaProperty->toString(_metaInstance);

                value[propertyName] = properyStringValue.c_str();
            }
        }

        return value;
    }

    //////////////////////////////////////////
    MAZE_CORE_API void DeserializeMetaInstanceFromJSONValue(
        Maze::MetaClass const* _metaClass,
        Maze::MetaInstance _metaInstance,
        Json::Value const& _value)
    {
        for (Maze::MetaClass* metaClass : _metaClass->getAllSuperMetaClasses())
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                Maze::MetaProperty* metaProperty = metaClass->getProperty(i);

                Maze::CString propertyName = metaProperty->getName();

                if (!_value[propertyName])
                    continue;

                CString attributeValue = _value[propertyName].asCString();
                if (attributeValue)
                    metaProperty->setString(_metaInstance, attributeValue);
            }
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
