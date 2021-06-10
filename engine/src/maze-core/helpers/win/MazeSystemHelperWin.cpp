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
#include "maze-core/helpers/win/MazeSystemHelperWin.hpp"
#include "maze-core/helpers/MazeStdHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include <ShlObj.h>
#include <shellapi.h>
#include <intrin.h>       
#include <iphlpapi.h>
#include <lm.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API GeoLocation GetGeoLocation()
        {
            GEOID myGEO = GetUserGeoID(GEOCLASS_NATION);
            return GeoLocation(myGEO);
        }

        //////////////////////////////////////////
        MAZE_CORE_API void OpenURL(String const& _url)
        {
            ShellExecuteA(NULL, "open", _url.c_str(), NULL, NULL, SW_SHOW);
        }

        //////////////////////////////////////////
        MAZE_CORE_API bool GetWinMajorMinorVersion(DWORD& _major, DWORD& _minor)
        {
            bool bRetCode = false;
            LPBYTE pinfoRawData = 0;

            if (NERR_Success == NetWkstaGetInfo(NULL, 100, &pinfoRawData))
            {
                WKSTA_INFO_100* pworkstationInfo = (WKSTA_INFO_100*)pinfoRawData;
                _major = pworkstationInfo->wki100_ver_major;
                _minor = pworkstationInfo->wki100_ver_minor;
                ::NetApiBufferFree(pinfoRawData);
                bRetCode = true;
            }

            return bRetCode;
        }


        //////////////////////////////////////////
        using GETSYSTEMINFO = void(__stdcall*)(LPSYSTEM_INFO);

        //////////////////////////////////////////
        MAZE_CORE_API String const& GetOSVersion()
        {
            static String value;

            if (value.empty())
            {
                String winver;
                OSVERSIONINFOEX osver;
                SYSTEM_INFO     sysInfo;

                __pragma(warning(push))
                __pragma(warning(disable:4996))
                memset(&osver, 0, sizeof(osver));
                osver.dwOSVersionInfoSize = sizeof(osver);
                GetVersionEx((LPOSVERSIONINFO)&osver);
                __pragma(warning(pop))

                DWORD major = 0;
                DWORD minor = 0;
                if (GetWinMajorMinorVersion(major, minor))
                {
                    osver.dwMajorVersion = major;
                    osver.dwMinorVersion = minor;
                }
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
                {
                    OSVERSIONINFOEXW osvi;
                    ULONGLONG cm = 0;
                    cm = VerSetConditionMask(cm, VER_MINORVERSION, VER_EQUAL);
                    ZeroMemory(&osvi, sizeof(osvi));
                    osvi.dwOSVersionInfoSize = sizeof(osvi);
                    osvi.dwMinorVersion = 3;

                    if (VerifyVersionInfoW(&osvi, VER_MINORVERSION, cm))
                    {
                        osver.dwMinorVersion = 3;
                    }
                }

                GETSYSTEMINFO getSysInfo = (GETSYSTEMINFO)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetNativeSystemInfo");

                if (getSysInfo == nullptr)
                    getSysInfo = ::GetSystemInfo;

                getSysInfo(&sysInfo);

                if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType != VER_NT_WORKSTATION) 
                    winver = "Windows 10 Server";
                else
                if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType == VER_NT_WORKSTATION)
                    winver = "Windows 10";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType != VER_NT_WORKSTATION)
                    winver = "Windows Server 2012 R2";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType == VER_NT_WORKSTATION)
                    winver = "Windows 8.1";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType != VER_NT_WORKSTATION)
                    winver = "Windows Server 2012";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION)
                    winver = "Windows 8";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType != VER_NT_WORKSTATION)
                    winver = "Windows Server 2008 R2";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType == VER_NT_WORKSTATION)
                    winver = "Windows 7";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType != VER_NT_WORKSTATION)
                    winver = "Windows Server 2008";
                else
                if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType == VER_NT_WORKSTATION)
                    winver = "Windows Vista";
                else
                if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION
                    && sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
                    winver = "Windows XP x64";
                else
                if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
                    winver = "Windows Server 2003";
                else
                if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
                    winver = "Windows XP";
                else
                if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
                    winver = "Windows 2000";
                else
                if (osver.dwMajorVersion < 5)
                    winver = "unknown";
                else
                    winver = "Windows (Major:" + StringHelper::S32ToString(osver.dwMajorVersion) + " Minor:" + StringHelper::S32ToString(osver.dwMinorVersion) + ")";

                if (osver.wServicePackMajor != 0)
                {
                    String sp;
                    char buf[128] = {0};
                    sp = " Service Pack ";
                    sprintf_s(buf, sizeof(buf), "%hd", osver.wServicePackMajor);
                    sp.append(buf);
                    winver += sp;
                }

                std::transform(winver.begin(), winver.end(), winver.begin(), ::tolower);

                value = winver;
            }

            return value;
        }

        //////////////////////////////////////////
        MAZE_CORE_API String const& GetDeviceName()
        {
            static String s_deviceName;
            if (s_deviceName.empty())
            {
                S8 deviceName[1024];
                DWORD size = 1024;
                GetComputerName(deviceName, &size);

                s_deviceName = deviceName;
            }
            return s_deviceName;
        }

        using ADD_DLL_PROC        = DLL_DIRECTORY_COOKIE (WINAPI *)(PCWSTR);
        using REMOVE_DLL_PROC    = BOOL (WINAPI *)(DLL_DIRECTORY_COOKIE);

    #if (!defined(LOAD_LIBRARY_SEARCH_USER_DIRS))
    #    define LOAD_LIBRARY_SEARCH_USER_DIRS       0x00000400
    #endif

        //////////////////////////////////////////
        MAZE_CORE_API void SetupMazeDynLibDirectoriesPolicy()
        {
            WString workingDirectory = FileHelper::GetWorkingDirectoryW();
            WString libDirectoryW = workingDirectory + L"/lib";

            WString archSubDirW = L"/" MAZE_ARCH_SUFFIX_WSTR;
            WString dllDirectoryW = libDirectoryW + archSubDirW;

            Debug::Log(L"DynLib directory: %ws", dllDirectoryW.c_str());

            SetDllDirectoryW(dllDirectoryW.c_str());

            ADD_DLL_PROC lpfnAdllDllDirectory = (ADD_DLL_PROC)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "AddDllDirectory");
            REMOVE_DLL_PROC lpfnRemoveDllDirectory = (REMOVE_DLL_PROC)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "RemoveDllDirectory");
            if (lpfnAdllDllDirectory && lpfnRemoveDllDirectory)
            {
                ((ADD_DLL_PROC)lpfnAdllDllDirectory)(libDirectoryW.c_str());
                ((ADD_DLL_PROC)lpfnAdllDllDirectory)(dllDirectoryW.c_str());
            }
        }

    } // namespace SystemHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
