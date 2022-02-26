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
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderMesh
    //
    //////////////////////////////////////////
    RenderMesh::RenderMesh()
    {
    }

    //////////////////////////////////////////
    RenderMesh::~RenderMesh()
    {
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderMesh::Create(
        RenderSystem* _renderSystem,
        RenderMeshDeleter const& _deleter)
    {
        if (!_renderSystem)
            _renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        RenderMeshPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR_EX(RenderMesh, object, _deleter, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderMesh::Create(
        AssetFilePtr const& _assetFile,
        RenderSystem* _renderSystem,
        RenderMeshDeleter const& _deleter,
        RenderTarget* _renderTarget)
    {
        RenderMeshPtr renderMesh = RenderMesh::Create(_renderSystem, _deleter);

        if (renderMesh)
            renderMesh->loadFromAssetFile(_assetFile, _renderTarget);

        return renderMesh;
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderMesh::Create(
        MeshPtr const& _mesh,
        RenderSystem* _renderSystem,
        RenderMeshDeleter const& _deleter,
        RenderTarget* _renderTarget)
    {
        RenderMeshPtr renderMesh = RenderMesh::Create(_renderSystem, _deleter);

        if (renderMesh)
            renderMesh->loadFromMesh(_mesh, _renderTarget);

        return renderMesh;
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderMesh::Create(
        String const& _assetFileName,
        RenderSystem* _renderSystem,
        RenderMeshDeleter const& _deleter,
        RenderTarget* _renderTarget)
    {
        RenderMeshPtr mesh = RenderMesh::Create(_renderSystem, _deleter);

        if (mesh)
            mesh->loadFromAssetFile(_assetFileName, _renderTarget);

        return mesh;
    }

    //////////////////////////////////////////
    bool RenderMesh::init(RenderSystem* _renderSystem)
    {
        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    void RenderMesh::setName(String const& _name)
    {
        m_name = _name;

#if MAZE_DEBUG
        updateVAODebugInfo();
#endif
    }

#if MAZE_DEBUG
    //////////////////////////////////////////
    void RenderMesh::updateVAODebugInfo()
    {
        for (S32 i = 0, in = (S32)m_vertexArrayObjects.size(); i < in; ++i)
        {
            VertexArrayObjectPtr const& vao = m_vertexArrayObjects[i];
            if (vao)
                vao->setDebugInfo(m_name + "__" + StringHelper::ToString(i));
        }
    }
#endif

    //////////////////////////////////////////
    void RenderMesh::loadFromAssetFile(
        AssetFilePtr const& _assetFile,
        RenderTarget* _renderTarget)
    {
        MAZE_ERROR_RETURN_IF(!_assetFile, "Null AssetFile")
        
        MeshPtr mesh = Mesh::Create(m_renderSystem);

        MAZE_LOG("Loading %s...", _assetFile->getFileName().c_str());
        LoadOBJ(_assetFile, mesh);

        loadFromMesh(mesh, _renderTarget);
        
    }

    //////////////////////////////////////////
    void RenderMesh::loadFromAssetFile(
        String const& _assetFileName,
        RenderTarget* _renderTarget)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile, _renderTarget);
    }

    //////////////////////////////////////////
    void RenderMesh::loadFromMesh(
        MeshPtr const& _mesh,
        RenderTarget* _renderTarget)
    {
        Size currentVaoCount = m_vertexArrayObjects.size();
        Size requiredVaoCount = _mesh->getSubMeshesCount();
        m_vertexArrayObjects.resize(requiredVaoCount);
        if (currentVaoCount < requiredVaoCount)
        {
            Size extraVao = requiredVaoCount - currentVaoCount;
            for (Size i = currentVaoCount, in = extraVao; i < in; ++i)
                m_vertexArrayObjects[i] = VertexArrayObject::Create(m_renderSystem, _renderTarget);
        }

        for (Size i = 0, in = requiredVaoCount; i < in; ++i)
        {
            if (m_vertexArrayObjects[i])
            {
                SubMeshPtr const& subMesh = _mesh->getSubMesh(i);
                m_vertexArrayObjects[i]->setMesh(subMesh);
            }
        }

#if MAZE_DEBUG
        updateVAODebugInfo();
#endif
    }

    //////////////////////////////////////////
    void RenderMesh::clearVertexArrayObjects()
    {
        m_vertexArrayObjects.clear();
    }

    //////////////////////////////////////////
    void RenderMesh::setVertexArrayObject(VertexArrayObjectPtr const& _vao)
    {
        clearVertexArrayObjects();

        m_vertexArrayObjects.push_back(_vao);
    }

    //////////////////////////////////////////
    void RenderMesh::clear()
    {
        clearVertexArrayObjects();
    }

    //////////////////////////////////////////
    String RenderMesh::toString() const
    {
        return getName();
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderMesh::FromString(String const& _string)
    {
        return GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getRenderMesh(_string);
    }
    

} // namespace Maze
//////////////////////////////////////////
