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
#include "maze-ui/ecs/components/MazeUITweenTransitionAlpha.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UITweenTransitionAlpha
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UITweenTransitionAlpha, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UITweenTransitionAlpha);

    //////////////////////////////////////////
    UITweenTransitionAlpha::UITweenTransitionAlpha()
        : m_isHidden(true)
        , m_showTime(0.3f)
        , m_hideKoef(1.0f)
        , m_hiddenAlpha(0.0f)
        , m_shownAlpha(1.0f)
        , m_alphaPower(1.0f)
        , m_hidingProgress(1.0f)
        , m_showSpeed(0.0f)
        , m_hideSpeed(0.0f)
    {
    }

    //////////////////////////////////////////
    UITweenTransitionAlpha::~UITweenTransitionAlpha()
    {

    }

    //////////////////////////////////////////
    UITweenTransitionAlphaPtr UITweenTransitionAlpha::Create()
    {
        UITweenTransitionAlphaPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(UITweenTransitionAlpha, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool UITweenTransitionAlpha::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_canvasGroup = getEntityRaw()->ensureComponent<CanvasGroup>();

        updateShowSpeed();

        if (m_isHidden)
            m_hidingProgress = 1.0f;
        else
            m_hidingProgress = 0.0f;

        updateCanvasGroup();
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::setHidden(bool _isHidden, bool _resetProgress)
    {
        if (m_isHidden == _isHidden)
            return;

        m_isHidden = _isHidden;

        if (_resetProgress)
        {
            if (m_isHidden)
            {
                m_hidingProgress = 0.0f;
            }
            else
            {
                m_hidingProgress = 1.0f;
            }

            updateCanvasGroup();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::setShowTime(F32 _showTime)
    {
        if (m_showTime == _showTime)
            return;

        m_showTime = _showTime;

        updateShowSpeed();
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::processUpdate(F32 _dt)
    {
        if (m_unscaledDeltaTime)
            _dt = UpdateManager::GetInstancePtr()->getUnscaledDeltaTime();

        if (m_isHidden && m_hidingProgress < 1.0f)
        {
            m_hidingProgress += _dt * m_hideSpeed;
            if (m_hidingProgress >= 1.0f)
            {
                m_hidingProgress = 1.0f;

                eventHideComplete(this);

                if (m_onHideComplete)
                {
                    m_onHideComplete();
                    m_onHideComplete.reset();
                }
            }

            updateCanvasGroup();
        }
        else if (!m_isHidden && m_hidingProgress > 0.0f)
        {
            m_hidingProgress -= _dt * m_showSpeed;
            if (m_hidingProgress <= 0.0f)
            {
                m_hidingProgress = 0.0f;

                eventShowComplete(this);

                if (m_onShowComplete)
                {
                    m_onShowComplete();
                    m_onShowComplete.reset();
                }
            }

            updateCanvasGroup();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::show(Delegate<void> const& _onComplete)
    {
        m_onShowComplete = _onComplete;

        setHidden(false);
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::hide(Delegate<void> const& _onComplete)
    {
        m_onHideComplete = _onComplete;

        setHidden(true);
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::showInstantly()
    {
        m_isHidden = false;
        m_hidingProgress = 0.0f;
        updateCanvasGroup();

        eventShowComplete(this);

        if (m_onShowComplete)
        {
            m_onShowComplete();
            m_onShowComplete.reset();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::hideInstantly()
    {
        m_isHidden = true;
        m_hidingProgress = 1.0f;
        updateCanvasGroup();

        eventHideComplete(this);

        if (m_onHideComplete)
        {
            m_onHideComplete();
            m_onHideComplete.reset();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::updateCanvasGroup()
    {
        F32 showingProgress = 1.0f - m_hidingProgress;

        F32 progress = m_animationCurve.evaluate(showingProgress);

        F32 alpha = Math::Lerp(m_hiddenAlpha, m_shownAlpha, progress);

        if (m_alphaPower != 1.0f)
        {
            alpha = pow(alpha, m_alphaPower);
        }

        m_canvasGroup->setLocalAlpha(alpha);
        m_canvasGroup->setBlocksRaycasts(!m_isHidden);
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::updateShowSpeed()
    {
        if (m_showTime > 0.0f)
            m_showSpeed = 1.0f / m_showTime;
        else
            m_showSpeed = F32_MAX;

        updateHideSpeed();
    }

    //////////////////////////////////////////
    void UITweenTransitionAlpha::updateHideSpeed()
    {
        if (m_hideKoef > 0.0f)
            m_hideSpeed = m_showSpeed / m_hideKoef;
        else
            m_hideSpeed = m_showSpeed;
    }



    //////////////////////////////////////////
    SIMPLE_COMPONENT_SYSTEM(UITweenTransitionAlphaSystem, 300,
        UpdateEvent const& _event,
        Entity* _entity,
        UITweenTransitionAlpha* _tweenTransition)
    {
        _tweenTransition->processUpdate(_event.getDt());
    }
    
    
} // namespace Maze
//////////////////////////////////////////
