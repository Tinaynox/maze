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
#include "maze-core/helpers/android/MazeSystemHelperAndroid.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/managers/android/MazeSystemManagerAndroid.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void OpenURL(Path const& _url)
        {
            SystemManager* systemManager = SystemManager::GetInstancePtr();
            MAZE_ERROR_RETURN_IF(!systemManager, "SystemManager is null!");

            SystemManagerAndroid* systemManagerAndroid = systemManager->castRaw<SystemManagerAndroid>();
            systemManagerAndroid->callActivityMethod("openURL", _url.c_str());
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API GeoLocation GetGeoLocation()
        {
            SystemManager* systemManager = SystemManager::GetInstancePtr();
            MAZE_ERROR_RETURN_VALUE_IF(!systemManager, GeoLocation::Unknown, "SystemManager is null!");

            SystemManagerAndroid* systemManagerAndroid = systemManager->castRaw<SystemManagerAndroid>();
            String iso2 = systemManagerAndroid->callActivityMethodString("getCountryCodeISO2");
            MAZE_ERROR_RETURN_VALUE_IF(iso2.empty(), GeoLocation::Unknown, "getCountryCodeISO2 failed!");

            return ISO2ToGeoLocation(iso2);
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API GeoLanguage GetUserLanguage()
        {
            return GeoLanguage::Unknown;
        }

        //////////////////////////////////////////
        MAZE_CORE_API GeoLanguage GetSystemLanguage()
        {
            return GeoLanguage::Unknown;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void SetupMazeDynLibDirectoriesPolicy()
        {
            
        }

    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
