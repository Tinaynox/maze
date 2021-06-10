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
#if (!defined(_MazeExtensionsOpenGL3_hpp_))
#define _MazeExtensionsOpenGL3_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl-core/MazeExtensionsOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ExtensionsOpenGL3);
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL3);


    //////////////////////////////////////////
    // Class ExtensionsOpenGL3
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API ExtensionsOpenGL3 
        : public ExtensionsOpenGL
    {
    public:

        //////////////////////////////////////////
        virtual ~ExtensionsOpenGL3();

    protected:

        //////////////////////////////////////////
        ExtensionsOpenGL3();

        //////////////////////////////////////////
        virtual bool init(ContextOpenGL* _context) MAZE_OVERRIDE;

    
    protected:
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeExtensionsOpenGL3_hpp_
//////////////////////////////////////////
