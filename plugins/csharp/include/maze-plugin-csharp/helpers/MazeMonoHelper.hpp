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
#pragma once
#if (!defined(_MazeMonoHelper_hpp_))
#define _MazeMonoHelper_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/services/MazeLogService.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptClass);
    MAZE_USING_SHARED_PTR(ScriptProperty);
    MAZE_USING_SHARED_PTR(ScriptField);
    MAZE_USING_MANAGED_SHARED_PTR(ScriptableObject);


    //////////////////////////////////////////
    enum class MAZE_PLUGIN_CSHARP_API MonoEntitySystemFlags : U8
    {
        EnableInEditor = MAZE_BIT(0)
    };


    //////////////////////////////////////////
    namespace MonoHelper
    {
        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API CStringSpan GetMonoGenericClassBaseName(CString _genericClassName);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API CStringSpan GetMonoGenericClassFirstGenericArgumentName(CString _genericClassName);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoClass* GetMonoGenericClassFirstGenericArgumentClass(MonoClass* _monoClass);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API CStringSpan GetMonoGenericClassGenericArgumentName(CString _genericClassName, Size _index);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API HashedString BuildMonoClassFullName(
            MonoClass* _class);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API HashedString BuildMonoClassFullName(
            CString _namespace,
            CString _className);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void PrintAssemblyTypes(MonoAssembly* _assembly);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* InvokeMethod(MonoObject* _instance, MonoMethod* _method, void** _params = nullptr);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* InvokeStaticMethod(MonoMethod* _method, void** _params = nullptr);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void ParseMonoEntitySystemAttributes(
            MonoMethod* _method,
            Set<HashedString>& _outTags,
            ComponentSystemOrder& _outOrder,
            U8& _outFlags);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsMethodPublic(MonoMethod* _method);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsMethodStatic(MonoMethod* _method);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateSerializableProperties(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptPropertyPtr const&)> const& _cb);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateAllProperties(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptPropertyPtr const&)> const& _cb);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateSerializableFields(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptFieldPtr const&)> const& _cb);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void IterateAllFields(
            ScriptClassPtr const& _scriptClass,
            std::function<void(ScriptFieldPtr const&)> const& _cb);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* GetComponentByType(
            Component* _component,
            MonoType* _monoType);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* DeserializeComponentFromDataBlock(
            EcsWorld* _world,
            DataBlock const& _dataBlock,
            CString _name,
            MonoType* _monoType);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeComponentToDataBlock(
            DataBlock& _dataBlock,
            CString _name,
            MonoObject* _componentInstance);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API bool IsValueType(MonoObject* _obj);


        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void WriteMetaPropertyToMonoClassFieldString(
            ConstMetaInstance const& _metaInstance,
            MetaProperty const* _metaProperty,
            MonoObject* _monoObj,
            MonoClassField* _field);

        //////////////////////////////////////////
        #define MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(DType)         \
            MAZE_PLUGIN_CSHARP_API void WriteMetaPropertyToMonoClassField ## DType( \
                ConstMetaInstance const& _metaInstance,                             \
                MetaProperty const* _metaProperty,                                  \
                MonoObject* _monoObj,                                               \
                MonoClassField* _field);

        //////////////////////////////////////////
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Bool);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(F32);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(F64);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S8);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S16);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S32);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(S64);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U8);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U16);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U32);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(U64);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec2F);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec3F);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec4F);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec2S);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec3S);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec4S);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec2U);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec3U);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Vec4U);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Mat3F);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Mat4F);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(TMat);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(Rect2F);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(EntityId);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(InputEventKeyboardType);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(KeyCode);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(ColorU32);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(ColorF128);

        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(CursorInputEvent);
        MAZE_DECLARE_WRITE_META_PROPERTY_TO_MONO_CLASS_FIELD(CursorWheelInputEvent);


        //////////////////////////////////////////
        #define MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(DType)                              \
            MAZE_PLUGIN_CSHARP_API void SerializeMonoObject ## DType ##ToDataBlock(                  \
                MonoObject* _boxedValue,                                                             \
                DataBlock& _dataBlock);

        //////////////////////////////////////////
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(String);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Bool);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(F32);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(F64);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(S8);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(S16);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(S32);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(S64);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(U8);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(U16);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(U32);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(U64);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec2F);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec3F);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec4F);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec2S);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec3S);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec4S);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec2U);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec3U);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Vec4U);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Mat3F);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Mat4F);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(TMat);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(Rect2F);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(EntityId);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(ColorU32);
        MAZE_DECLARE_SERIALIZE_MONO_OBJECT_TO_DATA_BLOCK(ColorF128);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeMonoObjectListToDataBlock(MonoObject* _value, DataBlock& _dataBlock);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void DeserializeDataBlockToMonoObjectList(DataBlock const& _dataBlock, MonoObject* _value);


        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API ResourceId GetIndexedResourceId(MonoObject* _indexedResourceInstance);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API MonoObject* CreateIndexedResource(
            HashedCString _className,
            ResourceId _resourceId);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API ScriptableObject* GetScriptableObject(MonoObject* _scritableObjectInstance);

        //////////////////////////////////////////
        MAZE_PLUGIN_CSHARP_API void SerializeScriptableObjectToDataBlock(
            DataBlock& _dataBlock,
            HashedCString _name,
            MonoObject* _scritableObjectInstance);

    } // namespace MonoHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMonoHelper_hpp_
//////////////////////////////////////////
