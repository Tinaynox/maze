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
#if (!defined(_MazeSoundData_hpp_))
#define _MazeSoundData_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SoundData);


    //////////////////////////////////////////
    // Class SoundData
    //
    //////////////////////////////////////////
    class MAZE_SOUND_API SoundData
    {
    public:

        //////////////////////////////////////////
        SoundData(
            ByteBufferPtr _data = ByteBufferPtr(),
            S32 _channels = 0,
            S32 _frequency = 0,
            S32 _bitsPerSample = 0,
            U32 _channelMask = 0);

        //////////////////////////////////////////
        virtual ~SoundData();


        //////////////////////////////////////////
        inline void setData(ByteBufferPtr const& _value) { m_data = _value; }

        //////////////////////////////////////////
        inline ByteBufferPtr const& getDataRO() const { return m_data; }


        //////////////////////////////////////////
        inline void setChannels(S32 _value) { m_channels = _value; }

        //////////////////////////////////////////
        inline S32 getChannels() const { return m_channels; }


        //////////////////////////////////////////
        inline void setFrequency(S32 _value) { m_frequency = _value; }

        //////////////////////////////////////////
        inline S32 getFrequency() const { return m_frequency; }


        //////////////////////////////////////////
        inline void setBitsPerSample(S32 _value) { m_bitsPerSample = _value; }

        //////////////////////////////////////////
        inline S32 getBitsPerSample() const { return m_bitsPerSample; }


        //////////////////////////////////////////
        inline void setChannelMask(S32 _value) { m_channelMask = _value; }

        //////////////////////////////////////////
        inline S32 getChannelMask() const { return m_channelMask; }

    protected:
        ByteBufferPtr m_data;
        S32 m_channels = 0;
        S32 m_frequency = 0;
        S32 m_bitsPerSample = 0;
        U32 m_channelMask = 0;
    };
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSoundData_hpp_
//////////////////////////////////////////
