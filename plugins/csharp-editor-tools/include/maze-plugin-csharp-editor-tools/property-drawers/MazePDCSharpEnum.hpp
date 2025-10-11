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
#if (!defined(_MazePropertyDrawerCSharpEnum_hpp_))
#define _MazePropertyDrawerCSharpEnum_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-plugin-csharp/MazeMonoHeader.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerCSharpEnum);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(DragAndDropZone);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(PropertyDrawerCSharpEnumAssetRef);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);
    MAZE_USING_SHARED_PTR(Dropdown2D);


    //////////////////////////////////////////
    // Class PropertyDrawerCSharpEnum
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API PropertyDrawerCSharpEnum
        : public PropertyDrawer
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerCSharpEnum, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerCSharpEnum);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerCSharpEnum();

        //////////////////////////////////////////
        static PropertyDrawerCSharpEnumPtr Create(
            MonoType* _monoType,
            DataBlock const& _dataBlock);


        ////////////////////////////////////////////
        S32 getValue() const;

        ////////////////////////////////////////////
        void setValue(S32 _value);


        ////////////////////////////////////////////
        virtual bool toDataBlock(DataBlock& _value) const MAZE_OVERRIDE;

        ////////////////////////////////////////////
        virtual bool setDataBlock(DataBlock const& _value) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerCSharpEnum();

        //////////////////////////////////////////
        bool init(
            MonoType* _monoType,
            DataBlock const& _dataBlock);


        //////////////////////////////////////////
        void notifyValueChanged(Dropdown2D* _dropdown, S32 _value);

    protected:
        Vector<String> m_enumNames;
        Vector<S32> m_enumValues;
        Dropdown2DPtr m_dropdown;

        ClickButton2DPtr m_CSharpEnumButton;
        
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerCSharpEnum_hpp_
//////////////////////////////////////////
