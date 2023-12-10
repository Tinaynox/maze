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
#if (!defined(_MazeGamepadProviderEmscripten_hpp_))
#define _MazeGamepadProviderEmscripten_hpp_


//////////////////////////////////////////
#include "maze-gamepad/MazeGamepadHeader.hpp"
#include "maze-gamepad/providers/MazeGamepadProvider.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2.hpp"

#include <emscripten/html5.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GamepadProviderEmscripten);
    
    
    //////////////////////////////////////////
    #define MAZE_EMSCRIPTEN_MAX_GAMEPADS_COUNT 32
    


    //////////////////////////////////////////
    // Class GamepadProviderEmscripten
    //
    //////////////////////////////////////////
    class MAZE_GAMEPAD_API GamepadProviderEmscripten
        : public GamepadProvider
    {
    public:

        //////////////////////////////////////////
        virtual ~GamepadProviderEmscripten();

        //////////////////////////////////////////
        static GamepadProviderEmscriptenPtr Create();


        //////////////////////////////////////////
        EM_BOOL processGamepadEvent(S32 _eventType, EmscriptenGamepadEvent const* _event);

    protected:

        //////////////////////////////////////////
        GamepadProviderEmscripten();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void detectGamepads() MAZE_OVERRIDE;


    protected:
        EmscriptenGamepadEvent m_cachedGamepadStates[MAZE_EMSCRIPTEN_MAX_GAMEPADS_COUNT];
    };
    

} // namespace Maze
//////////////////////////////////////////



#endif // _MazeGamepadProviderEmscripten_hpp_
//////////////////////////////////////////
