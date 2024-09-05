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
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(ByteBuffer);


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
    // Struct SoundLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_SOUND_API SoundLibraryData
    {
        //////////////////////////////////////////
        SoundLibraryData(
            SoundPtr const& _sound = nullptr,
            AssetFilePtr const& _assetFile = nullptr)
            : sound(_sound)
            , assetFile(_assetFile)
        {}

        SoundPtr sound;
        AssetFilePtr assetFile;
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
        SoundPtr const& getSound(HashedCString _assetFileName);

        //////////////////////////////////////////
        inline SoundPtr const& getSound(String const& _assetFileName) { return getSound(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        inline SoundPtr const& getSound(CString _assetFileName) { return getSound(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        SoundPtr const& getSound(AssetFilePtr const& _assetFile);

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
        Vector<String> getSoundLoaderExtensions();

        //////////////////////////////////////////
        void reloadAllAssetSounds();

    public:

        //////////////////////////////////////////
        MultiDelegate<HashedCString, SoundLoaderData const&> eventSoundLoaderAdded;


        //////////////////////////////////////////
        SoundLibraryData* addSoundToLibrary(SoundPtr const& _sound);

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
