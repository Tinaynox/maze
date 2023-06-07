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
#include "maze-graphics/helpers/win/MazeSystemGraphicsHelperWin.hpp"
#include "maze-graphics/helpers/MazePixelSheet2DHelper.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace SystemGraphicsHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool LoadPixelSheet2DToClipboard(PixelSheet2D const& _value)
        {
            MAZE_WARNING_RETURN_VALUE_IF(_value.getFormat() != PixelFormat::RGBA_U8, false, "Unsupported pixelsheet format - %d!", _value.getFormat());

            // Convert RGBA to BGRA
            PixelSheet2D bgraImage = _value;
            PixelSheet2DHelper::ConvertRGBToBGR(bgraImage);
            

            BITMAPINFO bitmapInfo = { 0 };
            bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bitmapInfo.bmiHeader.biWidth = (S32)bgraImage.getWidth();
            bitmapInfo.bmiHeader.biHeight = (S32)bgraImage.getHeight();
            bitmapInfo.bmiHeader.biPlanes = 1;
            bitmapInfo.bmiHeader.biBitCount = 32;
            bitmapInfo.bmiHeader.biCompression = BI_RGB;
            bitmapInfo.bmiHeader.biSizeImage = 0;

            HDC hdc = GetDC(NULL);
            void* bitmapPointer = nullptr;
            HBITMAP hBitmap = CreateDIBitmap(hdc, &(bitmapInfo.bmiHeader), CBM_INIT, bgraImage.getDataPointer(), &bitmapInfo, DIB_RGB_COLORS);
            ReleaseDC(NULL, hdc);

            if (hBitmap == NULL)
                return false;

            if (!OpenClipboard(NULL))
            {
                DeleteObject(hBitmap);
                return false;
            }
            else
            {
                if (!EmptyClipboard() || SetClipboardData(CF_BITMAP, hBitmap) == NULL)
                {
                    CloseClipboard();
                    DeleteObject(hBitmap);
                    return false;
                }
            }

            CloseClipboard();
            DeleteObject(hBitmap);
            return true;
        }


    } // namespace SystemGraphicsHelperWin
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
