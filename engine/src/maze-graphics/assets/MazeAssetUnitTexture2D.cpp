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
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"


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
        MAZE_CREATE_AND_INIT_SHARED_PTR(
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
    Texture2DPtr const& AssetUnitTexture2D::loadTexture()
    {
        if (!isLoaded())
        {
            initTexture();
            load();
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

        m_texture->loadFromAssetFile(assetFile);

        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitTexture2D::unloadNowImpl()
    {
        if (m_texture)
        {
            if (TextureManager::GetCurrentInstancePtr())
                TextureManager::GetCurrentInstancePtr()->removeTexture2DFromLibrary(m_texture->getName().asHashedCString());
            m_texture.reset();
        }

        return true;
    }

    //////////////////////////////////////////
    void AssetUnitTexture2D::initTexture()
    {
        if (m_texture)
            return;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return;

        m_texture = Texture2D::Create();
        m_texture->loadTexture(PixelSheet2D(Vec2S(1)));
        m_texture->setName(m_data.getString(MAZE_HCS("name"), assetFile->getFileName()));

        if (TextureManager::GetCurrentInstancePtr())
            TextureManager::GetCurrentInstancePtr()->addTextureToLibrary(
                m_texture,
                [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>()](bool _immediate)
                {
                    // Load
                    if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                        _immediate ? assetUnit->loadNow() : assetUnit->load();
                },
                [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>()](bool _immediate)
                {
                    // Unload
                    if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                        _immediate ? assetUnit->unloadNow() : assetUnit->unload();
                },
                [weakPtr = (AssetUnitTexture2DWPtr)cast<AssetUnitTexture2D>()](bool _immediate)
                {
                    // Reload
                    if (AssetUnitTexture2DPtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _immediate ? assetUnit->loadNow() : assetUnit->load();
                    }
                });
    }


} // namespace Maze
//////////////////////////////////////////
