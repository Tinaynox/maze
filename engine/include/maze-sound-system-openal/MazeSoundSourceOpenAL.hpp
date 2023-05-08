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
#if (!defined(_MazeSoundSourceOpenAL_hpp_))
#define _MazeSoundSourceOpenAL_hpp_


//////////////////////////////////////////
#include "maze-sound-system-openal/MazeSoundSystemOpenALHeader.hpp"
#include "maze-sound-system-openal/MazeHeaderOpenAL.hpp"
#include "maze-sound/MazeSoundSource.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundSourceOpenAL);
    MAZE_USING_SHARED_PTR(Sound);


    //////////////////////////////////////////
    // Class SoundSourceOpenAL
    //
    //////////////////////////////////////////
    class MAZE_SOUND_SYSTEM_OPENAL_API SoundSourceOpenAL
        : public SoundSource
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(SoundSourceOpenAL, SoundSource);

    public:

        //////////////////////////////////////////
        virtual ~SoundSourceOpenAL();

        //////////////////////////////////////////
        static SoundSourceOpenALPtr Create(SoundPtr const& _sound, SoundSystem* _soundSystem = nullptr);

        //////////////////////////////////////////
        virtual void play() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void stop() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isPlaying() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool rewindOffset(F32 _seconds) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool update(F32 _dt);

        //////////////////////////////////////////
        virtual void setLooped(bool _looped) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setVolume(F32 _volume) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setPitch(F32 _pitch) MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline MZALuint getSourceID() const { return m_sourceID; }

    public:

        //////////////////////////////////////////
        static void IterateSoundSourcesOpenAL(std::function<bool(SoundSourceOpenAL*)> _cb);


    protected:

        //////////////////////////////////////////
        SoundSourceOpenAL();

        //////////////////////////////////////////
        virtual bool init(SoundPtr const& _sound, SoundSystem* _soundSystem);


        //////////////////////////////////////////
        void generateALObjects();

        //////////////////////////////////////////
        void deleteALObjects();


        //////////////////////////////////////////
        void bindSound(MZALuint _bufferID);

        //////////////////////////////////////////
        virtual void updateVolume() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void updatePitch() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void updateLooped() MAZE_OVERRIDE;

    protected:
        MZALuint m_sourceID = 0;

    private:
        static SoundSourceOpenAL* s_sourcesList;
        SoundSourceOpenAL* m_sourcesListNext = nullptr;
        SoundSourceOpenAL* m_sourcesListPrev = nullptr;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundSourceOpenAL_hpp_
//////////////////////////////////////////
