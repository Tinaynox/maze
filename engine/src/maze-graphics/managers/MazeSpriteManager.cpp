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
#include "maze-graphics/managers/MazeSpriteManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/managers/MazeAssetUnitManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeSprite.hpp"
#include "maze-graphics/assets/MazeAssetUnitSprite.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Enum BuiltinSpriteType
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinSpriteType);


    //////////////////////////////////////////
    // Class SpriteManager
    //
    //////////////////////////////////////////
    SpriteManager::SpriteManager()
        : m_renderSystemRaw(nullptr)
    {
        
    }

    //////////////////////////////////////////
    SpriteManager::~SpriteManager()
    {
        m_defaultSpriteMaterial.reset();

        m_spritesLibrary.clear();

        for (BuiltinSpriteType t = BuiltinSpriteType(1); t < BuiltinSpriteType::MAX; ++t)
            m_builtinSprites[t].reset();
    }

    //////////////////////////////////////////
    void SpriteManager::Initialize(SpriteManagerPtr& _textureManager, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(SpriteManager, _textureManager, init(_renderSystem));
    }

    //////////////////////////////////////////
    bool SpriteManager::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();


        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                MAZE_HCS("sprite"),
                [](AssetFilePtr const& _file, DataBlock const& _data)
            {
                return AssetUnitSprite::Create(_file, _data);
            });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitSprite>::UID())
                        _assetUnit->castRaw<AssetUnitSprite>()->initSprite();
                });
        }

        return true;
    }

    //////////////////////////////////////////
    SpriteManagerPtr const& SpriteManager::GetCurrentInstance()
    {
        return RenderSystem::GetCurrentInstancePtr()->getSpriteManager();
    }

    //////////////////////////////////////////
    SpriteLibraryData const* SpriteManager::getSpriteLibraryData(HashedCString _spriteName)
    {
        StringKeyMap<SpriteLibraryData>::const_iterator it = m_spritesLibrary.find(_spriteName);
        if (it != m_spritesLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    SpritePtr const& SpriteManager::getOrLoadSprite(
        HashedCString _imageName,
        bool _syncLoad)
    {
        static SpritePtr const nullPointer;

        SpriteLibraryData const* libraryData = getSpriteLibraryData(_imageName);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->sprite;
        }
        
        TextureManagerPtr const& textureManager = m_renderSystemRaw->getTextureManager();

        Texture2DPtr texture2D = textureManager->getOrLoadTexture2D(_imageName);
        if (!texture2D)
            return nullPointer;

        SpritePtr sprite = Sprite::Create(texture2D);

        AssetFilePtr assetFile = AssetManager::GetInstancePtr()->getAssetFile(texture2D->getName().asHashedCString());
        if (assetFile)
        {
            DataBlock metaData;
            AssetManager::GetInstancePtr()->loadMetaData(assetFile, metaData);
            loadSpriteMetaData(sprite, metaData);
        }

        SpriteLibraryData* data = addSpriteToLibrary(sprite);
        if (data)
            return data->sprite;

        return nullPointer;
    }

    //////////////////////////////////////////
    void SpriteManager::loadSpriteMetaData(SpritePtr const& _sprite, DataBlock const& _metaData)
    {
        _sprite->set(
            _sprite->getTexture(),
            _metaData.getVec2F(MAZE_HCS("colorPosition"), Vec2S::c_zero),
            _metaData.getVec2F(MAZE_HCS("colorSize"), Vec2S::c_zero),
            _metaData.getVec2F(MAZE_HCS("colorOffset"), Vec2S::c_zero),
            _metaData.getVec2F(MAZE_HCS("nativeSize"), Vec2S::c_zero));

        if (DataBlock const* sliceBorder = _metaData.getDataBlock(MAZE_HCS("sliceBorder")))
        {
            F32 left = sliceBorder->getF32(MAZE_HCS("left"), 0.0f);
            F32 bottom = sliceBorder->getF32(MAZE_HCS("bottom"), 0.0f);
            F32 right = sliceBorder->getF32(MAZE_HCS("right"), 0.0f);
            F32 top = sliceBorder->getF32(MAZE_HCS("top"), 0.0f);
            _sprite->setSliceBorder(left, bottom, right, top);
        }
    }

    //////////////////////////////////////////
    SpriteLibraryData* SpriteManager::addSpriteToLibrary(
        SpritePtr const& _sprite,
        SpriteLibraryDataCallbacks const& _callbacks)
    {
        MAZE_ERROR_IF(_sprite->getName().empty(), "Sprite with no name!");

        auto it2 = m_spritesLibrary.insert(
            _sprite->getName(),
            { _sprite, _callbacks });

        return it2;
    }

    //////////////////////////////////////////
    void SpriteManager::removeSpriteFromLibrary(HashedCString _textureName)
    {
        m_spritesLibrary.erase(_textureName);
    }

    //////////////////////////////////////////
    void SpriteManager::unloadAssetSprites(Set<String> const& _tags)
    {
        MAZE_PROFILE_EVENT("SpriteManager::unloadAssetSprites");

        StringKeyMap<SpriteLibraryData>::iterator it = m_spritesLibrary.begin();
        StringKeyMap<SpriteLibraryData>::iterator end = m_spritesLibrary.end();
        for (; it != end; )
        {
            bool unload = false;

            if (it->second.sprite && it->second.sprite->getTexture())
            {
                Texture2DLibraryData const* textureLibraryData = m_renderSystemRaw->getTextureManager()->getTexture2DLibraryData(it->second.sprite->getTexture()->getName().asHashedCString());
                unload = textureLibraryData && textureLibraryData->callbacks.hasAnyOfTags && textureLibraryData->callbacks.hasAnyOfTags(_tags);
            }

            if (unload)
            {
                it = m_spritesLibrary.erase(it);
                end = m_spritesLibrary.end();
            }
            else
            {
                ++it;
            }
        }
    }

    //////////////////////////////////////////
    HashedCString SpriteManager::getSpriteName(Sprite const* _sprite)
    {
        for (StringKeyMap<SpriteLibraryData>::iterator it = m_spritesLibrary.begin(),
                                                       end = m_spritesLibrary.end();
                                                       it != end;
                                                       ++it)
        {
            if (it->second.sprite.get() == _sprite)
                return it.key();
        }

        return HashedCString();
    }

    //////////////////////////////////////////
    SpritePtr const& SpriteManager::createBuiltinSprite(BuiltinSpriteType _spriteType)
    {
        SpritePtr& sprite = m_builtinSprites[_spriteType];

        switch (_spriteType)
        {
        case BuiltinSpriteType::White:
        {
            sprite = Sprite::Create(
                TextureManager::GetCurrentInstancePtr()->ensureBuiltinTexture2D(
                    BuiltinTexture2DType::White));
            break;
        }
        case BuiltinSpriteType::Black:
        {
            sprite = Sprite::Create(
                TextureManager::GetCurrentInstancePtr()->ensureBuiltinTexture2D(
                    BuiltinTexture2DType::Black));
            break;
        }
        case BuiltinSpriteType::Error:
        {
            sprite = Sprite::Create(
                TextureManager::GetCurrentInstancePtr()->ensureBuiltinTexture2D(
                    BuiltinTexture2DType::Error));
            break;
        }
        default:
        {
            MAZE_NOT_IMPLEMENTED;
            break;
        }
        }

        if (sprite)
        {
            sprite->setName(_spriteType.toCString());
            addSpriteToLibrary(sprite);
        }

        return sprite;
    }

    //////////////////////////////////////////
    SpritePtr const& SpriteManager::ensureBuiltinSprite(BuiltinSpriteType _spriteType)
    {
        SpritePtr const& sprite = getBuiltinSprite(_spriteType);
        if (sprite)
            return sprite;

        return createBuiltinSprite(_spriteType);
    }

    //////////////////////////////////////////
    void SpriteManager::createBuiltinSprites()
    {
        MAZE_PROFILE_EVENT("SpriteManager::createBuiltinSprites");

        for (BuiltinSpriteType t = BuiltinSpriteType(1); t < BuiltinSpriteType::MAX; ++t)
            ensureBuiltinSprite(t);

        eventBuiltinSpritesCreated();
    }

} // namespace Maze
//////////////////////////////////////////
