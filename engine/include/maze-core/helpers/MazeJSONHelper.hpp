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
#if (!defined(_MazeJSONHelper_hpp_))
#define _MazeJSONHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include <json/json.h>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace JSONHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern String ToString(Json::Value const& _value);

        //////////////////////////////////////////
        MAZE_CORE_API extern Json::Value FromString(String const& _value);


        //////////////////////////////////////////
        MAZE_CORE_API Json::Value SerializeMetaInstanceToJSONValue(
            Maze::MetaClass const* _metaClass,
            Maze::ConstMetaInstance _metaInstance);

        //////////////////////////////////////////
        MAZE_CORE_API void DeserializeMetaInstanceFromJSONValue(
            Maze::MetaClass const* _metaClass,
            Maze::MetaInstance _metaInstance,
            Json::Value const& _value);


        //////////////////////////////////////////
#define MAZE_JSON_STRING_SERIALIZATION(DClassName) \
        inline String toString() const \
        { \
            return JSONHelper::ToString(toJSONValue()); \
        } \
        inline static DClassName FromString(String const& _string)\
        {\
            DClassName value;\
            value.loadFromJSONValue(JSONHelper::FromString(_string));\
            return value;\
        }


    } // namespace JSONHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeJSONHelper_hpp_
//////////////////////////////////////////
