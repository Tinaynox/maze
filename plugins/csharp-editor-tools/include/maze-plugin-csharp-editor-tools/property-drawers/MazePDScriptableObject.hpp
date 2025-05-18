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
#if (!defined(_MazePropertyDrawerScriptableObject_hpp_))
#define _MazePropertyDrawerScriptableObject_hpp_


//////////////////////////////////////////
#include "maze-plugin-csharp-editor-tools/MazeCSharpEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-plugin-csharp/mono/MazeScriptableObject.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-plugin-csharp/mono/MazeScriptableObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerScriptableObject);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(DragAndDropZone);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(PropertyDrawerScriptableObjectAssetRef);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);


    //////////////////////////////////////////
    // Class PropertyDrawerScriptableObject
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_CSHARP_EDITOR_TOOLS_API PropertyDrawerScriptableObject
        : public GenericPropertyDrawer<ScriptableObjectPtr>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerScriptableObject, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerScriptableObject);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerScriptableObject();

        //////////////////////////////////////////
        static PropertyDrawerScriptableObjectPtr Create(DataBlock const& _dataBlock);


        //////////////////////////////////////////
        virtual void setValue(ScriptableObjectPtr const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ScriptableObjectPtr getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerScriptableObject();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyScriptableObjectButtonClick(Button2D* _button, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifySelectAssetClick(Button2D* _button, CursorInputEvent& _event);

    protected:
        ClickButton2DPtr m_scriptableObjectButton;
        SpriteRenderer2DPtr m_dragAndDropFrame;
        DragAndDropZonePtr m_dragAndDropZone;
        SpriteRenderer2DPtr m_scriptableObjectIcon;
        AbstractTextRenderer2DPtr m_scriptableObjectNameDrawer;
        ScriptableObjectPtr m_scriptableObject;

        ClickButton2DPtr m_selectAssetButton;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerScriptableObject_hpp_
//////////////////////////////////////////
