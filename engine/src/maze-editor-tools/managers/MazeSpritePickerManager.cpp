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
#include "maze-editor-tools/managers/MazeSpritePickerManager.hpp"
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
#include "maze-editor-tools/texture-picker/MazeSceneTexturePicker.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-editor-tools/sprite-picker/MazeSceneSpritePicker.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SpritePickerManager
    //
    //////////////////////////////////////////
    SpritePickerManager* SpritePickerManager::s_instance = nullptr;

    //////////////////////////////////////////
    SpritePickerManager::SpritePickerManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SpritePickerManager::~SpritePickerManager()
    {
        closeSpritePickerEditorWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void SpritePickerManager::Initialize(SpritePickerManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpritePickerManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool SpritePickerManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void SpritePickerManager::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void SpritePickerManager::openSpritePicker(
        SpritePickerCallback _callback,
        SpritePtr const& _texture)
    {
        m_callback = nullptr;

        setSprite(_texture);

        m_callback = _callback;

        openSpritePickerEditorWindow();
    }

    //////////////////////////////////////////
    void SpritePickerManager::setSprite(SpritePtr const& _sprite)
    {
        m_sprite = _sprite;

        eventSpriteChanged(m_sprite);

        if (m_callback)
            m_callback(m_sprite);
    }

    //////////////////////////////////////////
    void SpritePickerManager::openSpritePickerEditorWindow()
    {
        if (m_texturePickerRenderWindow &&
            m_texturePickerRenderWindow->getWindow() &&
            m_texturePickerRenderWindow->getWindow()->isOpened())
        {
            m_texturePickerRenderWindow->getWindow()->setFocused(true);

            openSpritePickerEditorScene();

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2U(400, 340),
                32,
                "Sprite");
         
        m_texturePickerRenderWindow = RenderWindow::Create(params);

        m_texturePickerRenderWindow->setName("SpritePicker");

        if (m_texturePickerRenderWindow && m_texturePickerRenderWindow->getWindow())
        {
            m_texturePickerRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &SpritePickerManager::notifyWindowWillClose);
            m_texturePickerRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &SpritePickerManager::notifyWindowFocusChanged);

            openSpritePickerEditorScene();
        }
    }

    //////////////////////////////////////////
    void SpritePickerManager::closeSpritePickerEditorWindow()
    {
        m_callback = nullptr;

        if (!m_texturePickerRenderWindow)
            return;

        RenderWindowPtr renderWindow = m_texturePickerRenderWindow;
        m_texturePickerRenderWindow.reset();

        if (!renderWindow->getWindow())
            return;

        renderWindow->getWindow()->eventWindowWillClose.unsubscribe(this);
        renderWindow->getWindow()->eventWindowFocusChanged.unsubscribe(this);
        renderWindow->getWindow()->close();
    }

    //////////////////////////////////////////
    void SpritePickerManager::openSpritePickerEditorScene()
    {
        SceneSpritePickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneSpritePicker>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneSpritePicker>(true, m_texturePickerRenderWindow);
        }

        scene->setup();
    }

    //////////////////////////////////////////
    void SpritePickerManager::closeSpritePickerEditorScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneSpritePickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneSpritePicker>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void SpritePickerManager::notifyWindowWillClose(Window* _window)
    {
        closeSpritePickerEditorScene();
        closeSpritePickerEditorWindow();
    }

    //////////////////////////////////////////
    void SpritePickerManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
        {
            _window->close();
        }
    }

    //////////////////////////////////////////
    bool SpritePickerManager::isSpritePickerEditorWindowOpened()
    {
        if (!m_texturePickerRenderWindow)
            return false;

        if (!m_texturePickerRenderWindow->getWindow())
            return false;

        return m_texturePickerRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool SpritePickerManager::isSpritePickerEditorWindowFocused()
    {
        if (!m_texturePickerRenderWindow)
            return false;

        if (!m_texturePickerRenderWindow->getWindow())
            return false;

        return m_texturePickerRenderWindow->getWindow()->getFocused();
    }
    
} // namespace Maze
//////////////////////////////////////////
