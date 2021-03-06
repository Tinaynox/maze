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
#if (!defined(_MazeSoundGroup_hpp_))
#define _MazeSoundGroup_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-sound/MazeSoundSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundGroup);


    //////////////////////////////////////////
    // Class SoundGroup
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundGroup
        : public SharedObject<SoundGroup>
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS(SoundGroup);

    public:

        //////////////////////////////////////////
        virtual ~SoundGroup();

        //////////////////////////////////////////
        static SoundGroupPtr Create(SoundSystem* _soundSystem = nullptr);


        //////////////////////////////////////////
        void setVolume(F32 _volume);

        //////////////////////////////////////////
        inline F32 getVolume() const { return m_volume; }


        //////////////////////////////////////////
        void setPitch(F32 _pitch);

        //////////////////////////////////////////
        inline F32 getPitch() const { return m_pitch; }

    public:

        //////////////////////////////////////////
        MultiDelegate<F32> eventVolumeChanged;
        MultiDelegate<F32> eventPitchChanged;

    protected:

        //////////////////////////////////////////
        SoundGroup();

        //////////////////////////////////////////
        bool init(SoundSystem* _soundSystem);


    protected:
        SoundSystem* m_soundSystem = nullptr;
        F32 m_volume = 1.0f;
        F32 m_pitch = 1.0f;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundGroup_hpp_
//////////////////////////////////////////
