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
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptClass
    //
    //////////////////////////////////////////
    ScriptClass::ScriptClass()
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptClass::notifyEvent);
    }

    //////////////////////////////////////////
    ScriptClass::~ScriptClass()
    {
        EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
    }
    
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
    void ScriptClass::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            m_monoClass = nullptr;
            m_onCreateMethod = nullptr;
            m_onUpdateMethod = nullptr;
            m_onDestroyMethod = nullptr;
        }
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
        assignFields();
    }

    //////////////////////////////////////////
    void ScriptClass::postInit()
    {
        MonoClass* superClass = mono_class_get_parent(m_monoClass);
        if (superClass)
        {
            HashedString superClassFullName = MonoHelper::BuildMonoClassFullName(superClass);
            m_superClass = MonoEngine::GetScriptClass(superClassFullName);
        }
    }

    //////////////////////////////////////////
    void ScriptClass::assignDefaultMethods()
    {
        m_onCreateMethod = getMethod("OnCreate");
        m_onUpdateMethod = getMethod("OnUpdate", 1);
        m_onDestroyMethod = getMethod("OnDestroy");

        m_onMonoEventMethods.clear();
        m_onNativeEventMethods.clear();

        if (getMethod("OnEvent", 1))
        {
            MonoMethod* method;
            void* iter = nullptr;
            while ((method = mono_class_get_methods(m_monoClass, &iter)) != nullptr)
            {
                CString currentMethodName = mono_method_get_name(method);
                if (strcmp(currentMethodName, "OnEvent") == 0)
                {
                    MonoMethodSignature* sig = mono_method_signature(method);
                    if (!sig)
                        continue;

                    S32 paramCount = mono_signature_get_param_count(sig);
                    if (!paramCount)
                        continue;

                    void* iter2 = nullptr;
                    MonoType* paramType = mono_signature_get_params(sig, &iter2);
                    MonoClass* paramClass = mono_type_get_class(paramType);

                    if (mono_class_is_subclass_of(paramClass, MonoEngine::GetMonoEventClass()->getMonoClass(), false))
                    {
                        CString typeName = mono_type_get_name(paramType);
                        ClassUID eventUID = CalculateClassUID(typeName);

                        m_onMonoEventMethods.emplace(
                            std::piecewise_construct,
                            std::forward_as_tuple(eventUID),
                            std::forward_as_tuple(method));
                    }
                    else
                    if (mono_class_is_subclass_of(paramClass, MonoEngine::GetNativeEventClass()->getMonoClass(), false))
                    {
                        //CString typeName = mono_type_get_name(paramType);
                        //mono_type_get_class()
                        CString paramName = mono_class_get_name(paramClass);
                        ClassUID eventUID = CalculateClassUID((String("Maze::") + paramName).c_str());

                        m_onNativeEventMethods.emplace(
                            std::piecewise_construct,
                            std::forward_as_tuple(eventUID),
                            std::forward_as_tuple(method));
                    }
                }
            }
        }
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
    void ScriptClass::assignFields()
    {
        ScriptClassPtr thisShared = shared_from_this();
        void* propIt = nullptr;
        while (MonoClassField* field = mono_class_get_fields(m_monoClass, &propIt))
        {
            ScriptFieldPtr scriptField = MakeShared<ScriptField>(thisShared, field);
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
        return MonoHelper::BuildMonoClassFullName(m_namespace.c_str(), m_className.c_str());
    }

} // namespace Maze
//////////////////////////////////////////
