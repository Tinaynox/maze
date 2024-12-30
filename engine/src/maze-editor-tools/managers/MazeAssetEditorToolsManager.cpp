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
#include "MazeEditorToolsHeader.hpp"
#include "maze-editor-tools/managers/MazeAssetEditorToolsManager.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
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
#include "maze-editor-tools/scenes/SceneDebugEditor.hpp"
#include "maze-editor-tools/settings/MazeEditorToolsSettings.hpp"
#include "maze-editor-tools/helpers/MazeEditorToolsHelper.hpp"
#include "maze-editor-tools/helpers/MazeAssetEditorToolsHelper.hpp"
#include "maze-editor-tools/ecs/components/MazeAssetsController.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/managers/MazeMaterialManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-core/managers/MazeEntityManager.hpp"
#include "maze-core/ecs/MazeEcsWorld.hpp"
#include "maze-editor-tools/managers/MazeSelectionManager.hpp"
#include "maze-editor-tools/managers/MazeInspectorManager.hpp"
#include "maze-ui/managers/MazeUIManager.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static AssetFileEditorIconData TextureFileIconCallback(AssetFilePtr const& _assetFile)
    {
        AssetFileEditorIconData result;

        RenderSystemPtr const& renderSystem = GraphicsManager::GetInstancePtr()->getDefaultRenderSystem();
        TextureManagerPtr const& textureManager = renderSystem->getTextureManager();
        Texture2DPtr const& texture = textureManager->getOrLoadTexture2D(_assetFile);
        if (texture)
        {
            result.sprite = Sprite::Create(texture);
            result.sprite->setName(HashedString("TextureFileIcon_" + _assetFile->getFileName().toUTF8()));
        }

        return result;
    };


    //////////////////////////////////////////
    // Class AssetEditorToolsManager
    //
    //////////////////////////////////////////
    AssetEditorToolsManager* AssetEditorToolsManager::s_instance = nullptr;

    //////////////////////////////////////////
    AssetEditorToolsManager::AssetEditorToolsManager()
    {
        s_instance = this;
    }

    //////////////////////////////////////////
    AssetEditorToolsManager::~AssetEditorToolsManager()
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
    void AssetEditorToolsManager::Initialize(AssetEditorToolsManagerPtr& _manager)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(AssetEditorToolsManager, _manager, init());
    }

    //////////////////////////////////////////
    bool AssetEditorToolsManager::init()
    {
        GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemWillBeChanged.subscribe(this, &AssetEditorToolsManager::notifyDefaultRenderSystemWillBeChanged);
        GraphicsManager::GetInstancePtr()->eventDefaultRenderSystemChanged.subscribe(this, &AssetEditorToolsManager::notifyDefaultRenderSystemChanged);
        notifyDefaultRenderSystemChanged(GraphicsManager::GetInstancePtr()->getDefaultRenderSystem());

        registerAssetFileCallbacks();

        addPrefabExtension("mzprefab");

        return true;
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::notifyDefaultRenderSystemWillBeChanged(RenderSystemPtr const& _renderSystem)
    {
        if (RenderSystem::GetCurrentInstancePtr() && RenderSystem::GetCurrentInstancePtr()->getTextureManager())
            RenderSystem::GetCurrentInstancePtr()->getTextureManager()->eventTextureLoaderAdded.unsubscribe(this);
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::notifyDefaultRenderSystemChanged(RenderSystemPtr const& _renderSystem)
    {
        if (_renderSystem)
        {
            registerIconCallbacks();

            _renderSystem->getTextureManager()->eventTextureLoaderAdded.subscribe(this, &AssetEditorToolsManager::notifyTextureLoaderAdded);
        }
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::notifyTextureLoaderAdded(HashedCString _extension, TextureLoaderData const& _data)
    {
        registerIconCallbackForAssetFileExtension(_extension.str, TextureFileIconCallback);
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::registerIconCallbacks()
    {
        Vector<HashedString> textureExtensions = TextureManager::GetCurrentInstancePtr()->getTextureLoaderExtensions();
        for (HashedString const& textureExtension : textureExtensions)
            registerIconCallbackForAssetFileExtension(textureExtension, TextureFileIconCallback);
        registerIconCallbackForAssetFileExtension("mztexture", TextureFileIconCallback);


        std::function<AssetFileEditorIconData(AssetFilePtr const&)> textFileIconCallback =
            [](AssetFilePtr const& _assetFile)
        {
            return
                AssetFileEditorIconData
                {
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::TextFile),
                    ColorU32::c_white
                };
        };
        registerIconCallbackForAssetFileExtension("txt", textFileIconCallback);
        registerIconCallbackForAssetFileExtension("mzmeta", textFileIconCallback);

        registerIconCallbackForAssetFileExtension("mzscene",
            [](AssetFilePtr const& _assetFile)
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Scene),
                        ColorU32(17, 18, 16)
                    };
            });

        std::function<AssetFileEditorIconData(AssetFilePtr const&)> meshFileIconCallback =
            [](AssetFilePtr const& _assetFile)
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Mesh),
                        ColorU32::c_white
                    };
            };
        // #TODO: read from MeshManager
        registerIconCallbackForAssetFileExtension("obj", meshFileIconCallback);
        registerIconCallbackForAssetFileExtension("fbx", meshFileIconCallback);
        registerIconCallbackForAssetFileExtension("mzmesh", meshFileIconCallback);


        registerIconCallbackForAssetFileExtension("mzphysicsMaterial2D",
            [](AssetFilePtr const& _assetFile)
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::PhysicsMaterial2D),
                        ColorU32::c_white
                    };
            });

        registerIconCallbackForAssetFileExtension("mzshader",
            [](AssetFilePtr const& _assetFile)
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Shader),
                        ColorU32::c_white
                    };
            });
        registerIconCallbackForAssetFileExtension("mzglsl",
            [](AssetFilePtr const& _assetFile)
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Shader),
                        ColorU32::c_white
                    };
            });

        registerIconCallbackForAssetFileExtension("mzmaterial",
            [](AssetFilePtr const& _assetFile)
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Material),
                        ColorU32::c_white
                    };
            });

        registerIconCallbackForAssetFileClass<AssetDirectory>(
            []()
            {
                return
                    AssetFileEditorIconData
                    {
                        UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::FolderClosed),
                        ColorU32::c_white
                    };
            });
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::registerAssetFileCallbacks()
    {
        registerAssetFileContextMenuCallback(
            [](AssetsController* _controller, String const& _fullPath, MenuListTree2DPtr const& _menuListTree)
            {
                _menuListTree->addItem(
                    "Show in Explorer",
                    [_fullPath](String const& _text) { AssetEditorToolsHelper::ShowInExplorer(_fullPath); });
            });
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::callAssetFileContextMenuCallback(AssetsController* _controller, Path const& _fullPath, MenuListTree2DPtr const& _menuListTree)
    {
        for (auto callback : m_assetFileContextMenuCallbacks)
            callback(_controller, _fullPath, _menuListTree);

        _menuListTree->rebuildItems();
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::update(F32 _dt)
    {
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::registerIconCallbackForAssetFileExtension(
        String const& _assetFileExtension,
        std::function<AssetFileEditorIconData(AssetFilePtr const&)> _callback)
    {
        m_iconCallbackPerAssetFileExtension.insert(
            _assetFileExtension,
            _callback);
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::registerIconCallbackForAssetFileClass(
        ClassUID _assetFileClass,
        std::function<AssetFileEditorIconData()> _callback)
    {
        m_iconCallbackPerAssetFileClass.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_assetFileClass),
            std::forward_as_tuple(_callback));
    }

    //////////////////////////////////////////
    AssetFileEditorIconData AssetEditorToolsManager::getIconForAssetFile(AssetFilePtr const& _assetFile)
    {
        ClassUID assetFileUID = _assetFile->getClassUID();
        String extension = _assetFile->getExtension();

        auto it = m_iconCallbackPerAssetFileExtension.find(extension);
        if (it != m_iconCallbackPerAssetFileExtension.end())
            return it->second(_assetFile);

        auto it2 = m_iconCallbackPerAssetFileClass.find(assetFileUID);
        if (it2 != m_iconCallbackPerAssetFileClass.end())
            return it2->second();

        return
            AssetFileEditorIconData
            {
                UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::File),
                ColorU32::c_white
            };
    }

    //////////////////////////////////////////
    void AssetEditorToolsManager::addPrefabExtension(String const& _extension)
    {
        m_prefabExtensions.insert(_extension);

        registerIconCallbackForAssetFileExtension(_extension,
            [](AssetFilePtr const& _assetFile)
        {
            return
                AssetFileEditorIconData
                {
                    UIManager::GetInstancePtr()->getDefaultUISprite(DefaultUISprite::Prefab),
                    ColorU32::c_white
                };
        });
    }

} // namespace Maze
//////////////////////////////////////////
