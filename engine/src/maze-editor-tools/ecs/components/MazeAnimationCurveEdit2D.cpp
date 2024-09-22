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
#include "maze-editor-tools/ecs/components/MazeAnimationCurveEdit2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-editor-tools/managers/MazeAnimationCurveManager.hpp"
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
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AnimationCurveEdit2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AnimationCurveEdit2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, selected, false, getSelected, setSelected),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ComponentPtr, curveRenderer, ComponentPtr(), getCurveRendererComponent, setCurveRenderer),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(AnimationCurve, curve, AnimationCurve(), getCurve, setCurve));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(AnimationCurveEdit2D);

    //////////////////////////////////////////
    AnimationCurveEdit2D::AnimationCurveEdit2D()
        : m_selected(false)
    {
    }

    //////////////////////////////////////////
    AnimationCurveEdit2D::~AnimationCurveEdit2D()
    {
        if (m_UIElement2D)
        {
            m_UIElement2D->eventCursorPressIn.unsubscribe(this);
            m_UIElement2D->eventCursorReleaseOut.unsubscribe(this);
            m_UIElement2D->eventFocusChanged.unsubscribe(this);
            m_UIElement2D->eventPressedChanged.unsubscribe(this);
            m_UIElement2D->eventClick.unsubscribe(this);
        }
    }

    //////////////////////////////////////////
    AnimationCurveEdit2DPtr AnimationCurveEdit2D::Create()
    {
        AnimationCurveEdit2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AnimationCurveEdit2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool AnimationCurveEdit2D::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::update(F32 _dt)
    {
        
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_UIElement2D = getEntityRaw()->ensureComponent<UIElement2D>();
        m_UIElement2D->setCaptureCursorHits(true);

        m_UIElement2D->eventCursorPressIn.subscribe(this, &AnimationCurveEdit2D::notifyCursorPressIn);
        m_UIElement2D->eventCursorReleaseOut.subscribe(this, &AnimationCurveEdit2D::notifyCursorReleaseOut);
        m_UIElement2D->eventFocusChanged.subscribe(this, &AnimationCurveEdit2D::notifyFocusChanged);
        m_UIElement2D->eventPressedChanged.subscribe(this, &AnimationCurveEdit2D::notifyPressedChanged);
        m_UIElement2D->eventClick.subscribe(this, &AnimationCurveEdit2D::notifyClick);

        m_curvePixelSheet.setFormat(PixelFormat::RGBA_U8);

        m_curveTexture = Texture2D::Create();
        m_curveTexture->setMagFilter(TextureFilter::Nearest);
        m_curveTexture->setMinFilter(TextureFilter::Linear);

        m_curveSprite = Sprite::Create(m_curveTexture);
        m_curveSprite->setName(MAZE_HS("AnimationCurveSprite"));
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::notifyCursorPressIn(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
            
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::notifyCursorReleaseOut(CursorInputEvent const& _inputEvent)
    {
        setSelected(false);
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::notifyFocusChanged(bool _focused)
    {
        eventFocusChanged(this, _focused);
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::notifyPressedChanged(bool _pressed)
    {
        if (_pressed)
        {
            setSelected(true);
        }
        

        eventPressedChanged(this, _pressed);
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::notifyClick(Vec2F const& _positionOS, CursorInputEvent const& _inputEvent)
    {
        if (_inputEvent.button != 0)
            return;

        AnimationCurveEdit2DWPtr weakPtr = cast<AnimationCurveEdit2D>();
        AnimationCurveManager::GetInstancePtr()->openCurveEditor(
            [weakPtr](AnimationCurve const& _value)
            {
                AnimationCurveEdit2DPtr ptr = weakPtr.lock();
                if (ptr)
                    ptr->setCurve(_value);
            },
            m_curve);

        eventClick(this);
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::setSelected(bool _selected)
    {
        if (m_selected == _selected)
            return;

        m_selected = _selected;

        eventSelectedChanged(this, m_selected);
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::setCurve(AnimationCurve const& _value)
    {
        if (m_curve == _value)
            return;

        m_curve = _value;

        eventCurveChanged(this, m_curve);

        processCurve();
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::processCurve()
    {
        if (!m_curveRenderer)
            return;

        m_curvePixelSheet.setSize(m_curveRenderer->getTransform()->getSize());
        m_curvePixelSheet.fill(ColorU32(86, 86, 86));

        F32 startTime = m_curve.getStartTime();
        F32 time = m_curve.getTime();

        F32 width = (F32)m_curvePixelSheet.getSize().x - 1;
        F32 heigth = (F32)m_curvePixelSheet.getSize().y - 1.0f;

        if (m_curve.getMinMaxMode() == AnimationCurveMinMaxMode::NormalizedPositive)
        {
            F32 dt = 1.0f / (m_curvePixelSheet.getSize().x * 4);

            for (F32 x = 0.0f; x < (1.0f - dt); x += dt)
            {
                F32 x0 = x;
                F32 x1 = x + dt;

                F32 y0 = m_curve.evaluateUnscaled(x0);
                F32 y1 = m_curve.evaluateUnscaled(x1);

                if (Math::Abs(y0) >= 10000 || Math::Abs(y1) >= 10000)
                    continue;

                m_curvePixelSheet.drawLine(
                    (S32)(x0 * width), (S32)(y0 * heigth),
                    (S32)(x1 * width), (S32)(y1 * heigth),
                    ColorU32::c_green);
            }
        }
        else
        if (m_curve.getMinMaxMode() == AnimationCurveMinMaxMode::Normalized)
        {
            m_curvePixelSheet.drawLine(
                (S32)(0.0f), (S32)(heigth * 0.5f),
                (S32)(width), (S32)(heigth * 0.5f),
                ColorU32(70, 70, 70));

            F32 dt = 1.0f / (width * 4);

            for (F32 x = 0.0f; x < (1.0f - dt); x += dt)
            {
                F32 x0 = x;
                F32 x1 = x + dt;

                F32 y0 = (m_curve.evaluateUnscaled(x0) + 1.0f) * 0.5f;
                F32 y1 = (m_curve.evaluateUnscaled(x1) + 1.0f) * 0.5f;

                if (Math::Abs(y0) >= 10000 || Math::Abs(y1) >= 10000)
                    continue;

                m_curvePixelSheet.drawLine(
                    (S32)(x0 * width), (S32)(y0 * heigth),
                    (S32)(x1 * width), (S32)(y1 * heigth),
                    ColorU32::c_green);
            }
        }
        else
        {
            F32 maxValue = F32_MIN;
            F32 minValue = F32_MAX;
            Vector<F32> values;

            S32 w = (S32)m_curveRenderer->getTransform()->getSize().x;

            for (S32 x = 0; x < w; x++)
            {
                F32 p = (F32)x / m_curveRenderer->getTransform()->getSize().x;

                F32 t = startTime + time * p;

                F32 fy = m_curve.evaluate(t);

                maxValue = Math::Max(fy, maxValue);
                minValue = Math::Min(fy, minValue);

                values.push_back(fy);
            }

            F32 deltaValue = maxValue - minValue;
            if (deltaValue != 0.0f)
            {
                F32 inv = m_curveRenderer->getTransform()->getSize().y / deltaValue;
                for (F32& v : values)
                {
                    v -= minValue;
                    v *= inv;
                }
            }
            else
            {
                for (F32& v : values)
                    v += (m_curveRenderer->getTransform()->getSize().y * 0.5f - deltaValue * 0.5f);
            }


            for (S32 i = 0; i < w - 1; i++)
            {
                S32 y0 = (S32)values[i];
                S32 y1 = (S32)values[i + 1];

                S32 x0 = i;
                S32 x1 = i + 1;

                m_curvePixelSheet.drawLine(
                    x0, y0,
                    x1, y1,
                    ColorU32::c_green);
            }
        }

        m_curveTexture->loadTexture(m_curvePixelSheet);
        m_curveSprite->set(m_curveTexture);
    }

    //////////////////////////////////////////
    void AnimationCurveEdit2D::subscribeCurveRenderer()
    {
        if (m_curveRenderer)
            m_curveRenderer->setSprite(m_curveSprite);

        processCurve();
    }
    
    //////////////////////////////////////////
    void AnimationCurveEdit2D::normalize()
    {
        setCurve(m_curve.normalized());
    }


} // namespace Maze
//////////////////////////////////////////
