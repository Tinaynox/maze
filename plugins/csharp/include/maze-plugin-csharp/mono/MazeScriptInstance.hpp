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
#if (!defined(_MazeScriptInstance_hpp_))
#define _MazeScriptInstance_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-plugin-csharp/mono/MazeScriptField.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptClass);
    MAZE_USING_SHARED_PTR(ScriptProperty);


    //////////////////////////////////////////
    namespace MonoHelper
    {
        //////////////////////////////////////////
        extern MAZE_PLUGIN_CSHARP_API MonoObject* InvokeMethod(MonoObject* _instance, MonoMethod* _method, void** _params);

    } // namespace MonoHelper
    //////////////////////////////////////////


    //////////////////////////////////////////
    // Class ScriptInstance
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptInstance
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ScriptInstance();

        //////////////////////////////////////////
        ScriptInstance(
            ScriptClassPtr const& _scriptClass,
            MonoObject* _instance);

        //////////////////////////////////////////
        ~ScriptInstance();

        //////////////////////////////////////////
        ScriptInstance(ScriptInstance const&) = delete;

        //////////////////////////////////////////
        ScriptInstance(ScriptInstance&&) = delete;

        //////////////////////////////////////////
        ScriptInstance& operator=(ScriptInstance const&) = default;

        //////////////////////////////////////////
        ScriptInstance& operator=(ScriptInstance&&) = default;


        //////////////////////////////////////////
        MonoMethod* getMethod(CString _name, S32 _paramCount = 0);

        //////////////////////////////////////////
        ScriptPropertyPtr const& getProperty(HashedCString _propertyName);

        //////////////////////////////////////////
        bool invokeMethod(MonoMethod* _method);

        //////////////////////////////////////////
        bool invokeMethod(CString _name);


        //////////////////////////////////////////
        inline ScriptClassPtr getMonoClass() const { return m_scriptClass.lock(); }

        //////////////////////////////////////////
        inline MonoObject* getInstance() const { return m_instance; }


        //////////////////////////////////////////
        template <typename TValue>
        inline bool invokeMethod(MonoMethod* _method, TValue const& _value)
        {
            void* ptr = (void*)&_value;
            MonoHelper::InvokeMethod(m_instance, _method, &ptr);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool invokeMethod(MonoMethod* _method, MonoObject* const& _value)
        {
            void* ptr = (void*)_value;
            MonoHelper::InvokeMethod(m_instance, _method, &ptr);
            return true;
        }

        //////////////////////////////////////////
        template <typename TValue>
        inline bool invokeMethod(CString _name, TValue const& _value)
        {
            MonoMethod* method = getMethod(_name, 1);
            if (!method)
                return false;

            return invokeMethod(method, _value);
        }



        //////////////////////////////////////////
        template <typename TValue>
        inline bool setPropertyValue(ScriptPropertyPtr const& _property, TValue const& _value)
        {
            if (!_property->getSetterMethod())
                return false;

            return invokeMethod(_property->getSetterMethod(), _value);
        }

        //////////////////////////////////////////
        template <>
        inline bool setPropertyValue(ScriptPropertyPtr const& _property, String const& _value)
        {
            MonoString* monoString = mono_string_new(mono_domain_get(), _value.c_str());
            void* stringParam = monoString;
            MonoHelper::InvokeMethod(m_instance, _property->getSetterMethod(), &stringParam);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool setPropertyValue(ScriptPropertyPtr const& _property, MonoObject* const& _value)
        {
            void* objectParam = _value;
            MonoHelper::InvokeMethod(m_instance, _property->getSetterMethod(), &objectParam);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool setPropertyValue(ScriptPropertyPtr const& _property, MonoArray* const& _value)
        {
            void* arrayParam = _value;
            MonoHelper::InvokeMethod(m_instance, _property->getSetterMethod(), &arrayParam);
            return true;
        }

        //////////////////////////////////////////
        inline bool resetPropertyValue(ScriptPropertyPtr const& _property)
        {
            if (!_property->getSetterMethod())
                return false;

            return MonoHelper::InvokeMethod(m_instance, _property->getSetterMethod(), nullptr);            
        }

        //////////////////////////////////////////
        template <typename TValue>
        inline bool setPropertyValue(HashedCString _name, TValue const& _value)
        {
            ScriptPropertyPtr const& prop = getProperty(_name);
            MAZE_ERROR_RETURN_VALUE_IF(!prop, false, "%s property is not found!", _name.str);

            return setPropertyValue(prop, _value);
        }



        //////////////////////////////////////////
        template <typename TValue>
        inline bool getPropertyValue(ScriptPropertyPtr const& _property, TValue& _value) const
        {
            MAZE_DEBUG_ASSERT(_property->getMonoProperty());
            MonoObject* result = mono_property_get_value(_property->getMonoProperty(), m_instance, nullptr, nullptr);
            if (!result)
                return false;

            MAZE_ERROR_RETURN_VALUE_IF(!MonoHelper::IsValueType(result), false, "%s is not value type", _property->getTypeName().c_str());
            _value = *(TValue*)mono_object_unbox(result);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool getPropertyValue(ScriptPropertyPtr const& _property, String& _value) const
        {
            MAZE_DEBUG_ASSERT(_property->getMonoProperty());
            MonoObject* result = mono_property_get_value(_property->getMonoProperty(), m_instance, nullptr, nullptr);
            if (!result)
                return false;

            Char* cstr = mono_string_to_utf8((MonoString*)result);
            _value = cstr;
            mono_free(cstr);

            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool getPropertyValue(ScriptPropertyPtr const& _property, MonoObject*& _value) const
        {
            MAZE_DEBUG_ASSERT(_property->getMonoProperty());
            MonoObject* result = mono_property_get_value(_property->getMonoProperty(), m_instance, nullptr, nullptr);
            if (!result)
                return false;

            _value = result;
            return true;
        }



        //////////////////////////////////////////
        template <typename TValue>
        inline bool setFieldValue(ScriptFieldPtr const& _field, TValue const& _value)
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());
            mono_field_set_value(m_instance, _field->getMonoClassField(), (void*)&_value);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool setFieldValue(ScriptFieldPtr const& _field, String const& _value)
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());

            MonoString* monoString = mono_string_new(mono_domain_get(), _value.c_str());
            mono_field_set_value(m_instance, _field->getMonoClassField(), monoString);

            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool setFieldValue(ScriptFieldPtr const& _field, MonoObject* const& _value)
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());
            mono_field_set_value(m_instance, _field->getMonoClassField(), _value);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool setFieldValue(ScriptFieldPtr const& _field, MonoArray* const& _value)
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());
            mono_field_set_value(m_instance, _field->getMonoClassField(), _value);
            return true;
        }

        //////////////////////////////////////////
        inline bool resetFieldValue(ScriptFieldPtr const& _field)
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());
            mono_field_set_value(m_instance, _field->getMonoClassField(), nullptr);
            return false;
        }

        //////////////////////////////////////////
        template <typename TValue>
        inline bool getFieldValue(ScriptFieldPtr const& _field, TValue& _value) const
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());
            mono_field_get_value(m_instance, _field->getMonoClassField(), &_value);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool getFieldValue(ScriptFieldPtr const& _field, String& _value) const
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());

            MonoString* monoString = nullptr;
            mono_field_get_value(m_instance, _field->getMonoClassField(), &monoString);

            if (monoString)
            {
                Char* cStr = mono_string_to_utf8(monoString);
                _value = cStr;
                mono_free(cStr);
            }

            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool getFieldValue(ScriptFieldPtr const& _field, MonoObject*& _value) const
        {
            MAZE_DEBUG_ASSERT(_field->getMonoClassField());
            mono_field_get_value(m_instance, _field->getMonoClassField(), &_value);
            return true;
        }



        //////////////////////////////////////////
        void destroy();

        //////////////////////////////////////////
        inline bool isValid() const { return m_instance != nullptr && mono_object_get_class(m_instance) != nullptr; }


    private:

        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    private:
        ScriptClassWPtr m_scriptClass;
        MonoObject* m_instance = nullptr;
        U32 m_gcHandle = 0u;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptInstance_hpp_
//////////////////////////////////////////
