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
#if (!defined(_MazeRenderWindowParams_hpp_))
#define _MazeRenderWindowParams_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystem);


    //////////////////////////////////////////
    // Struct RenderWindowParams
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API RenderWindowParams
    {
        ////////////////////////////////////
        inline RenderWindowParams(
            RenderSystemPtr const& _renderSystem = RenderSystemPtr(),
            WindowParamsPtr _windowParams = WindowParamsPtr(),
            S32 _antialiasingLevel = 0)
            : renderSystem(_renderSystem)
            , windowParams(_windowParams)
            , antialiasingLevel(_antialiasingLevel)
        {
        }

        ////////////////////////////////////
        inline WindowParamsPtr const& EnsureWindowParams()
        {
            if (!windowParams)
                windowParams = WindowParams::Create();

            return windowParams;
        }

        RenderSystemPtr renderSystem;
        WindowParamsPtr windowParams;
        S32 antialiasingLevel;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderWindowParams_hpp_
//////////////////////////////////////////
