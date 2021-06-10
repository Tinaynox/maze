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
#include "maze-render-system-opengl-core/MazeRenderPassOpenGLPool.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderPassOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    // Class RenderPassOpenGLPool
    //
    //////////////////////////////////////////
    RenderPassOpenGLPool::RenderPassOpenGLPool(RenderSystem* _renderSystem)
        : m_renderSystem(_renderSystem)
    {
    }

    //////////////////////////////////////////
    RenderPassOpenGLPool::~RenderPassOpenGLPool()
    {
        clear();
    }

    //////////////////////////////////////////
    RenderPassOpenGLPtr RenderPassOpenGLPool::createRenderPassOpenGL(RenderPassType _renderPassType)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        auto& renderMeshPool = m_renderMeshPools[_renderPassType];

        std::function<void(RenderPassOpenGL* _ptr)> const deleter = 
            [this, _renderPassType](RenderPassOpenGL* _ptr)
            {
                releaseRenderPassOpenGL(_ptr, _renderPassType);
            };

        if (!renderMeshPool.empty())
        {
            RenderPassOpenGL* ptr = renderMeshPool.top();
            renderMeshPool.pop();

            RenderPassOpenGLPtr sharedPtr(
                ptr,
                deleter,
                GetDefaultStdMemoryAllocator<RenderPassOpenGL>());

            return sharedPtr;
        }
        else
        {
            RenderPassOpenGLPtr sharedPtr = RenderPassOpenGL::Create(
                m_renderSystem, 
                _renderPassType,
                deleter);

            return sharedPtr;
        }
    }

    //////////////////////////////////////////
    void RenderPassOpenGLPool::releaseRenderPassOpenGL(
            RenderPassOpenGL* _ptr,
            RenderPassType _renderPassType)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        auto& renderMeshPool = m_renderMeshPools[_renderPassType];
        renderMeshPool.push(_ptr);
    }

    //////////////////////////////////////////
    void RenderPassOpenGLPool::clear()
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        for (auto& renderMeshPool : m_renderMeshPools)
        {
            while (!renderMeshPool.empty())
            {
                MAZE_DELETE(renderMeshPool.top());
                renderMeshPool.pop();
            }
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
