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
#if (!defined(_MazePropertyDrawerQuaternion_hpp_))
#define _MazePropertyDrawerQuaternion_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerQuaternion);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);


    //////////////////////////////////////////
    // Class PropertyDrawerQuaternion
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerQuaternion
        : public GenericPropertyDrawer<Quaternion>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerQuaternion, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerQuaternion);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerQuaternion();

        //////////////////////////////////////////
        static PropertyDrawerQuaternionPtr Create(String const& _label);


        //////////////////////////////////////////
        virtual void setValue(Quaternion const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Quaternion getValue() const MAZE_OVERRIDE;


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
        PropertyDrawerQuaternion();

        //////////////////////////////////////////
        virtual bool init(String const& _label) MAZE_OVERRIDE;

        //////////////////////////////////////////
        SystemTextEditBox2DPtr createF32Edit(
            CString _name,
            Transform2DPtr const& _parent);

        //////////////////////////////////////////
        void notifyTextInput(SystemTextEditBox2D* _editBox);

        //////////////////////////////////////////
        void notifySelectedChanged(SystemTextEditBox2D* _editBox, bool _selected);

    protected:
        SystemTextEditBox2DPtr m_editBoxX;
        SystemTextEditBox2DPtr m_editBoxY;
        SystemTextEditBox2DPtr m_editBoxZ;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerQuaternion_hpp_
//////////////////////////////////////////
