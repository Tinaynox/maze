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
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/managers/MazeUpdateManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/preprocessor/MazePreprocessor_Memory.hpp"
#include "maze-core/memory/MazeMemory.hpp"
#include "maze-core/helpers/MazeWindowHelper.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class TextureManager
    //
    //////////////////////////////////////////
    TextureManager::TextureManager()
        : m_renderSystemRaw(nullptr)
    {
        
    }

    //////////////////////////////////////////
    TextureManager::~TextureManager()
    {
        if (m_renderSystemRaw)
            m_renderSystemRaw->eventSystemInited.unsubscribe(this);
    }

    //////////////////////////////////////////
    void TextureManager::Initialize(TextureManagerPtr& _textureManager, RenderSystemPtr const& _renderSystem)
    {
        MAZE_CREATE_AND_INIT_SHARED_PTR(TextureManager, _textureManager, init(_renderSystem));
    }

    //////////////////////////////////////////
    bool TextureManager::init(RenderSystemPtr const& _renderSystem)
    {
        m_renderSystem = _renderSystem;
        m_renderSystemRaw = _renderSystem.get();

        m_renderSystemRaw->eventSystemInited.subscribe(this, &TextureManager::notifyRenderSystemInited);

        return true;
    }

    //////////////////////////////////////////
    void TextureManager::notifyRenderSystemInited()
    {
        createSpecialTextures();
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::getTexture2D(String const& _textureName)
    {
        static Texture2DPtr nullPointer;

        UnorderedMap<String, Texture2DPtr>::const_iterator it = m_texturesByName.find(_textureName);
        if (it != m_texturesByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);
        if (!assetFile)
            return nullPointer;

        return getTexture2D(assetFile);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::getTexture2D(AssetFilePtr const& _assetFile)
    {
        UnorderedMap<String, Texture2DPtr>::const_iterator it = m_texturesByName.find(_assetFile->getFileName());
        if (it != m_texturesByName.end())
            return it->second;

        Texture2DPtr texture2D = Texture2D::Create(_assetFile, m_renderSystemRaw);
        texture2D->setName(_assetFile->getFileName());

        UnorderedMap<String, String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);
        if (metaData["magFilter"] != String())
            texture2D->setMagFilter(TextureFilter::FromString(metaData["magFilter"]));
        if (metaData["minFilter"] != String())
            texture2D->setMinFilter(TextureFilter::FromString(metaData["minFilter"]));
        if (metaData["wrapS"] != String())
            texture2D->setWrapS(TextureWrap::FromString(metaData["wrapS"]));
        if (metaData["wrapT"] != String())
            texture2D->setWrapT(TextureWrap::FromString(metaData["wrapT"]));

        return addTexture(texture2D);
    }

    //////////////////////////////////////////
    void TextureManager::createSpecialTextures()
    {
        m_whiteTexture = Texture2D::Create(m_renderSystemRaw);
        m_whiteTexture->setName("white");
        PixelSheet2D pixelSheet(Vec2DS(1,1), PixelFormat::RGBA_U8);
        pixelSheet.fill(ColorU32::c_white);
        m_whiteTexture->loadTexture(pixelSheet);
        m_whiteTexture->setMagFilter(TextureFilter::Nearest);
        m_whiteTexture->setMinFilter(TextureFilter::Nearest);
        addTexture(m_whiteTexture);

        m_blackTexture = Texture2D::Create(m_renderSystemRaw);
        m_blackTexture->setName("black");
        pixelSheet.fill(ColorU32::c_black);
        m_blackTexture->loadTexture(pixelSheet);
        m_blackTexture->setMagFilter(TextureFilter::Nearest);
        m_blackTexture->setMinFilter(TextureFilter::Nearest);
        addTexture(m_blackTexture);

        m_errorTexture = Texture2D::Create(m_renderSystemRaw);
        m_errorTexture->setName("error");
        pixelSheet.fill(ColorU32::c_magenta);
        m_errorTexture->loadTexture(pixelSheet);
        m_errorTexture->setMagFilter(TextureFilter::Nearest);
        m_errorTexture->setMinFilter(TextureFilter::Nearest);
        addTexture(m_errorTexture);

        m_systemFontTexture = Texture2D::Create(m_renderSystemRaw);
        m_systemFontTexture->setName("system_font");
        m_systemFontTexture->loadTexture(GraphicsUtilsHelper::GetSystemFontSheet());
        // m_systemFontTexture->setMagFilter(TextureFilter::Nearest);
        // m_systemFontTexture->setMinFilter(TextureFilter::NearestMipmapLinear);
        m_systemFontTexture->setMagFilter(TextureFilter::Nearest);
        m_systemFontTexture->setMinFilter(TextureFilter::LinearMipmapLinear);
        addTexture(m_systemFontTexture);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::addTexture(Texture2DPtr const& _texture)
    {
        auto it2 = m_texturesByName.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_texture->getName()),
            std::forward_as_tuple(_texture));

        return it2.first->second;
    }

    //////////////////////////////////////////
    Vector<Texture2DPtr> TextureManager::getTexturesSorted()
    {
        Vector<Texture2DPtr> result;

        for (auto const& value : m_texturesByName)
            result.emplace_back(value.second);

        std::sort(
            result.begin(),
            result.end(),
            [](Texture2DPtr const& _a, Texture2DPtr const& _b)
            {
                return _a->getName() < _b->getName();
            });

        return result;
    }

    //////////////////////////////////////////
    void TextureManager::loadAllAssetTextures()
    {
        Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions({ "mztexture" });
        for (AssetFilePtr const& assetFile : assetFiles)
        {
            getTexture2D(assetFile);
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
