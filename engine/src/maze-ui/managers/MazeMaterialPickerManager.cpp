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
#include "maze-ui/managers/MazeMaterialPickerManager.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/material-picker/SceneMaterialPicker.hpp"
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
    // Class MaterialPickerManager
    //
    //////////////////////////////////////////
    MaterialPickerManager* MaterialPickerManager::s_instance = nullptr;

    //////////////////////////////////////////
    MaterialPickerManager::MaterialPickerManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    MaterialPickerManager::~MaterialPickerManager()
    {
        closeMaterialPickerEditorWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void MaterialPickerManager::Initialize(MaterialPickerManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(MaterialPickerManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool MaterialPickerManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void MaterialPickerManager::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void MaterialPickerManager::openMaterialPicker(
        MaterialPickerCallback _callback,
        MaterialPtr const& _material)
    {
        m_callback = nullptr;

        setMaterial(_material);

        m_callback = _callback;

        openMaterialPickerEditorWindow();
    }

    //////////////////////////////////////////
    void MaterialPickerManager::setMaterial(MaterialPtr const& _material)
    {
        m_material = _material;

        eventMaterialChanged(m_material);

        if (m_callback)
            m_callback(m_material);
    }

    //////////////////////////////////////////
    void MaterialPickerManager::openMaterialPickerEditorWindow()
    {
        if (m_materialPickerRenderWindow &&
            m_materialPickerRenderWindow->getWindow() &&
            m_materialPickerRenderWindow->getWindow()->isOpened())
        {
            m_materialPickerRenderWindow->getWindow()->setFocused(true);

            openMaterialPickerEditorScene();

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2DU(400, 340),
                32,
                "Material");
         
        m_materialPickerRenderWindow = RenderWindow::Create(params);

        m_materialPickerRenderWindow->setName("MaterialPicker");

        if (m_materialPickerRenderWindow && m_materialPickerRenderWindow->getWindow())
        {
            m_materialPickerRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &MaterialPickerManager::notifyWindowWillClose);
            m_materialPickerRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &MaterialPickerManager::notifyWindowFocusChanged);

            openMaterialPickerEditorScene();
        }
    }

    //////////////////////////////////////////
    void MaterialPickerManager::closeMaterialPickerEditorWindow()
    {
        m_callback = nullptr;

        if (!m_materialPickerRenderWindow)
            return;

        RenderWindowPtr renderWindow = m_materialPickerRenderWindow;
        m_materialPickerRenderWindow.reset();

        if (!renderWindow->getWindow())
            return;

        renderWindow->getWindow()->eventWindowWillClose.unsubscribe(this);
        renderWindow->getWindow()->eventWindowFocusChanged.unsubscribe(this);
        renderWindow->getWindow()->close();
    }

    //////////////////////////////////////////
    void MaterialPickerManager::openMaterialPickerEditorScene()
    {
        SceneMaterialPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneMaterialPicker>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneMaterialPicker>(true, m_materialPickerRenderWindow);
        }

        scene->setup();
    }

    //////////////////////////////////////////
    void MaterialPickerManager::closeMaterialPickerEditorScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneMaterialPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneMaterialPicker>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void MaterialPickerManager::notifyWindowWillClose(Window* _window)
    {
        closeMaterialPickerEditorScene();
        closeMaterialPickerEditorWindow();
    }

    //////////////////////////////////////////
    void MaterialPickerManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
        {
            _window->close();
        }
    }

    //////////////////////////////////////////
    bool MaterialPickerManager::isMaterialPickerEditorWindowOpened()
    {
        if (!m_materialPickerRenderWindow)
            return false;

        if (!m_materialPickerRenderWindow->getWindow())
            return false;

        return m_materialPickerRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool MaterialPickerManager::isMaterialPickerEditorWindowFocused()
    {
        if (!m_materialPickerRenderWindow)
            return false;

        if (!m_materialPickerRenderWindow->getWindow())
            return false;

        return m_materialPickerRenderWindow->getWindow()->getFocused();
    }
    
} // namespace Maze
//////////////////////////////////////////
