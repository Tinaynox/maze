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
#if (!defined(_MazeRenderSystemOpenGL3_hpp_))
#define _MazeRenderSystemOpenGL3_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl3/MazeRenderSystemOpenGL3Header.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystemOpenGL3);
    MAZE_USING_SHARED_PTR(ContextOpenGL);


    //////////////////////////////////////////
    // Class RenderSystemOpenGL3
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL3_API RenderSystemOpenGL3 
        : public RenderSystemOpenGL
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderSystemOpenGL3, RenderSystem);

    public:

        //////////////////////////////////////////
        virtual ~RenderSystemOpenGL3();

        //////////////////////////////////////////
        static RenderSystemOpenGL3Ptr Create(
            RenderSystemOpenGLConfig const& _config = RenderSystemOpenGLConfig());

        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setDefaultRenderContext(const ContextOpenGLPtr& _context) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void clearCurrentRenderTarget(
            bool _colorBuffer = true,
            bool _depthBuffer = true) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderSystemOpenGL3();

        //////////////////////////////////////////
        virtual bool init(RenderSystemOpenGLConfig const& _config) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void assignGLFunctions(ContextOpenGLPtr const& _context);

        ////////////////////////////////////
        bool setupSystem(ContextOpenGLPtr const& _context);
    
        //////////////////////////////////////////
        virtual bool processRenderWindowCreated(RenderWindowPtr const& _renderWindow) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool processRenderWindowDestroyed(RenderWindow* _renderWindow) MAZE_OVERRIDE;

    protected:
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemOpenGL3_hpp_
//////////////////////////////////////////
