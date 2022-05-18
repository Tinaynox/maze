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
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinMeshType);


    //////////////////////////////////////////
    // Class MeshManager
    //
    //////////////////////////////////////////
    MeshManager::MeshManager()
        : m_renderSystemRaw(nullptr)
    {
        
    }

    //////////////////////////////////////////
    MeshManager::~MeshManager()
    {
        if (m_renderSystemRaw)
            m_renderSystemRaw->eventSystemInited.unsubscribe(this);
    }

    //////////////////////////////////////////
    void MeshManager::Initialize(MeshManagerPtr& _meshManager, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshManager, _meshManager, init(_renderSystem));
    }

    //////////////////////////////////////////
    MeshManagerPtr const& MeshManager::GetCurrentInstancePtr()
    {
        return RenderSystem::GetCurrentInstancePtr()->getMeshManager();
    }

    //////////////////////////////////////////
    bool MeshManager::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();
        return true;
    }

    //////////////////////////////////////////
    MeshPtr const& MeshManager::createBuiltinMesh(BuiltinMeshType _meshType)
    {
        MeshPtr& mesh = m_builtinMeshes[_meshType];

        switch (_meshType)
        {
            case BuiltinMeshType::Quad:
            {
                mesh = MeshHelper::CreateQuadMesh();
                break;
            }
            case BuiltinMeshType::Cube:
            {
                mesh = MeshHelper::CreateCubeMesh();
                break;
            }
            case BuiltinMeshType::Sphere:
            {
                mesh = MeshHelper::CreateSpherifiedCubeMesh();
                break;
            }
            case BuiltinMeshType::Cone:
            {
                mesh = MeshHelper::CreateConeMesh();
                break;
            }
            case BuiltinMeshType::Cylinder:
            {
                mesh = MeshHelper::CreateCylinderMesh();
                break;
            }
            case BuiltinMeshType::Torus:
            {
                mesh = MeshHelper::CreateTorusMesh();
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        return mesh;
    }

    //////////////////////////////////////////
    MeshPtr const& MeshManager::ensureBuiltinMesh(BuiltinMeshType _meshType)
    {
        MeshPtr const& mesh = getBuiltinMesh(_meshType);
        if (mesh)
            return mesh;

        return createBuiltinMesh(_meshType);
    }

    //////////////////////////////////////////
    void MeshManager::createBuiltinMeshes()
    {
        for (BuiltinMeshType t = BuiltinMeshType(1); t < BuiltinMeshType::MAX; ++t)
            ensureBuiltinMesh(t);
    }

    
} // namespace Maze
//////////////////////////////////////////
