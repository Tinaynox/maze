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
    UnorderedMap<String, MonoFieldType> g_monoFieldTypePerMonoTypeName =
    {
        { "System.String", MonoFieldType::TypeString },
        { "System.Boolean", MonoFieldType::TypeBool },
        { "System.Char", MonoFieldType::TypeS8 },
        { "System.Int16", MonoFieldType::TypeS16 },
        { "System.Int32", MonoFieldType::TypeS32 },
        { "System.Int64", MonoFieldType::TypeS64 },
        { "System.Byte", MonoFieldType::TypeU8 },
        { "System.UInt16", MonoFieldType::TypeU16 },
        { "System.UInt32", MonoFieldType::TypeU32 },
        { "System.UInt64", MonoFieldType::TypeU64 },
        { "System.Single", MonoFieldType::TypeF32 },
        { "System.Double", MonoFieldType::TypeF64 },
        { "Maze.Vec2S", MonoFieldType::TypeVec2S },
        { "Maze.Vec3S", MonoFieldType::TypeVec3S },
        { "Maze.Vec4S", MonoFieldType::TypeVec4S },
        { "Maze.Vec2U", MonoFieldType::TypeVec2U },
        { "Maze.Vec3U", MonoFieldType::TypeVec3U },
        { "Maze.Vec4U", MonoFieldType::TypeVec4U },
        { "Maze.Vec2F", MonoFieldType::TypeVec2F },
        { "Maze.Vec3F", MonoFieldType::TypeVec3F },
        { "Maze.Vec4F", MonoFieldType::TypeVec4F },
        { "Maze.Mat3F", MonoFieldType::TypeMat3F },
        { "Maze.Mat4F", MonoFieldType::TypeMat4F },
        { "Maze.TMat", MonoFieldType::TypeTMat },
        { "Maze.Rect2F", MonoFieldType::TypeRect2F },
    };


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
            MonoObject* exception = nullptr;
            mono_runtime_invoke(_method, _instance, _params, &exception);

            if (exception)
            {
                MonoString* excStr = mono_object_to_string(exception, nullptr);
                Char* cstr = mono_string_to_utf8(excStr);
                MAZE_ERROR("MONO runtime error: %s", cstr);
                mono_free(cstr);
            }
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void ParseMonoEntitySystemAttributes(
            MonoMethod* _method,
            Set<HashedString>& _outTags,
            ComponentSystemOrder& _outOrder)
        {
            _outTags.clear();
            _outOrder.after.clear();
            _outOrder.before.clear();

            MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_method(_method);
            if (!attrInfo)
                return;

            for (S32 i = 0; i < attrInfo->num_attrs; ++i)
            {
                MonoCustomAttrEntry& entry = attrInfo->attrs[i];
                MonoClass* attrClass = mono_method_get_class(entry.ctor);

                CString attrClassName = mono_class_get_name(attrClass);
                CString attrNamespace = mono_class_get_namespace(attrClass);
                if (strcmp(attrClassName, "EntitySystemAttribute") == 0 && strcmp(attrNamespace, "Maze") == 0)
                {
                    MonoObject* attrObj = mono_custom_attrs_get_attr(attrInfo, attrClass);
                    if (attrObj)
                    {
                        MonoClassField* tagsField = mono_class_get_field_from_name(attrClass, "Tags");
                        MonoClassField* afterField = mono_class_get_field_from_name(attrClass, "After");
                        MonoClassField* beforeField = mono_class_get_field_from_name(attrClass, "Before");

                        auto parseStringListMonoClassField = [&](MonoClassField* _field)
                        {
                            Set<HashedString> result;

                            MonoArray* array;
                            mono_field_get_value(attrObj, _field, &array);

                            if (!array)
                                return result;

                            S32 arrayLength = (S32)mono_array_length(array);
                            for (S32 i = 0; i < arrayLength; i++)
                            {
                                MonoString* monoStr = mono_array_get(array, MonoString*, i);

                                Char* cStr = mono_string_to_utf8(monoStr);
                                result.emplace(cStr);
                                mono_free(cStr);
                            }

                            return std::move(result);
                        };

                        _outTags = parseStringListMonoClassField(tagsField);
                        _outOrder.after = parseStringListMonoClassField(afterField);
                        _outOrder.before = parseStringListMonoClassField(beforeField);
                    }

                }
            }

            mono_custom_attrs_free(attrInfo);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoFieldType MonoTypeStringToMonoFieldType(String const& _string)
        {
            auto it = g_monoFieldTypePerMonoTypeName.find(_string);
            if (it != g_monoFieldTypePerMonoTypeName.end())
                return it->second;
            return MonoFieldType::None;
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoFieldType MonoTypeToMonoFieldType(MonoType* _monoType)
        {
            String typeName = mono_type_get_name(_monoType);
            return MonoTypeStringToMonoFieldType(typeName);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsMethodPublic(MonoMethod* _method)
        {
            if (_method == nullptr)
                return false;

            uint32_t flags = mono_method_get_flags(_method, nullptr);

            return flags & MONO_METHOD_ATTR_PUBLIC;
        }

    } // namespace AssetHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
