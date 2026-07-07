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
#if (!defined(_MazeAudioSource_hpp_))
#define _MazeAudioSource_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/ecs/MazeComponent.hpp"
#include "maze-core/math/MazeVec3.hpp"
#include "maze-sound/MazeSound.hpp"
#include "maze-sound/MazeSoundSource.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(AudioSource);
    MAZE_USING_SHARED_PTR(Transform3D);


    //////////////////////////////////////////
    // Class AudioSource
    //
    // Unity-like audio source component. Supports both non-spatial (2D) and
    // spatial (3D positional) playback at once via spatialBlend (0 = 2D,
    // 1 = fully 3D) - the blend is computed entirely at this layer by
    // interpolating between "centered, no attenuation" and "true world
    // position, real rolloff" using SoundSource's thin OpenAL primitives.
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API AudioSource
        : public Component
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(AudioSource, Component);

        //////////////////////////////////////////
        MAZE_DECLARE_MEMORY_ALLOCATION(AudioSource);

        //////////////////////////////////////////
        friend class Entity;

    public:

        //////////////////////////////////////////
        virtual ~AudioSource();

        //////////////////////////////////////////
        static AudioSourcePtr Create();


        //////////////////////////////////////////
        inline SoundAssetRef const& getClipRef() const { return m_clip; }

        //////////////////////////////////////////
        void setClipRef(SoundAssetRef const& _clip);

        //////////////////////////////////////////
        inline SoundPtr const& getClip() const { return m_clip.getSound(); }

        //////////////////////////////////////////
        inline void setClip(SoundPtr const& _clip) { setClipRef(SoundAssetRef(_clip)); }


        //////////////////////////////////////////
        void setVolume(F32 _volume);

        //////////////////////////////////////////
        inline F32 getVolume() const { return m_volume; }


        //////////////////////////////////////////
        void setPitch(F32 _pitch);

        //////////////////////////////////////////
        inline F32 getPitch() const { return m_pitch; }


        //////////////////////////////////////////
        void setLoop(bool _loop);

        //////////////////////////////////////////
        inline bool getLoop() const { return m_loop; }


        //////////////////////////////////////////
        void setMute(bool _mute);

        //////////////////////////////////////////
        inline bool getMute() const { return m_mute; }


        //////////////////////////////////////////
        inline void setPlayOnAwake(bool _value) { m_playOnAwake = _value; }

        //////////////////////////////////////////
        inline bool getPlayOnAwake() const { return m_playOnAwake; }


        //////////////////////////////////////////
        // 0 = fully non-spatial (2D), 1 = fully 3D positional
        void setSpatialBlend(F32 _spatialBlend);

        //////////////////////////////////////////
        inline F32 getSpatialBlend() const { return m_spatialBlend; }


        //////////////////////////////////////////
        void setMinDistance(F32 _minDistance);

        //////////////////////////////////////////
        inline F32 getMinDistance() const { return m_minDistance; }


        //////////////////////////////////////////
        void setMaxDistance(F32 _maxDistance);

        //////////////////////////////////////////
        inline F32 getMaxDistance() const { return m_maxDistance; }


        //////////////////////////////////////////
        void setRolloffFactor(F32 _rolloffFactor);

        //////////////////////////////////////////
        inline F32 getRolloffFactor() const { return m_rolloffFactor; }


        //////////////////////////////////////////
        void play();

        //////////////////////////////////////////
        void stop();

        //////////////////////////////////////////
        bool isPlaying() const;


        //////////////////////////////////////////
        // Called once per frame by AudioSourceOnUpdate - pushes volume/pitch/loop
        // and (if spatialBlend > 0 and a Transform3D is present) world position/
        // velocity into the underlying SoundSource.
        void updateSpatial(F32 _dt);

    protected:

        //////////////////////////////////////////
        AudioSource();

        //////////////////////////////////////////
        using Component::init;

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        virtual void processEntityAwakened() MAZE_OVERRIDE;


        //////////////////////////////////////////
        void applyAllParamsToSource();

        //////////////////////////////////////////
        void resyncSpatial(Vec3F const& _worldPosition, Vec3F const& _worldVelocity);

    protected:
        SoundAssetRef m_clip;
        SoundSourcePtr m_soundSource;

        F32 m_volume = 1.0f;
        F32 m_pitch = 1.0f;
        bool m_loop = false;
        bool m_mute = false;
        bool m_playOnAwake = false;

        F32 m_spatialBlend = 0.0f;
        F32 m_minDistance = 1.0f;
        F32 m_maxDistance = 500.0f;
        F32 m_rolloffFactor = 1.0f;

        Transform3D* m_transform3D = nullptr;

        // Set whenever a spatial-affecting parameter changes (or on first frame),
        // so updateSpatial() knows to resync even when the transform itself hasn't
        // moved since the last frame.
        bool m_spatialDirty = true;
        Vec3F m_lastWorldPosition = Vec3F::c_zero;
        bool m_hasLastWorldPosition = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAudioSource_hpp_
//////////////////////////////////////////
