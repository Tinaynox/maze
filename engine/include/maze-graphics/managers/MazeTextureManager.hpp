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
        Texture2DPtr const& getWhiteTexture() const { return m_whiteTexture; }

        //////////////////////////////////////////
        Texture2DPtr const& getBlackTexture() const { return m_blackTexture; }

        //////////////////////////////////////////
        Texture2DPtr const& getErrorTexture() const { return m_errorTexture; }


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
        TextureCubePtr const& getWhiteCubeTexture() const { return m_whiteCubeTexture; }

        //////////////////////////////////////////
        TextureCubePtr const& getTestCubeTexture() const { return m_testCubeTexture; }

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
        }

        //////////////////////////////////////////
        void clearTextureLoader(HashedCString _extension)
        {
            m_textureLoaders.erase(_extension);
        }

    protected:

        //////////////////////////////////////////
        TextureManager();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        void notifyRenderSystemInited();

        //////////////////////////////////////////
        void createSpecialTextures();
    
    protected:
        RenderSystemWPtr m_renderSystem;
        RenderSystem* m_renderSystemRaw;

        StringKeyMap<TextureLoaderData> m_textureLoaders;

        StringKeyMap<Texture2DPtr> m_textures2DByName;
        StringKeyMap<TextureCubePtr> m_texturesCubeByName;

        Texture2DPtr m_whiteTexture;
        Texture2DPtr m_blackTexture;
        Texture2DPtr m_errorTexture;

        TextureCubePtr m_whiteCubeTexture;
        TextureCubePtr m_testCubeTexture;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextureManager_hpp_
//////////////////////////////////////////
