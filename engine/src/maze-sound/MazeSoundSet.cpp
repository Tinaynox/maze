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
#include "maze-sound/MazeSoundSet.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SoundSet
    //
    //////////////////////////////////////////
    SoundSet::SoundSet(
        FastVector<SoundPtr> const& _sounds,
        F32 _volume)
        : m_sounds(_sounds)
        , m_volume(_volume)
    {

    }

    //////////////////////////////////////////
    SoundSet::SoundSet(
        SoundPtr const& _sound,
        F32 _volume)
        : m_volume(_volume)
    {
        addSound(_sound);
    }

    //////////////////////////////////////////
    SoundSet::SoundSet(
        CString _soundName,
        F32 _volume)
        : m_volume(_volume)
    {
        SoundPtr const& sound = SoundManager::GetInstancePtr()->getSound(_soundName);
        addSound(sound);
    }

    //////////////////////////////////////////
    SoundSet::SoundSet(
        CString _soundName00,
        CString _soundName01,
        F32 _volume)
        : m_volume(_volume)
    {
        SoundPtr const& sound00 = SoundManager::GetInstancePtr()->getSound(_soundName00);
        addSound(sound00);

        SoundPtr const& sound01 = SoundManager::GetInstancePtr()->getSound(_soundName01);
        addSound(sound01);
    }

    //////////////////////////////////////////
    void SoundSet::addSound(SoundPtr const& _sound)
    {
        if (_sound)
            m_sounds.push_back(_sound);
    }

    //////////////////////////////////////////
    SoundPtr const& SoundSet::fetch()
    {
        static SoundPtr nullPointer;

        if (m_sounds.size() == 0)
            return nullPointer;

        if (m_sounds.size() == 1)
            return m_sounds[0];

        S32 fetchIndex = Math::RangeRandom(0, (S32)m_sounds.size());
        if (m_noRepeats && fetchIndex == m_prevFetchIndex)
            fetchIndex = (fetchIndex + 1) % (S32)m_sounds.size();

        m_prevFetchIndex = fetchIndex;

        return m_sounds[fetchIndex];
    }

} // namespace Maze
//////////////////////////////////////////
