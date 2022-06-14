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
#include "MazeSoundHeader.hpp"
#include "maze-sound/loaders/MazeLoaderWAV.hpp"
#include "maze-sound/managers/MazeSoundManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct WaveFormatTag
    //
    //////////////////////////////////////////
    struct WaveFormatTag
    {
        U16 wormatTag = 0;      // Format type
        U16 channels = 0;       // Number of channels (i.e. mono, stereo, etc.)
        U32 samplesPerSec = 0;  // Sample rate
        U32 avgBytesPerSec = 0; // For buffer estimation
        U16 blockAlign = 0;     // Block size of data
    };

    //////////////////////////////////////////
    #define WAVE_FORMAT_PCM        1
    #define WAVE_FORMAT_EXTENSIBLE 0xFFFE


    //////////////////////////////////////////
    // Enum WaveFileType
    //
    //////////////////////////////////////////
    enum class WaveFileType
    {
        Ex = 1,
        Ext = 2
    };


    //////////////////////////////////////////
    // Enum WaveResult
    //
    //////////////////////////////////////////
    enum class WaveResult
    {
        Ok = 0,
        InvalidFileName = -1,
        BadWaveFile = -2,
        InvalidParam = -3,
        InvalidWaveId = -4,
        NotSupportedYet = -5,
        WaveMustBeMono = -6,
        WaveMustBeWaveFormatPCM = -7,
        WavesMustHaveSameBitResolution = -8,
        WavesMustHaveSameFrequency = -9,
        WavesMustHaveSameBitrate = -10,
        WavesMustHaveSameBlockAlignment = -11,
        OffsetOutOfDataRange = -12,
        FileError = -13,
        OutOfMemory = -14,
        InvalidSpeakerPos = -15,
        InvalidWaveFileType = -16,
        NotWaveFormatExtensibleFormat = -17
    };


    //////////////////////////////////////////
    // Struct WaveFormatEx
    //
    //////////////////////////////////////////
    struct WaveFormatEx
    {
        U16 formatTag = 0;
        U16 channels = 0;
        U32 samplesPerSec = 0;
        U32 avgBytesPerSec = 0;
        U16 blockAlign = 0;
        U16 bitsPerSample = 0;
        U16 size = 0;
    };


    //////////////////////////////////////////
    // Struct PCMWaveFormatTag
    //
    //////////////////////////////////////////
    struct PCMWaveFormatTag
    {
        WaveFormatTag waveFormat;
        U16 bitsPerSample = 0;
    };


    //////////////////////////////////////////
    // Struct WaveGUID
    //
    //////////////////////////////////////////
    struct WaveGUID
    {
        U32 data1 = 0;
        U16 data2 = 0;
        U16 data3 = 0;
        U8 data4[8];
    };


    //////////////////////////////////////////
    // Struct WaveFormatExtensible
    //
    //////////////////////////////////////////
    struct WaveFormatExtensible
    {
        WaveFormatEx format;

        union
        {
            U16 validBitsPerSample; // Bits of precision
            U16 samplesPerBlock;    // Valid if wBitsPerSample==0
            U16 reserved;           // If neither applies, set to zero.
        } samples;

        U32 channelMask = 0;            // which channels are
        WaveGUID subFormat;
    };


    //////////////////////////////////////////
    // Struct WaveFileInfo
    //
    //////////////////////////////////////////
    struct WaveFileInfo
    {
        WaveFileType type;
        WaveFormatExtensible extensible;
        ByteBufferPtr data;
        U32 dataSize = 0;
        U32 dataOffset = 0;
    };


    //////////////////////////////////////////
    // Struct WaveFileHeader
    //
    //////////////////////////////////////////
    #pragma pack(push, 4)
    struct WaveFileHeader
    {
        S8 riff[4];
        U32 riffSize;
        S8 wave[4];
    };


    //////////////////////////////////////////
    // Struct RiffChunk
    //
    //////////////////////////////////////////
    struct RiffChunk
    {
        S8 chunkName[4];
        U32 chunkSize = 0;
    };

    //////////////////////////////////////////
    // Struct WaveFMT
    //
    //////////////////////////////////////////
    struct WaveFMT
    {
        U16 formatTag = 0;
        U16 channels = 0;
        U32 samplesPerSec = 0;
        U32 avgBytesPerSec = 0;
        U16 blockAlign = 0;
        U16 bitsPerSample = 0;
        U16 size = 0;
        U16 reserved = 0;
        U32 channelMask = 0;
        WaveGUID guidSubFormat;
    };
    #pragma pack(pop)


    //////////////////////////////////////////
    WaveResult ParseFile(ByteBufferPtr const& _fileData, WaveFileInfo* waveInfo)
    {
        WaveFileHeader waveFileHeader;
        RiffChunk riffChunk;
        WaveFMT waveFmt;
        WaveResult wr = WaveResult::BadWaveFile;

        if (!_fileData || !waveInfo)
            return WaveResult::InvalidParam;

        // Read Wave file header
        U32 offset = 0;
        offset += _fileData->read(offset, &waveFileHeader, sizeof(WaveFileHeader));

        if (!MAZE_STRNICMP(waveFileHeader.riff, "RIFF", 4) && !MAZE_STRNICMP(waveFileHeader.wave, "WAVE", 4))
        {
            while (U32 bytesRead = _fileData->read(offset, &riffChunk, sizeof(RiffChunk)))
            {
                offset += bytesRead;

                if (!MAZE_STRNICMP(riffChunk.chunkName, "fmt ", 4))
                {
                    if (riffChunk.chunkSize <= sizeof(WaveFMT))
                    {
                        offset += _fileData->read(offset, &waveFmt, riffChunk.chunkSize);

                        // Determine if this is a WaveFormatEx or WaveFormatExtensible wave file
                        if (waveFmt.formatTag == WAVE_FORMAT_PCM)
                        {
                            waveInfo->type = WaveFileType::Ex;
                            memcpy(&waveInfo->extensible.format, &waveFmt, sizeof(PCMWaveFormatTag));
                        }
                        else
                        if (waveFmt.formatTag == WAVE_FORMAT_EXTENSIBLE)
                        {
                            waveInfo->type = WaveFileType::Ext;
                            memcpy(&waveInfo->extensible, &waveFmt, sizeof(WaveFormatExtensible));
                        }
                        else
                        {
                            MAZE_ERROR("WAV ParseFile error - unknown formatTag - %d", waveFmt.formatTag);
                        }
                    }
                    else
                    {
                        offset += riffChunk.chunkSize;
                    }
                }
                else
                if (!MAZE_STRNICMP(riffChunk.chunkName, "data", 4))
                {
                    waveInfo->dataSize = riffChunk.chunkSize;
                    waveInfo->dataOffset = offset;
                    offset += riffChunk.chunkSize;
                }
                else
                {
                    offset += riffChunk.chunkSize;
                }

                // Ensure that we are correctly aligned for next chunk
                if (riffChunk.chunkSize & 1)
                    offset += 1;
            }

            if (waveInfo->dataSize && waveInfo->dataOffset && ((waveInfo->type == WaveFileType::Ex) || (waveInfo->type == WaveFileType::Ext)))
                wr = WaveResult::Ok;
        }

        return wr;
    }

    //////////////////////////////////////////
    MAZE_SOUND_API bool LoadWAV(AssetFilePtr const& _file, SoundDataPtr& _soundData)
    {
        ByteBufferPtr fileData = _file->readAsByteBuffer();
        return LoadWAV(fileData, _soundData);
    }

    //////////////////////////////////////////
    MAZE_SOUND_API bool LoadWAV(ByteBufferPtr const& _fileData, SoundDataPtr& _soundData)
    {
        WaveResult wr = WaveResult::OutOfMemory;

        WaveFileInfo waveInfo;

        if (!_fileData)
            return false;

        if (S32(wr = ParseFile(_fileData, &waveInfo)) >= 0)
        {
            waveInfo.data = ByteBuffer::Create(waveInfo.dataSize);
            if (waveInfo.data)
            {
                // Seek to start of audio data
                U32 offset = waveInfo.dataOffset;

                // Read Sample Data
                U32 bytesRead = _fileData->read(offset, waveInfo.data->getDataPointer(), waveInfo.dataSize);
                offset += bytesRead;
                if (bytesRead == waveInfo.dataSize)
                {
                    _soundData = std::make_shared<SoundData>(                    
                        waveInfo.data,
                        waveInfo.extensible.format.channels,
                        waveInfo.extensible.format.samplesPerSec,
                        waveInfo.extensible.format.bitsPerSample,
                        waveInfo.extensible.channelMask);

                    wr = WaveResult::Ok;
                }
                else
                {
                    wr = WaveResult::BadWaveFile;
                }
            }
            else
            {
                wr = WaveResult::OutOfMemory;
            }
        }

        return (wr == WaveResult::Ok);
    }

    //////////////////////////////////////////
    MAZE_SOUND_API bool IsWAVFile(AssetFilePtr const& _file)
    {
        WaveFileHeader waveFileHeader;
        RiffChunk riffChunk;
        WaveFMT waveFmt;

        // #TODO: Rework, write data directly to waveFileHeader
        ByteBufferPtr headerByteBuffer = _file->readHeaderAsByteBuffer(sizeof(WaveFileHeader));

        // Read Wave file header
        headerByteBuffer->read(0, &waveFileHeader, sizeof(WaveFileHeader));

        if (!MAZE_STRNICMP(waveFileHeader.riff, "RIFF", 4) && !MAZE_STRNICMP(waveFileHeader.wave, "WAVE", 4))
        {
            return true;
        }

        return false;
    }

    //////////////////////////////////////////
    MAZE_SOUND_API bool IsWAVFile(ByteBufferPtr const& _byteBuffer)
    {
        WaveFileHeader waveFileHeader;
        RiffChunk riffChunk;
        WaveFMT waveFmt;

        // Read Wave file header
        _byteBuffer->read(0, &waveFileHeader, sizeof(WaveFileHeader));

        if (!MAZE_STRNICMP(waveFileHeader.riff, "RIFF", 4) && !MAZE_STRNICMP(waveFileHeader.wave, "WAVE", 4))
        {
            return true;
        }

        return false;
    }

} // namespace Maze
//////////////////////////////////////////
