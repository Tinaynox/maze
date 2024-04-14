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
        void setLabel(String const& _label);

        //////////////////////////////////////////
        void setColor(ColorU32 const& _color);


        //////////////////////////////////////////
        void setDropDownState(HierarchyLineDropDownState _value);

        //////////////////////////////////////////
        inline HierarchyLineDropDownState getDropDownState() const { return m_dropDownState; }


        //////////////////////////////////////////
        inline void setName(String const& _value) { m_name = _value; }

        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }


        //////////////////////////////////////////
        inline HierarchyLineType getType() const { return m_type; }


        //////////////////////////////////////////
        inline ECSWorld* getECSWorld() const { return m_world; }

        //////////////////////////////////////////
        void setECSWorld(ECSWorld* _world);


        //////////////////////////////////////////
        inline void* getUserData() const { return m_userData; }

        //////////////////////////////////////////
        void setUserData(void* _userData);


        //////////////////////////////////////////
        void updateIcon();

    public:

        /////////////////////////////////////////
        MultiDelegate<void*> eventUserDataChanged;
        MultiDelegate<HierarchyLine*> eventDropDownClick;
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
        void notifyDropDownClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyLineClick(Button2D* _button, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyLineCursorPressIn(Button2D* _button, Vec2F const& _pos, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyLineDoubleClick(Button2D* _button, CursorInputEvent const& _inputEvent);


    protected:
        ECSWorld* m_world = nullptr;

        String m_name;
        HierarchyLineType m_type;
        HierarchyLineDropDownState m_dropDownState;
        void* m_userData = nullptr;

        Transform2DPtr m_transform;
        SpriteRenderer2DPtr m_dropDownRenderer;
        SpriteRenderer2DPtr m_iconRenderer;
        AbstractTextRenderer2DPtr m_textRenderer;

        ContextMenu2DPtr m_contextMenu;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHierarchyLine_hpp_
//////////////////////////////////////////
