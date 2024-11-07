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
#if (!defined(_MazeScriptProperty_hpp_))
#define _MazeScriptProperty_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/data/MazeHashedString.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptProperty);
    MAZE_USING_SHARED_PTR(ScriptClass);


    //////////////////////////////////////////
    enum class MAZE_PLUGIN_CSHARP_API ScriptPropertyFlags : U8
    {
        PublicGetter = MAZE_BIT(0),
        PublicSetter = MAZE_BIT(1),
        Serializable = MAZE_BIT(2),
    };


    //////////////////////////////////////////
    // Class ScriptProperty
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptProperty
    {
    public:

        //////////////////////////////////////////
        ScriptProperty() = default;
    
        //////////////////////////////////////////
        ScriptProperty(
            ScriptClassPtr const& _scriptClass,
            MonoProperty* _monoProperty);

        //////////////////////////////////////////
        ScriptProperty(ScriptProperty const&) = default;

        //////////////////////////////////////////
        ScriptProperty(ScriptProperty&&) = default;

        //////////////////////////////////////////
        ScriptProperty& operator=(ScriptProperty const&) = default;

        //////////////////////////////////////////
        ScriptProperty& operator=(ScriptProperty&&) = default;



        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline HashedString const& getTypeName() const { return m_typeName; }

        //////////////////////////////////////////
        inline U8 getFlags() const { return m_flags; }

        //////////////////////////////////////////
        inline bool isPublicGetter() const { return m_flags & (U8)ScriptPropertyFlags::PublicGetter;  }

        //////////////////////////////////////////
        inline bool isPublicSetter() const { return m_flags & (U8)ScriptPropertyFlags::PublicSetter; }

        //////////////////////////////////////////
        inline bool isPublic() const { return isPublicGetter() && isPublicSetter(); }

        //////////////////////////////////////////
        inline bool isSerializable() const { return m_flags & (U8)ScriptPropertyFlags::Serializable; }


        //////////////////////////////////////////
        inline MonoType* getMonoType() const { return m_monoType; }

        //////////////////////////////////////////
        inline MonoProperty* getMonoProperty() const { return m_monoProperty; }

        //////////////////////////////////////////
        inline MonoMethod* getGetterMethod() const { return m_getterMethod; }

        //////////////////////////////////////////
        inline MonoMethod* getSetterMethod() const { return m_setterMethod; }

    private:
        ScriptClassWPtr m_scriptClass;

        HashedString m_name;
        HashedString m_typeName;

        U8 m_flags = 0;

        MonoType* m_monoType = nullptr;
        MonoProperty* m_monoProperty = nullptr;
        MonoMethod* m_getterMethod = nullptr;
        MonoMethod* m_setterMethod = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptProperty_hpp_
//////////////////////////////////////////
