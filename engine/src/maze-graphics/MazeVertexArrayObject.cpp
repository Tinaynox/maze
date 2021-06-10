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
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeSubMesh.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class VertexArrayObject
    //
    //////////////////////////////////////////
    VertexArrayObject::VertexArrayObject()
        : m_renderSystem(nullptr)
        , m_renderDrawTopology(RenderDrawTopology::Triangles)
        , m_indicesType(VertexAttributeType::U32)
        , m_indicesCount(0)
    {
    }

    //////////////////////////////////////////
    VertexArrayObject::~VertexArrayObject()
    {
    }

    //////////////////////////////////////////
    VertexArrayObjectPtr VertexArrayObject::Create(
        RenderSystem* _renderSystem,
        RenderTarget* _renderTarget)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        return _renderSystem->createVertexArrayObject(_renderTarget);
    }

    //////////////////////////////////////////
    bool VertexArrayObject::init(RenderSystem* _renderSystem)
    {
        m_renderSystem = _renderSystem;
        return true;
    }

    //////////////////////////////////////////
    void VertexArrayObject::setMesh(SubMeshPtr const& _subMesh)
    {
        m_subMeshCopy = _subMesh->createCopy();

        setRenderDrawTopology(_subMesh->getRenderDrawTopology());

        setIndices(
            _subMesh->getIndicesData(),
            _subMesh->getIndicesType(),
            _subMesh->getIndicesCount());

        for (VertexAttributeSemantic semantic = VertexAttributeSemantic(0);
                                     semantic < VertexAttributeSemantic::MAX;
                                     semantic = (VertexAttributeSemantic)((Size)semantic + 1))
        {
            U8 const* data;
            VertexAttributeDescription description;
            Size count;

            if (_subMesh->getVertexData(semantic, data, description, count))
            {
                setVerticesData(data, description, count);
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
