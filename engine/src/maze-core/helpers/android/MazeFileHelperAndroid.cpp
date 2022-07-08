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
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/android/MazeFileHelperAndroid.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/android/MazeSystemManagerAndroid.hpp"
#include <unistd.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace FileHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API Path GetWorkingDirectory()
        {
            S8 buff[PATH_MAX + 1];
            getcwd(buff, sizeof(buff));
            return buff;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryFullPath()
        {
            std::vector<char> buf(400);
            ssize_t len;

            do
            {
                buf.resize(buf.size() + 100);
                len = ::readlink("/proc/self/exe", &(buf[0]), buf.size());
            }
            while (buf.size() == len);

            if (len > 0)
            {
                buf[len] = '\0';
                return (Path( &( buf[0] ) ) );
            }

            return "";
        }


        //////////////////////////////////////////
        MAZE_CORE_API Path GetBinaryDirectory()
        {
            return GetDirectoryInPath(GetBinaryFullPath());
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDocumentsDirectory()
        {
            return GetInternalDataDirectory();
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultTemporaryDirectory()
        {
            return GetExternalDataDirectory() + "/tmp";
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetDefaultLogDirectory()
        {
            return GetExternalDataDirectory() + "/log";
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetPackageName()
        {
            static Path s_packageName;

            if (s_packageName.empty())
            {
                char path[64] = { 0 };
                sprintf(path, "/proc/self/cmdline");
                FILE *cmdline = fopen(path, "r");
                if (cmdline)
                {
                    char applicationId[64] = { 0 };
                    fread(applicationId, sizeof(applicationId), 1, cmdline);
                    s_packageName = applicationId;
                    fclose(cmdline);
                }

            }

            return s_packageName;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetLibDirectory()
        {
            return "/data/data/" + GetPackageName() + "/lib";
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetInternalDataDirectory()
        {
            SystemManager* systemManager = SystemManager::GetInstancePtr();
            MAZE_ERROR_RETURN_VALUE_IF(!systemManager, Path(), "SystemManager is null!");

            SystemManagerAndroid* systemManagerAndroid = systemManager->castRaw<SystemManagerAndroid>();

            android_app* androidApp = systemManagerAndroid->getAndroidApp();
            MAZE_ERROR_RETURN_VALUE_IF(!androidApp, Path(), "android_app is null!");

            ANativeActivity* nativeActivity = androidApp->activity;
            MAZE_ERROR_RETURN_VALUE_IF(!nativeActivity, Path(), "nativeActivity is null!");

            MAZE_ERROR_RETURN_VALUE_IF(!nativeActivity->internalDataPath, Path(), "internalDataPath is null!");

            return nativeActivity->internalDataPath;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetExternalDataDirectory()
        {
            SystemManager* systemManager = SystemManager::GetInstancePtr();
            MAZE_ERROR_RETURN_VALUE_IF(!systemManager, Path(), "SystemManager is null!");

            SystemManagerAndroid* systemManagerAndroid = systemManager->castRaw< SystemManagerAndroid >();

            android_app* androidApp = systemManagerAndroid->getAndroidApp();
            MAZE_ERROR_RETURN_VALUE_IF(!androidApp, Path(), "android_app is null!");

            ANativeActivity* nativeActivity = androidApp->activity;
            MAZE_ERROR_RETURN_VALUE_IF(!nativeActivity, Path(), "nativeActivity is null!");

            MAZE_ERROR_RETURN_VALUE_IF(!nativeActivity->externalDataPath, Path(), "externalDataPath is null!");

            return nativeActivity->externalDataPath;
        }

        //////////////////////////////////////////
        MAZE_CORE_API Path GetUnpackedAssetsDirectory()
        {
            return FileHelper::GetExternalDataDirectory() + "/assets";
        }


    } // namespace FileHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
