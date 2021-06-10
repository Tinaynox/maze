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
#include "maze-core/system/MazeDateTime.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeDateTimeHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct DateTime
    //
    //////////////////////////////////////////
    String DateTime::formatSeconds()
    {
        return StringHelper::ToString(seconds);
    }

    //////////////////////////////////////////
    String DateTime::formatMinutes()
    {
        return StringHelper::ToString(minutes);
    }

    //////////////////////////////////////////
    String DateTime::formatHours24()
    {
        return StringHelper::ToString(hours);
    }

    //////////////////////////////////////////
    String DateTime::formatHours12()
    {
        if (hours > 12)
            return StringHelper::ToString(hours - 12);
        else
            return StringHelper::ToString(hours);
    }

    //////////////////////////////////////////
    String DateTime::formatTime24()
    {
        String result;
        StringHelper::FormatString(
            result,
            "%02d:%02d",
            hours,
            minutes);
        return result;
    }

    //////////////////////////////////////////
    String DateTime::formatTime12()
    {
        String result;
        StringHelper::FormatString(
            result,
            "%02d:%02d",
            hours > 12 ? hours - 12 : hours,
            minutes);
        return result;
    }

    //////////////////////////////////////////
    String DateTime::formatTimeFull24()
    {
        String result;
        StringHelper::FormatString(
            result,
            "%02d:%02d:%02d",
            hours,
            minutes,
            seconds);
        return result;
    }

    //////////////////////////////////////////
    String DateTime::formatTimeFull12()
    {
        String result;
        StringHelper::FormatString(
            result,
            "%02d:%02d:%02d",
            hours > 12 ? hours - 12 : hours,
            minutes,
            seconds);
        return result;
    }

    //////////////////////////////////////////
    String DateTime::formatDateTime()
    {
        String result;

        tm stdDateTime;
        DateTimeHelper::ConvertDateTimeToStdDateTime(this, &stdDateTime);

#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
        S8 buff[128];
        asctime_s(buff, &stdDateTime);
        result = buff;
#else
        result = asctime(&stdDateTime);
#endif

        if (result.back() == '\n')
            result.pop_back();

        return result;
    }


} // namespace Maze
//////////////////////////////////////////
