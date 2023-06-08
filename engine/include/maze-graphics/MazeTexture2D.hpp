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
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2D, Texture);

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
        static Texture2DPtr Create(
            U32 _width,
            U32 _height,
            PixelFormat::Enum _pixelFormat,
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
        bool loadFromBuffer(
            ByteBufferPtr const& _buffer,
            PixelFormat::Enum _bufferPixelFormat,
            Vec2DU const& _size,
            PixelFormat::Enum _pixelFormat);

        //////////////////////////////////////////
        bool loadFromBuffers(
            Vector<ByteBufferPtr> const& _buffers,
            PixelFormat::Enum _bufferPixelFormat,
            Vec2DU const& _size,
            PixelFormat::Enum _pixelFormat);



        //////////////////////////////////////////
        inline AssetFilePtr const& getAssetFile() const { return m_assetFile; }

        //////////////////////////////////////////
        Path const& getAssetFileName() const; // #TODO: REMOVE THIS METHOD!

        //////////////////////////////////////////
        virtual bool loadTexture(
            Vector<PixelSheet2D> const& _pixelSheets,
            PixelFormat::Enum _internalPixelFormat = PixelFormat::None) MAZE_ABSTRACT;

        //////////////////////////////////////////
        bool loadTexture(
            PixelSheet2D const& _pixelSheet,
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
        inline void setMagMinFilters(TextureFilter _mag, TextureFilter _min)
        {
            setMagFilter(_mag);
            setMinFilter(_min);
        }


        //////////////////////////////////////////
        virtual bool setWrapS(TextureWrap _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline TextureWrap getWrapS() const { return m_wrapS; }

        //////////////////////////////////////////
        virtual bool setWrapT(TextureWrap _value) MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline TextureWrap getWrapT() const { return m_wrapT; }

        //////////////////////////////////////////
        PixelFormat::Enum getInternalPixelFormat() const { return m_internalPixelFormat; }

        //////////////////////////////////////////
        virtual void copyImageFrom(
            Texture2DPtr const& _texture,
            U32 _x = 0,
            U32 _y = 0) MAZE_ABSTRACT;

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
        virtual void saveToFileAsTGA(
            String const& _fileName,
            Vec2DU _size = Vec2DU::c_zero,
            bool _resetAlpha = false) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual PixelSheet2D readAsPixelSheet(PixelFormat::Enum _outputFormat = PixelFormat::None) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual void generateMipmaps() MAZE_ABSTRACT;


    public:

        //////////////////////////////////////////
        virtual String toString() const;

        //////////////////////////////////////////
        static void FromString(Texture2DPtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(Texture2D const* _value, String& _data);

    protected:

        //////////////////////////////////////////
        Texture2D();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;
    
    protected:
        Vec2DS m_size;

        TextureFilter m_magFilter;
        TextureFilter m_minFilter;
        TextureWrap m_wrapS;
        TextureWrap m_wrapT;

        PixelFormat::Enum m_internalPixelFormat;

        AssetFilePtr m_assetFile; // #TODO: REMOVE THIS!
    };

    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(Texture2D);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DPtr>::value), void>::type
        ValueToString(Texture2DPtr const& _value, String& _data)
    {
        if (!_value)
        {
            _data.clear();
            return;
        }

        Texture2D::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DPtr>::value), void>::type
        ValueFromString(Texture2DPtr& _value, CString _data, Size _count)
    {
         Texture2D::FromString(_value, _data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DPtr>::value), U32>::type
        GetValueSerializationSize(Texture2DPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DPtr>::value), void>::type
        SerializeValue(Texture2DPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DPtr>::value), void>::type
        DeserializeValue(Texture2DPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2D_hpp_
//////////////////////////////////////////
