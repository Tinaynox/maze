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
#include "MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/ecs/MazeCustomComponentSystemHolder.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsCore.hpp"
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsGraphics.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/mono/MazeScriptClass.hpp"
#include "maze-plugin-csharp/ecs/components/MazeMonoBehaviour.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API MonoEcsData
    {
        StringKeyMap<ScriptClassPtr> scriptClasses;

        ScriptClassPtr monoBehaviourClass;
        StringKeyMap<ScriptClassPtr> monoBehaviourSubClasses;
        UnorderedMap<ComponentId, ScriptClassPtr> monoBehaviourSubClassesByComponentId;
        Vector<SharedPtr<CustomComponentSystemHolder>> monoBehaviourSystems;

        ScriptClassPtr componentClass;
        ScriptPropertyPtr nativeComponentPtrProperty;
        ScriptClassPtr nativeComponentClass;
        ScriptClassPtr ecsUtilsClass;
        StringKeyMap<ScriptClassPtr> nativeComponentSubClasses;

        UnorderedMap<MonoType*, ComponentId> monoTypePerComponentId;        
    };


    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API MonoEngineData
    {
        MonoDomain* monoDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        HashedString coreAssemblyFilePath;
        MonoAssembly* coreAssembly = nullptr;
        MonoImage* coreAssemblyImage = nullptr;

        HashedString appAssemblyFilePath;
        MonoAssembly* appAssembly = nullptr;
        MonoImage* appAssemblyImage = nullptr;

        bool debugEnabled = false;

        MonoEcsData ecsData;
    };


    //////////////////////////////////////////
    MonoEngineData* g_monoEngineData = nullptr;


    //////////////////////////////////////////
    void LoadAssemblyClasses(MonoAssembly* _assembly)
    {
        Vector<ScriptClassPtr> loadedScriptClasses;

        MonoImage* image = mono_assembly_get_image(_assembly);
        MonoTableInfo const* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        S32 numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (S32 i = 0; i < numTypes; ++i)
        {
            U32 cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            CString typeNamespace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            CString typeName = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            MonoClass* monoClass = mono_class_from_name(
                image,
                typeNamespace,
                typeName);

            if (monoClass)
            {
                String fullName;
                String fullNamespace;
                if (typeNamespace && typeNamespace[0])
                {
                    fullNamespace = typeNamespace;
                    StringHelper::FormatString(fullName, "%s.%s", typeNamespace, typeName);
                }
                else
                    fullName = typeName;

                // MonoBehaviour subclasses
                if (mono_class_is_subclass_of(monoClass, MonoEngine::GetMonoBehaviourClass()->getMonoClass(), false) &&
                    monoClass != MonoEngine::GetMonoBehaviourClass()->getMonoClass())
                {
                    ScriptClassPtr scriptClass = MonoEngine::CreateScriptClass(fullNamespace, typeName, monoClass);
                    scriptClass->assignPrivateProperty(MAZE_HCS("NativeComponentPtr"));

                    loadedScriptClasses.push_back(scriptClass);

                    g_monoEngineData->ecsData.scriptClasses.insert(
                        HashedCString(fullName.c_str()),
                        scriptClass);

                    g_monoEngineData->ecsData.monoBehaviourSubClasses.insert(
                        HashedCString(fullName.c_str()),
                        scriptClass);

                    ComponentId componentId = GetComponentIdByName(fullName.c_str());
                    g_monoEngineData->ecsData.monoBehaviourSubClassesByComponentId.emplace(
                        componentId,
                        scriptClass);

                    MonoType* monoType = mono_class_get_type(monoClass);
                    g_monoEngineData->ecsData.monoTypePerComponentId[monoType] = GetComponentIdByName(fullName.c_str());
                    

                    // Process entity systems
                    Set<HashedString> systemTags;
                    ComponentSystemOrder systemOrder;

                    if (scriptClass->getOnCreateMethod())
                    {
                        HashedString systemName(fullName + "::OnCreate");

                        MonoHelper::ParseMonoEntitySystemAttributes(scriptClass->getOnCreateMethod(), systemTags, systemOrder);

                        systemTags.insert(MAZE_HS("default"));

                        g_monoEngineData->ecsData.monoBehaviourSystems.emplace_back(
                            MakeShared<CustomComponentSystemHolder>(
                                systemName,
                                ClassInfo<EntityAddedEvent>::UID(),
                                [componentId](EcsWorld* _world) { return _world->requestDynamicIdSample<MonoBehaviour>(componentId); },
                                (ComponentSystemEventHandler::Func)&MonoBehaviourOnCreate,
                                systemTags,
                                systemOrder));
                    }

                    if (scriptClass->getOnUpdateMethod())
                    {
                        HashedString systemName(fullName + "::OnUpdate");

                        MonoHelper::ParseMonoEntitySystemAttributes(scriptClass->getOnUpdateMethod(), systemTags, systemOrder);

                        systemTags.insert(MAZE_HS("default"));

                        g_monoEngineData->ecsData.monoBehaviourSystems.emplace_back(
                            MakeShared<CustomComponentSystemHolder>(
                                systemName,
                                ClassInfo<UpdateEvent>::UID(),
                                [componentId](EcsWorld* _world) { return _world->requestDynamicIdSample<MonoBehaviour>(componentId); },
                                (ComponentSystemEventHandler::Func)&MonoBehaviourOnUpdate,
                                systemTags,
                                systemOrder));
                    }

                    
                    {
                        HashedString systemName(fullName + "::OnDestroy");

                        if (scriptClass->getOnDestroyMethod())
                            MonoHelper::ParseMonoEntitySystemAttributes(scriptClass->getOnDestroyMethod(), systemTags, systemOrder);
                        else
                        {
                            systemTags.clear();
                            systemOrder = ComponentSystemOrder();
                        }

                        systemTags.insert(MAZE_HS("default"));

                        g_monoEngineData->ecsData.monoBehaviourSystems.emplace_back(
                            MakeShared<CustomComponentSystemHolder>(
                                systemName,
                                ClassInfo<EntityRemovedEvent>::UID(),
                                [componentId](EcsWorld* _world) { return _world->requestDynamicIdSample<MonoBehaviour>(componentId); },
                                (ComponentSystemEventHandler::Func)&MonoBehaviourOnDestroy,
                                systemTags,
                                systemOrder));
                    }
                }
                else
                // NativeComponent subclasses
                if (mono_class_is_subclass_of(monoClass, MonoEngine::GetNativeComponentClass()->getMonoClass(), false) &&
                    monoClass != MonoEngine::GetNativeComponentClass()->getMonoClass())
                {
                    ScriptClassPtr scriptClass = MonoEngine::CreateScriptClass(fullNamespace, typeName, monoClass);
                    scriptClass->assignPrivateProperty(MAZE_HCS("NativeComponentPtr"));

                    loadedScriptClasses.push_back(scriptClass);

                    g_monoEngineData->ecsData.nativeComponentSubClasses.insert(
                        HashedCString(fullName.c_str()),
                        scriptClass);

                    MonoType* monoType = mono_class_get_type(monoClass);
                    g_monoEngineData->ecsData.monoTypePerComponentId[monoType] = GetComponentIdByName((String("Maze::") + typeName).c_str());
                }
            }
        }

        for (ScriptClassPtr const& scriptClass : loadedScriptClasses)
            scriptClass->postInit();
    }



    //////////////////////////////////////////
    // Class MonoEngine
    //
    //////////////////////////////////////////
    bool MonoEngine::Initialize()
    {
        Debug::Log("MonoEngine - initializing...");

        g_monoEngineData = MAZE_NEW(MonoEngineData);

        if (!InitializeMonoPath())
            return false;

        if (!InitializeMono())
            return false;

        LoadCoreAssembly(MAZE_HCS("maze-csharp-core-lib.dll"));

        return true;
    }

    //////////////////////////////////////////
    bool MonoEngine::Shutdown()
    {
        Debug::Log("MonoEngine - shutdowning...");

        ShutdownMono();

        MAZE_SAFE_DELETE(g_monoEngineData);

        return true;
    }

    //////////////////////////////////////////
    bool MonoEngine::ReloadAssemblies()
    {
        ShutdownMono();
        if (!InitializeMono())
            return false;

        LoadCoreAssembly(g_monoEngineData->coreAssemblyFilePath);

        if (!g_monoEngineData->appAssemblyFilePath.empty())
            LoadAppAssembly(g_monoEngineData->appAssemblyFilePath);

        EventManager::GetInstancePtr()->broadcastEventImmediate<MonoPreReloadEvent>();
        EventManager::GetInstancePtr()->broadcastEventImmediate<MonoReloadEvent>();

        return true;
    }

    //////////////////////////////////////////
    bool MonoEngine::InitializeMonoPath()
    {
        Debug::Log("MonoEngine - initializing Mono...");

        MAZE_ERROR_RETURN_VALUE_IF(!AssetManager::GetInstancePtr(), false, "AssetManager is not initialized!");

        AssetFilePtr const& assetLibsDir = AssetManager::GetInstancePtr()->getAssetFileByFileName(MAZE_HCS("MonoLibs"));
        MAZE_ERROR_RETURN_VALUE_IF(!assetLibsDir, false, "MonoLibs dir is not found!");

        Path const& assetLibsDirPath = assetLibsDir->getFullPath();
        String assetLibsDirPathUTF8 = assetLibsDirPath.toUTF8() + "/4.5";

        Debug::Log("Asset Libs Dir: %s", assetLibsDirPathUTF8.c_str());

        mono_set_assemblies_path(assetLibsDirPathUTF8.c_str());

        MonoDomain* rootDomain = mono_jit_init("MazeJITRuntime");
        MAZE_ERROR_RETURN_VALUE_IF(!rootDomain, false, "Failed to initialize mono jit!");

        // Store the root domain pointer
        g_monoEngineData->monoDomain = rootDomain;

        if (g_monoEngineData->debugEnabled)
            mono_debug_domain_create(g_monoEngineData->monoDomain);

        mono_thread_set_main(mono_thread_current());

        return true;
    }

    //////////////////////////////////////////
    bool MonoEngine::InitializeMono()
    {
        if (g_monoEngineData->debugEnabled)
        {
            CString argv[2] =
            {
                "--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
                "--soft-breakpoints"
            };

            mono_jit_parse_options(2, (Char**)argv);
            mono_debug_init(MONO_DEBUG_FORMAT_MONO);
        }

        g_monoEngineData->appDomain = mono_domain_create_appdomain("MazeCSharpRuntime", nullptr);
        MAZE_ERROR_RETURN_VALUE_IF(!g_monoEngineData->appDomain, false, "Failed to create app domain!");
        mono_domain_set(g_monoEngineData->appDomain, true);

        BindCppFunctionsCore();
        BindCppFunctionsGraphics();

        return true;
    }

    //////////////////////////////////////////
    void MonoEngine::ShutdownMono()
    {
        EventManager::GetInstancePtr()->broadcastEventImmediate<MonoPreShutdownEvent>();
        EventManager::GetInstancePtr()->broadcastEventImmediate<MonoShutdownEvent>();

        mono_domain_set(mono_get_root_domain(), false);
        if (g_monoEngineData->appDomain)
            mono_domain_unload(g_monoEngineData->appDomain);

        g_monoEngineData->ecsData = MonoEcsData();

        g_monoEngineData->appAssembly = nullptr;
        g_monoEngineData->appAssemblyImage = nullptr;

        g_monoEngineData->coreAssembly = nullptr;
        g_monoEngineData->coreAssemblyImage = nullptr;

        g_monoEngineData->appDomain = nullptr;

        
    }

    //////////////////////////////////////////
    MonoAssembly* MonoEngine::LoadMonoAssembly(
        ByteBuffer const& _csharpFile,
        ByteBufferPtr const& _csharpPdb)
    {
        // NOTE: We can't use this image for anything other than loading the assembly
        // because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(
            (Char*)_csharpFile.getDataRO(),
            _csharpFile.getSize(),
            1,
            &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            CString errorMessage = mono_image_strerror(status);
            Debug::LogError(errorMessage);
            return nullptr;
        }

        if (_csharpPdb)
        {
            mono_debug_open_image_from_memory(
                image,
                _csharpPdb->getDataRO(),
                _csharpPdb->getSize());
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, "", &status, 0);
        mono_image_close(image);

        return assembly;
    }

    //////////////////////////////////////////
    MonoAssembly* MonoEngine::LoadMonoAssembly(AssetFilePtr const& _csharpFile)
    {
        if (!_csharpFile)
            return nullptr;

        ByteBufferPtr byteBuffer = _csharpFile->readAsByteBuffer();
        if (!byteBuffer)
            return nullptr;

        ByteBufferPtr pdbBuffer;
        if (g_monoEngineData->debugEnabled)
        {
            Path pdbPath = FileHelper::GetPathWithoutExtension(_csharpFile->getFullPath()) + ".pdb";
            AssetFilePtr const& csharpPdbFile = AssetManager::GetInstancePtr()->getAssetFileByFullPath(pdbPath);
            if (csharpPdbFile)
                pdbBuffer = csharpPdbFile->readAsByteBuffer();
        }

        MonoAssembly* assembly = LoadMonoAssembly(
            *byteBuffer.get(),
            pdbBuffer);
        if (!assembly)
            return nullptr;

        return assembly;
    }

    //////////////////////////////////////////
    MonoAssembly* MonoEngine::LoadMonoAssembly(HashedCString _csharpFile)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_csharpFile);
        if (!assetFile)
            return nullptr;

        return LoadMonoAssembly(assetFile);
    }

    //////////////////////////////////////////
    MonoAssembly* MonoEngine::LoadCoreAssembly(HashedCString _csharpFile)
    {
        g_monoEngineData->coreAssemblyFilePath = _csharpFile;
        g_monoEngineData->coreAssembly = LoadMonoAssembly(_csharpFile);
        MAZE_ERROR_RETURN_VALUE_IF(!g_monoEngineData->coreAssembly, nullptr, "Failed to load core assembly - %s", _csharpFile.str);

        g_monoEngineData->coreAssemblyImage = mono_assembly_get_image(g_monoEngineData->coreAssembly);
        g_monoEngineData->ecsData.monoBehaviourClass = MonoEngine::CreateScriptClass(
            "Maze.Core", "MonoBehaviour", g_monoEngineData->coreAssemblyImage);
        g_monoEngineData->ecsData.componentClass = MonoEngine::CreateScriptClass(
            "Maze.Core", "Component", g_monoEngineData->coreAssemblyImage);
        g_monoEngineData->ecsData.nativeComponentPtrProperty = g_monoEngineData->ecsData.componentClass->getProperty(
            MAZE_HCS("NativeComponentPtr"));
        g_monoEngineData->ecsData.nativeComponentClass = MonoEngine::CreateScriptClass(
            "Maze.Core", "NativeComponent", g_monoEngineData->coreAssemblyImage);
        g_monoEngineData->ecsData.ecsUtilsClass = MonoEngine::CreateScriptClass(
            "Maze.Core", "EcsUtils", g_monoEngineData->coreAssemblyImage);

        LoadAssemblyClasses(g_monoEngineData->coreAssembly);

        EventManager::GetInstancePtr()->broadcastEventImmediate<CSharpCoreAssemblyLoadedEvent>();

        return g_monoEngineData->coreAssembly;
    }

    //////////////////////////////////////////
    MonoAssembly* MonoEngine::LoadAppAssembly(HashedCString _csharpFile)
    {
        g_monoEngineData->appAssemblyFilePath = _csharpFile;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_csharpFile);
        if (!assetFile)
            return nullptr;

        g_monoEngineData->appAssembly = LoadMonoAssembly(assetFile);
        MAZE_ERROR_RETURN_VALUE_IF(!g_monoEngineData->appAssembly, nullptr, "Failed to load mono assembly: %s!");
        g_monoEngineData->appAssemblyImage = mono_assembly_get_image(g_monoEngineData->coreAssembly);

        LoadAssemblyClasses(g_monoEngineData->appAssembly);

        EventManager::GetInstancePtr()->broadcastEventImmediate<CSharpAppAssemblyLoadedEvent>();

        return g_monoEngineData->appAssembly;
    }

    //////////////////////////////////////////
    MonoDomain* MonoEngine::GetMonoDomain()
    {
        return g_monoEngineData->appDomain;
    }

    //////////////////////////////////////////
    MonoAssembly* MonoEngine::GetCoreAssembly()
    {
        return g_monoEngineData->coreAssembly;
    }

    //////////////////////////////////////////
    MonoImage* MonoEngine::GetCoreAssemblyImage()
    {
        return g_monoEngineData->coreAssemblyImage;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetMonoBehaviourClass()
    {
        return g_monoEngineData->ecsData.monoBehaviourClass;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetComponentClass()
    {
        return g_monoEngineData->ecsData.componentClass;
    }

    //////////////////////////////////////////
    ScriptPropertyPtr const& MonoEngine::GetNativeComponentPtrProperty()
    {
        return g_monoEngineData->ecsData.nativeComponentPtrProperty;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetNativeComponentClass()
    {
        return g_monoEngineData->ecsData.nativeComponentClass;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetEcsUtilsClass()
    {
        return g_monoEngineData->ecsData.ecsUtilsClass;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetScriptClass(HashedCString _name)
    {
        static ScriptClassPtr const nullPointer;

        auto it = g_monoEngineData->ecsData.scriptClasses.find(_name);
        if (it != g_monoEngineData->ecsData.scriptClasses.end())
            return it->second;
        else
            return nullPointer;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetMonoBehaviourSubClass(HashedCString _name)
    {
        static ScriptClassPtr const nullPointer;

        auto it = g_monoEngineData->ecsData.monoBehaviourSubClasses.find(_name);
        if (it != g_monoEngineData->ecsData.monoBehaviourSubClasses.end())
            return it->second;
        else
            return nullPointer;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetMonoBehaviourSubClass(ComponentId _id)
    {
        static ScriptClassPtr const nullPointer;

        auto it = g_monoEngineData->ecsData.monoBehaviourSubClassesByComponentId.find(_id);
        if (it != g_monoEngineData->ecsData.monoBehaviourSubClassesByComponentId.end())
            return it->second;
        else
            return nullPointer;
    }

    //////////////////////////////////////////
    StringKeyMap<ScriptClassPtr> const& MonoEngine::GetMonoBehaviourSubClasses()
    {
        return g_monoEngineData->ecsData.monoBehaviourSubClasses;
    }

    //////////////////////////////////////////
    ComponentId MonoEngine::GetComponentIdByMonoType(MonoType* _monoType)
    {
        auto it = g_monoEngineData->ecsData.monoTypePerComponentId.find(_monoType);
        if (it != g_monoEngineData->ecsData.monoTypePerComponentId.end())
            return it->second;
        else
            return c_invalidComponentId;
    }

    //////////////////////////////////////////
    MonoObject* MonoEngine::InstantiateClass(MonoClass* _monoClass)
    {
        MonoObject* instance = mono_object_new(g_monoEngineData->appDomain, _monoClass);
        MAZE_ERROR_RETURN_VALUE_IF(!instance, nullptr, "Failed to instatiate class!");
        mono_runtime_object_init(instance);

        MAZE_ERROR_RETURN_VALUE_IF(mono_object_get_class(instance) == nullptr, nullptr, "Instance is not instantiated!");
        

        return instance;
    }

    //////////////////////////////////////////
    ScriptClassPtr MonoEngine::CreateScriptClass(
        String const& _namespace,
        String const& _className,
        MonoClass* _monoClass)
    {
        ScriptClassPtr scriptClass = ScriptClass::Create(_namespace, _className, _monoClass);

        g_monoEngineData->ecsData.scriptClasses.insert(
            scriptClass->getFullName(),
            scriptClass);

        return scriptClass;
    }

    //////////////////////////////////////////
    ScriptClassPtr MonoEngine::CreateScriptClass(
        String const& _namespace,
        String const& _className,
        MonoImage* _monoImage)
    {
        ScriptClassPtr scriptClass = ScriptClass::Create(_namespace, _className, _monoImage);

        g_monoEngineData->ecsData.scriptClasses.insert(
            scriptClass->getFullName(),
            scriptClass);

        return scriptClass;
    }

} // namespace Maze
//////////////////////////////////////////
