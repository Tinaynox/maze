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
#include "maze-ui/ecs/components/MazeUITweenTransitionScale.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class UITweenTransitionScale
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(UITweenTransitionScale, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(UITweenTransitionScale);

    //////////////////////////////////////////
    UITweenTransitionScale::UITweenTransitionScale()
        : m_isHidden(true)
        , m_showTime(0.3f)
        , m_hideKoef(1.0f)
        , m_hiddenScale(Vec2DF::c_zero)
        , m_shownScale(Vec2DF::c_one)
        , m_hidingProgress(1.0f)
        , m_showSpeed(0.0f)
        , m_hideSpeed(0.0f)
    {
    }

    //////////////////////////////////////////
    UITweenTransitionScale::~UITweenTransitionScale()
    {

    }

    //////////////////////////////////////////
    UITweenTransitionScalePtr UITweenTransitionScale::Create()
    {
        UITweenTransitionScalePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(UITweenTransitionScale, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool UITweenTransitionScale::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();

        updateShowSpeed();

        if (m_isHidden)
            m_hidingProgress = 1.0f;
        else
            m_hidingProgress = 0.0f;

        updateTransform();
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::setHidden(bool _isHidden, bool _resetProgress)
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

            updateTransform();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::setShowTime(F32 _showTime)
    {
        if (m_showTime == _showTime)
            return;

        m_showTime = _showTime;

        updateShowSpeed();
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::processUpdate(F32 _dt)
    {
        if (m_isHidden && m_hidingProgress < 1.0f)
        {
            m_hidingProgress += _dt * m_hideSpeed;
            if (m_hidingProgress >= 1.0f)
            {
                m_hidingProgress = 1.0f;

                eventHideComplete();

                if (m_onHideComplete)
                {
                    m_onHideComplete();
                    m_onHideComplete.reset();
                }
            }

            updateTransform();
        }
        else if (!m_isHidden && m_hidingProgress > 0.0f)
        {
            m_hidingProgress -= _dt * m_showSpeed;
            if (m_hidingProgress <= 0.0f)
            {
                m_hidingProgress = 0.0f;

                eventShowComplete();

                if (m_onShowComplete)
                {
                    m_onShowComplete();
                    m_onShowComplete.reset();
                }
            }

            updateTransform();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::show(Delegate<void> const& _onComplete)
    {
        m_onShowComplete = _onComplete;

        setHidden(false);
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::hide(Delegate<void> const& _onComplete)
    {
        m_onHideComplete = _onComplete;

        setHidden(true);
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::showInstantly()
    {
        m_isHidden = false;
        m_hidingProgress = 0.0f;
        updateTransform();

        eventShowComplete();

        if (m_onShowComplete)
        {
            m_onShowComplete();
            m_onShowComplete.reset();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::hideInstantly()
    {
        m_isHidden = true;
        m_hidingProgress = 1.0f;
        updateTransform();

        eventHideComplete();

        if (m_onHideComplete)
        {
            m_onHideComplete();
            m_onHideComplete.reset();
        }
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::updateTransform()
    {
        F32 showingProgress = 1.0f - m_hidingProgress;

        Vec2DF scale(
            Math::Lerp(m_hiddenScale.x, m_shownScale.x, showingProgress),
            Math::Lerp(m_hiddenScale.y, m_shownScale.y, showingProgress));

        m_transform->setLocalScale(scale);
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::updateShowSpeed()
    {
        if (m_showTime > 0.0f)
            m_showSpeed = 1.0f / m_showTime;
        else
            m_showSpeed = F32_MAX;

        updateHideSpeed();
    }

    //////////////////////////////////////////
    void UITweenTransitionScale::updateHideSpeed()
    {
        if (m_hideKoef > 0.0f)
            m_hideSpeed = m_showSpeed / m_hideKoef;
        else
            m_hideSpeed = m_showSpeed;
    }
    
    
} // namespace Maze
//////////////////////////////////////////
