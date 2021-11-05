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
#if (!defined(_MazeSoundOpenAL_hpp_))
#define _MazeSoundOpenAL_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeHeaderOpenAL.hpp"
#include "maze-sound/MazeSound.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundOpenAL);
    MAZE_USING_SHARED_PTR(Sound);


    //////////////////////////////////////////
    // Class SoundOpenAL
    //
    //////////////////////////////////////////
    class MAZE_SOUND_SYSTEM_OPENAL_API SoundOpenAL
        : public Sound
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SoundOpenAL, Sound);

    public:

        //////////////////////////////////////////
        virtual ~SoundOpenAL();

        //////////////////////////////////////////
        static SoundOpenALPtr Create(SoundSystem* _soundSystem = nullptr);

        //////////////////////////////////////////
        virtual bool loadSound(SoundDataPtr const& _soundData) MAZE_OVERRIDE;

        //////////////////////////////////////////
        inline MZALuint getBufferID() const { return m_bufferID; }

    protected:

        //////////////////////////////////////////
        SoundOpenAL();

        //////////////////////////////////////////
        virtual bool init(SoundSystem* _soundSystem);


        //////////////////////////////////////////
        void generateALObjects();

        //////////////////////////////////////////
        void deleteALObjects();

    protected:
        MZALuint m_bufferID = 0;
        MZALenum m_bufferFormat = 0;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundOpenAL_hpp_
//////////////////////////////////////////
