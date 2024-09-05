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
#if (!defined(_MazeInputManagerEmscripten_hpp_))
#define _MazeInputManagerEmscripten_hpp_


//////////////////////////////////////////
#include "maze-core/managers/MazeInputManager.hpp"
#include <emscripten/html5.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class InputManagerEmscripten
    //
    //////////////////////////////////////////
    class MAZE_CORE_API InputManagerEmscripten
        : public InputManager
    {
    private:

        //////////////////////////////////////////
        friend class InputManager;

    public:

        //////////////////////////////////////////
        virtual ~InputManagerEmscripten();

        //////////////////////////////////////////
        EM_BOOL processMouseEvent(S32 _eventType, EmscriptenMouseEvent const* _event, void* _userData);
        
        //////////////////////////////////////////
        EM_BOOL processKeyboardEvent(S32 _eventType, EmscriptenKeyboardEvent const* _event, void* _userData);
        
    protected:

        //////////////////////////////////////////
        InputManagerEmscripten();

        //////////////////////////////////////////
        virtual bool init(DataBlock const& _config) MAZE_OVERRIDE;
             
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInputManagerEmscripten_hpp_
//////////////////////////////////////////
