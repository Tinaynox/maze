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
#include "maze-plugin-csharp/mono/MazeScriptField.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptField
    //
    //////////////////////////////////////////
    ScriptField::ScriptField()
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptField::notifyEvent);
    }
    
    //////////////////////////////////////////
    ScriptField::ScriptField(
        ScriptClassPtr const& _scriptClass,
        MonoClassField* _monoField)
        : m_scriptClass(_scriptClass)
        , m_monoField(_monoField)
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptField::notifyEvent);

        U32 flags = mono_field_get_flags(m_monoField);

        if (flags & MONO_FIELD_ATTR_PUBLIC)
            m_flags |= (U8)ScriptFieldFlags::Public;

        if (flags & MONO_FIELD_ATTR_STATIC)
            m_flags |= (U8)ScriptFieldFlags::Static;

        m_monoType = mono_field_get_type(m_monoField);

        m_name = mono_field_get_name(m_monoField);
        m_typeName = mono_type_get_name(m_monoType);

        MonoCustomAttrInfo* attrInfo = mono_custom_attrs_from_field(_scriptClass->getMonoClass(), m_monoField);
        if (attrInfo)
        {
            for (S32 i = 0; i < attrInfo->num_attrs; ++i)
            {
                MonoCustomAttrEntry& entry = attrInfo->attrs[i];
                MonoClass* attrClass = mono_method_get_class(entry.ctor);

                CString attrClassName = mono_class_get_name(attrClass);
                CString attrNamespace = mono_class_get_namespace(attrClass);
                if (strcmp(attrClassName, "SerializeFieldAttribute") == 0 && strcmp(attrNamespace, "Maze.Core") == 0)
                {
                    m_flags |= (U8)ScriptFieldFlags::Serializable;
                }
            }
        }
    }

    //////////////////////////////////////////
    ScriptField::~ScriptField()
    {
        EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
    }

    //////////////////////////////////////////
    void ScriptField::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            m_monoType = nullptr;
            m_monoField = nullptr;
        }
    }
     

} // namespace Maze
//////////////////////////////////////////
