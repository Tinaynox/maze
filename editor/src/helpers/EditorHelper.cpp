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
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/ecs/components/MazeStaticName.hpp"
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
#include "maze-ui/managers/MazeUIManager.hpp"
#include "maze-ui/managers/MazeFontMaterialManager.hpp"
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
#include "maze-editor-tools/editor-actions/MazeEditorActionActionsGroup.hpp"
#include "maze-editor-tools/editor-actions/MazeEditorActionSelectEntities.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsUIHelper.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorPrefabManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "managers/EditorSceneManager.hpp"
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "scenes/SceneEditor.hpp"


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
        void ProcessEditorActionsForCreatedEntity(
            EntityPtr const& _gameObject,
            bool _select)
        {
            if (EditorToolsActionManager::GetInstancePtr())
            {
                EditorActionActionsGroupPtr group = EditorActionActionsGroup::Create();

                group->addAction(EditorActionEntityAdd::Create(_gameObject));
                if (_select)
                    group->addAction(EditorActionSelectEntities::Create(
                        true,
                        _gameObject));

                EditorToolsActionManager::GetInstancePtr()->applyAction(group);
            }
            else
            {
                if (_select)
                    SelectionManager::GetInstancePtr()->selectObject(_gameObject);
            }
        }


        //////////////////////////////////////////
        EntityPtr CreateNewPrefab2D(
            Transform2DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = EditorManager::GetInstancePtr()->createNewPrefab();
            Transform2DPtr const& transform2D = gameObject->ensureComponent<Transform2D>();
            if (transform2D)
                transform2D->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateNewPrefab3D(
            Transform3DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = EditorManager::GetInstancePtr()->createNewPrefab();
            Transform3DPtr const& transform3D = gameObject->ensureComponent<Transform3D>();
            if (transform3D)
                transform3D->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateEntity(CString _entityName)
        {
            switch (EditorManager::GetInstancePtr()->getSceneMode())
            {
                case EditorSceneMode::Scene:
                    return EditorToolsHelper::CreateEntity(
                        _entityName,
                        EditorSceneManager::GetInstancePtr()->getScene().get());

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
                    return EditorToolsHelper::CreateEntity2D(
                        _entityName,
                        EditorSceneManager::GetInstancePtr()->getScene().get());
                }
                case EditorSceneMode::Prefab:
                {
                    EntityPtr const& prefabEntity = EditorPrefabManager::GetInstancePtr()->getPrefabEntity();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabEntity, EntityPtr(), "Prefab entity is null!");
                    Transform2DPtr prefabTransform = prefabEntity->getComponent<Transform2D>();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabTransform, EntityPtr(), "Prefab has no Transform2D!");

                    EntityPtr entity = EditorToolsHelper::CreateEntity2D(
                        _entityName,
                        EditorManager::GetInstancePtr()->getActiveScene().get());
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
        EntityPtr CreateEntity2D(
            CString _entityName,
            Transform2DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity2D(_entityName);
            if (!gameObject)
                return EntityPtr();

            Transform2D* transform2D = gameObject->getComponentRaw<Transform2D>();
            if (_parent)
                transform2D->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateSprite2D(
            CString _entityName,
            Transform2DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity2D(_entityName);
            if (!gameObject)
                return EntityPtr();

            Maze::SpriteRenderer2DPtr spriteRenderer = gameObject->createComponent<SpriteRenderer2D>();
            spriteRenderer->setSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Panel02));
            spriteRenderer->setMaterialCopy(SpriteManager::GetCurrentInstance()->getDefaultSpriteMaterial());

            if (spriteRenderer->getSprite())
            {
                if (spriteRenderer->getSprite()->getSliceBorder().isBorderExists())
                    spriteRenderer->setRenderMode(SpriteRenderMode::Sliced);
            }

            if (_parent)
                spriteRenderer->getTransform()->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateToggleButton2D(
            CString _entityName,
            Transform2DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity2D(_entityName);
            if (!gameObject)
                return EntityPtr();

            ToggleButton2DPtr button = EditorToolsUIHelper::CreateDefaultToggleButton(
                gameObject,
                _parent,
                gameObject->getEcsScene());
            button->setCheckByClick(false);
            SpriteRenderer2DPtr iconSprite = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Hamburger),
                Vec2F32(18.0f, 18.0f),
                Vec2F32::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                button->getTransform(),
                gameObject->getEcsScene());
            iconSprite->setColor(85, 85, 85);
            iconSprite->getEntityRaw()->ensureComponent<Name>("Icon");

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateClickButton2D(
            CString _entityName,
            Transform2DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity2D(_entityName);
            if (!gameObject)
                return EntityPtr();

            ClickButton2DPtr button = EditorToolsUIHelper::CreateDefaultClickButton(
                gameObject,
                _parent,
                gameObject->getEcsScene());
            SpriteRenderer2DPtr iconSprite = SpriteHelper::CreateSprite(
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Hamburger),
                Vec2F32(18.0f, 18.0f),
                Vec2F32::c_zero,
                MaterialManager::GetCurrentInstance()->getSpriteMaterial(),
                button->getTransform(),
                gameObject->getEcsScene());
            iconSprite->setColor(85, 85, 85);
            iconSprite->getEntityRaw()->ensureComponent<Name>("Icon");

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateText(
            CString _entityName,
            Transform2DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity2D(_entityName);
            if (!gameObject)
                return EntityPtr();

            TextRenderer2DPtr textRenderer = gameObject->createComponent<TextRenderer2D>();
            textRenderer->setText("Text");
            textRenderer->setFontSize(12u);
            textRenderer->setHorizontalAlignment(HorizontalAlignment2D::Center);
            textRenderer->setVerticalAlignment(VerticalAlignment2D::Middle);

            FontMaterialPtr const& fontMaterial = FontMaterialManager::GetInstancePtr()->getDefaultFontMaterial();
            if (fontMaterial)
                textRenderer->setFontMaterial(fontMaterial);

            Transform2DPtr transform = gameObject->ensureComponent<Transform2D>();
            transform->setParent(_parent);
            transform->setSize(60.0f, 14.0f);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateEntity3D(CString _entityName)
        {
            switch (EditorManager::GetInstancePtr()->getSceneMode())
            {
                case EditorSceneMode::Scene:
                {
                    return EditorToolsHelper::CreateEntity3D(
                        _entityName,
                        EditorSceneManager::GetInstancePtr()->getScene().get());
                }
                case EditorSceneMode::Prefab:
                {

                    EntityPtr const& prefabEntity = EditorPrefabManager::GetInstancePtr()->getPrefabEntity();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabEntity, EntityPtr(), "Prefab entity is null!");
                    Transform3DPtr prefabTransform = prefabEntity->getComponent<Transform3D>();
                    MAZE_ERROR_RETURN_VALUE_IF(!prefabTransform, EntityPtr(), "Prefab has no Transform2D!");

                    EntityPtr entity = EditorToolsHelper::CreateEntity3D(
                        _entityName,
                        EditorManager::GetInstancePtr()->getActiveScene().get());
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
        EntityPtr CreateBuiltinMesh(
            BuiltinRenderMeshType _meshType,
            Transform3DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity3D(_meshType.toCString());
            if (!gameObject)
                return EntityPtr();

            MeshRendererPtr meshRenderer = gameObject->ensureComponent<MeshRenderer>();
            meshRenderer->setRenderMesh(RenderMeshManager::GetCurrentInstancePtr()->getBuiltinRenderMesh(_meshType));
            meshRenderer->setMaterial(MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::Specular));

            if (_parent)
                gameObject->ensureComponent<Transform3D>()->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateDirectionalLight(
            CString _entityName,
            Transform3DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity3D(_entityName);
            if (!gameObject)
                return EntityPtr();

            Light3DPtr light3D = gameObject->ensureComponent<Light3D>();
            light3D->setLightType(Light3DType::Directional);
            light3D->setColor(ColorU32(255, 244, 214));

            if (_parent)
                light3D->getTransform()->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreatePointLight(
            CString _entityName,
            Transform3DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity3D(_entityName);
            if (!gameObject)
                return EntityPtr();

            Light3DPtr light3D = gameObject->ensureComponent<Light3D>();
            light3D->setLightType(Light3DType::Point);
            light3D->setRadius(5.0f);
            light3D->setColor(ColorU32(255, 244, 214));

            if (_parent)
                light3D->getTransform()->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateCamera3D(
            CString _entityName,
            Transform3DPtr const& _parent,
            bool _select)
        {
            EntityPtr gameObject = CreateEntity3D(_entityName);
            if (!gameObject)
                return EntityPtr();

            Camera3DPtr camera3D = gameObject->ensureComponent<Camera3D>();

            if (_parent)
                camera3D->getTransform()->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        EntityPtr CreateNewParticleSystem3D(
            CString _entityName,
            Transform3DPtr const& _parent,
            bool _select)
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

            if (_parent)
                particleSystem->getTransform()->setParent(_parent);

            ProcessEditorActionsForCreatedEntity(gameObject, _select);

            return gameObject;
        }

        //////////////////////////////////////////
        bool SaveValidate()
        {
            return true;
        }

        //////////////////////////////////////////
        void Save()
        {
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
                        EntitySerializationManager::GetInstancePtr()->savePrefabToDataBlockFile(prefabEntity, fullPath);

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
            return true;
        }

        //////////////////////////////////////////
        void SaveAs()
        {

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
                        EntitySerializationManager::GetInstancePtr()->savePrefabToDataBlockFile(prefabEntity, fullPath);

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
            return true;
        }

        //////////////////////////////////////////
        void Load()
        {
            Clear();

            String fullPath = SystemDialogHelper::OpenFile(
                "Maze Prefab (*.mzprefab)\0*.mzprefab\0",
                Editor::GetInstancePtr()->getMainRenderWindow()->getWindow().get());

            if (!fullPath.empty())
            {
                EntityPtr prefab = EntitySerializationManager::GetInstancePtr()->loadPrefab(fullPath);
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
