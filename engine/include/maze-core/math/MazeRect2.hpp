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
#if (!defined(_MazeRect2_hpp_))
#define _MazeRect2_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2.hpp"
#include "maze-core/data/MazeDataBlock.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Rect2
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Rect2
    {
    public:

        //////////////////////////////////////////
        static Rect2 const c_zero;

    public:

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2(Rect2 const& _rect);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2(Rect2&& _rect);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2(
            Vec2<TValue> const& _p,
            Vec2<TValue> const& _s);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2(
            TValue _x,
            TValue _y,
            TValue _w,
            TValue _h);

        //////////////////////////////////////////
        inline Rect2& operator=(Rect2 const& _rect);

        //////////////////////////////////////////
        inline Rect2& operator=(Rect2&& _rect);

        //////////////////////////////////////////
        inline bool operator==(Rect2 const& _rect) const;

        //////////////////////////////////////////
        inline bool operator!=(Rect2 const& _rect) const;

        //////////////////////////////////////////
        inline void applyUnion(Rect2 const& _rect);

        //////////////////////////////////////////
        inline void applyIntersection(Rect2 const& _rect);

        //////////////////////////////////////////
        inline Rect2<TValue> intersectedCopy(Rect2<TValue> const& _rect) const
        {
            Rect2<TValue> newRect = *this;
            newRect.applyIntersection(_rect);
            return newRect;
        }

        //////////////////////////////////////////
        inline bool contains(Vec2<TValue> const& _p);

        //////////////////////////////////////////
        inline bool hasCircleIntersection(Vec2<TValue> const& _pos, TValue _radius);

        //////////////////////////////////////////
        inline String toString(Char _separator = ',') const { return StringHelper::ToString(position.x) + _separator + StringHelper::ToString(position.y) + _separator + StringHelper::ToString(size.x) + _separator + StringHelper::ToString(size.y); }


        //////////////////////////////////////////
        static inline CString ParseString(CString _string, Size _size, Rect2& _result, Char _separator = ',')
        {
            CString end = _string + _size;

            _string = StringHelper::ParseF32(_string, end, _result.position.x);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseF32(_string, end, _result.position.y);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseF32(_string, end, _result.size.y);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ExpectSkipChar(_string, end, _separator);
            _string = StringHelper::SkipChar(_string, end, ' ');
            _string = StringHelper::ParseF32(_string, end, _result.size.y);
            return _string;
        }

        //////////////////////////////////////////
        static inline Rect2 FromString(CString _string, Size _size, Char _separator = ',')
        {
            Rect2 result = Rect2::c_zero;
            ParseString(_string, _size, result, _separator);
            return result;
        }

        //////////////////////////////////////////
        static inline Rect2 FromString(String const& _string, Char _separator = ',')
        {
            return FromString(&_string[0], _string.size(), _separator);
        }

        //////////////////////////////////////////
        inline TValue getLeft() const { return position.x; }

        //////////////////////////////////////////
        inline TValue getBottom() const { return position.y; }

        //////////////////////////////////////////
        inline TValue getRight() const { return position.x + size.x; }

        //////////////////////////////////////////
        inline TValue getTop() const { return position.y + size.y; }

        //////////////////////////////////////////
        inline Vec2<TValue> getCenter() const { return position + size * 0.5f; }
        

        //////////////////////////////////////////
        inline Vec2<TValue> const& getPosition() const { return position; }

        //////////////////////////////////////////
        inline void setPosition(Vec2<TValue> const& _value) { position = _value; }

        //////////////////////////////////////////
        inline Vec2<TValue> const& getSize() const { return size; }

        //////////////////////////////////////////
        inline void setSize(Vec2<TValue> const& _value) { size = _value; }

    public:
        Vec2<TValue> position;
        Vec2<TValue> size;
    }; 


    //////////////////////////////////////////
    using Rect2S = Rect2<S32>;
    using Rect2U = Rect2<U32>;
    using Rect2F = Rect2<F32>;


    //////////////////////////////////////////
    template <class TValue>
    inline std::ostream& operator<<(std::ostream& _o, Rect2<TValue> const& _rect)
    {
        _o << "Rect2(" << _rect.position << ", " << _rect.size <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsRect2 : std::false_type {};
    template <class TValue>
    struct IsRect2<Maze::Rect2<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsRect2<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsRect2<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsRect2<TValue>::value), void>::type
        GetValueSerializationSize(TValue const& _value, U32& _outSize)
    {
        _outSize = sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsRect2<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    MAZE_FORCEINLINE typename ::std::enable_if<(IsRect2<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(Rect2S const& _value, DataBlock& _data)
    {
        _data.setVec2S(MAZE_HCS("point"), _value.getPosition());
        _data.setVec2S(MAZE_HCS("direction"), _value.getSize());
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(Rect2S& _value, DataBlock const& _data)
    {
        _value.setPosition(_data.getVec2S(MAZE_HCS("point")));
        _value.setSize(_data.getVec2S(MAZE_HCS("direction")));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(Rect2U const& _value, DataBlock& _data)
    {
        _data.setVec2U(MAZE_HCS("point"), _value.getPosition());
        _data.setVec2U(MAZE_HCS("direction"), _value.getSize());
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(Rect2U& _value, DataBlock const& _data)
    {
        _value.setPosition(_data.getVec2U(MAZE_HCS("point")));
        _value.setSize(_data.getVec2U(MAZE_HCS("direction")));
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueToDataBlock(Rect2F const& _value, DataBlock& _data)
    {
        _data.setVec2F(MAZE_HCS("point"), _value.getPosition());
        _data.setVec2F(MAZE_HCS("direction"), _value.getSize());
    }

    //////////////////////////////////////////
    MAZE_FORCEINLINE void ValueFromDataBlock(Rect2F& _value, DataBlock const& _data)
    {
        _value.setPosition(_data.getVec2F(MAZE_HCS("point")));
        _value.setSize(_data.getVec2F(MAZE_HCS("direction")));
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeRect2.inl"


#endif // _MazeRect2_hpp_
//////////////////////////////////////////
