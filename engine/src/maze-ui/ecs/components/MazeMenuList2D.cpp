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
#include "MazeUIHeader.hpp"
#include "maze-ui/ecs/components/MazeMenuList2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MenuList2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MenuList2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, itemPrefab, ComponentPtr(), getItemPrefabComponent, setItemPrefab),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, itemsListTransform, ComponentPtr(), getItemsListTransformComponent, setItemsListTransform));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MenuList2D);

    //////////////////////////////////////////
    MenuList2D::MenuList2D()
    {
    }

    //////////////////////////////////////////
    MenuList2D::~MenuList2D()
    {
        
    }

    //////////////////////////////////////////
    MenuList2DPtr MenuList2D::Create()
    {
        MenuList2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MenuList2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MenuList2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void MenuList2D::update(F32 _dt)
    {

    }

    //////////////////////////////////////////
    void MenuList2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

    }

    //////////////////////////////////////////
    void MenuList2D::addItem(String const& _itemData, ItemCallback const& _callback)
    {
        m_items.push_back({ _itemData, _callback });

        rebuildItems();
    }

    //////////////////////////////////////////
    void MenuList2D::addItems(Vector<ItemData> const& _items)
    {
        m_items.insert(
            m_items.end(),
            _items.begin(),
            _items.end());

        rebuildItems();
    }


    //////////////////////////////////////////
    void MenuList2D::clearItems()
    {
        m_items.clear();

        rebuildItems();
    }

    //////////////////////////////////////////
    void MenuList2D::rebuildItems()
    {
        if (!m_itemPrefab)
            return;

        if (!m_itemsListTransform)
            return;

        if (!m_transform)
            return;

        Map<String, MenuListItem2DPtr> prevListItems = m_listItems;
        m_listItems.clear();

        for (ItemData const& itemData : m_items)
        {
            String itemText = itemData.text;
            ItemCallback callback = itemData.callback;

            auto prevListItemsIt = prevListItems.find(itemText);
            if (prevListItemsIt != prevListItems.end())
            {
                MenuListItem2DPtr menuListItem = prevListItemsIt->second;

                ClickButton2DPtr const& button = menuListItem->getButton();

                if (callback)
                {
                    button->eventClick.clear();
                    button->eventClick.subscribe(
                        [callback, itemText](Button2D* _button, CursorInputEvent const& _inputEvent)
                        {
                            if (_inputEvent.button != 0)
                                return;

                            callback(itemText);
                        });
                }

                m_listItems[itemText] = menuListItem;

                prevListItems.erase(prevListItemsIt);
            }
            else
            {
                Entity* itemPrefabEntity = m_itemPrefab->getEntityRaw();
                EntityPtr itemEntity = itemPrefabEntity->createCopy();
                itemEntity->setActiveSelf(true);

                MenuListItem2D* menuListItem = itemEntity->getComponentRaw<MenuListItem2D>();

                menuListItem->getTransform()->setParent(m_itemsListTransform);

                SystemTextRenderer2D* labelTextRenderer = menuListItem->getTransform()->findChildComponent<SystemTextRenderer2D>("Label");
                if (labelTextRenderer)
                {
                    labelTextRenderer->setText(itemText);
                }

                ClickButton2DPtr const& button = menuListItem->getButton();

                if (callback)
                {
                    button->eventClick.subscribe(
                        [callback, itemText](Button2D* _button, CursorInputEvent const& _inputEvent)
                        {
                            if (_inputEvent.button != 0)
                                return;

                            callback(itemText);
                        });
                }

                menuListItem->eventItemFocused.subscribe(
                    [&](MenuListItem2D* _item)
                    {
                        Size listItemIndex = getListItemIndex(_item);
                        eventListItemFocused(this, listItemIndex);
                    });

                m_listItems[itemText] = menuListItem->cast<MenuListItem2D>();
            }

        }

        for (auto prevListItemData : prevListItems)
            prevListItemData.second->getEntity()->removeFromECSWorld();
        prevListItems.clear();

        updateListSize();
    }

    //////////////////////////////////////////
    void MenuList2D::updateListSize()
    {
        if (!m_itemsListTransform)
            return;

        Layout2D* itemsListLayout = m_itemsListTransform->getEntityRaw()->getComponentRawInheritedFrom<Layout2D>();
        if (itemsListLayout)
            itemsListLayout->update();

        Vec2DF const& size = m_itemsListTransform->getSize();
        getTransform()->setSize(size);
    }

    //////////////////////////////////////////
    MenuListItem2DPtr MenuList2D::getListItem(Size _index)
    {
        if (_index >= m_itemsListTransform->getChildren().size())
            return nullptr;

        return m_itemsListTransform->getChild(_index)->getEntityRaw()->getComponent<MenuListItem2D>();
    }

    //////////////////////////////////////////
    MenuListItem2D* MenuList2D::getListItemRaw(Size _index)
    {
        if (_index >= m_itemsListTransform->getChildren().size())
            return nullptr;

        return m_itemsListTransform->getChild(_index)->getEntityRaw()->getComponentRaw<MenuListItem2D>();
    }

    //////////////////////////////////////////
    MenuListItem2DPtr MenuList2D::getListItem(String const& _optionName)
    {
        Size index = getListItemIndex(_optionName);
        return getListItem(index);
    }

    //////////////////////////////////////////
    MenuListItem2D* MenuList2D::getListItemRaw(String const& _optionName)
    {
        Size index = getListItemIndex(_optionName);
        return getListItemRaw(index);
    }

    //////////////////////////////////////////
    Size MenuList2D::getListItemIndex(String const& _optionName) const
    {
        for (Size i = 0, in = m_items.size(); i < in; ++i)
            if (m_items[i].text == _optionName)
                return i;

        return -1;
    }

    //////////////////////////////////////////
    String MenuList2D::getListItemOptionName(MenuListItem2D* _item) const
    {
        for (auto const& listItem : m_listItems)
        {
            if (listItem.second.get() == _item)
                return listItem.first;
        }

        return String();
    }

    //////////////////////////////////////////
    Size MenuList2D::getListItemIndex(MenuListItem2D* _item) const
    {
        return getListItemIndex(getListItemOptionName(_item));
    }
    
} // namespace Maze
//////////////////////////////////////////
