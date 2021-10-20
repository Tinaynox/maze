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
#if (!defined(_MazeContextOpenGLConfig_hpp_))
#define _MazeContextOpenGLConfig_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace ContextOpenGLFlags
    {
        enum Enum
        {
            Default             = 0,
            Core                = MAZE_BIT(0),
            Debug               = MAZE_BIT(1),
            EmbeddedSystems     = MAZE_BIT(2)
        };
    }


    //////////////////////////////////////////
    // Struct ContextOpenGLConfig
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_OPENGL_CORE_API ContextOpenGLConfig
    {
        //////////////////////////////////////////
        explicit ContextOpenGLConfig(
            U32 _flags = ContextOpenGLFlags::Default,
            U32 _majorVersion = 0,
            U32 _minorVersion = 0,
            U32 _depth = 8,
            U32 _stencil = 8,
            U32 _antialiasingLevel = 4,
            bool _sRgb = false,
            bool _transparentBackground = false);

        U32 flags;
        U32 majorVersion;
        U32 minorVersion;
        U32 depthBits;
        U32 stencilBits;
        U32 antialiasingLevel;
        bool sRgbCapable;
        bool transparentBackground;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeContextOpenGLConfig_hpp_
//////////////////////////////////////////
