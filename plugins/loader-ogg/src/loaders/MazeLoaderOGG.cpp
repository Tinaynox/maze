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
#include "MazeLoaderOGGHeader.hpp"
#include "maze-plugin-loader-ogg/loaders/MazeLoaderOGG.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_OGG_API bool LoadOGG(AssetFilePtr const& _file, SoundDataPtr& _soundData)
    {
        ByteBufferPtr fileData = _file->readAsByteBuffer();
        return LoadOGG(fileData, _soundData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_OGG_API bool LoadOGG(ByteBufferPtr const& _fileData, SoundDataPtr& _soundData)
    {

        return false;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_OGG_API bool IsOGGFile(AssetFilePtr const& _file)
    {
        // #TODO:
        ByteBufferPtr fileData = _file->readAsByteBuffer();
        return IsOGGFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_OGG_API bool IsOGGFile(ByteBufferPtr const& _fileData)
    {
        return false;
    }

} // namespace Maze
//////////////////////////////////////////
