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
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
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
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ContextMenuCanvas2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ContextMenuCanvas2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, menuListTreePrefab, ComponentPtr(), getMenuListTreePrefabComponent, setMenuListTreePrefab));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ContextMenuCanvas2D);

    //////////////////////////////////////////
    ContextMenuCanvas2D::ContextMenuCanvas2D()
        : m_contextMenuHolder(nullptr)
    {
    }

    //////////////////////////////////////////
    ContextMenuCanvas2D::~ContextMenuCanvas2D()
    {
        if (m_menuListTree)
            m_menuListTree->eventEnabledInHierarchyChanged.unsubscribe(this);
    }

    //////////////////////////////////////////
    ContextMenuCanvas2DPtr ContextMenuCanvas2D::Create()
    {
        ContextMenuCanvas2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextMenuCanvas2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ContextMenuCanvas2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ContextMenuCanvas2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_canvas = getEntityRaw()->ensureComponent<Canvas>();

        RenderTargetPtr const& renderTarget = getEntityRaw()->getECSScene()->cast<ECSRenderScene>()->getRenderTarget();
        m_canvas->setRenderTarget(renderTarget);
    }

    //////////////////////////////////////////
    void ContextMenuCanvas2D::closeContextMenu()
    {
        m_contextMenuHolder = nullptr;

        if (!m_menuListTree)
            return;

        m_menuListTree->eventEnabledInHierarchyChanged.unsubscribe(this);
        m_menuListTree->getEntityRaw()->removeFromECSWorld();
        m_menuListTree = nullptr;
    }

    //////////////////////////////////////////
    MenuListTree2DPtr const& ContextMenuCanvas2D::openContextMenu(
        void* _contextMenuHolder,
        Vec2DF const& _positionWS,
        std::function<void(MenuListTree2DPtr const&)> _initMenuFunc)
    {
        closeContextMenu();

        m_contextMenuHolder = _contextMenuHolder;

        EntityPtr menuListTreeEntity = m_menuListTreePrefab->getEntityRaw()->createCopy();
        menuListTreeEntity->setActiveSelf(true);
        m_menuListTree = menuListTreeEntity->getComponent<MenuListTree2D>();

        m_menuListTree->getTransform()->setParent(m_transform);

        m_menuListTree->getTransform()->setLocalPosition(
            { 
                Math::Clamp(_positionWS.x, 0.0f, m_transform->getWidth() - m_menuListTree->getTransform()->getWidth()),
                _positionWS.y
            });
        m_menuListTree->eventEnabledInHierarchyChanged.subscribe(this, &ContextMenuCanvas2D::notifyEnabledInHierarchyChanged);

        _initMenuFunc(m_menuListTree);

        return m_menuListTree;
    }

    //////////////////////////////////////////
    bool ContextMenuCanvas2D::isContextMenuOpened() const
    {
        if (!m_menuListTree)
            return false;

        bool activeInHierarchy = m_menuListTree->getEntityRaw()->getActiveInHierarchy();
        return activeInHierarchy;
    }

    //////////////////////////////////////////
    ContextMenuCanvas2DPtr ContextMenuCanvas2D::EnsureContextMenuCanvas(ECSScene* _scene)
    {
        ContextMenuCanvas2DPtr contextMenuCanvas = _scene->findEntityByComponent<ContextMenuCanvas2D>();
        if (!contextMenuCanvas)
        {
            EntityPtr contextMenuCanvasEntity = _scene->createEntity();
            contextMenuCanvasEntity->ensureComponent<Name>("Context Menu Canvas");
            contextMenuCanvas = contextMenuCanvasEntity->createComponent<ContextMenuCanvas2D>();

            MenuListTree2DPtr menuListTree = UIHelper::CreateDefaultMenuListTree(
                Vec2DF(0.0f, 0.0f),
                contextMenuCanvas->getTransform(),
                contextMenuCanvas->getEntityRaw()->getECSScene(),
                Vec2DF(0.0f, 0.0f),
                Vec2DF(0.0f, 1.0f));
            menuListTree->getEntityRaw()->setActiveSelf(false);
            contextMenuCanvas->setMenuListPrefab(menuListTree);
        }

        return contextMenuCanvas;
    }

    //////////////////////////////////////////
    void ContextMenuCanvas2D::notifyEnabledInHierarchyChanged(MenuListTree2D* _tree, bool _enabled)
    {
        if (_enabled)
        {
            eventContextMenuOpened(this, _tree->cast<MenuListTree2D>());
        }
        else
        {
            eventContextMenuClosed(this, _tree->cast<MenuListTree2D>());
        }
    }

} // namespace Maze
//////////////////////////////////////////
