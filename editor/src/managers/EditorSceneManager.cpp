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
#include "EditorSceneManager.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
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
#include "maze-editor-tools/editor-actions/MazeEditorAction.hpp"
#include "maze-editor-tools/managers/MazeEditorToolsActionManager.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "Editor.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorSceneManager
    //
    //////////////////////////////////////////
    EditorSceneManager* EditorSceneManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorSceneManager::EditorSceneManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorSceneManager::~EditorSceneManager()
    {
        if (EditorToolsActionManager::GetInstancePtr())
            EditorToolsActionManager::GetInstancePtr()->getLastChangeTimestampChangedEvent().unsubscribe(this);

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorSceneManager::Initialize(EditorSceneManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorSceneManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorSceneManager::init()
    {
        EditorToolsActionManager::GetInstancePtr()->getLastChangeTimestampChangedEvent().subscribe(
            this, &EditorSceneManager::notifyLastChangeTimestampChanged);
        m_sceneAssetFileSaveTimestamp.eventValueChanged.subscribe(
            this, &EditorSceneManager::notifySceneAssetFileSaveTimestampChanged);
        
        return true;
    }

    //////////////////////////////////////////
    void EditorSceneManager::setScene(EcsAssetScenePtr const& _value)
    {
        if (m_scene == _value)
            return;

        m_scene = _value;

        eventSceneChanged(m_scene);
    }

    //////////////////////////////////////////
    void EditorSceneManager::setSceneAssetFile(AssetFilePtr const& _value)
    {
        if (m_sceneAssetFile == _value)
            return;

        m_sceneAssetFile = _value;

        updateSceneAssetFile();

        m_sceneAssetFileSaveTimestamp = EditorAction::GetCurrentTimestamp();

        eventSceneAssetFileChanged(m_sceneAssetFile);
    }

    //////////////////////////////////////////
    void EditorSceneManager::updateSceneAssetFile()
    {
        if (m_sceneAssetFile)
        {
            DataBlock dataBlock = m_sceneAssetFile->readAsDataBlock();
            
            EcsAssetScenePtr scene = EcsAssetScene::Create(Editor::GetInstancePtr()->getMainRenderWindow());

            DataBlock* entitiesBlock = dataBlock.getDataBlock(MAZE_HCS("entities"));

            if (entitiesBlock)
            {
                EntitySerializationManager::GetInstancePtr()->loadEntitiesToScene(
                    scene.get(),
                    *entitiesBlock,
                    EditorManager::GetInstancePtr()->getSceneMain()->getWorld());
            }

            setScene(scene);
        }
        else
        {
            setScene(nullptr);
        }
    }
    
    //////////////////////////////////////////
    void EditorSceneManager::saveAssetFile()
    {
        if (!m_sceneAssetFile || !m_scene)
            return;

        EntitySerializationManager::GetInstancePtr()->saveSceneToDataBlockFile(
            m_scene, m_sceneAssetFile->getFullPath());

        m_sceneAssetFileSaveTimestamp = EditorAction::GetCurrentTimestamp();
    }

    //////////////////////////////////////////
    void EditorSceneManager::updateSceneAssetFileSaveEnabled()
    {
        U32 lastChangeTimestamp = EditorToolsActionManager::GetInstancePtr()->getLastChangeTimestamp();
        U32 sceneAssetFileSaveTimestamp = m_sceneAssetFileSaveTimestamp.getValue();

        m_sceneAssetFileSaveEnabled = (sceneAssetFileSaveTimestamp < lastChangeTimestamp);
    }

    //////////////////////////////////////////
    void EditorSceneManager::notifyLastChangeTimestampChanged(U32 const& _value)
    {
        updateSceneAssetFileSaveEnabled();
    }

    //////////////////////////////////////////
    void EditorSceneManager::notifySceneAssetFileSaveTimestampChanged(U32 const& _value)
    {
        updateSceneAssetFileSaveEnabled();
    }

} // namespace Maze
//////////////////////////////////////////
