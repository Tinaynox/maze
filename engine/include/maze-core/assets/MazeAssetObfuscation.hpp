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
#if (!defined(_MazeAssetObfuscation_hpp_))
#define _MazeAssetObfuscation_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/reflection/MazeMetaClass.hpp"
#include "maze-core/utils/MazeSharedObject.hpp"
#include "maze-core/data/MazeByteBuffer.hpp"
#include "maze-core/MazeObject.hpp"
#include <tinyxml2.h>


//////////////////////////////////////////
namespace Maze
{
    ////////////////////////////////////
    MAZE_USING_SHARED_PTR(AssetFile);

    ////////////////////////////////////
    using ObfuscationPasswordFunction = std::function<String(String const&)>;

    ////////////////////////////////////
    MAZE_CORE_API void SetObfuscationPasswordFunction(ObfuscationPasswordFunction _func);

    ////////////////////////////////////
    MAZE_CORE_API String CallObfuscationPasswordFunction(String const&);

    //////////////////////////////////////////
    MAZE_CORE_API void ObfuscateAssetPacks(
        String const& _srcPath,
        String const& _destPath);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeAssetObfuscation_hpp_
//////////////////////////////////////////
