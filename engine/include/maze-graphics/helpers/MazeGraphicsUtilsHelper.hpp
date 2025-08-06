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
#if (!defined(_MazeGraphicsUtilsHelper_hpp_))
#define _MazeGraphicsUtilsHelper_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazeFrustum.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);

    //////////////////////////////////////////
    namespace GraphicsUtilsHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D const& GetAsciiSymbolsSheet8x8();

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void ConstructAsciiSymbolsSheet8x8();

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D GenerateSystemFontExtrude(
            PixelSheet2D const& _inSheet,
            S8 columns,
            S8 rows,
            S8 charWidth,
            S8 charHeight);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D GenerateSystemFontExtrudeOutlined(
            PixelSheet2D const& _inSheet,
            S8 columns,
            S8 rows,
            S8 charWidth,
            S8 charHeight,
            ColorU32 const& _outlineColor);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool SaveMeshTangentsToFile(
            Mesh const& _mesh,
            Path const& _filePath);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool LoadMeshTangentsFromFile(
            Mesh& _mesh,
            Path const& _filePath);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API bool LoadMeshTangentsFromBuffer(
            Mesh& _mesh,
            ByteBuffer const& _byteBuffer);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void CalculateCameraFrustum(
            TMat const& _viewMatrix,
            Mat4F const& _projectionMatrix,
            Frustum& _outFrustum);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API void CalculateCameraFrustum(
            Mat4F const& _viewProjectionMatrix,
            Frustum& _outFrustum);

    } // namespace GraphicsUtilsHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeGraphicsUtilsHelper_hpp_
//////////////////////////////////////////
