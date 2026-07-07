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
#if (!defined(_MazeSoundStream_hpp_))
#define _MazeSoundStream_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundStream);


    //////////////////////////////////////////
    // Class SoundStream
    //
    // Format-agnostic incremental PCM decoder used for streamed (not fully
    // decoded to memory) sound playback. One instance is a single live decode
    // cursor - it is not meant to be shared across simultaneous playbacks of
    // the same sound.
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundStream
    {
    public:

        //////////////////////////////////////////
        virtual ~SoundStream() {}

        //////////////////////////////////////////
        virtual S32 getChannels() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual S32 getFrequency() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual S32 getBitsPerSample() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual F32 getLength() const MAZE_ABSTRACT;

        //////////////////////////////////////////
        // Decodes up to _maxSampleFrames interleaved sample-frames into _buffer.
        // Returns the number of frames actually written; 0 means end of stream.
        virtual Size getSamplesInterleaved(S16* _buffer, Size _maxSampleFrames) MAZE_ABSTRACT;

        //////////////////////////////////////////
        // Rewinds the decode cursor back to the start of the stream.
        virtual void seekStart() MAZE_ABSTRACT;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundStream_hpp_
//////////////////////////////////////////
