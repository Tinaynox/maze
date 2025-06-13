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
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-plugin-csharp/MazeCSharpService.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-plugin-csharp/managers/MazeScriptableObjectManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "mono-binds/MonoBindsEditor.hpp"
#include "events/EditorEvents.hpp"
#include "helpers/EditorProjectHelper.hpp"
#include "helpers/EditorAssetHelper.hpp"
#include "managers/EditorUIManager.hpp"
#include "managers/EditorAssetsManager.hpp"
#include "Editor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    U32 CalculateCSharpScriptsHash(Path _path)
    {
        U32 hash = 0u;
        Vector<Path> files = FileHelper::GetRegularFileNamesInPath(_path);
        for (Path const& path : files)
        {
            Path subPath = _path + Path("/") + path;
            if (FileHelper::IsDirectory(subPath))
            {
                U32 directoryHash = CalculateCSharpScriptsHash(subPath);
                if (directoryHash != 0u)
                    hash ^= directoryHash;
            }
            else
            if (FileHelper::GetFileExtension(path) == "cs")
            {
                U32 fileHash = Hash::CalculateFNV1((CString)subPath.c_str(), subPath.size() * sizeof(Path::CharType));
                hash ^= fileHash;
            }
        }
        return hash;
    }


    //////////////////////////////////////////
    U32 CalculateCSharpScriptsHash()
    {
        return CalculateCSharpScriptsHash(EditorHelper::GetProjectAssetsFolder());
    }


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
            EventManager::GetInstancePtr()->unsubscribeEvent<CSharpAssemblyLoadedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoInitializationEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<PlaytestModePrepareEvent>(this);
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
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        EventManager::GetInstancePtr()->subscribeEvent<EditorProjectOpenedEvent>(this, &EditorCSharpManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<EditorProjectWillBeClosedEvent>(this, &EditorCSharpManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<CSharpAssemblyLoadedEvent>(this, &EditorCSharpManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<MonoInitializationEvent>(this, &EditorCSharpManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &EditorCSharpManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<PlaytestModePrepareEvent>(this, &EditorCSharpManager::notifyEvent);

        /*
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
        */
        
        EditorUIManager::GetInstancePtr()->addTopBarOption(
            "Scripts",
            "Open project",
            [](String const& _text)
            {
                Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharpPrj/prj/Assembly-CSharp.sln";
                SystemHelper::OpenURL(csharpPath);
            },
            []() { return true; });

        EditorUIManager::GetInstancePtr()->addTopBarOption(
            "Scripts",
            "Reload scripts",
            [](String const& _text)
            {
                if (EditorCSharpManager::GetInstancePtr())
                {
                    U32 csharpScriptsHash = CalculateCSharpScriptsHash();
                    if (csharpScriptsHash != EditorCSharpManager::GetInstancePtr()->m_csharpScriptsHash)
                        EditorCSharpManager::GetInstancePtr()->generateCSharpAssembly();
                    
                    EditorCSharpManager::GetInstancePtr()->compileCSharpAssembly();
                    EditorCSharpManager::GetInstancePtr()->reloadCSharpScripts();
                }
            },
            []() { return true; });

        EditorAssetsManager::GetInstancePtr()->addAssetContextOption(
            MAZE_HCS("cs"),
            "Edit",
            [](AssetsController* _controller, Path const& _fullPath)
            {
                // #TODO: move to setting
                Path devenv = "F:/Program Files (x86)/Microsoft Visual Studio/2019/Community/Common7/IDE/devenv.exe";

                Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharpPrj/prj/Assembly-CSharp.sln";
                Path params = csharpPath + Path(" ") + _fullPath;
                SystemHelper::ExecuteShell(devenv, params);
            });

        AssetEditorToolsManager::GetInstancePtr()->registerAssetFileContextMenuCallback(
            [](AssetsController* _controller, Path const& _fullPath, MenuListTree2DPtr const& _menuListTree)
            {
                _menuListTree->addItem(
                    "Create/Script",
                    [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreateScript(_controller, _fullPath); },
                    nullptr,
                    false);
            });

        return true;
    }

    //////////////////////////////////////////
    void EditorCSharpManager::update(F32 _dt)
    {
        if (m_scriptActionsBlockedUntil < UpdateManager::GetInstancePtr()->getAppTime())
        {
            
        }
    }

    //////////////////////////////////////////
    void EditorCSharpManager::updateAndReloadScriptsIfRequired()
    {
        if (Editor::GetInstancePtr()->getMainRenderWindow() &&
            Editor::GetInstancePtr()->getMainRenderWindow()->getWindow())
        {
            if (m_csharpScriptsChanged)
            {
                m_csharpScriptsChanged = false;
                U32 csharpScriptsHash = CalculateCSharpScriptsHash();
                if (csharpScriptsHash != m_csharpScriptsHash)
                    generateCSharpAssembly();
            }

            if (m_csharpScriptsRecompileRequired)
            {
                m_csharpScriptsRecompileRequired = false;
                compileCSharpAssembly();
                m_scriptAssembliesReloadRequired = true;
            }
        }

        if (m_scriptAssembliesReloadRequired)
        {
            m_scriptAssembliesReloadRequired = false;
            reloadCSharpScripts();
        }
    }

    //////////////////////////////////////////
    void EditorCSharpManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoInitializationEvent>::UID())
        {
            bindCppFunctionsEditor();
        }
        else
        if (_eventUID == ClassInfo<EditorProjectOpenedEvent>::UID())
        {
            Path scriptAssembliesPath = EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies";
            Path csharpScriptsPath = EditorHelper::GetProjectAssetsFolder();

            updateLibraryFolder();
            AssetManager::GetInstancePtr()->addAssetsDirectoryPath(scriptAssembliesPath);

            updateCSharpFolder();
            generateCSharpAssembly();
            compileCSharpAssembly();

            loadCSharpAssembly();

            StdWString scriptAssembliesWatchPath(
                scriptAssembliesPath.getPath().begin(),
                scriptAssembliesPath.getPath().end());
            m_scriptAssembliesWatch = MakeUnique<MazeFileWatch>(
                scriptAssembliesWatchPath,
                EditorCSharpManager::NotifyScriptAssembliesWatch);

            StdWString csharpScriptsWatchPath(
                csharpScriptsPath.getPath().begin(),
                csharpScriptsPath.getPath().end());
            m_csharpScriptsWatch = MakeUnique<MazeFileWatch>(
                csharpScriptsWatchPath,
                EditorCSharpManager::NotifyCSharpScriptsWatch);
        }
        else
        if (_eventUID == ClassInfo<EditorProjectWillBeClosedEvent>::UID())
        {
            AssetManager::GetInstancePtr()->removeAssetsDirectoryPath(EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies");
        }
        else
        if (_eventUID == ClassInfo<CSharpAssemblyLoadedEvent>::UID())
        {
            // #TODO: Rework via dirty flag
            InspectorManager::GetInstancePtr()->removeAddComponentCallbacks();

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
        else
        if (_eventUID == ClassInfo<MonoPreShutdownEvent>::UID())
        {
            StringKeyMap<ScriptClassPtr> const& monoBehaviourSubClasses = MonoEngine::GetMonoBehaviourSubClasses();
            for (auto const& data : monoBehaviourSubClasses)
            {
                // InspectorManager::GetInstancePtr()->remove
            }
        }
        else
        if (_eventUID == ClassInfo<PlaytestModePrepareEvent>::UID())
        {
            updateAndReloadScriptsIfRequired();
        }
    }

    //////////////////////////////////////////
    void EditorCSharpManager::updateCSharpFolder()
    {
        Debug::Log("Updating CSharp folder...");

        AssetFilePtr const& csharp = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("CSharpPrj"));
        MAZE_ERROR_RETURN_IF(!csharp || csharp->getClassUID() != ClassInfo<AssetDirectory>::UID(), "CSharp dir is not found");

        Path const& csharpSrcPath = csharp->getFullPath();
        Path csharpDestPath = EditorHelper::GetProjectFolder() + "/CSharpPrj";
        FileHelper::CreateDirectoryRecursive(csharpDestPath);

        FileHelper::CopyDirectory(csharpSrcPath, csharpDestPath);
    }

    //////////////////////////////////////////
    void EditorCSharpManager::updateLibraryFolder()
    {
        Debug::Log("Updating Library folder...");

        // Maze lib
        {
            AssetFilePtr const& csharpCoreLib = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("maze-csharp-core-lib.dll"));
            MAZE_ERROR_RETURN_IF(!csharpCoreLib || csharpCoreLib->getClassUID() != ClassInfo<AssetRegularFile>::UID(), "CSharp core lib is not found");

            Path const& csharpCoreLibPath = csharpCoreLib->getFullPath();
            Path csharpDestPath = EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies";
            FileHelper::CreateDirectoryRecursive(csharpDestPath);
            FileHelper::CopyRegularFile(csharpCoreLibPath, csharpDestPath + "/" + csharpCoreLib->getFileName());
        }

        {
            AssetFilePtr const& csharpCoreLibPdb = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("maze-csharp-core-lib.pdb"));
            if (csharpCoreLibPdb && csharpCoreLibPdb->getClassUID() == ClassInfo<AssetRegularFile>::UID())
            {
                Path const& csharpCoreLibPath = csharpCoreLibPdb->getFullPath();
                Path csharpDestPath = EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies";
                FileHelper::CreateDirectoryRecursive(csharpDestPath);
                FileHelper::CopyRegularFile(csharpCoreLibPath, csharpDestPath + "/" + csharpCoreLibPdb->getFileName());
            }
        }


        // Editor lib
        {
            AssetFilePtr const& csharpEditorLib = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("maze-editor-lib.dll"));
            MAZE_ERROR_RETURN_IF(!csharpEditorLib || csharpEditorLib->getClassUID() != ClassInfo<AssetRegularFile>::UID(), "CSharp editor lib is not found");

            Path const& csharpEditorLibPath = csharpEditorLib->getFullPath();
            Path csharpDestPath = EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies";
            FileHelper::CreateDirectoryRecursive(csharpDestPath);
            FileHelper::CopyRegularFile(csharpEditorLibPath, csharpDestPath + "/" + csharpEditorLib->getFileName());
        }

        {
            AssetFilePtr const& csharpEditorLibPdb = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("maze-editor-lib.pdb"));
            if (csharpEditorLibPdb && csharpEditorLibPdb->getClassUID() == ClassInfo<AssetRegularFile>::UID())
            {
                Path const& csharpEditorLibPath = csharpEditorLibPdb->getFullPath();
                Path csharpDestPath = EditorHelper::GetProjectFolder() + "/Library/ScriptAssemblies";
                FileHelper::CreateDirectoryRecursive(csharpDestPath);
                FileHelper::CopyRegularFile(csharpEditorLibPath, csharpDestPath + "/" + csharpEditorLibPdb->getFileName());
            }
        }
    }

    //////////////////////////////////////////
    void EditorCSharpManager::generateCSharpAssembly()
    {
        Debug::Log("Generating CSharp assembly project...");

        Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharpPrj";

        m_csharpScriptsHash = CalculateCSharpScriptsHash();

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

        Path csharpPath = EditorHelper::GetProjectFolder() + "/CSharpPrj";

#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
#   if MAZE_DEBUG
        Path compileAssemblyPath = csharpPath + "/compile_debug.bat";
#   else
        Path compileAssemblyPath = csharpPath + "/compile_release.bat";
#   endif

        MAZE_ERROR_RETURN_IF(!SystemHelper::ExecuteSync(compileAssemblyPath, csharpPath), "Failed to compile charp assembly!");
#else
        MAZE_NOT_IMPLEMENTED;
#endif

        Debug::Log("CSharp assembly project compilation finished.");
    }

    //////////////////////////////////////////
    void EditorCSharpManager::loadCSharpAssembly()
    {
        CSharpService::GetInstancePtr()->loadAppAssembly(MAZE_HCS("Assembly-CSharp.dll"));
    }

    //////////////////////////////////////////
    void EditorCSharpManager::reloadCSharpScripts()
    {
        MonoEngine::ReloadAssemblies();
    }

    //////////////////////////////////////////
    void EditorCSharpManager::processScriptAssembliesModified()
    {
        m_scriptAssembliesReloadRequired = true;
        m_scriptActionsBlockedUntil = UpdateManager::GetInstancePtr()->getAppTime() + 0.1f;
    }

    //////////////////////////////////////////
    void EditorCSharpManager::processCSharpScriptsModified()
    {
        m_csharpScriptsRecompileRequired = true;
        m_scriptActionsBlockedUntil = UpdateManager::GetInstancePtr()->getAppTime() + 0.1f;
    }

    //////////////////////////////////////////
    void EditorCSharpManager::processCSharpScriptsChanged()
    {
        m_csharpScriptsChanged = true;
        m_scriptActionsBlockedUntil = UpdateManager::GetInstancePtr()->getAppTime() + 0.1f;
    }

    //////////////////////////////////////////
    void EditorCSharpManager::bindCppFunctionsEditor()
    {
        BindCppFunctionsEditor();
    }

    //////////////////////////////////////////
    void EditorCSharpManager::NotifyScriptAssembliesWatch(
        typename MazeFileWatchStringType const& _path,
        filewatch::Event const _changeType)
    {
        if (_changeType == filewatch::Event::modified)
        {
            TaskManager::GetInstancePtr()->addMainThreadTask(
                []()
                {
                    if (EditorCSharpManager::GetInstancePtr())
                        EditorCSharpManager::GetInstancePtr()->processScriptAssembliesModified();
                });
        }
    }

    //////////////////////////////////////////
    void EditorCSharpManager::NotifyCSharpScriptsWatch(
        typename MazeFileWatchStringType const& _path,
        filewatch::Event const _changeType)
    {

#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
        if (StringHelper::IsEndsWith(_path.c_str(), L".cs"))
#else
        if (StringHelper::IsEndsWith(_path.c_str(), ".cs"))
#endif
        {
            if (_changeType == filewatch::Event::modified)
            {
                TaskManager::GetInstancePtr()->addMainThreadTask(
                    []()
                    {
                        if (EditorCSharpManager::GetInstancePtr())
                            EditorCSharpManager::GetInstancePtr()->processCSharpScriptsModified();
                    });
            }
            else
            {
                TaskManager::GetInstancePtr()->addMainThreadTask(
                    []()
                    {
                        if (EditorCSharpManager::GetInstancePtr())
                            EditorCSharpManager::GetInstancePtr()->processCSharpScriptsChanged();
                    });
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
