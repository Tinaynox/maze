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
#if (!defined(_MazeExtensionsOpenGL3WGL_hpp_))
#define _MazeExtensionsOpenGL3WGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeExtensionsOpenGL3.hpp"
#include "maze-render-system-opengl3/win/wgl/MazeHeaderOpenGL3WGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ExtensionsOpenGL3WGL);


    //////////////////////////////////////////
    // Class ExtensionsOpenGL3WGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API ExtensionsOpenGL3WGL
        : public ExtensionsOpenGL3
    {
    public:

        //////////////////////////////////////////
        virtual ~ExtensionsOpenGL3WGL();

        //////////////////////////////////////////
        static ExtensionsOpenGL3WGLPtr Create(ContextOpenGL* _context);

        //////////////////////////////////////////
        virtual bool loadGLExtensions() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ExtensionsOpenGL3WGL();

        //////////////////////////////////////////
        virtual bool init(ContextOpenGL* _context) MAZE_OVERRIDE;
    
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeExtensionsOpenGL3WGL_hpp_
//////////////////////////////////////////
