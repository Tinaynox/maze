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
#if (!defined(_MazeMetaPropertyDrawerEnumClass_hpp_))
#define _MazeMetaPropertyDrawerEnumClass_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-editor-tools/meta-property-drawers/MazeMetaPropertyDrawer.hpp"
#include "maze-editor-tools/property-drawers/MazePDEnumClass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(MetaPropertyDrawerEnumClass);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(Dropdown2D);
    MAZE_USING_SHARED_PTR(SystemTextRenderer2D);


    //////////////////////////////////////////
    // Class MetaPropertyDrawerEnumClass
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API MetaPropertyDrawerEnumClass
        : public MetaPropertyDrawer
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MetaPropertyDrawerEnumClass, MetaPropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MetaPropertyDrawerEnumClass);

    public:

        //////////////////////////////////////////
        virtual ~MetaPropertyDrawerEnumClass();

        //////////////////////////////////////////
        static MetaPropertyDrawerEnumClassPtr Create(
            MetaProperty* _metaProperty,
            Vector<String> const& _enumValues);


        //////////////////////////////////////////
        virtual void processDataToUI() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processDataFromUI() MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline PropertyDrawerEnumClassPtr const& getDrawer() const { return m_drawer; }

    protected:

        //////////////////////////////////////////
        MetaPropertyDrawerEnumClass();

        //////////////////////////////////////////
        using MetaPropertyDrawer::init;

        //////////////////////////////////////////
        virtual bool init(
            MetaProperty* _metaProperty,
            Vector<String> const& _enumValues);

        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label) MAZE_OVERRIDE;

        ////////////////////////////////////////////
        virtual void unselectUI() MAZE_OVERRIDE;


        //////////////////////////////////////////
        bool fetchPropertyValue(
            String& _value,
            bool& _isMultiValue);


    protected:
        PropertyDrawerEnumClassPtr m_drawer;

        bool m_processingDataToUI = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMetaPropertyDrawerEnumClass_hpp_
//////////////////////////////////////////
