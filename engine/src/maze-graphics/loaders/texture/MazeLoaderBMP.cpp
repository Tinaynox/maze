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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderBMP.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/helpers/MazePixelSheet2DHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    #pragma pack(push,1)
    struct MAZE_GRAPHICS_API _BITMAPFILEHEADER
    {
        unsigned short  bfType;
        unsigned int    bfSize;
        unsigned short  bfReserved1;
        unsigned short  bfReserved2;
        unsigned int    bfOffBits;
    };
    #pragma pack(pop)
    

    //////////////////////////////////////////
    #pragma pack(push,1)
    struct MAZE_GRAPHICS_API _BITMAPINFOHEADER
    {
        unsigned int    biSize;
        int             biWidth;
        int             biHeight;
        unsigned short  biPlanes;
        unsigned short  biBitCount;
        unsigned int    biCompression;
        unsigned int    biSizeImage;
        int             biXPelsPerMeter;
        int             biYPelsPerMeter;
        unsigned int    biClrUsed;
        unsigned int    biClrImportant;
    };
    #pragma pack(pop)


    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool CheckSize(S32 _dimension);

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool FlipBitmap(U8* _image, S32 _bytesPerLine, S32 _height);


    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadBMP(AssetFile const& _file, Vector<PixelSheet2D>& _pixelSheets)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadBMP(fileData, _pixelSheets);
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadBMP(ByteBuffer const& _fileData, Vector<PixelSheet2D>& _pixelSheets)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");
        
        _BITMAPFILEHEADER bitmapFileHeader;
        U32 bufferShift = 0;
        memcpy(&bitmapFileHeader, _fileData.getData() + bufferShift, sizeof(_BITMAPFILEHEADER));
        bufferShift += sizeof(_BITMAPFILEHEADER);
        MAZE_ERROR_RETURN_VALUE_IF(bitmapFileHeader.bfType != 0x4D42, false, "Is not a valid Bitmap!");

        // Information
        _BITMAPINFOHEADER bitmapInfoHeader;
        memcpy(&bitmapInfoHeader, _fileData.getData() + bufferShift, sizeof(_BITMAPINFOHEADER));
        bufferShift += sizeof(_BITMAPINFOHEADER);
        
        bufferShift = bitmapFileHeader.bfOffBits;

        _pixelSheets.resize(1);
        PixelSheet2D& pixelSheet = _pixelSheets[0];
        pixelSheet.setSize(bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight);
        
        S32 bpp = bitmapInfoHeader.biBitCount;
        S32 bytesPP = bpp / 8;

        S32 imageSize = bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * bytesPP;
        
        switch (bpp)
        {
            case 8:
            {
                pixelSheet.setFormat(PixelFormat::R_U8);
                break;
            }
            case 16:
            {
                pixelSheet.setFormat(PixelFormat::RGBA_U5_5_5_1);
                break;
            }
            case 24:
            {
                pixelSheet.setFormat(PixelFormat::RGB_U8);
                break;
            }
            case 32:
            {
                pixelSheet.setFormat(PixelFormat::RGBA_U8);
                break;
            }
            default:
            {
                pixelSheet.setFormat(PixelFormat::Unknown);
                MAZE_NOT_IMPLEMENTED;
            }
        }

        MAZE_ERROR_RETURN_VALUE_IF((Size)imageSize > pixelSheet.getDataSize(), false, "Invalid buffer size!");

        memcpy(pixelSheet.getDataPointer(), _fileData.getData() + bufferShift, imageSize);
            
        if (bpp >= 24)
        {
            // Convert BGR to RGB
            PixelSheet2DHelper::ConvertRGBToBGR(pixelSheet);
        }
        else
        if (bpp == 16)
        {
            // Convert A1_RGB5 to RGB5_A1
            PixelSheet2DHelper::ConvertA1RGB5ToRGB5A1(pixelSheet);
        }

        return true;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsBMPFile(AssetFile const& _file)
    {
        ByteBuffer fileData;
        _file.readHeaderToByteBuffer(fileData, sizeof(_BITMAPFILEHEADER));
        return IsBMPFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsBMPFile(ByteBuffer const& _fileData)
    {
        _BITMAPFILEHEADER bitmapFileHeader;
        
        if (_fileData.getSize() < sizeof(_BITMAPFILEHEADER))
            return false;

        memcpy(&bitmapFileHeader, _fileData.getDataPointer(), sizeof(_BITMAPFILEHEADER));
    
        if (bitmapFileHeader.bfType != 0x4D42)
            return false;

        return true;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool CheckSize(S32 _dimension)
    {
#if (MAZE_ENDIAN == MAZE_ENDIAN_BIG)
#    MAZE_NOT_IMPLEMENTED;
#endif

        S32 i = 1;
        while (i < _dimension)
        {
            i <<= 1;
            if (i == _dimension)
                return true;
        }

        return false;
    }

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool FlipBitmap(U8* _image, S32 _bytesPerLine, S32 _height)
    {
        U8* buffer;
        S32 index;

        if (!(buffer = (U8*)malloc(_bytesPerLine * _height)))
            return false;

        memcpy(buffer, _image, _bytesPerLine * _height);

        for (index = 0; index < _height; ++index)
        {
            memcpy(
                &_image[((_height - 1) - index) * _bytesPerLine],
                &buffer[index * _bytesPerLine],
                _bytesPerLine);
        }

        free(buffer);

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
