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
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include <chrono>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace DateTimeHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetUnixTimeUTC()
        {
            return time(0);
        }

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetUnixTimeLocal()
        {
            UnixTime utcTime = GetUnixTimeUTC();

#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
            tm stdDateTime;
            localtime_s(&stdDateTime, &utcTime);
            return _mkgmtime(&stdDateTime);
#else
            return timegm(localtime(&utcTime));
#endif
        }

        //////////////////////////////////////////
        MAZE_CORE_API UnixTime GetUTCTimeZone()
        {
            return GetUnixTimeLocal() - GetUnixTimeUTC();
        }

        //////////////////////////////////////////
        MAZE_CORE_API void ConvertStdDateTimeToDateTime(tm const* _stdDateTime, DateTime* _dateTime)
        {
            _dateTime->seconds = _stdDateTime->tm_sec;
            _dateTime->minutes = _stdDateTime->tm_min;
            _dateTime->hours = _stdDateTime->tm_hour;
            _dateTime->months = _stdDateTime->tm_mon;
            _dateTime->years = _stdDateTime->tm_year;
            _dateTime->monthDay = _stdDateTime->tm_mday;
            _dateTime->weekDay = _stdDateTime->tm_wday;
            _dateTime->yearDay = _stdDateTime->tm_yday;
            _dateTime->daylightSavingsTime = _stdDateTime->tm_isdst;
        }

        //////////////////////////////////////////
        MAZE_CORE_API void ConvertDateTimeToStdDateTime(DateTime const* _dateTime, tm* _stdDateTime)
        {
            _stdDateTime->tm_sec = _dateTime->seconds;
            _stdDateTime->tm_min = _dateTime->minutes;
            _stdDateTime->tm_hour = _dateTime->hours;
            _stdDateTime->tm_mon = _dateTime->months;
            _stdDateTime->tm_year = _dateTime->years;
            _stdDateTime->tm_mday = _dateTime->monthDay;
            _stdDateTime->tm_wday = _dateTime->weekDay;
            _stdDateTime->tm_yday = _dateTime->yearDay;
            _stdDateTime->tm_isdst = _dateTime->daylightSavingsTime;
        }

        //////////////////////////////////////////
        MAZE_CORE_API void ConvertUnixTimeToDateTime(UnixTime _unixTime, DateTime* _dateTime)
        {
            tm stdDateTime;

#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
            gmtime_s(&stdDateTime, &_unixTime);
#else
            stdDateTime = *gmtime(&_unixTime);
#endif

            ConvertStdDateTimeToDateTime(&stdDateTime, _dateTime);
        }

        //////////////////////////////////////////
        MAZE_CORE_API void GetDateTimeUTC(DateTime* _dateTime)
        {
            UnixTime systemUnixTimeUTC = GetUnixTimeUTC();
            ConvertUnixTimeToDateTime(systemUnixTimeUTC, _dateTime);
        }

        //////////////////////////////////////////
        MAZE_CORE_API DateTime GetDateTimeUTC()
        {
            DateTime dateTime;
            GetDateTimeUTC(&dateTime);
            return dateTime;
        }

        //////////////////////////////////////////
        MAZE_CORE_API void GetDateTimeLocal(DateTime* _dateTime)
        {
            UnixTime localUnixTime = GetUnixTimeLocal();
            ConvertUnixTimeToDateTime(localUnixTime, _dateTime);
        }

        //////////////////////////////////////////
        MAZE_CORE_API DateTime GetDateTimeLocal()
        {
            DateTime dateTime;
            GetDateTimeLocal(&dateTime);
            return dateTime;
        }


    } // namespace DateTimeHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
