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
#include "helpers/EditorHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/helpers/MazeSystemDialogHelper.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-core/ecs/systems/MazeTransformEventsSystem.hpp"
#include "maze-graphics/ecs/systems/MazeRenderControlSystem.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/helpers/MazeMeshHelper.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
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
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-particles/ecs/components/MazeParticleSystem3D.hpp"
#include "maze-ui/ecs/components/MazeClickButton2D.hpp"
#include "maze-ui/ecs/components/MazeUIElement2D.hpp"
#include "maze-render-system-opengl-core/MazeVertexArrayObjectOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeContextOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeTexture2DOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeShaderUniformOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeStateMachineOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderQueueOpenGL.hpp"
#include "maze-render-system-opengl-core/MazeRenderWindowOpenGL.hpp"
#include "maze-editor-tools/ecs/components/MazeHierarchyController.hpp"
#include "maze-editor-tools/ecs/components/MazeInspectorController.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorPrefabManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "scenes/SceneEditor.hpp"
#include "scenes/SceneSelectMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EditorHelper
    {
        //////////////////////////////////////////
        bool IsValidSceneMode()
        {
            return EditorManager::GetInstancePtr()->getSceneMode() != EditorSceneMode::None;
        }

        //////////////////////////////////////////
        EntityPtr CreateNewPrefab2D()
        {
            EntityPtr gameObject = EditorManager::GetInstancePtr()->createNewPrefab();
            gameObject->ensureComponent<Transform2D>();
            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateNewPrefab3D()
        {
            EntityPtr gameObject = EditorManager::GetInstancePtr()->createNewPrefab();
            gameObject->ensureComponent<Transform3D>();
            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateEntity(CString _entityName)
        {
            switch (EditorManager::GetInstancePtr()->getSceneMode())
            {
                case EditorSceneMode::Scene:
                    return EditorToolsHelper::CreateEntity(_entityName);

                case EditorSceneMode::Prefab:
                    MAZE_ERROR_RETURN_VALUE(EntityPtr(), "Forbidden operation!");

                default:
                    MAZE_NOT_IMPLEMENTED;
                    return EntityPtr();
            }

        }

        //////////////////////////////////////////
        EntityPtr CreateEntity2D(CString _entityName)
        {
            switch (EditorManager::GetInstancePtr()->getSceneMode())
            {
                case EditorSceneMode::Scene:
                {
                    return EditorToolsHelper::CreateEntity2D(_entityName);
                }
                case EditorSceneMode::Prefab:
                {

                    EntityPtr const& prefabEntity = EditorPrefabManager::GetInstancePtr()->getPrefabEntity();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabEntity, EntityPtr(), "Prefab entity is null!");
                    Transform2DPtr prefabTransform = prefabEntity->getComponent<Transform2D>();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabTransform, EntityPtr(), "Prefab has no Transform2D!");

                    EntityPtr entity = EditorToolsHelper::CreateEntity2D(_entityName);
                    entity->getComponent<Transform2D>()->setParent(prefabTransform);
                    return entity;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                    return EntityPtr();
                }
            }
        }

        //////////////////////////////////////////
        EntityPtr CreateEntity3D(CString _entityName)
        {
            switch (EditorManager::GetInstancePtr()->getSceneMode())
            {
                case EditorSceneMode::Scene:
                {
                    return EditorToolsHelper::CreateEntity3D(_entityName);
                }
                case EditorSceneMode::Prefab:
                {

                    EntityPtr const& prefabEntity = EditorPrefabManager::GetInstancePtr()->getPrefabEntity();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabEntity, EntityPtr(), "Prefab entity is null!");
                    Transform3DPtr prefabTransform = prefabEntity->getComponent<Transform3D>();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabTransform, EntityPtr(), "Prefab has no Transform2D!");

                    EntityPtr entity = EditorToolsHelper::CreateEntity3D(_entityName);
                    entity->getComponent<Transform3D>()->setParent(prefabTransform);
                    return entity;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                    return EntityPtr();
                }
            }
        }

        //////////////////////////////////////////
        EntityPtr CreateCube(CString _entityName)
        {
            EntityPtr gameObject = CreateEntity3D(_entityName);
            if (!gameObject)
                return EntityPtr();

            MeshRendererPtr meshRenderer = gameObject->ensureComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultCubeMesh());
            meshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::Specular));

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateSphere(CString _entityName)
        {
            EntityPtr gameObject = CreateEntity3D(_entityName);
            if (!gameObject)
                return EntityPtr();

            MeshRendererPtr meshRenderer = gameObject->ensureComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getDefaultSphereMesh());
            meshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::Specular));

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateDirectionalLight(CString _entityName)
        {
            EntityPtr gameObject = CreateEntity3D(_entityName);
            if (!gameObject)
                return EntityPtr();

            Light3DPtr light3D = gameObject->ensureComponent<Light3D>();
            light3D->setLightType(Light3DType::Directional);
            light3D->setColor(ColorU32(255, 244, 214));
            light3D->getTransform()->setLocalDirection(0.377f, -0.777f, 0.577f);

            light3D->getTransform()->setLocalY(5.0f);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateNewParticleSystem3D(CString _entityName)
        {
            EntityPtr gameObject = CreateEntity3D(_entityName);
            if (!gameObject)
                return EntityPtr();

            ParticleSystem3DPtr particleSystem = gameObject->ensureComponent<ParticleSystem3D>();

            particleSystem->getTransform()->setLocalRotation(Quaternion(-Math::c_halfPi, 0.0f, 0.0f));
            particleSystem->getShapeModule().setType(ParticleSystem3DZoneType::Hemisphere);
            particleSystem->getMainModule().getSpeed().setConstant(5.0f);
            particleSystem->setMaterial(ParticlesManager::GetInstancePtr()->getDefaultParticleMaterial());
            particleSystem->getMainModule().getEmission().emissionPerSecond.setConstant(30.0f);

            return gameObject;
        }

        //////////////////////////////////////////
        bool SaveValidate()
        {
            return EditorManager::GetInstancePtr()->getMode() == EditorMode::Project;
        }

        //////////////////////////////////////////
        void Save()
        {
            if (!SaveValidate())
                return;

            switch (EditorManager::GetInstancePtr()->getSceneMode())
            {
                case EditorSceneMode::Scene:
                {
                    MAZE_NOT_IMPLEMENTED;
                    break;
                }
                case EditorSceneMode::Prefab:
                {
                    String fullPath;
                    
                    if (EditorManager::GetInstancePtr()->getCurrentEditFileFullPath().empty())
                        fullPath = SystemDialogHelper::SaveFile(
                            "Maze Prefab (*.mzprefab)\0*.mzprefab\0",
                            Editor::GetInstancePtr()->getMainRenderWindow()->getWindow().get());
                    else
                        fullPath = EditorManager::GetInstancePtr()->getCurrentEditFileFullPath();

                    if (!fullPath.empty())
                    {
                        EntityPtr const& prefabEntity = EditorPrefabManager::GetInstancePtr()->getPrefabEntity();
                        EntitySerializationManager::GetInstancePtr()->savePrefabToXMLFile(prefabEntity, fullPath);

                        EditorManager::GetInstancePtr()->setCurrentEditFileFullPath(fullPath);
                    }

                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                }
            }
        }

        //////////////////////////////////////////
        bool SaveAsValidate()
        {
            return EditorManager::GetInstancePtr()->getMode() == EditorMode::Project;
        }

        //////////////////////////////////////////
        void SaveAs()
        {
            if (!SaveAsValidate())
                return;

            switch (EditorManager::GetInstancePtr()->getSceneMode())
            {
                case EditorSceneMode::Scene:
                {
                    MAZE_NOT_IMPLEMENTED;
                    break;
                }
                case EditorSceneMode::Prefab:
                {
                    String fullPath = SystemDialogHelper::SaveFile(
                        "Maze Prefab (*.mzprefab)\0*.mzprefab\0",
                        Editor::GetInstancePtr()->getMainRenderWindow()->getWindow().get());

                    if (!fullPath.empty())
                    {
                        EntityPtr const& prefabEntity = EditorPrefabManager::GetInstancePtr()->getPrefabEntity();
                        EntitySerializationManager::GetInstancePtr()->savePrefabToXMLFile(prefabEntity, fullPath);

                        EditorManager::GetInstancePtr()->setCurrentEditFileFullPath(fullPath);
                    }

                    break;
                }
                default:
                {
                    MAZE_NOT_IMPLEMENTED;
                }
            }
        }

        //////////////////////////////////////////
        bool LoadValidate()
        {
            return EditorManager::GetInstancePtr()->getMode() == EditorMode::Project;
        }

        //////////////////////////////////////////
        void Load()
        {
            if (!LoadValidate())
                return;

            Clear();

            String fullPath = SystemDialogHelper::OpenFile(
                "Maze Prefab (*.mzprefab)\0*.mzprefab\0",
                Editor::GetInstancePtr()->getMainRenderWindow()->getWindow().get());

            if (!fullPath.empty())
            {
                EntityPtr prefab = EntitySerializationManager::GetInstancePtr()->loadPrefabFromXMLFile(fullPath);
                EditorPrefabManager::GetInstancePtr()->setPrefabEntity(prefab);
                EditorManager::GetInstancePtr()->setSceneMode(EditorSceneMode::Prefab);

                EditorManager::GetInstancePtr()->setCurrentEditFileFullPath(fullPath);
            }
        }

        //////////////////////////////////////////
        bool ClearValidate()
        {
            return EditorManager::GetInstancePtr()->getSceneMode() != EditorSceneMode::None;
        }

        //////////////////////////////////////////
        void Clear()
        {
            if (!ClearValidate())
                return;

            EditorManager::GetInstancePtr()->setCurrentEditFileFullPath(String());

            EditorWorkspaceManager::GetInstancePtr()->clearWorkspace();

            EditorManager::GetInstancePtr()->setSceneMode(EditorSceneMode::Scene);
        }
    };

} // namespace Maze
//////////////////////////////////////////
