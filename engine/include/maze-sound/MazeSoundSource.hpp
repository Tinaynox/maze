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
#if (!defined(_MazeSoundSource_hpp_))
#define _MazeSoundSource_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-sound/MazeSoundData.hpp"
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-sound/MazeSoundGroup.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundSource);


    //////////////////////////////////////////
    // Class SoundSource
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundSource
        : public SharedObject<SoundSource>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(SoundSource);

    public:

        //////////////////////////////////////////
        virtual ~SoundSource();


        //////////////////////////////////////////
        virtual void setVolume(F32 _volume) { m_volume = _volume; }

        //////////////////////////////////////////
        inline F32 getVolume() const { return m_volume; }


        //////////////////////////////////////////
        virtual void setPitch(F32 _pitch) { m_pitch = _pitch; }

        //////////////////////////////////////////
        inline F32 getPitch() const { return m_pitch; }


        //////////////////////////////////////////
        virtual void setLooped(bool _looped) { m_looped = _looped; }

        //////////////////////////////////////////
        inline bool getLooped() const { return m_looped; }


        //////////////////////////////////////////
        virtual void play() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void stop() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isPlaying() MAZE_ABSTRACT;

        //////////////////////////////////////////
        void setSoundGroup(SoundGroupPtr const& _soundGroup);

        //////////////////////////////////////////
        inline SoundPtr const& getSound() const { return m_sound; }

        //////////////////////////////////////////
        inline SoundGroupPtr const& getSoundGroup() const { return m_soundGroup; }


    protected:

        //////////////////////////////////////////
        SoundSource();

        //////////////////////////////////////////
        bool init(SoundPtr const& _sound, SoundSystem* _soundSystem);

        //////////////////////////////////////////
        virtual void updateVolume() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updatePitch() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateLooped() MAZE_ABSTRACT;


        //////////////////////////////////////////
        void notifySoundGroupVolumeChanged(F32 _volume);

        //////////////////////////////////////////
        void notifySoundGroupPitchChanged(F32 _pitch);

    protected:
        SoundSystem* m_soundSystem = nullptr;
        SoundPtr m_sound;
        SoundGroupPtr m_soundGroup;

        F32 m_volume = 1.0f;
        F32 m_pitch = 1.0f;
        bool m_looped = false;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSource_hpp_
//////////////////////////////////////////
