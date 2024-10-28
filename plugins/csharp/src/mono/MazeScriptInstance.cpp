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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptInstance
    //    
    //////////////////////////////////////////
    ScriptInstance::ScriptInstance(
        MonoClass* _monoClass,
        MonoObject* _instance)
        : m_monoClass(_monoClass)
        , m_instance(_instance)
    {

    }

    //////////////////////////////////////////
    MonoMethod* ScriptInstance::getMethod(CString _name, S32 _paramCount)
    {
        return mono_class_get_method_from_name(m_monoClass, _name, _paramCount);
    }

    //////////////////////////////////////////
    MonoProperty* ScriptInstance::getProperty(CString _propertyName)
    {
        return mono_class_get_property_from_name(m_monoClass, _propertyName);
    }

    //////////////////////////////////////////
    bool ScriptInstance::invokeMethod(MonoMethod* _method)
    {
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
    

} // namespace Maze
//////////////////////////////////////////
