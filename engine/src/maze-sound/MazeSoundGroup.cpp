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
#include "maze-sound/MazeSoundGroup.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-sound/loaders/MazeLoaderWAV.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SoundGroup
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(SoundGroup);

    //////////////////////////////////////////
    SoundGroup::SoundGroup()
    {
    }

    //////////////////////////////////////////
    SoundGroup::~SoundGroup()
    {
        
    }

    //////////////////////////////////////////
    SoundGroupPtr SoundGroup::Create(SoundSystem* _soundSystem)
    {
        if (!_soundSystem)
            _soundSystem = SoundSystem::GetCurrentInstancePtr();
        
        if (!_soundSystem)
            return nullptr;

        return _soundSystem->createSoundGroup();
    }

    //////////////////////////////////////////
    bool SoundGroup::init(SoundSystem* _soundSystem)
    {
        m_soundSystem = _soundSystem;

        return true;
    }

    //////////////////////////////////////////
    void SoundGroup::setVolume(F32 _volume)
    {
        if (m_volume == _volume)
            return;

        m_volume = _volume;

        eventVolumeChanged(m_volume);
    }

    //////////////////////////////////////////
    void SoundGroup::setPitch(F32 _pitch)
    {
        if (m_pitch == _pitch)
            return;

        m_pitch = _pitch;

        eventPitchChanged(m_pitch);
    }

} // namespace Maze
//////////////////////////////////////////
