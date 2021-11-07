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
#if (!defined(_MazeSoundSystem_hpp_))
#define _MazeSoundSystem_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-sound/MazeSoundData.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundSystem);
    MAZE_USING_SHARED_PTR(Sound);
    MAZE_USING_SHARED_PTR(SoundGroup);
    MAZE_USING_SHARED_PTR(SoundSource);
    MAZE_USING_SHARED_PTR(SoundSet);

    //////////////////////////////////////////
    // Class SoundSystem
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundSystem
        : public SharedObject<SoundSystem>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(SoundSystem);

    public:

        //////////////////////////////////////////
        virtual ~SoundSystem();

        //////////////////////////////////////////
        static SoundSystem* GetCurrentInstancePtr();


        //////////////////////////////////////////
        virtual String const& getName() MAZE_ABSTRACT;


        //////////////////////////////////////////
        virtual SoundPtr createSound() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual SoundGroupPtr createSoundGroup() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual SoundSourcePtr createSoundSource(SoundPtr const& _sound) MAZE_ABSTRACT;



        //////////////////////////////////////////
        SoundSourcePtr play(
            SoundPtr const& _sound,
            bool _looped = false,
            SoundGroupPtr const& _soundGroup = nullptr,
            F32 _volume = 1.0f);

        //////////////////////////////////////////
        SoundSourcePtr play(
            String const& _soundAssetName,
            bool _looped = false,
            SoundGroupPtr const& _soundGroup = nullptr,
            F32 _volume = 1.0f);

        //////////////////////////////////////////
        SoundSourcePtr play(
            SoundSetPtr const& _soundSet,
            bool _looped = false,
            SoundGroupPtr const& _soundGroup = nullptr);


        //////////////////////////////////////////
        SoundDataPtr loadSoundData(AssetFilePtr const& _assetFile);

    protected:

        //////////////////////////////////////////
        SoundSystem();

        //////////////////////////////////////////
        bool init();

    protected:
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSystem_hpp_
//////////////////////////////////////////
