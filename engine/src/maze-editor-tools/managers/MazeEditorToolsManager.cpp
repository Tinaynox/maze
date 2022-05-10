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
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-editor-tools/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorToolsManager
    //
    //////////////////////////////////////////
    EditorToolsManager* EditorToolsManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorToolsManager::EditorToolsManager()
        : m_debugEditorActive(false)
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorToolsManager::~EditorToolsManager()
    {
        m_selectionManager.reset();

        setGizmosSystem(nullptr);

        EntityManager::GetInstancePtr()->getDefaultWorldRaw()->eventComponentSystemAdded.unsubscribe(this);

        if (SettingsManager::GetInstancePtr())
        {
            EditorToolsSettings* editorToolsSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
            editorToolsSettings->getPauseChangedEvent().unsubscribe(this);
        }

        closeDebugEditor();

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorToolsManager::Initialize(EditorToolsManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorToolsManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorToolsManager::init()
    {
        SettingsManager::GetInstancePtr()->registerSettings<EditorToolsSettings>();

        SelectionManager::Initialize(m_selectionManager);
        if (!m_selectionManager)
            return false;

        InspectorManager::Initialize(m_inspectorManager);
        if (!m_inspectorManager)
            return false;

        GizmosManager::Initialize(m_gizmosManager);
        if (!m_gizmosManager)
            return false;        

        AssetEditorToolsManager::Initialize(m_assetEditorToolsManager);
        if (!m_assetEditorToolsManager)
            return false;

        EntityManager::GetInstancePtr()->getDefaultWorldRaw()->eventComponentSystemAdded.subscribe(this, &EditorToolsManager::notifyComponentSystemAdded);

        EditorToolsSettings* editorToolsSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
        editorToolsSettings->getPauseChangedEvent().subscribe(this, &EditorToolsManager::notifyPauseChanged);

        updatePause();

        return true;
    }

    //////////////////////////////////////////
    void EditorToolsManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void EditorToolsManager::openDebugEditor(RenderTargetPtr const& _renderTarget)
    {
        if (SceneManager::GetInstancePtr()->getScene<SceneDebugEditor>())
            return;

        SceneManager::GetInstancePtr()->loadScene<SceneDebugEditor>(true, _renderTarget);
        m_debugEditorActive = true;
    }

    //////////////////////////////////////////
    void EditorToolsManager::closeDebugEditor()
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
    void EditorToolsManager::notifyDrawGizmos(
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
    void EditorToolsManager::notifyComponentSystemAdded(ComponentSystemPtr const& _componentSystem)
    {
        if (_componentSystem->getClassUID() == GizmosSystem::GetMetaClass()->getClassUID())
        {
            setGizmosSystem(_componentSystem->cast<GizmosSystem>());
        }
    }

    //////////////////////////////////////////
    void EditorToolsManager::setGizmosSystem(GizmosSystemPtr const& _gizmosSystem)
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
            m_gizmosSystem->eventDrawGizmosEvent.subscribe(this, &EditorToolsManager::notifyDrawGizmos);
        }
    }

    //////////////////////////////////////////
    void EditorToolsManager::notifyPauseChanged(bool const& _value)
    {
        updatePause();
    }

    //////////////////////////////////////////
    void EditorToolsManager::updatePause()
    {
        EditorToolsSettings* editorToolsSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
        if (editorToolsSettings->getPause() && !m_pause)
            m_pause = UniquePtr<ScopedPause>(new ScopedPause());
        else
            m_pause.reset();
    }
    
} // namespace Maze
//////////////////////////////////////////
