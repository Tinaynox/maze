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
#include "maze-ui/ecs/components/MazeInputSystem2D.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeSizePolicy2D.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeWindowManager.hpp"
#include "maze-ui/ecs/components/MazeEditBox2D.hpp"
#include "maze-ui/ecs/components/MazeDropdown2D.hpp"
#include "maze-ui/ecs/components/MazeHorizontalLayout2D.hpp"
#include "maze-ui/ecs/components/MazeVerticalLayout2D.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    U32 const c_doubleClickTimeMS = 350u;
    F32 const c_doubleClickShiftThresholdSq = 5.0f * 5.0f;
    U32 const c_clickTimeMS = 170u;
    F32 const c_clickShiftThresholdSq = 5.0f * 5.0f;
    Vec2F const c_outCanvasCursorPosition = Vec2F(-1e6f, -1e6f);


    //////////////////////////////////////////
    class InputSystem2DSimpleBoolMutex
    {
    public:
        //////////////////////////////////////////
        InputSystem2DSimpleBoolMutex(bool& _mutex)
            : m_mutex(_mutex)
        {
            MAZE_ERROR_IF(_mutex, "Already locked!");
            m_mutex = true;
        }

        //////////////////////////////////////////
        ~InputSystem2DSimpleBoolMutex()
        {
            m_mutex = false;
        }

    private:
        bool& m_mutex;
    };


