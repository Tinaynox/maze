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
#include "maze-core/math/MazeVec3.hpp"


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
        virtual void setPosition(Vec3F const& _position) { m_position = _position; }

        //////////////////////////////////////////
        inline Vec3F const& getPosition() const { return m_position; }


        //////////////////////////////////////////
        virtual void setVelocity(Vec3F const& _velocity) { m_velocity = _velocity; }

        //////////////////////////////////////////
        inline Vec3F const& getVelocity() const { return m_velocity; }


        //////////////////////////////////////////
        virtual void setSourceRelative(bool _sourceRelative) { m_sourceRelative = _sourceRelative; }

        //////////////////////////////////////////
        inline bool getSourceRelative() const { return m_sourceRelative; }


        //////////////////////////////////////////
        virtual void setMinDistance(F32 _minDistance) { m_minDistance = _minDistance; }

        //////////////////////////////////////////
        inline F32 getMinDistance() const { return m_minDistance; }


        //////////////////////////////////////////
        virtual void setMaxDistance(F32 _maxDistance) { m_maxDistance = _maxDistance; }

        //////////////////////////////////////////
        inline F32 getMaxDistance() const { return m_maxDistance; }


        //////////////////////////////////////////
        virtual void setRolloffFactor(F32 _rolloffFactor) { m_rolloffFactor = _rolloffFactor; }

        //////////////////////////////////////////
        inline F32 getRolloffFactor() const { return m_rolloffFactor; }


        //////////////////////////////////////////
        virtual void play() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void stop() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool isPlaying() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual bool rewindOffset(F32 _seconds) MAZE_ABSTRACT;

        //////////////////////////////////////////
        void setSoundGroup(SoundGroupPtr const& _soundGroup);

        //////////////////////////////////////////
        inline SoundPtr const& getSound() const { return m_sound; }

        //////////////////////////////////////////
        inline SoundGroupPtr const& getSoundGroup() const { return m_soundGroup; }

    public:

        //////////////////////////////////////////
        static void IterateSoundSources(std::function<bool(SoundSource*)> _cb);

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
        virtual void updatePosition() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateVelocity() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateSourceRelative() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateMinDistance() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateMaxDistance() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void updateRolloffFactor() MAZE_ABSTRACT;


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

        Vec3F m_position = Vec3F::c_zero;
        Vec3F m_velocity = Vec3F::c_zero;
        bool m_sourceRelative = false;
        F32 m_minDistance = 1.0f;
        F32 m_maxDistance = 1000000.0f;
        F32 m_rolloffFactor = 1.0f;


    protected:
        static SoundSource* s_instancesList;
        SoundSource* m_instancesListNext = nullptr;
        SoundSource* m_instancesListPrev = nullptr;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSource_hpp_
//////////////////////////////////////////
