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
#if (!defined(_MazeDateTime_hpp_))
#define _MazeDateTime_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct DateTime
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API DateTime
    {
        S32 seconds;    // [0, 60] Including leap second
        S32 minutes;    // [0, 59]
        S32 hours;      // [0, 23]
        S32 months;     // [0, 11] (Since January)
        S32 years;      // Since 1900

        S32 monthDay;   // [1, 31]
        S32 weekDay;    // [0, 6] (Since Sunday)
        S32 yearDay;    // [0, 365] (Since January 1)

        S32 daylightSavingsTime;



        //////////////////////////////////////////
        String formatSeconds();

        //////////////////////////////////////////
        String formatMinutes();

        //////////////////////////////////////////
        String formatHours24();

        //////////////////////////////////////////
        String formatHours12();

        //////////////////////////////////////////
        String formatTime24();

        //////////////////////////////////////////
        String formatTime12();

        //////////////////////////////////////////
        String formatTimeFull24();

        //////////////////////////////////////////
        String formatTimeFull12();
        
        //////////////////////////////////////////
        String formatDateTime();
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeDateTime_hpp_
//////////////////////////////////////////
