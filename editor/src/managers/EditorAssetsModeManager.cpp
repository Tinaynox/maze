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
#include "EditorAssetsModeManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
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
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorAssetsModeManager
    //
    //////////////////////////////////////////
    EditorAssetsModeManager* EditorAssetsModeManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorAssetsModeManager::EditorAssetsModeManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorAssetsModeManager::~EditorAssetsModeManager()
    {
        s_instance = nullptr;

        if (SettingsManager::GetInstancePtr())
        {
            EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();
            if (editorSettings)
            {
                editorSettings->getEditorModeChangedEvent().unsubscribe(this);
                editorSettings->getAssetsFullPathChangedEvent().unsubscribe(this);
            }
        }
    }

    //////////////////////////////////////////
    void EditorAssetsModeManager::Initialize(EditorAssetsModeManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorAssetsModeManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorAssetsModeManager::init()
    {
        EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();
        editorSettings->getEditorModeChangedEvent().subscribe(this, &EditorAssetsModeManager::notifyEditorModeChanged);
        editorSettings->getAssetsFullPathChangedEvent().subscribe(this, &EditorAssetsModeManager::notifyAssetsFullPathChanged);
        
        updateAssets();

        return true;
    }

    //////////////////////////////////////////
    void EditorAssetsModeManager::notifyEditorModeChanged(EditorMode const& _mode)
    {
        updateAssets();
    }

    //////////////////////////////////////////
    void EditorAssetsModeManager::notifyAssetsFullPathChanged(String const& _mode)
    {
        updateAssets();
    }

    //////////////////////////////////////////
    void EditorAssetsModeManager::updateAssets()
    {
        EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();
        EditorMode editorMode = editorSettings->getEditorMode();
        String assetsFullPath = editorSettings->getAssetsFullPath();
        bool isAssetsFullPathValid = !assetsFullPath.empty() && FileHelper::IsDirectory(assetsFullPath);

        if (editorMode == EditorMode::Assets && isAssetsFullPathValid)
            setCurrentAssetsFolder(assetsFullPath);
        else
            setCurrentAssetsFolder(String());

        AssetManager::GetInstancePtr()->updateAssets();
    }

    //////////////////////////////////////////
    void EditorAssetsModeManager::setCurrentAssetsFolder(String const& _folder)
    {
        if (m_currentAssetsFullPath == _folder)
            return;

        if (!m_currentAssetsFullPath.empty())
        {
            AssetManager::GetInstancePtr()->removeAssetsDirectoryPath(m_currentAssetsFullPath);
        }

        m_currentAssetsFullPath = _folder;

        if (!m_currentAssetsFullPath.empty())
        {
            AssetManager::GetInstancePtr()->addAssetsDirectoryPath(m_currentAssetsFullPath);
        }

        eventCurrentAssetsFullPath(m_currentAssetsFullPath);
    }

} // namespace Maze
//////////////////////////////////////////
