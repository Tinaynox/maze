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
#include "maze-plugin-csharp/mono/MazeScriptProperty.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptProperty
    //
    //////////////////////////////////////////
    ScriptProperty::ScriptProperty()
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptProperty::notifyEvent);
    }

    //////////////////////////////////////////
    ScriptProperty::ScriptProperty(
        ScriptClassPtr const& _scriptClass,
        MonoProperty* _monoProperty)
        : m_scriptClass(_scriptClass)
        , m_monoProperty(_monoProperty)
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptProperty::notifyEvent);

        m_getterMethod = mono_property_get_get_method(m_monoProperty);
        m_setterMethod = mono_property_get_set_method(m_monoProperty);

        if (m_getterMethod)
        {
            if (MonoHelper::IsMethodPublic(m_getterMethod))
                m_flags |= (U8)ScriptPropertyFlags::PublicGetter;
            
            if (MonoHelper::IsMethodStatic(m_getterMethod))
                m_flags |= (U8)ScriptPropertyFlags::StaticGetter;
        }

        if (m_setterMethod)
        {
            if (MonoHelper::IsMethodPublic(m_setterMethod))
                m_flags |= (U8)ScriptPropertyFlags::PublicSetter;
            
            if (MonoHelper::IsMethodStatic(m_setterMethod))
                m_flags |= (U8)ScriptPropertyFlags::StaticSetter;
        }

        MonoMethodSignature* signature = mono_method_signature(m_getterMethod);
        m_monoType = mono_signature_get_return_type(signature);

        m_name = mono_property_get_name(m_monoProperty);
        m_typeName = mono_type_get_name(m_monoType);

        MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_property(_scriptClass->getMonoClass(), m_monoProperty);
        if (attrInfo)
        {
            for (S32 i = 0; i < attrInfo->num_attrs; ++i)
            {
                MonoCustomAttrEntry& entry = attrInfo->attrs[i];
                MonoClass* attrClass = mono_method_get_class(entry.ctor);

                CString attrClassName = mono_class_get_name(attrClass);
                CString attrNamespace = mono_class_get_namespace(attrClass);
                if (strcmp(attrClassName, "SerializePropertyAttribute") == 0 && strcmp(attrNamespace, "Maze.Core") == 0)
                {
                    m_flags |= (U8)ScriptPropertyFlags::Serializable;
                }
            }
        }

        if (m_typeName.getString().find('<') != String::npos)
            m_flags |= (U8)ScriptPropertyFlags::GenericType;
    }

    //////////////////////////////////////////
    ScriptProperty::~ScriptProperty()
    {
        EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
    }

    //////////////////////////////////////////
    void ScriptProperty::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            m_monoType = nullptr;
            m_monoProperty = nullptr;
            m_getterMethod = nullptr;
            m_setterMethod = nullptr;
        }
    }
     

} // namespace Maze
//////////////////////////////////////////
