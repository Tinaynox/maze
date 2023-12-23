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
#if (!defined(_MazeXMLHelper_hpp_))
#define _MazeXMLHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/serialization/MazeXMLSerializable.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include <tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    struct MAZE_CORE_API XMLToDataBlockConfig
    {
        Bool collapseRootBlock = false;
        Bool lowerCaseBlockNameCapitalButton = false;
    };


    //////////////////////////////////////////
    namespace XMLHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API bool SaveXMLFile(Path const& _fileFullPath, tinyxml2::XMLNode* _rootNode);

        //////////////////////////////////////////
        MAZE_CORE_API bool SaveXMLFile(Path const& _fileFullPath, IXMLElementSerializable* _serializable);

        //////////////////////////////////////////
        MAZE_CORE_API tinyxml2::XMLError LoadXMLFile(Path const& _fileFullPath, tinyxml2::XMLDocument& _doc);

        //////////////////////////////////////////
        MAZE_CORE_API tinyxml2::XMLError SaveXMLFile(Path const& _fileFullPath, tinyxml2::XMLDocument& _doc);



        //////////////////////////////////////////
        MAZE_CORE_API tinyxml2::XMLElement* SerializeMetaInstanceToXMLElement(
            Maze::MetaClass const* _metaClass,
            Maze::ConstMetaInstance _metaInstance,
            tinyxml2::XMLDocument& _doc);

        //////////////////////////////////////////
        MAZE_CORE_API void DeserializeMetaInstanceFromXMLElement(
            Maze::MetaClass const* _metaClass,
            Maze::MetaInstance _metaInstance,
            tinyxml2::XMLElement const* _element);


        //////////////////////////////////////////
        MAZE_CORE_API bool ConvertXMLDocumentToDataBlock(
            tinyxml2::XMLDocument const* _doc,
            DataBlock& _dataBlock,
            XMLToDataBlockConfig const& _config = XMLToDataBlockConfig());

        //////////////////////////////////////////
        MAZE_CORE_API bool ConvertXMLElementToDataBlock(
            tinyxml2::XMLElement const* _element,
            DataBlock& _dataBlock,
            XMLToDataBlockConfig const& _config = XMLToDataBlockConfig());

    } // namespace XMLHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeXMLHelper_hpp_
//////////////////////////////////////////
