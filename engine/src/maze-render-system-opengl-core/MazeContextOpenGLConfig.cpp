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
#include "MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGLConfig.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Struct ContextOpenGLConfig
    //
    //////////////////////////////////////////
    ContextOpenGLConfig::ContextOpenGLConfig(
        U32 _flags,
        U32 _majorVersion,
        U32 _minorVersion,
        U32 _depth,
        U32 _stencil,
        U32 _antialiasingLevel,
        bool _sRgb,
        bool _transparentBackground) 
        : flags(_flags)
        , majorVersion(_majorVersion)
        , minorVersion(_minorVersion)
        , depthBits(_depth)
        , stencilBits(_stencil)
        , antialiasingLevel(_antialiasingLevel)
        , sRgbCapable(_sRgb)
        , transparentBackground(_transparentBackground)
    {
    }
    

} // namespace Maze
//////////////////////////////////////////
