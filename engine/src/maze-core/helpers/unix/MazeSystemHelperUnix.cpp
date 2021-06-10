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
// subject to the follounixg restrictions:
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
#include "maze-core/helpers/unix/MazeSystemHelperUnix.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include <sys/utsname.h>
#include <langinfo.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void OpenURL(String const& _url)
        {


        }

        //////////////////////////////////////////
        MAZE_CORE_API String const& GetOSVersion()
        {
            static String s_osVersion;

            if (s_osVersion.empty())
            {
                utsname name;
                uname(&name);
                s_osVersion += name.sysname;
                s_osVersion += String(" ") + name.release;
                s_osVersion += String(" ") + name.version;
            }

            return s_osVersion;

        }

        //////////////////////////////////////////
        MAZE_CORE_API String const& GetDeviceName()
        {
            static String s_deviceName;

            if (s_deviceName.empty())
            {
                utsname name;
                uname(&name);

                s_deviceName += name.nodename;
            }

            return s_deviceName;
        }

    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
