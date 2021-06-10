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
#if (!defined(_MazeRenderQueueOpenGL_hpp_))
#define _MazeRenderQueueOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-render-system-opengl-core/instance-stream/MazeInstanceStreamModelMatrixOpenGL.hpp"
#include "maze-render-system-opengl-core/instance-stream/MazeInstanceStreamColorOpenGL.hpp"
#include "maze-render-system-opengl-core/instance-stream/MazeInstanceStreamUVOpenGL.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderQueueOpenGL);
    MAZE_USING_SHARED_PTR(ContextOpenGL);
    

    //////////////////////////////////////////
    // Class RenderQueueOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API RenderQueueOpenGL
        : public RenderQueue
    {
    public:

        //////////////////////////////////////////
        virtual ~RenderQueueOpenGL();

        //////////////////////////////////////////
        static RenderQueueOpenGLPtr Create(
            RenderTarget* _renderTarget,
            ContextOpenGL* _contextOpenGL);

        //////////////////////////////////////////
        ContextOpenGL* getContextOpenGL() const { return m_context; }

        //////////////////////////////////////////
        virtual void draw() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void clear() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderQueueOpenGL();

        //////////////////////////////////////////
        using RenderQueue::init;

        //////////////////////////////////////////
        virtual bool init(
            RenderTarget* _renderTarget,
            ContextOpenGL* _contextOpenGL);


        //////////////////////////////////////////
        void bindRenderPass(RenderPass* _materialPass);

        //////////////////////////////////////////
        inline void bindRenderPass(RenderPassPtr const& _materialPass)
        {
            bindRenderPass(_materialPass.get());
        }

        //////////////////////////////////////////
        void bindVertexArrayObject(VertexArrayObject* _vao);

        //////////////////////////////////////////
        inline void bindVertexArrayObject(VertexArrayObjectPtr const& _vao)
        {
            bindVertexArrayObject(_vao.get());
        }

        //////////////////////////////////////////
        void processDrawBegin();

    protected:
        ContextOpenGL* m_context;

        U32 m_maxInstancesPerDrawCall = 0;
        U32 m_maxInstancesPerDraw = 0;

        Timer m_timer;
        F32 m_drawTime;

        Stack<Rect2DS> m_scissorRects;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderQueueOpenGL_hpp_
//////////////////////////////////////////
