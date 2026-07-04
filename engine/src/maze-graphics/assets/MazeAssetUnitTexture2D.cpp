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
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/assets/MazeAssetUnitTexture2D.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/managers/MazeTaskManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitTexture2D
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitTexture2D, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitTexture2D::AssetUnitTexture2D()
    {

    }

    //////////////////////////////////////////
    AssetUnitTexture2D::~AssetUnitTexture2D()
    {
    }

    //////////////////////////////////////////
    AssetUnitTexture2DPtr AssetUnitTexture2D::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitTexture2DPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(
            AssetUnitTexture2D,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitTexture2D::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    Texture2DPtr const& AssetUnitTexture2D::loadTexture(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initTexture();
            _syncLoad ? loadNow() : load();
        }

        return m_texture;
    }

    //////////////////////////////////////////
    bool AssetUnitTexture2D::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initTexture();
        if (!m_texture)
            return false;

        TextureManagerPtr const& textureManager = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager();

        Maze::Vector<Maze::PixelSheet2D> pixelSheets = textureManager->loadPixelSheets2D(assetFile);

        m_texture->loadTexture(pixelSheets);
        textureManager->loadTextureMetaData(m_texture, m_data);

        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitTexture2D::loadAsyncImpl()
    {
        // Async path is scheduled from the main thread only - initTexture() and the
        // texture library are not thread safe. Returning false falls back to sync loading
        TaskManager* taskManager = TaskManager::GetInstancePtr();
        if (!taskManager || !TaskManager::IsMainThread())
            return false;

        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        // Registers the blank 1x1 texture in the library so the asset is usable immediately
        initTexture();
        if (!m_texture)
            return false;

        // Meta data is fetched on the main thread - AssetManager is not thread safe
        DataBlock metaData;
        AssetManager::GetInstancePtr()->loadMetaData(assetFile, metaData);

        return taskManager->addBackgroundTask(
            [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>(), assetFile, metaData]()
            {
                MAZE_PROFILE_EVENT("AssetUnitTexture2D::loadAsyncImpl");

                // Background thread: file IO and decoding only.
                // No AssetUnit state, AssetManager or OpenGL access is allowed here
                GraphicsManager* graphicsManager = GraphicsManager::GetInstancePtr();
                TaskManager* taskManager = TaskManager::GetInstancePtr();
                if (!graphicsManager || !graphicsManager->getDefaultRenderSystemRaw() || !taskManager)
                    return;

                TextureManagerPtr const& textureManager = graphicsManager->getDefaultRenderSystemRaw()->getTextureManager();

                SharedPtr<Vector<PixelSheet2D>> pixelSheets = MakeShared<Vector<PixelSheet2D>>(
                    textureManager->loadPixelSheets2D(assetFile, metaData));

                taskManager->addMainThreadTask(
                    [weakPtr, pixelSheets]()
                    {
                        if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                            assetUnit->finishLoadingAsync(*pixelSheets);
                    });
            });
    }

    //////////////////////////////////////////
    void AssetUnitTexture2D::finishLoadingAsync(Vector<PixelSheet2D> const& _pixelSheets)
    {
        // A sync loadNow() or unload could have won the race while pixel sheets were decoding
        if (m_loadingState != AssetUnitLoadingState::Loading)
            return;

        if (!m_texture || _pixelSheets.empty())
        {
            m_loadingState = AssetUnitLoadingState::Error;
            return;
        }

        m_texture->loadTexture(_pixelSheets);

        TextureManagerPtr const& textureManager = GraphicsManager::GetInstancePtr()->getDefaultRenderSystemRaw()->getTextureManager();
        textureManager->loadTextureMetaData(m_texture, m_data);

        m_loadingState = AssetUnitLoadingState::Loaded;
    }

    //////////////////////////////////////////
    bool AssetUnitTexture2D::unloadNowImpl()
    {
        if (m_texture)
        {
            m_texture->loadTexture(PixelSheet2D(Vec2S(1), ColorU32::c_green));
        }

        return true;
    }

    //////////////////////////////////////////
    Texture2DPtr const& AssetUnitTexture2D::initTexture()
    {
        if (m_texture)
            return m_texture;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_texture;

        m_texture = Texture2D::Create();
        m_texture->loadTexture(PixelSheet2D(Vec2S(16)));
        m_texture->setName(m_data.getString(MAZE_HCS("name"), assetFile->getFileName()));

        if (TextureManager::GetCurrentInstancePtr())
        {
            TextureLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>()](bool _syncLoad)
                {
                    if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>()] (bool _syncLoad)
                {
                    if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>()](bool _syncLoad)
                {
                    if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>()](Set<String> const& _tags)
                {
                    if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            DataBlock info;
            info.setU32(MAZE_HCS("auid"), getAssetUnitId());

            TextureManager::GetCurrentInstancePtr()->addTextureToLibrary(
                m_texture,
                callbacks,
                info);
        }

        return m_texture;
    }

    //////////////////////////////////////////
    void AssetUnitTexture2D::updateDataFromAssetUnit(DataBlock& _metaData) const
    {
        AssetUnit::updateDataFromAssetUnit(_metaData);

        if (m_texture)
            TextureManager::GetCurrentInstancePtr()->saveTextureMetaData(m_texture, _metaData);
    }


} // namespace Maze
//////////////////////////////////////////
