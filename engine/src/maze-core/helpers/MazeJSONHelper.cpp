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
#include "maze-core/helpers/MazeJSONHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace JSONHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API String ToString(Json::Value const& _value)
        {
            Json::StreamWriterBuilder builder;
            builder["commentStyle"] = "None";
            builder["indentation"] = "";
            StdString string = Json::writeString(builder, _value);

            return String(string.begin(), string.end());
        }

        //////////////////////////////////////////
        MAZE_CORE_API Json::Value FromString(String const& _value)
        {
            Json::Value value;

            Json::CharReaderBuilder builder;
            CString begin = &(*_value.begin());
            CString end = begin + _value.size();

            builder.newCharReader()->parse(begin, end, &value, nullptr);

            return value;
        }

    } // namespace JSONHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////
