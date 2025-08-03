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
#if (!defined(_MazeScriptField_hpp_))
#define _MazeScriptField_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp/MazeCSharpHeader.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/data/MazeHashedString.hpp"
#include "maze-core/events/MazeEvent.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ScriptField);
    MAZE_USING_SHARED_PTR(ScriptClass);


    //////////////////////////////////////////
    enum class MAZE_PLUGIN_CSHARP_API ScriptFieldFlags : U8
    {
        Public = MAZE_BIT(0),
        Serializable = MAZE_BIT(1),
        Static = MAZE_BIT(2),
        GenericType = MAZE_BIT(3),
    };


    //////////////////////////////////////////
    // Class ScriptField
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_API ScriptField
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        ScriptField();
    
        //////////////////////////////////////////
        ScriptField(
            ScriptClassPtr const& _scriptClass,
            MonoClassField* _monoField);

        //////////////////////////////////////////
        ~ScriptField();

        //////////////////////////////////////////
        ScriptField(ScriptField const&) = delete;

        //////////////////////////////////////////
        ScriptField(ScriptField&&) = delete;

        //////////////////////////////////////////
        ScriptField& operator=(ScriptField const&) = default;

        //////////////////////////////////////////
        ScriptField& operator=(ScriptField&&) = default;


        //////////////////////////////////////////
        inline ScriptClassPtr getScriptClass() const { return m_scriptClass.lock(); }

        //////////////////////////////////////////
        inline HashedString const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline HashedString const& getTypeName() const { return m_typeName; }

        //////////////////////////////////////////
        inline U8 getFlags() const { return m_flags; }

        //////////////////////////////////////////
        inline bool isPublic() const { return m_flags & (U8)ScriptFieldFlags::Public; }

        //////////////////////////////////////////
        inline bool isSerializable() const { return m_flags & (U8)ScriptFieldFlags::Serializable; }

        //////////////////////////////////////////
        inline bool isStatic() const { return m_flags & (U8)ScriptFieldFlags::Static; }

        //////////////////////////////////////////
        inline bool isGenericType() const { return m_flags & (U8)ScriptFieldFlags::GenericType; }


        //////////////////////////////////////////
        inline MonoType* getMonoType() const { return m_monoType; }

        //////////////////////////////////////////
        inline MonoClassField* getMonoClassField() const { return m_monoField; }

    private:

        //////////////////////////////////////////
        void notifyEvent(ClassUID _eventUID, Event* _event);

    private:
        ScriptClassWPtr m_scriptClass;

        HashedString m_name;
        HashedString m_typeName;

        U8 m_flags = 0;

        MonoType* m_monoType = nullptr;
        MonoClassField* m_monoField = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeScriptField_hpp_
//////////////////////////////////////////
