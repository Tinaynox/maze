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
#include "MazeLoaderDDSHeader.hpp"
#include "maze-plugin-loader-dds/loaders/MazeLoaderDDS.hpp"
#include "maze-graphics/MazePixelFormat.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    struct D3D_PixelFormat
    {
        S32 dwSize;
        S32 dwFlags;
        S32 dwFourCC;
        S32 dwRGBBitCount;
        S32 dwRBitMask;
        S32 dwGBitMask;
        S32 dwBBitMask;
        S32 dwRGBAlphaBitMask;
    };

    //////////////////////////////////////////
    struct D3D_Caps2
    {
        S32 dwCaps1;
        S32 dwCaps2;
        S32 Reserved[2];
    };

    //////////////////////////////////////////
    struct D3D_SurfaceDesc2
    {
        S32 dwSize;
        S32 dwFlags;
        S32 dwHeight;
        S32 dwWidth;
        S32 dwPitchOrLinearSize;
        S32 dwDepth;
        S32 dwMipMapCount;
        S32 dwReserved1[11];
        D3D_PixelFormat ddpfPixelFormat;
        D3D_Caps2 ddsCaps;
        S32 dwReserved2;
    };

    //////////////////////////////////////////
#define FOURCC(a, b, c, d) \
	    ((U32)((U32)(a) ) | \
	    ((U32)(b) << 8) | \
	    ((U32)(c) << 16) | \
	    ((U32)(d) << 24))

    //////////////////////////////////////////
    using DDS_COMPRESSION_TYPE =
        enum
        {
            DDS_COMPRESS_NONE = 0,
            DDS_COMPRESS_BC1,       // DXT1
            DDS_COMPRESS_BC2,       // DXT3
            DDS_COMPRESS_BC3,       // DXT5
            DDS_COMPRESS_BC3N,      // DXT5n
            DDS_COMPRESS_BC4,       // ATI1
            DDS_COMPRESS_BC5,       // ATI2
            DDS_COMPRESS_AEXP,      // DXT5
            DDS_COMPRESS_YCOCG,     // DXT5
            DDS_COMPRESS_YCOCGS,    // DXT5
            DDS_COMPRESS_MAX
        };

    //////////////////////////////////////////
    using DDS_SAVE_TYPE =
        enum
        {
            DDS_SAVE_SELECTED_LAYER = 0,
            DDS_SAVE_CUBEMAP,
            DDS_SAVE_VOLUMEMAP,
            DDS_SAVE_MAX
        };

    //////////////////////////////////////////
    using DDS_FORMAT_TYPE =
        enum
        {
            DDS_FORMAT_DEFAULT = 0,
            DDS_FORMAT_RGB8,
            DDS_FORMAT_RGBA8,
            DDS_FORMAT_BGR8,
            DDS_FORMAT_ABGR8,
            DDS_FORMAT_R5G6B5,
            DDS_FORMAT_RGBA4,
            DDS_FORMAT_RGB5A1,
            DDS_FORMAT_RGB10A2,
            DDS_FORMAT_R3G3B2,
            DDS_FORMAT_A8,
            DDS_FORMAT_L8,
            DDS_FORMAT_L8A8,
            DDS_FORMAT_AEXP,
            DDS_FORMAT_YCOCG,
            DDS_FORMAT_MAX
        };

    //////////////////////////////////////////
    using DDS_COLOR_TYPE =
        enum
        {
            DDS_COLOR_DEFAULT = 0,
            DDS_COLOR_DISTANCE,
            DDS_COLOR_LUMINANCE,
            DDS_COLOR_INSET_BBOX,
            DDS_COLOR_MAX
        };

    //////////////////////////////////////////
    using DDS_MIPMAP_TYPE =
        enum
        {
            DDS_MIPMAP_DEFAULT = 0,
            DDS_MIPMAP_NEAREST,
            DDS_MIPMAP_BOX,
            DDS_MIPMAP_BILINEAR,
            DDS_MIPMAP_BICUBIC,
            DDS_MIPMAP_LANCZOS,
            DDS_MIPMAP_MAX
        };

    //////////////////////////////////////////
    #define DDS_HEADERSIZE 128

    #define DDSD_CAPS 0x00000001
    #define DDSD_HEIGHT 0x00000002
    #define DDSD_WIDTH 0x00000004
    #define DDSD_PITCH 0x00000008
    #define DDSD_PIXELFORMAT 0x00001000
    #define DDSD_MIPMAPCOUNT 0x00020000
    #define DDSD_LINEARSIZE 0x00080000
    #define DDSD_DEPTH 0x00800000

    #define DDPF_ALPHAPIXELS 0x00000001
    #define DDPF_ALPHA 0x00000002
    #define DDPF_FOURCC 0x00000004
    #define DDPF_PALETTEINDEXED8 0x00000020
    #define DDPF_RGB 0x00000040
    #define DDPF_LUMINANCE 0x00020000

    #define DDSCAPS_COMPLEX 0x00000008
    #define DDSCAPS_TEXTURE 0x00001000
    #define DDSCAPS_MIPMAP 0x00400000

    #define DDSCAPS2_CUBEMAP 0x00000200
    #define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400
    #define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800
    #define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000
    #define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000
    #define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000
    #define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000
    #define DDSCAPS2_VOLUME 0x00200000


    #define MAGIC_DDS 0x20534444      // DDS<space>
    #define MAX_MIPMAP_LEVEL 16

    #define ID_DXT1   0x31545844
    #define ID_DXT3   0x33545844
    #define ID_DXT5   0x35545844


    //////////////////////////////////////////
    static inline void ConvertARGB2RGBA(U8* _a, S32 _n)
    {
        _n /= 4;
        for (S32 i = 0; i < _n; i++, _a += 4)
            std::swap(_a[0], _a[1]);
    }

    //////////////////////////////////////////
    // A DXT1 block layout is:
    // [0-1] color0.
    // [2-3] color1.
    // [4-7] color bitmap, 2 bits per pixel.
    // So each of the 4-7 bytes represents one line, flipping a block is just
    // flipping those bytes.
    // Note that http://src.chromium.org/viewvc/chrome/trunk/src/o3d/core/cross/bitmap_dds.cc?view=markup&pathrev=21227
    // contains an error in the last line: data[6]=data[5] is a bug!
    static void FlipDXT1BlockFull(U8* data)
    {
        std::swap(data[4], data[7]);
        std::swap(data[5], data[6]);
    }

    //////////////////////////////////////////
    // Flips a full DXT3 block in the y direction
    static void FlipDXT3BlockFull(U8* block)
    {
        // A DXT3 block layout is:
        // [0-7]  alpha bitmap, 4 bits per pixel.
        // [8-15] a DXT1 block.

        // We can flip the alpha bits at the byte level (2 bytes per line).
        std::swap(block[0], block[6]);
        std::swap(block[1], block[7]);
        std::swap(block[3], block[5]);

        // And flip the DXT1 block using the above function.
        FlipDXT1BlockFull(block + 8);
    }

    //////////////////////////////////////////
    // From http://src.chromium.org/viewvc/chrome/trunk/src/o3d/core/cross/bitmap_dds.cc?view=markup&pathrev=21227
    // Original source contained bugs; fixed here.
    static void FlipDXT5BlockFull(unsigned char *block)
    {
        // A DXT5 block layout is:
        // [0]    alpha0.
        // [1]    alpha1.
        // [2-7]  alpha bitmap, 3 bits per pixel.
        // [8-15] a DXT1 block.

        // The alpha bitmap doesn't easily map lines to bytes, so we have to
        // interpret it correctly.  Extracted from
        // http://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt :
        //
        //   The 6 "bits" bytes of the block are decoded into one 48-bit integer:
        //
        //     bits = bits_0 + 256 * (bits_1 + 256 * (bits_2 + 256 * (bits_3 +
        //                   256 * (bits_4 + 256 * bits_5))))
        //
        //   bits is a 48-bit unsigned integer, from which a three-bit control code
        //   is extracted for a texel at location (x,y) in the block using:
        //
        //       code(x,y) = bits[3*(4*y+x)+1..3*(4*y+x)+0]
        //
        //   where bit 47 is the most significant and bit 0 is the least
        //   significant bit.

        // From Chromium (source was buggy)
        U32 line_0_1 = block[2] + 256 * (block[3] + 256 * block[4]);
        U32 line_2_3 = block[5] + 256 * (block[6] + 256 * block[7]);
        // swap lines 0 and 1 in line_0_1.
        U32 line_1_0 = ((line_0_1 & 0x000fff) << 12) |
            ((line_0_1 & 0xfff000) >> 12);
        // swap lines 2 and 3 in line_2_3.
        U32 line_3_2 = ((line_2_3 & 0x000fff) << 12) |
            ((line_2_3 & 0xfff000) >> 12);
        block[2] = line_3_2 & 0xff;
        block[3] = (line_3_2 & 0xff00) >> 8;
        block[4] = (line_3_2 & 0xff0000) >> 16;
        block[5] = line_1_0 & 0xff;
        block[6] = (line_1_0 & 0xff00) >> 8;
        block[7] = (line_1_0 & 0xff0000) >> 16;

        // And flip the DXT1 block using the above function.
        FlipDXT1BlockFull(block + 8);
    }


    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_DDS_API bool LoadDDS(AssetFile const& _file, Vector<PixelSheet2D>& _pixelSheets)
    {
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return LoadDDS(fileData, _pixelSheets);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_DDS_API bool LoadDDS(ByteBuffer const& _fileData, Vector<PixelSheet2D>& _pixelSheets)
    {
        MAZE_DEBUG_ERROR_RETURN_VALUE_IF(
            _fileData.getSize() == 0,
            false,
            "File loading error!");
        
        U32 bufferShift = 0;
        U8* temp = nullptr;
        S32 compressionType = 0;

        do
        {

            S32 magic = 0;
            memcpy(&magic, _fileData.getData() + bufferShift, sizeof(S32));
            bufferShift += sizeof(S32);

            MAZE_ERROR_RETURN_VALUE_IF(magic != MAGIC_DDS, false , "This texture is not actually DDS!");

            // Direct3D 9 format
            D3D_SurfaceDesc2 header;
            memcpy(&header, _fileData.getData() + bufferShift, sizeof(D3D_SurfaceDesc2));
            bufferShift += sizeof(D3D_SurfaceDesc2);

            // Remember info for users of this object
            Vec2DS size(header.dwWidth, header.dwHeight);

            S32 mipMapCount = Math::Max(header.dwMipMapCount, 1);
            _pixelSheets.resize(mipMapCount);

            // Number of pixels
            S32 bytes = header.dwHeight * header.dwWidth;

            // Block size default
            S32 blockSize = 16;
            S32 format = 0;

            PixelFormat::Enum pixelFormat = PixelFormat::Unknown;

            // DXT5?
            if (header.ddpfPixelFormat.dwFlags & DDPF_FOURCC)
            {
                // Compressed
                format = DDS_FORMAT_YCOCG;      // ???????? Not right
                U32 fourCC = header.ddpfPixelFormat.dwFourCC;
                if (fourCC == ID_DXT1)
                {
                    blockSize = 8;
                    pixelFormat = PixelFormat::DXT1_RGB;
                }
                else
                if (fourCC == ID_DXT3)
                {
                    pixelFormat = PixelFormat::DXT3_RGBA;
                }
                else
                if (fourCC == ID_DXT5)
                {
                    pixelFormat = PixelFormat::DXT5_RGBA;
                }
                else
                {
                    S8 buf[5];
                    buf[0] = fourCC & 255;
                    buf[1] = (fourCC >> 8) & 255;
                    buf[2] = (fourCC >> 16) & 255;
                    buf[3] = (fourCC >> 24) & 255;
                    buf[4] = 0;
                    MAZE_ERROR("Unknown compressed DDS format (%s)", buf);
                    break;
                }
                // DXT5 compression
                // bytes/=4;
            }
            else if (header.ddpfPixelFormat.dwRGBBitCount == 32)
            {
                format = DDS_FORMAT_RGBA8;
                // Calculate bytes for highest mipmap level
                bytes *= header.ddpfPixelFormat.dwRGBBitCount / 8;
            }
            else
            {
                MAZE_ERROR("Unknown DDS format (rgb bitcount not 32, not DXT5)");
                break;
            }

            // Read all mipmap levels
            S32 w, h;
            w = size.x;
            h = size.y;
            for (S32 i = 0; i < mipMapCount && i < MAX_MIPMAP_LEVEL; i++)
            {
                // Deduce # of bytes
                // Close to the higher mipmap levels, wid or hgt may become 0; keep things at 1
                if (w == 0) w = 1;
                if (h == 0) h = 1;

                if (format == DDS_FORMAT_RGBA8)
                {
                    // RGBA8
                    bytes = w * h * 4;
                }
                else
                {
                    // DXTx
                    bytes = ((w + 3) / 4)*((h + 3) / 4) * blockSize;
                    if (i == 0)
                    {
                        // Create temp buffer to flip DDS
                        temp = (U8*)malloc(bytes);
                    }
                }

                _pixelSheets[i].setSize(w, h);
                _pixelSheets[i].setFormat(pixelFormat);

                // Flip Y if required
                if (format != DDS_FORMAT_RGBA8)
                {
                    // First read in temp buffer
                    memcpy(temp, _fileData.getData() + bufferShift, bytes);
                    bufferShift += bytes;

                    // Flip & copy to actual pixel buffer
                    S32 j;
                    S32 widBytes;
                    S32 k;
                    U8* s;
                    U8* d;
                    widBytes = ((w + 3) / 4) * blockSize;
                    s = temp;
                    d = _pixelSheets[i].getDataPointer() + ((h + 3) / 4 - 1) * widBytes;

                    for (j = 0; j < (h + 3) / 4; j++)
                    {
                        memcpy(d, s, widBytes);
                        if (blockSize == 8)
                        {
                            for (k = 0; k < widBytes / blockSize; k++)
                                FlipDXT1BlockFull(d + k * blockSize);
                        }
                        else
                        {
                            // DXT3, DXT5
                            if (compressionType == DDS_COMPRESS_BC2)
                            {
                                // DXT3
                                for (k = 0; k < widBytes / blockSize; k++)
                                    FlipDXT3BlockFull((U8*)d + k * blockSize);
                            }
                            else
                            {
                                // DXT5
                                for (k = 0; k < widBytes / blockSize; k++)
                                    FlipDXT5BlockFull((U8*)d + k * blockSize);
                            }
                        }
                        s += widBytes;
                        d -= widBytes;
                    }

                }
                else
                {
                    MAZE_ERROR_RETURN_VALUE_IF(
                        (S32)_pixelSheets[i].getDataSize() < bytes,
                        false,
                        "Failed to read DDS to pixel sheet. PixelSheet size=%d dataSize=%d",
                        (S32)_pixelSheets[i].getDataSize(),
                        bytes);
                    memcpy(_pixelSheets[i].getDataPointer(), _fileData.getData() + bufferShift, bytes);
                    bufferShift += bytes;

                    // RGBA8
                    if (format == DDS_FORMAT_RGBA8)
                    {
                        ConvertARGB2RGBA(_pixelSheets[i].getDataPointer(), bytes);
                    }
                }
                // Next level is smaller
                w /= 2;
                h /= 2;
            }
        }
        while (false);

        // Release temp buffer
        if (temp)
            free(temp);

        return true;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_DDS_API bool IsDDSFile(AssetFile const& _file)
    {
        // #TODO:
        ByteBuffer fileData;
        _file.readToByteBuffer(fileData);
        return IsDDSFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_DDS_API bool IsDDSFile(ByteBuffer const& _fileData)
    {
        U8 fileHeaderData[sizeof(S32)];
        _fileData.read(0, fileHeaderData, sizeof(S32));
        S32 magic = *((S32*)&fileHeaderData);

        return magic == MAGIC_DDS;
    }

} // namespace Maze
//////////////////////////////////////////
