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
#include "MazeLoaderPNGHeader.hpp"
#include "maze-plugin-loader-png/loaders/MazeLoaderPNG.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include <png.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_PNG_API bool LoadPNG(AssetFile const& _file, Vector<PixelSheet2D>& _pixelSheets)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadPNG(fileData, _pixelSheets);
    }

    //////////////////////////////////////////
    #define PNGSIGSIZE (8)

    //////////////////////////////////////////
    #define RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va)        \
        (U32)(((U32)((U8)(vr) * ((U8)(va) + 1)) >> 8) |  \
        ((U32)((U8)(vg) * ((U8)(va) + 1) >> 8) << 8)  |  \
        ((U32)((U8)(vb) * ((U8)(va) + 1) >> 8) << 16) |  \
        ((U32)(U8)(va) << 24))

    using ImageSource =
        struct
        {
            U8* data;
            png_size_t size;
            png_size_t offset;
        };
    

    //////////////////////////////////////////
    static void ReadPNGBlock(png_structp _pngStruct, png_bytep _data, png_size_t _length)
    {
        ImageSource* isource = (ImageSource*)png_get_io_ptr(_pngStruct);

        if ((S32)(isource->offset + _length) <= isource->size)
        {
            memcpy(_data, isource->data + isource->offset, _length);
            isource->offset += _length;
        }
        else
        {
            MAZE_ERROR("pngReaderCallback failed!");
        }
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_PNG_API bool LoadPNG(ByteBuffer const& _fileData, Vector<PixelSheet2D>& _pixelSheets)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");
        
        bool ret = false;
        png_byte header[PNGSIGSIZE] = {0};
        png_structp pngStruct = 0;
        png_infop info = 0;

        do
        {
            // PNG header len is 8 bytes
            MAZE_ERROR_RETURN_VALUE_IF(_fileData.getSize() < PNGSIGSIZE, false, "Corrupted PNG!");

            // Check the data is png or not
            memcpy(header, _fileData.getData(), PNGSIGSIZE);
            MAZE_ERROR_RETURN_VALUE_IF(png_sig_cmp(header, 0, PNGSIGSIZE), false, "This texture is not actually PNG!");

            // Init png_struct
            pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
            if (!pngStruct)
                break;

            // Init png_info
            info = png_create_info_struct(pngStruct);
            if (!info)
                break;

            if (setjmp(png_jmpbuf(pngStruct)))
                break;

            // Set the read call back function
            ImageSource imageSource;
            imageSource.data = _fileData.getDataPointer();
            imageSource.size = _fileData.getSize();
            imageSource.offset = 0;
            png_set_read_fn(pngStruct, &imageSource, ReadPNGBlock);

            // Read png file info
            png_read_info(pngStruct, info);

            U32 width = png_get_image_width(pngStruct, info);
            U32 height = png_get_image_height(pngStruct, info);
            U32 bpp = png_get_bit_depth(pngStruct, info);
            png_uint_32 colorType = png_get_color_type(pngStruct, info);

            // Force palette images to be expanded to 24-bit RGB
            // it may include alpha channel
            if (colorType == PNG_COLOR_TYPE_PALETTE)
            {
                png_set_palette_to_rgb(pngStruct);
            }

            // Low-bit-depth grayscale images are to be expanded to 8 bits
            if (colorType == PNG_COLOR_TYPE_GRAY && bpp < 8)
            {
                png_set_expand_gray_1_2_4_to_8(pngStruct);
            }

            // Expand any tRNS chunk data into a full alpha channel
            if (png_get_valid(pngStruct, info, PNG_INFO_tRNS))
            {
                png_set_tRNS_to_alpha(pngStruct);
            }

            // Reduce images with 16-bit samples to 8 bits
            if (bpp == 16)
            {
                png_set_strip_16(pngStruct);
            }

            // Expand grayscale images to RGB
            if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
            {
                png_set_gray_to_rgb(pngStruct);
            }

            png_size_t rowbytes;
            png_bytep* rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
            if (!rowPointers)
                break;

            png_read_update_info(pngStruct, info);

            rowbytes = png_get_rowbytes(pngStruct, info);

            png_size_t bytesPerPixel = rowbytes / width;

            PixelFormat::Enum pixelFormat = PixelFormat::Unknown;
            
            switch (bytesPerPixel)
            {
            case 3:
                pixelFormat = PixelFormat::RGB_U8;
                break;

            case 4:
                pixelFormat = PixelFormat::RGBA_U8;
                break;

            default:
                MAZE_ERROR("Unsupported PNG format!");
                break;
            }

            _pixelSheets.resize(1);
            PixelSheet2D& pixelSheet = _pixelSheets[0];
            pixelSheet.setSize(width, height);
            pixelSheet.setFormat(pixelFormat);

            for (U32 i = 0; i < height; ++i)
            {
                rowPointers[i] = pixelSheet.getDataPointer() + i * rowbytes;
            }

            png_read_image(pngStruct, rowPointers);

            png_read_end(pngStruct, 0);

            pixelSheet.flipY();

            if (rowPointers)
                free(rowPointers);

            ret = true;
        }
        while (0);

        if (pngStruct)
        {
            png_destroy_read_struct(&pngStruct, (info) ? &info : 0, 0);
        }

        return ret;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_PNG_API bool IsPNGFile(AssetFile const& _file)
    {
        // #TODO:
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return IsPNGFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_PNG_API bool IsPNGFile(ByteBuffer const& _fileData)
    {
        png_byte header[PNGSIGSIZE] = {0};

        if (_fileData.getSize() < PNGSIGSIZE)
            return false;

        // Check the data is png or not
        memcpy(header, _fileData.getDataPointer(), PNGSIGSIZE);
        if (png_sig_cmp(header, 0, PNGSIGSIZE))
            return false;

        return true;
    }

} // namespace Maze
//////////////////////////////////////////
