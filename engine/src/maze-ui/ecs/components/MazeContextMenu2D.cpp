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
#include "maze-ui/ecs/components/MazeContextMenu2D.hpp"
#include "maze-ui/ecs/components/MazeContextMenuCanvas2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/ecs/helpers/MazeUIHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSScene.hpp"
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
    // Class ContextMenu2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ContextMenu2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, cursorButtonIndex, 1, getCursorButtonIndex, setCursorButtonIndex));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(ContextMenu2D);


    //////////////////////////////////////////
    ContextMenu2D::ContextMenu2D()
        : m_cursorButtonIndex(1)
    {
    }

    //////////////////////////////////////////
    ContextMenu2D::~ContextMenu2D()
    {
        if (m_UIElement2D)
            m_UIElement2D->eventCursorReleaseIn.unsubscribe(this);
    }

    //////////////////////////////////////////
    ContextMenu2DPtr ContextMenu2D::Create()
    {
        ContextMenu2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextMenu2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool ContextMenu2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void ContextMenu2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorReleaseIn.subscribe(this, &ContextMenu2D::notifyCursorReleaseIn);
    }

    //////////////////////////////////////////
    void ContextMenu2D::notifyCursorReleaseIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (!m_callback)
            return;

        if (_inputEvent.capturerHandle != 0 && _inputEvent.capturerHandle != (S32)getEntityId())
            return;

        if (_inputEvent.button == m_cursorButtonIndex)
        {
            ContextMenuCanvas2DPtr contextMenuCanvas = ContextMenuCanvas2D::EnsureContextMenuCanvas(getEntityRaw()->getECSScene());
            
            Canvas* canvas = m_transform->getFirstTrunkComponent<Canvas>();
            Vec2F positionRTS = canvas->convertViewportCoordsToRenderTargetCoords(_inputEvent.position);    

            contextMenuCanvas->openContextMenu(
                this,
                positionRTS,
                [this](MenuListTree2DPtr const& _menuListTree)
                {
                    m_callback(_menuListTree);
                });
            _inputEvent.captureHit((S32)getEntityId());
        }
    }


} // namespace Maze
//////////////////////////////////////////
