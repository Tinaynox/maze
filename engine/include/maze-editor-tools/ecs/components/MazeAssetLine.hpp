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
#if (!defined(_MazeAssetLine_hpp_))
#define _MazeAssetLine_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/events/MazeEcsInputEvents.hpp"
#include "maze-core/utils/MazeObservableValue.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(AssetLine);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(ContextMenu2D);
    MAZE_USING_SHARED_PTR(AssetsController);
    MAZE_USING_SHARED_PTR(EditBox2D);


    //////////////////////////////////////////
    // Class AssetLine
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API AssetLine
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AssetLine, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AssetLine);

    public:

        //////////////////////////////////////////
        virtual ~AssetLine();

        //////////////////////////////////////////
        static AssetLinePtr Create(
            AssetsController* _assetsController,
            AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        VerticalLayout2DPtr const& getMainLayout() const { return m_mainLayout; }

        //////////////////////////////////////////
        Transform2DPtr const& getMainTransform() const { return m_mainTransform; }

        //////////////////////////////////////////
        AbstractTextRenderer2DPtr const& getTextRenderer() const { return m_textRenderer; }

        //////////////////////////////////////////
        VerticalLayout2DPtr const& getChildrenLayout() const { return m_childrenLayout; }

        //////////////////////////////////////////
        Transform2DPtr const& getChildrenTransform() const { return m_childrenTransform; }

        //////////////////////////////////////////
        SpriteRenderer2DPtr const& getIconRenderer() const { return m_iconRenderer; }


        //////////////////////////////////////////
        inline ContextMenu2DPtr const& getContextMenu() const { return m_contextMenu; }

        //////////////////////////////////////////
        void setIcon(
            SpritePtr const& _sprite,
            ColorU32 const& _color = ColorU32::c_white);

        //////////////////////////////////////////
        void setDropDownVisible(bool _value);

        //////////////////////////////////////////
        bool getDropDownVisible() const;

        //////////////////////////////////////////
        void setExpanded(bool _value);

        //////////////////////////////////////////
        bool isExpanded() const;

        //////////////////////////////////////////
        void setSelected(bool _value);

        //////////////////////////////////////////
        void setEditMode(bool _value);


        //////////////////////////////////////////
        inline void setDragAndDropEnabled(bool _value) { m_dragAndDropEnabled = _value; }

        //////////////////////////////////////////
        inline bool getDragAndDropEnabled() const { return m_dragAndDropEnabled; }


        //////////////////////////////////////////
        AssetFilePtr const& getAssetFile() const { return m_assetFile; }


        //////////////////////////////////////////
        void setSelectAssetFileByClick(bool _value) { m_selectAssetFileByClick = _value; }


        //////////////////////////////////////////
        void processCursorRelease(InputCursorReleaseEvent const& _event);

    public:

        //////////////////////////////////////////
        MultiDelegate<AssetLine*> eventDropDownClick;
        MultiDelegate<AssetLine*, bool> eventExpandedChanged;
        MultiDelegate<AssetLine*> eventLineClick;
        MultiDelegate<AssetLine*> eventLineDoubleClick;

    protected:

        //////////////////////////////////////////
        AssetLine();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(
            AssetsController* _assetsController,
            AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void processEntityAwakened() MAZE_OVERRIDE;
        
        //////////////////////////////////////////
        void notifyDropDownClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyLineClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyLineCursorPressIn(Button2D* _button, Vec2F const& _pos, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyLineDoubleClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyLineFocusChanged(Button2D* _button, bool _value);

        //////////////////////////////////////////
        void updateState();


        //////////////////////////////////////////
        void notifyTextEditSelectedChanged(EditBox2D* _edit, bool _value);

        //////////////////////////////////////////
        void notifyTextEditInput(EditBox2D* _edit);


        //////////////////////////////////////////
        void notifyPressedAssetLineChanged(AssetLine* const& _line);

        

    private:

        //////////////////////////////////////////
        static ObservableValue<AssetLine*> s_pressedAssetLine;

    protected:
        AssetsController* m_assetsController = nullptr;
        AssetFilePtr m_assetFile;

        Transform2DPtr m_transform;

        Transform2DPtr m_mainTransform;
        SpriteRenderer2DPtr m_backgroundRenderer;
        VerticalLayout2DPtr m_mainLayout;
        SpriteRenderer2DPtr m_dropDownRenderer;
        SpriteRenderer2DPtr m_iconRenderer;
        AbstractTextRenderer2DPtr m_textRenderer;
        EditBox2DPtr m_textEdit;

        ClickButton2DPtr m_textButton;

        ContextMenu2DPtr m_contextMenu;

        VerticalLayout2DPtr m_childrenLayout;
        Transform2DPtr m_childrenTransform;

        bool m_selected;
        bool m_dragAndDropEnabled = true;

        bool m_selectAssetFileByClick;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetLine_hpp_
//////////////////////////////////////////
