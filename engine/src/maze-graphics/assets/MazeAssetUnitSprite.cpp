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
#include "maze-graphics/assets/MazeAssetUnitSprite.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-graphics/managers/MazeGraphicsManager.hpp"
#include "maze-graphics/assets/MazeAssetUnitTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class AssetUnitSprite
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(AssetUnitSprite, AssetUnit);
    
    //////////////////////////////////////////
    AssetUnitSprite::AssetUnitSprite()
    {

    }

    //////////////////////////////////////////
    AssetUnitSprite::~AssetUnitSprite()
    {
    }

    //////////////////////////////////////////
    AssetUnitSpritePtr AssetUnitSprite::Create(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        AssetUnitSpritePtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(
            AssetUnitSprite,
            object,
            init(_assetFile, _data));
        return object;
    }

    //////////////////////////////////////////
    bool AssetUnitSprite::init(
        AssetFilePtr const& _assetFile,
        DataBlock const& _data)
    {
        if (!AssetUnit::init(_assetFile, _data))
            return false;
            
        return true;
    }

    //////////////////////////////////////////
    SpritePtr const& AssetUnitSprite::loadSprite()
    {
        if (!isLoaded())
        {
            initSprite();
            load();
        }

        return m_sprite;
    }

    //////////////////////////////////////////
    bool AssetUnitSprite::loadNowImpl()
    {
        AssetFilePtr assetFile = m_assetFile.lock();
        if (!assetFile)
            return false;

        initSprite();
        if (!m_sprite)
            return false;

        AssetUnitTexture2DPtr assetUnitTexture2D = assetFile->getAssetUnit<AssetUnitTexture2D>();
        if (!assetUnitTexture2D)
            return false;

        m_sprite->set(
            assetUnitTexture2D->initTexture(),
            m_data.getVec2F(MAZE_HCS("colorPosition"), Vec2S::c_zero),
            m_data.getVec2F(MAZE_HCS("colorSize"), Vec2S::c_zero),
            m_data.getVec2F(MAZE_HCS("colorOffset"), Vec2S::c_zero),
            m_data.getVec2F(MAZE_HCS("nativeSize"), Vec2S::c_zero));

        if (DataBlock* sliceBorder = m_data.getDataBlock(MAZE_HCS("sliceBorder")))
        {
            F32 left = sliceBorder->getF32(MAZE_HCS("left"), 0.0f);
            F32 bottom = sliceBorder->getF32(MAZE_HCS("bottom"), 0.0f);
            F32 right = sliceBorder->getF32(MAZE_HCS("right"), 0.0f);
            F32 top = sliceBorder->getF32(MAZE_HCS("top"), 0.0f);
            m_sprite->setSliceBorder(left, bottom, right, top);
        }

        return true;
    }

    //////////////////////////////////////////
    bool AssetUnitSprite::unloadNowImpl()
    {
        if (m_sprite)
        {
            if (SpriteManager::GetCurrentInstance())
                SpriteManager::GetCurrentInstance()->removeSpriteFromLibrary(m_sprite->getName());
            m_sprite.reset();
        }

        return true;
    }

    //////////////////////////////////////////
    SpritePtr const& AssetUnitSprite::initSprite()
    {
        if (m_sprite)
            return m_sprite;

        AssetFilePtr assetFile = getAssetFile();
        if (!assetFile)
            return m_sprite;

        m_sprite = Sprite::Create();
        
        m_sprite->setName(m_data.getString(MAZE_HCS("name"), assetFile->getFileName()));

        if (SpriteManager::GetCurrentInstance())
        {
            SpriteLibraryDataCallbacks callbacks;

            callbacks.requestLoad = 
                [weakPtr = (AssetUnitSpriteWPtr)cast<AssetUnitSprite>()](bool _immediate)
                {
                    if (AssetUnitSpritePtr assetUnit = weakPtr.lock())
                        _immediate ? assetUnit->loadNow() : assetUnit->load();
                };

            callbacks.requestUnload =
                [weakPtr = (AssetUnitSpriteWPtr)cast<AssetUnitSprite>()] (bool _immediate)
                {
                    if (AssetUnitSpritePtr assetUnit = weakPtr.lock())
                        _immediate ? assetUnit->unloadNow() : assetUnit->unload();
                };

            callbacks.requestReload =
                [weakPtr = (AssetUnitSpriteWPtr)cast<AssetUnitSprite>()](bool _immediate)
                {
                    if (AssetUnitSpritePtr assetUnit = weakPtr.lock())
                    {
                        assetUnit->unloadNow();
                        _immediate ? assetUnit->loadNow() : assetUnit->load();
                    }
                };
            callbacks.hasAnyOfTags = 
                [weakPtr = (AssetUnitSpriteWPtr)cast<AssetUnitSprite>()](Set<String> const& _tags)
                {
                    if (AssetUnitSpritePtr assetUnit = weakPtr.lock())
                        if (AssetFilePtr assetFile = assetUnit->getAssetFile())
                            return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            SpriteManager::GetCurrentInstance()->addSpriteToLibrary(
                m_sprite,
                callbacks);
        }

        return m_sprite;
    }


} // namespace Maze
//////////////////////////////////////////
