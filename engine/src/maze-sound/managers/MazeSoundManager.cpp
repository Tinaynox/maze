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
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-sound/MazeSound.hpp"
#include "maze-sound/loaders/MazeLoaderWAV.hpp"


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
    void SoundManager::Initialize(SoundManagerPtr& _soundManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundManager, _soundManager, init());
    }

    //////////////////////////////////////////
    bool SoundManager::init()
    {
        registerSoundLoader(
            MAZE_HASHED_CSTRING("wav"),
            SoundLoaderData(
                (LoadSoundAssetFileFunction)&LoadWAV,
                (LoadSoundByteBufferFunction)&LoadWAV,
                (IsSoundAssetFileFunction)&IsWAVFile,
                (IsSoundByteBufferFunction)&IsWAVFile));

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
    SoundPtr const& SoundManager::getSound(HashedCString _assetFileName)
    {
        static SoundPtr nullPointer;

        StringKeyMap<SoundPtr>::const_iterator it = m_soundsByName.find(_assetFileName);
        if (it != m_soundsByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        if (!assetFile)
            return nullPointer;

        return getSound(assetFile);
    }

    //////////////////////////////////////////
    SoundPtr const& SoundManager::getSound(AssetFilePtr const& _assetFile)
    {
        static SoundPtr const nullPointer;
        
        StringKeyMap<SoundPtr>::const_iterator it = m_soundsByName.find(_assetFile->getFileName());
        if (it != m_soundsByName.end())
            return it->second;

        MAZE_ERROR_RETURN_VALUE_IF(!m_defaultSoundSystem, nullPointer, "SoundSystem is not available!");

        SoundPtr sound = Sound::Create(_assetFile, m_defaultSoundSystem.get());
        if (!sound)
            return nullPointer;

        sound->setName(_assetFile->getFileName());

        return addSound(sound);
    }

    //////////////////////////////////////////
    SoundPtr const& SoundManager::addSound(SoundPtr const& _sound)
    {
        auto it2 = m_soundsByName.insert(            
            _sound->getName(),
            _sound);

        return *it2;
    }

    //////////////////////////////////////////
    String const& SoundManager::getSoundName(Sound const* _sound)
    {
        static String nullPointer;

        for (auto const& soundData : m_soundsByName)
        {
            if (soundData.second.get() == _sound)
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

        Debug::Log("Loading sound data: %s...", _assetFile->getFileName().c_str());

        StringKeyMap<String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);

        if (metaData.empty() || !metaData.contains("ext"))
        {
            bool loaderFound = false;
            for (auto const& soundLoaderData : m_soundLoaders)
            {
                SoundLoaderData const& loaderData = soundLoaderData.second;
                if (loaderData.isSoundAssetFileFunc(_assetFile))
                {
                    loaderFound = true;
                    MAZE_ERROR_IF(!loaderData.loadSoundAssetFileFunc(_assetFile, soundData), "SoundData is not loaded - '%s'", _assetFile->getFileName().c_str());
                    break;
                }
            }

            MAZE_ERROR_IF(!loaderFound, "Unsupported sound format - %s!", _assetFile->getFileName().c_str());
        }
        else
        {
            HashedString fileExtension = StringHelper::ToLower(metaData["ext"]);

            auto it = m_soundLoaders.find(fileExtension);
            if (it != m_soundLoaders.end())
            {
                SoundLoaderData const& loaderData = it->second;
                MAZE_ERROR_IF(!loaderData.loadSoundAssetFileFunc(_assetFile, soundData), "SoundData is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported sound format - %s!", _assetFile->getFileName().c_str());
            }
        }

        Debug::Log("Loaded.", _assetFile->getFileName().c_str());

        return soundData;
    }

    //////////////////////////////////////////
    void SoundManager::loadSounds(Set<String> const& _tags)
    {
        Vector<String> extensions = getSoundLoaderExtensions();
        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(
            Set<String>(extensions.begin(), extensions.end()),
            [&](AssetFilePtr const& _assetFile)
            {
                return _assetFile->hasAnyOfTags(_tags);
            });

        for (AssetFilePtr const& assetFile : assetFiles)
        {
            String fileName = assetFile->getFileName();
            getSound(fileName);
        }
    }

    //////////////////////////////////////////
    Vector<String> SoundManager::getSoundLoaderExtensions()
    {
        Vector<String> result;
        for (auto const& soundLoaderData : m_soundLoaders)
            result.push_back(soundLoaderData.first);

        return result;
    }
    
} // namespace Maze
//////////////////////////////////////////
