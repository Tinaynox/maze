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
#if (!defined(_MazeCursorInputEvent_hpp_))
#define _MazeCursorInputEvent_hpp_


//////////////////////////////////////////
#include "maze-ui/MazeUIHeader.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/utils/MazeIndexedResource.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_UI_API, CursorInputSource,
        Mouse,
        Touch,
        VirtualCursor);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_7_API(MAZE_UI_API, CursorInputType,
        Press,
        Move,
        Drag,
        Release,
        Click,
        DoubleClick,
        Trace);


    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Canvas);
    MAZE_USING_SHARED_PTR(Window);


    //////////////////////////////////////////
    // Struct CursorInputEvent
    //
    //////////////////////////////////////////
    struct MAZE_UI_API CursorInputEvent
    {
        //////////////////////////////////////////
        inline CursorInputEvent(
            CursorInputType _type = CursorInputType::None,
            S32 _index = 0,
            Vec2F const& _position = Vec2F::c_zero,
            S32 _button = 0,
            CursorInputSource const& _inputSource = CursorInputSource::None,
            ResourceId _windowId = c_invalidResourceId)
            : type(_type)
            , index(_index)
            , position(_position)
            , capturerHandle(0)
            , button(_button)
            , inputSource(_inputSource)
            , windowId(_windowId)
        {}

        //////////////////////////////////////////
        inline void captureHit(S32 _capturerHandle) { capturerHandle = _capturerHandle; }

        //////////////////////////////////////////
        inline bool isCaptured() const { return capturerHandle != 0; }

        S32 type;
        S32 index;
        Vec2F position;
        S32 capturerHandle;
        S32 button;
        S32 inputSource;
        ResourceId windowId;
        ResourceId canvasId = c_invalidResourceId;
        ResourceId rootCanvasId = c_invalidResourceId;
    };


    //////////////////////////////////////////
    // Struct CursorWheelInputEvent
    //
    //////////////////////////////////////////
    struct MAZE_UI_API CursorWheelInputEvent
    {
        //////////////////////////////////////////
        inline CursorWheelInputEvent(
            S32 _index = 0,
            F32 _deltaWheel = 0.0f,
            ResourceId _windowId = c_invalidResourceId)
            : index(_index)
            , position(Vec2F::c_zero)
            , deltaWheel(_deltaWheel)
            , capturerHandle(0)
            , windowId(_windowId)
        {}

        //////////////////////////////////////////
        inline void captureHit(S32 _capturerHandle) { capturerHandle = _capturerHandle; }

        //////////////////////////////////////////
        inline bool isCaptured() const { return capturerHandle != 0; }

        S32 index;
        Vec2F position;
        F32 deltaWheel;
        S32 capturerHandle;
        ResourceId windowId;
        ResourceId canvasId = c_invalidResourceId;
        ResourceId rootCanvasId = c_invalidResourceId;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCursorInputEvent_hpp_
//////////////////////////////////////////
