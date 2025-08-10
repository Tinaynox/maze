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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/managers/MazeRenderMeshPickerManager.hpp"
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
#include "maze-editor-tools/pickers/MazeSceneRenderMeshPicker.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderMeshPickerManager
    //
    //////////////////////////////////////////
    RenderMeshPickerManager* RenderMeshPickerManager::s_instance = nullptr;

    //////////////////////////////////////////
    RenderMeshPickerManager::RenderMeshPickerManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    RenderMeshPickerManager::~RenderMeshPickerManager()
    {
        closeRenderMeshPickerEditorWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::Initialize(RenderMeshPickerManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderMeshPickerManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool RenderMeshPickerManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::openRenderMeshPicker(
        RenderMeshPickerCallback _callback,
        RenderMeshPtr const& _renderMesh)
    {
        m_callback = nullptr;

        setRenderMesh(_renderMesh);

        m_callback = _callback;

        openRenderMeshPickerEditorWindow();
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::setRenderMesh(RenderMeshPtr const& _renderMesh)
    {
        m_renderMesh = _renderMesh;

        eventRenderMeshChanged(m_renderMesh);

        if (m_callback)
            m_callback(m_renderMesh);
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::openRenderMeshPickerEditorWindow()
    {
        if (m_renderMeshPickerRenderWindow &&
            m_renderMeshPickerRenderWindow->getWindow() &&
            m_renderMeshPickerRenderWindow->getWindow()->isOpened())
        {
            m_renderMeshPickerRenderWindow->getWindow()->setFocused(true);

            openRenderMeshPickerEditorScene();

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2U(400, 340),
                32,
                "Mesh");
         
        m_renderMeshPickerRenderWindow = RenderWindow::Create(params);

        m_renderMeshPickerRenderWindow->setName("RenderMeshPicker");

        if (m_renderMeshPickerRenderWindow && m_renderMeshPickerRenderWindow->getWindow())
        {
            m_renderMeshPickerRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &RenderMeshPickerManager::notifyWindowWillClose);
            m_renderMeshPickerRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &RenderMeshPickerManager::notifyWindowFocusChanged);

            openRenderMeshPickerEditorScene();
        }
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::closeRenderMeshPickerEditorWindow()
    {
        m_callback = nullptr;

        if (!m_renderMeshPickerRenderWindow)
            return;

        RenderWindowPtr renderWindow = m_renderMeshPickerRenderWindow;
        m_renderMeshPickerRenderWindow.reset();

        if (!renderWindow->getWindow())
            return;

        renderWindow->getWindow()->eventWindowWillClose.unsubscribe(this);
        renderWindow->getWindow()->eventWindowFocusChanged.unsubscribe(this);
        renderWindow->getWindow()->close();
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::openRenderMeshPickerEditorScene()
    {
        SceneRenderMeshPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneRenderMeshPicker>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneRenderMeshPicker>(true, m_renderMeshPickerRenderWindow);
        }

        scene->setup();
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::closeRenderMeshPickerEditorScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneRenderMeshPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneRenderMeshPicker>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::notifyWindowWillClose(Window* _window)
    {
        closeRenderMeshPickerEditorScene();
        closeRenderMeshPickerEditorWindow();
    }

    //////////////////////////////////////////
    void RenderMeshPickerManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
        {
            _window->close();
        }
    }

    //////////////////////////////////////////
    bool RenderMeshPickerManager::isRenderMeshPickerEditorWindowOpened()
    {
        if (!m_renderMeshPickerRenderWindow)
            return false;

        if (!m_renderMeshPickerRenderWindow->getWindow())
            return false;

        return m_renderMeshPickerRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool RenderMeshPickerManager::isRenderMeshPickerEditorWindowFocused()
    {
        if (!m_renderMeshPickerRenderWindow)
            return false;

        if (!m_renderMeshPickerRenderWindow->getWindow())
            return false;

        return m_renderMeshPickerRenderWindow->getWindow()->getFocused();
    }
    
} // namespace Maze
//////////////////////////////////////////
