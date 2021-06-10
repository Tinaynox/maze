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
#include "maze-ui/managers/MazeTexturePickerManager.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/texture-picker/SceneTexturePicker.hpp"
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
    // Class TexturePickerManager
    //
    //////////////////////////////////////////
    TexturePickerManager* TexturePickerManager::s_instance = nullptr;

    //////////////////////////////////////////
    TexturePickerManager::TexturePickerManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    TexturePickerManager::~TexturePickerManager()
    {
        closeTexturePickerEditorWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void TexturePickerManager::Initialize(TexturePickerManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(TexturePickerManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool TexturePickerManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void TexturePickerManager::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void TexturePickerManager::openTexturePicker(
        TexturePickerCallback _callback,
        Texture2DPtr const& _texture)
    {
        m_callback = nullptr;

        setTexture(_texture);

        m_callback = _callback;

        openTexturePickerEditorWindow();
    }

    //////////////////////////////////////////
    void TexturePickerManager::setTexture(Texture2DPtr const& _texture)
    {
        m_texture = _texture;

        eventTextureChanged(m_texture);

        if (m_callback)
            m_callback(m_texture);
    }

    //////////////////////////////////////////
    void TexturePickerManager::openTexturePickerEditorWindow()
    {
        if (m_texturePickerRenderWindow &&
            m_texturePickerRenderWindow->getWindow() &&
            m_texturePickerRenderWindow->getWindow()->isOpened())
        {
            m_texturePickerRenderWindow->getWindow()->setFocused(true);

            openTexturePickerEditorScene();

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2DU(400, 340),
                32,
                "Texture");
         
        m_texturePickerRenderWindow = RenderWindow::Create(params);

        m_texturePickerRenderWindow->setName("TexturePicker");

        if (m_texturePickerRenderWindow && m_texturePickerRenderWindow->getWindow())
        {
            m_texturePickerRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &TexturePickerManager::notifyWindowWillClose);
            m_texturePickerRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &TexturePickerManager::notifyWindowFocusChanged);

            openTexturePickerEditorScene();
        }
    }

    //////////////////////////////////////////
    void TexturePickerManager::closeTexturePickerEditorWindow()
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
    void TexturePickerManager::openTexturePickerEditorScene()
    {
        SceneTexturePickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneTexturePicker>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneTexturePicker>(true, m_texturePickerRenderWindow);
        }

        scene->setup();
    }

    //////////////////////////////////////////
    void TexturePickerManager::closeTexturePickerEditorScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneTexturePickerPtr scene = SceneManager::GetInstancePtr()->getScene<SceneTexturePicker>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void TexturePickerManager::notifyWindowWillClose(Window* _window)
    {
        closeTexturePickerEditorScene();
        closeTexturePickerEditorWindow();
    }

    //////////////////////////////////////////
    void TexturePickerManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
        {
            _window->close();
        }
    }

    //////////////////////////////////////////
    bool TexturePickerManager::isTexturePickerEditorWindowOpened()
    {
        if (!m_texturePickerRenderWindow)
            return false;

        if (!m_texturePickerRenderWindow->getWindow())
            return false;

        return m_texturePickerRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool TexturePickerManager::isTexturePickerEditorWindowFocused()
    {
        if (!m_texturePickerRenderWindow)
            return false;

        if (!m_texturePickerRenderWindow->getWindow())
            return false;

        return m_texturePickerRenderWindow->getWindow()->getFocused();
    }
    
} // namespace Maze
//////////////////////////////////////////
