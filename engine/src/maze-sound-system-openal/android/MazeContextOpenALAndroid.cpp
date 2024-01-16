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
// subject to the folloAndroidg restrictions:
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
#include "maze-sound-system-openal/android/MazeContextOpenALAndroid.hpp"
#include "maze-sound-system-openal/MazeSoundOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundSystemOpenAL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ContextOpenALAndroid
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ContextOpenALAndroid, ContextOpenAL);

    //////////////////////////////////////////
    ContextOpenALAndroid::ContextOpenALAndroid()
    {
    }

    //////////////////////////////////////////
    ContextOpenALAndroid::~ContextOpenALAndroid()
    {

        if (m_soundSystemRaw)
            m_soundSystemRaw->eventFunctionsAssigned.unsubscribe(this);
    }

    //////////////////////////////////////////
    ContextOpenALAndroidPtr ContextOpenALAndroid::Create(
        SoundSystemOpenALPtr const& _soundSystem,
        S32 _deviceIndex)
    {
        ContextOpenALAndroidPtr soundSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ContextOpenALAndroid, soundSystem, init(_soundSystem, _deviceIndex));
        return soundSystem;
    }

    //////////////////////////////////////////
    bool ContextOpenALAndroid::init(
        SoundSystemOpenALPtr const& _soundSystem,
        S32 _deviceIndex)
    {
        if (!ContextOpenAL::init(_soundSystem, _deviceIndex))
            return false;
        
        setup();

        return true;
    }

    //////////////////////////////////////////
    void ContextOpenALAndroid::setup()
    {
        if (m_deviceIndex < 0)
            return;

        createALContext();
    }

    //////////////////////////////////////////
    ContextOpenAL::ALFunctionPointer ContextOpenALAndroid::getFunction(CString _name)
    {
        MAZE_ERROR("Not supported!");
        return nullptr;
    }

    //////////////////////////////////////////
    void ContextOpenALAndroid::notifyFunctionsAssigned()
    {
        
    }

} // namespace Maze
//////////////////////////////////////////
