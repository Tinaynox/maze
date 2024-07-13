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
#include "maze-core/helpers/MazeStringHelper.hpp"
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
    Texture2DLibraryData const* TextureManager::getTexture2DLibraryData(HashedCString _textureName)
    {
        StringKeyMap<Texture2DLibraryData>::const_iterator it = m_textures2DLibrary.find(_textureName);
        if (it != m_textures2DLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::getTexture2D(HashedCString _textureName)
    {
        static Texture2DPtr const nullPointer;

        Texture2DLibraryData const* libraryData = getTexture2DLibraryData(_textureName);
        if (libraryData)
            return libraryData->texture;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);
        if (!assetFile)
            return nullPointer;

        return getTexture2D(assetFile);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::getTexture2D(AssetFilePtr const& _assetFile)
    {
        static Texture2DPtr const nullPointer;

        Texture2DLibraryData const* libraryData = getTexture2DLibraryData(_assetFile->getFileName());
        if (libraryData)
            return libraryData->texture;

        Texture2DPtr texture2D = Texture2D::Create(_assetFile, m_renderSystemRaw);
        texture2D->setName(_assetFile->getFileName());

        loadTextureMetaData(_assetFile, texture2D);

        Texture2DLibraryData* data = addTextureToLibrary(texture2D);
        if (data)
        {
            data->assetFile = _assetFile;
            return data->texture;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    bool TextureManager::loadTextureMetaData(AssetFilePtr const& _assetFile, Texture2DPtr const& _texture)
    {
        Texture2DPtr const& texture2D = _texture;
        if (!texture2D)
            return false;

        DataBlock metaData;
        if (AssetManager::GetInstancePtr()->getMetaData(_assetFile, metaData))
        {
            if (metaData.isParamExists(MAZE_HCS("magFilter")))
                texture2D->setMagFilter(
                    TextureFilter::FromString(
                        metaData.getString(MAZE_HCS("magFilter"))));
            
            if (metaData.isParamExists(MAZE_HCS("minFilter")))
                texture2D->setMinFilter(
                    TextureFilter::FromString(
                        metaData.getString(MAZE_HCS("minFilter"))));
            
            if (metaData.isParamExists(MAZE_HCS("wrapS")))
                texture2D->setWrapS(
                    TextureWrap::FromString(
                        metaData.getString(MAZE_HCS("wrapS"))));
            
            if (metaData.isParamExists(MAZE_HCS("wrapT")))
                texture2D->setWrapT(
                    TextureWrap::FromString(
                        metaData.getString(MAZE_HCS("wrapT"))));
            
            if (metaData.isParamExists(MAZE_HCS("anisotropy")))
                texture2D->setAnisotropyLevel(
                    metaData.getF32(MAZE_HCS("anisotropy")));
        }

        return true;
    }

    //////////////////////////////////////////
    bool TextureManager::saveTextureMetaData(AssetFilePtr const& _assetFile)
    {
        Texture2DPtr const& texture2D = getTexture2D(_assetFile);
        if (!texture2D)
            return false;

        DataBlock metaData;
        AssetManager::GetInstancePtr()->getMetaData(_assetFile, metaData);
        
        metaData.setString(MAZE_HCS("magFilter"), texture2D->getMagFilter().toString());
        metaData.setString(MAZE_HCS("minFilter"), texture2D->getMinFilter().toString());
        metaData.setString(MAZE_HCS("wrapS"), texture2D->getWrapS().toString());
        metaData.setString(MAZE_HCS("wrapT"), texture2D->getWrapT().toString());
        metaData.setF32(MAZE_HCS("anisotropy"), texture2D->getAnisotropyLevel());

        AssetManager::GetInstancePtr()->saveMetaData(_assetFile, metaData);

        return true;
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
                texture->loadTexture(PixelSheet2D(Vec2S(1, 1), ColorU32::c_white));
                texture->setMagFilter(TextureFilter::Nearest);
                texture->setMinFilter(TextureFilter::Nearest);
                break;
            }
            case BuiltinTexture2DType::Black:
            {
                texture = Texture2D::Create(m_renderSystemRaw);
                texture->loadTexture(PixelSheet2D(Vec2S(1, 1), ColorU32::c_black));
                texture->setMagFilter(TextureFilter::Nearest);
                texture->setMinFilter(TextureFilter::Nearest);
                break;
            }
            case BuiltinTexture2DType::Normal:
            {
                texture = Texture2D::Create(m_renderSystemRaw);
                texture->loadTexture(PixelSheet2D(Vec2S(1, 1), ColorU32(127, 127, 255)));
                texture->setMagFilter(TextureFilter::Nearest);
                texture->setMinFilter(TextureFilter::Nearest);
                break;
            }
            case BuiltinTexture2DType::Error:
            {
                texture = Texture2D::Create(m_renderSystemRaw);
                texture->loadTexture(PixelSheet2D(Vec2S(1, 1), ColorU32::c_magenta));
                texture->setMagFilter(TextureFilter::Nearest);
                texture->setMinFilter(TextureFilter::Nearest);
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        if (texture)
        {
            texture->setName(_texture2DType.toCString());
            addTextureToLibrary(texture);
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
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_white),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_white),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_white),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_white),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_white),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_white),
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
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_red),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_green),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_blue),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_cyan),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_magenta),
                        PixelSheet2D(Vec2S(1, 1), ColorU32::c_yellow),
                    };

                    texture->loadTexture(faces);
                }
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
                break;
            }
        }

        if (texture)
        {
            texture->setName(_textureCubeType.toCString());
            addTextureToLibrary(texture);
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
        MAZE_PROFILE_EVENT("TextureManager::createBuiltinTextures");

        for (BuiltinTexture2DType t = BuiltinTexture2DType(1); t < BuiltinTexture2DType::MAX; ++t)
            ensureBuiltinTexture2D(t);
        
        for (BuiltinTextureCubeType t = BuiltinTextureCubeType(1); t < BuiltinTextureCubeType::MAX; ++t)
            ensureBuiltinTextureCube(t);
    }

    //////////////////////////////////////////
    Texture2DLibraryData* TextureManager::addTextureToLibrary(Texture2DPtr const& _texture)
    {
        auto it2 = m_textures2DLibrary.insert(
            _texture->getName(),
            { _texture, nullptr });

        return it2;
    }

    //////////////////////////////////////////
    void TextureManager::removeTexture2DFromLibrary(HashedCString _textureName)
    {
        m_textures2DLibrary.erase(_textureName);
    }

    //////////////////////////////////////////
    Vector<Texture2DPtr> TextureManager::getTextures2DSorted()
    {
        Vector<Texture2DPtr> result;

        for (auto const& value : m_textures2DLibrary)
            result.emplace_back(value.second.texture);

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
        MAZE_PROFILE_EVENT("TextureManager::loadPixelSheets2D");

        Vector<PixelSheet2D> pixelSheets;

        if (!_assetFile)
            return pixelSheets;

        Debug::Log("Loading texture pixel sheet: %s...", _assetFile->getFileName().toUTF8().c_str());
        Timer timer;

        DataBlock metaData;
        AssetManager::GetInstancePtr()->getMetaData(_assetFile, metaData);

        if (metaData.isEmpty() || !metaData.isParamExists(MAZE_HCS("ext")))
        {
            String assetFileExtension = _assetFile->getExtension().toUTF8();
            bool loaderFound = false;

            if (!assetFileExtension.empty())
            {
                auto it = m_textureLoaders.find(assetFileExtension);
                if (it != m_textureLoaders.end())
                {
                    loaderFound = true;
                    MAZE_ERROR_IF(!it->second.loadTextureAssetFileFunc(*_assetFile.get(), pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
                }
            }
            else
            {
                for (auto const& textureLoaderData : m_textureLoaders)
                {
                    TextureLoaderData const& loaderData = textureLoaderData.second;
                    if (loaderData.isTextureAssetFileFunc(*_assetFile.get()))
                    {
                        loaderFound = true;
                        MAZE_ERROR_IF(!loaderData.loadTextureAssetFileFunc(*_assetFile.get(), pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
                        break;
                    }
                }
            }

            MAZE_ERROR_IF(!loaderFound, "Unsupported texture format - %s!", _assetFile->getFileName().toUTF8().c_str());
        }
        else
        {
            HashedString fileExtension = HashedString(
                StringHelper::ToLower(
                    metaData.getString(MAZE_HCS("ext"))));

            auto it = m_textureLoaders.find(fileExtension);
            if (it != m_textureLoaders.end())
            {
                TextureLoaderData const& loaderData = it->second;
                MAZE_ERROR_IF(!loaderData.loadTextureAssetFileFunc(*_assetFile.get(), pixelSheets), "PixelSheet is not loaded - '%s'", _assetFile->getFileName().toUTF8().c_str());
            }
            else
            {
                MAZE_ERROR("Unsupported texture format - %s!", _assetFile->getFileName().toUTF8().c_str());
            }
        }

        F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
        Debug::Log("Texture pixel sheet %s loaded for %.1fms.", _assetFile->getFileName().toUTF8().c_str(), msTime);

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
    TextureCubeLibraryData const* TextureManager::getTextureCubeLibraryData(HashedCString _textureName)
    {
        StringKeyMap<TextureCubeLibraryData>::const_iterator it = m_texturesCubeLibrary.find(_textureName);
        if (it != m_texturesCubeLibrary.end())
            return &it->second;
        return nullptr;
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::getTextureCube(String const& _textureName)
    {
        static TextureCubePtr const nullPointer;

        TextureCubeLibraryData const* libraryData = getTextureCubeLibraryData(_textureName);
        if (libraryData)
            return libraryData->texture;

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);
        if (!assetFile)
            return nullPointer;

        return getTextureCube(assetFile);
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::getTextureCube(AssetFilePtr const& _assetFile)
    {
        static TextureCubePtr const nullPointer;

        TextureCubeLibraryData const* libraryData = getTextureCubeLibraryData(_assetFile->getFileName());
        if (libraryData)
            return libraryData->texture;

        TextureCubePtr textureCube = TextureCube::Create(_assetFile, m_renderSystemRaw);
        textureCube->setName(_assetFile->getFileName());

        DataBlock metaData;
        if (AssetManager::GetInstancePtr()->getMetaData(_assetFile, metaData))
        {
            if (metaData.isParamExists(MAZE_HCS("magFilter")))
                textureCube->setMagFilter(
                    TextureFilter::FromString(
                        metaData.getString(MAZE_HCS("magFilter"))));

            if (metaData.isParamExists(MAZE_HCS("minFilter")))
                textureCube->setMinFilter(
                    TextureFilter::FromString(
                        metaData.getString(MAZE_HCS("minFilter"))));

            if (metaData.isParamExists(MAZE_HCS("wrapS")))
                textureCube->setWrapS(
                    TextureWrap::FromString(
                        metaData.getString(MAZE_HCS("wrapS"))));

            if (metaData.isParamExists(MAZE_HCS("wrapT")))
                textureCube->setWrapT(
                    TextureWrap::FromString(
                        metaData.getString(MAZE_HCS("wrapT"))));

            if (metaData.isParamExists(MAZE_HCS("wrapR")))
                textureCube->setWrapR(
                    TextureWrap::FromString(
                        metaData.getString(MAZE_HCS("wrapR"))));
        }

        TextureCubeLibraryData* data = addTextureToLibrary(textureCube);
        if (data)
        {
            data->assetFile = _assetFile;
            return data->texture;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    TextureCubeLibraryData* TextureManager::addTextureToLibrary(TextureCubePtr const& _texture)
    {
        auto it2 = m_texturesCubeLibrary.insert(
            _texture->getName(),
            { _texture, nullptr });

        return it2;
    }

    //////////////////////////////////////////
    void TextureManager::removeTextureCubeFromLibrary(HashedCString _textureName)
    {
        m_texturesCubeLibrary.erase(_textureName);
    }

    //////////////////////////////////////////
    void TextureManager::loadAllAssetTextures()
    {
        MAZE_PROFILE_EVENT("TextureManager::loadAllAssetTextures");
        {
            Vector<String> textureExtensions = getTextureLoaderExtensions();
            textureExtensions.push_back("mztexture");

            Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(
                Set<String>(textureExtensions.begin(), textureExtensions.end()));
            for (AssetFilePtr const& assetFile : assetFiles)
            {
                getTexture2D(assetFile);
            }
        }

        {
            Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(Set<Path>{ "mzcubemap" });
            for (AssetFilePtr const& assetFile : assetFiles)
            {
                getTextureCube(assetFile);
            }
        }
    }

    //////////////////////////////////////////
    void TextureManager::reloadAllAssetTextures()
    {
        for (auto const& textureData : m_textures2DLibrary)
        {
            if (textureData.second.assetFile)
                textureData.second.texture->loadFromAssetFile(textureData.second.assetFile);
        }
    }

    //////////////////////////////////////////
    void TextureManager::unloadAssetTextures(
        Set<String> const& _tags,
        Vector<Texture2DPtr>* _unloadedTextures2D)
    {
        MAZE_PROFILE_EVENT("TextureManager::unloadAssetTextures");

        {
            StringKeyMap<Texture2DLibraryData>::iterator it = m_textures2DLibrary.begin();
            StringKeyMap<Texture2DLibraryData>::iterator end = m_textures2DLibrary.end();
            for (; it != end; )
            {
                if (it->second.assetFile && it->second.assetFile->hasAnyOfTags(_tags))
                {
                    if (_unloadedTextures2D)
                        _unloadedTextures2D->push_back(it->second.texture);

                    it = m_textures2DLibrary.erase(it);
                    end = m_textures2DLibrary.end();
                }
                else
                {
                    ++it;
                }
            }
        }
        {
            StringKeyMap<TextureCubeLibraryData>::iterator it = m_texturesCubeLibrary.begin();
            StringKeyMap<TextureCubeLibraryData>::iterator end = m_texturesCubeLibrary.end();
            for (; it != end; )
            {
                if (it->second.assetFile && it->second.assetFile->hasAnyOfTags(_tags))
                {
                    it = m_texturesCubeLibrary.erase(it);
                    end = m_texturesCubeLibrary.end();
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    //////////////////////////////////////////
    Vector<String> TextureManager::getTextureLoaderExtensions()
    {
        Vector<String> result;
        for (auto const& textureLoaderData : m_textureLoaders)
            result.push_back(textureLoaderData.first);
        
        return result;
    }

    //////////////////////////////////////////
    String const& TextureManager::getTextureName(Texture2D const* _texture)
    {
        static String nullPointer;

        for (auto const& textureData : m_textures2DLibrary)
        {
            if (textureData.second.texture.get() == _texture)
                return textureData.first;
        }

        return nullPointer;
    }
    
} // namespace Maze
//////////////////////////////////////////
