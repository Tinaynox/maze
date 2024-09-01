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
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeComponentFactory.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-editor-tools/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-editor-tools/managers/MazeGizmosManager.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-editor-tools/layout/MazeEditorToolsStyles.hpp"
#include "maze-editor-tools/managers/MazeColorPickerManager.hpp"
#include "maze-editor-tools/managers/MazeColorGradientPickerManager.hpp"
#include "maze-editor-tools/managers/MazeAnimationCurveManager.hpp"
#include "maze-editor-tools/managers/MazeMaterialPickerManager.hpp"
#include "maze-editor-tools/managers/MazeRenderMeshPickerManager.hpp"
#include "maze-editor-tools/managers/MazeTexturePickerManager.hpp"
#include "maze-editor-tools/ecs/components/MazeColorEdit2D.hpp"
#include "maze-editor-tools/ecs/components/MazeColorHDREdit2D.hpp"
#include "maze-editor-tools/ecs/components/MazeColorGradientEdit2D.hpp"
#include "maze-editor-tools/ecs/components/MazeAnimationCurveEdit2D.hpp"
#include "maze-editor-tools/ecs/components/MazeGizmosController.hpp"


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
        m_editorActionManager.reset();
        m_selectionManager.reset();

        setGizmosController(nullptr);

        // EntityManager::GetInstancePtr()->getDefaultWorldRaw()->eventComponentSystemAdded.unsubscribe(this);

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

        EditorToolsActionManager::Initialize(m_editorActionManager);
        if (!m_editorActionManager)
            return false;

        EditorToolsStyles::Initialize(m_styles);
        if (!m_styles)
            return false;

        ColorPickerManager::Initialize(m_colorPickerManager);
        if (!m_colorPickerManager)
            return false;

        ColorGradientPickerManager::Initialize(m_colorGradientPickerManager);
        if (!m_colorGradientPickerManager)
            return false;

        AnimationCurveManager::Initialize(m_animationCurveManager);
        if (!m_animationCurveManager)
            return false;

        MaterialPickerManager::Initialize(m_materialPickerManager);
        if (!m_materialPickerManager)
            return false;

        RenderMeshPickerManager::Initialize(m_renderMeshPickerManager);
        if (!m_renderMeshPickerManager)
            return false;

        TexturePickerManager::Initialize(m_texturePickerManager);
        if (!m_texturePickerManager)
            return false;

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

        EditorToolsSettings* editorToolsSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorToolsSettings>();
        editorToolsSettings->getPauseChangedEvent().subscribe(this, &EditorToolsManager::notifyPauseChanged);


        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ColorEdit2D>("Editor Tools");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ColorHDREdit2D>("Editor Tools");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<ColorGradientEdit2D>("Editor Tools");
        EntityManager::GetInstancePtr()->getComponentFactory()->registerComponent<AnimationCurveEdit2D>("Editor Tools");

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
    void EditorToolsManager::setGizmosController(GizmosController* _gizmosController)
    {
        if (m_gizmosController == _gizmosController)
            return;

        if (m_gizmosController)
        {
            m_gizmosController->eventDrawGizmosEvent.unsubscribe(this);
        }

        m_gizmosController = _gizmosController;

        if (m_gizmosController)
        {
            m_gizmosController->eventDrawGizmosEvent.subscribe(this, &EditorToolsManager::notifyDrawGizmos);
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
