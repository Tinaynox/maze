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
#if (!defined(_MazeTextureCube_hpp_))
#define _MazeTextureCube_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeTexture.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(TextureCube);
    

    //////////////////////////////////////////
    // Class TextureCube
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TextureCube
        : public Texture
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(TextureCube, Texture);

    public:

        //////////////////////////////////////////
        virtual ~TextureCube();

        //////////////////////////////////////////
        static TextureCubePtr Create(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        static TextureCubePtr Create(
            AssetFilePtr const& _assetFile,
            RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        static TextureCubePtr Create(
            String const& _assetFileName,
            RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_ABSTRACT;

        //////////////////////////////////////////
        void loadFromAssetFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void loadFromAssetFile(String const& _assetFileName);


        //////////////////////////////////////////
        Path const& getAssetFileName() const;


        //////////////////////////////////////////
        virtual bool loadTexture(
            Vector<PixelSheet2D> const _pixelSheets[6],
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None) MAZE_ABSTRACT;

        //////////////////////////////////////////
        bool loadTexture(
            PixelSheet2D _pixelSheet[6],
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None);

        //////////////////////////////////////////
        bool loadTexture(
            String const& _rightName,
            String const& _leftName,
            String const& _topName,
            String const& _bottomName,
            String const& _frontName,
            String const& _backName,
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None);


        //////////////////////////////////////////
        virtual bool setMagFilter(TextureFilter _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline TextureFilter getMagFilter() const { return m_magFilter; }

        //////////////////////////////////////////
        virtual bool setMinFilter(TextureFilter _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline TextureFilter getMinFilter() const { return m_minFilter; }

        //////////////////////////////////////////
        virtual bool setWrapS(TextureWrap _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline TextureWrap getWrapS() const { return m_wrapS; }

        //////////////////////////////////////////
        virtual bool setWrapT(TextureWrap _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline TextureWrap getWrapT() const { return m_wrapT; }

        //////////////////////////////////////////
        virtual bool setWrapR(TextureWrap _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline TextureWrap getWrapR() const { return m_wrapR; }



        //////////////////////////////////////////
        inline Vec2S const& getSize() const { return m_size; }

        //////////////////////////////////////////
        inline S32 getWidth() const { return m_size.x; }

        //////////////////////////////////////////
        inline S32 getHeight() const { return m_size.y; }


        //////////////////////////////////////////
        virtual void generateMipmaps() MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void reload() MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        TextureCube();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;
    

    protected:
        Vec2S m_size = Vec2S::c_zero;

        TextureFilter m_magFilter = TextureFilter::Linear;
        TextureFilter m_minFilter = TextureFilter::Linear;
        TextureWrap m_wrapS = TextureWrap::ClampToEdge;
        TextureWrap m_wrapT = TextureWrap::ClampToEdge;
        TextureWrap m_wrapR = TextureWrap::ClampToEdge;

        PixelFormat::Enum m_internalPixelFormat = PixelFormat::None;

        AssetFilePtr m_assetFile;
    };


    //////////////////////////////////////////
    // Class TextureCubeAssetRef
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API TextureCubeAssetRef
        : public IDataBlockSerializable
    {
    public:

        //////////////////////////////////////////
        TextureCubeAssetRef(TextureCubePtr const& _value = nullptr)
            : m_textureCube(_value)
        {}

        //////////////////////////////////////////
        void setTextureCube(TextureCubePtr const& _value) { m_textureCube = _value; }

        //////////////////////////////////////////
        inline TextureCubePtr const& getTextureCube() const { return m_textureCube; }


        //////////////////////////////////////////
        inline bool operator==(TextureCubeAssetRef const& _value) const { return m_textureCube == _value.getTextureCube(); }

        //////////////////////////////////////////
        inline bool operator!=(TextureCubeAssetRef const& _value) const { return m_textureCube != _value.getTextureCube(); }

    public:
        //////////////////////////////////////////
        virtual bool loadFromDataBlock(DataBlock const& _dataBlock) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void toDataBlock(DataBlock& _dataBlock) const MAZE_OVERRIDE;

    private:
        TextureCubePtr m_textureCube;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTextureCube_hpp_
//////////////////////////////////////////
