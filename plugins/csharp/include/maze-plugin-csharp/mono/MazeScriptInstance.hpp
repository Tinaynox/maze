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
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace MonoHelper
    {
        //////////////////////////////////////////
        extern MAZE_PLUGIN_CSHARP_API void InvokeMethod(MonoObject* _instance, MonoMethod* _method, void** _params);

    } // namespace MonoHelper
    //////////////////////////////////////////


    //////////////////////////////////////////
    // Class ScriptInstance
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptInstance
    {
    public:

        //////////////////////////////////////////
        ScriptInstance() = default;

        //////////////////////////////////////////
        ScriptInstance(
            MonoClass* _monoClass,
            MonoObject* _instance);

        //////////////////////////////////////////
        ScriptInstance(ScriptInstance const&) = default;

        //////////////////////////////////////////
        ScriptInstance(ScriptInstance&&) = default;

        //////////////////////////////////////////
        ScriptInstance& operator=(ScriptInstance const&) = default;

        //////////////////////////////////////////
        ScriptInstance& operator=(ScriptInstance&&) = default;


        //////////////////////////////////////////
        MonoMethod* getMethod(CString _name, S32 _paramCount = 0);

        //////////////////////////////////////////
        MonoProperty* getProperty(CString _propertyName);

        //////////////////////////////////////////
        bool invokeMethod(MonoMethod* _method);

        //////////////////////////////////////////
        bool invokeMethod(CString _name);


        //////////////////////////////////////////
        inline MonoClass* getMonoClass() const { return m_monoClass; }

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
        inline bool setProperty(MonoProperty* _property, TValue const& _value)
        {
            MonoMethod* setter = mono_property_get_set_method(_property);
            if (!setter)
                return false;

            return invokeMethod(setter, _value);
        }

        //////////////////////////////////////////
        template <>
        inline bool setProperty(MonoProperty* _property, String const& _value)
        {
            MonoMethod* setter = mono_property_get_set_method(_property);
            if (!setter)
                return false;

            MonoString* monoString = mono_string_new(mono_domain_get(), _value.c_str());

            void* stringParam = monoString;
            MonoHelper::InvokeMethod(m_instance, setter, &stringParam);
            return true;
        }

        //////////////////////////////////////////
        template <typename TValue>
        inline bool setProperty(CString _name, TValue const& _value)
        {
            MonoProperty* prop = getProperty(_name);
            MAZE_ERROR_RETURN_VALUE_IF(!prop, false, "%s property is not found!", _name);

            return setProperty(prop, _value);
        }



        //////////////////////////////////////////
        template <typename TValue>
        inline bool getProperty(MonoProperty* _property, TValue& _value) const
        {
            MonoMethod* getter = mono_property_get_get_method(_property);
            if (!getter)
                return false;

            MonoObject* result = mono_property_get_value(_property, m_instance, nullptr, nullptr);
            if (!result)
                return false;

            _value = *(TValue*)mono_object_unbox(result);
            return true;
        }

        //////////////////////////////////////////
        template <>
        inline bool getProperty(MonoProperty* _property, String& _value) const
        {
            MonoMethod* getter = mono_property_get_get_method(_property);
            if (!getter)
                return false;

            MonoObject* result = mono_property_get_value(_property, m_instance, nullptr, nullptr);
            if (!result)
                return false;

            Char* cstr = mono_string_to_utf8((MonoString*)result);
            _value = cstr;
            mono_free(cstr);

            return true;
        }



        //////////////////////////////////////////
        inline bool isValid() const { return m_instance != nullptr; }

    private:
        MonoClass* m_monoClass = nullptr;
        MonoObject* m_instance = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptInstance_hpp_
//////////////////////////////////////////
