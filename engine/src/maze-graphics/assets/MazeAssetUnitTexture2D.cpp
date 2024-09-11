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
        TextureManager::GetCurrentInstancePtr()->addTextureToLibrary(m_texture);

        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitTexture2D::unloadNowImpl()
    {
        if (m_texture)
        {
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

        m_texture = Texture2D::Create();
        m_texture->loadTexture(PixelSheet2D(Vec2S(1)));
    }


} // namespace Maze
//////////////////////////////////////////
