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
#if (!defined(_MazeGamepadProviderLibstem_hpp_))
#define _MazeGamepadProviderLibstem_hpp_


//////////////////////////////////////////
#include "maze-gamepad/MazeGamepadHeader.hpp"
#include "maze-gamepad/providers/MazeGamepadProvider.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/math/MazeVec2D.hpp"


//////////////////////////////////////////
#if (MAZE_LIBSTEM_GAMEPAD_ENABLED)

//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(GamepadProviderLibstem);


    //////////////////////////////////////////
    // Class GamepadProviderLibstem
    //
    //////////////////////////////////////////
    class MAZE_GAMEPAD_API GamepadProviderLibstem
        : public GamepadProvider
    {
    public:

        //////////////////////////////////////////
        virtual ~GamepadProviderLibstem();

        //////////////////////////////////////////
        static GamepadProviderLibstemPtr Create();

    protected:

        //////////////////////////////////////////
        GamepadProviderLibstem();

        //////////////////////////////////////////
        virtual bool init() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void detectGamepads() MAZE_OVERRIDE;

        //////////////////////////////////////////
        void initLibstem();

        //////////////////////////////////////////
        void shutdownLibstem();

    protected:
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // MAZE_LIBSTEM_GAMEPAD_ENABLED
//////////////////////////////////////////


#endif // _MazeGamepadProviderLibstem_hpp_
//////////////////////////////////////////
