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
#if (!defined(_MazeSearchableMenuList2D_hpp_))
#define _MazeSearchableMenuList2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListItem2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(SearchableMenuList2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(EditBox2D);
    MAZE_USING_SHARED_PTR(ScrollRect2D);
    MAZE_USING_SHARED_PTR(MenuListItem2D);


    //////////////////////////////////////////
    // Class SearchableMenuList2D
    //
    // A modern "Add Component"-style popup - a filter edit box on top of a
    // scrollable, flat list of items. Typing into the filter box narrows the
    // list down to items whose text contains the filter (case-insensitive).
    //
    //////////////////////////////////////////
    class MAZE_UI_API SearchableMenuList2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SearchableMenuList2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(SearchableMenuList2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        using CloseMenuByPressOutCallback = std::function<bool(CursorInputEvent&)>;

        //////////////////////////////////////////
        using ItemCallback = std::function<void(String const& _text)>;
        using ItemValidateCallback = std::function<bool()>;

        //////////////////////////////////////////
        struct ItemData
        {
            String text;
            ItemCallback callback;
            ItemValidateCallback validate;
        };

    public:

        //////////////////////////////////////////
        virtual ~SearchableMenuList2D();

        //////////////////////////////////////////
        static SearchableMenuList2DPtr Create();


        //////////////////////////////////////////
        void update(F32 _dt);


        //////////////////////////////////////////
        Transform2DPtr const& getTransform() const { return m_transform; }

        //////////////////////////////////////////
        UIElement2DPtr const& getUIElement() const { return m_UIElement2D; }

        //////////////////////////////////////////
        inline bool getFocused() const { return m_UIElement2D->getFocused(); }

        //////////////////////////////////////////
        inline bool getPressed() const { return m_UIElement2D->getPressed(); }


        //////////////////////////////////////////
        void setFilterEditBox(EditBox2DPtr const& _value);

        //////////////////////////////////////////
        inline EditBox2DPtr const& getFilterEditBox() const { return m_filterEditBox; }

        //////////////////////////////////////////
        inline void setFilterEditBox(ComponentPtr _value) { setFilterEditBox(_value ? _value->safeCast<EditBox2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getFilterEditBoxComponent() const { return m_filterEditBox; }


        //////////////////////////////////////////
        MAZE_IMPLEMENT_COMPONENT_PROPERTY_SETTER_GETTER(ScrollRect2D, scrollRect, ScrollRect);
        MAZE_IMPLEMENT_COMPONENT_PROPERTY_SETTER_GETTER(Transform2D, itemsListTransform, ItemsListTransform);
        MAZE_IMPLEMENT_COMPONENT_PROPERTY_SETTER_GETTER(MenuListItem2D, itemPrefab, ItemPrefab);
        MAZE_IMPLEMENT_COMPONENT_PROPERTY_SETTER_GETTER(SpriteRenderer2D, backgroundSpriteRenderer, BackgroundSpriteRenderer);


        //////////////////////////////////////////
        inline void setCloseMenuByPressOutCallback(CloseMenuByPressOutCallback _value) { m_closeMenuByPressOutCallback = _value; }

        //////////////////////////////////////////
        inline CloseMenuByPressOutCallback getCloseMenuByPressOutCallback() const { return m_closeMenuByPressOutCallback; }


        //////////////////////////////////////////
        inline void setMaxVisibleHeight(F32 _value) { m_maxVisibleHeight = _value; }

        //////////////////////////////////////////
        inline F32 getMaxVisibleHeight() const { return m_maxVisibleHeight; }


        //////////////////////////////////////////
        void addItem(
            String const& _itemData,
            ItemCallback const& _callback,
            ItemValidateCallback const& _validate = nullptr,
            bool _rebuildItems = true);

        //////////////////////////////////////////
        void addItems(
            Vector<ItemData> const& _items,
            bool _rebuildItems = true);

        //////////////////////////////////////////
        void clearItems();

        //////////////////////////////////////////
        void rebuildItems();

        //////////////////////////////////////////
        void focusFilterEditBox();

    public:

        //////////////////////////////////////////
        MultiDelegate<SearchableMenuList2D*, bool> eventEnabledInHierarchyChanged;

    protected:

        //////////////////////////////////////////
        SearchableMenuList2D();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityEnabled() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processEntityDisabled() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorPressOut(CursorInputEvent& _inputEvent);

        //////////////////////////////////////////
        void notifyFilterTextInput(EditBox2D* _editBox);

        //////////////////////////////////////////
        void notifyFilterTextChanged(EditBox2D* _editBox, String const& _text);

        //////////////////////////////////////////
        void notifyKeyboard(InputEventKeyboardData const& _event);


        //////////////////////////////////////////
        bool isMenuContainsPoint(Vec2F const& _positionWS);

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        EditBox2DPtr m_filterEditBox;
        ScrollRect2DPtr m_scrollRect;
        Transform2DPtr m_itemsListTransform;
        MenuListItem2DPtr m_itemPrefab;
        SpriteRenderer2DPtr m_backgroundSpriteRenderer;

        Vector<ItemData> m_items;
        Vector<MenuListItem2DPtr> m_visibleItems;

        CloseMenuByPressOutCallback m_closeMenuByPressOutCallback;

        F32 m_maxVisibleHeight = 300.0f;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSearchableMenuList2D_hpp_
//////////////////////////////////////////
