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
#include "maze-core/serialization/MazeXMLSerializable.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_CORE_API tinyxml2::XMLElement* SerializeMetaInstanceToXMLElement(
        Maze::MetaClass const* _metaClass,
        Maze::ConstMetaInstance _metaInstance,
        tinyxml2::XMLDocument& _doc)
    {
        Maze::CString className = _metaClass->getName();
        tinyxml2::XMLElement* element = _doc.NewElement(className);

        for (Maze::MetaClass* metaClass : _metaClass->getAllSuperMetaClasses())
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                Maze::MetaProperty* metaProperty = metaClass->getProperty(i);

                Maze::CString propertyName = metaProperty->getName();
                String properyStringValue = metaProperty->toString(_metaInstance);

                element->SetAttribute(propertyName, properyStringValue.c_str());
            }
        }

        return element;
    }

    //////////////////////////////////////////
    MAZE_CORE_API void DeserializeMetaInstanceFromXMLElement(
        Maze::MetaClass const* _metaClass,
        Maze::MetaInstance _metaInstance,
        tinyxml2::XMLElement const* _element)
    {
        for (Maze::MetaClass* metaClass : _metaClass->getAllSuperMetaClasses())
        {
            for (S32 i = 0; i < metaClass->getPropertiesCount(); ++i)
            {
                Maze::MetaProperty* metaProperty = metaClass->getProperty(i);

                Maze::CString propertyName = metaProperty->getName();

                CString attributeValue = _element->Attribute(propertyName);
                if (attributeValue)
                    metaProperty->setString(_metaInstance, attributeValue);
            }
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
