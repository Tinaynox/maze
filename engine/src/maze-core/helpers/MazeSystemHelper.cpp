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
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API String ConstructSystemInfo()
        {
            String info;

            UnixTime timeZoneSeconds = Maze::DateTimeHelper::GetUTCTimeZone();
            UnixTime timeZoneHours = timeZoneSeconds / (60 * 60);

            info += "[System Info]\n";
            info += String("\tPlatform: ") + GetOSVersion() + "\n";
            info += String("\tDate Time(Local): ") + Maze::DateTimeHelper::GetDateTimeLocal().formatDateTime() + " (UTC+" + StringHelper::ToString((U32)timeZoneHours) + ")\n";
            info += String("\tDate Time(UTC): ") + Maze::DateTimeHelper::GetDateTimeUTC().formatDateTime() + "\n";
            info += String("\tGeoLocation: ") + GeoLocationToString(SystemHelper::GetGeoLocation()) + "\n";
            info += String("\tSystemLanguage: ") + GeoLanguageToString(SystemHelper::GetSystemLanguage()) + "\n";
            info += String("\tUserLanguage: ") + GeoLanguageToString(SystemHelper::GetUserLanguage()) + "\n";
            info += String("\tDevice Name: ") + GetDeviceName() + "\n";

            return info;
        }

    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
