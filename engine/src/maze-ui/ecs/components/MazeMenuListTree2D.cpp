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
#include "maze-ui/ecs/components/MazeMenuListTree2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
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
    // Class MenuListTree2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MenuListTree2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, menuListPrefab, ComponentPtr(), getMenuListPrefabComponent, setMenuListPrefab),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, itemsListTransform, ComponentPtr(), getItemsListTransformComponent, setItemsListTransform),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(MenuListTree2D::CloseMenuByPressOutCallback, closeMenuByPressOutCallback, nullptr, getCloseMenuByPressOutCallback, setCloseMenuByPressOutCallback));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MenuListTree2D);

    //////////////////////////////////////////
    MenuListTree2D::MenuListTree2D()
    {
    }

    //////////////////////////////////////////
    MenuListTree2D::~MenuListTree2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
            m_UIElement2D->eventCursorPressOut.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    MenuListTree2DPtr MenuListTree2D::Create()
    {
        MenuListTree2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MenuListTree2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MenuListTree2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void MenuListTree2D::update(F32 _dt)
    {

    }

    //////////////////////////////////////////
    void MenuListTree2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &MenuListTree2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorPressOut.subscribe(this, &MenuListTree2D::notifyCursorPressOut);
    }

    //////////////////////////////////////////
    void MenuListTree2D::processEntityEnabled()
    {
        for (auto listData : m_lists)
        {
            bool isActiveList = listData.first.empty();
            listData.second->getEntityRaw()->setActiveSelf(isActiveList);
        }

        eventEnabledInHierarchyChanged(this, true);
    }

    //////////////////////////////////////////
    void MenuListTree2D::processEntityDisabled()
    {
        eventEnabledInHierarchyChanged(this, false);
    }

    //////////////////////////////////////////
    void MenuListTree2D::addItem(
        String const& _itemData,
        ItemCallback const& _callback,
        bool _rebuildItems)
    {
        m_items.push_back({ _itemData, _callback });

        if (_rebuildItems)
            rebuildItems();
    }

    //////////////////////////////////////////
    void MenuListTree2D::addItems(
        Vector<ItemData> const& _items,
        bool _rebuildItems)
    {
        m_items.insert(
            m_items.end(),
            _items.begin(),
            _items.end());

        if (_rebuildItems)
            rebuildItems();
    }

    //////////////////////////////////////////
    void MenuListTree2D::clearItems()
    {
        m_items.clear();

        rebuildItems();
    }

    //////////////////////////////////////////
    void MenuListTree2D::rebuildItems()
    {
        if (!m_menuListPrefab)
            return;

        if (!m_itemsListTransform)
            return;

        m_lists.clear();
        m_subMenuLists.clear();
        m_listsByListItem.clear();
        m_itemsListTransform->destroyAllChildren();

        for (ItemData const& itemData : m_items)
        {
            String itemText = itemData.text;
            ItemCallback callback = itemData.callback;

            Vector<String> words;
            StringHelper::SplitWords(itemText, words, '/');

            String fullListId;
            MenuListItem2D* parentListItem = nullptr;

            for (Size w = 0, wc = words.size(); w < wc; ++w)
            {
                MenuList2DPtr menuList = fetchList(
                    fullListId,
                    words[w],
                    m_itemsListTransform,
                    parentListItem);

                bool isSubMenu = w != wc - 1;

                if (!menuList->getListItemRaw(words[w]))
                {
                    ItemCallback func = 
                        [callback, this](String const& _text)
                        {
                            getEntityRaw()->setActiveSelf(false);
                            callback(_text);
                        };

                    menuList->addItem(
                        words[w],
                        isSubMenu ? nullptr : func);
                }

                MenuListItem2D* listItem = menuList->getListItemRaw(words[w]);
                m_listsByListItem.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(listItem),
                    std::forward_as_tuple(menuList));

                listItem->setIsSubMenu(isSubMenu);

                parentListItem = listItem;

                if (!fullListId.empty())
                    fullListId += '/';

                fullListId += words[w];
            }
        }

        updateListPositions();

        for (auto listData : m_lists)
        {
            bool isActiveList = listData.first.empty();
            listData.second->getEntityRaw()->setActiveSelf(isActiveList);
        }
    }

    //////////////////////////////////////////
    MenuList2DPtr MenuListTree2D::fetchList(
        String const& _fullListId,
        String const& _localListId,
        Transform2DPtr const& _parent,
        MenuListItem2D* _parentItem)
    {
        auto it = m_lists.find(_fullListId);
        if (it != m_lists.end())
        {
            return it->second;
        }
        else
        {
            Entity* itemPrefabEntity = m_menuListPrefab->getEntityRaw();
            EntityPtr itemEntity = itemPrefabEntity->createCopy();
            itemEntity->setActiveSelf(true);


            MenuList2DPtr menuList = itemEntity->getComponent<MenuList2D>();

            menuList->eventListItemFocused.subscribe(
                [&](MenuList2D* _list, Size _index)
                {
                    Set<MenuList2DPtr> activeLists;

                    MenuListItem2DPtr menuListItem = _list->getListItem(_index);
                    if (menuListItem->getIsSubMenu())
                    {
                        auto const subMenuListIt = m_subMenuLists.find(menuListItem.get());
                        if (subMenuListIt != m_subMenuLists.end())
                        {
                            MenuList2DPtr subMenuList = subMenuListIt->second;
                            subMenuList->getEntityRaw()->setActiveSelf(true);

                            activeLists.insert(subMenuList);
                        }
                    }

                    MenuList2D* list = _list;
                    do
                    {
                        activeLists.insert(list->cast<MenuList2D>());

                        bool listFound = false;

                        for (auto subMenuListsData : m_subMenuLists)
                        {
                            if (subMenuListsData.second.get() == list)
                            {
                                MenuListItem2D* parentListItem = subMenuListsData.first;

                                auto parentListIt = m_listsByListItem.find(parentListItem);
                                if (parentListIt != m_listsByListItem.end())
                                {
                                    list = parentListIt->second.get();
                                }
                                else
                                {
                                    list = nullptr;
                                }

                                listFound = true;
                                break;
                            }
                        }

                        if (!listFound)
                            list = nullptr;
                    }
                    while (list != nullptr);

                    for (auto listData : m_lists)
                    {
                        bool isActiveList = (activeLists.find(listData.second) != activeLists.end());
                        listData.second->getEntityRaw()->setActiveSelf(isActiveList);
                    }
                });

            menuList->getTransform()->setParent(_parent);

            menuList->getTransform()->setPivot(0.0f, 1.0f);



            if (_parentItem)
            {
                m_subMenuLists.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(_parentItem),
                    std::forward_as_tuple(menuList));
            }

            m_lists.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_fullListId),
                std::forward_as_tuple(menuList));

            return menuList;
        }
    }

    //////////////////////////////////////////
    void MenuListTree2D::updateListPositions()
    {
        for (auto listData : m_lists)
        {
            bool isMainList = listData.first.empty();
            
            MenuList2DPtr const& menuList = listData.second;

            if (!isMainList)
            {
                MenuListItem2D* parentListItem = nullptr;
                for (auto subMenuListsData : m_subMenuLists)
                {
                    if (subMenuListsData.second == menuList)
                    {
                        parentListItem = subMenuListsData.first;
                        break;
                    }
                }

                if (!parentListItem)
                    continue;

                Transform2D* root = parentListItem->getTransform()->getRootTransform();
                Vec2DF const& rootSize = root->getSize();

                AABB2D menuListAABB = menuList->getTransform()->calculateWorldAABB();
                F32 menuListWorldWidth = menuListAABB.getWidth();
                F32 menuListWorldHeight = menuListAABB.getHeight();

                Vec2DF positionOS(
                    parentListItem->getTransform()->getWidth(),
                    parentListItem->getTransform()->getHeight());

                Vec2DF positionWS = parentListItem->getTransform()->getWorldTransform().transformAffine(positionOS);
                if (positionWS.x + menuListWorldWidth > rootSize.x)
                    positionOS.x = 0 - menuList->getTransform()->getWidth();

                positionWS = parentListItem->getTransform()->getWorldTransform().transformAffine(positionOS);
                positionWS.y = Math::Clamp(positionWS.y, menuList->getTransform()->getHeight(), rootSize.y);

                Vec2DF menuListPositionOS = m_itemsListTransform->getWorldTransform().inversedAffineCopy().transformAffine(positionWS);

                menuList->getTransform()->setPivot(0.0f, 1.0f);
                menuList->getTransform()->setAnchor(0.0f, 0.0f);
                menuList->getTransform()->setLocalPosition(menuListPositionOS);
            }
            else
            {
                menuList->getTransform()->setAnchor(0.0f, 1.0f);
                menuList->getTransform()->setLocalPosition(Vec2DF::c_zero);
            }
        }
    }

    //////////////////////////////////////////
    void MenuListTree2D::notifyCursorPressIn(Vec2DF const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (!isMenuContainsPoint(_inputEvent.position))
        {
            if (!m_closeMenuByPressOutCallback || m_closeMenuByPressOutCallback(_inputEvent))
                getEntityRaw()->setActiveSelf(false);
        }
    }

    //////////////////////////////////////////
    void MenuListTree2D::notifyCursorPressOut(CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        if (!isMenuContainsPoint(_inputEvent.position))
        {
            if (!m_closeMenuByPressOutCallback || m_closeMenuByPressOutCallback(_inputEvent))
                getEntityRaw()->setActiveSelf(false);
        }
    }

    //////////////////////////////////////////
    bool MenuListTree2D::isMenuContainsPoint(Vec2DF const& _positionWS)
    {
        for (auto listData : m_lists)
        {
            MenuList2DPtr const& menuList = listData.second;

            AABB2D aabb = menuList->getItemsListTransform()->calculateWorldAABB();
            if (aabb.contains(_positionWS))
            {
                return true;
            }
        }

        return false;
    }


} // namespace Maze
//////////////////////////////////////////
