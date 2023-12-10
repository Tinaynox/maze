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
#include "maze-graphics/ecs/components/MazeSimpleLineRenderer2D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/utils/MazeProfiler.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/MazeECSRenderScene.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static VertexAttributeDescription const c_positionDescription = VertexAttributeDescription
        {
            VertexAttributeSemantic::Position,
            3,
            VertexAttributeType::F32,
            false,
            3 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
            0
        };

    //////////////////////////////////////////
    static VertexAttributeDescription const c_colorDescription = VertexAttributeDescription
        {
            VertexAttributeSemantic::Color,
            4,
            VertexAttributeType::F32,
            false,
            4 * GetVertexAttributeTypeSize(VertexAttributeType::F32),
            0
        };


    //////////////////////////////////////////
    // Class SimpleLineRenderer2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(SimpleLineRenderer2D, Component,
        MAZE_IMPLEMENT_METACLASS_PROPERTY(ColorF128, color, ColorF128(), getColor, setColor),
        MAZE_IMPLEMENT_METACLASS_PROPERTY(Vector<Vec3F>, positions, Vector<Vec3F>(), getPositions, setPositions));

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(SimpleLineRenderer2D);


    //////////////////////////////////////////
    SimpleLineRenderer2D::SimpleLineRenderer2D()
        : m_renderSystem(nullptr)
        , m_color(ColorF128::c_white)
    {
    }

    //////////////////////////////////////////
    SimpleLineRenderer2D::~SimpleLineRenderer2D()
    {
        
    }

    //////////////////////////////////////////
    SimpleLineRenderer2DPtr SimpleLineRenderer2D::Create(RenderSystem* _renderSystem)
    {
        SimpleLineRenderer2DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(SimpleLineRenderer2D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool SimpleLineRenderer2D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        m_renderSystem = _renderSystem;
        
        return true;
    }

    //////////////////////////////////////////
    bool SimpleLineRenderer2D::init(
        Component* _component,
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<SimpleLineRenderer2D>()->m_renderSystem;

        if (!Component::init(_component, _world, _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void SimpleLineRenderer2D::rebuildMesh()
    {
        if (!m_vao)
            return;

        S32 positionsCount = (S32)m_positions.size();
        if (positionsCount <= 0)
        {
            m_vao->clear();
            return;
        }

        m_colors.resize(positionsCount);
        m_indices.resize(positionsCount);

        for (S32 i = 0, in = positionsCount; i < in; ++i)
        {
            m_colors[i] = m_color.toVec4F32();
            m_indices[i] = i;
        }

        m_vao->setVerticesData((U8 const*)&m_positions[0], c_positionDescription, positionsCount);
        m_vao->setVerticesData((U8 const*)&m_colors[0], c_colorDescription, positionsCount);        
        m_vao->setIndices((U8 const*)&m_indices[0], VertexAttributeType::U32, positionsCount);
    }

    //////////////////////////////////////////
    void SimpleLineRenderer2D::processEntityAwakened()
    {
        m_renderMask = getEntityRaw()->ensureComponent<RenderMask>();
        m_transform = getEntityRaw()->ensureComponent<Transform2D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();
    }

    //////////////////////////////////////////
    void SimpleLineRenderer2D::processSceneSet()
    {
        MAZE_ERROR_RETURN_IF(!getEntityRaw(), "Entity is null");
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getECSScene(), "Entity Scene is null");

#if (MAZE_DEBUG)
        MAZE_ERROR_RETURN_IF(!getEntityRaw()->getECSScene()->getMetaClass()->isInheritedFrom<ECSRenderScene>(), "Entity Scene is not Render Scene");
#endif

        RenderTargetPtr const& renderTarget = getEntityRaw()->getECSScene()->castRaw<ECSRenderScene>()->getRenderTarget();

        m_vao = VertexArrayObject::Create(m_renderSystem);
        m_vao->setRenderDrawTopology(RenderDrawTopology::LinesStrip);
        m_renderMesh = renderTarget->createRenderMeshFromPool(1);
        m_renderMesh->setVertexArrayObject(m_vao);
        m_meshRenderer->setRenderMesh(m_renderMesh);

        rebuildMesh();
    }

    //////////////////////////////////////////
    void SimpleLineRenderer2D::processEntityRemoved()
    {
        m_renderMesh.reset();
    }
    
    //////////////////////////////////////////
    void SimpleLineRenderer2D::clear()
    {
        m_positions.clear();
        m_vao->clear();        
    }

} // namespace Maze
//////////////////////////////////////////
