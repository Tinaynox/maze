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
#if (!defined(_MazePropertyDrawerEnumClass_hpp_))
#define _MazePropertyDrawerEnumClass_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerEnumClass);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(SystemTextDropdown2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);


    //////////////////////////////////////////
    // Class PropertyDrawerEnumClass
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API PropertyDrawerEnumClass
        : public PropertyDrawer
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerEnumClass, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerEnumClass);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerEnumClass();

        //////////////////////////////////////////
        static PropertyDrawerEnumClassPtr Create(
            String const& _label,
            Vector<String> const& _enumValues);


        //////////////////////////////////////////
        void setValue(String const& _value);

        //////////////////////////////////////////
        String getValue() const;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(String const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual String getString() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerEnumClass();

        //////////////////////////////////////////
        using PropertyDrawer::init;

        //////////////////////////////////////////
        virtual bool init(
            String const& _label,
            Vector<String> const& _enumValues);
                

        //////////////////////////////////////////
        void notifyValueChanged(SystemTextDropdown2D* _dropdown, S32 _value);

    protected:
        SystemTextDropdown2DPtr m_dropdown;
        Vector<String> m_enumValues;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerEnumClass_hpp_
//////////////////////////////////////////
