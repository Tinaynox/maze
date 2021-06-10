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
#include "maze-core/helpers/linux/MazeSystemHelperLinux.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <locale>
#include <langinfo.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API GeoLocation GetGeoLocation()
        {
            if (setlocale(LC_ALL, "") == NULL)
            {
                return GeoLocation::Unknown;
            }

            S8* telephonePrefix = nl_langinfo(_NL_TELEPHONE_INT_PREFIX);
            if (!telephonePrefix)
                return GeoLocation::Unknown;


            String telephonePrefixStr = telephonePrefix;
            StringHelper::RemoveSubstring(telephonePrefixStr, " ");
            StringHelper::RemoveSubstring(telephonePrefixStr, "-");

            S32 telephonePrefixS32 = StringHelper::StringToS32(telephonePrefixStr);

            return CallingCodeToGeoLocation(telephonePrefixS32);
        }

        //////////////////////////////////////////
        MAZE_CORE_API void SetupMazeDynLibDirectoriesPolicy()
        {

        }

    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
