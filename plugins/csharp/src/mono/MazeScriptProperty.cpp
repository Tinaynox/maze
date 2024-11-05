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


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ScriptProperty
    //
    //////////////////////////////////////////
    ScriptProperty::ScriptProperty(
        ScriptClassPtr const& _scriptClass,
        MonoProperty* _monoProperty)
        : m_scriptClass(_scriptClass)
        , m_monoProperty(_monoProperty)
    {
        m_getterMethod = mono_property_get_get_method(m_monoProperty);
        m_setterMethod = mono_property_get_set_method(m_monoProperty);

        if (m_getterMethod && MonoHelper::IsMethodPublic(m_getterMethod))
            m_flags |= (U8)ScriptPropertyFlags::PublicGetter;

        if (m_setterMethod && MonoHelper::IsMethodPublic(m_setterMethod))
            m_flags |= (U8)ScriptPropertyFlags::PublicSetter;

        MonoMethodSignature* signature = mono_method_signature(m_getterMethod);
        MonoType* fieldType = mono_signature_get_return_type(signature);

        m_name = mono_property_get_name(m_monoProperty);
        m_typeName = mono_type_get_name(fieldType);
    }
     

} // namespace Maze
//////////////////////////////////////////
