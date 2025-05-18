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
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-plugin-csharp/mono/MazeMonoEngine.hpp"
#include "maze-plugin-csharp/helpers/MazeMonoHelper.hpp"
#include "maze-plugin-csharp/events/MazeCSharpEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptInstance
    // 
    //////////////////////////////////////////
    ScriptInstance::ScriptInstance()
    {
        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptInstance::notifyEvent);
    }
    
    //////////////////////////////////////////
    ScriptInstance::ScriptInstance(
        ScriptClassPtr const& _scriptClass,
        MonoObject* _instance)
        : m_scriptClass(_scriptClass)
        , m_instance(_instance)
    {
        MAZE_DEBUG_ASSERT(m_instance);

        EventManager::GetInstancePtr()->subscribeEvent<MonoShutdownEvent>(this, &ScriptInstance::notifyEvent);

        m_gcHandle = mono_gchandle_new(m_instance, true);
    }

    //////////////////////////////////////////
    ScriptInstance::~ScriptInstance()
    {
        if (EventManager::GetInstancePtr())
            EventManager::GetInstancePtr()->unsubscribeEvent<MonoShutdownEvent>(this);
    }

    //////////////////////////////////////////
    void ScriptInstance::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<MonoShutdownEvent>::UID())
        {
            m_instance = nullptr;
        }
    }

    //////////////////////////////////////////
    MonoMethod* ScriptInstance::getMethod(CString _name, S32 _paramCount)
    {
        return mono_class_get_method_from_name(getMonoClass()->getMonoClass(), _name, _paramCount);
    }

    //////////////////////////////////////////
    ScriptPropertyPtr const& ScriptInstance::getProperty(HashedCString _propertyName)
    {
        return getMonoClass()->getProperty(_propertyName);
    }

    //////////////////////////////////////////
    bool ScriptInstance::invokeMethod(MonoMethod* _method)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(!isValid(), false, "Instance is not valid!");

        MonoHelper::InvokeMethod(m_instance, _method);
        return true;
    }

    //////////////////////////////////////////
    bool ScriptInstance::invokeMethod(CString _name)
    {
        MonoMethod* method = getMethod(_name);
        if (!method)
            return false;

        return invokeMethod(method);
    }

    //////////////////////////////////////////
    void ScriptInstance::destroy()
    {
        if (!m_instance)
            return;

        m_instance = nullptr;
        if (m_gcHandle != 0u)
        {
            mono_gchandle_free(m_gcHandle);
            m_gcHandle = 0u;
        }
    }
    

} // namespace Maze
//////////////////////////////////////////
