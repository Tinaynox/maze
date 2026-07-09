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
#include "maze-ui/ecs/components/MazeSearchableMenuList2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/ecs/components/MazeAbstractTextRenderer2D.hpp"
#include "maze-ui/ecs/components/MazeLayout2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/math/MazeMath.hpp"
#include <algorithm>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SearchableMenuList2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SearchableMenuList2D, Component,
        MAZE_IMPLEMENT_METACLASS_COMPONENT_PROPERTY(filterEditBox, FilterEditBox),
        MAZE_IMPLEMENT_METACLASS_COMPONENT_PROPERTY(scrollRect, ScrollRect),
        MAZE_IMPLEMENT_METACLASS_COMPONENT_PROPERTY(itemsListTransform, ItemsListTransform),
        MAZE_IMPLEMENT_METACLASS_COMPONENT_PROPERTY(itemPrefab, ItemPrefab),
        MAZE_IMPLEMENT_METACLASS_COMPONENT_PROPERTY(backgroundSpriteRenderer, BackgroundSpriteRenderer));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SearchableMenuList2D);

    //////////////////////////////////////////
    SearchableMenuList2D::SearchableMenuList2D()
    {
    }

    //////////////////////////////////////////
    SearchableMenuList2D::~SearchableMenuList2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
            m_UIElement2D->eventCursorPressOut.unsubscribe(this);
        }

        if (m_filterEditBox)
        {
            m_filterEditBox->eventTextInput.unsubscribe(this);
            m_filterEditBox->eventTextChanged.unsubscribe(this);
        }

        if (InputManager::GetInstancePtr())
            InputManager::GetInstancePtr()->eventKeyboard.unsubscribe(this);
    }

    //////////////////////////////////////////
    SearchableMenuList2DPtr SearchableMenuList2D::Create()
    {
        SearchableMenuList2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SearchableMenuList2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool SearchableMenuList2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::update(F32 _dt)
    {

    }

    //////////////////////////////////////////
    void SearchableMenuList2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &SearchableMenuList2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorPressOut.subscribe(this, &SearchableMenuList2D::notifyCursorPressOut);

        InputManager::GetInstancePtr()->eventKeyboard.subscribe(this, &SearchableMenuList2D::notifyKeyboard);
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::processEntityEnabled()
    {
        eventEnabledInHierarchyChanged(this, true);
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::processEntityDisabled()
    {
        eventEnabledInHierarchyChanged(this, false);
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::setFilterEditBox(EditBox2DPtr const& _value)
    {
        if (m_filterEditBox == _value)
            return;

        if (m_filterEditBox)
        {
            m_filterEditBox->eventTextInput.unsubscribe(this);
            m_filterEditBox->eventTextChanged.unsubscribe(this);
        }

        m_filterEditBox = _value;

        if (m_filterEditBox)
        {
            m_filterEditBox->eventTextInput.subscribe(this, &SearchableMenuList2D::notifyFilterTextInput);
            m_filterEditBox->eventTextChanged.subscribe(this, &SearchableMenuList2D::notifyFilterTextChanged);
        }
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::addItem(
        String const& _itemData,
        ItemCallback const& _callback,
        ItemValidateCallback const& _validate,
        bool _rebuildItems)
    {
        m_items.push_back({ _itemData, _callback, _validate });

        if (_rebuildItems)
            rebuildItems();
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::addItems(
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
    void SearchableMenuList2D::clearItems()
    {
        m_items.clear();

        rebuildItems();
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::rebuildItems()
    {
        if (!m_itemPrefab || !m_itemsListTransform || !m_itemsListTransform->getEntityRaw())
            return;

        m_itemsListTransform->destroyAllChildren();
        m_visibleItems.clear();

        String filterText = m_filterEditBox ? m_filterEditBox->getText() : String();
        std::transform(filterText.begin(), filterText.end(), filterText.begin(), ::tolower);

        Vector<ItemData const*> matches;
        matches.reserve(m_items.size());

        for (ItemData const& itemData : m_items)
        {
            if (itemData.validate && !itemData.validate())
                continue;

            if (!filterText.empty())
            {
                String itemTextLower = itemData.text;
                std::transform(itemTextLower.begin(), itemTextLower.end(), itemTextLower.begin(), ::tolower);

                if (itemTextLower.find(filterText) == String::npos)
                    continue;
            }

            matches.push_back(&itemData);
        }

        std::sort(
            matches.begin(),
            matches.end(),
            [](ItemData const* _a, ItemData const* _b) { return _a->text < _b->text; });

        Entity* itemPrefabEntity = m_itemPrefab->getEntityRaw();

        for (ItemData const* itemData : matches)
        {
            EntityPtr itemEntity = itemPrefabEntity->createCopy();
            itemEntity->setActiveSelf(true);

            MenuListItem2DPtr menuListItem = itemEntity->getComponent<MenuListItem2D>();
            menuListItem->getTransform()->setParent(m_itemsListTransform);

            AbstractTextRenderer2D* labelTextRenderer = menuListItem->getTransform()->findChildComponentInheritedFrom<AbstractTextRenderer2D>("Label");
            if (labelTextRenderer)
                labelTextRenderer->setText(itemData->text);

            String text = itemData->text;
            ItemCallback callback = itemData->callback;
            EntityPtr thisEntity = getEntity();
            menuListItem->getButton()->eventClick.subscribe(
                [thisEntity, callback, text](Button2D* _button, CursorInputEvent& _inputEvent)
                {
                    if (_inputEvent.button != 0)
                        return;

                    thisEntity->setActiveSelf(false);
                    callback(text);
                });

            m_visibleItems.push_back(menuListItem);
        }

        Layout2D* itemsListLayout = m_itemsListTransform->getEntityRaw()->getComponentRawInheritedFrom<Layout2D>();
        if (itemsListLayout)
            itemsListLayout->update();

        F32 contentHeight = m_itemsListTransform->getHeight();
        F32 viewportHeight = Math::Clamp(contentHeight, 1.0f, m_maxVisibleHeight);

        if (m_scrollRect)
        {
            m_scrollRect->getTransform()->setHeight(viewportHeight);

            // The viewport/scrollbar sizes are normally refreshed lazily (once per frame,
            // via SizePolicy2D reacting to the parent transform resize above). Force the whole
            // chain to settle synchronously, in dependency order, otherwise the bounds/handle
            // size computed below would use stale geometry:
            //   scrollRect -> viewport
            //   scrollRect -> vertical scrollbar -> sliding area (handle's parent)
            if (Transform2DPtr const& viewportTransform = m_scrollRect->getViewportTransform())
                if (SizePolicy2DPtr sizePolicy = viewportTransform->getEntityRaw()->getComponent<SizePolicy2D>())
                    sizePolicy->updateSize();

            if (Scrollbar2DPtr const& verticalScrollbar = m_scrollRect->getVerticalScrollbar())
            {
                if (SizePolicy2DPtr sizePolicy = verticalScrollbar->getEntityRaw()->getComponent<SizePolicy2D>())
                    sizePolicy->updateSize();

                if (Transform2DPtr const& handleTransform = verticalScrollbar->getHandleTransform())
                    if (Transform2DPtr const& slidingArea = handleTransform->getParent())
                        if (SizePolicy2DPtr sizePolicy = slidingArea->getEntityRaw()->getComponent<SizePolicy2D>())
                            sizePolicy->updateSize();
            }

            // Scroll to the very top of the list. Set the content position directly rather
            // than through normalized-position math - that depends on the exact same bounds
            // computation being fresh, which is otherwise fragile with a popup that resizes
            // on every keystroke.
            if (Transform2DPtr const& contentTransform = m_scrollRect->getContentTransform())
                contentTransform->setLocalPosition(Vec2F::c_zero);

            m_scrollRect->update(0.0f);

            // ScrollRect2D::update() only pushes a new handle size/position through
            // Scrollbar2D::updateUI() when the normalized ratio it computes actually differs
            // from the scrollbar's previously stored value (setHandleSize()/setValue() both
            // no-op otherwise). The sliding area was just resized above, so the handle's pixel
            // geometry (computed from the sliding area's size) is stale even when the ratio is
            // unchanged - force it to recompute here rather than waiting for a focus/press event
            // to call updateUI() incidentally.
            if (Scrollbar2DPtr const& verticalScrollbar = m_scrollRect->getVerticalScrollbar())
                verticalScrollbar->updateUI();
        }

        F32 topOffset = m_filterEditBox ? m_filterEditBox->getTransform()->getHeight() + 4.0f : 0.0f;
        m_transform->setHeight(topOffset + viewportHeight);
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::focusFilterEditBox()
    {
        if (m_filterEditBox)
            m_filterEditBox->setSelected(true);
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::notifyFilterTextInput(EditBox2D* _editBox)
    {
        rebuildItems();
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::notifyFilterTextChanged(EditBox2D* _editBox, String const& _text)
    {
        rebuildItems();
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::notifyKeyboard(InputEventKeyboardData const& _event)
    {
        if (_event.type != InputEventKeyboardType::KeyDown)
            return;

        if (_event.keyCode != KeyCode::Escape)
            return;

        if (!getEntityRaw()->getActiveInHierarchy())
            return;

        getEntityRaw()->setActiveSelf(false);
    }

    //////////////////////////////////////////
    void SearchableMenuList2D::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent& _inputEvent)
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
    void SearchableMenuList2D::notifyCursorPressOut(CursorInputEvent& _inputEvent)
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
    bool SearchableMenuList2D::isMenuContainsPoint(Vec2F const& _positionWS)
    {
        AABB2D aabb = m_transform->calculateWorldAABB();
        return aabb.contains(_positionWS);
    }

} // namespace Maze
//////////////////////////////////////////
