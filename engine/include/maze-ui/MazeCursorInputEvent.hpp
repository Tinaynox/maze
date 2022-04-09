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
#include "maze-core/math/MazeVec2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_UI_API, CursorInputSource,
        Mouse,
        Touch,
        Joystick);


    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_UI_API, CursorInputType,
        Press,
        Move,
        Drag,
        Release);


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
            CursorInputType _type,
            S32 _index = 0,
            Vec2DF const& _position = Vec2DF::c_zero,
            S32 _button = 0,
            CursorInputSource const& _inputSource = CursorInputSource::None,
            Window* _window = nullptr)
            : type(_type)
            , index(_index)
            , position(_position)
            , hitCaptured(false)
            , button(_button)
            , inputSource(_inputSource)
            , window(_window)
            , canvas(nullptr)
            , rootCanvas(nullptr)
        {}

        //////////////////////////////////////////
        inline void captureHit() const { hitCaptured = true; }

        CursorInputType type;
        S32 index;
        Vec2DF position;
        mutable bool hitCaptured;
        S32 button;
        CursorInputSource inputSource;
        Window* window;
        Canvas* canvas;
        Canvas* rootCanvas;
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
            Window* _window = nullptr)
            : index(_index)
            , position(Vec2DF::c_zero)
            , deltaWheel(_deltaWheel)
            , hitCaptured(false)
            , window(_window)
            , canvas(nullptr)
            , rootCanvas(nullptr)
        {}

        //////////////////////////////////////////
        inline void captureHit() const { hitCaptured = true; }

        S32 index;
        Vec2DF position;
        F32 deltaWheel;
        mutable bool hitCaptured;
        Window* window;
        Canvas* canvas;
        Canvas* rootCanvas;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeCursorInputEvent_hpp_
//////////////////////////////////////////
