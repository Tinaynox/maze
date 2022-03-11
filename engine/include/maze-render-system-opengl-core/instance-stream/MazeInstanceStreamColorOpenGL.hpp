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
#if (!defined(_MazeInstanceStreamColorOpenGL_hpp_))
#define _MazeInstanceStreamColorOpenGL_hpp_


//////////////////////////////////////////
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGLCoreHeader.hpp"
#include "maze-render-system-opengl-core/MazeHeaderOpenGL.hpp"
#include "maze-graphics/MazeRenderQueue.hpp"
#include "maze-render-system-opengl-core/instance-stream/MazeInstanceStreamOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-graphics/instance-stream/MazeInstanceStreamColor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(InstanceStreamColorOpenGL);


    //////////////////////////////////////////
    // Class InstanceStreamColorOpenGL
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_OPENGL_CORE_API InstanceStreamColorOpenGL
        : public InstanceStreamColor
    {
    public:

        //////////////////////////////////////////
        static InstanceStreamColorOpenGLPtr Create(
            RenderSystemOpenGL* _rs,
            ContextOpenGL* _contextOpenGL,
            InstanceStreamModeOpenGL _mode);

        //////////////////////////////////////////
        ~InstanceStreamColorOpenGL();        

        //////////////////////////////////////////
        void prepareForRender(S32 _instancesCount);

        //////////////////////////////////////////
        void bindRenderPass();

        //////////////////////////////////////////
        void processDrawBegin();

        //////////////////////////////////////////
        inline GPUTextureBufferPtr const& getModelMatriciesTextureBuffer() const { return m_bufferInfo.buffer; }

    protected:

        //////////////////////////////////////////
        InstanceStreamColorOpenGL();

        //////////////////////////////////////////
        bool init(
            RenderSystemOpenGL* _rs,
            ContextOpenGL* _contextOpenGL,
            InstanceStreamModeOpenGL _mode);

    protected:
        ContextOpenGL* m_context = nullptr;
        InstanceStreamModeOpenGL m_mode = InstanceStreamModeOpenGL::None;

        RenderQueueGPUBufferInfo m_bufferInfo;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeInstanceStreamColorOpenGL_hpp_
//////////////////////////////////////////
