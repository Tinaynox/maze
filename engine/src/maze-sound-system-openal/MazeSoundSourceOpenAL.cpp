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
            bindSound(_sound->castRaw<SoundOpenAL>()->getBufferID());

        updateVolume();
        updateLooped();

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
        MAZE_AL_CALL(mzalSourcei(m_sourceID, AL_LOOPING, m_looped ? 1 : 0));
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

        MAZE_AL_CALL(mzalDeleteSources(1, &m_sourceID));
        m_sourceID = 0;
    }

    //////////////////////////////////////////
    void SoundSourceOpenAL::bindSound(MZALuint _bufferID)
    {
        MAZE_AL_CALL(mzalSourcei(m_sourceID, AL_BUFFER, _bufferID));
    }

} // namespace Maze
//////////////////////////////////////////
