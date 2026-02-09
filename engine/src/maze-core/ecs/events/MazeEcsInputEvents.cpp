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
#include "MazeCoreHeader.hpp"
#include "maze-core/ecs/events/MazeEcsInputEvents.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InputMouseEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InputKeyboardEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(InputEventKeyboardType, type, InputEventKeyboardType::None, getType, setType),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S64, scanCode, 0, getScanCode, setScanCode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S64, virtualCode, 0, getVirtualCode, setVirtualCode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(KeyCode, keyCode, KeyCode::None, getKeyCode, setKeyCode),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(U32, modifiers, 0u, getModifiers, setModifiers),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S64, data, 0, getData, setData));
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InputTouchEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InputVirtualCursorEvent, Event);
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(InputCursorReleaseEvent, Event,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, cursorId, 0, getCursorId, setCursorId),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, buttonId, 0, getButtonId, setButtonId),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vec2S, position, Vec2S::c_zero, getPosition, setPosition),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, windowResourceId, c_invalidResourceId, getWindowId, setWindowId));

    
} // namespace Maze
//////////////////////////////////////////
