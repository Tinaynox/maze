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
#include "maze-ui/managers/MazeAnimationCurveManager.hpp"
#include "maze-ui/managers/MazeColorPickerManager.hpp"
#include "maze-ui/curve-editor/MazeSceneCurveEditor.hpp"
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
    // Class AnimationCurveManager
    //
    //////////////////////////////////////////
    AnimationCurveManager* AnimationCurveManager::s_instance = nullptr;

    //////////////////////////////////////////
    AnimationCurveManager::AnimationCurveManager()
        : m_curve(0.0f, 0.0f, 1.0f, 1.0f)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    AnimationCurveManager::~AnimationCurveManager()
    {
        closeAnimationCurveEditorWindow();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void AnimationCurveManager::Initialize(AnimationCurveManagerPtr& _uiManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(AnimationCurveManager, _uiManager, init());
    }

    //////////////////////////////////////////
    bool AnimationCurveManager::init()
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void AnimationCurveManager::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void AnimationCurveManager::openCurveEditor(
        AnimationCurveCallback _callback,
        AnimationCurve const& _curve,
        AnimationCurveMinMaxMode _mode)
    {
        m_callback = nullptr;

        setCurve(_curve);

        m_callback = _callback;

        openAnimationCurveEditorWindow(_mode);
    }

    //////////////////////////////////////////
    void AnimationCurveManager::setCurve(AnimationCurve const& _curve)
    {
        m_curve = _curve;

        eventCurveChanged(m_curve);

        if (m_callback)
            m_callback(m_curve);
    }

    //////////////////////////////////////////
    void AnimationCurveManager::openAnimationCurveEditorWindow(AnimationCurveMinMaxMode _mode)
    {
        if (m_animationCurveEditorRenderWindow &&
            m_animationCurveEditorRenderWindow->getWindow() &&
            m_animationCurveEditorRenderWindow->getWindow()->isOpened())
        {
            m_animationCurveEditorRenderWindow->getWindow()->setFocused(true);

            openAnimationCurveEditorScene(_mode);

            return;
        }

        RenderWindowParams params;
        params.windowParams = 
            WindowParams::Create(
                Vec2DU(400, 340),
                32,
                "Curve");
         
        m_animationCurveEditorRenderWindow = RenderWindow::Create(params);

        m_animationCurveEditorRenderWindow->setName("AnimationCurvePicker");

        if (m_animationCurveEditorRenderWindow && m_animationCurveEditorRenderWindow->getWindow())
        {
            m_animationCurveEditorRenderWindow->getWindow()->eventWindowWillClose.subscribe(this, &AnimationCurveManager::notifyWindowWillClose);
            m_animationCurveEditorRenderWindow->getWindow()->eventWindowFocusChanged.subscribe(this, &AnimationCurveManager::notifyWindowFocusChanged);

            openAnimationCurveEditorScene(_mode);
        }
    }

    //////////////////////////////////////////
    void AnimationCurveManager::closeAnimationCurveEditorWindow()
    {
        m_callback = nullptr;

        if (!m_animationCurveEditorRenderWindow)
            return;

        RenderWindowPtr renderWindow = m_animationCurveEditorRenderWindow;
        m_animationCurveEditorRenderWindow.reset();

        if (!renderWindow->getWindow())
            return;

        renderWindow->getWindow()->eventWindowWillClose.unsubscribe(this);
        renderWindow->getWindow()->eventWindowFocusChanged.unsubscribe(this);
        renderWindow->getWindow()->close();
    }

    //////////////////////////////////////////
    void AnimationCurveManager::openAnimationCurveEditorScene(AnimationCurveMinMaxMode _mode)
    {
        SceneCurveEditorPtr scene = SceneManager::GetInstancePtr()->getScene<SceneCurveEditor>();
        if (!scene)
        {
            scene = SceneManager::GetInstancePtr()->loadScene<SceneCurveEditor>(true, m_animationCurveEditorRenderWindow);
        }

        scene->setup(_mode);
    }

    //////////////////////////////////////////
    void AnimationCurveManager::closeAnimationCurveEditorScene()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneCurveEditorPtr scene = SceneManager::GetInstancePtr()->getScene<SceneCurveEditor>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
            }
        }
    }

    //////////////////////////////////////////
    void AnimationCurveManager::notifyWindowWillClose(Window* _window)
    {
        closeAnimationCurveEditorScene();
        closeAnimationCurveEditorWindow();
    }

    //////////////////////////////////////////
    void AnimationCurveManager::notifyWindowFocusChanged(Window* _window)
    {
        if (!_window->getFocused())
        {
            _window->close();
        }
    }

    //////////////////////////////////////////
    bool AnimationCurveManager::isAnimationCurveEditorWindowOpened()
    {
        if (!m_animationCurveEditorRenderWindow)
            return false;

        if (!m_animationCurveEditorRenderWindow->getWindow())
            return false;

        return m_animationCurveEditorRenderWindow->getWindow()->isOpened();
    }

    //////////////////////////////////////////
    bool AnimationCurveManager::isAnimationCurveEditorWindowFocused()
    {
        if (!m_animationCurveEditorRenderWindow)
            return false;

        if (!m_animationCurveEditorRenderWindow->getWindow())
            return false;

        return m_animationCurveEditorRenderWindow->getWindow()->getFocused();
    }
    
} // namespace Maze
//////////////////////////////////////////
