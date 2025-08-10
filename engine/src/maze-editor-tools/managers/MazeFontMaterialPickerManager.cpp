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
#include "maze-editor-tools/managers/MazeFontMaterialPickerManager.hpp"
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
#include "maze-editor-tools/pickers/MazeSceneFontMaterialPicker.hpp"
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
    // Class FontMaterialPickerManager
    //
    //////////////////////////////////////////
    FontMaterialPickerManager* FontMaterialPickerManager::s_instance = nullptr;

    //////////////////////////////////////////
    FontMaterialPickerManager::FontMaterialPickerManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    FontMaterialPickerManager::~FontMaterialPickerManager()
    {
        closeFontMaterialPickerEditorWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::Initialize(FontMaterialPickerManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(FontMaterialPickerManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool FontMaterialPickerManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::openFontMaterialPicker(
        FontMaterialPickerCallback _callback,
        FontMaterialPtr const& _material)
    {
        m_callback = nullptr;

        setFontMaterial(_material);

        m_callback = _callback;

        openFontMaterialPickerEditorWindow();
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::setFontMaterial(FontMaterialPtr const& _material)
    {
        m_material = _material;

        eventFontMaterialChanged(m_material);

        if (m_callback)
            m_callback(m_material);
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::openFontMaterialPickerEditorWindow()
    {
        if (m_materialPickerRenderWindow &&
            m_materialPickerRenderWindow->getWindow() &&
            m_materialPickerRenderWindow->getWindow()->isOpened())
        {
            m_materialPickerRenderWindow->getWindow()->setFocused(true);

            openFontMaterialPickerEditorScene();

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2U(400, 340),
                32,
                "FontMaterial");
         
        m_materialPickerRenderWindow = RenderWindow::Create(params);

        m_materialPickerRenderWindow->setName("FontMaterialPicker");

        if (m_materialPickerRenderWindow && m_materialPickerRenderWindow->getWindow())
        {
            m_materialPickerRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &FontMaterialPickerManager::notifyWindowWillClose);
            m_materialPickerRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &FontMaterialPickerManager::notifyWindowFocusChanged);

            openFontMaterialPickerEditorScene();
        }
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::closeFontMaterialPickerEditorWindow()
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
    void FontMaterialPickerManager::openFontMaterialPickerEditorScene()
    {
        SceneFontMaterialPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneFontMaterialPicker>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneFontMaterialPicker>(true, m_materialPickerRenderWindow);
        }

        scene->setup();
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::closeFontMaterialPickerEditorScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneFontMaterialPickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneFontMaterialPicker>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::notifyWindowWillClose(Window* _window)
    {
        closeFontMaterialPickerEditorScene();
        closeFontMaterialPickerEditorWindow();
    }

    //////////////////////////////////////////
    void FontMaterialPickerManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
        {
            _window->close();
        }
    }

    //////////////////////////////////////////
    bool FontMaterialPickerManager::isFontMaterialPickerEditorWindowOpened()
    {
        if (!m_materialPickerRenderWindow)
            return false;

        if (!m_materialPickerRenderWindow->getWindow())
            return false;

        return m_materialPickerRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool FontMaterialPickerManager::isFontMaterialPickerEditorWindowFocused()
    {
        if (!m_materialPickerRenderWindow)
            return false;

        if (!m_materialPickerRenderWindow->getWindow())
            return false;

        return m_materialPickerRenderWindow->getWindow()->getFocused();
    }
    
} // namespace Maze
//////////////////////////////////////////
