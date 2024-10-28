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
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MonoHelper
    {
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void PrintAssemblyTypes(MonoAssembly* _assembly)
        {
            MonoImage* image = mono_assembly_get_image(_assembly);
            MonoTableInfo const* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
            S32 numTypes = mono_table_info_get_rows(typeDefinitionsTable);

            Debug::Log("Assembly types: ");
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
                    Debug::Log(
                        "\t%s.%s %s",
                        typeNamespace,
                        typeName,
                        isMonoBehaviour ? "[MB]" : "");
                }
            }
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void InvokeMethod(MonoObject* _instance, MonoMethod* _method, void** _params)
        {
            mono_runtime_invoke(_method, _instance, _params, nullptr);
        }

    } // namespace AssetHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
