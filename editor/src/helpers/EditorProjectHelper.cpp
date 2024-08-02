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
#include "helpers/EditorProjectHelper.hpp"
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
#include "maze-core/helpers/MazeFileHelper.hpp"
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
        Path SelectProjectFolder()
        {
            Path path;

            do
            {
                path = SystemDialogHelper::OpenFolder(
                    "Select Project Folder",
                    Editor::GetInstancePtr()->getMainRenderWindow()->getWindowRaw());

                if (path.empty())
                    return path;
            }
            while (!FileHelper::IsDirectory(path));

            return path;
        }

        //////////////////////////////////////////
        bool IsProjectPathValid(String const& _path)
        {
            if (_path.empty() || !FileHelper::IsDirectory(_path))
                return false;

            return true;
        }

        //////////////////////////////////////////
        bool IsProjectPathValid()
        {
            EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();
            return IsProjectPathValid(editorSettings->getProjectFullPath());
        }

        //////////////////////////////////////////
        bool SelectProject()
        {
            SceneManager::GetInstancePtr()->unloadScene<SceneEditor>();

            EditorSettings* editorSettings = SettingsManager::GetInstancePtr()->getSettingsRaw<EditorSettings>();

            AssetManager::GetInstancePtr()->removeAssetsDirectoryPath(editorSettings->getProjectFullPath());

            editorSettings->setProjectFullPath(String());
            SettingsManager::GetInstancePtr()->saveSettings();

            bool isProjectFullPathValid = false;

            String projectFullPath;
            do
            {
                projectFullPath = EditorHelper::SelectProjectFolder().toUTF8();
                isProjectFullPathValid = IsProjectPathValid(projectFullPath);

                if (projectFullPath.empty())
                    break;
            }
            while (!isProjectFullPathValid);

            if (!isProjectFullPathValid)
            {
                Editor::GetInstancePtr()->shutdown();
                return false;
            }

            editorSettings->setProjectFullPath(projectFullPath);
            SettingsManager::GetInstancePtr()->saveSettings();

            AssetManager::GetInstancePtr()->addAssetsDirectoryPath(editorSettings->getProjectFullPath());

            SceneManager::GetInstancePtr()->loadScene<SceneEditor>();

            return true;
        }
    };

} // namespace Maze
//////////////////////////////////////////
