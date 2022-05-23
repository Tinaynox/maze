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
#pragma once
#if (!defined(_MazeInputSystem2D_hpp_))
#define _MazeInputSystem2D_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/ecs/MazeComponentSystem.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/ecs/MazeEntitiesSample.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-ui/ecs/components/MazeScrollRect2D.hpp"
#include "maze-ui/MazeCursorInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(EntitiesSample);
    MAZE_USING_SHARED_PTR(InputSystem2D);
    MAZE_USING_SHARED_PTR(Camera3D);
    MAZE_USING_SHARED_PTR(MeshRenderer);
    MAZE_USING_SHARED_PTR(Transform2D);
    MAZE_USING_SHARED_PTR(SystemTextEditBox2D);
    MAZE_USING_SHARED_PTR(SystemTextDropdown2D);
    MAZE_USING_SHARED_PTR(HorizontalLayout2D);
    MAZE_USING_SHARED_PTR(VerticalLayout2D);
    MAZE_USING_SHARED_PTR(SizePolicy2D);


    //////////////////////////////////////////
    // Class InputSystem2D
    //
    //////////////////////////////////////////
    class MAZE_UI_API InputSystem2D
        : public ComponentSystem
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(InputSystem2D, ComponentSystem);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(InputSystem2D);

        //////////////////////////////////////////
        friend class Entity;

    protected:

        //////////////////////////////////////////
        struct CanvasData
        {
            Canvas* canvas;
            Canvas* rootCanvas;
            Vector<UIElement2D*> sortedUIElements2D;
        };

        //////////////////////////////////////////
        struct ClickData
        {
            //////////////////////////////////////////
            ClickData(
                Window* _window = nullptr,
                S32 _cursorIndex = 0,
                S32 _buttonIndex = 0,
                Vec2DF _renderTargetCoords = Vec2DF::c_zero,
                CursorInputSource _inputSource = CursorInputSource())
                : window(_window)
                , cursorIndex(_cursorIndex)
                , buttonIndex(_buttonIndex)
                , renderTargetCoords(_renderTargetCoords)
                , inputSource(_inputSource)
            {}

            Window* window = nullptr;
            S32 cursorIndex = 0;
            S32 buttonIndex = 0;
            Vec2DF renderTargetCoords = Vec2DF::c_zero;
            CursorInputSource inputSource;
        };

        //////////////////////////////////////////
        static inline Vec2DF DefaultCoordsConverter(Vec2DF const& _coords) { return _coords; }

    public:

        //////////////////////////////////////////
        virtual ~InputSystem2D();

        //////////////////////////////////////////
        static InputSystem2DPtr Create();



        //////////////////////////////////////////
        virtual S32 getOrder() const MAZE_OVERRIDE { return 20000; }

        //////////////////////////////////////////
        inline void setCoordsConverter(std::function<Vec2DF(Vec2DF const&)> const& _coordsConverter) { m_coordsConverter = _coordsConverter; }

    protected:

        //////////////////////////////////////////
        InputSystem2D();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processSystemAdded() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processUpdate(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void processCanvasEntityAdded(Entity* _entity, Canvas* _canvas);

        //////////////////////////////////////////
        void processCanvasEntityRemoved(Entity* _entity, Canvas* _canvas);

        //////////////////////////////////////////
        void updateSortedCanvasesList();

        //////////////////////////////////////////
        void notifyMouse(Maze::InputEventMouseData const& _mouseData);

        //////////////////////////////////////////
        void notifyTouch(Maze::InputEventTouchData const& _touchData);

        //////////////////////////////////////////
        void processUIElement2DEntityAdded(Entity* _entity, UIElement2D* _UIElement2D);

        //////////////////////////////////////////
        void processUIElement2DEntityRemoved(Entity* _entity, UIElement2D* _UIElement2D);

        //////////////////////////////////////////
        void updateSortedUIElements2DList();
            
        //////////////////////////////////////////
        void processCursorPress(
            Window* _window,
            S32 _cursorIndex,
            S32 _buttonIndex,
            Vec2DF const& _renderTargetCoords,
            CursorInputSource const& _inputSource);

        //////////////////////////////////////////
        void processCursorClick(
            Window* _window,
            S32 _cursorIndex,
            S32 _buttonIndex,
            Vec2DF const& _renderTargetCoords,
            CursorInputSource const& _inputSource);

        //////////////////////////////////////////
        void processCursorDoubleClick(
            Window* _window,
            S32 _cursorIndex,
            S32 _buttonIndex,
            Vec2DF const& _renderTargetCoords,
            CursorInputSource const& _inputSource);

        //////////////////////////////////////////
        void processCursorRelease(
            Window* _window,
            S32 _cursorIndex,
            S32 _buttonIndex,
            Vec2DF const& _renderTargetCoords,
            CursorInputSource const& _inputSource);

        //////////////////////////////////////////
        void processCursorMove(
            Window* _window,
            S32 _cursorIndex,
            S32 _buttonIndex,
            Vec2DF const& _renderTargetCoords,
            CursorInputSource const& _inputSource);

        //////////////////////////////////////////
        void processCursorDrag(
            Window* _window,
            S32 _cursorIndex,
            S32 _buttonIndex,
            Vec2DF const& _renderTargetCoords,
            CursorInputSource const& _inputSource);

        //////////////////////////////////////////
        void processCursorWheel(
            Window* _window,
            S32 _cursorIndex,
            F32 _deltaWheel,
            Vec2DF const& _renderTargetCoords,
            CursorInputSource const& _inputSource);

    protected:
        SharedPtr<GenericInclusiveEntitiesSample<Canvas>> m_canvasesSample;
        SharedPtr<GenericInclusiveEntitiesSample<CanvasScaler>> m_canvasScalersSample;
        SharedPtr<GenericInclusiveEntitiesSample<UIElement2D>> m_UIElements2DSample;
        SharedPtr<GenericInclusiveEntitiesSample<SystemTextEditBox2D>> m_systemTextEditBoxesSample;
        SharedPtr<GenericInclusiveEntitiesSample<SystemTextDropdown2D>> m_systemTextDropdownsSample;
        SharedPtr<GenericInclusiveEntitiesSample<HorizontalLayout2D>> m_horizontalLayouts2D;
        SharedPtr<GenericInclusiveEntitiesSample<VerticalLayout2D>> m_verticalLayouts2D;
        SharedPtr<GenericInclusiveEntitiesSample<SizePolicy2D>> m_sizePolicy2D;
        SharedPtr<GenericInclusiveEntitiesSample<ScrollRect2D>> m_scrollRects2D;

        Vector<Canvas*> m_sortedCanvases;

        bool m_sortedUIElements2DDirty;
        Vector<CanvasData> m_sortedUIElements2D;

        std::function<Vec2DF(Vec2DF const&)> m_coordsConverter = DefaultCoordsConverter;

        Vec2DF m_firstCursorPressRenderTargetCoords = Vec2DF::c_zero;
        U32 m_firstCursorPressTime = 0u;
        U32 m_firstCursorReleaseTime = 0u;
        ClickData m_clickData;
        bool m_processClick = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInputSystem2D_hpp_
//////////////////////////////////////////
