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
#include "maze-graphics/MazeTextureCube.hpp"
#include "maze-graphics/helpers/MazeGraphicsUtilsHelper.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderBMP.hpp"


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
    TextureManagerPtr const& TextureManager::GetCurrentInstancePtr()
    {
        return RenderSystem::GetCurrentInstancePtr()->getTextureManager();
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

        UnorderedMap<String, Texture2DPtr>::const_iterator it = m_textures2DByName.find(_textureName);
        if (it != m_textures2DByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);
        if (!assetFile)
            return nullPointer;

        return getTexture2D(assetFile);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::getTexture2D(AssetFilePtr const& _assetFile)
    {
        UnorderedMap<String, Texture2DPtr>::const_iterator it = m_textures2DByName.find(_assetFile->getFileName());
        if (it != m_textures2DByName.end())
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
        m_whiteTexture->loadTexture(PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white));
        m_whiteTexture->setMagFilter(TextureFilter::Nearest);
        m_whiteTexture->setMinFilter(TextureFilter::Nearest);
        addTexture(m_whiteTexture);

        m_blackTexture = Texture2D::Create(m_renderSystemRaw);
        m_blackTexture->setName("black");
        m_blackTexture->loadTexture(PixelSheet2D(Vec2DS(1, 1), ColorU32::c_black));
        m_blackTexture->setMagFilter(TextureFilter::Nearest);
        m_blackTexture->setMinFilter(TextureFilter::Nearest);
        addTexture(m_blackTexture);

        m_errorTexture = Texture2D::Create(m_renderSystemRaw);
        m_errorTexture->setName("error");
        m_errorTexture->loadTexture(PixelSheet2D(Vec2DS(1, 1), ColorU32::c_magenta));
        m_errorTexture->setMagFilter(TextureFilter::Nearest);
        m_errorTexture->setMinFilter(TextureFilter::Nearest);
        addTexture(m_errorTexture);

        m_whiteCubeTexture = TextureCube::Create(m_renderSystemRaw);
        m_whiteCubeTexture->setName("white_cube");
        {
            PixelSheet2D faces[6] = 
                { 
                    PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white),
                    PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white),
                    PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white),
                    PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white),
                    PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white),
                    PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white),
                };

            m_whiteCubeTexture->loadTexture(faces);
        }
        addTexture(m_whiteCubeTexture);

        m_testCubeTexture = TextureCube::Create(m_renderSystemRaw);
        m_testCubeTexture->setName("test_cube");
        {
            PixelSheet2D faces[6] =
            {
                PixelSheet2D(Vec2DS(1, 1), ColorU32::c_red),
                PixelSheet2D(Vec2DS(1, 1), ColorU32::c_green),
                PixelSheet2D(Vec2DS(1, 1), ColorU32::c_blue),
                PixelSheet2D(Vec2DS(1, 1), ColorU32::c_cyan),
                PixelSheet2D(Vec2DS(1, 1), ColorU32::c_magenta),
                PixelSheet2D(Vec2DS(1, 1), ColorU32::c_yellow),
            };
            
            m_testCubeTexture->loadTexture(faces);
        }
        addTexture(m_testCubeTexture);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::addTexture(Texture2DPtr const& _texture)
    {
        auto it2 = m_textures2DByName.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_texture->getName()),
            std::forward_as_tuple(_texture));

        return it2.first->second;
    }

    //////////////////////////////////////////
    Vector<Texture2DPtr> TextureManager::getTextures2DSorted()
    {
        Vector<Texture2DPtr> result;

        for (auto const& value : m_textures2DByName)
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
    Vector<PixelSheet2D> TextureManager::loadPixelSheets2D(AssetFilePtr const& _assetFile)
    {
        Vector<PixelSheet2D> pixelSheets;

        if (!_assetFile)
            return pixelSheets;

        Debug::Log("Loading texture pixel sheet: %s...", _assetFile->getFileName().c_str());

        UnorderedMap<String, String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);

        if (metaData.empty())
        {
            if (Maze::IsBMPFile(_assetFile))
            {
                MAZE_ERROR_IF(!Maze::LoadBMP(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            if (Maze::IsPNGFile(_assetFile))
            {
                MAZE_ERROR_IF(!Maze::LoadPNG(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format!");
            }
        }
        else
        {
            String fileExtension = StringHelper::ToLower(metaData["ext"]);
            if (fileExtension == "bmp")
            {
                MAZE_ERROR_IF(!Maze::LoadBMP(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            if (fileExtension == "png")
            {
                MAZE_ERROR_IF(!Maze::LoadPNG(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format!");
            }
        }

        Debug::Log("Loaded.", _assetFile->getFileName().c_str());

        return pixelSheets;
    }

    //////////////////////////////////////////
    Vector<PixelSheet2D> TextureManager::loadPixelSheets2D(String const& _assetFileName)
    {
        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_assetFileName);

        MAZE_WARNING_IF(!assetFile, "Undefined asset: %s", _assetFileName.c_str());

        return loadPixelSheets2D(assetFile);
    }


    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::getTextureCube(String const& _textureName)
    {
        static TextureCubePtr nullPointer;

        UnorderedMap<String, TextureCubePtr>::const_iterator it = m_texturesCubeByName.find(_textureName);
        if (it != m_texturesCubeByName.end())
            return it->second;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);
        if (!assetFile)
            return nullPointer;

        return getTextureCube(assetFile);
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::getTextureCube(AssetFilePtr const& _assetFile)
    {
        UnorderedMap<String, TextureCubePtr>::const_iterator it = m_texturesCubeByName.find(_assetFile->getFileName());
        if (it != m_texturesCubeByName.end())
            return it->second;

        TextureCubePtr textureCube = TextureCube::Create(_assetFile, m_renderSystemRaw);
        textureCube->setName(_assetFile->getFileName());

        UnorderedMap<String, String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);
        if (metaData["magFilter"] != String())
            textureCube->setMagFilter(TextureFilter::FromString(metaData["magFilter"]));
        if (metaData["minFilter"] != String())
            textureCube->setMinFilter(TextureFilter::FromString(metaData["minFilter"]));
        if (metaData["wrapS"] != String())
            textureCube->setWrapS(TextureWrap::FromString(metaData["wrapS"]));
        if (metaData["wrapT"] != String())
            textureCube->setWrapT(TextureWrap::FromString(metaData["wrapT"]));
        if (metaData["wrapR"] != String())
            textureCube->setWrapR(TextureWrap::FromString(metaData["wrapR"]));

        return addTexture(textureCube);
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::addTexture(TextureCubePtr const& _texture)
    {
        auto it2 = m_texturesCubeByName.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_texture->getName()),
            std::forward_as_tuple(_texture));

        return it2.first->second;
    }

    //////////////////////////////////////////
    void TextureManager::loadAllAssetTextures()
    {
        {
            Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions({ "mztexture" });
            for (AssetFilePtr const& assetFile : assetFiles)
            {
                getTexture2D(assetFile);
            }
        }

        {
            Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions({ "mzcubemap" });
            for (AssetFilePtr const& assetFile : assetFiles)
            {
                getTextureCube(assetFile);
            }
        }
    }

    //////////////////////////////////////////
    void TextureManager::reloadAllAssetTextures()
    {
        for (auto const& textureData : m_textures2DByName)
        {
            if (textureData.second->getAssetFile())
                textureData.second->reload();
        }
    }

    
} // namespace Maze
//////////////////////////////////////////
