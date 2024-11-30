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
#include "maze-core/MazeTypes.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptClass);
    MAZE_USING_SHARED_PTR(ScriptInstance);
    MAZE_USING_SHARED_PTR(ScriptProperty);
    MAZE_USING_SHARED_PTR(ScriptField);


    //////////////////////////////////////////
    // Class ScriptClass
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptClass
        : public std::enable_shared_from_this<ScriptClass>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        static ScriptClassPtr Create(
            String const& _namespace,
            String const& _className,
            MonoClass* _monoClass);

        //////////////////////////////////////////
        static ScriptClassPtr Create(
            String const& _namespace,
            String const& _className,
            MonoImage* _monoImage);
    

        //////////////////////////////////////////
        ~ScriptClass();


        //////////////////////////////////////////
        ScriptClass(ScriptClass const&) = delete;

        //////////////////////////////////////////
        ScriptClass(ScriptClass&&) = delete;

        //////////////////////////////////////////
        ScriptClass& operator=(ScriptClass const&) = delete;

        //////////////////////////////////////////
        ScriptClass& operator=(ScriptClass&&) = delete;


        //////////////////////////////////////////
        inline bool operator==(ScriptClass const& _value) const { return m_monoClass == _value.m_monoClass; }


        //////////////////////////////////////////
        inline MonoClass* getMonoClass() const { return m_monoClass; }


        //////////////////////////////////////////
        ScriptInstancePtr instantiate();

        //////////////////////////////////////////
        MonoMethod* getMethod(CString _name, S32 _paramCount = 0);

        //////////////////////////////////////////
        MonoProperty* getProperty(CString _propertyName);

        //////////////////////////////////////////
        ScriptPropertyPtr const& getProperty(HashedCString _propertyName);

        //////////////////////////////////////////
        ScriptFieldPtr const& getField(HashedCString _fieldName);

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
        inline String const& getNamespace() const { return m_namespace; }

        //////////////////////////////////////////
        inline String const& getClassName() const { return m_className; }

        //////////////////////////////////////////
        inline HashedString const& getFullName() const { return m_fullName; }


        //////////////////////////////////////////
        inline StringKeyMap<ScriptPropertyPtr> const& getProperties() const { return m_properties; }

        //////////////////////////////////////////
        ScriptPropertyPtr assignPrivateProperty(HashedCString _propertyName);


        //////////////////////////////////////////
        inline StringKeyMap<ScriptFieldPtr> const& getFields() const { return m_fields; }

    protected:

        //////////////////////////////////////////
        ScriptClass();

        //////////////////////////////////////////
        bool init(
            String const& _namespace,
            String const& _className,
            MonoClass* _monoClass);

        //////////////////////////////////////////
        bool init(
            String const& _namespace,
            String const& _className,
            MonoImage* _monoImage);


        //////////////////////////////////////////
        void setup();

        //////////////////////////////////////////
        void assignDefaultMethods();

        //////////////////////////////////////////
        void assignPublicProperties();

        //////////////////////////////////////////
        void assignPublicFields();

        //////////////////////////////////////////
        HashedString buildFullName() const;


        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    private:
        String m_namespace;
        String m_className;
        HashedString m_fullName;
        MonoClass* m_monoClass = nullptr;

        MonoMethod* m_onCreateMethod = nullptr;
        MonoMethod* m_onUpdateMethod = nullptr;

        StringKeyMap<ScriptPropertyPtr> m_properties;
        StringKeyMap<ScriptFieldPtr> m_fields;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptClass_hpp_
//////////////////////////////////////////
