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
#include "MazeLoaderTTFHeader.hpp"
#include "maze-plugin-loader-ttf/loaders/MazeLoaderTTF.hpp"
#include "maze-plugin-loader-ttf/fonts/MazeTrueTypeFontFreetype.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TTF_API bool LoadTTF(AssetFilePtr const& _file, TrueTypeFontPtr& _font)
    {
        _font = TrueTypeFontFreetype::Create(_file);
        return _font != nullptr;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TTF_API bool LoadTTF(ByteBufferPtr const& _fileData, TrueTypeFontPtr& _font)
    {
        _font = TrueTypeFontFreetype::Create(*_fileData.get());
        return _font != nullptr;
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TTF_API bool IsTTFFile(AssetFilePtr const& _file)
    {
        // #TODO:
        ByteBufferPtr fileData = _file->readAsByteBuffer();
        return IsTTFFile(fileData);
    }

    //////////////////////////////////////////
    MAZE_PLUGIN_LOADER_TTF_API bool IsTTFFile(ByteBufferPtr const& _fileData)
    {
        // #TODO:
        return true;
    }

} // namespace Maze
//////////////////////////////////////////