#define INPUT_SYSTEM2D_ELEMENTS_LOCK() InputSystem2DSimpleBoolMutex elementsLock(m_sortedCanvasDataLocked)



    //////////////////////////////////////////
    void SetupCursorInputEventForCanvasData(
        CursorInputEvent& _event,
        InputSystem2D::CanvasData const& _canvasData,
        Vec2F const& _renderTargetCoords)
    {
        _event.canvasId = _canvasData.canvas ? _canvasData.canvas->getResourceId() : c_invalidResourceId;
        _event.rootCanvasId = _canvasData.rootCanvas ? _canvasData.rootCanvas->getResourceId() : c_invalidResourceId;

        //bool canvasContainsCursor = _canvasData.canvas->getRenderTargetAABB().contains(_renderTargetCoords);

        if (_canvasData.rootCanvas)
            _event.position = _canvasData.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
        else
            _event.position = _canvasData.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

    }


    //////////////////////////////////////////
    inline Vector<InputSystem2D::UIElementData>::const_iterator ProcessUIElementData(
        Vector<InputSystem2D::UIElementData> const& _sortedUIElements2D,
        Vector<InputSystem2D::UIElementData>::const_iterator const& _it,
        Vector<InputSystem2D::UIElementData>::const_iterator& _end,
        CursorInputEvent& _cursorInputEvent,
        std::function<void(UIElement2D*)> _cb)
    {
        InputSystem2D::UIElementData const& elementData = *_it;

        Vector<InputSystem2D::UIElementData>::const_iterator nextIt = _it + elementData.childrenCount + 1;

        if (elementData.scissorMask && !elementData.scissorMask->getScissorBounds().contains(_cursorInputEvent.position))
            return nextIt;

        // Process children first
        if (elementData.childrenCount > 0)
        {
            for (Vector<InputSystem2D::UIElementData>::const_iterator it2 = _it + 1,
                                                                      end2 = nextIt;
                                                                      it2 != end2;)
            {
                it2 = ProcessUIElementData(_sortedUIElements2D, it2, end2, _cursorInputEvent, _cb);
            }
        }

        if (_cursorInputEvent.isCaptured())
        {
            // Stop
            _end = nextIt;
            return nextIt;
        }
        
        _cb(elementData.element);

        if (_cursorInputEvent.isCaptured())
        {
            // Stop
            _end = nextIt;
            return nextIt;
        }

        return nextIt;
    }


    //////////////////////////////////////////
    inline void ProcessUIElementsInput(
        Vector<InputSystem2D::UIElementData> const& _sortedUIElements2D,
        CursorInputEvent& _cursorInputEvent,
        std::function<void(UIElement2D*)> _cb)
    {
        if (_cursorInputEvent.isCaptured())
            return;

        for (Vector<InputSystem2D::UIElementData>::const_iterator it = _sortedUIElements2D.begin(),
            end = _sortedUIElements2D.end();
            it != end;)
        {
            it = ProcessUIElementData(_sortedUIElements2D, it, end, _cursorInputEvent, _cb);
        }
    }

    //////////////////////////////////////////
    inline Vector<InputSystem2D::UIElementData>::const_iterator ProcessUIElementDataMoveEvent(
        Vector<InputSystem2D::UIElementData> const& _sortedUIElements2D,
        Vector<InputSystem2D::UIElementData>::const_iterator const& _it,
        Vector<InputSystem2D::UIElementData>::const_iterator& _end,
        CursorInputEvent& _cursorInputEvent,
        std::function<void(UIElement2D*)> _cbValid,
        std::function<void(UIElement2D*)> _cbInvalid)
    {
        InputSystem2D::UIElementData const& elementData = *_it;

        Vector<InputSystem2D::UIElementData>::const_iterator nextIt = _it + elementData.childrenCount + 1;

        bool isBoundsValid = !elementData.scissorMask || elementData.scissorMask->getScissorBounds().contains(_cursorInputEvent.position);

        // Process children first
        if (elementData.childrenCount > 0)
        {
            if (isBoundsValid)
            {
                for (Vector<InputSystem2D::UIElementData>::const_iterator it2 = _it + 1,
                    end2 = nextIt;
                    it2 != end2;)
                {
                    it2 = ProcessUIElementDataMoveEvent(_sortedUIElements2D, it2, end2, _cursorInputEvent, _cbValid, _cbInvalid);
                }
            }
            else
            {
                for (Vector<InputSystem2D::UIElementData>::const_iterator it2 = _it + 1,
                    end2 = nextIt;
                    it2 != end2;
                    ++it2)
                {
                    _cbInvalid(it2->element);
                }
            }
        }

        if (!_cursorInputEvent.isCaptured() && isBoundsValid)
            _cbValid(elementData.element);
        else
            _cbInvalid(elementData.element);



        return nextIt;
    }

    //////////////////////////////////////////
    inline void ProcessUIElementsMoveInput(
        Vector<InputSystem2D::UIElementData> const& _sortedUIElements2D,
        CursorInputEvent& _cursorInputEvent,
        std::function<void(UIElement2D*)> _cbValid,
        std::function<void(UIElement2D*)> _cbInvalid)
    {
        for (Vector<InputSystem2D::UIElementData>::const_iterator it = _sortedUIElements2D.begin(),
            end = _sortedUIElements2D.end();
            it != end;)
        {
            it = ProcessUIElementDataMoveEvent(_sortedUIElements2D, it, end, _cursorInputEvent, _cbValid, _cbInvalid);
        }
    }


    //////////////////////////////////////////
    inline Vector<InputSystem2D::UIElementData>::const_iterator TraceUIElementData(
        Vector<InputSystem2D::UIElementData> const& _sortedUIElements2D,
        Vector<InputSystem2D::UIElementData>::const_iterator const& _it,
        Vector<InputSystem2D::UIElementData>::const_iterator& _end,
        CursorInputEvent& _cursorInputEvent,
        UIElement2D const* _element,
        CursorElementTraceParams const& _traceParams,
        bool& _outResult)
    {
        InputSystem2D::UIElementData const& elementData = *_it;

        Vector<InputSystem2D::UIElementData>::const_iterator nextIt = _it + elementData.childrenCount + 1;

        if (_traceParams.ignoreElements.count(elementData.element->getEntityId()))
            return nextIt;

        if (elementData.scissorMask && !elementData.scissorMask->getScissorBounds().contains(_cursorInputEvent.position))
            return nextIt;

        // Process children first
        if (elementData.childrenCount > 0)
        {
            for (Vector<InputSystem2D::UIElementData>::const_iterator it2 = _it + 1,
                end2 = nextIt;
                it2 != end2;)
            {
                it2 = TraceUIElementData(_sortedUIElements2D, it2, end2, _cursorInputEvent, _element, _traceParams, _outResult);
                if (_outResult)
                    break;
            }
        }

        if (_outResult || _cursorInputEvent.isCaptured())
        {
            // Stop
            _end = nextIt;
            return nextIt;
        }

        bool traceResult = elementData.element->processCursorTrace(_cursorInputEvent);

        if (_element == elementData.element)
        {
            // Stop
            _outResult = traceResult;
            _end = nextIt;
            return nextIt;
        }

        if (_cursorInputEvent.isCaptured())
        {
            // Stop
            _end = nextIt;
            return nextIt;
        }

        return nextIt;
    }


    //////////////////////////////////////////
    inline bool TraceUIElement(
        Vector<InputSystem2D::UIElementData> const& _sortedUIElements2D,
        CursorInputEvent& _cursorInputEvent,
        UIElement2D const* _element,
        CursorElementTraceParams const& _traceParams)
    {
        if (_cursorInputEvent.isCaptured())
            return false;

        bool result = false;
        for (Vector<InputSystem2D::UIElementData>::const_iterator it = _sortedUIElements2D.begin(),
            end = _sortedUIElements2D.end();
            it != end;)
        {
            it = TraceUIElementData(_sortedUIElements2D, it, end, _cursorInputEvent, _element, _traceParams, result);
            if (result)
                break;
        }

        return result;
    }


    //////////////////////////////////////////
    // Class InputSystem2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InputSystem2D, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(InputSystem2D);

    //////////////////////////////////////////
    InputSystem2D::InputSystem2D()
        : m_sortedCanvasDataDirty(false)
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
        InputManager::GetInstancePtr()->eventVirtualCursor.unsubscribe(this);
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
        InputManager::GetInstancePtr()->eventVirtualCursor.subscribe(this, &InputSystem2D::notifyVirtualCursor);

        return true;
    }

    //////////////////////////////////////////
    void InputSystem2D::processEntityAwakened()
    {
        m_canvasesSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<Canvas>();
        m_canvasesSample->eventEntityAdded.subscribe(this, &InputSystem2D::processCanvasEntityAdded);
        m_canvasesSample->eventEntityRemoved.subscribe(this, &InputSystem2D::processCanvasEntityRemoved);
        dirtySortedCanvasData();

        m_canvasScalersSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<CanvasScaler>();


        m_UIElements2DSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<UIElement2D>();
        m_UIElements2DSample->eventEntityAdded.subscribe(this, &InputSystem2D::processUIElement2DEntityAdded);
        m_UIElements2DSample->eventEntityRemoved.subscribe(this, &InputSystem2D::processUIElement2DEntityRemoved);

        m_systemTextEditBoxesSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<EditBox2D>();
        m_systemTextDropdownsSample = getEntityRaw()->getEcsWorld()->requestInclusiveSample<Dropdown2D>();
        m_horizontalLayouts2D = getEntityRaw()->getEcsWorld()->requestInclusiveSample<HorizontalLayout2D>();
        m_verticalLayouts2D = getEntityRaw()->getEcsWorld()->requestInclusiveSample<VerticalLayout2D>();
        m_sizePolicy2D = getEntityRaw()->getEcsWorld()->requestInclusiveSample<SizePolicy2D, Transform2D>();
        m_scrollRects2D = getEntityRaw()->getEcsWorld()->requestInclusiveSample<ScrollRect2D>();
    }

    //////////////////////////////////////////
    void InputSystem2D::update(UpdateEvent const& _event)
    {
        MAZE_PROFILE_EVENT("InputSystem2D::update");

        F32 dt = _event.getDt();

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
        m_horizontalLayouts2D->query(
            [&](Entity* _entity, HorizontalLayout2D* _layout)
            {
                layouts.emplace_back(
                    Layout2DSortData
                    {
                        _layout,
                        _layout->getTransform()->getNestingLevel()
                    });
            });

        m_verticalLayouts2D->query(
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

        m_canvasScalersSample->query(
            [&](Entity* _entity, CanvasScaler* _canvasScaler)
            {
                _canvasScaler->updateCanvasScale();
            });

        m_sizePolicy2D->query(
            [](Entity* entity, SizePolicy2D* _sizePolicy, Transform2D* _transform)
            {
                if (_transform->isWorldTransformChanged())
                    _sizePolicy->updateSize();
            });

        for (Layout2DSortData const& layoutData : layouts)
            layoutData.layout->update();

        m_systemTextEditBoxesSample->query(
            [&](Entity* _entity, EditBox2D* _editBox)
            {
                _editBox->update(dt);
            });

        m_systemTextDropdownsSample->query(
            [&](Entity* _entity, Dropdown2D* _dropdown)
            {
                _dropdown->update(dt);
            });

        m_scrollRects2D->query(
            [&](Entity* _entity, ScrollRect2D* _scrollRect)
            {
                _scrollRect->update(dt);
            });
    }

    //////////////////////////////////////////
    bool InputSystem2D::traceElement(
        UIElement2D const* _element,
        Vec2F const& _renderTargetCoords,
        CursorElementTraceParams const& _traceParams)
    {
        Window* window = _traceParams.window;

        if (!window)
            window = WindowManager::GetInstancePtr()->getFirstOpenedWindow();

        if (!m_sortedCanvasDataLocked)
            updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Trace,
            _traceParams.cursorIndex,
            Maze::Vec2F::c_zero,
            _traceParams.buttonIndex,
            _traceParams.inputSource,
            window ? window->getResourceId() : c_invalidResourceId);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
                                                        end = m_sortedCanvasData.rend();
                                                        it != end;
                                                        ++it)
        {
            CanvasData const& canvasData = *it;

            if (canvasData.rootCanvas &&
                canvasData.rootCanvas->getRenderTarget() &&
                canvasData.rootCanvas->getRenderTarget()->getMetaClass()->isInheritedFrom<RenderWindow>())
            {
                if (window != canvasData.rootCanvas->getRenderTarget()->castRaw<RenderWindow>()->getWindowRaw())
                    continue;
            }

            Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
            SetupCursorInputEventForCanvasData(cursorInputEvent, canvasData, _renderTargetCoords);

            if (TraceUIElement(sortedUIElements2D, cursorInputEvent, _element, _traceParams))
                return true;

            if (cursorInputEvent.isCaptured())
                return false;
        }

        return false;
    }

    //////////////////////////////////////////
    void InputSystem2D::processCanvasEntityAdded(Entity* _entity)
    {
        updateSortedCanvasesList();
    }

    //////////////////////////////////////////
    void InputSystem2D::processCanvasEntityRemoved(Entity* _entity)
    {
        updateSortedCanvasesList();
    }

    //////////////////////////////////////////
    void InputSystem2D::processUIElement2DEntityAdded(Entity* _entity)
    {
        m_sortedCanvasDataDirty = true;
    }

    //////////////////////////////////////////
    void InputSystem2D::processUIElement2DEntityRemoved(Entity* _entity)
    {
        m_sortedCanvasDataDirty = true;
    }

    //////////////////////////////////////////
    void InputSystem2D::updateSortedCanvasesList()
    {
        m_sortedCanvases.clear();

        m_canvasesSample->query(
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

        m_sortedCanvasDataDirty = true;
    }

    //////////////////////////////////////////
    void InputSystem2D::notifyMouse(Maze::InputEventMouseData const& _mouseData)
    {
        Vec2F mousePosition = m_coordsConverter(_mouseData.window, Vec2F((F32)_mouseData.x, (F32)_mouseData.y));

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
        Vec2F touchPosition = m_coordsConverter(_touchData.window, Vec2F((F32)_touchData.x, (F32)_touchData.y));

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
                processCursorDrag(_touchData.window, _touchData.index, 0, touchPosition, CursorInputSource::Touch);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::notifyVirtualCursor(Maze::InputEventVirtualCursorData const& _virtualCursorData)
    {
        Vec2F mousePosition = m_coordsConverter(_virtualCursorData.window, Vec2F((F32)_virtualCursorData.x, (F32)_virtualCursorData.y));

        switch (_virtualCursorData.type)
        {
            case InputEventVirtualCursorType::Press:
            {
                processCursorPress(_virtualCursorData.window, 0, 0, mousePosition, CursorInputSource::VirtualCursor);
                break;
            }
            case InputEventVirtualCursorType::Release:
            {
                processCursorRelease(_virtualCursorData.window, 0, 0, mousePosition, CursorInputSource::VirtualCursor);
                break;
            }
            case InputEventVirtualCursorType::Move:
            {
                processCursorMove(_virtualCursorData.window, 0, 0, mousePosition, CursorInputSource::VirtualCursor);
                break;
            }
            case InputEventVirtualCursorType::Drag:
            {
                processCursorDrag(_virtualCursorData.window, 0, 0, mousePosition, CursorInputSource::VirtualCursor);
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
        Vec2F const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        {
            INPUT_SYSTEM2D_ELEMENTS_LOCK();
            updateSortedUIElements2DList();

            CursorInputEvent cursorInputEvent(
                CursorInputType::Press,
                _cursorIndex,
                Maze::Vec2F::c_zero,
                _buttonIndex,
                _inputSource,
                _window ? _window->getResourceId() : c_invalidResourceId);

            for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
                end = m_sortedCanvasData.rend();
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

                Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
                SetupCursorInputEventForCanvasData(cursorInputEvent, canvasData, _renderTargetCoords);

                ProcessUIElementsInput(
                    sortedUIElements2D,
                    cursorInputEvent,
                    [&](UIElement2D* _element)
                    {
                        _element->processCursorPress(cursorInputEvent);
                    });

                if (cursorInputEvent.isCaptured())
                    break;
            }
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
        Vec2F const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        INPUT_SYSTEM2D_ELEMENTS_LOCK();
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Click,
            _cursorIndex,
            Maze::Vec2F::c_zero,
            _buttonIndex,
            _inputSource,
            _window ? _window->getResourceId() : c_invalidResourceId);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
            end = m_sortedCanvasData.rend();
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

            Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
            SetupCursorInputEventForCanvasData(cursorInputEvent, canvasData, _renderTargetCoords);

            ProcessUIElementsInput(
                sortedUIElements2D,
                cursorInputEvent,
                [&](UIElement2D* _element)
                {
                    _element->processCursorClick(cursorInputEvent);
                });

            if (cursorInputEvent.isCaptured())
                break;
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorDoubleClick(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2F const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        INPUT_SYSTEM2D_ELEMENTS_LOCK();
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::DoubleClick,
            _cursorIndex,
            Maze::Vec2F::c_zero,
            _buttonIndex,
            _inputSource,
            _window ? _window->getResourceId() : c_invalidResourceId);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
            end = m_sortedCanvasData.rend();
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

            Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
            SetupCursorInputEventForCanvasData(cursorInputEvent, canvasData, _renderTargetCoords);

            ProcessUIElementsInput(
                sortedUIElements2D,
                cursorInputEvent,
                [&](UIElement2D* _element)
                {
                    _element->processCursorDoubleClick(cursorInputEvent);
                });

            if (cursorInputEvent.isCaptured())
                break;
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorRelease(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2F const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        INPUT_SYSTEM2D_ELEMENTS_LOCK();
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Press,
            _cursorIndex,
            Maze::Vec2F::c_zero,
            _buttonIndex,
            _inputSource,
            _window ? _window->getResourceId() : c_invalidResourceId);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
            end = m_sortedCanvasData.rend();
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

            Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
            SetupCursorInputEventForCanvasData(cursorInputEvent, canvasData, _renderTargetCoords);

            ProcessUIElementsMoveInput(
                sortedUIElements2D,
                cursorInputEvent,
                [&](UIElement2D* _element)
                {
                    _element->processCursorRelease(cursorInputEvent, false);

                    if (_inputSource == CursorInputSource::Touch)
                    {
                        if (_element->getFocused() && (_element->getCursorIndex() == _cursorIndex || _element->getCursorIndex() == -1))
                            _element->setFocused(false);
                    }
                },
                [&](UIElement2D* _element)
                {
                    _element->processCursorRelease(cursorInputEvent, true);

                    if (_inputSource == CursorInputSource::Touch)
                    {
                        if (_element->getFocused() && (_element->getCursorIndex() == _cursorIndex || _element->getCursorIndex() == -1))
                            _element->setFocused(false);
                    }
                });
        }

        if (_cursorIndex == 0 && _buttonIndex == 0)
        {
            U32 curTime = UpdateManager::GetInstancePtr()->getMilliseconds();
            if (curTime - m_firstCursorPressTime < c_clickTimeMS &&
                (_renderTargetCoords - m_firstCursorPressRenderTargetCoords).squaredLength() <= c_clickShiftThresholdSq)
            {
                m_clickData = ClickData(_window, _cursorIndex, _buttonIndex, _renderTargetCoords, _inputSource);
                m_processClick = true;
            }
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorMove(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2F const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        INPUT_SYSTEM2D_ELEMENTS_LOCK();
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Press,
            _cursorIndex,
            Maze::Vec2F::c_zero,
            _buttonIndex,
            _inputSource,
            _window ? _window->getResourceId() : c_invalidResourceId);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
                                                        end = m_sortedCanvasData.rend();
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

            Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
            SetupCursorInputEventForCanvasData(cursorInputEvent, canvasData, _renderTargetCoords);

            ProcessUIElementsMoveInput(
                sortedUIElements2D,
                cursorInputEvent,
                [&](UIElement2D* _element)
                {
                    _element->processCursorMove(cursorInputEvent);
                },
                [&](UIElement2D* _element)
                {
                    if (_element->getFocused() &&
                        (_element->getCursorIndex() == _cursorIndex || _element->getCursorIndex() == -1))
                        _element->setFocused(false);
                });

        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorDrag(
        Window* _window,
        S32 _cursorIndex,
        S32 _buttonIndex,
        Vec2F const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        INPUT_SYSTEM2D_ELEMENTS_LOCK();
        updateSortedUIElements2DList();

        CursorInputEvent cursorInputEvent(
            CursorInputType::Drag,
            _cursorIndex,
            Maze::Vec2F::c_zero,
            _buttonIndex,
            _inputSource,
            _window ? _window->getResourceId() : c_invalidResourceId);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
                                                        end = m_sortedCanvasData.rend();
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

            Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
            SetupCursorInputEventForCanvasData(cursorInputEvent, canvasData, _renderTargetCoords);

            ProcessUIElementsMoveInput(
                sortedUIElements2D,
                cursorInputEvent,
                [&](UIElement2D* _element)
                {
                    _element->processCursorDrag(cursorInputEvent);
                },
                [&](UIElement2D* _element)
                {
                    if (_element->getFocused() &&
                        (_element->getCursorIndex() == _cursorIndex || _element->getCursorIndex() == -1))
                        _element->setFocused(false);
                });
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::processCursorWheel(
        Window* _window,
        S32 _cursorIndex,
        F32 _deltaWheel,
        Vec2F const& _renderTargetCoords,
        CursorInputSource const& _inputSource)
    {
        INPUT_SYSTEM2D_ELEMENTS_LOCK();
        updateSortedUIElements2DList();

        CursorWheelInputEvent cursorInputEvent(
            _cursorIndex,
            _deltaWheel,
            _window ? _window->getResourceId() : c_invalidResourceId);

        for (Vector<CanvasData>::const_reverse_iterator it = m_sortedCanvasData.rbegin(),
                                                        end = m_sortedCanvasData.rend();
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

            Vector<UIElementData> const& sortedUIElements2D = canvasData.sortedUIElements2D;
            cursorInputEvent.canvasId = canvasData.canvas ? canvasData.canvas->getResourceId() : c_invalidResourceId;
            cursorInputEvent.rootCanvasId = canvasData.rootCanvas ? canvasData.rootCanvas->getResourceId() : c_invalidResourceId;

            //bool canvasContainsCursor = canvasData.canvas->getRenderTargetAABB().contains(_renderTargetCoords);

            if (canvasData.rootCanvas)
                cursorInputEvent.position = canvasData.rootCanvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);
            else
                cursorInputEvent.position = canvasData.canvas->convertRenderTargetCoordsToViewportCoords(_renderTargetCoords);

            for (Vector<UIElementData>::const_iterator it2 = sortedUIElements2D.begin(),
                                                             end2 = sortedUIElements2D.end();
                                                             it2 != end2;
                                                             ++it2)
            {
                UIElementData const& elementData = *it2;

                if (elementData.scissorMask && !elementData.scissorMask->getScissorBounds().contains(cursorInputEvent.position))
                {
                    // Skip children
                    it2 += elementData.childrenCount;
                    continue;
                }

                elementData.element->processCursorWheel(cursorInputEvent);

                if (cursorInputEvent.isCaptured())
                {
                    // Limit for children only
                    end2 = it2 + elementData.childrenCount + 1;
                }
            }

            if (cursorInputEvent.isCaptured())
                break;
        }
    }

    //////////////////////////////////////////
    void InputSystem2D::updateSortedUIElements2DList()
    {
        if (!m_sortedCanvasDataDirty)
            return;

        m_sortedCanvasData.clear();

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

                        S32 elementIndex = -1;

                        UIElement2D* element = entity->getComponentRaw<UIElement2D>();
                        if (element && element->getEntityRaw())
                        {
                            elementIndex = (S32)canvasData.sortedUIElements2D.size();

                            UIElementData data;
                            data.element = element;
                            data.scissorMask = entity->getComponentRaw<ScissorMask2D>();
                            canvasData.sortedUIElements2D.emplace_back(data);
                        }

                        for (auto it = _transform->getChildren().rbegin(),
                                  end = _transform->getChildren().rend();
                                  it != end;
                                  ++it)
                            processElementFunc(*it);

                        if (elementIndex != -1)
                        {
                            S32 currentElementSize = (S32)canvasData.sortedUIElements2D.size();
                            canvasData.sortedUIElements2D[elementIndex].childrenCount = Size(currentElementSize - elementIndex - 1);
                        }
                    }
                };

            processElementFunc(_canvas->getTransform().get());

            m_sortedCanvasData.emplace_back(canvasData);
        }

        m_sortedCanvasDataDirty = false;
    }
    

    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(InputSystem2DSystemOnAdded,
        {},
        {},
        EntityAddedToSampleEvent const& _event,
        Entity* _entity,
        InputSystem2D* _inputSystem2D)
    {
        _inputSystem2D->dirtySortedCanvasData();
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(InputSystem2DSystem,
        {},
        MAZE_ECS_ORDER(
            MAZE_ECS_ORDER_AFTER(),
            MAZE_ECS_ORDER_BEFORE(MAZE_HS("Bounds2DSystemUpdate"))),
        UpdateEvent const& _event,
        Entity* _entity,
        InputSystem2D* _inputSystem2D)
    {
        _inputSystem2D->update(_event);
    }
    
} // namespace Maze
//////////////////////////////////////////
