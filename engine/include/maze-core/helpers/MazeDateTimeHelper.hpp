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
#if (!defined(_MazeDateTimeHelper_hpp_))
#define _MazeDateTimeHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeDateTime.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace DateTimeHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetUnixTimeUTC();

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetUnixTimeLocal();


        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetUTCTimeZone();

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime ConvertUTCToLocal(UnixTime _utc);

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime ConvertLocalToUTC(UnixTime _localTime);

        //////////////////////////////////////////
        MAZE_CORE_API void ConvertStdDateTimeToDateTime(tm const* _stdDateTime, DateTime* _dateTime);

        //////////////////////////////////////////
        MAZE_CORE_API void ConvertDateTimeToStdDateTime(DateTime const* _dateTime, tm* _stdDateTime);

        //////////////////////////////////////////
        MAZE_CORE_API void ConvertUnixTimeToDateTime(UnixTime _unixTime, DateTime* _dateTime);

        //////////////////////////////////////////
        inline DateTime ConvertUnixTimeToDateTime(UnixTime _unixTime)
        {
            DateTime dateTime;
            ConvertUnixTimeToDateTime(_unixTime, &dateTime);
            return dateTime;
        }


        //////////////////////////////////////////
        MAZE_CORE_API void GetDateTimeUTC(DateTime* _systemTime);

        //////////////////////////////////////////
        MAZE_CORE_API DateTime GetDateTimeUTC();


        //////////////////////////////////////////
        MAZE_CORE_API void GetDateTimeLocal(DateTime* _systemTime);

        //////////////////////////////////////////
        MAZE_CORE_API DateTime GetDateTimeLocal();


    } // namespace DateTimeHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDateTimeHelper_hpp_
//////////////////////////////////////////
