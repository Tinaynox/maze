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
#include "MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeContextOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundSystemOpenAL.hpp"
#include "maze-sound-system-openal/MazeFunctionsOpenAL.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class ContextOpenAL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(ContextOpenAL);

    //////////////////////////////////////////
    ContextOpenAL::ContextOpenAL()
    {
    }

    //////////////////////////////////////////
    ContextOpenAL::~ContextOpenAL()
    {
        destroyALContext();
    }

    //////////////////////////////////////////
    ContextOpenALPtr ContextOpenAL::Create(SoundSystemOpenALPtr const& _soundSystem, S32 _deviceIndex)
    {
        if (!_soundSystem)
            SoundSystem::GetCurrentInstancePtr()->castRaw<SoundSystemOpenAL>()->createContext(_deviceIndex);
        
        return _soundSystem->createContext(_deviceIndex);
    }

    //////////////////////////////////////////
    bool ContextOpenAL::init(SoundSystemOpenALPtr const& _soundSystem, S32 _deviceIndex)
    {
        m_soundSystemWeak = _soundSystem;
        m_soundSystemRaw = _soundSystem.get();
       
        m_deviceIndex = _deviceIndex;

        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenAL::makeCurrent()
    {
        if (!m_context)
            return false;

        MAZE_AL_CALL(mzalcMakeContextCurrent(m_context));
        return true;
    }

    //////////////////////////////////////////
    bool ContextOpenAL::createALContext()
    {
        destroyALContext();

        DeviceInfoOpenAL const* deviceInfo = m_soundSystemRaw->getDeviceInfo(m_deviceIndex);
        MAZE_ERROR_RETURN_VALUE_IF(!deviceInfo, false, "DeviceInfo is null!");

        MAZE_AL_CALL(m_device = mzalcOpenDevice(deviceInfo->deviceName.c_str()));
        MAZE_ERROR_RETURN_VALUE_IF(!m_device, false, "Failed to open ALC Device!");
        
        MAZE_AL_CALL(m_context = mzalcCreateContext(m_device, nullptr));
        MAZE_ERROR_RETURN_VALUE_IF(!m_context, false, "Failed to create ALC Context!");

        return true;
    }

    //////////////////////////////////////////
    void ContextOpenAL::destroyALContext()
    {
        if (m_context)
        {
            MAZE_AL_CALL(mzalcMakeContextCurrent(nullptr));
            MAZE_AL_CALL(mzalcDestroyContext(m_context));
            m_context = nullptr;
        }

        if (m_device)
        {
            MAZE_AL_CALL(mzalcCloseDevice(m_device));
            m_device = nullptr;
        }
    }

} // namespace Maze
//////////////////////////////////////////
