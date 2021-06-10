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
#if (!defined(_MazeRenderPassOpenGLPool_hpp_))
#define _MazeRenderPassOpenGLPool_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-graphics/MazeRenderPass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderPassOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class RenderPassOpenGLPool
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API RenderPassOpenGLPool
    {
    public:

        //////////////////////////////////////////
        RenderPassOpenGLPool(RenderSystem* _renderSystem);

        //////////////////////////////////////////
        ~RenderPassOpenGLPool();

        //////////////////////////////////////////
        RenderPassOpenGLPtr createRenderPassOpenGL(RenderPassType _renderPassType = RenderPassType::Default);
    
        //////////////////////////////////////////
        void clear();

    protected:

        //////////////////////////////////////////
        void releaseRenderPassOpenGL(
            RenderPassOpenGL* _ptr,
            RenderPassType _renderPassType);

    protected:
        RenderSystem* m_renderSystem;

        Mutex m_mutex;

        Stack<RenderPassOpenGL*> m_renderMeshPools[(Size)RenderPassType::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderPassOpenGLPool_hpp_
//////////////////////////////////////////
