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
#if (!defined(_MazeSoundStreamOGG_hpp_))
#define _MazeSoundStreamOGG_hpp_


//////////////////////////////////////////
#include "maze-plugin-loader-ogg/MazeLoaderOGGHeader.hpp"
#include "maze-sound/MazeSoundStream.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
struct stb_vorbis;


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundStreamOGG);


    //////////////////////////////////////////
    // Class SoundStreamOGG
    //
    //////////////////////////////////////////
    class MAZE_PLUGIN_LOADER_OGG_API SoundStreamOGG
        : public SoundStream
    {
    public:

        //////////////////////////////////////////
        virtual ~SoundStreamOGG();

        //////////////////////////////////////////
        static SoundStreamOGGPtr Create(ByteBufferPtr const& _fileData);

        //////////////////////////////////////////
        virtual S32 getChannels() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getFrequency() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getBitsPerSample() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getLength() const MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Size getSamplesInterleaved(S16* _buffer, Size _maxSampleFrames) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void seekStart() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        SoundStreamOGG();

        //////////////////////////////////////////
        bool init(ByteBufferPtr const& _fileData);

    protected:
        ByteBufferPtr m_fileData;
        stb_vorbis* m_vorbis = nullptr;
        S32 m_channels = 0;
        S32 m_frequency = 0;
        F32 m_length = 0.0f;
    };

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_OGG_API SoundStreamPtr OpenOGGStream(ByteBufferPtr const& _fileData);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundStreamOGG_hpp_
//////////////////////////////////////////
