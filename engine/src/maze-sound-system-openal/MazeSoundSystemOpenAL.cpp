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
#include "maze-sound-system-openal/MazeSoundSystemOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundGroupOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundSourceOpenAL.hpp"
#include "maze-sound-system-openal/MazeFunctionsOpenAL.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"

#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
#   include "maze-sound-system-openal/win/MazeContextOpenALWin.hpp"
#endif


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SoundSystemOpenAL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SoundSystemOpenAL, SoundSystem);

    //////////////////////////////////////////
    SoundSystemOpenAL::SoundSystemOpenAL()
    {
    }

    //////////////////////////////////////////
    SoundSystemOpenAL::~SoundSystemOpenAL()
    {
    }

    //////////////////////////////////////////
    SoundSystemOpenALPtr SoundSystemOpenAL::Create(SoundSystemOpenALConfig const& _config)
    {
        SoundSystemOpenALPtr soundSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundSystemOpenAL, soundSystem, init(_config));
        return soundSystem;
    }

    //////////////////////////////////////////
    bool SoundSystemOpenAL::init(SoundSystemOpenALConfig const& _config)
    {
        m_config = _config;

        if (!setupSystem())
            return false;

        UpdateManager::GetInstancePtr()->addUpdatable(this);
        
        return true;
    }

    //////////////////////////////////////////
    void SoundSystemOpenAL::update(F32 _dt)
    {
        Stack<Size> deadSourceIds;
        for (Size i = 0, in = m_soundSources.size(); i != in; ++i)
        {
            if (!m_soundSources[i]->update(_dt))
                deadSourceIds.push(i);
        }

        while (!deadSourceIds.empty())
        {
            m_soundSources.eraseUnordered(m_soundSources.begin() + deadSourceIds.top());
            deadSourceIds.pop();
        }
    }

    //////////////////////////////////////////
    String const& SoundSystemOpenAL::getName()
    {
        static String s_name = "Sound System OpenAL";
        return s_name;
    }

    //////////////////////////////////////////
    ContextOpenALPtr SoundSystemOpenAL::createContext(S32 _deviceIndex)
    {
#if MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS
        return ContextOpenALWin::Create(cast<SoundSystemOpenAL>(), _deviceIndex);
#else
        MAZE_NOT_IMPLEMENTED;
        return ContextOpenALPtr();
#endif
    }

    //////////////////////////////////////////
    SoundPtr SoundSystemOpenAL::createSound()
    {
        return SoundOpenAL::Create();
    }

    //////////////////////////////////////////
    SoundGroupPtr SoundSystemOpenAL::createSoundGroup()
    {
        return SoundGroupOpenAL::Create();
    }

    //////////////////////////////////////////
    SoundSourcePtr SoundSystemOpenAL::createSoundSource(SoundPtr const& _sound)
    {
        SoundSourceOpenALPtr soundSource = SoundSourceOpenAL::Create(_sound);
        m_soundSources.push_back(soundSource);
        return soundSource;
    }

    //////////////////////////////////////////
    bool SoundSystemOpenAL::setupSystem()
    {
        m_dummyContext = createContext();
        if (!m_dummyContext)
            return false;

        assignFunctionsOpenAL(m_dummyContext);

        updateDevicesInfo();

        MAZE_ERROR_RETURN_VALUE_IF(m_devicesInfo.empty(), false, "There are no available audio devices!");

        m_mainContext = createContext(m_defaultDeviceIndex);
        if (!m_mainContext)
            return false;

        if (!m_mainContext->makeCurrent())
            return false;

        return true;
    }

    //////////////////////////////////////////
    void SoundSystemOpenAL::assignFunctionsOpenAL(ContextOpenALPtr const& _soundContext)
    {
        MAZE_ERROR_RETURN_IF(m_functionsAssigned, "Functions already assigned!");

        AssignFunctionsOpenAL(_soundContext);

        m_functionsAssigned = true;
        eventFunctionsAssigned();
    }

    //////////////////////////////////////////
    DeviceInfoOpenAL const* SoundSystemOpenAL::getDeviceInfo(Size _index) const
    {
        if (_index >= m_devicesInfo.size())
            return nullptr;

        return &m_devicesInfo[_index];
    }

    //////////////////////////////////////////
    void SoundSystemOpenAL::updateDevicesInfo()
    {
        bool isEnumeration;
        MAZE_AL_CALL(isEnumeration = mzalcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"));
        if (!isEnumeration)
            return;

        m_devicesInfo.clear();
        m_devicesInfo.reserve(10);

        Char* devices;
        MAZE_AL_CALL(devices = (Char*)mzalcGetString(nullptr, ALC_DEVICE_SPECIFIER));

        CString defaultDeviceName;
        MAZE_AL_CALL(defaultDeviceName = (CString)mzalcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER));

        S32 index = 0;
        S32 defaultDeviceIndex = 0;

        // Go through device list (each device terminated with a single NULL, list terminated with double NULL)
        while (*devices != NULL) 
        {
            if (strcmp(defaultDeviceName, devices) == 0)
                defaultDeviceIndex = index;

            ALCdevice* device;
            MAZE_AL_CALL(device = mzalcOpenDevice(devices));

            if (device) 
            {
                ALCcontext* context;
                MAZE_AL_CALL(context = mzalcCreateContext(device, nullptr));

                if (context) 
                {
                    MAZE_AL_CALL(mzalcMakeContextCurrent(context));

                    // If new actual device name isn't already in the list, then add it...
                    CString actualDeviceName;
                    MAZE_AL_CALL(actualDeviceName = mzalcGetString(device, ALC_DEVICE_SPECIFIER));

                    bool newName = true;
                    for (Size i = 0, in = m_devicesInfo.size(); i < in; i++) 
                    {
                        if (strcmp(m_devicesInfo[i].deviceName.c_str(), actualDeviceName) == 0)
                        {
                            newName = false;
                            break;
                        }
                    }

                    if ((newName) && (actualDeviceName != nullptr) && (strlen(actualDeviceName) > 0))
                    {
                        DeviceInfoOpenAL deviceInfo;
                        deviceInfo.selected = true;
                        deviceInfo.deviceName = actualDeviceName;
                        MAZE_AL_CALL(alcGetIntegerv(device, ALC_MAJOR_VERSION, sizeof(S32), &deviceInfo.majorVersion));
                        MAZE_AL_CALL(alcGetIntegerv(device, ALC_MINOR_VERSION, sizeof(S32), &deviceInfo.minorVersion));

                        // Check for ALC Extensions
                        if (alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_TRUE)
                            deviceInfo.extensions.insert("ALC_EXT_CAPTURE");
                        if (alcIsExtensionPresent(device, "ALC_EXT_EFX") == AL_TRUE)
                            deviceInfo.extensions.insert("ALC_EXT_EFX");

                        // Check for AL Extensions
                        if (alIsExtensionPresent("AL_EXT_OFFSET") == AL_TRUE)
                            deviceInfo.extensions.insert("AL_EXT_OFFSET");

                        if (alIsExtensionPresent("AL_EXT_LINEAR_DISTANCE") == AL_TRUE)
                            deviceInfo.extensions.insert("AL_EXT_LINEAR_DISTANCE");
                        if (alIsExtensionPresent("AL_EXT_EXPONENT_DISTANCE") == AL_TRUE)
                            deviceInfo.extensions.insert("AL_EXT_EXPONENT_DISTANCE");

                        if (alIsExtensionPresent("EAX2.0") == AL_TRUE)
                            deviceInfo.extensions.insert("EAX2.0");
                        if (alIsExtensionPresent("EAX3.0") == AL_TRUE)
                            deviceInfo.extensions.insert("EAX3.0");
                        if (alIsExtensionPresent("EAX4.0") == AL_TRUE)
                            deviceInfo.extensions.insert("EAX4.0");
                        if (alIsExtensionPresent("EAX5.0") == AL_TRUE)
                            deviceInfo.extensions.insert("EAX5.0");

                        if (alIsExtensionPresent("EAX-RAM") == AL_TRUE)
                            deviceInfo.extensions.insert("EAX-RAM");

                        // Get Source Count
                        deviceInfo.sourceCount = testMaxNumSources();

                        m_devicesInfo.push_back(deviceInfo);
                    }

                    MAZE_AL_CALL(mzalcMakeContextCurrent(nullptr));
                    MAZE_AL_CALL(mzalcDestroyContext(context));
                }

                MAZE_AL_CALL(mzalcCloseDevice(device));
            }

            devices += strlen(devices) + 1;
            index += 1;
        }

        m_devicesInfoLoaded = true;
        m_defaultDeviceIndex = defaultDeviceIndex;

        eventDevicesInfoLoaded();
    }

    //////////////////////////////////////////
    U32 SoundSystemOpenAL::testMaxNumSources()
    {
        MZALuint sources[256];
        U32 sourceCount = 0;

        // Clear AL Error Code
        mzalGetError();

        // Generate up to 256 Sources, checking for any errors
        for (sourceCount = 0; sourceCount < 256u; sourceCount++)
        {
            mzalGenSources(1, &sources[sourceCount]);
            if (mzalGetError() != AL_NO_ERROR)
                break;
        }

        // Release the Sources
        mzalDeleteSources(sourceCount, sources);
        if (mzalGetError() != AL_NO_ERROR)
        {
            for (U32 i = 0; i < 256u; i++)
            {
                MAZE_AL_CALL(mzalDeleteSources(1, &sources[i]));
            }
        }

        return sourceCount;
    }

} // namespace Maze
//////////////////////////////////////////
