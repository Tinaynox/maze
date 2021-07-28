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
#if (!defined(_MazePropertyDrawerVector_hpp_))
#define _MazePropertyDrawerVector_hpp_


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
    MAZE_USING_SHARED_PTR(PropertyDrawerVector);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(SystemTextDropdown2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(VerticalLayout2D);


    //////////////////////////////////////////
    // Class PropertyDrawerVector
    //
    //////////////////////////////////////////
    class MAZE_DEBUGGER_API PropertyDrawerVector
        : public PropertyDrawer
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerVector, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerVector);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerVector();

        //////////////////////////////////////////
        static PropertyDrawerVectorPtr Create(
            ClassUID _childPropertyClassUID,
            String const& _label);


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setString(String const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual String getString() MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline Vector<String> getVector()
        {
            String string = getString();

            Vector<String> result;
            ValueFromString(result, string.c_str(), string.size());
            return result;
        }

        //////////////////////////////////////////
        template <typename TVectorChild>
        inline Vector<TVectorChild> getVector()
        {
            String string = getString();

            Vector<TVectorChild> result;
            ValueFromString(result, string.c_str(), string.size());
            return result;
        }


        //////////////////////////////////////////
        inline void setVector(Vector<String> const& _vector)
        {
            String value;
            ValueToString(_vector, value);
            setString(value);
        }

        //////////////////////////////////////////
        template <typename TVectorChild>
        inline void setVector(Vector<TVectorChild> const& _vector)
        {
            String value;
            ValueToString(_vector, value);
            setString(value);
        }
        
        //////////////////////////////////////////
        Vector<PropertyDrawerPtr> const& getItemDrawers() const { return m_itemDrawers; }

    protected:

        //////////////////////////////////////////
        PropertyDrawerVector();

        //////////////////////////////////////////
        virtual bool init(
            ClassUID _childPropertyClassUID,
            String const& _label);

        //////////////////////////////////////////
        using PropertyDrawer::init;

        //////////////////////////////////////////
        void notifyExpandButtonClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyVectorSizeChanged();

        //////////////////////////////////////////
        void ensureItemDrawers(Size _count);

        //////////////////////////////////////////
        void processItemPropertyUIData();

    protected:
        ClassUID m_childPropertyClassUID = 0;

        SpriteRenderer2DPtr m_expandButtonSprite;
        ClickButton2DPtr m_expandButton;
        VerticalLayout2DPtr m_bodyLayout;
        PropertyDrawerS32Ptr m_vectorSizeDrawer;

        VerticalLayout2DPtr m_itemsLayout;
        Vector<PropertyDrawerPtr> m_itemDrawers;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerVector_hpp_
//////////////////////////////////////////
