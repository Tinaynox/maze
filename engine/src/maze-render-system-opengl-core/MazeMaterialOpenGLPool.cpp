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
#include "maze-render-system-opengl-core/MazeMaterialOpenGLPool.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderSystemOpenGL.hpp"
#include "maze-render-system-opengl-core/MazePixelFormatOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeMaterialOpenGL.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    
    //////////////////////////////////////////
    // Class MaterialOpenGLPool
    //
    //////////////////////////////////////////
    MaterialOpenGLPool::MaterialOpenGLPool(RenderSystem* _renderSystem)
        : m_renderSystem(_renderSystem)
    {
    }

    //////////////////////////////////////////
    MaterialOpenGLPool::~MaterialOpenGLPool()
    {
        clear();
    }

    //////////////////////////////////////////
    MaterialOpenGLPtr MaterialOpenGLPool::createMaterialOpenGL(ContextOpenGL* _contextOpenGL)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        Stack<MaterialOpenGL*>* renderMeshPool = nullptr;

        Map<ContextOpenGL*, Stack<MaterialOpenGL*>>::iterator it = m_renderMeshPools.find(_contextOpenGL);
        if (it == m_renderMeshPools.end())
        {
            _contextOpenGL->eventDestroyed.subscribe(this, &MaterialOpenGLPool::notifyContextDestroyed);

            renderMeshPool = &m_renderMeshPools[_contextOpenGL];
        }
        else
        {
            renderMeshPool = &it->second;
        }

        std::function<void(MaterialOpenGL* _ptr)> const deleter = 
            [this, _contextOpenGL](MaterialOpenGL* _ptr)
            {
                releaseMaterialOpenGL(_ptr, _contextOpenGL);
            };

        if (!renderMeshPool->empty())
        {
            MaterialOpenGL* ptr = renderMeshPool->top();
            renderMeshPool->pop();

            MaterialOpenGLPtr sharedPtr(
                ptr,
                deleter,
                GetDefaultStdMemoryAllocator<MaterialOpenGL>());

            return sharedPtr;
        }
        else
        {
            MaterialOpenGLPtr sharedPtr = MaterialOpenGL::Create(
                m_renderSystem, 
                _contextOpenGL,
                deleter);

            return sharedPtr;
        }
    }

    //////////////////////////////////////////
    void MaterialOpenGLPool::releaseMaterialOpenGL(
            MaterialOpenGL* _ptr,
            ContextOpenGL* _contextOpenGL)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        auto& renderMeshPool = m_renderMeshPools[_contextOpenGL];
        renderMeshPool.push(_ptr);
    }

    //////////////////////////////////////////
    void MaterialOpenGLPool::clear()
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        for (auto& renderMeshPool : m_renderMeshPools)
        {
            while (!renderMeshPool.second.empty())
            {
                MAZE_DELETE(renderMeshPool.second.top());
                renderMeshPool.second.pop();
            }

            renderMeshPool.first->eventDestroyed.unsubscribe(this);
        }
        m_renderMeshPools.clear();
    }

    //////////////////////////////////////////
    void MaterialOpenGLPool::notifyContextDestroyed(ContextOpenGL* _contextOpenGL)
    {
        _contextOpenGL->eventDestroyed.unsubscribe(this);

        auto& renderMeshPool = m_renderMeshPools[_contextOpenGL];

        while (!renderMeshPool.empty())
        {
            MAZE_DELETE(renderMeshPool.top());
            renderMeshPool.pop();
        }

        m_renderMeshPools.erase(_contextOpenGL);
    }
    
} // namespace Maze
//////////////////////////////////////////
