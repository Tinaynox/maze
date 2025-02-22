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
#include "maze-sound/assets/MazeAssetUnitSound.hpp"
#include "maze-sound/MazeSound.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitSound
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitSound, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitSound::AssetUnitSound()
    {

    }

    //////////////////////////////////////////
    AssetUnitSound::~AssetUnitSound()
    {
    }

    //////////////////////////////////////////
    AssetUnitSoundPtr AssetUnitSound::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitSoundPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitSound,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitSound::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    SoundPtr const& AssetUnitSound::loadSound(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initSound();
            _syncLoad ? loadNow() : load();
        }

        return m_sound;
    }

    //////////////////////////////////////////
    bool AssetUnitSound::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initSound();
        if (!m_sound)
            return false;

        m_sound->loadFromAssetFile(assetFile);
        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitSound::unloadNowImpl()
    {
        if (m_sound)
        {
            // #TODO: Unload sound
        }

        return true;
    }

    //////////////////////////////////////////
    SoundPtr const& AssetUnitSound::initSound()
    {
        if (m_sound)
            return m_sound;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_sound;

        m_sound = Sound::Create();
        m_sound->setName(m_data.getString(MAZE_HCS("name"), assetFile->getFileName()));

        if (SoundManager::GetInstancePtr())
        {
            SoundLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitSoundWPtr)cast<AssetUnitSound>()](bool _syncLoad)
                {
                    if (AssetUnitSoundPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitSoundWPtr)cast<AssetUnitSound>()] (bool _syncLoad)
                {
                    if (AssetUnitSoundPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitSoundWPtr)cast<AssetUnitSound>()](bool _syncLoad)
                {
                    if (AssetUnitSoundPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitSoundWPtr)cast<AssetUnitSound>()](Set<String> const& _tags)
                {
                    if (AssetUnitSoundPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            SoundManager::GetInstancePtr()->addSoundToLibrary(
                m_sound,
                callbacks,
                info);
        }

        return m_sound;
    }


} // namespace Maze
//////////////////////////////////////////
