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
#include "maze-core/helpers/MazeXMLHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/serialization/MazeDataBlockSerializationUtils.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace XMLHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool SaveXMLFile(Path const& _fileFullPath, tinyxml2::XMLNode* _rootNode)
        {
            if (!_rootNode)
                return false;

            if (_fileFullPath.empty())
                return false;

            Path directoryFullPath = FileHelper::GetDirectoryInPath(_fileFullPath);
            FileHelper::CreateDirectoryRecursive(directoryFullPath);

            tinyxml2::XMLDocument doc;

            tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
            doc.LinkEndChild(decl);

            doc.InsertEndChild(_rootNode);

            tinyxml2::XMLError saveError = SaveXMLFile(_fileFullPath, doc);
            if (tinyxml2::XML_SUCCESS != saveError)
            {
                MAZE_ERROR("Saving file '%s' error - %d!", _fileFullPath.toUTF8().c_str(), saveError);
                return false;
            }

            return true;
        }


        //////////////////////////////////////////
        MAZE_CORE_API bool SaveXMLFile(Path const& _fileFullPath, IXMLElementSerializable* _serializable)
        {
            if (!_serializable)
                return false;

            if (_fileFullPath.empty())
                return false;

            Path directoryFullPath = FileHelper::GetDirectoryInPath(_fileFullPath);
            FileHelper::CreateDirectoryRecursive(directoryFullPath.toUTF8().c_str());

            tinyxml2::XMLDocument doc;

            tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
            doc.LinkEndChild(decl);

            tinyxml2::XMLNode* rootNode = _serializable->toXMLElement(doc);

            doc.InsertEndChild(rootNode);

            tinyxml2::XMLError saveError = SaveXMLFile(_fileFullPath, doc);
            if (tinyxml2::XML_SUCCESS != saveError)
            {
                MAZE_ERROR("Saving file '%s' error - %d!", _fileFullPath.toUTF8().c_str(), saveError);
                return false;
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API tinyxml2::XMLError LoadXMLFile(Path const& _fileFullPath, tinyxml2::XMLDocument& _doc)
        {
            FILE* file = StdHelper::OpenFile(_fileFullPath, "rb");
            if (!file)
                return tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND;
            tinyxml2::XMLError loadError = _doc.LoadFile(file);
            fclose(file);
            return loadError;
        }

        //////////////////////////////////////////
        MAZE_CORE_API tinyxml2::XMLError SaveXMLFile(Path const& _fileFullPath, tinyxml2::XMLDocument& _doc)
        {
            FILE* file = StdHelper::OpenFile(_fileFullPath, "wb");
            if (!file)
                return tinyxml2::XMLError::XML_ERROR_FILE_COULD_NOT_BE_OPENED;
            tinyxml2::XMLError saveError = _doc.SaveFile(file);
            fclose(file);
            return saveError;
        }



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



        //////////////////////////////////////////
        MAZE_CORE_API bool ConvertXMLDocumentToDataBlock(
            tinyxml2::XMLDocument const* _doc,
            DataBlock& _dataBlock,
            XMLToDataBlockConfig const& _config)
        {
            _dataBlock.clear();

            tinyxml2::XMLNode const* subNode = _doc->FirstChild();

            Size rootElementsCount = 0;
            while (subNode)
            {
                if (subNode->ToElement())
                    ++rootElementsCount;

                subNode = subNode->NextSibling();
            }
            bool singleRootElement = (rootElementsCount == 1);

            subNode = _doc->FirstChild();
            while (subNode)
            {
                tinyxml2::XMLElement const* subElement = subNode->ToElement();
                if (subElement)
                {
                    Bool collapseRootBlock = (_config.collapseRootBlock && singleRootElement);

                    String name = subElement->Name();
                    if (_config.lowerCaseBlockNameCapitalButton)
                        name[0] = tolower(name[0]);

                    ConvertXMLElementToDataBlock(
                        subElement,
                        collapseRootBlock ? _dataBlock : *_dataBlock.addNewDataBlock(name.c_str()),
                        _config);
                    break;
                }

                subNode = subNode->NextSibling();
            }

            return true;
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool ConvertXMLElementToDataBlock(
            tinyxml2::XMLElement const* _element,
            DataBlock& _dataBlock,
            XMLToDataBlockConfig const& _config)
        {
            _dataBlock.clearData();

            if (_element)
            {
                tinyxml2::XMLAttribute const* attribute = _element->FirstAttribute();
                while (attribute)
                {
                    _dataBlock.addCString(attribute->Name(), attribute->Value());
                    attribute = attribute->Next();
                }
            }

            tinyxml2::XMLNode const* subNode = _element->FirstChild();
            while (subNode)
            {
                tinyxml2::XMLComment const* subComment = subNode->ToComment();
                if (subComment)
                {
                    bool isMultiline = strchr(subComment->Value(), '\n') != nullptr;
                    HashedCString const commentKey = isMultiline ? MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_C)
                                                                 : MAZE_HASHED_CSTRING(MAZE_DATA_BLOCK_COMMENT_CPP);
                    _dataBlock.addNewDataBlock(commentKey)->addCString(
                        commentKey,
                        subComment->Value());
                    subNode = subNode->NextSibling();
                    continue;
                }

                tinyxml2::XMLElement const* subElement = subNode->ToElement();
                if (subElement)
                {
                    String name = subElement->Name();
                    if (_config.lowerCaseBlockNameCapitalButton)
                        name[0] = tolower(name[0]);

                    if (!ConvertXMLElementToDataBlock(
                        subElement,
                        *_dataBlock.addNewDataBlock(name.c_str()),
                        _config))
                        return false;
                }

                subNode = subNode->NextSibling();
            }

            return true;
        }

    } // namespace XMLHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
