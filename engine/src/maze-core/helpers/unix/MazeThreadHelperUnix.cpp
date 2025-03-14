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
// subject to the follounixg restrictions:
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
#include "maze-core/helpers/unix/MazeThreadHelperUnix.hpp"
#include "maze-core/helpers/MazeThreadHelper.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace ThreadHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API void SleepCurrentThread(U32 _ms)
        {
            MAZE_NOT_IMPLEMENTED;
        }
        
        //////////////////////////////////////////
        MAZE_CORE_API void SetCurrentThreadPriority(ThreadPriority _priority)
        {
            MAZE_NOT_IMPLEMENTED;
        }

        //////////////////////////////////////////
        MAZE_CORE_API ThreadPriority GetCurrentThreadPriority()
        {
            MAZE_NOT_IMPLEMENTED;
            return ThreadPriority::None;
        }

    } // namespace ThreadHelper
    //////////////////////////////////////////


} // namespace Maze
//////////////////////////////////////////
