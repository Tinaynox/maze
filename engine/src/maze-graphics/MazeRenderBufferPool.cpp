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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderBufferPool.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderBuffer.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderBufferPool
    //
    //////////////////////////////////////////
    RenderBufferPool::RenderBufferPool(RenderSystem* _renderSystem)
        : m_renderSystem(_renderSystem)
    {
    }

    //////////////////////////////////////////
    RenderBufferPool::~RenderBufferPool()
    {
        clear();
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBufferPool::createRenderBuffer(RenderBufferSpecification _specifiaction)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        auto& renderBufferPool = m_renderBufferPools[_specifiaction];

        std::function<void(RenderBuffer* _ptr)> const deleter = 
            [this, _specifiaction](RenderBuffer* _ptr)
            {
                releaseRenderBuffer(_ptr, _specifiaction);
            };

        if (!renderBufferPool.empty())
        {
            RenderBuffer* ptr = renderBufferPool.top();
            renderBufferPool.pop();

            RenderBufferPtr sharedPtr(
                ptr,
                deleter,
                GetDefaultStdMemoryAllocator<RenderBuffer>());

            return sharedPtr;
        }
        else
        {
            RenderBufferPtr sharedPtr = RenderBuffer::Create(
                _specifiaction,
                m_renderSystem, 
                deleter);

            return sharedPtr;
        }
    }

    //////////////////////////////////////////
    void RenderBufferPool::releaseRenderBuffer(
            RenderBuffer* _ptr,
            RenderBufferSpecification _specifiaction)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        auto& renderBufferPool = m_renderBufferPools[_specifiaction];
        renderBufferPool.push(_ptr);
    }

    //////////////////////////////////////////
    void RenderBufferPool::clear()
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        for (auto& renderBufferPool : m_renderBufferPools)
        {
            while (!renderBufferPool.second.empty())
            {
                MAZE_DELETE(renderBufferPool.second.top());
                renderBufferPool.second.pop();
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
