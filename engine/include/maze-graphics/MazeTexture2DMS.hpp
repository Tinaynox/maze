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
#if (!defined(_MazeTexture2DMS_hpp_))
#define _MazeTexture2DMS_hpp_


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
    MAZE_USING_SHARED_PTR(Texture2DMS);
    MAZE_USING_SHARED_PTR(AssetFile);
    

    //////////////////////////////////////////
    // Class Texture2DMS
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Texture2DMS
        : public Texture
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2DMS, Texture);

    public:

        //////////////////////////////////////////
        virtual ~Texture2DMS();

        //////////////////////////////////////////
        static Texture2DMSPtr Create(RenderSystem* _renderSystem = nullptr);

        //////////////////////////////////////////
        static Texture2DMSPtr Create(
            Vec2U const& _size,
            PixelFormat::Enum _internalPixelFormat,
            S32 _samples,
            RenderSystem* _renderSystem = nullptr);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_ABSTRACT;

        //////////////////////////////////////////
        inline AssetFilePtr const& getAssetFile() const { return m_assetFile; }

        //////////////////////////////////////////
        Path const& getAssetFileName() const;

        //////////////////////////////////////////
        virtual bool loadEmpty(
            Vec2U const& _size,
            PixelFormat::Enum _internalPixelFormat,
            S32 _samples) MAZE_ABSTRACT;


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
        inline Vec2S const& getSize() const { return m_size; }

        //////////////////////////////////////////
        inline S32 getWidth() const { return m_size.x; }

        //////////////////////////////////////////
        inline S32 getHeight() const { return m_size.y; }


        //////////////////////////////////////////
        inline S32 getSamples() const { return m_samples; }


        //////////////////////////////////////////
        virtual void reload();


        //////////////////////////////////////////
        virtual void saveToFileAsTGA(String const& _fileName, Vec2U _size = Vec2U::c_zero) MAZE_ABSTRACT;

        //////////////////////////////////////////
        virtual PixelSheet2D readAsPixelSheet(PixelFormat::Enum _outputFormat = PixelFormat::None) MAZE_ABSTRACT;


    public:

        //////////////////////////////////////////
        virtual String toString() const;

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count);

        //////////////////////////////////////////
        static Texture2DMSPtr const& FromString(CString _data, Size _count);

    protected:

        //////////////////////////////////////////
        Texture2DMS();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;
    
    protected:
        Vec2S m_size;
        S32 m_samples = 0;

        PixelFormat::Enum m_internalPixelFormat;

        AssetFilePtr m_assetFile;
    };

    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(Texture2DMS);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DMSPtr>::value), void>::type
        ValueToString(Texture2DMSPtr const& _value, String& _data)
    {
        _data = _value->toString();
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DMSPtr>::value), void>::type
        ValueFromString(Texture2DMSPtr& _value, CString _data, Size _count)
    {
        _value = Texture2DMS::FromString(_data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DMSPtr>::value), U32>::type
        GetValueSerializationSize(Texture2DMSPtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DMSPtr>::value), void>::type
        SerializeValue(Texture2DMSPtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<Texture2DMSPtr>::value), void>::type
        DeserializeValue(Texture2DMSPtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2DMS_hpp_
//////////////////////////////////////////
