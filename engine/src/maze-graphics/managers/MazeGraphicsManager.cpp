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
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/managers/MazeMeshManager.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCamera3DShadowBuffer.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeCanvasRenderer.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeGizmoBillboard3D.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeMeshRendererInstanced.hpp"
#include "maze-graphics/ecs/components/MazeSkinnedMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeScissorMask2D.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSystemTextRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLineRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeSimpleLineRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeLineRenderer3D.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3D.hpp"
#include "maze-graphics/ecs/components/MazeTrailRenderer3DHider.hpp"
#include "maze-graphics/ecs/components/MazeTerrainMesh3D.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class GraphicsManager
    //
    //////////////////////////////////////////
    GraphicsManager* GraphicsManager::s_instance = nullptr;

    //////////////////////////////////////////
    GraphicsManager::GraphicsManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    GraphicsManager::~GraphicsManager()
    {
        m_renderSystems.clear();
        setDefaultRenderSystem(nullptr);

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void GraphicsManager::Initialize(
        GraphicsManagerPtr& _graphicsManager,
        DataBlock const& _config)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(GraphicsManager, _graphicsManager, init(_config));
    }

    //////////////////////////////////////////
    bool GraphicsManager::init(DataBlock const& _config)
    {
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        MeshManager::Initialize(m_meshManager);

        
        GraphicsUtilsHelper::ConstructAsciiSymbolsSheet8x8();


        // 2D
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Canvas>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<CanvasGroup>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<CanvasRenderer>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<CanvasScaler>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ScissorMask2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SpriteRenderer2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SystemTextRenderer2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<LineRenderer2D>("2D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SimpleLineRenderer2D>("2D");

        // 3D
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Camera3D>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Camera3DShadowBuffer>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<GizmoBillboard3D>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<Light3D>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<TrailRenderer3D>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<TrailRenderer3DHider>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<LineRenderer3D>("3D");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<TerrainMesh3D>("3D");

        // Renderer
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<MeshRenderer>("Renderer");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<MeshRendererInstanced>("Renderer");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<SkinnedMeshRenderer>("Renderer");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<RenderMask>("Renderer");
        

        return true;
    }


    //////////////////////////////////////////
    void GraphicsManager::update(F32 dt)
    {
        
    }

    //////////////////////////////////////////
    void GraphicsManager::addRenderSystem(RenderSystemPtr const& _renderSystem)
    {
        StringKeyMap<RenderSystemPtr>::iterator it = m_renderSystems.find(_renderSystem->getName());
        MAZE_ERROR_RETURN_IF(it != m_renderSystems.end(), "RenderSystem %s is already exists!", _renderSystem->getName().c_str());

        m_renderSystems.insert(_renderSystem->getName(), _renderSystem);


        if (m_defaultRenderSystem == nullptr)
        {
            setDefaultRenderSystem(_renderSystem);
        }
    }

    //////////////////////////////////////////
    void GraphicsManager::removeRenderSystem(RenderSystemPtr const& _renderSystem)
    {
        StringKeyMap<RenderSystemPtr>::iterator it = m_renderSystems.find(_renderSystem->getName());
        MAZE_ERROR_RETURN_IF(it == m_renderSystems.end(), "RenderSystem %s is not exists!", _renderSystem->getName().c_str());

        m_renderSystems.erase(it);

        if (_renderSystem == m_defaultRenderSystem)
        {
            if (m_renderSystems.empty())
                m_defaultRenderSystem.reset();
            else
                m_defaultRenderSystem.reset();
        }
    }

    //////////////////////////////////////////
    void GraphicsManager::setDefaultRenderSystem(RenderSystemPtr const& _renderSystem)
    {
        if (m_defaultRenderSystem == _renderSystem)
            return;

        if (_renderSystem)
        {
            StringKeyMap<RenderSystemPtr>::iterator it = m_renderSystems.find(_renderSystem->getName());
            MAZE_ERROR_RETURN_IF(it == m_renderSystems.end(), "RenderSystem %s is not in RenderSystems list!", _renderSystem->getName().c_str());
        }

        eventDefaultRenderSystemWillBeChanged(_renderSystem);
        m_defaultRenderSystem = _renderSystem;
        eventDefaultRenderSystemChanged(m_defaultRenderSystem);
    }

    //////////////////////////////////////////
    void GraphicsManager::createBuiltinAssets()
    {
        m_meshManager->createBuiltinMeshes();

        if (m_defaultRenderSystem)
            m_defaultRenderSystem->createBuiltinAssets();
    }

} // namespace Maze
//////////////////////////////////////////
