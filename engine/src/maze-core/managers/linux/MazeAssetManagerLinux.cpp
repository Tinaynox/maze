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
#include "MazeCoreHeader.hpp"
#include "maze-core/managers/linux/MazeAssetManagerLinux.hpp"
#include "maze-core/system/MazeDisplay.hpp"
#include "maze-core/helpers/linux/MazeWindowHelperLinux.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AssetManagerLinux
    //
    //////////////////////////////////////////
    AssetManagerLinux::AssetManagerLinux()
    {
    }

    //////////////////////////////////////////
    AssetManagerLinux::~AssetManagerLinux()
    {
    }

    //////////////////////////////////////////
    bool AssetManagerLinux::init(DataBlock const& _config)
    {
        if (!AssetManager::init(_config))
            return false;


        return true;
    }

    //////////////////////////////////////////
    Path AssetManagerLinux::getDefaultAssetsDirectory()
    {
        return FileHelper::GetBinaryDirectory();
    }


} // namespace Maze
//////////////////////////////////////////
