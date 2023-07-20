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
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-graphics/MazeSprite.hpp"


//////////////////////////////////////////
namespace Maze
{

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
    SpritePtr const& SpriteManager::getSprite(HashedCString _imageName)
    {
        static SpritePtr nullPointer;

        SpriteLibraryData const* libraryData = getSpriteLibraryData(_imageName);
        if (libraryData)
            return libraryData->sprite;
        
        TextureManagerPtr const& textureManager = m_renderSystemRaw->getTextureManager();

        Texture2DPtr texture2D = textureManager->getTexture2D(_imageName);
        if (!texture2D)
            return nullPointer;

        SpritePtr sprite = Sprite::Create(texture2D);

        Texture2DLibraryData const* textureLibraryData = m_renderSystemRaw->getTextureManager()->getTexture2DLibraryData(texture2D->getName().asHashedCString());
        if (textureLibraryData && textureLibraryData->assetFile)
        {
            StringKeyMap<String> metaData = AssetManager::GetInstancePtr()->getMetaData(textureLibraryData->assetFile);
            auto metaDataIt = metaData.find("sliceBorder");
            if (metaDataIt != metaData.end())
            {
                String const& borderData = metaDataIt->second;
                Vector<String> words;
                StringHelper::SplitWords(borderData, words, ',');
                if (words.size() == 4)
                {
                    sprite->setSliceBorder(
                        StringHelper::StringToF32(words[0]),
                        StringHelper::StringToF32(words[1]),
                        StringHelper::StringToF32(words[2]),
                        StringHelper::StringToF32(words[3]));
                }
            }
        }

        SpriteLibraryData* data = addSpriteToLibrary(sprite);
        return data->sprite;
    }

    //////////////////////////////////////////
    SpriteLibraryData* SpriteManager::addSpriteToLibrary(SpritePtr const& _sprite)
    {
        MAZE_ERROR_IF(_sprite->getName().empty(), "Sprite with no name!");

        auto it2 = m_spritesLibrary.insert(
            _sprite->getName(),
            _sprite);

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
        StringKeyMap<SpriteLibraryData>::iterator it = m_spritesLibrary.begin();
        StringKeyMap<SpriteLibraryData>::iterator end = m_spritesLibrary.end();
        for (; it != end; )
        {
            bool unload = false;

            if (it->second.sprite && it->second.sprite->getTexture())
            {
                Texture2DLibraryData const* textureLibraryData = m_renderSystemRaw->getTextureManager()->getTexture2DLibraryData(it->second.sprite->getTexture()->getName().asHashedCString());
                unload = textureLibraryData && textureLibraryData->assetFile && textureLibraryData->assetFile->hasAnyOfTags(_tags);
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

} // namespace Maze
//////////////////////////////////////////
