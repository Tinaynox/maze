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
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-plugin-csharp/mono/MazeScriptField.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptClass
    //
    //////////////////////////////////////////
    ScriptClassPtr ScriptClass::Create(
        String const& _namespace,
        String const& _className,
        MonoClass* _monoClass)
    {
        ScriptClassPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScriptClass, object, init(_namespace, _className, _monoClass));
        return object;
    }

    //////////////////////////////////////////
    ScriptClassPtr ScriptClass::Create(
        String const& _namespace,
        String const& _className,
        MonoImage* _monoImage)
    {
        ScriptClassPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ScriptClass, object, init(_namespace, _className, _monoImage));
        return object;
    }
     
    //////////////////////////////////////////
    bool ScriptClass::init(
        String const& _namespace,
        String const& _className,
        MonoClass* _monoClass)
    {
        m_namespace = _namespace;
        m_className = _className;
        m_monoClass = _monoClass;
        m_fullName = buildFullName();

        if (m_monoClass)
            setup();

        return true;
    }
     
    //////////////////////////////////////////
    bool ScriptClass::init(
        String const& _namespace,
        String const& _className,
        MonoImage* _monoImage)
    {
        m_namespace = _namespace;
        m_className = _className;
        m_fullName = buildFullName();
        m_monoClass = mono_class_from_name(
            _monoImage,
            m_namespace.c_str(),
            m_className.c_str());

        MAZE_ERROR_RETURN_VALUE_IF(!m_monoClass, false, "Unknown Core class: %s.%s", m_namespace.c_str(), m_className.c_str());
            
        if (m_monoClass)
            setup();

        return true;
    }

    //////////////////////////////////////////
    ScriptInstancePtr ScriptClass::instantiate()
    {
        return MakeShared<ScriptInstance>(
            shared_from_this(),
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
    ScriptPropertyPtr const& ScriptClass::getProperty(HashedCString _propertyName)
    {
        static ScriptPropertyPtr const nullPointer;

        auto it = m_properties.find(_propertyName);
        if (it != m_properties.end())
            return it->second;

        return nullPointer;
    }

    //////////////////////////////////////////
    ScriptFieldPtr const& ScriptClass::getField(HashedCString _fieldName)
    {
        static ScriptFieldPtr const nullPointer;

        auto it = m_fields.find(_fieldName);
        if (it != m_fields.end())
            return it->second;

        return nullPointer;
    }

    //////////////////////////////////////////
    void ScriptClass::setup()
    {
        assignDefaultMethods();
        assignPublicProperties();
        assignPublicFields();
    }

    //////////////////////////////////////////
    void ScriptClass::assignDefaultMethods()
    {
        m_onCreateMethod = getMethod("OnCreate");
        m_onUpdateMethod = getMethod("OnUpdate", 1);
    }

    //////////////////////////////////////////
    void ScriptClass::assignPublicProperties()
    {
        ScriptClassPtr thisShared = shared_from_this();
        void* propIt = nullptr;
        while (MonoProperty* prop = mono_class_get_properties(m_monoClass, &propIt))
        {
            ScriptPropertyPtr scriptProperty = MakeShared<ScriptProperty>(thisShared, prop);
            m_properties.emplace(scriptProperty->getName(), scriptProperty);
        }
    }

    //////////////////////////////////////////
    void ScriptClass::assignPublicFields()
    {
        ScriptClassPtr thisShared = shared_from_this();
        void* propIt = nullptr;
        while (MonoClassField* field = mono_class_get_fields(m_monoClass, &propIt))
        {
            ScriptFieldPtr scriptField = MakeShared<ScriptField>(thisShared, field);
            if (!scriptField->isPublic() && !scriptField->isSerializable())
                continue;
            
            m_fields.emplace(scriptField->getName(), scriptField);
        }

    }

    //////////////////////////////////////////
    ScriptPropertyPtr ScriptClass::assignPrivateProperty(HashedCString _propertyName)
    {
        ScriptPropertyPtr scriptProperty = getProperty(_propertyName);
        if (scriptProperty)
            return scriptProperty;

        MonoProperty* prop = mono_class_get_property_from_name(m_monoClass, _propertyName);
        if (prop)
        {
            ScriptPropertyPtr scriptProperty = MakeShared<ScriptProperty>(shared_from_this(), prop);
            m_properties.emplace(scriptProperty->getName(), scriptProperty);
            return scriptProperty;
        }

        return nullptr;
    }

    //////////////////////////////////////////
    HashedString ScriptClass::buildFullName() const
    {
        if (!m_namespace.empty())
        {
            String result;
            StringHelper::FormatString(result, "%s.%s", m_namespace.c_str(), m_className.c_str());
            return HashedString(result);
        }
        else
            return HashedString(m_className);
    }

} // namespace Maze
//////////////////////////////////////////
