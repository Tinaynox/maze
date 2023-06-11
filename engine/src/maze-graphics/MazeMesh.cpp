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
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeSubMesh.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class Mesh
    //
    //////////////////////////////////////////
    Mesh::Mesh()
    {
    }

    //////////////////////////////////////////
    Mesh::~Mesh()
    {
    }

    //////////////////////////////////////////
    MeshPtr Mesh::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = Maze::GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        MeshPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(Mesh, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool Mesh::init(RenderSystem* _renderSystem)
    {
        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    SubMeshPtr const& Mesh::ensureSubMesh()
    {
        if (!m_subMeshes.empty())
            return m_subMeshes.front();

        addSubMesh(SubMesh::Create());
        return m_subMeshes.back();
    }

    //////////////////////////////////////////
    void Mesh::clear()
    {
        m_subMeshes.clear();
    }

    //////////////////////////////////////////
    void Mesh::setRenderDrawTopology(RenderDrawTopology _renderDrawTopology)
    {
        SubMeshPtr subMesh = ensureSubMesh();
        subMesh->setRenderDrawTopology(_renderDrawTopology);
    }

    //////////////////////////////////////////
    void Mesh::setIndices(U32* _indices, Size _count)
    {
        SubMeshPtr subMesh = ensureSubMesh();
        subMesh->setIndices(_indices, _count);
    }

    //////////////////////////////////////////
    void Mesh::setPositions(Vec3DF* _positions, Size _count)
    {
        SubMeshPtr subMesh = ensureSubMesh();
        subMesh->setPositions(_positions, _count);
    }

    //////////////////////////////////////////
    void Mesh::setNormals(Vec3DF* _normals, Size _count)
    {
        SubMeshPtr subMesh = ensureSubMesh();
        subMesh->setNormals(_normals, _count);
    }

    //////////////////////////////////////////
    void Mesh::setColors(Vec4DF* _colors, Size _count)
    {
        SubMeshPtr subMesh = ensureSubMesh();
        subMesh->setColors(_colors, _count);
    }

    //////////////////////////////////////////
    void Mesh::setTexCoords(S32 _uvIndex, Vec2DF* _data, Size _count)
    {
        SubMeshPtr subMesh = ensureSubMesh();
        subMesh->setTexCoords(_uvIndex, _data, _count);
    }

    //////////////////////////////////////////
    void Mesh::setTexCoords(S32 _uvIndex, Vec4DF* _data, Size _count)
    {
        SubMeshPtr subMesh = ensureSubMesh();
        subMesh->setTexCoords(_uvIndex, _data, _count);
    }

    //////////////////////////////////////////
    SubMeshPtr const& Mesh::getSubMesh(Size _index) const
    {
        static SubMeshPtr nullPointer;

        if (_index >= m_subMeshes.size())
            return nullPointer;
        

        return m_subMeshes[_index];
    }

    //////////////////////////////////////////
    void Mesh::addSubMesh(SubMeshPtr const& _subMesh)
    {
        m_subMeshes.push_back(_subMesh);
    }

    //////////////////////////////////////////
    void Mesh::scale(F32 _scale)
    {
        for (SubMeshPtr const& subMesh : m_subMeshes)
            subMesh->scale(_scale);
    }

    //////////////////////////////////////////
    void Mesh::mergeSubMeshes()
    {
        for (S32 i = 0; i < (S32)m_subMeshes.size(); ++i)
        {
            for (S32 j = i + 1; j < (S32)m_subMeshes.size(); )
            {
                SubMeshPtr const& subMesh0 = m_subMeshes[i];
                SubMeshPtr const& subMesh1 = m_subMeshes[j];

                if (subMesh0->mergeWith(subMesh1))
                {
                    m_subMeshes.erase(m_subMeshes.begin() + j);
                }
                else
                {
                    ++j;
                }
            }
        }
    }

} // namespace Maze
//////////////////////////////////////////
