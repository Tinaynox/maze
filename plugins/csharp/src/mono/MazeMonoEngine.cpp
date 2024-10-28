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
#include "maze-plugin-csharp/mono-binds/MazeMonoBindsCore.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/mono/MazeScriptClass.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    struct MAZE_PLUGIN_CSHARP_API MonoEngineData
    {
        MonoDomain* monoDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        MonoAssembly* coreAssembly = nullptr;
        MonoImage* coreAssemblyImage = nullptr;

        ScriptClassPtr monoBehaviourClass;
        StringKeyMap<ScriptClassPtr> monoBehaviourSubClasses;
    };


    //////////////////////////////////////////
    MonoEngineData g_monoEngineData;


    //////////////////////////////////////////
    void LoadAssemblyClasses(MonoAssembly* _assembly)
    {
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
                MonoEngine::GetCoreAssemblyImage(),
                typeNamespace,
                typeName);

            if (monoClass)
            {
                bool isMonoBehaviour = mono_class_is_subclass_of(
                    monoClass,
                    MonoEngine::GetMonoBehaviourClass()->getMonoClass(),
                    false);
                
                if (isMonoBehaviour && monoClass != MonoEngine::GetMonoBehaviourClass()->getMonoClass())
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

                    g_monoEngineData.monoBehaviourSubClasses.insert(
                        HashedCString(fullName.c_str()),
                        MakeShared<ScriptClass>(fullNamespace, typeName, monoClass));
                }
            }
        }
    }



    //////////////////////////////////////////
    // Class MonoEngine
    //
    //////////////////////////////////////////
    bool MonoEngine::Initialize()
    {
        Debug::Log("MonoEngine - initializing...");

        if (!InitializeMono())
            return false;

        BindCppFunctionsCore();

        g_monoEngineData.coreAssembly = LoadMonoAssembly(MAZE_HCS("maze-csharp-core-lib.dll"));
        g_monoEngineData.coreAssemblyImage = mono_assembly_get_image(g_monoEngineData.coreAssembly);
        g_monoEngineData.monoBehaviourClass = MakeShared<ScriptClass>("Maze", "MonoBehaviour", g_monoEngineData.coreAssemblyImage);

        LoadAssemblyClasses(g_monoEngineData.coreAssembly);


        // Tests
        ScriptInstance monoBeh = g_monoEngineData.monoBehaviourClass->instantiate();
        monoBeh.invokeMethod("OnCreate");
        monoBeh.invokeMethod("OnUpdate", 1.0f / 60.0f);


        return true;
    }

    //////////////////////////////////////////
    bool MonoEngine::Shutdown()
    {
        Debug::Log("MonoEngine - shutdowning...");

        ShutdownMono();

        return true;
    }

    //////////////////////////////////////////
    bool MonoEngine::InitializeMono()
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
        g_monoEngineData.monoDomain = rootDomain;

        g_monoEngineData.appDomain = mono_domain_create_appdomain("MazeCSharpRuntime", nullptr);
        MAZE_ERROR_RETURN_VALUE_IF(!rootDomain, false, "Failed to create app domain!");
        mono_domain_set(g_monoEngineData.appDomain, true);

        return true;
    }

    //////////////////////////////////////////
    void MonoEngine::ShutdownMono()
    {
        g_monoEngineData.monoDomain = nullptr;
        g_monoEngineData.appDomain = nullptr;
        g_monoEngineData.coreAssembly = nullptr;
    }
    
    //////////////////////////////////////////
    MonoAssembly* MonoEngine::LoadMonoAssembly(ByteBuffer const& _csharpFile)
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

        return LoadMonoAssembly(*byteBuffer.get());
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
    MonoDomain* MonoEngine::GetMonoDomain()
    {
        return g_monoEngineData.appDomain;
    }

    //////////////////////////////////////////
    MonoAssembly* MonoEngine::GetCoreAssembly()
    {
        return g_monoEngineData.coreAssembly;
    }

    //////////////////////////////////////////
    MonoImage* MonoEngine::GetCoreAssemblyImage()
    {
        return g_monoEngineData.coreAssemblyImage;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetMonoBehaviourClass()
    {
        return g_monoEngineData.monoBehaviourClass;
    }

    //////////////////////////////////////////
    ScriptClassPtr const& MonoEngine::GetMonoBehaviourSubClass(HashedCString _name)
    {
        static ScriptClassPtr const nullPointer;

        auto it = g_monoEngineData.monoBehaviourSubClasses.find(_name);
        if (it != g_monoEngineData.monoBehaviourSubClasses.end())
            return it->second;
        else
            return nullPointer;
    }

    //////////////////////////////////////////
    MonoObject* MonoEngine::InstantiateClass(MonoClass* _monoClass)
    {
        MonoObject* instance = mono_object_new(g_monoEngineData.appDomain, _monoClass);
        mono_runtime_object_init(instance);

        return instance;
    }

} // namespace Maze
//////////////////////////////////////////
