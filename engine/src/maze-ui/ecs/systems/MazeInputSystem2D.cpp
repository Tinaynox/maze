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
#include "maze-ui/ecs/systems/MazeInputSystem2D.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-ui/ecs/components/MazeSystemTextEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeSystemTextDropdown2D.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    U32 const c_doubleClickTimeMS = 175u;
    F32 const c_doubleClickShiftThresholdSq = 5.0f * 5.0f;
    U32 const c_clickTimeMS = 145u;
    F32 const c_clickShiftThresholdSq = 5.0f * 5.0f;


    //////////////////////////////////////////
    // Class InputSystem2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InputSystem2D, ComponentSystem);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(InputSystem2D);

    //////////////////////////////////////////
    InputSystem2D::InputSystem2D()
        : m_sortedUIElements2DDirty(false)
    {
    }

    //////////////////////////////////////////
    InputSystem2D::~InputSystem2D()
    {
        m_canvasesSample->eventEntityAdded.unsubscribe(this);
        m_canvasesSample->eventEntityRemoved.unsubscribe(this);

        m_UIElements2DSample->eventEntityAdded.unsubscribe(this);
        m_UIElements2DSample->eventEntityRemoved.unsubscribe(this);

        InputManager::GetInstancePtr()->eventMouse.unsubscribe(this);
        InputManager::GetInstancePtr()->eventTouch.unsubscribe(this);
    }

    //////////////////////////////////////////
    InputSystem2DPtr InputSystem2D::Create()
    {
        InputSystem2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(InputSystem2D, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool InputSystem2D::init()
    {
        InputManager::GetInstancePtr()->eventMouse.subscribe(this, &InputSystem2D::notifyMouse);
        InputManager::GetInstancePtr()->eventTouch.subscribe(this, &InputSystem2D::notifyTouch);        

        return true;
    }

    //////////////////////////////////////////
    void InputSystem2D::processSystemAdded()
    {
        m_canvasesSample = m_worldRaw->requestInclusiveSample<Canvas>();
        m_canvasesSample->eventEntityAdded.subscribe(this, &InputSystem2D::processCanvasEntityAdded);
        m_canvasesSample->eventEntityRemoved.subscribe(this, &InputSystem2D::processCanvasEntityRemoved);
        updateSortedCanvasesList();

        m_canvasScalersSample = m_worldRaw->requestInclusiveSample<CanvasScaler>();


        m_UIElements2DSample = m_worldRaw->requestInclusiveSample<UIElement2D>();
        m_UIElements2DSample->eventEntityAdded.subscribe(this, &InputSystem2D::processUIElement2DEntityAdded);
        m_UIElements2DSample->eventEntityRemoved.subscribe(this, &InputSystem2D::processUIElement2DEntityRemoved);

        m_systemTextEditBoxesSample = m_worldRaw->requestInclusiveSample<SystemTextEditBox2D>();
        m_systemTextDropdownsSample = m_worldRaw->requestInclusiveSample<SystemTextDropdown2D>();
        m_horizontalLayouts2D = m_worldRaw->requestInclusiveSample<HorizontalLayout2D>();
        m_verticalLayouts2D = m_worldRaw->requestInclusiveSample<VerticalLayout2D>();
        m_sizePolicy2D = m_worldRaw->requestInclusiveSample<SizePolicy2D>();
        m_scrollRects2D = m_worldRaw->requestInclusiveSample<ScrollRect2D>();
    }

    //////////////////////////////////////////
    void InputSystem2D::processUpdate(F32 _dt)
    {
        if (m_processClick)
        {
            U32 curTime = UpdateManager::GetInstancePtr()->getMilliseconds();
            if (curTime >= m_firstCursorPressTime + c_doubleClickTimeMS)
            {
                processCursorClick(
                    m_clickData.window,
                    m_clickData.cursorIndex,
                    m_clickData.buttonIndex,
                    m_clickData.renderTargetCoords,
                    m_clickData.inputSource);
                m_processClick = false;

                m_firstCursorPressTime = 0u;
            }
        }

        struct Layout2DSortData
        {
            Layout2D* layout;
            Size nestingLevel;
        };

        Vector<Layout2DSortData> layouts;
        m_horizontalLayouts2D->process(
            [&](Entity* _entity, HorizontalLayout2D* _layout)
            {
                layouts.emplace_back(
                    Layout2DSortData
                    {
                        _layout,
                        _layout->getTransform()->getNestingLevel()
                    });
            });

        m_verticalLayouts2D->process(
            [&](Entity* _entity, VerticalLayout2D* _layout)
            {
                layouts.emplace_back(
                    Layout2DSortData
                    {
                        _layout,
                        _layout->getTransform()->getNestingLevel()
                    });
            });

        std::sort(
            layouts.begin(),
            layouts.end(),
            [](Layout2DSortData const& _layoutData0, Layout2DSortData const& _layoutData1)
            {
                Size nestingLevel0 = _layoutData0.nestingLevel;
                Size zOrder0 = _layoutData0.layout->getTransform()->getZ();
                Size orderOfArrival0 = _layoutData0.layout->getTransform()->getOrderOfArrival();

                Size nestingLevel1 = _layoutData1.nestingLevel;
                Size zOrder1 = _layoutData1.layout->getTransform()->getZ();
                Size orderOfArrival1 = _layoutData1.layout->getTransform()->getOrderOfArrival();

                if (nestingLevel0 > nestingLevel1)
                    return true;
                else
                if (nestingLevel0 == nestingLevel1)
                {
                    if (zOrder0 > zOrder1)
                        return true;
                    else
                    if (zOrder0 == zOrder1)
                    {
                        if (orderOfArrival0 > orderOfArrival1)
                            return true;
                    }
                }            

                return false;
            });

        m_canvasScalersSample->process(
            [&](Entity* _entity, CanvasScaler* _canvasScaler)
            {
                _canvasScaler->updateCanvasScale();
            });

        m_sizePolicy2D->process(
            [](Entity* entity, SizePolicy2D* _sizePolicy)
            {
                if (_sizePolicy->getTransform()->isWorldTransformChanged())
                    _sizePolicy->updateSize();
            });

        for (Layout2DSortData const& layoutData : layouts)
            layoutData.layout->update();

        m_systemTextEditBoxesSample->process(
            [&](Entity* _entity, SystemTextEditBox2D* _editBox)
            {
                _editBox->update(_dt);
            });

        m_systemTextDropdownsSample->process(
            [&](Entity* _entity, SystemTextDropdown2D* _dropdown)
            {
                _dropdown->update(_dt);
            });

        m_scrollRects2D->process(
            [&](Entity* _entity, ScrollRect2D* _scrollRect)
            {
                _scrollRect->update(_dt);
            });
    }

    //////////////////////////////////////////
    void InputSystem2D::processCanvasEntityAdded(Entity* _entity, Canvas* _canvas)
    {
        updateSortedCanvasesList();
    }

    //////////////////////////////////////////
    void InputSystem2D::processCanvasEntityRemoved(Entity* _entity, Canvas* _canvas)
    {
        updateSortedCanvasesList();
    }

    //////////////////////////////////////////
    void InputSystem2D::processUIElement2DEntityAdded(Entity* _entity, UIElement2D* _UIElement2D)
    {
        m_sortedUIElements2DDirty = true;
    }

    //////////////////////////////////////////
    void InputSystem2D::processUIElement2DEntityRemoved(Entity* _entity, UIElement2D* _UIElement2D)
    {
        m_sortedUIElements2DDirty = true;
    }

    //////////////////////////////////////////
    void InputSystem2D::updateSortedCanvasesList()
    {
        m_sortedCanvases.clear();

        m_canvasesSample->process(
            [&](Entity* _entity, Canvas* _canvas)        
            {
                m_sortedCanvases.emplace_back(_canvas);
            });
        std::sort(
            m_sortedCanvases.begin(),
            m_sortedCanvases.end(),
            [](Canvas* _canvas0, Canvas* _canvas1) -> bool
            {
                return _canvas0->getSortOrder() < _canvas1->getSortOrder();
            });

        m_sortedUIElements2DDirty = true;
    }

    //////////////////////////////////////////
    void InputSystem2D::notifyMouse(Maze::InputEventMouseData const& _mouseData)
    {
        Vec2DF mousePosition = m_coordsConverter(Vec2DF((F32)_mouseData.x, (F32)_mouseData.y));

        switch (_mouseData.type)
        {
            case InputEventMouseType::ButtonDown:
            {
                processCursorPress(_mouseData.window, 0, _mouseData.buttonId, mousePosition, CursorInputSource::Mouse);
                break;
            }
            case InputEventMouseType::ButtonUp:
            {
                processCursorRelease(_mouseData.window, 0, _mouseData.buttonId, mousePosition, CursorInputSource::Mouse);
                break;
            }
            case InputEventMouseType::Move:
            {
                processCursorMove(_mouseData.window, 0, _mouseData.buttonId, mousePosition, CursorInputSource::Mouse);
                break;
            }
            case InputEventMouseType::Drag:
            {
                processCursorDrag(_mouseData.window, 0, _mouseData.buttonId, mousePosition, CursorInputSource::Mouse);
                break;
            }
            case InputEventMouseType::Wheel:
            {
                processCursorWheel(_mouseData.window, 0, (F32)_mouseData.z, mousePosition, CursorInputSource::Mouse);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::notifyTouch(Maze::InputEventTouchData const& _touchData)
    {
        Vec2DF touchPosition = m_coordsConverter(Vec2DF((F32)_touchData.x, (F32)_touchData.y));

        switch (_touchData.type)
        {
            case InputEventTouchType::Press:
            {
                processCursorMove(_touchData.window, _touchData.index, 0, touchPosition, CursorInputSource::Touch);
                processCursorPress(_touchData.window, _touchData.index, 0, touchPosition, CursorInputSource::Touch);
                break;
            }
            case InputEventTouchType::Release:
            {
                processCursorRelease(_touchData.window, _touchData.index, 0, touchPosition, CursorInputSource::Touch);
                break;
            }
            case InputEventTouchType::Move:
            {
                processCursorMove(_touchData.window, _touchData.index, 0, touchPosition, CursorInputSource::Touch);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorPress(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2DF const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Press,
            _cursorIndex,
            Maze::Vec2DF::c_zero,
            _buttonIndex,
            _inputSource,
            _window);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedUIElements2D.rbegin(),
                                                        end = m_sortedUIElements2D.rend();
                                                        it != end;
                                                        ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (_window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            cursorInputEvent.canvas = canvasData.canvas;
            cursorInputEvent.rootCanvas = canvasData.rootCanvas;
            Vector<UIElement2D*> const& sortedUIElements2D = canvasData.sortedUIElements2D;

            if (cursorInputEvent.rootCanvas)
                cursorInputEvent.position = cursorInputEvent.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = cursorInputEvent.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElement2D*>::const_reverse_iterator   it2 = sortedUIElements2D.rbegin(),
                                                                end2 = sortedUIElements2D.rend();
                                                                it2 != end2;
                                                                ++it2)
            {
                UIElement2D* element = *it2;

                element->processCursorPress(cursorInputEvent);

                if (cursorInputEvent.hitCaptured)
                    break;
            }

            if (cursorInputEvent.hitCaptured)
                break;
        }

        if (_cursorIndex == 0 && _buttonIndex == 0)
        {
            U32 curTime = UpdateManager::GetInstancePtr()->getMilliseconds();
            if (curTime - m_firstCursorPressTime < c_doubleClickTimeMS &&
                (_renderTargetCoords - m_firstCursorPressRenderTargetCoords).squaredLength() <= c_doubleClickShiftThresholdSq)
            {
                processCursorDoubleClick(_window, _cursorIndex, _buttonIndex, _renderTargetCoords, _inputSource);

                m_processClick = false;
                m_firstCursorPressTime = 0u;
            }
            else
            {
                m_firstCursorPressTime = curTime;
                m_firstCursorPressRenderTargetCoords = _renderTargetCoords;
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorClick(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2DF const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        CursorInputEvent cursorInputEvent(
            CursorInputType::Click,
            _cursorIndex,
            Maze::Vec2DF::c_zero,
            _buttonIndex,
            _inputSource,
            _window);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedUIElements2D.rbegin(),
            end = m_sortedUIElements2D.rend();
            it != end;
            ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (_window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            cursorInputEvent.canvas = canvasData.canvas;
            cursorInputEvent.rootCanvas = canvasData.rootCanvas;
            Vector<UIElement2D*> const& sortedUIElements2D = canvasData.sortedUIElements2D;

            if (cursorInputEvent.rootCanvas)
                cursorInputEvent.position = cursorInputEvent.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = cursorInputEvent.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElement2D*>::const_reverse_iterator   it2 = sortedUIElements2D.rbegin(),
                end2 = sortedUIElements2D.rend();
                it2 != end2;
                ++it2)
            {
                UIElement2D* element = *it2;

                element->processCursorClick(cursorInputEvent);

                if (cursorInputEvent.hitCaptured)
                    break;
            }

            if (cursorInputEvent.hitCaptured)
                break;
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorDoubleClick(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2DF const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        CursorInputEvent cursorInputEvent(
            CursorInputType::DoubleClick,
            _cursorIndex,
            Maze::Vec2DF::c_zero,
            _buttonIndex,
            _inputSource,
            _window);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedUIElements2D.rbegin(),
            end = m_sortedUIElements2D.rend();
            it != end;
            ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (_window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            cursorInputEvent.canvas = canvasData.canvas;
            cursorInputEvent.rootCanvas = canvasData.rootCanvas;
            Vector<UIElement2D*> const& sortedUIElements2D = canvasData.sortedUIElements2D;

            if (cursorInputEvent.rootCanvas)
                cursorInputEvent.position = cursorInputEvent.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = cursorInputEvent.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElement2D*>::const_reverse_iterator   it2 = sortedUIElements2D.rbegin(),
                end2 = sortedUIElements2D.rend();
                it2 != end2;
                ++it2)
            {
                UIElement2D* element = *it2;

                element->processCursorDoubleClick(cursorInputEvent);

                if (cursorInputEvent.hitCaptured)
                    break;
            }

            if (cursorInputEvent.hitCaptured)
                break;
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorRelease(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2DF const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Press,
            _cursorIndex,
            Maze::Vec2DF::c_zero,
            _buttonIndex,
            _inputSource,
            _window);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedUIElements2D.rbegin(),
            end = m_sortedUIElements2D.rend();
            it != end;
            ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (_window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            cursorInputEvent.canvas = canvasData.canvas;
            cursorInputEvent.rootCanvas = canvasData.rootCanvas;
            Vector<UIElement2D*> const& sortedUIElements2D = canvasData.sortedUIElements2D;

            if (cursorInputEvent.rootCanvas)
                cursorInputEvent.position = cursorInputEvent.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = cursorInputEvent.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElement2D*>::const_reverse_iterator    it2 = sortedUIElements2D.rbegin(),
                                                                end2 = sortedUIElements2D.rend();
                                                                it2 != end2;
                                                                ++it2)
            {
                UIElement2D* element = *it2;

                element->processCursorRelease(cursorInputEvent);
            }
        }

        if (_cursorIndex == 0 && _buttonIndex == 0)
        {
            U32 curTime = UpdateManager::GetInstancePtr()->getMilliseconds();
            if (curTime - m_firstCursorPressTime < c_clickTimeMS &&
                (_renderTargetCoords - m_firstCursorPressRenderTargetCoords).squaredLength() <= c_clickShiftThresholdSq)
            {
                m_clickData = { _window, _cursorIndex, _buttonIndex, _renderTargetCoords, _inputSource };
                m_processClick = true;
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorMove(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2DF const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Press,
            _cursorIndex,
            Maze::Vec2DF::c_zero,
            _buttonIndex,
            _inputSource,
            _window);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedUIElements2D.rbegin(),
                                                        end = m_sortedUIElements2D.rend();
                                                        it != end;
                                                        ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (_window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            cursorInputEvent.canvas = canvasData.canvas;
            cursorInputEvent.rootCanvas = canvasData.rootCanvas;
            Vector<UIElement2D*> const& sortedUIElements2D = canvasData.sortedUIElements2D;

            if (cursorInputEvent.rootCanvas)
                cursorInputEvent.position = cursorInputEvent.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = cursorInputEvent.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElement2D*>::const_reverse_iterator it2 = sortedUIElements2D.rbegin(),
                                                              end2 = sortedUIElements2D.rend();
                                                              it2 != end2;
                                                              ++it2)
            {
                UIElement2D* element = *it2;

                if (!cursorInputEvent.hitCaptured)
                    element->processCursorMove(cursorInputEvent);
                else
                {
                    if (    (element->getFocused())
                        &&  (element->getCursorIndex() == _cursorIndex || element->getCursorIndex() == -1))
                        element->setFocused(false);
                }
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorDrag(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2DF const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Drag,
            _cursorIndex,
            Maze::Vec2DF::c_zero,
            _buttonIndex,
            _inputSource,
            _window);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedUIElements2D.rbegin(),
                                                        end = m_sortedUIElements2D.rend();
                                                        it != end;
                                                        ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (_window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            cursorInputEvent.canvas = canvasData.canvas;
            cursorInputEvent.rootCanvas = canvasData.rootCanvas;
            Vector<UIElement2D*> const& sortedUIElements2D = canvasData.sortedUIElements2D;

            if (cursorInputEvent.rootCanvas)
                cursorInputEvent.position = cursorInputEvent.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = cursorInputEvent.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElement2D*>::const_reverse_iterator it2 = sortedUIElements2D.rbegin(),
                                                              end2 = sortedUIElements2D.rend();
                                                              it2 != end2;
                                                              ++it2)
            {
                UIElement2D* element = *it2;

                if (!cursorInputEvent.hitCaptured)
                    element->processCursorDrag(cursorInputEvent);
                else
                {
                    if (   (element->getFocused())
                        && (element->getCursorIndex() == _cursorIndex || element->getCursorIndex() == -1))
                        element->setFocused(false);
                }
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorWheel(
        Window* _window,
        S32 _cursorIndex,
        F32 _deltaWheel,
        Vec2DF const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        updateSortedUIElements2DList();

        CursorWheelInputEvent cursorInputEvent(
            _cursorIndex,
            _deltaWheel,
            _window);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedUIElements2D.rbegin(),
                                                        end = m_sortedUIElements2D.rend();
                                                        it != end;
                                                        ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (_window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            cursorInputEvent.canvas = canvasData.canvas;
            cursorInputEvent.rootCanvas = canvasData.rootCanvas;
            Vector<UIElement2D*> const& sortedUIElements2D = canvasData.sortedUIElements2D;

            if (cursorInputEvent.rootCanvas)
                cursorInputEvent.position = cursorInputEvent.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = cursorInputEvent.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElement2D*>::const_reverse_iterator it2 = sortedUIElements2D.rbegin(),
                                                              end2 = sortedUIElements2D.rend();
                                                              it2 != end2;
                                                              ++it2)
            {
                UIElement2D* element = *it2;

                element->processCursorWheel(cursorInputEvent);

                if (cursorInputEvent.hitCaptured)
                    break;
            }

            if (cursorInputEvent.hitCaptured)
                break;
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::updateSortedUIElements2DList()
    {
        if (!m_sortedUIElements2DDirty)
            return;

        m_sortedUIElements2D.clear();

        for (Canvas* _canvas : m_sortedCanvases)
        {
            CanvasData canvasData;
            canvasData.canvas = _canvas;

            if (_canvas->getTransform()->getParent())
                canvasData.rootCanvas = _canvas->getTransform()->getRootTransform()->getEntityRaw()->getComponentRaw<Canvas>();
            else
                canvasData.rootCanvas = _canvas;

            std::function<void(Transform2D*)> processElementFunc = 
                [&](Transform2D* _transform)
                {
                    Entity* entity = _transform->getEntityRaw();

                    if (entity->getActiveInHierarchy())
                    {
                        if (_transform->getParent())
                        {
                            Canvas* canvas = entity->getComponentRaw<Canvas>();
                            if (canvas && canvas != _canvas)
                                return;
                        }

                        UIElement2D* element = entity->getComponentRaw<UIElement2D>();
                        if (element)
                        {
                            canvasData.sortedUIElements2D.emplace_back(element);
                        }

                        for (Transform2D* _child : _transform->getChildren())
                            processElementFunc(_child);
                    }
                };

            processElementFunc(_canvas->getTransform().get());

            m_sortedUIElements2D.emplace_back(canvasData);
        }

        m_sortedUIElements2DDirty = false;
    }
    
    
} // namespace Maze
//////////////////////////////////////////
