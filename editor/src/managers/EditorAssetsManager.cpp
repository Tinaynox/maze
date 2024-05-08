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
#include "EditorAssetsManager.hpp"
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
#include "maze-graphics/ecs/components/MazeSpriteRenderer2D.hpp"
#include "maze-graphics/ecs/helpers/MazeSpriteHelper.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeRenderMeshManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
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
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-editor-tools/inspectors/asset-materials/MazeMaterialsInspector.hpp"
#include "maze-editor-tools/inspectors/asset-materials/MazeTexture2DsInspector.hpp"
#include "Editor.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "helpers/EditorAssetHelper.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class EditorAssetsManager
    //
    //////////////////////////////////////////
    EditorAssetsManager* EditorAssetsManager::s_instance = nullptr;

    //////////////////////////////////////////
    EditorAssetsManager::EditorAssetsManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    EditorAssetsManager::~EditorAssetsManager()
    {
        if (Editor::GetInstancePtr())
        {
            if (Editor::GetInstancePtr()->getMainRenderWindow())
            {
                if (Editor::GetInstancePtr()->getMainRenderWindow()->getWindow())
                {
                    Editor::GetInstancePtr()->getMainRenderWindow()->getWindow()->eventWindowFocusChanged.unsubscribe(this);
                }
            }
        }

        s_instance = nullptr;
    }

    //////////////////////////////////////////
    void EditorAssetsManager::Initialize(EditorAssetsManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(EditorAssetsManager, _manager, init());
    }

    //////////////////////////////////////////
    bool EditorAssetsManager::init()
    {
        Editor::GetInstancePtr()->eventMainRenderWindowCreated.subscribe(
            [this]()
            {
                Editor::GetInstancePtr()->getMainRenderWindow()->getWindow()->eventWindowFocusChanged.subscribe(
                    this, &EditorAssetsManager::notifyWindowFocusChanged);
            });

        EventManager::GetInstancePtr()->subscribeEvent<EditorToolsMaterialChangedEvent>(
            [](EditorToolsMaterialChangedEvent* _event)
            {
                AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_event->getMaterialName());
                if (assetFile)
                {
                    MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getMaterial(_event->getMaterialName());
                    if (material)
                        material->saveToFile(assetFile->getFullPath());
                }
            });

        EventManager::GetInstancePtr()->subscribeEvent<EditorToolsTexture2DChangedEvent>(
            [](EditorToolsTexture2DChangedEvent* _event)
            {
                AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_event->getTextureName());
                if (assetFile)
                {
                    Texture2DPtr const& texture = TextureManager::GetCurrentInstancePtr()->getTexture2D(_event->getTextureName());
                    if (texture)
                    {
                        TextureManager::GetCurrentInstancePtr()->saveTextureMetaData(assetFile);
                    }
                }
            });

        registerAssetFileCallbacks();

        return true;
    }

    //////////////////////////////////////////
    void EditorAssetsManager::notifyWindowFocusChanged(Window* _window)
    {
        if (_window->getFocused())
        {
            AssetManager::GetInstancePtr()->updateAssets();
        }
    }

    //////////////////////////////////////////
    void EditorAssetsManager::registerAssetFileCallbacks()
    {
        AssetEditorToolsManager::GetInstancePtr()->registerAssetFileContextMenuCallback(
            [](AssetsController* _controller, Path const& _fullPath, MenuListTree2DPtr const& _menuListTree)
        {
            Path extension = FileHelper::GetFileExtension(_fullPath);

            _menuListTree->addItem(
                "Create/Folder",
                [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreateFolder(_controller, _fullPath); });

            _menuListTree->addItem(
                "Create/Material",
                [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreateMaterial(_controller, _fullPath); });

            _menuListTree->addItem(
                "Create/Prefab/3D",
                [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreatePrefab3D(_controller, _fullPath); });

            auto  const& assetDirectoryPathes = AssetManager::GetInstancePtr()->getAssetDirectoryPathes();
            bool isRootAssetDirectory = assetDirectoryPathes.find(_fullPath) != assetDirectoryPathes.end();

            if (!isRootAssetDirectory)
            {
                _menuListTree->addItem(
                    "Rename",
                    [_controller, _fullPath](String const& _text) { EditorAssetHelper::Rename(_controller, _fullPath); });

                _menuListTree->addItem(
                    "Delete",
                    [_fullPath](String const& _text) { EditorAssetHelper::Delete(_fullPath); });
            }

            if (AssetEditorToolsManager::GetInstancePtr()->isPrefabExtension(extension))
            {
                _menuListTree->addItem(
                    "Duplicate",
                    [_controller, _fullPath](String const& _text) { EditorAssetHelper::Duplicate(_controller, _fullPath); });

                _menuListTree->addItem(
                    "Edit",
                    [_fullPath](String const& _text) { EditorAssetHelper::Edit(_fullPath); });
            }
            else
            if (extension == "mzmaterial")
            {
                _menuListTree->addItem(
                    "Duplicate",
                    [_controller, _fullPath](String const& _text) { EditorAssetHelper::Duplicate(_controller, _fullPath); });
            }
        });
    }

} // namespace Maze
//////////////////////////////////////////
