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
    SpritePtr const& SpriteManager::getSprite(String const& _imageName)
    {
        static SpritePtr nullPointer;

        UnorderedMap<String, SpritePtr>::const_iterator it = m_imagesByName.find(_imageName);
        if (it != m_imagesByName.end())
            return it->second;
        
        TextureManagerPtr const& textureManager = m_renderSystemRaw->getTextureManager();

        Texture2DPtr texture2D = textureManager->getTexture2D(_imageName);
        if (texture2D)
        {
            SpritePtr image = Sprite::Create(texture2D);
            auto it2 = m_imagesByName.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(_imageName),
                std::forward_as_tuple(image));
            return it2.first->second;
        }

        return nullPointer;
    }

} // namespace Maze
//////////////////////////////////////////
