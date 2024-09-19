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
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-sound/MazeSound.hpp"
#include "maze-sound/loaders/MazeLoaderWAV.hpp"
#include "maze-sound/assets/MazeAssetUnitSound.hpp"
#include "maze-core/system/MazeTimer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SoundManager
    //
    //////////////////////////////////////////
    SoundManager* SoundManager::s_instance = nullptr;

    //////////////////////////////////////////
    SoundManager::SoundManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    SoundManager::~SoundManager()
    {
        m_soundSystems.clear();
        m_defaultSoundSystem.reset();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void SoundManager::Initialize(
        SoundManagerPtr& _soundManager,
        DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundManager, _soundManager, init(_config));
    }

    //////////////////////////////////////////
    bool SoundManager::init(DataBlock const& _config)
    {
        registerSoundLoader(
            MAZE_HASHED_CSTRING("wav"),
            SoundLoaderData(
                (LoadSoundAssetFileFunction)&LoadWAV,
                (LoadSoundByteBufferFunction)&LoadWAV,
                (IsSoundAssetFileFunction)&IsWAVFile,
                (IsSoundByteBufferFunction)&IsWAVFile));


        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitSound::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitSound::Create(_file, _data);
                });
            

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitSound>::UID())
                        _assetUnit->castRaw<AssetUnitSound>()->initSound();
                });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
            {
                if (SoundManager::GetInstancePtr()->hasSoundLoader(_extension))
                {
                    if (!_assetFile->getAssetUnit<AssetUnitSound>())
                        _assetFile->addAssetUnit(AssetUnitSound::Create(_assetFile));
                }
            });
        }

        return true;
    }

    //////////////////////////////////////////
    void SoundManager::addSoundSystem(SoundSystemPtr const& _soundSystem)
    {
        StringKeyMap<SoundSystemPtr>::iterator it = m_soundSystems.find(_soundSystem->getName());
        MAZE_ERROR_RETURN_IF(it != m_soundSystems.end(), "SoundSystem %s is already exists!", _soundSystem->getName().c_str());

        m_soundSystems.insert(            
            _soundSystem->getName(),
            _soundSystem);


        if (m_defaultSoundSystem == nullptr)
        {
            setDefaultSoundSystem(_soundSystem);
        }
    }

    //////////////////////////////////////////
    void SoundManager::removeSoundSystem(SoundSystemPtr const& _soundSystem)
    {
        StringKeyMap<SoundSystemPtr>::iterator it = m_soundSystems.find(_soundSystem->getName());
        MAZE_ERROR_RETURN_IF(it == m_soundSystems.end(), "SoundSystem %s is not exists!", _soundSystem->getName().c_str());

        m_soundSystems.erase(it);

        if (_soundSystem == m_defaultSoundSystem)
        {
            if (m_soundSystems.empty())
                m_defaultSoundSystem.reset();
            else
                m_defaultSoundSystem.reset();
        }
    }

    //////////////////////////////////////////
    void SoundManager::setDefaultSoundSystem(SoundSystemPtr const& _soundSystem)
    {
        if (_soundSystem == nullptr)
        {
            m_defaultSoundSystem.reset();
            return;
        }

        StringKeyMap<SoundSystemPtr>::iterator it = m_soundSystems.find(_soundSystem->getName());
        MAZE_ERROR_RETURN_IF(it == m_soundSystems.end(), "SoundSystem %s is not in SoundSystems list!", _soundSystem->getName().c_str());

        m_defaultSoundSystem = _soundSystem;
    }

    //////////////////////////////////////////
    SoundLibraryData const* SoundManager::getSoundLibraryData(HashedCString _soundName)
    {
        StringKeyMap<SoundLibraryData>::const_iterator it = m_soundsLibrary.find(_soundName);
        if (it != m_soundsLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    SoundPtr const& SoundManager::getOrLoadSound(
        HashedCString _assetFileName,
        bool _syncLoad)
    {
        static SoundPtr const nullPointer;

        if (_assetFileName.empty())
            return nullPointer;

        SoundLibraryData const* libraryData = getSoundLibraryData(_assetFileName);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->sound;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        if (!assetFile)
            return nullPointer;

        return getOrLoadSound(assetFile);
    }

    //////////////////////////////////////////
    SoundPtr const& SoundManager::getOrLoadSound(
        AssetFilePtr const& _assetFile,
        bool _syncLoad)
    {
        static SoundPtr const nullPointer;
        
        SoundLibraryData const* libraryData = getSoundLibraryData(_assetFile->getFileName());
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->sound;
        }

        MAZE_WARNING_RETURN_VALUE_IF(!m_defaultSoundSystem, nullPointer, "SoundSystem is not available!");

        SoundPtr sound = Sound::Create(_assetFile, m_defaultSoundSystem.get());
        if (!sound)
            return nullPointer;

        sound->setName(_assetFile->getFileName());

        SoundLibraryData* data = addSoundToLibrary(sound);
        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)_assetFile,
                    materialWeak = (SoundWPtr)sound
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    SoundPtr sound = materialWeak.lock();
                    if (assetFile && sound)
                        sound->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)_assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            return data->sound;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    SoundLibraryData* SoundManager::addSoundToLibrary(
        SoundPtr const& _sound,
        SoundLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_soundsLibrary.insert(            
            _sound->getName(),
            { _sound, _callbacks, _info });

        return it2;
    }

    //////////////////////////////////////////
    String const& SoundManager::getSoundName(Sound const* _sound)
    {
        static String nullPointer;

        for (auto const& soundData : m_soundsLibrary)
        {
            if (soundData.second.sound.get() == _sound)
                return soundData.first;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    SoundDataPtr SoundManager::loadSoundData(AssetFilePtr const& _assetFile)
    {
        SoundDataPtr soundData;

        if (!_assetFile)
            return soundData;

        Debug::Log("Loading sound data: %s...", _assetFile->getFileName().toUTF8().c_str());
        Timer timer;

        DataBlock metaData;
        AssetManager::GetInstancePtr()->loadMetaData(_assetFile, metaData);

        if (metaData.isEmpty() || !metaData.isParamExists(MAZE_HCS("ext")))
        {
            bool loaderFound = false;
            for (auto const& soundLoaderData : m_soundLoaders)
            {
                SoundLoaderData const& loaderData = soundLoaderData.second;
                if (loaderData.isSoundAssetFileFunc(_assetFile))
                {
                    loaderFound = true;
                    MAZE_ERROR_IF(!loaderData.loadSoundAssetFileFunc(_assetFile, soundData), "SoundData is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
                    break;
                }
            }

            MAZE_ERROR_IF(!loaderFound, "Unsupported sound format - %s!", _assetFile->getFileName().toUTF8().c_str());
        }
        else
        {
            HashedString fileExtension = HashedString(
                StringHelper::ToLower(
                    metaData.getString(
                        MAZE_HCS("ext"))));

            auto it = m_soundLoaders.find(fileExtension);
            if (it != m_soundLoaders.end())
            {
                SoundLoaderData const& loaderData = it->second;
                MAZE_ERROR_IF(!loaderData.loadSoundAssetFileFunc(_assetFile, soundData), "SoundData is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported sound format - %s!", _assetFile->getFileName().toUTF8().c_str());
            }
        }

        F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
        MAZE_LOG("Sound data %s loaded for %.1fms.", _assetFile->getFileName().toUTF8().c_str(), msTime);

        return soundData;
    }

    //////////////////////////////////////////
    void SoundManager::loadAssetSounds(Set<String> const& _tags)
    {
        MAZE_PROFILE_EVENT("SoundManager::loadAssetSounds");

        Vector<HashedString> extensions = getSoundLoaderExtensions();
        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(
            Set<String>(extensions.begin(), extensions.end()),
            [&](AssetFilePtr const& _assetFile)
            {
                return _assetFile->hasAnyOfTags(_tags);
            });

        for (AssetFilePtr const& assetFile : assetFiles)
        {
            String fileName = assetFile->getFileName();
            getOrLoadSound(fileName);
        }
    }

    //////////////////////////////////////////
    void SoundManager::unloadAssetSounds(Set<String> const& _tags)
    {
        MAZE_PROFILE_EVENT("SoundManager::unloadAssetSounds");

        Vector<std::function<void(bool)>> unloadCallbacks;

        m_soundsLibrary.iterate(
            [&](HashedCString _name, SoundLibraryData const& _data)
            {
                if (_data.callbacks.hasAnyOfTags && _data.callbacks.hasAnyOfTags(_tags) && _data.callbacks.requestUnload)
                    unloadCallbacks.push_back(_data.callbacks.requestUnload);

                return true;
            });

        for (std::function<void(bool)> const& unloadCallback : unloadCallbacks)
            unloadCallback(true);
    }

    //////////////////////////////////////////
    Vector<HashedString> SoundManager::getSoundLoaderExtensions()
    {
        Vector<HashedString> result;
        for (StringKeyMap<SoundLoaderData>::iterator it = m_soundLoaders.begin(),
                                                     end = m_soundLoaders.end();
                                                     it != end;
                                                     ++it)
                                                     result.push_back(HashedString(it.key()));

        return result;
    }

    //////////////////////////////////////////
    void SoundManager::reloadAllAssetSounds()
    {
        for (auto const& soundData : m_soundsLibrary)
            if (soundData.second.callbacks.requestReload)
                soundData.second.callbacks.requestReload(true);
    }
    
} // namespace Maze
//////////////////////////////////////////
