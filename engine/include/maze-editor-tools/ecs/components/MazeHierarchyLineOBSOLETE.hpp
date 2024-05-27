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
#if (!defined(_MazeHierarchyLineOBSOLETE_hpp_))
#define _MazeHierarchyLineOBSOLETE_hpp_


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
    MAZE_USING_SHARED_PTR(HierarchyLineOBSOLETE);
    MAZE_USING_SHARED_PTR(Bounds2D);
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(AbstractTextRenderer2D);
    MAZE_USING_SHARED_PTR(SpriteRenderer2D);
    MAZE_USING_SHARED_PTR(ContextMenu2D);


    //////////////////////////////////////////
    enum class HierarchyLineOBSOLETEType
    {
        Scene,
        Entity,

        MAX,
    };

    //////////////////////////////////////////
    enum class HierarchyLineOBSOLETEDropDownState
    {
        None,
        Collapsed,
        Expanded,
    };


    //////////////////////////////////////////
    // Class HierarchyLineOBSOLETE
    //
    //////////////////////////////////////////
    class MAZE_EDITOR_TOOLS_API HierarchyLineOBSOLETE
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(HierarchyLineOBSOLETE, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(HierarchyLineOBSOLETE);

    public:

        //////////////////////////////////////////
        virtual ~HierarchyLineOBSOLETE();

        //////////////////////////////////////////
        static HierarchyLineOBSOLETEPtr Create(
            HierarchyLineOBSOLETEType _type);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        void setLabel(String const& _label);

        //////////////////////////////////////////
        void setColor(ColorU32 const& _color);


        //////////////////////////////////////////
        void setDropDownState(HierarchyLineOBSOLETEDropDownState _value);

        //////////////////////////////////////////
        inline HierarchyLineOBSOLETEDropDownState getDropDownState() const { return m_dropDownState; }


        //////////////////////////////////////////
        inline void setName(String const& _value) { m_name = _value; }

        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }


        //////////////////////////////////////////
        inline HierarchyLineOBSOLETEType getType() const { return m_type; }


        //////////////////////////////////////////
        inline EcsWorld* getEcsWorld() const { return m_world; }

        //////////////////////////////////////////
        void setEcsWorld(EcsWorld* _world);


        //////////////////////////////////////////
        inline void* getUserData() const { return m_userData; }

        //////////////////////////////////////////
        void setUserData(void* _userData);


        //////////////////////////////////////////
        void updateIcon();


        //////////////////////////////////////////
        void setSelected(bool _value);

        //////////////////////////////////////////
        void setActive(bool _value);

    public:

        /////////////////////////////////////////
        MultiDelegate<void*> eventUserDataChanged;
        MultiDelegate<HierarchyLineOBSOLETE*> eventDropDownClick;
        MultiDelegate<HierarchyLineOBSOLETE*> eventLineClick;
        MultiDelegate<HierarchyLineOBSOLETE*> eventLineCursorPressIn;
        MultiDelegate<HierarchyLineOBSOLETE*> eventLineDoubleClick;

    protected:

        //////////////////////////////////////////
        HierarchyLineOBSOLETE();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init(
            HierarchyLineOBSOLETEType _type);

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

    protected:
        EcsWorld* m_world = nullptr;

        String m_name;
        HierarchyLineOBSOLETEType m_type;
        HierarchyLineOBSOLETEDropDownState m_dropDownState;
        void* m_userData = nullptr;

        Transform2DPtr m_transform;
        SpriteRenderer2DPtr m_backgroundRenderer;
        SpriteRenderer2DPtr m_dropDownRenderer;
        SpriteRenderer2DPtr m_iconRenderer;
        AbstractTextRenderer2DPtr m_textRenderer;

        ClickButton2DPtr m_textButton;

        ContextMenu2DPtr m_contextMenu;

        bool m_selected = true;
        bool m_active = true;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHierarchyLineOBSOLETE_hpp_
//////////////////////////////////////////
