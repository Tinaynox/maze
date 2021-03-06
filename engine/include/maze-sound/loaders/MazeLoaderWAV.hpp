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
#if (!defined(_MazeLoaderWAV_hpp_))
#define _MazeLoaderWAV_hpp_


//////////////////////////////////////////
#include "maze-sound/MazeSoundHeader.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-sound/MazeSoundData.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_SOUND_API bool LoadWAV(AssetFilePtr const& _file, SoundDataPtr& _soundData);

    //////////////////////////////////////////
    MAZE_SOUND_API bool LoadWAV(ByteBufferPtr const& _fileData, SoundDataPtr& _soundData);

    //////////////////////////////////////////
    MAZE_SOUND_API bool IsWAVFile(AssetFilePtr const& _file);

    //////////////////////////////////////////
    MAZE_SOUND_API bool IsWAVFile(ByteBufferPtr const& _file);
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeLoaderWAV_hpp_
//////////////////////////////////////////
