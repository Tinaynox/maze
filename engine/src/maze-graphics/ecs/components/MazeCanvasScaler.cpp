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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(CanvasScalerScaleMode)

    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(CanvasScalerScreenMatchMode)


    //////////////////////////////////////////
    // Class CanvasScaler
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(CanvasScaler, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CanvasScalerScaleMode, scaleMode, CanvasScalerScaleMode::ConstantPixelSize, getScaleMode, setScaleMode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(CanvasScalerScreenMatchMode, screenMatchMode, CanvasScalerScreenMatchMode::MatchWidthOrHeight, getScreenMatchMode, setScreenMatchMode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2F, referenceResolution, Vec2F(800, 600), getReferenceResolution, setReferenceResolution),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, matchWidthOrHeight, 0.0f, getMatchWidthOrHeight, setMatchWidthOrHeight));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(CanvasScaler);

    //////////////////////////////////////////
    CanvasScaler::CanvasScaler()
        : m_scaleMode(CanvasScalerScaleMode::ConstantPixelSize)
        , m_screenMatchMode(CanvasScalerScreenMatchMode::MatchWidthOrHeight)
        , m_referenceResolution(800, 600)
        , m_matchWidthOrHeight(0.0f)
    {
    }

    //////////////////////////////////////////
    CanvasScaler::~CanvasScaler()
    {
    }

    //////////////////////////////////////////
    CanvasScalerPtr CanvasScaler::Create()
    {
        CanvasScalerPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(CanvasScaler, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool CanvasScaler::init()
    {
            

        return true;
    }

    //////////////////////////////////////////
    void CanvasScaler::setScaleMode(CanvasScalerScaleMode _scaleMode)
    {
        if (m_scaleMode == _scaleMode)
            return;

        m_scaleMode = _scaleMode;

        updateCanvasScale();
    }

    //////////////////////////////////////////
    void CanvasScaler::setScreenMatchMode(CanvasScalerScreenMatchMode _screenMatchMode)
    {
        if (m_screenMatchMode == _screenMatchMode)
            return;

        m_screenMatchMode = _screenMatchMode;

        updateCanvasScale();
    }

    //////////////////////////////////////////
    void CanvasScaler::setReferenceResolution(Vec2F _referenceResolution)
    {
        m_referenceResolution = _referenceResolution;

        const F32 c_minimumResolution = 0.00001f;

        if (m_referenceResolution.x > -c_minimumResolution && m_referenceResolution.x < c_minimumResolution)
            m_referenceResolution.x = c_minimumResolution * Math::Sign(m_referenceResolution.x);

        if (m_referenceResolution.y > -c_minimumResolution && m_referenceResolution.y < c_minimumResolution) 
            m_referenceResolution.y = c_minimumResolution * Math::Sign(m_referenceResolution.y);
    }

    //////////////////////////////////////////
    void CanvasScaler::setMatchWidthOrHeight(F32 _matchWidthOrHeight)
    {
        if (m_matchWidthOrHeight == _matchWidthOrHeight)
            return;

        m_matchWidthOrHeight = _matchWidthOrHeight;

        updateCanvasScale();
    }

    //////////////////////////////////////////
    void CanvasScaler::processEntityAwakened()
    {
        m_canvas = getEntityRaw()->getComponent<Canvas>();
    }

    //////////////////////////////////////////
    void CanvasScaler::setCanvasScale(F32 _scale)
    {
        if (m_canvas->getTransform()->getLocalScale().x == _scale)
            return;

        m_canvas->getTransform()->setLocalScale(_scale);
    }

    //////////////////////////////////////////
    void CanvasScaler::updateCanvasScale()
    {
        if (!m_canvas)
            return;
        
        if (m_canvas->getViewportTransformPolicy() != ViewportTransformPolicy::ViewportToTransform)
            return;

        switch (m_scaleMode)
        {
            case CanvasScalerScaleMode::ConstantPixelSize: handleConstantPixelSize(); break;
            case CanvasScalerScaleMode::ScaleWithViewportSize: handleScaleWithViewportSize(); break;
            default: break;
        }
    }

    //////////////////////////////////////////
    void CanvasScaler::handleConstantPixelSize()
    {
        if (!m_canvas->getRenderTarget())
            return;

        Vec2F viewportSize = (Vec2F)m_canvas->getRenderTarget()->getRenderTargetSize() * m_canvas->getViewport().size;

        setCanvasScale(1.0f);
        m_canvas->getTransform()->setSize(viewportSize);
    }

    //////////////////////////////////////////
    void CanvasScaler::handleScaleWithViewportSize()
    {
        if (!m_canvas->getRenderTarget())
            return;

        Vec2F viewportSize = (Vec2F)m_canvas->getRenderTarget()->getRenderTargetSize() * m_canvas->getViewport().size;

        F32 scaleFactor = 0;
        F32 width = 0;
        F32 height = 0;

        F32 x = viewportSize.x / m_referenceResolution.x;
        F32 y = viewportSize.y / m_referenceResolution.y;
        F32 screenSizeAR = viewportSize.y != 0.0f ? viewportSize.x / viewportSize.y : 1.0f;

        switch (m_screenMatchMode)
        {
            case CanvasScalerScreenMatchMode::MatchWidthOrHeight:
            {
                F32 logWidth = log2(x);
                F32 logHeight = log2(y);
                F32 logWeightedAverage = Math::Lerp(logWidth, logHeight, m_matchWidthOrHeight);
                scaleFactor = pow(2.0f, logWeightedAverage);

                width = Math::Lerp(m_referenceResolution.x, m_referenceResolution.y * screenSizeAR, m_matchWidthOrHeight);
                height = Math::Lerp(m_referenceResolution.x / screenSizeAR, m_referenceResolution.y, m_matchWidthOrHeight);
                    
                break;
            }
            case CanvasScalerScreenMatchMode::Expand:
            {
                scaleFactor = Math::Min(x, y);

                if (x < y)
                {
                    width = m_referenceResolution.x;
                    height = m_referenceResolution.x / screenSizeAR;
                }
                else
                {
                    width = m_referenceResolution.y * screenSizeAR;
                    height = m_referenceResolution.y;
                }

                break;
            }
            case CanvasScalerScreenMatchMode::Shrink:
            {
                scaleFactor = Math::Max(x, y);

                if (x < y)
                {
                    width = m_referenceResolution.y * screenSizeAR;
                    height = m_referenceResolution.y;
                }
                else
                {
                    width = m_referenceResolution.x;
                    height = m_referenceResolution.x / screenSizeAR;
                }

                break;
            }
            default:
                break;
        }

        m_canvas->getTransform()->setSize(Vec2F(width, height));
        setCanvasScale(scaleFactor);
    }
    
    
} // namespace Maze
//////////////////////////////////////////
