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
#if (!defined(_MazePropertyDrawerFontMaterial_hpp_))
#define _MazePropertyDrawerFontMaterial_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/settings/MazeSettings.hpp"
#include "maze-editor-tools/property-drawers/MazePropertyDrawer.hpp"
#include "maze-ui/fonts/MazeFontMaterial.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(PropertyDrawerFontMaterial);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(DragAndDropZone);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(PropertyDrawerFontMaterialAssetRef);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);


    //////////////////////////////////////////
    // Class PropertyDrawerFontMaterial
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerFontMaterial
        : public GenericPropertyDrawer<FontMaterialPtr>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerFontMaterial, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerFontMaterial);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerFontMaterial();

        //////////////////////////////////////////
        static PropertyDrawerFontMaterialPtr Create(DataBlock const& _dataBlock);


        //////////////////////////////////////////
        virtual void setValue(FontMaterialPtr const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FontMaterialPtr getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerFontMaterial();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void notifyFontMaterialButtonClick(Button2D* _button, CursorInputEvent& _event);

        //////////////////////////////////////////
        void notifySelectAssetClick(Button2D* _button, CursorInputEvent& _event);

    protected:
        ClickButton2DPtr m_fontMaterialButton;
        SpriteRenderer2DPtr m_dragAndDropFrame;
        DragAndDropZonePtr m_dragAndDropZone;
        SpriteRenderer2DPtr m_fontMaterialIcon;
        AbstractTextRenderer2DPtr m_fontMaterialNameDrawer;
        FontMaterialPtr m_fontMaterial;

        ClickButton2DPtr m_selectAssetButton;
    };


    //////////////////////////////////////////
    // Class PropertyDrawerFontMaterialAssetRef
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API PropertyDrawerFontMaterialAssetRef
        : public GenericPropertyDrawer<FontMaterialAssetRef>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(PropertyDrawerFontMaterialAssetRef, PropertyDrawer);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(PropertyDrawerFontMaterialAssetRef);

    public:

        //////////////////////////////////////////
        virtual ~PropertyDrawerFontMaterialAssetRef();

        //////////////////////////////////////////
        static PropertyDrawerFontMaterialAssetRefPtr Create(DataBlock const& _dataBlock);


        //////////////////////////////////////////
        virtual void setValue(FontMaterialAssetRef const& _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual FontMaterialAssetRef getValue() const MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void buildUI(
            Transform2DPtr const& _parent,
            CString _label = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        PropertyDrawerFontMaterialAssetRef();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processDataFromUI();

    protected:
        PropertyDrawerFontMaterialPtr m_drawer;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazePropertyDrawerFontMaterial_hpp_
//////////////////////////////////////////
