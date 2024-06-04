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
#if (!defined(_MazeHierarchyLine_hpp_))
#define _MazeHierarchyLine_hpp_


//////////////////////////////////////////
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/MazeEditorToolsHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/utils/MazeComponentPool.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(Transform3D);
    MAZE_USING_SHARED_PTR(ClickButton2D);
    MAZE_USING_SHARED_PTR(Button2D);
    MAZE_USING_SHARED_PTR(HierarchyLine);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ContextMenu2D);


    //////////////////////////////////////////
    enum class HierarchyLineType
    {
        Scene,
        Entity,

        MAX,
    };

    //////////////////////////////////////////
    enum class HierarchyLineDropDownState
    {
        None,
        Collapsed,
        Expanded,
    };


    //////////////////////////////////////////
    // Class HierarchyLine
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API HierarchyLine
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(HierarchyLine, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(HierarchyLine);

    public:

        //////////////////////////////////////////
        virtual ~HierarchyLine();

        //////////////////////////////////////////
        static HierarchyLinePtr Create(
            HierarchyLineType _type);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        inline ContextMenu2DPtr const& getContextMenu() const { return m_contextMenu; }



        //////////////////////////////////////////
        void setExpanded(bool _value);

        //////////////////////////////////////////
        bool getExpanded() const;


        //////////////////////////////////////////
        inline HierarchyLineType getType() const { return m_type; }

        

        //////////////////////////////////////////
        inline void* getUserData() const { return m_userData; }

        //////////////////////////////////////////
        void setUserData(void* _userData);

        
        /////////////////////////////////////////
        void setText(String const& _text);


        //////////////////////////////////////////
        void setSelected(bool _value);

        //////////////////////////////////////////
        void setActive(bool _value);


        //////////////////////////////////////////
        void setIndent(S32 _indent);

        //////////////////////////////////////////
        inline S32 getIndent() const { return m_indent; }


        //////////////////////////////////////////
        inline VerticalLayout2DPtr const& getChildrenLayout() const { return m_childrenLayout; }

        //////////////////////////////////////////
        Size getChildrenCount() const;

        //////////////////////////////////////////
        inline bool hasChildren() const { return getChildrenCount() > 0; }


        //////////////////////////////////////////
        void addChild(HierarchyLinePtr const& _hierarchyLine);

        //////////////////////////////////////////
        void clearChildren();

        //////////////////////////////////////////
        void release();

        //////////////////////////////////////////
        void prepare();


        //////////////////////////////////////////
        void processAwake(ComponentPoolObject<HierarchyLine>* _poolObject);

        //////////////////////////////////////////
        void processReleased();

    public:

        /////////////////////////////////////////
        MultiDelegate<HierarchyLine*> eventRelease;
        MultiDelegate<void*> eventUserDataChanged;
        MultiDelegate<HierarchyLine*> eventLineClick;
        MultiDelegate<HierarchyLine*> eventLineCursorPressIn;
        MultiDelegate<HierarchyLine*> eventLineDoubleClick;

    protected:

        //////////////////////////////////////////
        HierarchyLine();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(
            HierarchyLineType _type);

        //////////////////////////////////////////
        void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void buildUI();



        //////////////////////////////////////////
        void updateState();

        //////////////////////////////////////////
        void updateNodeContainerIndent();


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
        void updateDropDownRenderer();

    protected:
        String m_text;
        HierarchyLineType m_type = HierarchyLineType::Entity;
        void* m_userData = nullptr;

        Transform2DPtr m_transform;
        VerticalLayout2DPtr m_verticalLayout;
        SizePolicy2DPtr m_sizePolicy;
        
        F32 m_rowHeight = 12.0f;
        SpriteRenderer2DPtr m_backgroundRenderer;

        ClickButton2DPtr m_backgroundButton;
        ContextMenu2DPtr m_contextMenu;

        S32 m_indent = 0;
        F32 m_indentWidth = 12.0f;
        Transform2DPtr m_nodeContainer;
        SizePolicy2DPtr m_nodeContainerSizePolicy;

        SpriteRenderer2DPtr m_dropDownRenderer;
        SpriteRenderer2DPtr m_iconRenderer;
        AbstractTextRenderer2DPtr m_textRenderer;

        VerticalLayout2DPtr m_childrenLayout;

        bool m_selected = false;
        bool m_active = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHierarchyLine_hpp_
//////////////////////////////////////////
