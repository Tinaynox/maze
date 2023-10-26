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
#include "MazeLoaderTGAHeader.hpp"
#include "maze-plugin-loader-tga/loaders/MazeLoaderTGA.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/helpers/MazePixelSheet2DHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #pragma pack(push, 1)
    struct TGAHeader
    {
        U8 idLength;
        U8 colorMapType;
        U8 imageType;
        U16 colorMapOrigin;
        U16 colorMapLength;
        U8 colorMapDepth;
        U16 xOrigin;
        U16 yOrigin;
        U16 width;
        U16 height;
        U8 bitsPerPixel;
        U8 imageDescriptor;
    };
    #pragma pack(pop)


    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TGA_API bool LoadTGA(AssetFile const& _file, Vector<PixelSheet2D>& _pixelSheets)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadTGA(fileData, _pixelSheets);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TGA_API bool LoadTGA(ByteBuffer const& _fileData, Vector<PixelSheet2D>& _pixelSheets)
    {
        MAZE_PROFILE_EVENT("LoadTGA");

        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");
        
        TGAHeader header;
        U32 bytesRead = _fileData.read(0, &header, sizeof(TGAHeader));
        if (bytesRead < sizeof(TGAHeader))
            return false;

        if (header.idLength > 0 || header.colorMapType != 0)
            return false;

        if (header.imageType != 2 && header.imageType != 10)
            return false;

        // Extract relevant information from the header
        PixelSheet2D pixelSheet;
        pixelSheet.setSize(header.width, header.height);
        S32 channels = header.bitsPerPixel / 8;
        switch (channels)
        {
            case 2:
            {
                pixelSheet.setFormat(PixelFormat::RGBA_U5_5_5_1);
                break;
            }
            case 3:
            {
                pixelSheet.setFormat(PixelFormat::RGB_U8);
                break;
            }
            case 4:
            {
                pixelSheet.setFormat(PixelFormat::RGBA_U8);
                break;
            }
            default:
            {
                MAZE_ERROR_RETURN_VALUE(false, "Unsupported TGA channels count=%d!", channels);
            }
        }

        // Determine if the TGA file is compressed (RLE encoded)
        bool isCompressed = (header.imageType == 10);
        
        // Read and process the pixel data
        if (isCompressed)
        {
            S32 pixelCount = S32(header.width * header.height);
            S32 pixelIndex = 0;

            while (pixelIndex < pixelCount)
            {
                U8 chunkHeader;
                bytesRead += _fileData.read(bytesRead, &chunkHeader, sizeof(chunkHeader));

                if (chunkHeader < 128)
                {
                    // Raw packet: RLE packet with length (chunkHeader + 1)
                    S32 packetLength = chunkHeader + 1;
                    bytesRead += _fileData.read(bytesRead, pixelSheet.getDataPointer() + pixelIndex * channels, packetLength * channels);
                    pixelIndex += packetLength;
                }
                else
                {
                    // RLE packet: Repeated pixel value (chunkHeader - 127) times
                    S32 packetLength = chunkHeader - 127;
                    U8 pixelData[4];
                    bytesRead += _fileData.read(bytesRead, pixelData, channels);

                    for (S32 i = 0; i < packetLength; ++i)
                    {
                        std::copy(pixelData, pixelData + channels, pixelSheet.getDataPointer() + pixelIndex * channels);
                        pixelIndex++;
                    }
                }
            }
        }
        else
        {
            bytesRead += _fileData.read(bytesRead, pixelSheet.getDataPointer(), (U32)pixelSheet.getDataSize());
        }

        if (channels > 2)
        {
            // Convert BGR to RGB
            PixelSheet2DHelper::ConvertRGBToBGR(pixelSheet);
        }
        else
        if (channels == 2)
        {
            // Convert A1_RGB5 to RGB5_A1
            PixelSheet2DHelper::ConvertA1RGB5ToRGB5A1(pixelSheet);
        }

        _pixelSheets.resize(1);
        _pixelSheets[0] = std::move(pixelSheet);

        return true;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TGA_API bool IsTGAFile(AssetFile const& _file)
    {
        ByteBuffer fileData;
        _file.readHeaderToByteBuffer(fileData, sizeof(TGAHeader));
        return IsTGAFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TGA_API bool IsTGAFile(ByteBuffer const& _fileData)
    {
        TGAHeader header;
        if (_fileData.read(0, &header, sizeof(TGAHeader)) < sizeof(TGAHeader))
            return false;

        if (header.idLength > 0 || header.colorMapType != 0)
            return false;

        if (header.imageType != 2 && header.imageType != 10)
            return false;

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
