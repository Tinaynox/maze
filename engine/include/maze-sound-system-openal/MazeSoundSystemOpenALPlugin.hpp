//////////////////////////////////////////
//
// Maze SoundSystemAL
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
#if (!defined(_MazeSoundSystemOpenALPlugin_hpp_))
#define _MazeSoundSystemOpenALPlugin_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-core/system/MazePlugin.hpp"
#include "maze-sound-system-openal/MazeSoundSystemOpenAL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundSystemOpenALPlugin);
    MAZE_USING_SHARED_PTR(SoundSystemOpenAL);


#if (MAZE_STATIC)

    //////////////////////////////////////////
    void InstallSoundSystemOpenALPlugin(SoundSystemOpenALConfig const& _config = SoundSystemOpenALConfig());

    //////////////////////////////////////////
    void UninstallSoundSystemOpenALPlugin();

#endif


    //////////////////////////////////////////
    // Class SoundSystemOpenALPlugin
    //
    //////////////////////////////////////////
    class MAZE_SOUND_SYSTEM_OPENAL_API SoundSystemOpenALPlugin
        : public Plugin
        , public std::enable_shared_from_this<SoundSystemOpenALPlugin>
    {
    public:

        //////////////////////////////////////////
        virtual ~SoundSystemOpenALPlugin();

        //////////////////////////////////////////
        static SoundSystemOpenALPluginPtr Create(SoundSystemOpenALConfig const& _config = SoundSystemOpenALConfig());

        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void install() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void uninstall() MAZE_OVERRIDE;


        //////////////////////////////////////////
        SoundSystemOpenALConfig const& getConfig() const { return m_config; }

        //////////////////////////////////////////
        void setConfig(SoundSystemOpenALConfig const& _config) { m_config = _config; }

    protected:

        //////////////////////////////////////////
        SoundSystemOpenALPlugin();

        //////////////////////////////////////////
        bool init(SoundSystemOpenALConfig const& _config);

    protected:
        SoundSystemOpenALWPtr m_soundSystem;

        SoundSystemOpenALConfig m_config;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSystemOpenALPlugin_hpp_
//////////////////////////////////////////
