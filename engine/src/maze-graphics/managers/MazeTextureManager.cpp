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
#include "maze-core/managers/MazeAssetUnitManager.hpp"
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
#include "maze-graphics/assets/MazeAssetUnitTexture2D.hpp"
#include "maze-graphics/assets/MazeAssetUnitTextureCube.hpp"


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
        static TextureManagerPtr nullPointer;

        RenderSystem* renderSystem = RenderSystem::GetCurrentInstancePtr();
        if (!renderSystem)
            return nullPointer;

        return renderSystem->getTextureManager();
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


        if (AssetUnitManager::GetInstancePtr())
        {
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitTexture2D::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitTexture2D::Create(_file, _data);
                });
            AssetUnitManager::GetInstancePtr()->registerAssetUnitProcessor(
                AssetUnitTextureCube::GetDataBlockId(),
                [](AssetFilePtr const& _file, DataBlock const& _data)
                {
                    return AssetUnitTextureCube::Create(_file, _data);
                });

            AssetUnitManager::GetInstancePtr()->eventAssetUnitAdded.subscribe(
                [](AssetUnitPtr const& _assetUnit)
                {
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitTexture2D>::UID())
                        _assetUnit->castRaw<AssetUnitTexture2D>()->initTexture();
                    else
                    if (_assetUnit->getClassUID() == ClassInfo<AssetUnitTextureCube>::UID())
                        _assetUnit->castRaw<AssetUnitTextureCube>()->initTexture();
                });
        }

        if (AssetManager::GetInstancePtr())
        {
            AssetManager::GetInstancePtr()->eventAssetFileAdded.subscribe(
                [](AssetFilePtr const& _assetFile, HashedString const& _extension)
            {
                if (TextureManager::GetCurrentInstancePtr()->hasTextureLoader(_extension))
                {
                    if (!_assetFile->getAssetUnit<AssetUnitTexture2D>())
                        _assetFile->addAssetUnit(AssetUnitTexture2D::Create(_assetFile));
                }
                else
                if (_extension == MAZE_HCS("mzcubemap"))
                {
                    if (!_assetFile->getAssetUnit<AssetUnitTextureCube>())
                        _assetFile->addAssetUnit(AssetUnitTextureCube::Create(_assetFile));
                }
            });
        }

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
    Texture2DPtr const& TextureManager::getOrLoadTexture2D(
        HashedCString _textureName,
        bool _syncLoad)
    {
        static Texture2DPtr const nullPointer;

        Texture2DLibraryData const* libraryData = getTexture2DLibraryData(_textureName);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->texture;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);
        if (!assetFile)
            return nullPointer;

        return getOrLoadTexture2D(assetFile, _syncLoad);
    }

    //////////////////////////////////////////
    Texture2DPtr const& TextureManager::getOrLoadTexture2D(
        AssetFilePtr const& _assetFile,
        bool _syncLoad)
    {
        static Texture2DPtr const nullPointer;

        Texture2DLibraryData const* libraryData = getTexture2DLibraryData(_assetFile->getFileName());
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->texture;
        }

        Texture2DPtr texture2D = Texture2D::Create(_assetFile, m_renderSystemRaw);
        texture2D->setName(_assetFile->getFileName());

        loadTextureMetaData(_assetFile, texture2D);

        Texture2DLibraryData* data = addTextureToLibrary(texture2D);
        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)_assetFile,
                    texture2DWeak = (Texture2DWPtr)texture2D
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    Texture2DPtr texture = texture2DWeak.lock();
                    if (assetFile && texture)
                        texture->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)_assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            return data->texture;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    void TextureManager::loadTextureMetaData(Texture2DPtr const& _texture, DataBlock const& _metaData)
    {
        if (_metaData.isParamExists(MAZE_HCS("magFilter")))
            _texture->setMagFilter(
                TextureFilter::FromString(
                    _metaData.getString(MAZE_HCS("magFilter"))));

        if (_metaData.isParamExists(MAZE_HCS("minFilter")))
            _texture->setMinFilter(
                TextureFilter::FromString(
                    _metaData.getString(MAZE_HCS("minFilter"))));

        if (_metaData.isParamExists(MAZE_HCS("wrapS")))
            _texture->setWrapS(
                TextureWrap::FromString(
                    _metaData.getString(MAZE_HCS("wrapS"))));

        if (_metaData.isParamExists(MAZE_HCS("wrapT")))
            _texture->setWrapT(
                TextureWrap::FromString(
                    _metaData.getString(MAZE_HCS("wrapT"))));

        if (_metaData.isParamExists(MAZE_HCS("anisotropyLevel")))
            _texture->setAnisotropyLevel(
                _metaData.getF32(MAZE_HCS("anisotropyLevel")));
    }

    //////////////////////////////////////////
    bool TextureManager::loadTextureMetaData(AssetFilePtr const& _assetFile, Texture2DPtr const& _texture)
    {
        if (!_texture)
            return false;

        DataBlock metaData;
        if (AssetManager::GetInstancePtr()->loadMetaData(_assetFile, metaData))
            loadTextureMetaData(_texture, metaData);

        return true;
    }

    //////////////////////////////////////////
    void TextureManager::saveTextureMetaData(Texture2DPtr const& _texture, DataBlock& _metaData)
    {
        _metaData.setString(MAZE_HCS("magFilter"), _texture->getMagFilter().toString());
        _metaData.setString(MAZE_HCS("minFilter"), _texture->getMinFilter().toString());
        _metaData.setString(MAZE_HCS("wrapS"), _texture->getWrapS().toString());
        _metaData.setString(MAZE_HCS("wrapT"), _texture->getWrapT().toString());
        _metaData.setF32(MAZE_HCS("anisotropyLevel"), _texture->getAnisotropyLevel());
    }

    //////////////////////////////////////////
    bool TextureManager::saveTexture2DMetaData(AssetFilePtr const& _assetFile)
    {
        Texture2DPtr const& texture2D = getOrLoadTexture2D(_assetFile);
        if (!texture2D)
            return false;

        DataBlock metaData;
        AssetManager::GetInstancePtr()->loadMetaData(_assetFile, metaData);
        saveTextureMetaData(texture2D, metaData);
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
    Texture2DLibraryData* TextureManager::addTextureToLibrary(
        Texture2DPtr const& _texture,
        TextureLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_textures2DLibrary.insert(
            _texture->getName(),
            { _texture, _callbacks, _info });

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
        AssetManager::GetInstancePtr()->loadMetaData(_assetFile, metaData);

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
    void TextureManager::loadTextureMetaData(TextureCubePtr const& _texture, DataBlock const& _metaData)
    {
        if (_metaData.isParamExists(MAZE_HCS("magFilter")))
            _texture->setMagFilter(
                TextureFilter::FromString(
                    _metaData.getString(MAZE_HCS("magFilter"))));

        if (_metaData.isParamExists(MAZE_HCS("minFilter")))
            _texture->setMinFilter(
                TextureFilter::FromString(
                    _metaData.getString(MAZE_HCS("minFilter"))));

        if (_metaData.isParamExists(MAZE_HCS("wrapS")))
            _texture->setWrapS(
                TextureWrap::FromString(
                    _metaData.getString(MAZE_HCS("wrapS"))));

        if (_metaData.isParamExists(MAZE_HCS("wrapT")))
            _texture->setWrapT(
                TextureWrap::FromString(
                    _metaData.getString(MAZE_HCS("wrapT"))));

        if (_metaData.isParamExists(MAZE_HCS("wrapR")))
            _texture->setWrapR(
                TextureWrap::FromString(
                    _metaData.getString(MAZE_HCS("wrapR"))));
    }

    //////////////////////////////////////////
    bool TextureManager::loadTextureMetaData(AssetFilePtr const& _assetFile, TextureCubePtr const& _texture)
    {
        if (!_texture)
            return false;

        DataBlock metaData;
        if (AssetManager::GetInstancePtr()->loadMetaData(_assetFile, metaData))
            loadTextureMetaData(_texture, metaData);

        return true;
    }

    //////////////////////////////////////////
    void TextureManager::saveTextureMetaData(TextureCubePtr const& _texture, DataBlock& _metaData)
    {
        _metaData.setString(MAZE_HCS("magFilter"), _texture->getMagFilter().toString());
        _metaData.setString(MAZE_HCS("minFilter"), _texture->getMinFilter().toString());
        _metaData.setString(MAZE_HCS("wrapS"), _texture->getWrapS().toString());
        _metaData.setString(MAZE_HCS("wrapT"), _texture->getWrapT().toString());
        _metaData.setString(MAZE_HCS("wrapR"), _texture->getWrapR().toString());
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::getOrLoadTextureCube(
        String const& _textureName,
        bool _syncLoad)
    {
        static TextureCubePtr const nullPointer;

        TextureCubeLibraryData const* libraryData = getTextureCubeLibraryData(_textureName);
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->texture;
        }

        AssetFilePtr const& assetFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(_textureName);
        if (!assetFile)
            return nullPointer;

        return getOrLoadTextureCube(assetFile, _syncLoad);
    }

    //////////////////////////////////////////
    TextureCubePtr const& TextureManager::getOrLoadTextureCube(
        AssetFilePtr const& _assetFile,
        bool _syncLoad)
    {
        static TextureCubePtr const nullPointer;

        TextureCubeLibraryData const* libraryData = getTextureCubeLibraryData(_assetFile->getFileName());
        if (libraryData)
        {
            if (libraryData->callbacks.requestLoad)
                libraryData->callbacks.requestLoad(_syncLoad);

            return libraryData->texture;
        }

        TextureCubePtr textureCube = TextureCube::Create(_assetFile, m_renderSystemRaw);
        textureCube->setName(_assetFile->getFileName());

        DataBlock metaData;
        if (AssetManager::GetInstancePtr()->loadMetaData(_assetFile, metaData))
            loadTextureMetaData(_assetFile, textureCube);

        TextureCubeLibraryData* data = addTextureToLibrary(textureCube);
        if (data)
        {
            data->callbacks.requestReload =
                [
                    assetFileWeak = (AssetFileWPtr)_assetFile,
                    textureCubeWeak = (TextureCubeWPtr)textureCube
                ](bool _immediate)
                {
                    AssetFilePtr assetFile = assetFileWeak.lock();
                    TextureCubePtr texture = textureCubeWeak.lock();
                    if (assetFile && texture)
                        texture->loadFromAssetFile(assetFile);
                };
            data->callbacks.hasAnyOfTags =
                [assetFileWeak = (AssetFileWPtr)_assetFile](Set<String> const& _tags)
                {
                    if (AssetFilePtr assetFile = assetFileWeak.lock())
                        return assetFile->hasAnyOfTags(_tags);

                    return false;
                };

            return data->texture;
        }

        return nullPointer;
    }

    //////////////////////////////////////////
    TextureCubeLibraryData* TextureManager::addTextureToLibrary(
        TextureCubePtr const& _texture,
        TextureLibraryDataCallbacks const& _callbacks,
        DataBlock const& _info)
    {
        auto it2 = m_texturesCubeLibrary.insert(
            _texture->getName(),
            { _texture, _callbacks, _info });

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
            Vector<HashedString> textureExtensions = getTextureLoaderExtensions();
            textureExtensions.push_back(MAZE_HS("mztexture"));

            Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(
                Set<String>(textureExtensions.begin(), textureExtensions.end()));
            for (AssetFilePtr const& assetFile : assetFiles)
            {
                getOrLoadTexture2D(assetFile);
            }
        }

        {
            Vector<AssetFilePtr> assetFiles = AssetManager::GetInstancePtr()->getAssetFilesWithExtensions(Set<Path>{ "mzcubemap" });
            for (AssetFilePtr const& assetFile : assetFiles)
            {
                getOrLoadTextureCube(assetFile);
            }
        }
    }

    //////////////////////////////////////////
    void TextureManager::reloadAllAssetTextures()
    {
        for (auto const& textureData : m_textures2DLibrary)
        {
            if (textureData.second.callbacks.requestReload)
                textureData.second.callbacks.requestReload(true);
        }
    }

    //////////////////////////////////////////
    void TextureManager::unloadAssetTextures(
        Set<String> const& _tags,
        Vector<Texture2DPtr>* _unloadedTextures2D)
    {
        MAZE_PROFILE_EVENT("TextureManager::unloadAssetTextures");

        {
            Vector<std::function<void(bool)>> unloadCallbacks;

            m_textures2DLibrary.iterate(
                [&](HashedCString _name, Texture2DLibraryData const& _data)
                {
                    if (_data.callbacks.hasAnyOfTags && _data.callbacks.hasAnyOfTags(_tags) && _data.callbacks.requestUnload)
                    {
                        if (_unloadedTextures2D)
                            _unloadedTextures2D->push_back(_data.texture);

                        unloadCallbacks.push_back(_data.callbacks.requestUnload);
                    }

                    return true;
                });

            for (std::function<void(bool)> const& unloadCallback : unloadCallbacks)
                unloadCallback(true);
        }
        {
            Vector<std::function<void(bool)>> unloadCallbacks;

            m_texturesCubeLibrary.iterate(
                [&](HashedCString _name, TextureCubeLibraryData const& _data)
                {
                    if (_data.callbacks.hasAnyOfTags && _data.callbacks.hasAnyOfTags(_tags) && _data.callbacks.requestUnload)
                        unloadCallbacks.push_back(_data.callbacks.requestUnload);

                    return true;
                });

            for (std::function<void(bool)> const& unloadCallback : unloadCallbacks)
                unloadCallback(true);
        }

    }

    //////////////////////////////////////////
    Vector<HashedString> TextureManager::getTextureLoaderExtensions()
    {
        Vector<HashedString> result;
        for (StringKeyMap<TextureLoaderData>::iterator it = m_textureLoaders.begin(),
                                                       end = m_textureLoaders.end();
                                                       it != end;
                                                       ++it)
            result.push_back(HashedString(it.key()));
        
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
