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
#include "maze-ui/ecs/components/MazeMenuListItem2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
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
    // Class MenuListItem2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MenuListItem2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(EntityPtr, checkedNode, EntityPtr(), getCheckedNode, setCheckedNode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, checked, false, getChecked, setChecked),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(EntityPtr, subMenuNode, EntityPtr(), getSubMenuNode, setSubMenuNode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, isSubMenu, false, getIsSubMenu, setIsSubMenu));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MenuListItem2D);

    //////////////////////////////////////////
    MenuListItem2D::MenuListItem2D()
        : m_checked(false)
        , m_isSubMenu(false)
    {
    }

    //////////////////////////////////////////
    MenuListItem2D::~MenuListItem2D()
    {
        if (m_UIElement2D)
            m_UIElement2D->eventFocusChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    MenuListItem2DPtr MenuListItem2D::Create()
    {
        MenuListItem2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MenuListItem2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MenuListItem2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void MenuListItem2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void MenuListItem2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventFocusChanged.subscribe(this, &MenuListItem2D::notifyFocusChanged);

        m_button = getEntityRaw()->ensureComponent<ClickButton2D>();

        updateCheckedUI();
        updateSubMenuUI();
    }

    //////////////////////////////////////////
    void MenuListItem2D::updateCheckedUI()
    {
        if (!m_checkedNode)
            return;

        m_checkedNode->setActiveSelf(m_checked);
    }

    //////////////////////////////////////////
    void MenuListItem2D::updateSubMenuUI()
    {
        if (!m_subMenuNode)
            return;

        m_subMenuNode->setActiveSelf(m_isSubMenu);
    }

    //////////////////////////////////////////
    void MenuListItem2D::notifyFocusChanged(bool _focus)
    {
        if (_focus)
        {
            eventItemFocused(this);
        }
    }


} // namespace Maze
//////////////////////////////////////////
