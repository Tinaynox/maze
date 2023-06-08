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
#include "MazeLoaderJPGHeader.hpp"
#include "maze-plugin-loader-jpg/loaders/MazeLoaderJPG.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include <jpeglib.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static inline void ProcessJPGError(j_common_ptr cinfo)
    {
        S8 jpegLastErrorMsg[JMSG_LENGTH_MAX];
        (*cinfo->err->format_message)(cinfo, jpegLastErrorMsg);
        MAZE_ERROR("JPEG Error: %s", jpegLastErrorMsg);        
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_JPG_API bool LoadJPG(AssetFile const& _file, Vector<PixelSheet2D>& _pixelSheets)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadJPG(fileData, _pixelSheets);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_JPG_API bool LoadJPG(ByteBuffer const& _fileData, Vector<PixelSheet2D>& _pixelSheets)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");

        _pixelSheets.resize(1);
        PixelSheet2D& pixelSheet = _pixelSheets[0];
        

        // Create JPEG decompression object
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;

        cinfo.err = jpeg_std_error(&jerr);
        jerr.error_exit = ProcessJPGError;
        
        jpeg_create_decompress(&cinfo);
        jpeg_mem_src(&cinfo, _fileData.getDataPointer(), (U32)_fileData.getSize());

        // Read JPEG header
        S32 readHeaderRes = jpeg_read_header(&cinfo, TRUE);
        if (readHeaderRes != JPEG_HEADER_OK)
        {
            jpeg_destroy_decompress(&cinfo);
            MAZE_ERROR_RETURN_VALUE(false, "Invalid JPEG header");
        }

        // Start decompression
        jpeg_start_decompress(&cinfo);

        S32 width = cinfo.output_width;
        S32 height = cinfo.output_height;
        S32 channels = cinfo.output_components;

        if (channels != 3)
        {
            jpeg_finish_decompress(&cinfo);
            jpeg_destroy_decompress(&cinfo);
            MAZE_ERROR_RETURN_VALUE(false, "Unsupported channels count! cinfo.output_components=%d", cinfo.output_components);
        }

        pixelSheet.setFormat(PixelFormat::RGB_U8);
        pixelSheet.setSize(width, height);

        // Read scanlines
        S32 row_stride = width * channels;
        JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

        S32 row = height - 1;
        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, buffer, 1);
            
            memcpy(pixelSheet.getDataPointer() + (row * width * channels), buffer[0], width * channels);
            row--;
        }

        // Finish decompression
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        
        return true;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_JPG_API bool IsJPGFile(AssetFile const& _file)
    {
        // #TODO:
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return IsJPGFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_JPG_API bool IsJPGFile(ByteBuffer const& _fileData)
    {
        U8 header[2];
        _fileData.read(0, header, 2);

        bool isJPEG = (header[0] == 0xFF) && (header[1] == 0xD8);
        return isJPEG;
    }

} // namespace Maze
//////////////////////////////////////////
