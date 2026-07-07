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
#include "maze-sound-system-openal/MazeSoundSourceOpenAL.hpp"
#include "maze-sound-system-openal/MazeFunctionsOpenAL.hpp"
#include "maze-sound-system-openal/MazeSoundOpenAL.hpp"
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SoundSourceOpenAL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SoundSourceOpenAL, SoundSource);

    //////////////////////////////////////////
    SoundSourceOpenAL::SoundSourceOpenAL()
    {
        
    }

    //////////////////////////////////////////
    SoundSourceOpenAL::~SoundSourceOpenAL()
    {
        deleteALObjects();
    }

    //////////////////////////////////////////
    SoundSourceOpenALPtr SoundSourceOpenAL::Create(SoundPtr const& _sound, SoundSystem* _soundSystem)
    {
        if (!_soundSystem)
            _soundSystem = SoundSystem::GetCurrentInstancePtr();

        SoundSourceOpenALPtr sound;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundSourceOpenAL, sound, init(_sound, _soundSystem));
        return sound;
    }

    //////////////////////////////////////////
    bool SoundSourceOpenAL::init(SoundPtr const& _sound, SoundSystem* _soundSystem)
    {
        if (!SoundSource::init(_sound, _soundSystem))
            return false;
        
        generateALObjects();

        if (_sound)
        {
            SoundOpenAL* soundAL = _sound->castRaw<SoundOpenAL>();
            if (soundAL->getStreamed())
                bindStream();
            else
                bindSound(soundAL->getBufferID());
        }

        updateVolume();
        updateLooped();
        updatePosition();
        updateVelocity();
        updateSourceRelative();
        updateMinDistance();
        updateMaxDistance();
        updateRolloffFactor();

        return true;
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::play()
    {
        MAZE_AL_CALL(mzalSourcePlay(m_sourceID));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::stop()
    {
        MAZE_AL_CALL(mzalSourceStop(m_sourceID));
        // setCycled(false);
    }

    //////////////////////////////////////////
    bool SoundSourceOpenAL::isPlaying()
    {
        MZALint state;
        MAZE_AL_CALL(mzalGetSourcei(m_sourceID, AL_SOURCE_STATE, &state));
        return (state != AL_STOPPED);
    }

    //////////////////////////////////////////
    bool SoundSourceOpenAL::rewindOffset(F32 _seconds)
    {
        if (!m_sound)
            return false;

        MAZE_AL_CALL(mzalSourcef(m_sourceID, AL_SEC_OFFSET, _seconds));
        return true;
    }

    //////////////////////////////////////////
    bool SoundSourceOpenAL::update(F32 _dt)
    {
        if (m_stream)
        {
            MZALint processed = 0;
            MAZE_AL_CALL(mzalGetSourcei(m_sourceID, AL_BUFFERS_PROCESSED, &processed));
            while (processed-- > 0)
            {
                MZALuint freedBuffer;
                MAZE_AL_CALL(mzalSourceUnqueueBuffers(m_sourceID, 1, &freedBuffer));
                fillAndQueueStreamBuffer(freedBuffer);
            }

            MZALint state;
            MAZE_AL_CALL(mzalGetSourcei(m_sourceID, AL_SOURCE_STATE, &state));
            if (state == AL_STOPPED)
            {
                if (m_streamEndReached)
                    return false;

                // AL can auto-stop if the refill lagged behind playback - if there is
                // still data queued, this isn't a real end of stream, just resume
                MZALint queued = 0;
                MAZE_AL_CALL(mzalGetSourcei(m_sourceID, AL_BUFFERS_QUEUED, &queued));
                if (queued > 0)
                    MAZE_AL_CALL(mzalSourcePlay(m_sourceID));
            }

            return true;
        }

        MZALint state;
        MAZE_AL_CALL(mzalGetSourcei(m_sourceID, AL_SOURCE_STATE, &state));
        if (state == AL_STOPPED)
            return false;

        return true;
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setLooped(bool _cycled)
    {
        SoundSource::setLooped(_cycled);
        updateLooped();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setVolume(F32 _volume)
    {
        SoundSource::setVolume(_volume);
        updateVolume();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setPitch(F32 _pitch)
    {
        SoundSource::setPitch(_pitch);
        updatePitch();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setPosition(Vec3F const& _position)
    {
        SoundSource::setPosition(_position);
        updatePosition();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setVelocity(Vec3F const& _velocity)
    {
        SoundSource::setVelocity(_velocity);
        updateVelocity();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setSourceRelative(bool _sourceRelative)
    {
        SoundSource::setSourceRelative(_sourceRelative);
        updateSourceRelative();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setMinDistance(F32 _minDistance)
    {
        SoundSource::setMinDistance(_minDistance);
        updateMinDistance();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setMaxDistance(F32 _maxDistance)
    {
        SoundSource::setMaxDistance(_maxDistance);
        updateMaxDistance();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::setRolloffFactor(F32 _rolloffFactor)
    {
        SoundSource::setRolloffFactor(_rolloffFactor);
        updateRolloffFactor();
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updateVolume()
    {
        F32 volume = m_volume;

        if (m_soundGroup)
            volume *= m_soundGroup->getVolume();

        MAZE_AL_CALL(mzalSourcef(m_sourceID, AL_GAIN, volume));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updatePitch()
    {
        F32 pitch = m_pitch;

        if (m_soundGroup)
            pitch *= m_soundGroup->getPitch();

        MAZE_AL_CALL(mzalSourcef(m_sourceID, AL_PITCH, pitch));

        MZALint state;
        MAZE_AL_CALL(mzalGetSourcei(m_sourceID, AL_SOURCE_STATE, &state));
        if (state != AL_STOPPED)
        {
            if (pitch == 0.0f)
            {
                MAZE_AL_CALL(mzalSourcePause(m_sourceID));
            }
            else
            {
                MAZE_AL_CALL(mzalSourcePlay(m_sourceID));
            }
        }
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updateLooped()
    {
        // For streamed sources, looping is handled by fillAndQueueStreamBuffer()
        // reseeking the decode cursor - AL_LOOPING only applies to a single
        // statically-bound buffer and is meaningless for queued streaming playback.
        if (m_stream)
            return;

        MAZE_AL_CALL(mzalSourcei(m_sourceID, AL_LOOPING, m_looped ? 1 : 0));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updatePosition()
    {
        MAZE_AL_CALL(mzalSource3f(m_sourceID, AL_POSITION, m_position.x, m_position.y, m_position.z));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updateVelocity()
    {
        MAZE_AL_CALL(mzalSource3f(m_sourceID, AL_VELOCITY, m_velocity.x, m_velocity.y, m_velocity.z));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updateSourceRelative()
    {
        MAZE_AL_CALL(mzalSourcei(m_sourceID, AL_SOURCE_RELATIVE, m_sourceRelative ? 1 : 0));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updateMinDistance()
    {
        MAZE_AL_CALL(mzalSourcef(m_sourceID, AL_REFERENCE_DISTANCE, m_minDistance));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updateMaxDistance()
    {
        MAZE_AL_CALL(mzalSourcef(m_sourceID, AL_MAX_DISTANCE, m_maxDistance));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::updateRolloffFactor()
    {
        MAZE_AL_CALL(mzalSourcef(m_sourceID, AL_ROLLOFF_FACTOR, m_rolloffFactor));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::generateALObjects()
    {
        if (m_sourceID != 0)
        {
            deleteALObjects();
        }

        MAZE_AL_CALL(mzalGenSources(1, &m_sourceID));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::deleteALObjects()
    {
        if (m_sourceID == 0)
            return;

        MAZE_AL_CALL(mzalSourceStop(m_sourceID));

        if (m_stream)
        {
            MZALint queued = 0;
            MAZE_AL_CALL(mzalGetSourcei(m_sourceID, AL_BUFFERS_QUEUED, &queued));
            while (queued-- > 0)
            {
                MZALuint freedBuffer;
                MAZE_AL_CALL(mzalSourceUnqueueBuffers(m_sourceID, 1, &freedBuffer));
            }

            MAZE_AL_CALL(mzalDeleteBuffers(c_streamBufferCount, m_streamBufferIds));
            m_stream.reset();
        }

        MAZE_AL_CALL(mzalSourcei(m_sourceID, AL_BUFFER, AL_NONE));
        MAZE_AL_CALL(mzalDeleteSources(1, &m_sourceID));
        m_sourceID = 0;
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::bindSound(MZALuint _bufferID)
    {
        MAZE_AL_CALL(mzalSourcei(m_sourceID, AL_BUFFER, _bufferID));
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::bindStream()
    {
        SoundOpenAL* soundAL = m_sound->castRaw<SoundOpenAL>();

        m_stream = SoundManager::GetInstancePtr()->openSoundStream(
            soundAL->getStreamFileData(),
            soundAL->getStreamExtension());
        MAZE_ERROR_RETURN_IF(!m_stream, "Failed to open sound stream for playback - '%s'!", m_sound->getName().c_str());

        m_streamBufferFormat = SoundOpenAL::GetALFormat(m_stream->getChannels(), m_stream->getBitsPerSample());
        m_streamScratch.resize((Size)(c_streamBufferSampleFrames * m_stream->getChannels()));

        MAZE_AL_CALL(mzalGenBuffers(c_streamBufferCount, m_streamBufferIds));

        m_streamEndReached = false;
        for (S32 i = 0; i < c_streamBufferCount; ++i)
            fillAndQueueStreamBuffer(m_streamBufferIds[i]);
    }

    //////////////////////////////////////////
    bool SoundSourceOpenAL::fillAndQueueStreamBuffer(MZALuint _bufferId)
    {
        Size framesGot = m_stream->getSamplesInterleaved(m_streamScratch.data(), (Size)c_streamBufferSampleFrames);
        if (framesGot == 0)
        {
            if (!m_looped)
            {
                m_streamEndReached = true;
                return false;
            }

            m_stream->seekStart();
            framesGot = m_stream->getSamplesInterleaved(m_streamScratch.data(), (Size)c_streamBufferSampleFrames);
            if (framesGot == 0)
            {
                m_streamEndReached = true;
                return false;
            }
        }

        MZALsizei byteSize = (MZALsizei)(framesGot * m_stream->getChannels() * sizeof(S16));
        MAZE_AL_CALL(mzalBufferData(_bufferId, m_streamBufferFormat, m_streamScratch.data(), byteSize, m_stream->getFrequency()));
        MAZE_AL_CALL(mzalSourceQueueBuffers(m_sourceID, 1, &_bufferId));
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
