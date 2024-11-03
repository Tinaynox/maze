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
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/ecs/components/MazeTransform2D.hpp"
#include "maze-core/ecs/components/MazeTransform3D.hpp"
#include "maze-core/ecs/components/MazeRotor3D.hpp"
#include "maze-core/ecs/components/MazeSinMovement3D.hpp"
#include "maze-core/ecs/components/MazeName.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
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
#include "maze-graphics/assets/MazeAssetUnitTexture2D.hpp"
#include "maze-graphics/assets/MazeAssetUnitTextureCube.hpp"
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
#include "events/EditorEvents.hpp"
#include "settings/MazeEditorSettings.hpp"
#include "helpers/EditorAssetHelper.hpp"
#include "helpers/EditorProjectHelper.hpp"


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
        if (EventManager::GetInstancePtr())
        {
            EventManager::GetInstancePtr()->unsubscribeEvent<EditorProjectOpenedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<EditorProjectWillBeClosedEvent>(this);
            EventManager::GetInstancePtr()->unsubscribeEvent<AssetFileAssetFileAssetUnitIdGeneratedEvent>(this);
        }

        if (Editor::GetInstancePtr())
        {
            if (Editor::GetInstancePtr()->getMainRenderWindow())
            {
                if (Editor::GetInstancePtr()->getMainRenderWindow()->getWindow())
                {
                    Editor::GetInstancePtr()->getMainRenderWindow()->getWindow()->eventWindowFocusChanged.unsubscribe(this);
                }
            }

            Editor::GetInstancePtr()->eventCoreEditorResourcesLoaded.unsubscribe(this);
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
        UpdateManager::GetInstancePtr()->addUpdatable(this);

        EventManager::GetInstancePtr()->subscribeEvent<EditorProjectOpenedEvent>(this, &EditorAssetsManager::notifyEvent);
        EventManager::GetInstancePtr()->subscribeEvent<EditorProjectWillBeClosedEvent>(this, &EditorAssetsManager::notifyEvent);

        Editor::GetInstancePtr()->eventMainRenderWindowCreated.subscribe(
            [this]()
            {
                Editor::GetInstancePtr()->getMainRenderWindow()->getWindow()->eventWindowFocusChanged.subscribe(
                    this, &EditorAssetsManager::notifyWindowFocusChanged);
            });

        Editor::GetInstancePtr()->eventCoreEditorResourcesLoaded.subscribe(this, &EditorAssetsManager::notifyCoreEditorResourcesLoaded);

        EventManager::GetInstancePtr()->subscribeEvent<EditorToolsMaterialChangedEvent>(
            [](EditorToolsMaterialChangedEvent* _event)
            {
                AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_event->getMaterialName());
                if (assetFile)
                {
                    MaterialPtr const& material = MaterialManager::GetCurrentInstance()->getOrLoadMaterial(_event->getMaterialName());
                    if (material)
                        material->saveToFile(assetFile->getFullPath());
                }
            });

        EventManager::GetInstancePtr()->subscribeEvent<EditorToolsTexture2DChangedEvent>(
            [](EditorToolsTexture2DChangedEvent* _event)
            {
                EditorAssetsManager* manager = EditorAssetsManager::GetInstancePtr();
                if (manager)
                {
                    AssetUnitPtr const& assetUnit = AssetUnitManager::GetInstancePtr()->getAssetUnit(_event->getTextureName());
                    if (assetUnit && assetUnit->getClassUID() == ClassInfo<AssetUnitTexture2D>::UID())
                        manager->fixAssetFile(assetUnit->castRaw<AssetUnitTexture2D>()->getAssetFile());
                }
            });

        EventManager::GetInstancePtr()->subscribeEvent<AssetFileAssetFileAssetUnitIdGeneratedEvent>(
            this, &EditorAssetsManager::notifyEvent);

        registerAssetFileCallbacks();

        return true;
    }

    //////////////////////////////////////////
    void EditorAssetsManager::update(F32 _dt)
    {
        fixAssetFilesNow();
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
    void EditorAssetsManager::addAssetContextOption(
        HashedCString _extension,
        String const& _menuName,
        std::function<void(AssetsController*, Path const&)> const& _callback,
        MenuListTree2D::ItemValidateCallback _validate)
    {
        m_editorAssetContextOptions[_extension].emplace_back(
            _menuName, _callback, _validate);
    }

    //////////////////////////////////////////
    void EditorAssetsManager::registerAssetFileCallbacks()
    {
        addAssetContextOption(
            MAZE_HCS("mzmaterial"),
            "Duplicate",
            [](AssetsController* _controller, Path const& _fullPath) { EditorAssetHelper::Duplicate(_controller, _fullPath); });

        AssetEditorToolsManager::GetInstancePtr()->registerAssetFileContextMenuCallback(
            [](AssetsController* _controller, Path const& _fullPath, MenuListTree2DPtr const& _menuListTree)
        {
            Path extension = FileHelper::GetFileExtension(_fullPath);

            _menuListTree->addItem(
                "Create/Folder",
                [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreateFolder(_controller, _fullPath); });

            _menuListTree->addItem(
                "Create/Scene",
                [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreateScene(_controller, _fullPath); });

            _menuListTree->addItem(
                "Create/Prefab/3D",
                [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreatePrefab3D(_controller, _fullPath); });

            _menuListTree->addItem(
                "Create/Material",
                [_controller, _fullPath](String const& _text) { EditorAssetHelper::CreateMaterial(_controller, _fullPath); });

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

            if (EditorAssetsManager::GetInstancePtr())
            {
                auto it = EditorAssetsManager::GetInstancePtr()->m_editorAssetContextOptions.find(extension);
                if (it != EditorAssetsManager::GetInstancePtr()->m_editorAssetContextOptions.end())
                {
                    for (EditorAssetContextOption const& option : it->second)
                    {
                        std::function<void(AssetsController*, Path const&)> callback = option.callback;
                        _menuListTree->addItem(
                            option.menuName,
                            [_controller, _fullPath, callback](String const& _text)
                            {
                                callback(_controller, _fullPath);
                            },
                            option.validate);
                    }
                }
            }
        });
    }

    //////////////////////////////////////////
    void EditorAssetsManager::notifyCoreEditorResourcesLoaded()
    {

    }

    //////////////////////////////////////////
    void EditorAssetsManager::notifyEvent(ClassUID _eventUID, Event* _event)
    {
        if (_eventUID == ClassInfo<EditorProjectOpenedEvent>::UID())
        {
            Path assetsPath = EditorHelper::GetProjectAssetsFolder();
            Path packagesPath = EditorHelper::GetProjectPackagesFolder();

            FileHelper::CreateDirectoryRecursive(assetsPath);
            FileHelper::CreateDirectoryRecursive(packagesPath);

            AssetManager::GetInstancePtr()->addAssetsDirectoryPath(assetsPath);
            AssetManager::GetInstancePtr()->addAssetsDirectoryPath(packagesPath);

            if (AssetManager::GetInstancePtr()->getGenerateIdsForNewAssetFiles())
            {
                for (AssetFilePtr const& assetFile : AssetManager::GetInstancePtr()->getAssetFilesInFolder(assetsPath))
                    if (assetFile->getAssetFileId() == c_invalidAssetFileId)
                        fixAssetFile(assetFile);
                for (AssetFilePtr const& assetFile : AssetManager::GetInstancePtr()->getAssetFilesInFolder(packagesPath))
                    if (assetFile->getAssetFileId() == c_invalidAssetFileId)
                        fixAssetFile(assetFile);
            }

            fixAssetFilesNow();
        }
        else
        if (_eventUID == ClassInfo<EditorProjectWillBeClosedEvent>::UID())
        {
            AssetManager::GetInstancePtr()->removeAssetsDirectoryPath(EditorHelper::GetProjectAssetsFolder());
            AssetManager::GetInstancePtr()->removeAssetsDirectoryPath(EditorHelper::GetProjectPackagesFolder());
        }
        else
        if (_eventUID == ClassInfo<AssetFileAssetFileAssetUnitIdGeneratedEvent>::UID())
        {
            AssetFileAssetFileAssetUnitIdGeneratedEvent* evt = _event->castRaw<AssetFileAssetFileAssetUnitIdGeneratedEvent>();

            if (evt->getAssetFile())
                fixAssetFile(evt->getAssetFile());
        }
    }

    //////////////////////////////////////////
    void EditorAssetsManager::fixAssetFile(AssetFilePtr const& _assetFile)
    {
        if (_assetFile->getExtension() == "mzmeta")
            return;

        if (_assetFile->getMetaClass()->isInheritedFrom<AssetDirectory>())
            return;

        m_assetFilessToFix.insert(_assetFile);
    }

    //////////////////////////////////////////
    void EditorAssetsManager::fixAssetFileNow(AssetFilePtr const& _assetFile)
    {
        if (_assetFile->getAssetFileId() == c_invalidAssetFileId && AssetManager::GetInstancePtr()->getGenerateIdsForNewAssetFiles())
            _assetFile->setAssetFileId(AssetManager::GetInstancePtr()->generateAssetFileId());
        
        AssetManager::GetInstancePtr()->updateAndSaveMetaData(_assetFile);
    }

    //////////////////////////////////////////
    void EditorAssetsManager::fixAssetFilesNow()
    {
        for (AssetFilePtr const& assetFile : m_assetFilessToFix)
            fixAssetFileNow(assetFile);
        m_assetFilessToFix.clear();
    }

} // namespace Maze
//////////////////////////////////////////
