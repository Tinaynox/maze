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
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-plugin-csharp/MazeCSharpService.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "events/EditorEvents.hpp"
#include "helpers/EditorProjectHelper.hpp"
#include "helpers/EditorAssetHelper.hpp"
#include "managers/EditorUIManager.hpp"
#include "managers/EditorAssetsManager.hpp"


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
            EventManager::GetInstancePtr()->unsubscribeEvent<EditorProjectWillBeClosedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<CSharpAppAssemblyLoadedEvent>(this);
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
        EventManager::GetInstancePtr()->subscribeEvent<EditorProjectWillBeClosedEvent>(this, &EditorCSharpManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<CSharpAppAssemblyLoadedEvent>(this, &EditorCSharpManager::notifyEvent);

        EditorUIManager::GetInstancePtr()->addTopBarOption(
            "Scripts",
            "Generate",
            [](String const& _text)
            {
                if (EditorCSharpManager::GetInstancePtr())
                    EditorCSharpManager::GetInstancePtr()->generateCSharpAssembly();
            },
            []() { return true; });
        EditorUIManager::GetInstancePtr()->addTopBarOption(
            "Scripts",
            "Compile",
            [](String const& _text)
            {
                if (EditorCSharpManager::GetInstancePtr())
                    EditorCSharpManager::GetInstancePtr()->compileCSharpAssembly();
            },
            []() { return true; });
        EditorUIManager::GetInstancePtr()->addTopBarOption(
            "Scripts",
            "Open project",
            [](String const& _text)
            {
                Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharp/prj/Assembly-CSharp.sln";
                SystemHelper::OpenURL(csharpPath);
            },
            []() { return true; });

        EditorAssetsManager::GetInstancePtr()->addAssetContextOption(
            MAZE_HCS("cs"),
            "Edit",
            [](AssetsController* _controller, Path const& _fullPath)
            {
                // #TODO: move to setting
                Path devenv = "F:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/devenv.exe";

                Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharp/prj/Assembly-CSharp.sln";
                Path params = csharpPath + Path(" ") + _fullPath;
                SystemHelper::ExecuteShell(devenv, params);
            });

        return true;
    }

    //////////////////////////////////////////
    void EditorCSharpManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<EditorProjectOpenedEvent>::UID())
        {
            updateLibraryFolder();
            AssetManager::GetInstancePtr()->addAssetsDirectoryPath(EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies");

            updateCSharpFolder();
            generateCSharpAssembly();
            compileCSharpAssembly();

            loadCSharpAssembly();
        }
        else
        if (_eventUID == ClassInfo<EditorProjectWillBeClosedEvent>::UID())
        {
            AssetManager::GetInstancePtr()->removeAssetsDirectoryPath(EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies");
        }
        else
        if (_eventUID == ClassInfo<CSharpAppAssemblyLoadedEvent>::UID())
        {
            StringKeyMap<ScriptClassPtr> const& monoBehaviourSubClasses = MonoEngine::GetMonoBehaviourSubClasses();
            for (auto const& data : monoBehaviourSubClasses)
            {
                InspectorManager::GetInstancePtr()->addAddComponentCallback(
                    "Scripts/" + data.first,
                    [&](EntityPtr const& _entity)
                    {
                        _entity->createComponent<MonoBehaviour>(
                            data.second->getFullName().asHashedCString());
                    });
            }
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

        MAZE_ERROR_RETURN_IF(!SystemHelper::ExecuteSync(generateAssemblyPath, csharpPath), "Failed to generate charp assembly project!");
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

        MAZE_ERROR_RETURN_IF(!SystemHelper::ExecuteSync(compileAssemblyPath, csharpPath), "Failed to compile charp assembly!");
#else
        MAZE_NOT_IMPLEMENTED;
#endif

        Debug::Log("CSharp assembly project compilation finished.");
    }

    //////////////////////////////////////////
    void EditorCSharpManager::loadCSharpAssembly()
    {
        CSharpService::GetInstancePtr()->loadMonoAssembly(MAZE_HCS("Assembly-CSharp.dll"));
    }

} // namespace Maze
//////////////////////////////////////////