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
#include "maze-core/managers/emscripten/MazeSystemManagerEmscripten.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include <emscripten/emscripten.h>


//////////////////////////////////////////
static Maze::SystemManagerEmscripten* g_systemManagerEmscripten = nullptr;


//////////////////////////////////////////
void OnEmscriptenFrame()
{
    g_systemManagerEmscripten->processApplicationFrame();
}


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SystemManagerEmscripten
    //
    //////////////////////////////////////////
    SystemManagerEmscripten::SystemManagerEmscripten()
    {
        m_deviceCategory = DeviceCategory::WebBrowser;
        
        g_systemManagerEmscripten = this;
    }

    //////////////////////////////////////////
    SystemManagerEmscripten::~SystemManagerEmscripten()
    {
    }

    //////////////////////////////////////////
    bool SystemManagerEmscripten::initApplication()
    {
        processApplicationInit();
        
        emscripten_set_main_loop(
            OnEmscriptenFrame,
            0,
            1 );
            
        
        return true;
    }

    //////////////////////////////////////////
    bool SystemManagerEmscripten::init(Vector<Maze::S8 const*> const& _commandLineArguments)
    {
        if (!SystemManagerUnix::init(_commandLineArguments))
            return false;

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
