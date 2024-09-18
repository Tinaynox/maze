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
#include "maze-graphics/assets/MazeAssetUnitTextureCube.hpp"
#include "maze-graphics/MazeTextureCube.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitTextureCube
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitTextureCube, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitTextureCube::AssetUnitTextureCube()
    {

    }

    //////////////////////////////////////////
    AssetUnitTextureCube::~AssetUnitTextureCube()
    {
    }

    //////////////////////////////////////////
    AssetUnitTextureCubePtr AssetUnitTextureCube::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitTextureCubePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            AssetUnitTextureCube,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitTextureCube::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    TextureCubePtr const& AssetUnitTextureCube::loadTexture(bool _syncLoad)
    {
        if (!isLoaded())
        {
            initTexture();
            _syncLoad ? loadNow() : load();
        }

        return m_texture;
    }

    //////////////////////////////////////////
    bool AssetUnitTextureCube::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initTexture();
        if (!m_texture)
            return false;

        m_texture->loadFromAssetFile(assetFile);

        TextureManagerPtr const& textureManager = TextureManager::GetCurrentInstancePtr();
        if (textureManager)
            textureManager->loadTextureMetaData(m_texture, m_data);

        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitTextureCube::unloadNowImpl()
    {
        if (m_texture)
        {
            PixelSheet2D pixelSheet[6] =
            {
                PixelSheet2D(Vec2S(1), ColorU32::c_green),
                PixelSheet2D(Vec2S(1), ColorU32::c_green),
                PixelSheet2D(Vec2S(1), ColorU32::c_green),
                PixelSheet2D(Vec2S(1), ColorU32::c_green),
                PixelSheet2D(Vec2S(1), ColorU32::c_green),
                PixelSheet2D(Vec2S(1), ColorU32::c_green)
            };
            m_texture->loadTexture(pixelSheet);
        }

        return true;
    }

    //////////////////////////////////////////
    TextureCubePtr const& AssetUnitTextureCube::initTexture()
    {
        if (m_texture)
            return m_texture;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_texture;

        m_texture = TextureCube::Create();
        PixelSheet2D pixelSheet[6] =
            {
                PixelSheet2D(Vec2S(1)),
                PixelSheet2D(Vec2S(1)),
                PixelSheet2D(Vec2S(1)),
                PixelSheet2D(Vec2S(1)),
                PixelSheet2D(Vec2S(1)),
                PixelSheet2D(Vec2S(1))
            };
        m_texture->loadTexture(pixelSheet);
        m_texture->setName(m_data.getString(MAZE_HCS("name"), assetFile->getFileName()));

        if (TextureManager::GetCurrentInstancePtr())
        {
            TextureLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitTextureCubeWPtr)cast<AssetUnitTextureCube>()](bool _syncLoad)
                {
                    if (AssetUnitTextureCubePtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitTextureCubeWPtr)cast<AssetUnitTextureCube>()] (bool _syncLoad)
                {
                    if (AssetUnitTextureCubePtr assetUnit = weakPtr.lock())
                        _syncLoad ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitTextureCubeWPtr)cast<AssetUnitTextureCube>()](bool _syncLoad)
                {
                    if (AssetUnitTextureCubePtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _syncLoad ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitTextureCubeWPtr)cast<AssetUnitTextureCube>()](Set<String> const& _tags)
                {
                    if (AssetUnitTextureCubePtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            TextureManager::GetCurrentInstancePtr()->addTextureToLibrary(
                m_texture,
                callbacks);
        }

        return m_texture;
    }


} // namespace Maze
//////////////////////////////////////////
