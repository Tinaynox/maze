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
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/ecs/components/MazeRenderController.hpp"
#include "maze-graphics/ecs/components/MazeGraphicsEventRetranslator.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-ui/ecs/components/MazeInputSystem2D.hpp"
#include "maze-editor-tools/ecs/components/MazeGizmosController.hpp"
#include "maze-core/ecs/MazeComponentSystemHolder.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
#include "maze-particles/ecs/components/MazeParticlesDrawerController.hpp"
#include "ecs/components/EditorEventRetranslator.hpp"
#include "layout/EditorLayout.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "Editor.hpp"


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

        m_workspaceWorld.reset();
    }

    //////////////////////////////////////////
    void EditorEntityManager::Initialize(EditorEntityManagerPtr& _updateManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorEntityManager, _updateManager, init());
    }

    //////////////////////////////////////////
    bool EditorEntityManager::init()
    {
        m_workspaceWorld = EcsWorld::Create(
            MAZE_HS("Workspace"), true, MAZE_ECS_TAGS(MAZE_HS("render"), MAZE_HS("editor")));

        RenderSystemPtr renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();

        // RenderController
        {
            EntityPtr entity = m_workspaceWorld->createEntity();
            entity->createComponent<StaticName>("RenderController");
            m_renderController = entity->createComponent<RenderController>(renderSystem);
        }

        // Particles
        {
            EntityPtr entity = m_workspaceWorld->createEntity();
            entity->createComponent<StaticName>("ParticlesDrawerController");
            m_particlesDrawerController = entity->createComponent<ParticlesDrawerController>(renderSystem);
        }

        // 2D Input
        {
            EntityPtr entity = m_workspaceWorld->createEntity();
            entity->createComponent<StaticName>("InputSystem2D");
            m_inputSystem2D = entity->createComponent<InputSystem2D>();
            m_inputSystem2D->setCoordsConverter(
                [](Window* _window, Vec2F32 const& _coords)
            {
                if (_window == Editor::GetInstancePtr()->getMainRenderWindow()->getWindow().get())
                    return EditorLayout::ConvertRenderWindowCoordsToWorkspaceViewport(_coords);

                return _coords;
            });
        }

        // Event retranslators
        {
            EntityPtr graphicsEventRetranslator = m_workspaceWorld->createEntity();
            graphicsEventRetranslator->createComponent<StaticName>("GraphicsEventRetranslator");
            graphicsEventRetranslator->createComponent<GraphicsEventRetranslator>();

            EntityPtr editorEventRetranslator = m_workspaceWorld->createEntity();
            editorEventRetranslator->createComponent<StaticName>("EditorEventRetranslator");
            editorEventRetranslator->createComponent<EditorEventRetranslator>();
        }

        

        UpdateManager::GetInstancePtr()->addUpdatable(this);

        return true;
    }

    //////////////////////////////////////////
    void EditorEntityManager::start()
    {
        // GizmosController
        {
            RenderBufferPtr const& renderBuffer = EditorWorkspaceManager::GetInstancePtr()->getWorkspaceRenderBuffer();

            EntityPtr entity = m_workspaceWorld->createEntity();
            entity->createComponent<Name>("GizmosController");
            m_gizmosController = entity->createComponent<GizmosController>(renderBuffer.get());
        }
    }

    //////////////////////////////////////////
    void EditorEntityManager::update(F32 _dt)
    {
        if (!EditorManager::GetInstancePtr()->getPlaytestModeEnabled())
            m_workspaceWorld->update(_dt);
    }

} // namespace Maze
//////////////////////////////////////////
