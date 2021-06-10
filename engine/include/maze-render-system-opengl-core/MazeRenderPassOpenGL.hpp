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
#if (!defined(_MazeRenderPassOpenGL_hpp_))
#define _MazeRenderPassOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeRenderPass.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderPassOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class RenderPassOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API RenderPassOpenGL
        : public RenderPass
    {
    public:

        //////////////////////////////////////////
        using RenderPassDeleter = std::function<void(RenderPassOpenGL* _ptr)>;

    public:

        //////////////////////////////////////////
        virtual ~RenderPassOpenGL();

        //////////////////////////////////////////
        static RenderPassOpenGLPtr Create(
            RenderSystem* _renderSystem,
            RenderPassType _passType,
            RenderPassDeleter const& _deleter = DefaultDelete<RenderPassOpenGL>());

        //////////////////////////////////////////
        static RenderPassOpenGLPtr Create(
            RenderPassOpenGLPtr const& _renderPass,
            RenderPassDeleter const& _deleter = DefaultDelete<RenderPassOpenGL>());

        //////////////////////////////////////////
        virtual RenderPassPtr createCopy() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderPassOpenGL();

        //////////////////////////////////////////
        using RenderPass::init;

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            RenderPassType _passType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool init(
            RenderPassOpenGLPtr const& _renderPass);

    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderPassOpenGL_hpp_
//////////////////////////////////////////
