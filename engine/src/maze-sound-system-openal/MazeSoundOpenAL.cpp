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
#include "maze-sound-system-openal/MazeSoundOpenAL.hpp"
#include "maze-sound-system-openal/MazeFunctionsOpenAL.hpp"
#include "maze-sound/MazeSoundSystem.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SoundOpenAL
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SoundOpenAL, Sound);

    //////////////////////////////////////////
    SoundOpenAL::SoundOpenAL()
    {
    }

    //////////////////////////////////////////
    SoundOpenAL::~SoundOpenAL()
    {
        deleteALObjects();
    }

    //////////////////////////////////////////
    SoundOpenALPtr SoundOpenAL::Create(SoundSystem* _soundSystem)
    {
        if (!_soundSystem)
            _soundSystem = SoundSystem::GetCurrentInstancePtr();

        SoundOpenALPtr sound;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundOpenAL, sound, init(_soundSystem));
        return sound;
    }


    //////////////////////////////////////////
    bool SoundOpenAL::init(SoundSystem* _soundSystem)
    {
        if (!Sound::init(_soundSystem))
            return false;
        
        generateALObjects();

        return true;
    }

    //////////////////////////////////////////
    bool SoundOpenAL::loadSound(SoundDataPtr const& _soundData)
    {
        S32 channels = _soundData->getChannels();
        S32 bitsPerSample = _soundData->getBitsPerSample();

        if (channels == 1)
        {
            switch (bitsPerSample)
            {
                case 4: MAZE_AL_CALL(m_bufferFormat = mzalGetEnumValue("AL_FORMAT_MONO_IMA4")); break;
                case 8: MAZE_AL_CALL(m_bufferFormat = mzalGetEnumValue("AL_FORMAT_MONO8")); break;
                case 16: MAZE_AL_CALL(m_bufferFormat = mzalGetEnumValue("AL_FORMAT_MONO16")); break;
                default: MAZE_ERROR("Sound '%s' - unknown format(c=%d, bps=%d)!", getName().c_str(), channels, bitsPerSample); return false;
            }
        }
        else
        if (channels == 2)
        {
            switch (bitsPerSample)
            {
                case 4: MAZE_AL_CALL(m_bufferFormat = mzalGetEnumValue("AL_FORMAT_STEREO_IMA4")); break;
                case 8: MAZE_AL_CALL(m_bufferFormat = mzalGetEnumValue("AL_FORMAT_STEREO8")); break;
                case 16: MAZE_AL_CALL(m_bufferFormat = mzalGetEnumValue("AL_FORMAT_STEREO16")); break;
                default: MAZE_ERROR("Sound '%s' - unknown format(c=%d, bps=%d)!", getName().c_str(), channels, bitsPerSample); return false;
            }
        }
        else
        if (channels == 4)
        {
            switch (bitsPerSample)
            {
                case 16: MAZE_AL_CALL(m_bufferFormat = mzalGetEnumValue("AL_FORMAT_QUAD16")); break;
                default: MAZE_ERROR("Sound '%s' - unknown format(c=%d, bps=%d)!", getName().c_str(), channels, bitsPerSample); return false;
            }
        }
        else
        {
            MAZE_ERROR("Sound '%s' - unknown format(c=%d, bps=%d)!", getName().c_str(), channels, bitsPerSample);
            return false;
        }

        MAZE_AL_CALL(
            mzalBufferData(
                m_bufferID,
                m_bufferFormat,
                _soundData->getData()->getData(),
                _soundData->getData()->getSize(),
                _soundData->getFrequency()));
        return true;
    }

    //////////////////////////////////////////
    void SoundOpenAL::generateALObjects()
    {
        if (m_bufferID != 0)
        {
            deleteALObjects();
        }

        MAZE_AL_CALL(mzalGenBuffers(1, &m_bufferID));
    }

    //////////////////////////////////////////
    void SoundOpenAL::deleteALObjects()
    {
        if (m_bufferID == 0)
            return;

        MAZE_AL_CALL(mzalDeleteBuffers(1, &m_bufferID));
        m_bufferID = 0;
    }

} // namespace Maze
//////////////////////////////////////////
