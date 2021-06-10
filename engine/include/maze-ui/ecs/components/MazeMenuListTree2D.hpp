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
#if (!defined(_MazeMenuListTree2D_hpp_))
#define _MazeMenuListTree2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeMenuList2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListItem2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MenuListTree2D);
    MAZE_USING_SHARED_PTR(UIElement2D);


    //////////////////////////////////////////
    // Class MenuListTree2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API MenuListTree2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MenuListTree2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MenuListTree2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        using CloseMenuByPressOutCallback = std::function<bool(CursorInputEvent const&)>;

        //////////////////////////////////////////
        using ItemCallback = std::function<void(String const& _text)>;

        //////////////////////////////////////////
        struct ItemData
        {
            String text;
            ItemCallback callback;
        };

    public:

        //////////////////////////////////////////
        virtual ~MenuListTree2D();

        //////////////////////////////////////////
        static MenuListTree2DPtr Create();


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
        inline void setMenuListPrefab(MenuList2DPtr const& _value)
        {
            if (m_menuListPrefab == _value)
                return;

            m_menuListPrefab = _value;

            rebuildItems();
        }

        //////////////////////////////////////////
        inline MenuList2DPtr const& getMenuListPrefab() const { return m_menuListPrefab; }

        //////////////////////////////////////////
        inline void setMenuListPrefab(ComponentPtr _value) { setMenuListPrefab(_value ? _value->safeCast<MenuList2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getMenuListPrefabComponent() const { return m_menuListPrefab; }


        //////////////////////////////////////////
        inline void setItemsListTransform(Transform2DPtr const& _value)
        {
            if (m_itemsListTransform == _value)
                return;

            m_itemsListTransform = _value;

            rebuildItems();
        }

        //////////////////////////////////////////
        inline Transform2DPtr const& getItemsListTransform() const { return m_itemsListTransform; }

        //////////////////////////////////////////
        inline void setItemsListTransform(ComponentPtr _value) { setItemsListTransform(_value ? _value->safeCast<Transform2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getItemsListTransformComponent() const { return m_itemsListTransform; }


        //////////////////////////////////////////
        inline void setCloseMenuByPressOutCallback(CloseMenuByPressOutCallback _value) { m_closeMenuByPressOutCallback = _value; }

        //////////////////////////////////////////
        inline CloseMenuByPressOutCallback getCloseMenuByPressOutCallback() const { return m_closeMenuByPressOutCallback; }


        //////////////////////////////////////////
        void addItem(
            String const& _itemData,
            ItemCallback const& _callback,
            bool _rebuildItems = true);

        //////////////////////////////////////////
        void addItems(
            Vector<ItemData> const& _items,
            bool _rebuildItems = true);

        //////////////////////////////////////////
        void clearItems();

        //////////////////////////////////////////
        void rebuildItems();

    public:

        //////////////////////////////////////////
        MultiDelegate<MenuListTree2D*, bool> eventEnabledInHierarchyChanged;

    protected:

        //////////////////////////////////////////
        MenuListTree2D();

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
        MenuList2DPtr fetchList(
            String const& _fullListId,
            String const& _localListId,
            Transform2DPtr const& _parent,
            MenuListItem2D* _parentItem = nullptr);

        //////////////////////////////////////////
        void updateListPositions();

        //////////////////////////////////////////
        void notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent);

        //////////////////////////////////////////
        void notifyCursorPressOut(CursorInputEvent const& _inputEvent);


        //////////////////////////////////////////
        bool isMenuContainsPoint(Vec2DF const& _positionWS);

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        MenuList2DPtr m_menuListPrefab;
        Transform2DPtr m_itemsListTransform;

        Vector<ItemData> m_items;
        Map<String, MenuList2DPtr> m_lists;
        Map<MenuListItem2D*, MenuList2DPtr> m_subMenuLists;
        Map<MenuListItem2D*, MenuList2DPtr> m_listsByListItem;

        CloseMenuByPressOutCallback m_closeMenuByPressOutCallback;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMenuListTree2D_hpp_
//////////////////////////////////////////
