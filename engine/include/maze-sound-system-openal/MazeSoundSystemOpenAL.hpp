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
#if (!defined(_MazeSoundSystemOpenAL_hpp_))
#define _MazeSoundSystemOpenAL_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeSoundSystemOpenALConfig.hpp"
#include "maze-sound-system-openal/MazeHeaderOpenAL.hpp"
#include "maze-sound-system-openal/MazeDeviceInfoOpenAL.hpp"
#include "maze-sound/MazeSoundSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundSystemOpenAL);
    MAZE_USING_SHARED_PTR(Sound);
    MAZE_USING_SHARED_PTR(SoundSourceOpenAL);
    MAZE_USING_SHARED_PTR(ContextOpenAL);


    //////////////////////////////////////////
    // Class SoundSystemOpenAL
    //
    //////////////////////////////////////////
    class MAZE_SOUND_SYSTEM_OPENAL_API SoundSystemOpenAL
        : public SoundSystem
        , public Updatable
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SoundSystemOpenAL, SoundSystem);

    public:

        //////////////////////////////////////////
        virtual ~SoundSystemOpenAL();

        //////////////////////////////////////////
        static SoundSystemOpenALPtr Create(
            SoundSystemOpenALConfig const& _config = SoundSystemOpenALConfig());


        //////////////////////////////////////////
        virtual void update(F32 _dt) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline ContextOpenALPtr const& getDummyContext() const { return m_dummyContext; }

        //////////////////////////////////////////
        inline ContextOpenALPtr const& getMainContext() const { return m_mainContext; }


        //////////////////////////////////////////
        virtual ContextOpenALPtr createContext(S32 _deviceIndex = -1);

        //////////////////////////////////////////
        virtual SoundPtr createSound() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual SoundGroupPtr createSoundGroup() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual SoundSourcePtr createSoundSource(SoundPtr const& _sound) MAZE_OVERRIDE;



        //////////////////////////////////////////
        inline bool getFunctionsAssigned() const { return m_functionsAssigned; }

        //////////////////////////////////////////
        Vector<DeviceInfoOpenAL> const& getDevicesInfo() const { return m_devicesInfo; }

        //////////////////////////////////////////
        DeviceInfoOpenAL const* getDeviceInfo(Size _index) const;

    public:

        //////////////////////////////////////////
        MultiDelegate<> eventFunctionsAssigned;
        MultiDelegate<> eventDevicesInfoLoaded;

    protected:

        //////////////////////////////////////////
        SoundSystemOpenAL();

        //////////////////////////////////////////
        virtual bool init(SoundSystemOpenALConfig const& _config);

        //////////////////////////////////////////
        bool setupSystem();

        //////////////////////////////////////////
        void assignFunctionsOpenAL(ContextOpenALPtr const& _soundContext);

        //////////////////////////////////////////
        void updateDevicesInfo();

        //////////////////////////////////////////
        U32 testMaxNumSources();

    protected:
        SoundSystemOpenALConfig m_config;

        ContextOpenALPtr m_dummyContext;
        ContextOpenALPtr m_mainContext;

        bool m_functionsAssigned = false;

        Vector<DeviceInfoOpenAL> m_devicesInfo;
        bool m_devicesInfoLoaded = false;
        S32 m_defaultDeviceIndex = 0;

        FastVector<SoundSourceOpenALPtr> m_soundSources;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSystemOpenAL_hpp_
//////////////////////////////////////////
