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
#include "EditorEntityManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/ecs/systems/MazeBounds2DSystem.hpp"
#include "maze-core/ecs/systems/MazeSizePolicy2DSystem.hpp"
#include "maze-core/ecs/systems/MazeTransformUtilsSystem.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-graphics/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderPreparationSystem.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-ui/ecs/systems/MazeInputSystem2D.hpp"
#include "layout/EditorLayout.hpp"
#include "managers/EditorManager.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class EditorEntityManager
    //
    //////////////////////////////////////////
    EditorEntityManager* EditorEntityManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorEntityManager::EditorEntityManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorEntityManager::~EditorEntityManager()
    {
        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorEntityManager::Initialize(EditorEntityManagerPtr& _updateManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorEntityManager, _updateManager, init());
    }

    //////////////////////////////////////////
    bool EditorEntityManager::init()
    {
        m_workspaceWorld = ECSWorld::Create();

        RenderSystemPtr renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        InputSystem2DPtr inputSystem = InputSystem2D::Create();
        inputSystem->setCoordsConverter(
            [](Vec2DF const& _coords)
            {
                return EditorLayout::ConvertRenderWindowCoordsToWorkspaceViewport(_coords);
            });

        m_workspaceWorld->addSystem(SizePolicy2DSystem::Create());
        m_workspaceWorld->addSystem(TransformEventsSystem::Create());
        m_workspaceWorld->addSystem(RenderPreparationSystem::Create(renderSystem));
        m_workspaceWorld->addSystem(RenderControlSystem::Create(renderSystem));
        m_workspaceWorld->addSystem(Bounds2DSystem::Create());
        m_workspaceWorld->addSystem(inputSystem);


        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void EditorEntityManager::update(F32 _dt)
    {
        if (!EditorManager::GetInstancePtr()->getPlaytestModeEnabled())
            m_workspaceWorld->update(_dt);
    }

} // namespace Maze
//////////////////////////////////////////
