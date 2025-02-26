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
#pragma once
#if (!defined(_MazeTextureManager_hpp_))
#define _MazeTextureManager_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeStringKeyMap.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/MazeSystemFont.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TextureManager);
    MAZE_USING_SHARED_PTR(RenderSystem);
    MAZE_USING_MANAGED_SHARED_PTR(AssetFile);
    MAZE_USING_MANAGED_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(Texture3D);
    MAZE_USING_MANAGED_SHARED_PTR(TextureCube);


    //////////////////////////////////////////
    // Enum BuiltinTexture2DType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_4_API(MAZE_GRAPHICS_API, BuiltinTexture2DType,
        White,
        Black,
        Normal,
        Error);


    //////////////////////////////////////////
    // Enum BuiltinTextureCubeType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2_API(MAZE_GRAPHICS_API, BuiltinTextureCubeType,
        White,
        Test);


    //////////////////////////////////////////
    using LoadTextureAssetFileFunction = bool(*)(AssetFile const& _file, Vector<PixelSheet2D>& _pixelSheets);
    using LoadTextureByteBufferFunction = bool(*)(ByteBuffer const& _fileData, Vector<PixelSheet2D>& _pixelSheets);
    using IsTextureAssetFileFunction = bool(*)(AssetFile const& _file);
    using IsTextureByteBufferFunction = bool(*)(ByteBuffer const& _fileData);


    //////////////////////////////////////////
    // Struct TextureLoaderData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API TextureLoaderData
    {
        //////////////////////////////////////////
        TextureLoaderData() = default;

        //////////////////////////////////////////
        TextureLoaderData(
            LoadTextureAssetFileFunction _loadTextureAssetFileFunc,
            LoadTextureByteBufferFunction _loadTextureByteBufferFunc,
            IsTextureAssetFileFunction _isTextureAssetFileFunc,
            IsTextureByteBufferFunction _isTextureByteBufferFunc)
            : loadTextureAssetFileFunc(_loadTextureAssetFileFunc)
            , loadTextureByteBufferFunc(_loadTextureByteBufferFunc)
            , isTextureAssetFileFunc(_isTextureAssetFileFunc)
            , isTextureByteBufferFunc(_isTextureByteBufferFunc)
        {}

        LoadTextureAssetFileFunction loadTextureAssetFileFunc = nullptr;
        LoadTextureByteBufferFunction loadTextureByteBufferFunc = nullptr;
        IsTextureAssetFileFunction isTextureAssetFileFunc = nullptr;
        IsTextureByteBufferFunction isTextureByteBufferFunc = nullptr;
    };


    //////////////////////////////////////////
    // Struct TextureLibraryDataCallbacks
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API TextureLibraryDataCallbacks
    {
        std::function<void(bool)> requestLoad;
        std::function<void(bool)> requestUnload;
        std::function<void(bool)> requestReload;
        std::function<bool(Set<String> const&)> hasAnyOfTags;
    };


    //////////////////////////////////////////
    // Struct Texture2DLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API Texture2DLibraryData
    {
        //////////////////////////////////////////
        Texture2DLibraryData(
            Texture2DPtr const& _texture = nullptr,
            TextureLibraryDataCallbacks const& _callbacks = TextureLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : texture(_texture)
            , callbacks(_callbacks)
            , data(_data)
        {}

        Texture2DPtr texture;
        TextureLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Struct TextureCubeLibraryData
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API TextureCubeLibraryData
    {
        //////////////////////////////////////////
        TextureCubeLibraryData(
            TextureCubePtr const& _texture = nullptr,
            TextureLibraryDataCallbacks const& _callbacks = TextureLibraryDataCallbacks(),
            DataBlock const& _data = DataBlock::c_empty)
            : texture(_texture)
            , callbacks(_callbacks)
            , data(_data)
        {}

        TextureCubePtr texture;
        TextureLibraryDataCallbacks callbacks;
        DataBlock data;
    };


    //////////////////////////////////////////
    // Class TextureManager
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TextureManager 
        : public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~TextureManager();

        //////////////////////////////////////////
        static void Initialize(TextureManagerPtr& _textureManager, RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static TextureManagerPtr const& GetCurrentInstancePtr();


        //////////////////////////////////////////
        inline StringKeyMap<Texture2DLibraryData> const& getTextures2DLibrary() const { return m_textures2DLibrary; }


        //////////////////////////////////////////
        Texture2DLibraryData const* getTexture2DLibraryData(HashedCString _textureName);

        //////////////////////////////////////////
        Texture2DLibraryData const* getTexture2DLibraryData(String const& _assetFileName) { return getTexture2DLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        Texture2DLibraryData const* getTexture2DLibraryData(CString _assetFileName) { return getTexture2DLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        Texture2DPtr const& getOrLoadTexture2D(HashedCString _assetFileName, bool _syncLoad = true);

        //////////////////////////////////////////
        inline Texture2DPtr const& getOrLoadTexture2D(String const& _assetFileName, bool _syncLoad = true) { return getOrLoadTexture2D(MAZE_HASHED_CSTRING(_assetFileName.c_str()), _syncLoad); }

        //////////////////////////////////////////
        inline Texture2DPtr const& getOrLoadTexture2D(CString _assetFileName, bool _syncLoad = true) { return getOrLoadTexture2D(MAZE_HASHED_CSTRING(_assetFileName), _syncLoad); }

        //////////////////////////////////////////
        Texture2DPtr const& getOrLoadTexture2D(AssetFilePtr const& _assetFile, bool _syncLoad = true);



        //////////////////////////////////////////
        void loadTextureMetaData(Texture2DPtr const& _texture, DataBlock const& _metaData);

        //////////////////////////////////////////
        // bool loadTextureMetaData(AssetFilePtr const& _assetFile, Texture2DPtr const& _texture);

        //////////////////////////////////////////
        void saveTextureMetaData(Texture2DPtr const& _texture, DataBlock& _metaData);

        //////////////////////////////////////////
        // bool saveTexture2DMetaData(AssetFilePtr const& _assetFile);


        //////////////////////////////////////////
        inline Texture2DPtr const& getBuiltinTexture2D(BuiltinTexture2DType _texture2DType) const { return m_builtinTexture2Ds[_texture2DType]; }

        //////////////////////////////////////////
        Texture2DPtr const& createBuiltinTexture2D(BuiltinTexture2DType _texture2DType);

        //////////////////////////////////////////
        Texture2DPtr const& ensureBuiltinTexture2D(BuiltinTexture2DType _texture2DType);

        //////////////////////////////////////////
        inline TextureCubePtr const& getBuiltinTextureCube(BuiltinTextureCubeType _textureCubeType) const { return m_builtinTextureCubes[_textureCubeType]; }

        //////////////////////////////////////////
        TextureCubePtr const& createBuiltinTextureCube(BuiltinTextureCubeType _textureCubeType);

        //////////////////////////////////////////
        TextureCubePtr const& ensureBuiltinTextureCube(BuiltinTextureCubeType _textureCubeType);

        //////////////////////////////////////////
        void createBuiltinTextures();


        //////////////////////////////////////////
        Texture2DPtr const& getWhiteTexture() const { return getBuiltinTexture2D(BuiltinTexture2DType::White); }

        //////////////////////////////////////////
        Texture2DPtr const& getBlackTexture() const { return getBuiltinTexture2D(BuiltinTexture2DType::Black); }

        //////////////////////////////////////////
        Texture2DPtr const& getErrorTexture() const { return getBuiltinTexture2D(BuiltinTexture2DType::Error); }


        //////////////////////////////////////////
        Texture2DLibraryData* addTextureToLibrary(
            Texture2DPtr const& _texture,
            TextureLibraryDataCallbacks const& _callbacks = TextureLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        void removeTexture2DFromLibrary(HashedCString _textureName);

        //////////////////////////////////////////
        inline void removeTexture2DFromLibrary(CString _textureName) { removeTexture2DFromLibrary(HashedCString(_textureName)); }

        //////////////////////////////////////////
        inline void removeTexture2DFromLibrary(String const& _textureName) { removeTexture2DFromLibrary(HashedCString(_textureName.c_str())); }


        //////////////////////////////////////////
        Vector<Texture2DPtr> getTextures2DSorted();


        //////////////////////////////////////////
        Vector<PixelSheet2D> loadPixelSheets2D(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        Vector<PixelSheet2D> loadPixelSheets2D(String const& _assetFileName);



        //////////////////////////////////////////
        inline StringKeyMap<TextureCubeLibraryData> const& getTexturesCubeLibrary() const { return m_texturesCubeLibrary; }


        //////////////////////////////////////////
        void loadTextureMetaData(TextureCubePtr const& _texture, DataBlock const& _metaData);

        //////////////////////////////////////////
        // bool loadTextureMetaData(AssetFilePtr const& _assetFile, TextureCubePtr const& _texture);

        //////////////////////////////////////////
        void saveTextureMetaData(TextureCubePtr const& _texture, DataBlock& _metaData);


        //////////////////////////////////////////
        TextureCubeLibraryData const* getTextureCubeLibraryData(HashedCString _textureName);

        //////////////////////////////////////////
        TextureCubeLibraryData const* getTextureCubeLibraryData(String const& _assetFileName) { return getTextureCubeLibraryData(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        TextureCubeLibraryData const* getTextureCubeLibraryData(CString _assetFileName) { return getTextureCubeLibraryData(MAZE_HASHED_CSTRING(_assetFileName)); }


        //////////////////////////////////////////
        TextureCubePtr const& getOrLoadTextureCube(String const& _assetFileName, bool _syncLoad = true);

        //////////////////////////////////////////
        TextureCubePtr const& getOrLoadTextureCube(AssetFilePtr const& _assetFile, bool _syncLoad = true);


        //////////////////////////////////////////
        TextureCubePtr const& getWhiteCubeTexture() const { return getBuiltinTextureCube(BuiltinTextureCubeType::White); }

        //////////////////////////////////////////
        TextureCubePtr const& getTestCubeTexture() const { return getBuiltinTextureCube(BuiltinTextureCubeType::Test); }


        //////////////////////////////////////////
        TextureCubeLibraryData* addTextureToLibrary(
            TextureCubePtr const& _texture,
            TextureLibraryDataCallbacks const& _callbacks = TextureLibraryDataCallbacks(),
            DataBlock const& _info = DataBlock::c_empty);

        //////////////////////////////////////////
        void removeTextureCubeFromLibrary(HashedCString _textureName);

        //////////////////////////////////////////
        inline void removeTextureCubeFromLibrary(CString _textureName) { removeTextureCubeFromLibrary(HashedCString(_textureName)); }

        //////////////////////////////////////////
        inline void removeTextureCubeFromLibrary(String const& _textureName) { removeTextureCubeFromLibrary(HashedCString(_textureName.c_str())); }


        //////////////////////////////////////////
        void loadAllAssetTextures();

        //////////////////////////////////////////
        void reloadAllAssetTextures();

        //////////////////////////////////////////
        void unloadAssetTextures(
            Set<String> const& _tags,
            Vector<Texture2DPtr>* _unloadedTextures2D);


        //////////////////////////////////////////
        inline void registerTextureLoader(
            HashedCString _extension,
            TextureLoaderData const& _data)
        {
            m_textureLoaders.insert(_extension, _data);
            eventTextureLoaderAdded(_extension, _data);
        }

        //////////////////////////////////////////
        inline void clearTextureLoader(HashedCString _extension)
        {
            m_textureLoaders.erase(_extension);
        }

        //////////////////////////////////////////
        Vector<HashedString> getTextureLoaderExtensions();

        //////////////////////////////////////////
        inline bool hasTextureLoader(HashedCString _extension)
        {
            return m_textureLoaders.find(_extension) != m_textureLoaders.end();
        }


        //////////////////////////////////////////
        String const& getTextureName(Texture2D const* _texture);

    public:

        //////////////////////////////////////////
        MultiDelegate<HashedCString, TextureLoaderData const&> eventTextureLoaderAdded;

    protected:

        //////////////////////////////////////////
        TextureManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void notifyRenderSystemInited();
    
    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<TextureLoaderData> m_textureLoaders;

        StringKeyMap<Texture2DLibraryData> m_textures2DLibrary;
        StringKeyMap<TextureCubeLibraryData> m_texturesCubeLibrary;

        Texture2DPtr m_builtinTexture2Ds[BuiltinTexture2DType::MAX];
        TextureCubePtr m_builtinTextureCubes[BuiltinTexture2DType::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextureManager_hpp_
//////////////////////////////////////////
