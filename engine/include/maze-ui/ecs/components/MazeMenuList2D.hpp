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
#if (!defined(_MazeMenuList2D_hpp_))
#define _MazeMenuList2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeMenuListItem2D.hpp"
#include "maze-core/managers/MazeInputManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderMesh);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(MenuList2D);
    MAZE_USING_SHARED_PTR(UIElement2D);
    MAZE_USING_SHARED_PTR(MenuListItem2D);


    //////////////////////////////////////////
    // Class MenuList2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API MenuList2D
        : public Component
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(MenuList2D, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(MenuList2D);

        //////////////////////////////////////////
        friend class Entity;

    public:

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
        virtual ~MenuList2D();

        //////////////////////////////////////////
        static MenuList2DPtr Create();


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
        inline void setItemPrefab(MenuListItem2DPtr const& _value)
        {
            if (m_itemPrefab == _value)
                return;

            m_itemPrefab = _value;

            rebuildItems();
        }

        //////////////////////////////////////////
        inline MenuListItem2DPtr const& getItemPrefab() const { return m_itemPrefab; }

        //////////////////////////////////////////
        inline void setItemPrefab(ComponentPtr _value) { setItemPrefab(_value ? _value->safeCast<MenuListItem2D>() : nullptr); }

        //////////////////////////////////////////
        inline ComponentPtr getItemPrefabComponent() const { return m_itemPrefab; }


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
        void addItem(String const& _itemData, ItemCallback const& _callback);

        //////////////////////////////////////////
        void addItems(Vector<ItemData> const& _items);

        //////////////////////////////////////////
        void clearItems();


        //////////////////////////////////////////
        MenuListItem2DPtr getListItem(Size _index);

        //////////////////////////////////////////
        MenuListItem2D* getListItemRaw(Size _index);

        //////////////////////////////////////////
        MenuListItem2DPtr getListItem(String const& _optionName);

        //////////////////////////////////////////
        MenuListItem2D* getListItemRaw(String const& _optionName);


        //////////////////////////////////////////
        Size getListItemIndex(String const& _optionName) const;

        //////////////////////////////////////////
        String getListItemOptionName(MenuListItem2D* _item) const;

        //////////////////////////////////////////
        Size getListItemIndex(MenuListItem2D* _item) const;

        //////////////////////////////////////////
        void updateListSize();

    public:
        //////////////////////////////////////////
        MultiDelegate<MenuList2D*, Size> eventListItemFocused;

    protected:

        //////////////////////////////////////////
        MenuList2D();

        //////////////////////////////////////////
        using Component::init;
        
        //////////////////////////////////////////
        bool init();


        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;
            
        //////////////////////////////////////////
        void rebuildItems();

    protected:
        Transform2DPtr m_transform;
        UIElement2DPtr m_UIElement2D;

        MenuListItem2DPtr m_itemPrefab;
        Transform2DPtr m_itemsListTransform;

        Vector<ItemData> m_items;
        Map<String, MenuListItem2DPtr> m_listItems;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeMenuList2D_hpp_
//////////////////////////////////////////
