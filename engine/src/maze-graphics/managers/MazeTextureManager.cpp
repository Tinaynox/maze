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
#include "maze-graphics/loaders/texture/MazeLoaderBMP.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinTexture2DType);


    //////////////////////////////////////////
    MAZE_IMPLEMENT_ENUMCLASS(BuiltinTextureCubeType);


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

        registerTextureLoader(
            MAZE_HASHED_CSTRING("bmp"),
            TextureLoaderData(
                (LoadTextureAssetFileFunction)&LoadBMP,
                (LoadTextureByteBufferFunction)&LoadBMP,
                (IsTextureAssetFileFunction)&IsBMPFile,
                (IsTextureByteBufferFunction)&IsBMPFile));

        return true;
    }

    //////////////////////////////////////////
    void TextureManager::notifyRenderSystemInited()
    {
        // createBuiltinTextures();
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::getTexture2D(HashedCString _textureName)
    {
        static Texture2DPtr nullPointer;

        StringKeyMap<Texture2DPtr>::const_iterator it = m_textures2DByName.find(_textureName);
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
        StringKeyMap<Texture2DPtr>::const_iterator it = m_textures2DByName.find(_assetFile->getFileName());
        if (it != m_textures2DByName.end())
            return it->second;

        Texture2DPtr texture2D = Texture2D::Create(_assetFile, m_renderSystemRaw);
        texture2D->setName(_assetFile->getFileName());

        StringKeyMap<String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);
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
    Texture2DPtr const& TextureManager::ensureBuiltinTexture2D(BuiltinTexture2DType _texture2DType)
    {
        Texture2DPtr const& texture = getBuiltinTexture2D(_texture2DType);
        if (texture)
            return texture;

        return createBuiltinTexture2D(_texture2DType);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::createBuiltinTexture2D(BuiltinTexture2DType _texture2DType)
    {
        Texture2DPtr& texture = m_builtinTexture2Ds[_texture2DType];

        switch (_texture2DType)
        {
            case BuiltinTexture2DType::White:
            {
                texture = Texture2D::Create(m_renderSystemRaw);
                texture->loadTexture(PixelSheet2D(Vec2DS(1, 1), ColorU32::c_white));
                texture->setMagFilter(TextureFilter::Nearest);
                texture->setMinFilter(TextureFilter::Nearest);
                break;
            }
            case BuiltinTexture2DType::Black:
            {
                texture = Texture2D::Create(m_renderSystemRaw);
                texture->loadTexture(PixelSheet2D(Vec2DS(1, 1), ColorU32::c_black));
                texture->setMagFilter(TextureFilter::Nearest);
                texture->setMinFilter(TextureFilter::Nearest);
                break;
            }
            case BuiltinTexture2DType::Error:
            {
                texture = Texture2D::Create(m_renderSystemRaw);
                texture->loadTexture(PixelSheet2D(Vec2DS(1, 1), ColorU32::c_magenta));
                texture->setMagFilter(TextureFilter::Nearest);
                texture->setMinFilter(TextureFilter::Nearest);
                break;
            }
        }

        if (texture)
        {
            texture->setName(_texture2DType.toCString());
            addTexture(texture);
        }

        return texture;
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::createBuiltinTextureCube(BuiltinTextureCubeType _textureCubeType)
    {
        TextureCubePtr& texture = m_builtinTextureCubes[_textureCubeType];

        switch (_textureCubeType)
        {
            case BuiltinTextureCubeType::White:
            {
                texture = TextureCube::Create(m_renderSystemRaw);
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

                    texture->loadTexture(faces);
                }
                break;
            }
            case BuiltinTextureCubeType::Test:
            {
                texture = TextureCube::Create(m_renderSystemRaw);
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

                    texture->loadTexture(faces);
                }
                break;
            }
        }

        if (texture)
        {
            texture->setName(_textureCubeType.toCString());
            addTexture(texture);
        }

        return texture;
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::ensureBuiltinTextureCube(BuiltinTextureCubeType _textureCubeType)
    {
        TextureCubePtr const& texture = getBuiltinTextureCube(_textureCubeType);
        if (texture)
            return texture;

        return createBuiltinTextureCube(_textureCubeType);
    }

    //////////////////////////////////////////
    void TextureManager::createBuiltinTextures()
    {
        for (BuiltinTexture2DType t = BuiltinTexture2DType(1); t < BuiltinTexture2DType::MAX; ++t)
            ensureBuiltinTexture2D(t);
        
        for (BuiltinTextureCubeType t = BuiltinTextureCubeType(1); t < BuiltinTextureCubeType::MAX; ++t)
            ensureBuiltinTextureCube(t);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::addTexture(Texture2DPtr const& _texture)
    {
        auto it2 = m_textures2DByName.insert(
            _texture->getName(),
            _texture);

        return *it2;
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

        StringKeyMap<String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);

        if (metaData.empty())
        {
            bool loaderFound = false;
            for (auto const& textureLoaderData : m_textureLoaders)
            {
                TextureLoaderData const& loaderData = textureLoaderData.second;
                if (loaderData.isTextureAssetFileFunc(_assetFile))
                {
                    loaderFound = true;
                    MAZE_ERROR_IF(!loaderData.loadTextureAssetFileFunc(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
                    break;
                }
            }

            MAZE_ERROR_IF(!loaderFound, "Unsupported texture format - %s!", _assetFile->getFileName().c_str());
        }
        else
        {
            HashedString fileExtension = StringHelper::ToLower(metaData["ext"]);

            auto it = m_textureLoaders.find(fileExtension);
            if (it != m_textureLoaders.end())
            {
                TextureLoaderData const& loaderData = it->second;
                MAZE_ERROR_IF(!loaderData.loadTextureAssetFileFunc(_assetFile, pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format - %s!", _assetFile->getFileName().c_str());
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

        StringKeyMap<TextureCubePtr>::const_iterator it = m_texturesCubeByName.find(_textureName);
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
        StringKeyMap<TextureCubePtr>::const_iterator it = m_texturesCubeByName.find(_assetFile->getFileName());
        if (it != m_texturesCubeByName.end())
            return it->second;

        TextureCubePtr textureCube = TextureCube::Create(_assetFile, m_renderSystemRaw);
        textureCube->setName(_assetFile->getFileName());

        StringKeyMap<String> metaData = AssetManager::GetInstancePtr()->getMetaData(_assetFile);
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
        auto it2 = m_texturesCubeByName.insert(
            _texture->getName(),
            _texture);

        return *it2;
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
