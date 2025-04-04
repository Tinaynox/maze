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
#include "maze-sound/MazeSoundSource.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-sound/loaders/MazeLoaderWAV.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SoundSource
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(SoundSource);

    //////////////////////////////////////////
    SoundSource* SoundSource::s_instancesList = nullptr;

    //////////////////////////////////////////
    SoundSource::SoundSource()
    {
        if (s_instancesList)
            s_instancesList->m_instancesListNext = this;
        m_instancesListPrev = s_instancesList;
        s_instancesList = this;
    }

    //////////////////////////////////////////
    SoundSource::~SoundSource()
    {
        if (m_soundGroup)
        {
            m_soundGroup->eventVolumeChanged.unsubscribe(this, &SoundSource::notifySoundGroupVolumeChanged);
            m_soundGroup->eventPitchChanged.unsubscribe(this, &SoundSource::notifySoundGroupPitchChanged);
            m_soundGroup.reset();
        }

        if (m_instancesListPrev)
            m_instancesListPrev->m_instancesListNext = m_instancesListNext;
        if (m_instancesListNext)
            m_instancesListNext->m_instancesListPrev = m_instancesListPrev;
        else
        if (s_instancesList == this)
            s_instancesList = m_instancesListPrev;
    }

    //////////////////////////////////////////
    bool SoundSource::init(SoundPtr const& _sound, SoundSystem* _soundSystem)
    {
        m_sound = _sound;
        m_soundSystem = _soundSystem;

        return true;
    }

    //////////////////////////////////////////
    void SoundSource::setSoundGroup(SoundGroupPtr const& _soundGroup)
    {
        if (m_soundGroup == _soundGroup)
            return;

        if (m_soundGroup)
        {
            m_soundGroup->eventVolumeChanged.unsubscribe(this, &SoundSource::notifySoundGroupVolumeChanged);
            m_soundGroup->eventPitchChanged.unsubscribe(this, &SoundSource::notifySoundGroupPitchChanged);
        }

        m_soundGroup = _soundGroup;

        if (m_soundGroup)
        {
            m_soundGroup->eventVolumeChanged.subscribe(this, &SoundSource::notifySoundGroupVolumeChanged);
            m_soundGroup->eventPitchChanged.subscribe(this, &SoundSource::notifySoundGroupPitchChanged);
        }

        updateVolume();
        updatePitch();
    }

    //////////////////////////////////////////
    void SoundSource::notifySoundGroupVolumeChanged(F32 _volume)
    {
        updateVolume();
    }

    //////////////////////////////////////////
    void SoundSource::notifySoundGroupPitchChanged(F32 _pitch)
    {
        updatePitch();
    }

    //////////////////////////////////////////
    void SoundSource::IterateSoundSources(std::function<bool(SoundSource*)> _cb)
    {
        SoundSource* instance = s_instancesList;
        while (instance)
        {
            if (!_cb(instance))
                break;

            instance = instance->m_instancesListPrev;
        }
    }

} // namespace Maze
//////////////////////////////////////////
