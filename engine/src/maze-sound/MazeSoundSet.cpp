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
    MAZE_IMPLEMENT_METACLASS(SoundSet,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<SoundPtr>, sounds, Vector<SoundPtr>(), getSounds, setSounds),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(F32, volume, 1.0, getVolume, setVolume),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(S32, prevFetchIndex, -1, getPrevFetchIndex, setPrevFetchIndex),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(bool, noRepeats, false, getNoRepeats, setNoRepeats));

    //////////////////////////////////////////
    SoundSet::SoundSet(
        Vector<SoundPtr> const& _sounds,
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

    //////////////////////////////////////////
    String SoundSet::toString() const
    {
        String str;
        ToString(this, str);
        return str;
    }

    //////////////////////////////////////////
    void SoundSet::setString(CString _data, Size _count)
    {
        loadFromJSONValue(JSONHelper::FromString(String(_data, _data + _count)));
    }

    //////////////////////////////////////////
    void SoundSet::FromString(SoundSetPtr& _value, CString _data, Size _count)
    {
        if (!_data || strcmp(_data, "") == 0)
        {
            _value.reset();
            return;
        }

        if (_count == 0)
            _count = strlen(_data);

        if (StringHelper::IsStartsWith(_data, "ptr:"))
        {
            String data = String(_data + 4, _data + _count);
            StringHelper::StringToObjectPtr(_value, data);
        }
        else if (StringHelper::IsStartsWith(_data, "json:"))
        {
            _value = std::make_shared<SoundSet>();
            _value->setString(_data + 5, _count - 5);
        }
        else
        {
            MAZE_NOT_IMPLEMENTED;
        }
    }

    //////////////////////////////////////////
    void SoundSet::ToString(SoundSet const* _value, String& _data)
    {
        if (!_value)
        {
            _data.clear();
            return;
        }

        if (true)
        {
            StringHelper::FormatString(_data, "json:%s", JSONHelper::ToString(_value->toJSONValue()).c_str());
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }

} // namespace Maze
//////////////////////////////////////////
