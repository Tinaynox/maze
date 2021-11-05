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
#include "maze-sound/MazeSound.hpp"


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
        return true;
    }

    //////////////////////////////////////////
    void SoundManager::addSoundSystem(SoundSystemPtr const& _soundSystem)
    {
        UnorderedMap<String, SoundSystemPtr>::iterator it = m_soundSystems.find(_soundSystem->getName());
        MAZE_ERROR_RETURN_IF(it != m_soundSystems.end(), "SoundSystem %s is already exists!", _soundSystem->getName().c_str());

        m_soundSystems.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_soundSystem->getName()),
            std::forward_as_tuple(_soundSystem));


        if (m_defaultSoundSystem == nullptr)
        {
            setDefaultSoundSystem(_soundSystem);
        }
    }

    //////////////////////////////////////////
    void SoundManager::removeSoundSystem(SoundSystemPtr const& _soundSystem)
    {
        UnorderedMap<String, SoundSystemPtr>::iterator it = m_soundSystems.find(_soundSystem->getName());
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

        UnorderedMap<String, SoundSystemPtr>::iterator it = m_soundSystems.find(_soundSystem->getName());
        MAZE_ERROR_RETURN_IF(it == m_soundSystems.end(), "SoundSystem %s is not in SoundSystems list!", _soundSystem->getName().c_str());

        m_defaultSoundSystem = _soundSystem;
    }

    //////////////////////////////////////////
    SoundPtr const& SoundManager::getSound(String const& _assetFileName)
    {
        static SoundPtr nullPointer;

        UnorderedMap<String, SoundPtr>::const_iterator it = m_soundsByName.find(_assetFileName);
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
        UnorderedMap<String, SoundPtr>::const_iterator it = m_soundsByName.find(_assetFile->getFileName());
        if (it != m_soundsByName.end())
            return it->second;

        SoundPtr sound = Sound::Create(_assetFile, m_defaultSoundSystem.get());
        sound->setName(_assetFile->getFileName());

        return addSound(sound);
    }

    //////////////////////////////////////////
    SoundPtr const& SoundManager::addSound(SoundPtr const& _sound)
    {
        auto it2 = m_soundsByName.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_sound->getName()),
            std::forward_as_tuple(_sound));

        return it2.first->second;
    }
    
} // namespace Maze
//////////////////////////////////////////
