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
#if (!defined(_MazeTexture2D_hpp_))
#define _MazeTexture2D_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeTexture.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
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
    MAZE_USING_SHARED_PTR(Texture2D);
    MAZE_USING_SHARED_PTR(AssetFile);
    

    //////////////////////////////////////////
    // Class Texture2D
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Texture2D
        : public Texture
    {
    public:

        //////////////////////////////////////////
        virtual ~Texture2D();

        //////////////////////////////////////////
        static Texture2DPtr Create(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        static Texture2DPtr Create(
            AssetFilePtr const& _assetFile,
            RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        static Texture2DPtr Create(
            String const& _assetFileName,
            RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_ABSTRACT;

        //////////////////////////////////////////
        void loadFromAssetFile(AssetFilePtr const& _assetFile);

        //////////////////////////////////////////
        void loadFromAssetFile(String const& _assetFileName);

        //////////////////////////////////////////
        bool loadEmpty(Vec2DU const& _size, PixelFormat::Enum _pixelFormat);

        //////////////////////////////////////////
        inline bool loadEmpty(U32 _w, U32 _h, PixelFormat::Enum _pixelFormat)
        {
            return loadEmpty(Vec2DU{_w, _h}, _pixelFormat);
        }

        //////////////////////////////////////////
        inline AssetFilePtr const& getAssetFile() const { return m_assetFile; }

        //////////////////////////////////////////
        String const& getAssetFileName() const;

        //////////////////////////////////////////
        virtual bool loadTexture(
            Vector<PixelSheet2D> const& _pixelSheets,
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None) MAZE_ABSTRACT;

        //////////////////////////////////////////
        bool loadTexture(
            PixelSheet2D const& _pixelSheet,
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None);


        ////////////////////////////////////
        virtual bool setMagFilter(TextureFilter _value) MAZE_ABSTRACT;

        ////////////////////////////////////
        inline TextureFilter getMagFilter() const { return m_magFilter; }

        ////////////////////////////////////
        virtual bool setMinFilter(TextureFilter _value) MAZE_ABSTRACT;

        ////////////////////////////////////
        inline TextureFilter getMinFilter() const { return m_minFilter; }

        ////////////////////////////////////
        virtual bool setWrapS(TextureWrap _value) MAZE_ABSTRACT;

        ////////////////////////////////////
        inline TextureWrap getWrapS() const { return m_wrapS; }

        ////////////////////////////////////
        virtual bool setWrapT(TextureWrap _value) MAZE_ABSTRACT;

        ////////////////////////////////////
        inline TextureWrap getWrapT() const { return m_wrapT; }

        ////////////////////////////////////
        PixelFormat::Enum getInternalPixelFormat() const { return m_internalPixelFormat; }

        //////////////////////////////////////////
        virtual void copyImageFrom(
            U8 const* _pixels,
            PixelFormat::Enum _pixelFormat,
            U32 _width,
            U32 _height,
            U32 _x,
            U32 _y) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline Vec2DS const& getSize() const { return m_size; }

        //////////////////////////////////////////
        inline S32 getWidth() const { return m_size.x; }

        //////////////////////////////////////////
        inline S32 getHeight() const { return m_size.y; }


        //////////////////////////////////////////
        virtual void reload();


        //////////////////////////////////////////
        virtual void saveToFileAsTGA(String const& _fileName, Vec2DU _size = Vec2DU::c_zero) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void generateMipmaps() MAZE_ABSTRACT;

    protected:

        //////////////////////////////////////////
        Texture2D();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem);
    
    protected:
        Vec2DS m_size;

        TextureFilter m_magFilter;
        TextureFilter m_minFilter;
        TextureWrap m_wrapS;
        TextureWrap m_wrapT;

        PixelFormat::Enum m_internalPixelFormat;

        AssetFilePtr m_assetFile;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2D_hpp_
//////////////////////////////////////////
