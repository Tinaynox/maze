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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace XMLHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern bool SaveXMLFile(Path const& _fileFullPath, tinyxml2::XMLNode* _rootNode)
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
        MAZE_CORE_API extern bool SaveXMLFile(Path const& _fileFullPath, IXMLElementSerializable* _serializable)
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
        MAZE_CORE_API extern tinyxml2::XMLError LoadXMLFile(Path const& _fileFullPath, tinyxml2::XMLDocument& _doc)
        {
            FILE* file = StdHelper::OpenFile(_fileFullPath, "rb");
            if (!file)
                return tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND;
            tinyxml2::XMLError loadError = _doc.LoadFile(file);
            fclose(file);
            return loadError;
        }

        //////////////////////////////////////////
        MAZE_CORE_API extern tinyxml2::XMLError SaveXMLFile(Path const& _fileFullPath, tinyxml2::XMLDocument& _doc)
        {
            FILE* file = StdHelper::OpenFile(_fileFullPath, "wb");
            if (!file)
                return tinyxml2::XMLError::XML_ERROR_FILE_COULD_NOT_BE_OPENED;
            tinyxml2::XMLError saveError = _doc.SaveFile(file);
            fclose(file);
            return saveError;
        }

    } // namespace XMLHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
