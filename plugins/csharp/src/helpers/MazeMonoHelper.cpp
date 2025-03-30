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
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-plugin-csharp/mono/MazeScriptField.hpp"
#include "maze-core/ecs/helpers/MazeEcsHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MonoHelper
    {
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API HashedString BuildMonoClassFullName(
            MonoClass* _class)
        {
            CString className = mono_class_get_name(_class);
            CString classNamespace = mono_class_get_namespace(_class);

            return BuildMonoClassFullName(classNamespace, className);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API HashedString BuildMonoClassFullName(
            CString _namespace,
            CString _className)
        {
            if (_namespace && _namespace[0])
            {
                String result;
                StringHelper::FormatString(result, "%s.%s", _namespace, _className);
                return HashedString(result);
            }
            else
                return HashedString(_className);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void PrintAssemblyTypes(MonoAssembly* _assembly)
        {
            MAZE_DEBUG_ASSERT(_assembly);

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
        MAZE_PLUGIN_CSHARP_API MonoObject* InvokeMethod(MonoObject* _instance, MonoMethod* _method, void** _params)
        {
            MAZE_DEBUG_ASSERT(_method);

            try
            {
                MonoObject* exception = nullptr;
                MonoObject* result = mono_runtime_invoke(_method, _instance, _params, &exception);

                if (exception)
                {
                    MonoString* excStr = mono_object_to_string(exception, nullptr);
                    Char* cstr = mono_string_to_utf8(excStr);
                    MAZE_ERROR("MONO runtime error: %s", cstr);
                    mono_free(cstr);

                    return nullptr;
                }

                return result;
            }
            catch (const std::exception& ex)
            {
                Debug::logerr << "MONO exception caught: " << ex.what() << Maze::endl;
            };

            return nullptr;
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* InvokeStaticMethod(MonoMethod* _method, void** _params)
        {
            return InvokeMethod(nullptr, _method, _params);
        }


        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void ParseMonoEntitySystemAttributes(
            MonoMethod* _method,
            Set<HashedString>& _outTags,
            ComponentSystemOrder& _outOrder,
            U8& _outFlags)
        {
            _outTags.clear();
            _outOrder.after.clear();
            _outOrder.before.clear();
            _outFlags = 0u;

            MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_method(_method);
            if (!attrInfo)
                return;

            for (S32 i = 0; i < attrInfo->num_attrs; ++i)
            {
                MonoCustomAttrEntry& entry = attrInfo->attrs[i];
                MonoClass* attrClass = mono_method_get_class(entry.ctor);

                CString attrClassName = mono_class_get_name(attrClass);
                CString attrNamespace = mono_class_get_namespace(attrClass);
                if (strcmp(attrClassName, "EntitySystemAttribute") == 0 && strcmp(attrNamespace, "Maze.Core") == 0)
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
                else
                if (strcmp(attrClassName, "EnableInEditorAttribute") == 0 && strcmp(attrNamespace, "Maze.Core") == 0)
                {
                    _outFlags |= U8(MonoEntitySystemFlags::EnableInEditor);
                }
            }

            mono_custom_attrs_free(attrInfo);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsMethodPublic(MonoMethod* _method)
        {
            if (_method == nullptr)
                return false;

            uint32_t flags = mono_method_get_flags(_method, nullptr);

            return flags & MONO_METHOD_ATTR_PUBLIC;
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsMethodStatic(MonoMethod* _method)
        {
            if (_method == nullptr)
                return false;

            uint32_t flags = mono_method_get_flags(_method, nullptr);

            return flags & MONO_METHOD_ATTR_STATIC;
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateSerializableProperties(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptPropertyPtr const&)> const& _cb)
        {
            if (_scriptClass->getSuperClass())
                IterateSerializableProperties(_scriptClass->getSuperClass(), _cb);

            for (auto const& data : _scriptClass->getProperties())
            {
                ScriptPropertyPtr const& prop = data.second;
                if (prop->isSerializable())
                    _cb(prop);
            }
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateAllProperties(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptPropertyPtr const&)> const& _cb)
        {
            if (_scriptClass->getSuperClass())
                IterateAllProperties(_scriptClass->getSuperClass(), _cb);

            for (auto const& data : _scriptClass->getProperties())
            {
                ScriptPropertyPtr const& prop = data.second;
                _cb(prop);
            }
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateSerializableFields(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptFieldPtr const&)> const& _cb)
        {
            if (_scriptClass->getSuperClass())
                IterateSerializableFields(_scriptClass->getSuperClass(), _cb);

            for (auto const& data : _scriptClass->getFields())
            {
                ScriptFieldPtr const& field = data.second;
                if (field->isPublic() || field->isSerializable())
                    _cb(field);
            }
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateAllFields(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptFieldPtr const&)> const& _cb)
        {
            if (_scriptClass->getSuperClass())
                IterateAllFields(_scriptClass->getSuperClass(), _cb);

            for (auto const& data : _scriptClass->getFields())
            {
                ScriptFieldPtr const& field = data.second;
                _cb(field);
            }
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* GetComponentByType(
            Component* _component,
            MonoType* _monoType)
        {
            MonoMethod* getComponentByType = MonoEngine::GetEcsUtilsClass()->getMethod("GetComponentByType", 2);

            MonoReflectionType* reflectionType = mono_type_get_object(mono_domain_get(), _monoType);
            MAZE_DEBUG_ASSERT(reflectionType);

            void* params[] = {
                &_component,
                reflectionType
            };

            return MonoHelper::InvokeStaticMethod(getComponentByType, params);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* DeserializeComponentFromDataBlock(
            EcsWorld* _world,
            DataBlock const& _dataBlock,
            CString _name,
            MonoType* _monoType)
        {
            Component* component = EcsHelper::DeserializeComponentFromDataBlock(_world, _dataBlock, _name);
            if (!component)
                return nullptr;

            return GetComponentByType(component, _monoType);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeComponentToDataBlock(
            DataBlock& _dataBlock,
            CString _name,
            MonoObject* _componentInstance)
        {
            if (_componentInstance)
            {
                MonoProperty* componentPtrProperty = MonoEngine::GetNativeComponentPtrProperty()->getMonoProperty();
                MonoObject* result = mono_property_get_value(componentPtrProperty, _componentInstance, nullptr, nullptr);
                if (!result)
                    return;

                Component* component = *(Component**)mono_object_unbox(result);
                EcsHelper::SerializeComponentToDataBlock(_dataBlock, _name, component);
            }
            else
            {
                Char buffer[128];
                StringHelper::FormatString(buffer, sizeof(buffer), "%s:Component", _name);
                _dataBlock.removeDataBlock(HashedCString(buffer));
            }
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsValueType(MonoObject* _obj)
        {
            if (!_obj)
                return false;

            MonoClass* klass = mono_object_get_class(_obj);
            return mono_class_is_valuetype(klass);
        }

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void WriteMetaPropertyToMonoClassFieldString(
            ConstMetaInstance const& _metaInstance,
            MetaProperty const* _metaProperty,
            MonoObject* _monoObj,
            MonoClassField* _field)
        {
            String const& value = _metaProperty->getValueRef<String>(_metaInstance);

            MonoString* monoStr = mono_string_new(MonoEngine::GetMonoDomain(), value.c_str());
            mono_field_set_value(_monoObj, _field, monoStr);
        }

        //////////////////////////////////////////
        #define MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(DType)       \
            MAZE_PLUGIN_CSHARP_API void WriteMetaPropertyToMonoClassField ## DType( \
                ConstMetaInstance const& _metaInstance,                             \
                MetaProperty const* _metaProperty,                                  \
                MonoObject* _monoObj,                                               \
                MonoClassField* _field)                                             \
        {                                                                           \
            DType value;                                                            \
            _metaProperty->getValue<DType>(_metaInstance, value);                   \
            mono_field_set_value(_monoObj, _field, &value);                         \
        }

        //////////////////////////////////////////
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Bool);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(F32);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(F64);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S8);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S16);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S32);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S64);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U8);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U16);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U32);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U64);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec2F);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec3F);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec4F);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec2S);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec3S);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec4S);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec2U);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec3U);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec4U);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Mat3F);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Mat4F);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(TMat);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Rect2F);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(EntityId);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(ColorU32);
        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(ColorF128);

        MAZE_IMPLEMENT_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(CursorInputEvent);


    } // namespace MonoHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
