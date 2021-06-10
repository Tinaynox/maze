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
#if (!defined(_MazeMetaPropertyDrawerVector_hpp_))
#define _MazeMetaPropertyDrawerVector_hpp_


//////////////////////////////////////////
#include "maze-debugger/MazeDebuggerHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-debugger/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-debugger/property-drawers/MazePropertyDrawerS32.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MetaPropertyDrawerVector);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(SystemTextDropdown2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(VerticalLayout2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerVector
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API MetaPropertyDrawerVector
        : public MetaPropertyDrawer
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MetaPropertyDrawerVector, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MetaPropertyDrawerVector);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawerVector();

        //////////////////////////////////////////
        static MetaPropertyDrawerVectorPtr Create(
            MetaProperty* _metaProperty);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawerVector();

        //////////////////////////////////////////
        using MetaPropertyDrawer::init;

        //////////////////////////////////////////
        virtual bool init(
            MetaProperty* _metaProperty) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;


        //////////////////////////////////////////
        bool fetchVectorSizeValue(
            Size& _value,
            bool& _isMultiValue);

        //////////////////////////////////////////
        bool fetchVectorElementClassUID(
            ClassUID& _value,
            bool& _isMultiValue);

        //////////////////////////////////////////
        bool fetchVectorElementValue(
            Size _index,
            String& _value,
            bool& _isMultiValue);

        //////////////////////////////////////////
        void notifyExpandButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyValueChanged(SystemTextDropdown2D* _dropdown, S32 _value);

        //////////////////////////////////////////
        void ensureItemDrawers(Size _count);

    protected:
        SpriteRenderer2DPtr m_expandButtonSprite;
        ClickButton2DPtr m_expandButton;
        VerticalLayout2DPtr m_bodyLayout;
        PropertyDrawerS32Ptr m_vectorSizeDrawer;

        VerticalLayout2DPtr m_itemsLayout;
        Vector<PropertyDrawerPtr> m_itemDrawers;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerVector_hpp_
//////////////////////////////////////////
