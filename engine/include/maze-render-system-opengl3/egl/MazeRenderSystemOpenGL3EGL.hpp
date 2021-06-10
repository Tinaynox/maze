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
#if (!defined(_MazeRenderSystemOpenGL3EGL_hpp_))
#define _MazeRenderSystemOpenGL3EGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL3EGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class RenderSystemOpenGL3EGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API RenderSystemOpenGL3EGL 
        : public RenderSystemOpenGL3
    {
    public:
    
        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderSystemOpenGL3EGL, RenderSystemOpenGL3);

        //////////////////////////////////////////
        friend class RenderSystemOpenGL3;


    public:

        //////////////////////////////////////////
        virtual ~RenderSystemOpenGL3EGL();

        //////////////////////////////////////////
        static RenderSystemOpenGL3EGLPtr Create(RenderSystemOpenGLConfig const& _config);

        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;

        
        //////////////////////////////////////////
        virtual ContextOpenGLPtr createContextImpl(
            const WindowPtr& _window,
            ContextOpenGLConfig* _config = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual ContextOpenGLPtr createContextImpl(
            U32 _width = 1, U32 _height = 1,
            ContextOpenGLConfig* _config = nullptr) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderSystemOpenGL3EGL();

        //////////////////////////////////////////
        virtual bool init(RenderSystemOpenGLConfig const& _config) MAZE_OVERRIDE;
    
        //////////////////////////////////////////
        virtual void assignGLFunctions(ContextOpenGLPtr const& _context) MAZE_OVERRIDE;

    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemOpenGL3EGL_hpp_
//////////////////////////////////////////
