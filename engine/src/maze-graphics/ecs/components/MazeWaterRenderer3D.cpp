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
#include "maze-graphics/ecs/components/MazeWaterRenderer3D.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeMaterial.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeSubMeshHelper.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include <cassert>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class WaterRenderer3D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(WaterRenderer3D, Component);

    //////////////////////////////////////////
    MAZE_IMPLEMENT_MEMORY_ALLOCATION_BLOCK(WaterRenderer3D);


    //////////////////////////////////////////
    WaterRenderer3D::WaterRenderer3D()
    {
        
    }

    //////////////////////////////////////////
    WaterRenderer3D::~WaterRenderer3D()
    {
        
    }

    //////////////////////////////////////////
    WaterRenderer3DPtr WaterRenderer3D::Create(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            _renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw();

        WaterRenderer3DPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(WaterRenderer3D, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool WaterRenderer3D::init(RenderSystem* _renderSystem)
    {
        if (!_renderSystem)
            return false;

        m_renderSystem = _renderSystem;

        return true;
    }

    //////////////////////////////////////////
    bool WaterRenderer3D::init(
        Component* _component,
        ECSWorld* _world,
        EntityCopyData _copyData)
    {
        m_renderSystem = _component->castRaw<WaterRenderer3D>()->m_renderSystem;

        if (!Component::init(
            _component,
            _world,
            _copyData))
            return false;

        return true;
    }

    //////////////////////////////////////////
    void WaterRenderer3D::processEntityAwakened()
    {
        m_transform = getEntityRaw()->ensureComponent<Transform3D>();
        m_meshRenderer = getEntityRaw()->ensureComponent<MeshRenderer>();
        m_meshRenderer->getRenderMask()->setMask(DefaultRenderMask::Water);

        m_meshRenderer->setRenderMesh(RenderMeshPtr());
        
    }

    //////////////////////////////////////////
    void WaterRenderer3D::setMaterial(MaterialPtr const& _material)
    {
        m_meshRenderer->setMaterial(_material ? _material->createCopy() : nullptr);
    }

    //////////////////////////////////////////
    void WaterRenderer3D::setMaterial(String const& _materialName)
    {
        MaterialPtr const& material = m_renderSystem->getMaterialManager()->getMaterial(_materialName);
        setMaterial(material);
    }

    //////////////////////////////////////////
    void WaterRenderer3D::prepare(
        RenderBuffer* _reflectionBuffer,
        RenderBuffer* _refractionBuffer)
    {
        m_reflectionBuffer = _reflectionBuffer;
        m_refractionBuffer = _refractionBuffer;

        MaterialPtr const& material = m_meshRenderer->getMaterial();
        if (material)
        {
            material->setUniform("u_reflectionMap", m_reflectionBuffer->getColorTexture());
            material->setUniform("u_refractionMap", m_refractionBuffer->getColorTexture());
            material->setUniform("u_depthMap", m_refractionBuffer->getDepthTexture());
        }
    }
            
    
} // namespace Maze
//////////////////////////////////////////
