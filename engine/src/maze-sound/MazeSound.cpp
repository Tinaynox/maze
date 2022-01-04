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
#include "maze-sound/MazeSound.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Sound
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(Sound);

    //////////////////////////////////////////
    Sound::Sound()
    {
    }

    //////////////////////////////////////////
    Sound::~Sound()
    {
        
    }

    //////////////////////////////////////////
    SoundPtr Sound::Create(SoundSystem* _soundSystem)
    {
        if (!_soundSystem)
            _soundSystem = SoundManager::GetInstancePtr()->getDefaultSoundSystemRaw();

        if (!_soundSystem)
            return nullptr;
        
        return _soundSystem->createSound();
    }

    //////////////////////////////////////////
    SoundPtr Sound::Create(
        AssetFilePtr const& _assetFile,
        SoundSystem* _soundSystem)
    {
        SoundPtr sound = Sound::Create(_soundSystem);
        if (sound)
        {
            sound->loadFromAssetFile(_assetFile);
            sound->setName(_assetFile->getFileName());
        }

        return sound;
    }

    //////////////////////////////////////////
    SoundPtr Sound::Create(
        String const& _assetFileName,
        SoundSystem* _soundSystem)
    {
        SoundPtr sound = Sound::Create(_soundSystem);
        if (sound)
        {
            sound->loadFromAssetFile(_assetFileName);
            sound->setName(_assetFileName);
        }

        return sound;
    }

    //////////////////////////////////////////
    bool Sound::init(SoundSystem* _soundSystem)
    {
        m_soundSystem = _soundSystem;

        return true;
    }

    //////////////////////////////////////////
    void Sound::loadFromAssetFile(AssetFilePtr const& _assetFile)
    {
        SoundDataPtr soundData = SoundSystem::GetCurrentInstancePtr()->loadSoundData(_assetFile);
        loadSound(soundData);
    }

    //////////////////////////////////////////
    void Sound::loadFromAssetFile(String const& _assetFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile);
    }

    //////////////////////////////////////////
    SoundSourcePtr Sound::play(
        bool _looped,
        SoundGroupPtr const& _soundGroup,
        F32 _volume)
    {
        return m_soundSystem->play(cast<Sound>(), _looped, _soundGroup, _volume);
    }

    //////////////////////////////////////////
    String Sound::toString() const
    {
        String str;
        ToString(this, str);
        return str;
    }

    //////////////////////////////////////////
    void Sound::setString(CString _data, Size _count)
    {
        loadFromJSONValue(JSONHelper::FromString(String(_data, _data + _count)));
    }

    //////////////////////////////////////////
    void Sound::FromString(SoundPtr& _value, CString _data, Size _count)
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
        else
        {
            _value = SoundManager::GetInstancePtr()->getSound(_data);
        }
    }

    //////////////////////////////////////////
    void Sound::ToString(Sound const* _value, String& _data)
    {
        if (!_value)
        {
            _data.clear();
            return;
        }
        
        String const& soundName = SoundManager::GetInstancePtr()->getSoundName(_value);
        if (!soundName.empty())
        {
            _data = soundName;
        }
        else
        {
            StringHelper::FormatString(_data, "ptr:%p", _value);
        }
    }

} // namespace Maze
//////////////////////////////////////////
