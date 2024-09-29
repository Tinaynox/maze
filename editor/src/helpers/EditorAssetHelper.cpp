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
#include "helpers/EditorAssetHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-core/ecs/MazeEntity.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeEntitySerializationManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/helpers/MazeSystemDialogHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-graphics/ecs/components/MazeCamera3D.hpp"
#include "maze-graphics/ecs/components/MazeCanvas.hpp"
#include "maze-graphics/ecs/components/MazeCanvasScaler.hpp"
#include "maze-graphics/ecs/components/MazeCanvasGroup.hpp"
#include "maze-graphics/ecs/components/MazeRenderMask.hpp"
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/components/MazeMeshRenderer.hpp"
#include "maze-graphics/ecs/components/MazeLight3D.hpp"
#include "maze-graphics/ecs/MazeEcsAssetScene.hpp"
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
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-particles/managers/MazeParticlesManager.hpp"
#include "managers/EditorManager.hpp"
#include "managers/EditorPrefabManager.hpp"
#include "managers/EditorWorkspaceManager.hpp"
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "scenes/SceneEditor.hpp"
#include "helpers/EditorHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace EditorAssetHelper
    {
        //////////////////////////////////////////
        void CreateFolder(AssetsController* _controller, Path const& _fullPath)
        {
            String dir = FileHelper::GetDirectoryInPath(_fullPath);
            String newFolderFullPath = EditorToolsHelper::BuildNewAssetFileName(dir + "/New Folder");
            FileHelper::CreateDirectoryRecursive(newFolderFullPath.c_str());
            AssetManager::GetInstancePtr()->updateAssets();

            _controller->setAssetFileRename(AssetManager::GetInstancePtr()->getAssetFileByFullPath(newFolderFullPath), true);
        }

        //////////////////////////////////////////
        void CreateMaterial(AssetsController* _controller, Path const& _fullPath)
        {
            String dir = FileHelper::GetDirectoryInPath(_fullPath);
            MaterialPtr srcMaterial = MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::Specular);
            MaterialPtr material = srcMaterial->createCopy();
            String newMaterialFullPath = EditorToolsHelper::BuildNewAssetFileName(dir + "/New Material.mzmaterial");
            material->saveToFile(newMaterialFullPath);
            AssetManager::GetInstancePtr()->updateAssets();

            AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(newMaterialFullPath);
            if (assetFile && MaterialManager::GetCurrentInstance()->getOrLoadMaterial(assetFile))
            {
                SelectionManager::GetInstancePtr()->selectObject(assetFile);
                _controller->setAssetFileRename(assetFile, true);
            }
        }

        //////////////////////////////////////////
        void CreatePrefab3D(AssetsController* _controller, Path const& _fullPath)
        {
            Path dir = FileHelper::GetDirectoryInPath(_fullPath);

            Path newPrefabFullPath = EditorToolsHelper::BuildNewAssetFileName(dir + "/New Prefab.mzprefab");

            Path name = FileHelper::GetFileNameWithoutExtension(newPrefabFullPath);
            EntityPtr entity = EditorManager::GetInstancePtr()->getSceneMain()->createEntity(name);
            EntitySerializationManager::GetInstancePtr()->savePrefabToDataBlockFile(entity, newPrefabFullPath);
            AssetManager::GetInstancePtr()->updateAssets();

            entity->removeFromEcsWorld();

            AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(newPrefabFullPath);
            if (assetFile)
            {
                SelectionManager::GetInstancePtr()->selectObject(assetFile);
                _controller->setAssetFileRename(assetFile, true);
            }
        }

        //////////////////////////////////////////
        void CreateScene(AssetsController* _controller, Path const& _fullPath)
        {
            Path dir = FileHelper::GetDirectoryInPath(_fullPath);

            Path newPrefabFullPath = EditorToolsHelper::BuildNewAssetFileName(dir + "/New Scene.mzscene");

            Path name = FileHelper::GetFileNameWithoutExtension(newPrefabFullPath);

            EcsAssetScenePtr scene = EcsAssetScene::Create(EditorManager::GetInstancePtr()->getSceneMain()->getRenderTarget());
            scene->setName(HashedString(name.toUTF8()));
            // EntityPtr entity = EditorManager::GetInstancePtr()->getSceneMain()->createEntity(name);
            // EntitySerializationManager::GetInstancePtr()->savePrefabToDataBlockFile(entity, newPrefabFullPath);

            EntitySerializationManager::GetInstancePtr()->saveSceneToDataBlockFile(scene, newPrefabFullPath);

            AssetManager::GetInstancePtr()->updateAssets();

            // entity->removeFromEcsWorld();

            AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(newPrefabFullPath);
            if (assetFile)
            {
                SelectionManager::GetInstancePtr()->selectObject(assetFile);
                _controller->setAssetFileRename(assetFile, true);
            }
        }

        //////////////////////////////////////////
        void Rename(AssetsController* _controller, Path const& _fullPath)
        {
            AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_fullPath);
            if (assetFile)
                _controller->setAssetFileRename(assetFile, true);
        }

        //////////////////////////////////////////
        void Delete(Path const& _fullPath)
        {
            AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_fullPath);
            AssetManager::GetInstancePtr()->deleteAssetFile(assetFile);
        }

        //////////////////////////////////////////
        Path Duplicate(Path const& _fullPath)
        {
            Path dir = FileHelper::GetDirectoryInPath(_fullPath);

            Path newPrefabFullPath = EditorToolsHelper::BuildNewAssetFileName(_fullPath);
            FileHelper::CopyRegularFile(_fullPath.c_str(), newPrefabFullPath.c_str());
            AssetManager::GetInstancePtr()->updateAssets();

            return newPrefabFullPath;
        }

        //////////////////////////////////////////
        void Duplicate(AssetsController* _controller, Path const& _fullPath)
        {
            Path newPrefabFullPath = Duplicate(_fullPath);
            Rename(_controller, newPrefabFullPath);
        }

        //////////////////////////////////////////
        void Edit(Path const& _fullPath)
        {
            AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_fullPath);
            MAZE_ERROR_RETURN_IF(!assetFile, "Asset file %s is null!", _fullPath.toUTF8().c_str());

            Path extension = assetFile->getExtension();
            if (AssetEditorToolsManager::GetInstancePtr()->isPrefabExtension(extension))
                EditorManager::GetInstancePtr()->openPrefab(assetFile);
            else
            if (extension == "mzscene")
                EditorManager::GetInstancePtr()->openScene(assetFile);
        }
    };

} // namespace Maze
//////////////////////////////////////////
