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
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-sound/assets/MazeAssetUnitSound.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Sound
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(Sound);

    //////////////////////////////////////////
    Sound* Sound::s_instancesList = nullptr;

    //////////////////////////////////////////
    Sound::Sound()
    {
        if (s_instancesList)
            s_instancesList->m_instancesListNext = this;
        m_instancesListPrev = s_instancesList;
        s_instancesList = this;
    }

    //////////////////////////////////////////
    Sound::~Sound()
    {
        if (m_instancesListPrev)
            m_instancesListPrev->m_instancesListNext = m_instancesListNext;
        if (m_instancesListNext)
            m_instancesListNext->m_instancesListPrev = m_instancesListPrev;
        else
        if (s_instancesList == this)
            s_instancesList = m_instancesListPrev;
    }

    //////////////////////////////////////////
    SoundPtr Sound::Create(SoundSystem* _soundSystem)
    {
        if (!_soundSystem)
            _soundSystem = SoundManager::GetInstancePtr()->getDefaultSoundSystemRaw();

        MAZE_ERROR_RETURN_VALUE_IF(!_soundSystem, nullptr, "SoundSystem is not available!");
        
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
        MAZE_PROFILE_EVENT("Sound::loadFromAssetFile");
        SoundDataPtr soundData = SoundManager::GetInstancePtr()->loadSoundData(_assetFile);
        loadSound(soundData);
    }

    //////////////////////////////////////////
    void Sound::loadFromAssetFile(String const& _assetFileName)
    {
        MAZE_PROFILE_EVENT("Sound::loadFromAssetFile");
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
    void Sound::reload()
    {
        SoundLibraryData const* libraryData = SoundManager::GetInstancePtr()->getSoundLibraryData(m_name.asHashedCString());
        if (libraryData && libraryData->callbacks.requestReload)
            libraryData->callbacks.requestReload(true);
    }

    //////////////////////////////////////////
    void Sound::FromString(SoundPtr& _value, CString _data, Size _count)
    {
        MAZE_PROFILE_EVENT("Sound::FromString");

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
            _value = SoundManager::GetInstancePtr()->getOrLoadSound(_data);
        }
    }

    //////////////////////////////////////////
    void Sound::ToString(Sound const* _value, String& _data)
    {
        MAZE_PROFILE_EVENT("Sound::ToString");

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

    //////////////////////////////////////////
    void Sound::IterateSounds(std::function<bool(Sound*)> _cb)
    {
        Sound* instance = s_instancesList;
        while (instance)
        {
            if (!_cb(instance))
                break;

            instance = instance->m_instancesListPrev;
        }
    }


    //////////////////////////////////////////
    // Class SoundAssetRef
    //
    //////////////////////////////////////////
    bool SoundAssetRef::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
        {
            DataBlockParamType paramType = _dataBlock.getParamType(paramIndex);
            switch (paramType)
            {
                // by AUID
                case DataBlockParamType::ParamU32:
                {
                    AssetUnitId auid = _dataBlock.getU32(paramIndex);

                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(auid);
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitSound>::UID())
                    {
                        setSound(assetUnit->castRaw<AssetUnitSound>()->loadSound(true));
                        return true;
                    }

                    break;
                }
                // by name
                case DataBlockParamType::ParamString:
                {
                    String const& name = _dataBlock.getString(paramIndex);
                    SoundPtr const& material = SoundManager::GetInstancePtr()->getOrLoadSound(name);
                    setSound(material);
                    return true;
                }
                default:
                {
                    MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                    break;
                }
            }
        }

        setSound(SoundPtr());
        return true;
    }

    //////////////////////////////////////////
    void SoundAssetRef::toDataBlock(DataBlock& _dataBlock) const
    {
        if (!m_sound)
        {
            _dataBlock.clearData();
            return;
        }

        // Save as AUID
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(m_sound->getName());
            if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitSound>::UID())
            {
                SoundPtr const& assetUnitSound = assetUnit->castRaw<AssetUnitSound>()->getSound();
                if (assetUnitSound == m_sound)
                {
                    ValueToDataBlock(assetUnit->getAssetUnitId(), _dataBlock);
                    return;
                }
            }
        }

        // Save as string
        ValueToDataBlock(m_sound->getName().c_str(), _dataBlock);
    }

} // namespace Maze
//////////////////////////////////////////
