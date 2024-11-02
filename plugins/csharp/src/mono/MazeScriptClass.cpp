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
#include "maze-plugin-csharp/mono/MazeScriptClass.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptClass
    //
    //////////////////////////////////////////
    ScriptClass::ScriptClass(
        String const& _namespace,
        String const& _className,
        MonoClass* _monoClass)
        : m_namespace(_namespace)
        , m_className(_className)
        , m_monoClass(_monoClass)
    {
        assignDefaultMethods();
    }
     
    //////////////////////////////////////////
    ScriptClass::ScriptClass(
        String const& _namespace,
        String const& _className,
        MonoImage* _monoImage)
        : m_namespace(_namespace)
        , m_className(_className)
    {
        m_monoClass = mono_class_from_name(
            _monoImage,
            m_namespace.c_str(),
            m_className.c_str());

        MAZE_ERROR_IF(!m_monoClass, "Unknown Core class: %s.%s", m_namespace.c_str(), m_className.c_str());
            
        if (m_monoClass)
            assignDefaultMethods();
    }

    //////////////////////////////////////////
    ScriptInstance ScriptClass::instantiate()
    {
        return ScriptInstance(
            m_monoClass,
            MonoEngine::InstantiateClass(m_monoClass));
    }

    //////////////////////////////////////////
    MonoMethod* ScriptClass::getMethod(CString _name, S32 _paramCount)
    {
        return mono_class_get_method_from_name(m_monoClass, _name, _paramCount);
    }

    //////////////////////////////////////////
    MonoProperty* ScriptClass::getProperty(CString _propertyName)
    {
        return mono_class_get_property_from_name(m_monoClass, _propertyName);
    }

    //////////////////////////////////////////
    void ScriptClass::assignDefaultMethods()
    {
        m_onCreateMethod = getMethod("OnCreate");
        m_onUpdateMethod = getMethod("OnUpdate", 1);
    }

    //////////////////////////////////////////
    String ScriptClass::getFullname() const
    {
        if (!m_namespace.empty())
        {
            String result;
            StringHelper::FormatString(result, "%s.%s", m_namespace.c_str(), m_className.c_str());
            return std::move(result);
        }
        else
            return m_className;
    }

} // namespace Maze
//////////////////////////////////////////
