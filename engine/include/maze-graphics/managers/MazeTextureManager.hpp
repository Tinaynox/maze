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
    MAZE_USING_SHARED_PTR(AssetFile);
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(Texture3D);
    MAZE_USING_SHARED_PTR(TextureCube);


    //////////////////////////////////////////
    // Enum BuiltinTexture2DType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_3_API(MAZE_GRAPHICS_API, BuiltinTexture2DType,
        White,
        Black,
        Error);


    //////////////////////////////////////////
    // Enum BuiltinTextureCubeType
    //
    //////////////////////////////////////////
    MAZE_DECLARE_ENUMCLASS_2_API(MAZE_GRAPHICS_API, BuiltinTextureCubeType,
        White,
        Test);


    //////////////////////////////////////////
    using LoadTextureAssetFileFunction = bool(*)(AssetFilePtr const& _file, Vector<PixelSheet2D>& _pixelSheets);
    using LoadTextureByteBufferFunction = bool(*)(ByteBufferPtr const& _fileData, Vector<PixelSheet2D>& _pixelSheets);
    using IsTextureAssetFileFunction = bool(*)(AssetFilePtr const& _file);
    using IsTextureByteBufferFunction = bool(*)(ByteBufferPtr const& _fileData);


    //////////////////////////////////////////
    // Struct TextureLoaderData
    //
    //////////////////////////////////////////
    struct TextureLoaderData
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

        LoadTextureAssetFileFunction loadTextureAssetFileFunc;
        LoadTextureByteBufferFunction loadTextureByteBufferFunc;
        IsTextureAssetFileFunction isTextureAssetFileFunc;
        IsTextureByteBufferFunction isTextureByteBufferFunc;
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
        Texture2DPtr const& getTexture2D(HashedCString _assetFileName);

        //////////////////////////////////////////
        inline Texture2DPtr const& getTexture2D(String const& _assetFileName) { return getTexture2D(MAZE_HASHED_CSTRING(_assetFileName.c_str())); }

        //////////////////////////////////////////
        inline Texture2DPtr const& getTexture2D(CString _assetFileName) { return getTexture2D(MAZE_HASHED_CSTRING(_assetFileName)); }

        //////////////////////////////////////////
        Texture2DPtr const& getTexture2D(AssetFilePtr const& _assetFile);


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
        Texture2DPtr const& addTexture(Texture2DPtr const& _texture);

        //////////////////////////////////////////
        Vector<Texture2DPtr> getTextures2DSorted();


        //////////////////////////////////////////
        Vector<PixelSheet2D> loadPixelSheets2D(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        Vector<PixelSheet2D> loadPixelSheets2D(String const& _assetFileName);



        //////////////////////////////////////////
        TextureCubePtr const& getTextureCube(String const& _assetFileName);

        //////////////////////////////////////////
        TextureCubePtr const& getTextureCube(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        TextureCubePtr const& getWhiteCubeTexture() const { return getBuiltinTextureCube(BuiltinTextureCubeType::White); }

        //////////////////////////////////////////
        TextureCubePtr const& getTestCubeTexture() const { return getBuiltinTextureCube(BuiltinTextureCubeType::Test); }


        //////////////////////////////////////////
        TextureCubePtr const& addTexture(TextureCubePtr const& _texture);


        //////////////////////////////////////////
        void loadAllAssetTextures();

        //////////////////////////////////////////
        void reloadAllAssetTextures();


        //////////////////////////////////////////
        void registerTextureLoader(
            HashedCString _extension,
            TextureLoaderData const& _data)
        {
            m_textureLoaders.insert(_extension, _data);
            eventTextureLoaderAdded(_extension, _data);
        }

        //////////////////////////////////////////
        void clearTextureLoader(HashedCString _extension)
        {
            m_textureLoaders.erase(_extension);
        }

        //////////////////////////////////////////
        Vector<String> getTextureLoaderExtensions();


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

        StringKeyMap<Texture2DPtr> m_textures2DByName;
        StringKeyMap<TextureCubePtr> m_texturesCubeByName;

        Texture2DPtr m_builtinTexture2Ds[BuiltinTexture2DType::MAX];
        TextureCubePtr m_builtinTextureCubes[BuiltinTexture2DType::MAX];
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextureManager_hpp_
//////////////////////////////////////////
