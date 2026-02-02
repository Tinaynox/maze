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
#include "maze-graphics/MazeRenderMeshPool.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderMeshPool
    //
    //////////////////////////////////////////
    RenderMeshPool::RenderMeshPool(RenderSystem* _renderSystem)
        : m_renderSystem(_renderSystem)
        , m_activeMeshes(0)
    {
    }

    //////////////////////////////////////////
    RenderMeshPool::~RenderMeshPool()
    {
        MAZE_ERROR_IF(m_activeMeshes > 0, "Pool still has active meshes!");

        clear();
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderMeshPool::createRenderMeshFromPool(S32 _subMeshCount)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        auto& renderMeshPool = m_renderMeshPools[_subMeshCount];

        std::function<void(RenderMesh* _ptr)> const deleter = 
            [this, _subMeshCount](RenderMesh* _ptr)
            {
                releaseRenderMesh(_ptr, _subMeshCount);
            };

        ++m_activeMeshes;

        if (!renderMeshPool.empty())
        {
            RenderMesh* ptr = renderMeshPool.top();
            renderMeshPool.pop();

            RenderMeshPtr sharedPtr(
                ptr,
                deleter);

            return sharedPtr;
        }
        else
        {
            RenderMeshPtr sharedPtr = RenderMesh::Create(
                m_renderSystem, 
                deleter);

            return sharedPtr;
        }
    }

    //////////////////////////////////////////
    void RenderMeshPool::releaseRenderMesh(
            RenderMesh* _ptr,
            S32 _key)
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        --m_activeMeshes;

        auto& renderMeshPool = m_renderMeshPools[_key];
        renderMeshPool.push(_ptr);
    }

    //////////////////////////////////////////
    void RenderMeshPool::clear()
    {
        MAZE_MUTEX_SCOPED_LOCK(m_mutex);

        for (auto& renderMeshPool : m_renderMeshPools)
        {
            while (!renderMeshPool.second.empty())
            {
                // Debug::Log("Deleting render mesh: %s (%x)...", renderMeshPool.second.top()->getName().c_str(), renderMeshPool.second.top());
                MAZE_DELETE(renderMeshPool.second.top());
                renderMeshPool.second.pop();
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
