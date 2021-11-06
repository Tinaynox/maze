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
#if (!defined(_MazeSound_hpp_))
#define _MazeSound_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-sound/MazeSoundData.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Sound);
    MAZE_USING_SHARED_PTR(SoundSource);
    MAZE_USING_SHARED_PTR(SoundGroup);
    MAZE_USING_SHARED_PTR(SoundSystem);
    MAZE_USING_SHARED_PTR(AssetFile);


    //////////////////////////////////////////
    // Class Sound
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API Sound
        : public SharedObject<Sound>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(Sound);

    public:

        //////////////////////////////////////////
        virtual ~Sound();

        //////////////////////////////////////////
        static SoundPtr Create(SoundSystem* _soundSystem = nullptr);

        //////////////////////////////////////////
        static SoundPtr Create(
            AssetFilePtr const& _assetFile,
            SoundSystem* _soundSystem = nullptr);

        //////////////////////////////////////////
        static SoundPtr Create(
            String const& _assetFileName,
            SoundSystem* _soundSystem = nullptr);


        //////////////////////////////////////////
        inline String const& getName() const { return m_name; }

        //////////////////////////////////////////
        inline void setName(String const& _name) { m_name = _name; }


        //////////////////////////////////////////
        void loadFromAssetFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void loadFromAssetFile(String const& _assetFileName);

        //////////////////////////////////////////
        virtual bool loadSound(SoundDataPtr const& _soundData) MAZE_ABSTRACT;


        //////////////////////////////////////////
        SoundSourcePtr play(
            bool _cycled = false,
            SoundGroupPtr const& _soundGroup = nullptr);

    protected:

        //////////////////////////////////////////
        Sound();

        //////////////////////////////////////////
        virtual bool init(SoundSystem* _soundSystem);

    protected:
        SoundSystem* m_soundSystem;

        String m_name;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSound_hpp_
//////////////////////////////////////////
