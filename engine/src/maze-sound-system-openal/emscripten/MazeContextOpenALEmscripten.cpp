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
// subject to the folloEmscripteng restrictions:
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
#include "MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/emscripten/MazeContextOpenALEmscripten.hpp"
#include "maze-sound-system-openal/MazeSoundOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundSystemOpenAL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ContextOpenALEmscripten
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ContextOpenALEmscripten, ContextOpenAL);

    //////////////////////////////////////////
    ContextOpenALEmscripten::ContextOpenALEmscripten()
    {
    }

    //////////////////////////////////////////
    ContextOpenALEmscripten::~ContextOpenALEmscripten()
    {

        if (m_soundSystemRaw)
            m_soundSystemRaw->eventFunctionsAssigned.unsubscribe(this);
    }

    //////////////////////////////////////////
    ContextOpenALEmscriptenPtr ContextOpenALEmscripten::Create(
        SoundSystemOpenALPtr const& _soundSystem,
        S32 _deviceIndex)
    {
        ContextOpenALEmscriptenPtr soundSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenALEmscripten, soundSystem, init(_soundSystem, _deviceIndex));
        return soundSystem;
    }

    //////////////////////////////////////////
    bool ContextOpenALEmscripten::init(
        SoundSystemOpenALPtr const& _soundSystem,
        S32 _deviceIndex)
    {
        if (!ContextOpenAL::init(_soundSystem, _deviceIndex))
            return false;
        
        setup();

        return true;
    }

    //////////////////////////////////////////
    void ContextOpenALEmscripten::setup()
    {
        if (m_deviceIndex < 0)
            return;

        createALContext();
    }

    //////////////////////////////////////////
    ContextOpenAL::ALFunctionPointer ContextOpenALEmscripten::getFunction(CString _name)
    {
        MAZE_ERROR("Not supported!");
        return nullptr;
    }

    //////////////////////////////////////////
    void ContextOpenALEmscripten::notifyFunctionsAssigned()
    {
        
    }

} // namespace Maze
//////////////////////////////////////////
