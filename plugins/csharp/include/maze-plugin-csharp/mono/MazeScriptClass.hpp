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
#if (!defined(_MazeScriptClass_hpp_))
#define _MazeScriptClass_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-plugin-csharp/mono/MazeScriptInstance.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptClass);


    //////////////////////////////////////////
    // Class ScriptClass
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptClass
    {
    public:

        //////////////////////////////////////////
        ScriptClass() = default;
    
        //////////////////////////////////////////
        ScriptClass(
            String const& _namespace,
            String const& _className,
            MonoClass* _monoClass);

        //////////////////////////////////////////
        ScriptClass(
            String const& _namespace,
            String const& _className,
            MonoImage* _monoImage);

        //////////////////////////////////////////
        ScriptClass(ScriptClass const&) = default;

        //////////////////////////////////////////
        ScriptClass(ScriptClass&&) = default;

        //////////////////////////////////////////
        ScriptClass& operator=(ScriptClass const&) = default;

        //////////////////////////////////////////
        ScriptClass& operator=(ScriptClass&&) = default;


        //////////////////////////////////////////
        inline bool operator==(ScriptClass const& _value) const { return m_monoClass == _value.m_monoClass; }


        //////////////////////////////////////////
        inline MonoClass* getMonoClass() const { return m_monoClass; }


        //////////////////////////////////////////
        ScriptInstance instantiate();

        //////////////////////////////////////////
        MonoMethod* getMethod(CString _name, S32 _paramCount = 0);

        //////////////////////////////////////////
        MonoProperty* getProperty(CString _propertyName);

        //////////////////////////////////////////
        bool invokeMethod(MonoObject* _instance, CString _name);

        //////////////////////////////////////////
        static void InvokeMethod(MonoObject* _instance, MonoMethod* _method, void** _params = nullptr);


        //////////////////////////////////////////
        inline bool isValid() const { return m_monoClass != nullptr; }

        //////////////////////////////////////////
        inline MonoMethod* getOnCreateMethod() const { return m_onCreateMethod; }

        //////////////////////////////////////////
        inline MonoMethod* getOnUpdateMethod() const { return m_onUpdateMethod; }


        //////////////////////////////////////////
        String getFullname() const;

    protected:

        //////////////////////////////////////////
        void assignDefaultMethods();

    private:
        String m_namespace;
        String m_className;
        MonoClass* m_monoClass = nullptr;

        MonoMethod* m_onCreateMethod = nullptr;
        MonoMethod* m_onUpdateMethod = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptClass_hpp_
//////////////////////////////////////////
