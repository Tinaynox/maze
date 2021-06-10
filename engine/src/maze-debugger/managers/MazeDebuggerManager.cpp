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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/managers/MazeDebuggerManager.hpp"
#include "maze-debugger/managers/MazeSelectionManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-debugger/scenes/SceneDebugEditor.hpp"
#include "maze-debugger/settings/MazeDebuggerSettings.hpp"
#include "maze-graphics/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-graphics/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-debugger/managers/MazeSelectionManager.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-debugger/managers/MazeAssetDebuggerManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class DebuggerManager
    //
    //////////////////////////////////////////
    DebuggerManager* DebuggerManager::s_instance = nullptr;

    //////////////////////////////////////////
    DebuggerManager::DebuggerManager()
        : m_debugEditorActive(false)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    DebuggerManager::~DebuggerManager()
    {
        setGizmosSystem(nullptr);

        EntityManager::GetInstancePtr()->getDefaultWorldRaw()->eventComponentSystemAdded.unsubscribe(this);

        closeDebugEditor();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void DebuggerManager::Initialize(DebuggerManagerPtr& _debuggerManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(DebuggerManager, _debuggerManager, init());
    }

    //////////////////////////////////////////
    bool DebuggerManager::init()
    {
        SettingsManager::GetInstancePtr()->registerSettings<DebuggerSettings>();

        SelectionManager::Initialize(m_selectionManager);
        if (!m_selectionManager)
            return false;

        InspectorManager::Initialize(m_inspectorManager);
        if (!m_inspectorManager)
            return false;

        AssetDebuggerManager::Initialize(m_assetDebuggerManager);
        if (!m_assetDebuggerManager)
            return false;

        EntityManager::GetInstancePtr()->getDefaultWorldRaw()->eventComponentSystemAdded.subscribe(this, &DebuggerManager::notifyComponentSystemAdded);

        return true;
    }

    //////////////////////////////////////////
    void DebuggerManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void DebuggerManager::openDebugEditor(RenderTargetPtr const& _renderTarget)
    {
        if (SceneManager::GetInstancePtr()->getScene<SceneDebugEditor>())
            return;

        SceneManager::GetInstancePtr()->loadScene<SceneDebugEditor>(true, _renderTarget);
        m_debugEditorActive = true;
    }

    //////////////////////////////////////////
    void DebuggerManager::closeDebugEditor()
    {
        if (SceneManager::GetInstancePtr())
        {
            SceneDebugEditorPtr scene = SceneManager::GetInstancePtr()->getScene<SceneDebugEditor>();
            if (scene)
            {
                SceneManager::GetInstancePtr()->destroyScene(scene);
                m_debugEditorActive = false;
            }
        }
    }

    //////////////////////////////////////////
    void DebuggerManager::notifyDrawGizmos(
        ComponentGizmosPtr const& _gizmos,
        Entity* _entity,
        ComponentPtr const& _component,
        GizmosDrawer* _drawer)
    {
        if (m_selectionManager->isObjectSelected(_entity->getSharedPtr()))
        {
            _gizmos->drawGizmosSelected(_entity, _component, _drawer);
        }
    }

    //////////////////////////////////////////
    void DebuggerManager::notifyComponentSystemAdded(ComponentSystemPtr const& _componentSystem)
    {
        if (_componentSystem->getClassUID() == GizmosSystem::GetMetaClass()->getClassUID())
        {
            setGizmosSystem(_componentSystem->cast<GizmosSystem>());
        }
    }

    //////////////////////////////////////////
    void DebuggerManager::setGizmosSystem(GizmosSystemPtr const& _gizmosSystem)
    {
        if (m_gizmosSystem == _gizmosSystem)
            return;

        if (m_gizmosSystem)
        {
            m_gizmosSystem->eventDrawGizmosEvent.unsubscribe(this);
        }

        m_gizmosSystem = _gizmosSystem;

        if (m_gizmosSystem)
        {
            m_gizmosSystem->eventDrawGizmosEvent.subscribe(this, &DebuggerManager::notifyDrawGizmos);
        }
    }
    
} // namespace Maze
//////////////////////////////////////////
