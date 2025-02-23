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
#if (!defined(_MazeLoaderMZMESH_hpp_))
#define _MazeLoaderMZMESH_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/system/MazePath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Mesh);

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadMZMESH(
        AssetFile const& _file,
        Mesh& _mesh,
        MeshLoaderProperties const& _props);

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool LoadMZMESH(
        ByteBuffer const& _fileData,
        Mesh& _mesh,
        MeshLoaderProperties const& _props);

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsMZMESHFile(AssetFile const& _file);

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool IsMZMESHFile(ByteBuffer const& _fileData);

    //////////////////////////////////////////
    MAZE_GRAPHICS_API bool SaveMZMESH(
        Mesh const& _mesh,
        Path _filePath);

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLoaderMZMESH_hpp_
//////////////////////////////////////////
