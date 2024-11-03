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
#include "EditorCSharpManager.hpp"
#include "maze-core/managers/MazeEventManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "events/EditorEvents.hpp"
#include "helpers/EditorProjectHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorCSharpManager
    //
    //////////////////////////////////////////
    EditorCSharpManager* EditorCSharpManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorCSharpManager::EditorCSharpManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorCSharpManager::~EditorCSharpManager()
    {
        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->unsubscribeEvent<EditorProjectOpenedEvent>(this);
        }

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorCSharpManager::Initialize(EditorCSharpManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorCSharpManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorCSharpManager::init()
    {
        EventManager::GetInstancePtr()->subscribeEvent<EditorProjectOpenedEvent>(this, &EditorCSharpManager::notifyEvent);

        return true;
    }

    //////////////////////////////////////////
    void EditorCSharpManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<EditorProjectOpenedEvent>::UID())
        {
            updateLibraryFolder();
            updateCSharpFolder();
            generateCSharpAssembly();
            compileCSharpAssembly();
        }
    }

    //////////////////////////////////////////
    void EditorCSharpManager::updateCSharpFolder()
    {
        Debug::Log("Updating CSharp folder...");

        AssetFilePtr const& csharp = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("CSharp"));
        MAZE_ERROR_RETURN_IF(!csharp || csharp->getClassUID() != ClassInfo<AssetDirectory>::UID(), "CSharp dir is not found");

        Path const& csharpSrcPath = csharp->getFullPath();
        Path csharpDestPath = EditorHelper::GetProjectFolder() + "/CSharp";
        FileHelper::CreateDirectoryRecursive(csharpDestPath);

        FileHelper::CopyDirectory(csharpSrcPath, csharpDestPath);
    }

    //////////////////////////////////////////
    void EditorCSharpManager::updateLibraryFolder()
    {
        Debug::Log("Updating Library folder...");

        AssetFilePtr const& csharpCoreLib = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("maze-csharp-core-lib.dll"));
        MAZE_ERROR_RETURN_IF(!csharpCoreLib || csharpCoreLib->getClassUID() != ClassInfo<AssetRegularFile>::UID(), "CSharp core lib is not found");

        Path const& csharpCoreLibPath = csharpCoreLib->getFullPath();
        Path csharpDestPath = EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies";
        FileHelper::CreateDirectoryRecursive(csharpDestPath);

        FileHelper::CopyRegularFile(csharpCoreLibPath, csharpDestPath + "/" + csharpCoreLib->getFileName());
    }

    //////////////////////////////////////////
    void EditorCSharpManager::generateCSharpAssembly()
    {
        Debug::Log("Generating CSharp assembly project...");

        Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharp";

#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
        Path generateAssemblyPath = csharpPath + "/generate.bat";

        PROCESS_INFORMATION processInfo;
        STARTUPINFOW startupInfo;
        ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&startupInfo, sizeof(STARTUPINFOW));
        startupInfo.cb = sizeof(STARTUPINFOW);
        startupInfo.lpReserved = NULL;
        startupInfo.lpDesktop = NULL;
        startupInfo.lpTitle = NULL;
        startupInfo.dwFlags = NULL;
        startupInfo.cbReserved2 = 0;
        startupInfo.lpReserved2 = NULL;
        startupInfo.wShowWindow = SW_SHOWDEFAULT;

        // Launch the new process with the specified working directory
        BOOL res = CreateProcessW(
            generateAssemblyPath.c_str(),                       // Path to the executable
            nullptr,                                            // Command-line arguments (optional)
            nullptr,                                            // Process handle not inheritable
            nullptr,                                            // Thread handle not inheritable
            FALSE,                                              // Set handle inheritance to FALSE
            0,                                                  // No creation flags
            nullptr,                                            // Use parent's environment block
            csharpPath.c_str(),                                 // Set the working directory
            &startupInfo,                                       // Pointer to STARTUPINFO structure
            &processInfo                                        // Pointer to PROCESS_INFORMATION structure
        );
        MAZE_ERROR_RETURN_IF(!res, "Failed to generate charp assembly project!");

        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
#else
        MAZE_NOT_IMPLEMENTED;
#endif

        Debug::Log("CSharp assembly project generation finished.");
    }

    //////////////////////////////////////////
    void EditorCSharpManager::compileCSharpAssembly()
    {
        Debug::Log("Compiling CSharp assembly...");

        Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharp";

#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
        Path compileAssemblyPath = csharpPath + "/compile.bat";

        PROCESS_INFORMATION processInfo;
        STARTUPINFOW startupInfo;
        ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));
        ZeroMemory(&startupInfo, sizeof(STARTUPINFOW));
        startupInfo.cb = sizeof(STARTUPINFOW);
        startupInfo.lpReserved = NULL;
        startupInfo.lpDesktop = NULL;
        startupInfo.lpTitle = NULL;
        startupInfo.dwFlags = NULL;
        startupInfo.cbReserved2 = 0;
        startupInfo.lpReserved2 = NULL;
        startupInfo.wShowWindow = SW_SHOWDEFAULT;

        // Launch the new process with the specified working directory
        BOOL res = CreateProcessW(
            compileAssemblyPath.c_str(),                        // Path to the executable
            nullptr,                                            // Command-line arguments (optional)
            nullptr,                                            // Process handle not inheritable
            nullptr,                                            // Thread handle not inheritable
            FALSE,                                              // Set handle inheritance to FALSE
            0,                                                  // No creation flags
            nullptr,                                            // Use parent's environment block
            csharpPath.c_str(),                                 // Set the working directory
            &startupInfo,                                       // Pointer to STARTUPINFO structure
            &processInfo                                        // Pointer to PROCESS_INFORMATION structure
        );
        MAZE_ERROR_RETURN_IF(!res, "Failed to compile charp assembly!");

        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
#else
        MAZE_NOT_IMPLEMENTED;
#endif

        Debug::Log("CSharp assembly project compilation finished.");
    }

} // namespace Maze
//////////////////////////////////////////
