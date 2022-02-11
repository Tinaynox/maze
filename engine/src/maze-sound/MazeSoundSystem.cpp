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
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-sound/MazeSound.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-sound/loaders/MazeLoaderWAV.hpp"
#include "maze-sound/MazeSoundSource.hpp"
#include "maze-sound/MazeSoundSet.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class SoundSystem
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS(SoundSystem);

    //////////////////////////////////////////
    SoundSystem::SoundSystem()
    {
    }

    //////////////////////////////////////////
    SoundSystem::~SoundSystem()
    {
        
    }

    //////////////////////////////////////////
    bool SoundSystem::init()
    {

        return true;
    }

    //////////////////////////////////////////
    SoundSystem* SoundSystem::GetCurrentInstancePtr()
    {
        return SoundManager::GetInstancePtr()->getDefaultSoundSystemRaw();
    }

    //////////////////////////////////////////
    SoundSourcePtr SoundSystem::play(
        SoundPtr const& _sound,
        bool _looped,
        SoundGroupPtr const& _soundGroup,
        F32 _volume)
    {
        SoundSourcePtr soundSource = createSoundSource(_sound);
        soundSource->setLooped(_looped);
        soundSource->setSoundGroup(_soundGroup);
        soundSource->setVolume(_volume);
        soundSource->play();
        return soundSource;
    }

    //////////////////////////////////////////
    SoundSourcePtr SoundSystem::play(
        String const& _soundAssetName,
        bool _looped,
        SoundGroupPtr const& _soundGroup,
        F32 _volume)
    {
        SoundPtr const& sound = SoundManager::GetInstancePtr()->getSound(_soundAssetName);
        MAZE_ERROR_RETURN_VALUE_IF(!sound, nullptr, "Sound is not found - %s", _soundAssetName.c_str());
        return play(sound, _looped, _soundGroup, _volume);
    }

    //////////////////////////////////////////
    SoundSourcePtr SoundSystem::play(
        SoundSetPtr const& _soundSet,
        bool _looped,
        SoundGroupPtr const& _soundGroup)
    {
        return play(_soundSet->fetch(), _looped, _soundGroup, _soundSet->getVolume());
    }

    //////////////////////////////////////////
    SoundDataPtr SoundSystem::loadSoundData(AssetFilePtr const& _assetFile)
    {
        SoundDataPtr soundData;

        Debug::Log("Loading texture pixel sheet: %s...", _assetFile->getFileName().c_str());

        UnorderedMap<String, String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);

        if (metaData.empty())
        {
            if (Maze::IsWAVFile(_assetFile))
            {
                MAZE_ERROR_IF(!Maze::LoadWAV(_assetFile, soundData), "SoundData is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported sound format!");
            }
        }
        else
        {
            String fileExtension = StringHelper::ToLower(metaData["ext"]);
            if (fileExtension.empty())
                fileExtension = FileHelper::GetFileExtension(_assetFile->getFileName());

            if (fileExtension == "wav")
            {
                MAZE_ERROR_IF(!Maze::LoadWAV(_assetFile, soundData), "SoundData is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported sound format!");
            }
        }

        Debug::Log("Loaded.", _assetFile->getFileName().c_str());

        return soundData;
    }

} // namespace Maze
//////////////////////////////////////////
