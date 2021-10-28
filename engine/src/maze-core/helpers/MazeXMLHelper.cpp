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
        MAZE_CORE_API extern bool SaveXMLFile(String const& _fileFullPath, tinyxml2::XMLNode* _rootNode)
        {
            if (!_rootNode)
                return false;

            if (_fileFullPath.empty())
                return false;

            String directoryFullPath = FileHelper::GetDirectoryInPath(_fileFullPath);
            FileHelper::CreateDirectoryRecursive(directoryFullPath.c_str());

            tinyxml2::XMLDocument doc;

            tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
            doc.LinkEndChild(decl);

            doc.InsertEndChild(_rootNode);

            tinyxml2::XMLError loadError = doc.SaveFile(_fileFullPath.c_str());
            if (tinyxml2::XML_SUCCESS != loadError)
            {
                MAZE_ERROR("Saving file '%s' error - %d!", _fileFullPath.c_str(), loadError);
                return false;
            }

            return true;
        }


        //////////////////////////////////////////
        MAZE_CORE_API extern bool SaveXMLFile(String const& _fileFullPath, IXMLElementSerializable* _serializable)
        {
            if (!_serializable)
                return false;

            if (_fileFullPath.empty())
                return false;

            String directoryFullPath = FileHelper::GetDirectoryInPath(_fileFullPath);
            FileHelper::CreateDirectoryRecursive(directoryFullPath.c_str());

            tinyxml2::XMLDocument doc;

            tinyxml2::XMLDeclaration* decl = doc.NewDeclaration();
            doc.LinkEndChild(decl);

            tinyxml2::XMLNode* rootNode = _serializable->toXMLElement(doc);

            doc.InsertEndChild(rootNode);

            tinyxml2::XMLError loadError = doc.SaveFile(_fileFullPath.c_str());
            if (tinyxml2::XML_SUCCESS != loadError)
            {
                MAZE_ERROR("Saving file '%s' error - %d!", _fileFullPath.c_str(), loadError);
                return false;
            }

            return true;
        }

    } // namespace XMLHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
