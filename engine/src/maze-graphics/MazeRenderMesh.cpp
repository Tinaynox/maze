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
#include "maze-graphics/assets/MazeAssetUnitRenderMesh.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class RenderMesh
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderMesh, Object);

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
    void RenderMesh::setName(HashedString const& _name)
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
                vao->setDebugInfo(m_name.getString() + "__" + StringHelper::ToString(i));
        }
    }
#endif

    //////////////////////////////////////////
    void RenderMesh::loadFromAssetFile(
        AssetFilePtr const& _assetFile,
        RenderTarget* _renderTarget)
    {
        MAZE_FATAL_IF(m_locked, "RenderMesh is locked!");

        MAZE_PROFILE_EVENT("RenderMesh::loadFromAssetFile");

        MAZE_ERROR_RETURN_IF(!_assetFile, "Null AssetFile");

        MeshPtr mesh = MeshManager::GetInstancePtr()->loadMesh(_assetFile);
        loadFromMesh(mesh, _renderTarget);
    }

    //////////////////////////////////////////
    void RenderMesh::loadFromAssetFile(
        String const& _assetFileName,
        RenderTarget* _renderTarget)
    {
        MAZE_FATAL_IF(m_locked, "RenderMesh is locked!");

        MAZE_PROFILE_EVENT("RenderMesh::loadFromAssetFile");

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);
        loadFromAssetFile(assetFile, _renderTarget);
    }

    //////////////////////////////////////////
    void RenderMesh::loadFromMesh(
        MeshPtr const& _mesh,
        RenderTarget* _renderTarget)
    {
        MAZE_FATAL_IF(m_locked, "RenderMesh is locked!");

        MAZE_PROFILE_EVENT("RenderMesh::loadFromMesh");

        MAZE_ERROR_RETURN_IF(!_mesh, "Mesh is null!");

        m_mesh = _mesh;

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
        MAZE_FATAL_IF(m_locked, "RenderMesh is locked!");

        m_vertexArrayObjects.clear();
        m_mesh.reset();
    }

    //////////////////////////////////////////
    void RenderMesh::setVertexArrayObject(VertexArrayObjectPtr const& _vao)
    {
        MAZE_FATAL_IF(m_locked, "RenderMesh is locked!");

        clearVertexArrayObjects();

        m_vertexArrayObjects.push_back(_vao);
    }

    //////////////////////////////////////////
    void RenderMesh::clear()
    {
        clearVertexArrayObjects();
    }

    //////////////////////////////////////////
    RenderMeshPtr RenderMesh::FromString(String const& _string)
    {
        if (_string.empty())
            return nullptr;

        return GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getRenderMeshManager()->getOrLoadRenderMesh(
            _string);
    }

    //////////////////////////////////////////
    String RenderMesh::toString() const
    {
        return getName();
    }

    //////////////////////////////////////////
    void RenderMesh::setString(CString _data, Size _count)
    {
        MAZE_TODO;
    }


    //////////////////////////////////////////
    // Class RenderMeshAssetRef
    //
    //////////////////////////////////////////
    String RenderMeshAssetRef::toString() const
    {
        if (!m_renderMesh)
            return String();

        HashedCString name = RenderSystem::GetCurrentInstancePtr()->getRenderMeshManager()->getRenderMeshName(m_renderMesh.get());
        return !name.empty() ? name.str : String();
    }

    //////////////////////////////////////////
    void RenderMeshAssetRef::setString(CString _data, Size _count)
    {
        RenderMeshPtr const& material = RenderSystem::GetCurrentInstancePtr()->getRenderMeshManager()->getOrLoadRenderMesh(
            _data);
        setRenderMesh(material);
    }

    //////////////////////////////////////////
    bool RenderMeshAssetRef::loadFromDataBlock(DataBlock const& _dataBlock)
    {
        DataBlock::ParamIndex paramIndex = _dataBlock.findParamIndex(MAZE_HCS("value"));
        if (paramIndex >= 0)
        {
            DataBlockParamType paramType = _dataBlock.getParamType(paramIndex);
            switch (paramType)
            {
                // by AUID
                case DataBlockParamType::ParamU32:
                {
                    AssetUnitId auid = _dataBlock.getU32(paramIndex);

                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(auid);
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitRenderMesh>::UID())
                    {
                        setRenderMesh(assetUnit->castRaw<AssetUnitRenderMesh>()->loadRenderMesh(true));
                        return true;
                    }

                    break;
                }
                // by name
                case DataBlockParamType::ParamString:
                {
                    String const& name = _dataBlock.getString(paramIndex);
                    RenderMeshPtr const& renderMesh = RenderSystem::GetCurrentInstancePtr()->getRenderMeshManager()->getOrLoadRenderMesh(name);
                    setRenderMesh(renderMesh);
                    return true;
                }
                default:
                {
                    MAZE_ERROR("No supported asset ref type: %s!", c_dataBlockParamTypeInfo[(U8)paramType].name.str);
                    break;
                }
            }
        }

        setRenderMesh(RenderMeshPtr());
        return true;
    }

    //////////////////////////////////////////
    void RenderMeshAssetRef::toDataBlock(DataBlock& _dataBlock) const
    {
        if (!m_renderMesh)
        {
            _dataBlock.clearData();
            return;
        }

        // Save as AUID
        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(m_renderMesh->getName());
            if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitRenderMesh>::UID())
            {
                RenderMeshPtr const& assetUnitRenderMesh = assetUnit->castRaw<AssetUnitRenderMesh>()->getRenderMesh();
                if (assetUnitRenderMesh == m_renderMesh)
                {
                    ValueToDataBlock(assetUnit->getAssetUnitId(), _dataBlock);
                    return;
                }
            }
        }

        // Save as string
        ValueToDataBlock(m_renderMesh->getName().c_str(), _dataBlock);
    }
    

} // namespace Maze
//////////////////////////////////////////
