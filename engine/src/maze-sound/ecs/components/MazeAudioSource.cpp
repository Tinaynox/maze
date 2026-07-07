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
#include "MazeSoundHeader.hpp"
#include "maze-sound/ecs/components/MazeAudioSource.hpp"
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/events/MazeEcsCoreEvents.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AudioSource
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AudioSource, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(SoundAssetRef, clip, SoundAssetRef(), getClipRef, setClipRef),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, volume, 1.0f, getVolume, setVolume),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, pitch, 1.0f, getPitch, setPitch),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, loop, false, getLoop, setLoop),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, mute, false, getMute, setMute),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, playOnAwake, false, getPlayOnAwake, setPlayOnAwake),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, spatialBlend, 0.0f, getSpatialBlend, setSpatialBlend),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, minDistance, 1.0f, getMinDistance, setMinDistance),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, maxDistance, 500.0f, getMaxDistance, setMaxDistance),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, rolloffFactor, 1.0f, getRolloffFactor, setRolloffFactor));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(AudioSource);

    //////////////////////////////////////////
    AudioSource::AudioSource()
    {
    }

    //////////////////////////////////////////
    AudioSource::~AudioSource()
    {
        if (m_soundSource)
            m_soundSource->stop();
    }

    //////////////////////////////////////////
    AudioSourcePtr AudioSource::Create()
    {
        AudioSourcePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(AudioSource, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool AudioSource::init()
    {
        return true;
    }

    //////////////////////////////////////////
    void AudioSource::processEntityAwakened()
    {
        m_transform3D = getEntityRaw()->getComponentRaw<Transform3D>();

        if (m_playOnAwake)
            play();
    }

    //////////////////////////////////////////
    void AudioSource::setClipRef(SoundAssetRef const& _clip)
    {
        if (m_clip == _clip)
            return;

        m_clip = _clip;

        // The current SoundSource is bound to the previous clip - SoundSource
        // has no rebind API, so drop it and let the next play() create a fresh
        // one bound to the new clip.
        if (m_soundSource)
        {
            m_soundSource->stop();
            m_soundSource.reset();
        }
    }

    //////////////////////////////////////////
    void AudioSource::setVolume(F32 _volume)
    {
        m_volume = _volume;
        if (m_soundSource)
            m_soundSource->setVolume(m_mute ? 0.0f : m_volume);
    }

    //////////////////////////////////////////
    void AudioSource::setPitch(F32 _pitch)
    {
        m_pitch = _pitch;
        if (m_soundSource)
            m_soundSource->setPitch(m_pitch);
    }

    //////////////////////////////////////////
    void AudioSource::setLoop(bool _loop)
    {
        m_loop = _loop;
        if (m_soundSource)
            m_soundSource->setLooped(m_loop);
    }

    //////////////////////////////////////////
    void AudioSource::setMute(bool _mute)
    {
        m_mute = _mute;
        if (m_soundSource)
            m_soundSource->setVolume(m_mute ? 0.0f : m_volume);
    }

    //////////////////////////////////////////
    void AudioSource::setSpatialBlend(F32 _spatialBlend)
    {
        m_spatialBlend = Math::Clamp01(_spatialBlend);
        m_spatialDirty = true;
    }

    //////////////////////////////////////////
    void AudioSource::setMinDistance(F32 _minDistance)
    {
        m_minDistance = _minDistance;
        m_spatialDirty = true;
    }

    //////////////////////////////////////////
    void AudioSource::setMaxDistance(F32 _maxDistance)
    {
        m_maxDistance = _maxDistance;
        m_spatialDirty = true;
    }

    //////////////////////////////////////////
    void AudioSource::setRolloffFactor(F32 _rolloffFactor)
    {
        m_rolloffFactor = _rolloffFactor;
        m_spatialDirty = true;
    }

    //////////////////////////////////////////
    void AudioSource::play()
    {
        SoundPtr const& clip = getClip();
        if (!clip)
            return;

        if (!m_soundSource)
        {
            m_soundSource = SoundSystem::GetCurrentInstancePtr()->createSoundSource(clip);
            applyAllParamsToSource();
        }

        m_soundSource->play();
    }

    //////////////////////////////////////////
    void AudioSource::stop()
    {
        if (m_soundSource)
            m_soundSource->stop();
    }

    //////////////////////////////////////////
    bool AudioSource::isPlaying() const
    {
        return m_soundSource && m_soundSource->isPlaying();
    }

    //////////////////////////////////////////
    void AudioSource::applyAllParamsToSource()
    {
        m_soundSource->setVolume(m_mute ? 0.0f : m_volume);
        m_soundSource->setPitch(m_pitch);
        m_soundSource->setLooped(m_loop);
        m_spatialDirty = true;
    }

    //////////////////////////////////////////
    void AudioSource::resyncSpatial(Vec3F const& _worldPosition, Vec3F const& _worldVelocity)
    {
        if (m_spatialBlend <= 0.0f || !m_transform3D)
        {
            m_soundSource->setSourceRelative(true);
            m_soundSource->setPosition(Vec3F::c_zero);
            m_soundSource->setVelocity(Vec3F::c_zero);
            m_soundSource->setRolloffFactor(0.0f);
            m_soundSource->setMinDistance(m_minDistance);
            m_soundSource->setMaxDistance(m_maxDistance);
            return;
        }

        m_soundSource->setSourceRelative(false);
        m_soundSource->setPosition(_worldPosition);
        m_soundSource->setVelocity(_worldVelocity);
        m_soundSource->setRolloffFactor(m_rolloffFactor * m_spatialBlend);
        m_soundSource->setMinDistance(m_minDistance);
        m_soundSource->setMaxDistance(m_maxDistance);
    }

    //////////////////////////////////////////
    void AudioSource::updateSpatial(F32 _dt)
    {
        if (!m_soundSource)
            return;

        bool transformChanged = m_transform3D && m_transform3D->isWorldTransformChanged();
        if (!m_spatialDirty && !transformChanged)
            return;

        Vec3F worldPosition = m_transform3D ? m_transform3D->getWorldPosition() : Vec3F::c_zero;
        Vec3F velocity = (m_hasLastWorldPosition && _dt > 1e-5f)
            ? (worldPosition - m_lastWorldPosition) / _dt
            : Vec3F::c_zero;

        resyncSpatial(worldPosition, velocity);

        m_lastWorldPosition = worldPosition;
        m_hasLastWorldPosition = true;
        m_spatialDirty = false;
    }


    //////////////////////////////////////////
    COMPONENT_SYSTEM_EVENT_HANDLER(AudioSourceOnUpdate,
        MAZE_ECS_TAGS(MAZE_HS("default")),
        {},
        UpdateEvent const& _event,
        Entity* _entity,
        AudioSource* _audioSource)
    {
        _audioSource->updateSpatial(_event.getDt());
    }

} // namespace Maze
//////////////////////////////////////////
