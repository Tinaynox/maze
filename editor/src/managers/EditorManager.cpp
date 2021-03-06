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
#include "EditorManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-core/math/MazeMathGeometry.hpp"
#include "maze-graphics/MazeMesh.hpp"
#include "maze-graphics/MazeSubMesh.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/MazeShaderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/MazeGPUTextureBuffer.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/loaders/mesh/MazeLoaderOBJ.hpp"
#include "maze-graphics/MazeRenderMesh.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-gamepad/managers/MazeGamepadManager.hpp"
#include "maze-gamepad/gamepad/MazeGamepad.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-physics2d/ecs/components/MazeBoxCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeCircleCollider2D.hpp"
#include "maze-physics2d/ecs/components/MazeRigidbody2D.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/ecs/systems/MazeGizmosSystem.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "Editor.hpp"
#include "managers/EditorAssetsManager.hpp"
#include "managers/EditorAssetsModeManager.hpp"
#include "managers/EditorProjectModeManager.hpp"
#include "managers/EditorGizmosManager.hpp"
#include "managers/EditorEntityManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "managers/EditorPlaytestManager.hpp"
#include "managers/EditorUpdateManager.hpp"
#include "layout/EditorLayout.hpp"
#include "scenes/SceneWorkspace.hpp"
#include "scenes/SceneWorkspaceTools.hpp"
#include "scenes/ScenePlaytest.hpp"
#include "scenes/ScenePlaytestTools.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorManager
    //
    //////////////////////////////////////////
    EditorManager* EditorManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorManager::EditorManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorManager::~EditorManager()
    {
        s_instance = nullptr;        
    }

    //////////////////////////////////////////
    void EditorManager::Initialize(EditorManagerPtr& _playerManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorManager, _playerManager, init());
    }

    //////////////////////////////////////////
    bool EditorManager::init()
    {
        EditorAssetsManager::Initialize(m_editorAssetsManager);
        if (!m_editorAssetsManager)
            return false;

        EditorPrefabManager::Initialize(m_editorPrefabManager);
        if (!m_editorPrefabManager)
            return false;

        EditorAssetsModeManager::Initialize(m_editorAssetsModeManager);
        if (!m_editorAssetsModeManager)
            return false;

        EditorProjectModeManager::Initialize(m_editorProjectModeManager);
        if (!m_editorProjectModeManager)
            return false;

        EditorGizmosManager::Initialize(m_editorGizmosManager);
        if (!m_editorGizmosManager)
            return false;

        EditorEntityManager::Initialize(m_editorEntityManager);
        if (!m_editorEntityManager)
            return false;

        EditorWorkspaceManager::Initialize(m_editorWorkspaceManager);
        if (!m_editorWorkspaceManager)
            return false;

        EditorPlaytestManager::Initialize(m_editorPlaytestManager);
        if (!m_editorPlaytestManager)
            return false;

        EditorUpdateManager::Initialize(m_editorUpdateManager);
        if (!m_editorUpdateManager)
            return false;

        return true;
    }

    //////////////////////////////////////////
    EditorMode EditorManager::getMode() const
    {
        EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();
        if (!editorSettings)
            return EditorMode::None;

        return editorSettings->getEditorMode();
    }

    //////////////////////////////////////////
    void EditorManager::setSceneMode(EditorSceneMode _mode)
    {
        if (m_sceneMode == _mode)
            return;

        m_sceneMode = _mode;

        eventSceneModeChanged(m_sceneMode);
    }

    //////////////////////////////////////////
    void EditorManager::clearMode()
    {
        setSceneMode(EditorSceneMode::None);
        m_editorWorkspaceManager->clearWorkspace();
        m_editorPlaytestManager->clearWorkspace();

        m_editorPrefabManager->setPrefabAssetFile(nullptr);
        m_editorPrefabManager->setPrefabEntity(nullptr);

        setWindowTitle("Editor");
    }

    //////////////////////////////////////////
    void EditorManager::openPrefab(EntityPtr const& _value)
    {
        setSceneMode(EditorSceneMode::Prefab);
        getSceneMain()->destroyAllEntitiesExcept(_value);
        m_editorPrefabManager->setPrefabAssetFile(nullptr);
        m_editorPrefabManager->setPrefabEntity(_value);

        setWindowTitle("Editor - New Prefab");
    }

    //////////////////////////////////////////
    void EditorManager::openPrefab(AssetFilePtr const& _value)
    {
        if (getSceneMode() == EditorSceneMode::Prefab &&
            m_editorPrefabManager->getPrefabAssetFile() == _value)
            return;
        
        setSceneMode(EditorSceneMode::Prefab);
        getSceneMain()->destroyAllEntities();
        m_editorPrefabManager->setPrefabAssetFile(_value);

        setWindowTitle("Editor - %s", _value->getFileName().c_str());
    }

    //////////////////////////////////////////
    EntityPtr EditorManager::createNewPrefab()
    {        
        EntityPtr gameObject = getSceneMain()->createEntity("Entity");
        openPrefab(gameObject);

        return gameObject;
    }

    //////////////////////////////////////////
    void EditorManager::start()
    {
        m_editorWorkspaceManager->start();
        m_editorPlaytestManager->start();
        m_editorEntityManager->start();

        SceneManager::GetInstancePtr()->setMainScene(getSceneMain());

        EditorToolsManager::GetInstancePtr()->setGizmosSystem(getMainECSWorld()->getSystem<GizmosSystem>());
    }

    //////////////////////////////////////////
    void EditorManager::setWindowTitle(CString _title, ...)
    {
        String text;
        MAZE_FORMAT_VA_STRING(_title, text);
        setWindowTitle(text);
    }

    //////////////////////////////////////////
    void EditorManager::setWindowTitle(String const& _title)
    {
        Editor::GetInstancePtr()->getMainRenderWindow()->getWindow()->setTitle(_title);
    }

    //////////////////////////////////////////
    void EditorManager::setPlaytestModeEnabled(bool _value)
    {
        if (m_playtestModeEnabled == _value)
            return;

        
        if (_value)
        {
            EditorPrefabManager::GetInstancePtr()->saveAssetFile();
        }

        Mat4DF prevCameraTransform = getSceneMainTools()->getCamera3D()->getTransform()->getLocalTransform();
        Vec3DF cameraTargetPosition = getSceneMainTools()->getCamera3DTargetPosition();
        F32 yawAngle = getSceneMainTools()->getYawAngle();
        F32 pitchAngle = getSceneMainTools()->getPitchAngle();

        m_playtestModeEnabled = _value;

        if (m_playtestModeEnabled)
        {
            m_editorWorkspaceManager->destroyScenes();
            m_editorPlaytestManager->createScenes();
        }
        else
        {
            m_editorPlaytestManager->destroyScenes();
            m_editorWorkspaceManager->createScenes();
        }

        SceneManager::GetInstancePtr()->setMainScene(getSceneMain());

        EditorToolsManager::GetInstancePtr()->setGizmosSystem(getMainECSWorld()->getSystem<GizmosSystem>());

        if (m_editorPrefabManager->getPrefabAssetFile())
            m_editorPrefabManager->updatePrefabAssetFile();

        getSceneMainTools()->getCamera3D()->getTransform()->setLocalTransform(prevCameraTransform);
        getSceneMainTools()->setCamera3DTargetPosition(cameraTargetPosition);
        getSceneMainTools()->setYawAngle(yawAngle);
        getSceneMainTools()->setPitchAngle(pitchAngle);

        eventPlaytestModeEnabledChanged(m_playtestModeEnabled);
    }

    //////////////////////////////////////////
    SceneMainPtr EditorManager::getSceneMain() const
    {
        if (m_playtestModeEnabled)
            return m_editorPlaytestManager->getScenePlaytest();
        else
            return m_editorWorkspaceManager->getSceneWorkspace();
    }

    //////////////////////////////////////////
    SceneMainToolsPtr EditorManager::getSceneMainTools() const
    {
        if (m_playtestModeEnabled)
            return m_editorPlaytestManager->getScenePlaytestTools();
        else
            return m_editorWorkspaceManager->getSceneWorkspaceTools();
    }

    //////////////////////////////////////////
    ECSWorld* EditorManager::getMainECSWorld() const
    {
        if (m_playtestModeEnabled)
            return EntityManager::GetInstancePtr()->getDefaultWorldRaw();
        else
            return EditorEntityManager::GetInstancePtr()->getWorkspaceWorld().get();
    }

} // namespace Maze
//////////////////////////////////////////
