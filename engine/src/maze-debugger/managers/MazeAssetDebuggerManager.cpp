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
#include "MazeDebuggerHeader.hpp"
#include "maze-debugger/managers/MazeAssetDebuggerManager.hpp"
#include "maze-debugger/managers/MazeSelectionManager.hpp"
#include "maze-core/helpers/MazeTextHelper.hpp"
#include "maze-core/helpers/MazeFileHelper.hpp"
#include "maze-core/helpers/MazeSystemHelper.hpp"
#include "maze-core/managers/MazeInputManager.hpp"
#include "maze-core/managers/MazeSystemManager.hpp"
#include "maze-core/managers/MazeSceneManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/settings/MazeSettingsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/assets/MazeAssetDirectory.hpp"
#include "maze-debugger/scenes/SceneDebugEditor.hpp"
#include "maze-debugger/settings/MazeDebuggerSettings.hpp"
#include "maze-debugger/helpers/MazeDebuggerHelper.hpp"
#include "maze-graphics/ecs/systems/MazeGizmosSystem.hpp"
#include "maze-graphics/ecs/components/gizmos/MazeComponentGizmos.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeECSWorld.hpp"
#include "maze-debugger/managers/MazeSelectionManager.hpp"
#include "maze-debugger/managers/MazeInspectorManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static SpritePtr TextureFileIconCallback(AssetFilePtr const& _assetFile)
    {
        SpritePtr result;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        TextureManagerPtr const& textureManager = renderSystem->getTextureManager();
        Texture2DPtr const& texture = textureManager->getTexture2D(_assetFile);
        if (texture)
        {
            result = Sprite::Create(texture);
        }

        return result;
    };


    //////////////////////////////////////////
    // Class AssetDebuggerManager
    //
    //////////////////////////////////////////
    AssetDebuggerManager* AssetDebuggerManager::s_instance = nullptr;

    //////////////////////////////////////////
    AssetDebuggerManager::AssetDebuggerManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    AssetDebuggerManager::~AssetDebuggerManager()
    {
        s_instance = nullptr;

        if (GraphicsManager::GetInstancePtr())
        {
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemWillBeChanged.unsubscribe(this);
            GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.unsubscribe(this);
        }

        if (RenderSystem::GetCurrentInstancePtr() && RenderSystem::GetCurrentInstancePtr()->getTextureManager())
            RenderSystem::GetCurrentInstancePtr()->getTextureManager()->eventTextureLoaderAdded.unsubscribe(this);
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::Initialize(AssetDebuggerManagerPtr& _debuggerManager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetDebuggerManager, _debuggerManager, init());
    }

    //////////////////////////////////////////
    bool AssetDebuggerManager::init()
    {
        GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemWillBeChanged.subscribe(this, &AssetDebuggerManager::notifyDefaultRenderSystemWillBeChanged);
        GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.subscribe(this, &AssetDebuggerManager::notifyDefaultRenderSystemChanged);
        notifyDefaultRenderSystemChanged(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem());

        registerAssetFileCallbacks();

        return true;
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::notifyDefaultRenderSystemWillBeChanged(RenderSystemPtr const& _renderSystem)
    {
        if (RenderSystem::GetCurrentInstancePtr() && RenderSystem::GetCurrentInstancePtr()->getTextureManager())
            RenderSystem::GetCurrentInstancePtr()->getTextureManager()->eventTextureLoaderAdded.unsubscribe(this);
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::notifyDefaultRenderSystemChanged(RenderSystemPtr const& _renderSystem)
    {
        if (_renderSystem)
        {
            registerIconCallbacks();

            _renderSystem->getTextureManager()->eventTextureLoaderAdded.subscribe(this, &AssetDebuggerManager::notifyTextureLoaderAdded);
        }
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::notifyTextureLoaderAdded(HashedCString _extension, TextureLoaderData const& _data)
    {
        registerIconCallbackForAssetFileExtension(_extension.str, TextureFileIconCallback);
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::registerIconCallbacks()
    {
        Vector<String> textureExtensions = TextureManager::GetCurrentInstancePtr()->getTextureLoaderExtensions();
        for (String const& textureExtension : textureExtensions)
            registerIconCallbackForAssetFileExtension(textureExtension, TextureFileIconCallback);
        registerIconCallbackForAssetFileExtension("mztexture", TextureFileIconCallback);


        std::function<SpritePtr(AssetFilePtr const&)> textFileIconCallback =
            [](AssetFilePtr const& _assetFile)
        {
            return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TextFile);
        };
        registerIconCallbackForAssetFileExtension("txt", textFileIconCallback);
        registerIconCallbackForAssetFileExtension("meta", textFileIconCallback);

        std::function<SpritePtr(AssetFilePtr const&)> meshFileIconCallback =
            [](AssetFilePtr const& _assetFile)
        {
            return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Mesh);
        };
        registerIconCallbackForAssetFileExtension("obj", meshFileIconCallback);


        registerIconCallbackForAssetFileExtension("mzphysicsMaterial2D",
            [](AssetFilePtr const& _assetFile)
        {
            return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::PhysicsMaterial2D);
        });

        registerIconCallbackForAssetFileExtension("mzshader",
            [](AssetFilePtr const& _assetFile)
        {
            return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Shader);
        });
        registerIconCallbackForAssetFileExtension("mzglsl",
            [](AssetFilePtr const& _assetFile)
        {
            return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Shader);
        });

        registerIconCallbackForAssetFileExtension("mzmaterial",
            [](AssetFilePtr const& _assetFile)
        {
            return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Material);
        });

        registerIconCallbackForAssetFileClass<AssetDirectory>(
            []()
        {
            return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::FolderClosed);
        });
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::registerAssetFileCallbacks()
    {
        registerAssetFileContextMenuCallback(
            [](String const& _fullPath, MenuListTree2DPtr const& _menuListTree)
            {
                _menuListTree->addItem(
                    "Create/Folder",
                    [_fullPath](String const& _text)
                    {
                        String dir = FileHelper::GetDirectoryInPath(_fullPath);
                        String newFolderFullPath = DebuggerHelper::BuildNewAssetFileName(dir + "/New Folder");
                        FileHelper::CreateDirectoryRecursive(newFolderFullPath.c_str());
                        AssetManager::GetInstancePtr()->updateAssets();
                    });

                _menuListTree->addItem(
                    "Create/Material",
                    [_fullPath](String const& _text)
                    {
                        String dir = FileHelper::GetDirectoryInPath(_fullPath);
                        MaterialPtr srcMaterial = MaterialManager::GetCurrentInstance()->getBuiltinMaterial(BuiltinMaterialType::Specular);
                        MaterialPtr material = srcMaterial->createCopy();
                        String newMaterialFullPath = DebuggerHelper::BuildNewAssetFileName(dir + "/New Material.mzmaterial");
                        material->saveToFile(newMaterialFullPath);
                        AssetManager::GetInstancePtr()->updateAssets();

                        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(newMaterialFullPath);
                        if (assetFile && MaterialManager::GetCurrentInstance()->getMaterial(assetFile))
                        {
                            SelectionManager::GetInstancePtr()->selectObject(assetFile);
                        }
                    });

                _menuListTree->addItem(
                    "Show in Explorer",
                    [_fullPath](String const& _text)
                    {                        
                        SystemHelper::OpenExplorer(_fullPath);
                    });

                _menuListTree->addItem(
                    "Delete",
                    [_fullPath](String const& _text)
                    {
                        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFile(_fullPath);
                        AssetManager::GetInstancePtr()->deleteAssetFile(assetFile);
                    });
            });
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::callAssetFileContextMenuCallback(String const& _fullPath, MenuListTree2DPtr const& _menuListTree)
    {
        for (auto callback : m_assetFileContextMenuCallbacks)
            callback(_fullPath, _menuListTree);
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::registerIconCallbackForAssetFileExtension(
        String const& _assetFileExtension,
        std::function<SpritePtr(AssetFilePtr const&)> _callback)
    {
        m_iconCallbackPerAssetFileExtension.insert(
            _assetFileExtension,
            _callback);
    }

    //////////////////////////////////////////
    void AssetDebuggerManager::registerIconCallbackForAssetFileClass(
        ClassUID _assetFileClass,
        std::function<SpritePtr()> _callback)
    {
        m_iconCallbackPerAssetFileClass.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_assetFileClass),
            std::forward_as_tuple(_callback));
    }

    //////////////////////////////////////////
    SpritePtr AssetDebuggerManager::getIconForAssetFile(AssetFilePtr const& _assetFile)
    {
        ClassUID assetFileUID = _assetFile->getClassUID();
        String extension = _assetFile->getExtension();

        auto it = m_iconCallbackPerAssetFileExtension.find(extension);
        if (it != m_iconCallbackPerAssetFileExtension.end())
            return it->second(_assetFile);

        auto it2 = m_iconCallbackPerAssetFileClass.find(assetFileUID);
        if (it2 != m_iconCallbackPerAssetFileClass.end())
            return it2->second();

        return UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::File);
    }

} // namespace Maze
//////////////////////////////////////////
