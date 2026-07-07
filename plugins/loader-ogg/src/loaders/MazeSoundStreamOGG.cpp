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
#include "MazeLoaderOGGHeader.hpp"
#include "maze-plugin-loader-ogg/loaders/MazeSoundStreamOGG.hpp"
#include "maze-core/math/MazeMath.hpp"
#include <stb_vorbis.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class SoundStreamOGG
    //
    //////////////////////////////////////////
    SoundStreamOGG::SoundStreamOGG()
    {
    }

    //////////////////////////////////////////
    SoundStreamOGG::~SoundStreamOGG()
    {
        if (m_vorbis)
            stb_vorbis_close(m_vorbis);
    }

    //////////////////////////////////////////
    SoundStreamOGGPtr SoundStreamOGG::Create(ByteBufferPtr const& _fileData)
    {
        SoundStreamOGGPtr stream;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SoundStreamOGG, stream, init(_fileData));
        return stream;
    }

    //////////////////////////////////////////
    bool SoundStreamOGG::init(ByteBufferPtr const& _fileData)
    {
        MAZE_PROFILE_EVENT("SoundStreamOGG::init");

        // Vorbis decodes directly from this buffer as it streams, so it must be
        // kept alive for the lifetime of the stream (unlike a full-decode loader,
        // which only needs the compressed bytes for the duration of one call).
        m_fileData = _fileData;

        S32 error = -1;
        m_vorbis = stb_vorbis_open_memory(
            m_fileData->getDataRW(),
            (S32)m_fileData->getSize(),
            &error,
            nullptr);

        if (!m_vorbis || error != VORBIS__no_error)
            return false;

        stb_vorbis_info info = stb_vorbis_get_info(m_vorbis);
        m_channels = info.channels;
        m_frequency = info.sample_rate;

        S32 samples = stb_vorbis_stream_length_in_samples(m_vorbis);
        m_length = (F32)samples / (F32)Math::Max(m_frequency, 1);

        return true;
    }

    //////////////////////////////////////////
    S32 SoundStreamOGG::getChannels() const
    {
        return m_channels;
    }

    //////////////////////////////////////////
    S32 SoundStreamOGG::getFrequency() const
    {
        return m_frequency;
    }

    //////////////////////////////////////////
    S32 SoundStreamOGG::getBitsPerSample() const
    {
        return 16;
    }

    //////////////////////////////////////////
    F32 SoundStreamOGG::getLength() const
    {
        return m_length;
    }

    //////////////////////////////////////////
    Size SoundStreamOGG::getSamplesInterleaved(S16* _buffer, Size _maxSampleFrames)
    {
        S32 got = stb_vorbis_get_samples_short_interleaved(
            m_vorbis, m_channels, _buffer, (S32)(_maxSampleFrames * m_channels));
        return (Size)got;
    }

    //////////////////////////////////////////
    void SoundStreamOGG::seekStart()
    {
        stb_vorbis_seek_start(m_vorbis);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_OGG_API SoundStreamPtr OpenOGGStream(ByteBufferPtr const& _fileData)
    {
        SoundStreamOGGPtr stream = SoundStreamOGG::Create(_fileData);
        if (!stream)
            return SoundStreamPtr();

        return stream;
    }

} // namespace Maze
//////////////////////////////////////////
