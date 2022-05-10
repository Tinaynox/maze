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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeMeshRendererGizmos.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-core/math/MazeAABB2D.hpp"
#include "maze-core/math/MazeAABB3D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class MeshRendererGizmos
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(MeshRendererGizmos, ComponentGizmos);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(MeshRendererGizmos);


    //////////////////////////////////////////
    MeshRendererGizmos::MeshRendererGizmos()
    {

    }

    //////////////////////////////////////////
    MeshRendererGizmos::~MeshRendererGizmos()
    {

    }

    //////////////////////////////////////////
    MeshRendererGizmosPtr MeshRendererGizmos::Create()
    {
        MeshRendererGizmosPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(MeshRendererGizmos, object, init());
        return object;
    }

    //////////////////////////////////////////
    bool MeshRendererGizmos::init()
    {

        return true;
    }

    //////////////////////////////////////////
    void MeshRendererGizmos::drawGizmos(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        
    }

    //////////////////////////////////////////
    void MeshRendererGizmos::drawGizmosSelected(
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        MeshRenderer* meshRenderer = _component->castRaw<MeshRenderer>();    

        Transform3D* transform3D = _entity->getComponentRaw<Transform3D>();
        if (transform3D)
        {
            Vector<Vec3DF> vertices;

            Mat4DF const& worldTransform = transform3D->getWorldTransform();
            if (meshRenderer->getRenderMesh())
            {
                for (VertexArrayObjectPtr const& vao : meshRenderer->getRenderMesh()->getVertexArrayObjects())
                {
                    SubMeshPtr const& subMesh = vao->getSubMeshCopy();
                    if (subMesh)
                    {
                        U8 const* positionData = subMesh->getVertexData(VertexAttributeSemantic::Position);
                        VertexAttributeDescription const& positionDesc = subMesh->getVertexDescription(VertexAttributeSemantic::Position);
                        Size verticesCount = subMesh->getVerticesCount(VertexAttributeSemantic::Position);
                        for (Size i = 0; i < verticesCount; i++, positionData += positionDesc.stride)
                        {
                            U8 const* data = positionData + positionDesc.offset;
                            Vec3DF position = *(Vec3DF*)data;

                            Vec3DF worldPosition = worldTransform.transformAffine(position);
                            vertices.emplace_back(worldPosition);
                        }
                    }
                }
            }

            if (!vertices.empty())
            {
                AABB3D aabb3D(vertices.front());

                for (Size i = 1, in = vertices.size(); i < in; ++i)
                    aabb3D.applyUnion(vertices[i]);

                _drawer->setColor(ColorF128::c_white);
                _drawer->drawAABB(aabb3D);
            }
        }

    }

} // namespace Maze
//////////////////////////////////////////
