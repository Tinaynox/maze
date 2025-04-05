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
#pragma once
#if (!defined(_MazeSoundManager_hpp_))
#define _MazeSoundManager_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-sound/MazeSoundSystem.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundManager);
    MAZE_USING_SHARED_PTR(SoundSystem);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(ByteBuffer);


    //////////////////////////////////////////
    using LoadSoundAssetFileFunction = bool(*)(AssetFilePtr const& _file, SoundDataPtr& _soundData);
    using LoadSoundByteBufferFunction = bool(*)(ByteBufferPtr const& _fileData, SoundDataPtr& _soundData);
    using IsSoundAssetFileFunction = bool(*)(AssetFilePtr const& _file);
    using IsSoundByteBufferFunction = bool(*)(ByteBufferPtr const& _fileData);


    //////////////////////////////////////////
    // Struct SoundLoaderData
    //
    //////////////////////////////////////////
    struct MAZE_SOUND_API SoundLoaderData
    {
        //////////////////////////////////////////
        SoundLoaderData() = default;

        //////////////////////////////////////////
        SoundLoaderData(
            LoadSoundAssetFileFunction _loadSoundAssetFileFunc,
            LoadSoundByteBufferFunction _loadSoundByteBufferFunc,
            IsSoundAssetFileFunction _isSoundAssetFileFunc,
            IsSoundByteBufferFunction _isSoundByteBufferFunc)
            : loadSoundAssetFileFunc(_loadSoundAssetFileFunc)
            , loadSoundByteBufferFunc(_loadSoundByteBufferFunc)
            , isSoundAssetFileFunc(_isSoundAssetFileFunc)
            , isSoundByteBufferFunc(_isSoundByteBufferFunc)
        {}

        LoadSoundAssetFileFunction loadSoundAssetFileFunc = nullptr;
        LoadSoundByteBufferFunction loadSoundByteBufferFunc = nullptr;
        IsSoundAssetFileFunction isSoundAssetFileFunc = nullptr;
        IsSoundByteBufferFunction isSoundByteBufferFunc = nullptr;
    };


    //////////////////////////////////////////
    // Struct SoundLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_SOUND_API SoundLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct SoundLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_SOUND_API SoundLibraryData
    {
        //////////////////////////////////////////
        SoundLibraryData(
            SoundPtr const& _sound = nullptr,
            SoundLibraryDataCallbacks const& _callbacks = SoundLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : sound(_sound)
            , callbacks(_callbacks)
            , data(_data)
        {}

        SoundPtr sound;
        SoundLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Class SoundManager
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundManager
    {
    public:

        //////////////////////////////////////////
        virtual ~SoundManager();

        //////////////////////////////////////////
        static void Initialize(
            SoundManagerPtr& _soundManager,
            DataBlock const& _config = DataBlock::c_empty);


        //////////////////////////////////////////
        static inline SoundManager* GetInstancePtr() { return s_instance; }

        //////////////////////////////////////////
        static inline SoundManager& GetInstance() { return *s_instance; }       


        //////////////////////////////////////////
        void addSoundSystem(SoundSystemPtr const& _soundSystem);

        //////////////////////////////////////////
        void removeSoundSystem(SoundSystemPtr const& _soundSystem);


        //////////////////////////////////////////
        inline StringKeyMap<SoundLibraryData> const& getSoundsLibrary() const { return m_soundsLibrary; }


        //////////////////////////////////////////
        SoundLibraryData const* getSoundLibraryData(HashedCString _soundName);

        //////////////////////////////////////////
        SoundLibraryData const* getSoundLibraryData(String const& _soundName) { return getSoundLibraryData(MAZE_HASHED_CSTRING(_soundName.c_str())); }

        //////////////////////////////////////////
        SoundLibraryData const* getSoundLibraryData(CString _soundName) { return getSoundLibraryData(MAZE_HASHED_CSTRING(_soundName)); }


        //////////////////////////////////////////
        SoundPtr const& getOrLoadSound(HashedCString _assetFileName, bool _syncLoad = true);

        //////////////////////////////////////////
        inline SoundPtr const& getOrLoadSound(String const& _assetFileName, bool _syncLoad = true) { return getOrLoadSound(MAZE_HASHED_CSTRING(_assetFileName.c_str()), _syncLoad); }

        //////////////////////////////////////////
        inline SoundPtr const& getOrLoadSound(CString _assetFileName, bool _syncLoad = true) { return getOrLoadSound(MAZE_HASHED_CSTRING(_assetFileName), _syncLoad); }

        //////////////////////////////////////////
        SoundPtr const& getOrLoadSound(
            AssetFilePtr const& _assetFile,
            bool _syncLoad = true);


        //////////////////////////////////////////
        String const& getSoundName(Sound const* _sound);

        //////////////////////////////////////////
        inline StringKeyMap<SoundSystemPtr> const& getSoundSystems() const { return m_soundSystems; }


        //////////////////////////////////////////
        inline SoundSystemPtr const& getDefaultSoundSystem() const { return m_defaultSoundSystem; }

        //////////////////////////////////////////
        inline SoundSystem* getDefaultSoundSystemRaw() const { return m_defaultSoundSystem.get(); }

        //////////////////////////////////////////
        void setDefaultSoundSystem(SoundSystemPtr const& _soundSystem);


        //////////////////////////////////////////
        SoundDataPtr loadSoundData(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void loadAssetSounds(Set<String> const& _tags);

        //////////////////////////////////////////
        void unloadAssetSounds(Set<String> const& _tags);



        //////////////////////////////////////////
        inline void registerSoundLoader(
            HashedCString _extension,
            SoundLoaderData const& _data)
        {
            m_soundLoaders.insert(_extension, _data);
            eventSoundLoaderAdded(_extension, _data);
        }

        //////////////////////////////////////////
        inline void clearSoundLoader(HashedCString _extension)
        {
            m_soundLoaders.erase(_extension);
        }

        //////////////////////////////////////////
        Vector<HashedString> getSoundLoaderExtensions();

        //////////////////////////////////////////
        inline bool hasSoundLoader(HashedCString _extension)
        {
            return m_soundLoaders.find(_extension) != m_soundLoaders.end();
        }

        //////////////////////////////////////////
        void reloadAllAssetSounds();

    public:

        //////////////////////////////////////////
        MultiDelegate<HashedCString, SoundLoaderData const&> eventSoundLoaderAdded;


        //////////////////////////////////////////
        SoundLibraryData* addSoundToLibrary(
            SoundPtr const& _sound,
            SoundLibraryDataCallbacks const& _callbacks = SoundLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

    protected:

        //////////////////////////////////////////
        SoundManager();

        //////////////////////////////////////////
        bool init(DataBlock const& _config);

    protected:
        static SoundManager* s_instance;

        StringKeyMap<SoundSystemPtr> m_soundSystems;
        SoundSystemPtr m_defaultSoundSystem;

        StringKeyMap<SoundLoaderData> m_soundLoaders;

        StringKeyMap<SoundLibraryData> m_soundsLibrary;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundManager_hpp_
//////////////////////////////////////////
