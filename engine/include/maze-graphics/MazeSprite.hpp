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
#if (!defined(_MazeSprite_hpp_))
#define _MazeSprite_hpp_


//////////////////////////////////////////
#include "maze-graphics/MazeGraphicsHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-core/utils/MazeMultiDelegate.hpp"
#include "maze-core/utils/MazeEnumClass.hpp"
#include "maze-core/system/MazeWindowVideoMode.hpp"
#include "maze-core/system/MazeWindow.hpp"
#include "maze-core/utils/MazeUpdater.hpp"
#include "maze-core/system/MazeInputEvent.hpp"
#include "maze-core/containers/MazeFastVector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(Sprite);
    MAZE_USING_SHARED_PTR(Texture2D);


    //////////////////////////////////////////
    // Struct SpriteSliceBorder
    //
    //////////////////////////////////////////
    struct MAZE_GRAPHICS_API SpriteSliceBorder
    {
        //////////////////////////////////////////
        SpriteSliceBorder();

        //////////////////////////////////////////
        SpriteSliceBorder(F32 _left, F32 _bottom, F32 _right, F32 _top);

        //////////////////////////////////////////
        bool isBorderExists() const;

        F32 left;
        F32 bottom;
        F32 right;
        F32 top;        
    };


    //////////////////////////////////////////
    // Class Sprite
    //
    //                  nativeSize
    //                -------------------
    //                |    colorSize    |
    //                |   -----------   |
    //                |   |XYXYXYXYX|   |
    //                |   |YXYXYXYXY|   |
    //                |   |XYXYXYXYX|   |
    // colorOffset    |-> o----------   |
    // colorPosition  | /               |
    //                -------------------
    //
    //////////////////////////////////////////
    class MAZE_GRAPHICS_API Sprite
        : public SharedObject<Sprite>
        , public MultiDelegateCallbackReceiver
    {
    public:

        //////////////////////////////////////////
        virtual ~Sprite();

        //////////////////////////////////////////
        static SpritePtr Create();

        //////////////////////////////////////////
        static SpritePtr Create(
            Texture2DPtr const& _texture,
            Vec2DF const& _colorPosition,
            Vec2DF const& _colorSize,
            Vec2DF const& _colorOffset,
            Vec2DF const& _nativeSize = Vec2DF::c_zero);

        //////////////////////////////////////////
        static SpritePtr Create(
            Texture2DPtr const& _texture,
            Vec2DF const& _nativePosition = Vec2DF::c_zero,
            Vec2DF const& _nativeSize = Vec2DF::c_zero);

        //////////////////////////////////////////
        void set(Texture2DPtr const& _texture);

        //////////////////////////////////////////
        void set(
            Texture2DPtr const& _texture,
            Vec2DF const& _colorPosition,
            Vec2DF const& _colorSize,
            Vec2DF const& _colorOffset,
            Vec2DF const& _nativeSize);


        //////////////////////////////////////////
        String const& getName() const { return m_name; }

        //////////////////////////////////////////
        void setName(String const& _name)
        {
            m_name = _name;
            eventDataChanged(this);
        }


        //////////////////////////////////////////
        inline const Texture2DPtr& getTexture() const { return m_texture; }


        //////////////////////////////////////////
        inline Vec2DF const& getColorOffset() const { return m_colorOffset; }


        //////////////////////////////////////////
        void setColorPosition(Vec2DF const& _colorPosition);

        //////////////////////////////////////////
        inline Vec2DF const& getColorPosition() const { return m_colorPosition; }

        //////////////////////////////////////////
        void setColorSize(Vec2DF const& _colorSize);

        //////////////////////////////////////////
        inline Vec2DF const& getColorSize() const { return m_colorSize; }


        //////////////////////////////////////////
        void setNativeSize(Vec2DF const& _nativeSize);

        //////////////////////////////////////////
        inline Vec2DF const& getNativeSize() const { return m_nativeSize; }


        //////////////////////////////////////////
        inline Vec2DF const& getTextureCoordLB() const { return m_textureCoordLB; }

        //////////////////////////////////////////
        inline Vec2DF const& getTextureCoordRT() const { return m_textureCoordRT; }


        //////////////////////////////////////////
        inline void setSliceBorder(SpriteSliceBorder const& _sliceBorder)
        {
            m_sliceBorder = _sliceBorder;
            eventDataChanged(this);
        }

        //////////////////////////////////////////
        inline void setSliceBorder(F32 _left, F32 _bottom, F32 _right, F32 _top)
        {
            setSliceBorder(SpriteSliceBorder(_left, _bottom, _right, _top));
        }

        //////////////////////////////////////////
        inline SpriteSliceBorder const& getSliceBorder() const { return m_sliceBorder; }

        //////////////////////////////////////////
        void updateTextureCoords();

    public:

        //////////////////////////////////////////
        MultiDelegate<Sprite*> eventDataChanged;

    public:

        //////////////////////////////////////////
        static void FromString(SpritePtr& _value, CString _data, Size _count);

        //////////////////////////////////////////
        static void ToString(Sprite const* _value, String& _data);

    protected:

        //////////////////////////////////////////
        Sprite();

        //////////////////////////////////////////
        bool init();

        //////////////////////////////////////////
        bool init(
            Texture2DPtr const& _texture,
            Vec2DF const& _colorPosition,
            Vec2DF const& _colorSize,
            Vec2DF const& _colorOffset,
            Vec2DF const& _nativeSize);
    
    protected:
        String m_name;
        Texture2DPtr m_texture;

        Vec2DF m_colorOffset;
        Vec2DF m_colorPosition;
        Vec2DF m_colorSize;
        Vec2DF m_nativeSize;

        Vec2DF m_textureCoordLB;
        Vec2DF m_textureCoordRT;

        SpriteSliceBorder m_sliceBorder;
    };


    //////////////////////////////////////////
    MAZE_NOT_IMPLEMENTED_SERIALIZATION(Sprite);


    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SpritePtr>::value), void>::type
        ValueToString(SpritePtr const& _value, String& _data)
    {
        Sprite::ToString(_value.get(), _data);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SpritePtr>::value), void>::type
        ValueFromString(SpritePtr& _value, CString _data, Size _count)
    {
        Sprite::FromString(_value, _data, _count);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SpritePtr>::value), U32>::type
        GetValueSerializationSize(SpritePtr const& _value)
    {
        MAZE_NOT_IMPLEMENTED_RETURN_VALUE(0);
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SpritePtr>::value), void>::type
        SerializeValue(SpritePtr const& _value, U8* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    template <>
    inline typename ::std::enable_if<(IsSharedPtr<SpritePtr>::value), void>::type
        DeserializeValue(SpritePtr& _value, U8 const* _data)
    {
        MAZE_NOT_IMPLEMENTED;
    }


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSprite_hpp_
//////////////////////////////////////////
