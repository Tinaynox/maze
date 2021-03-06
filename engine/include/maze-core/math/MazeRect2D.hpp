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
#if (!defined(_MazeRect2D_hpp_))
#define _MazeRect2D_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/math/MazeVec2D.hpp"
#include <ostream>


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Rect2D
    //
    //////////////////////////////////////////
    template <class TValue = F32>
    class Rect2D
    {
    public:

        ////////////////////////////////////
        static Rect2D const c_zero;

    public:

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2D();

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2D(Rect2D const& _rect);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2D(Rect2D&& _rect);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2D(
            Vec2D<TValue> const& _p,
            Vec2D<TValue> const& _s);

        //////////////////////////////////////////
        inline MAZE_CONSTEXPR Rect2D(
            TValue _x,
            TValue _y,
            TValue _w,
            TValue _h);

        //////////////////////////////////////////
        inline Rect2D& operator=(Rect2D const& _rect);

        //////////////////////////////////////////
        inline Rect2D& operator=(Rect2D&& _rect);

        //////////////////////////////////////////
        inline bool operator==(Rect2D const& _rect) const;

        //////////////////////////////////////////
        inline bool operator!=(Rect2D const& _rect) const;

        //////////////////////////////////////////
        inline void applyUnion(Rect2D const& _rect);

        //////////////////////////////////////////
        inline void applyIntersection(Rect2D const& _rect);

        //////////////////////////////////////////
        inline Rect2D<TValue> intersectedCopy(Rect2D<TValue> const& _rect) const
        {
            Rect2D<TValue> newRect = *this;
            newRect.applyIntersection(_rect);
            return newRect;
        }

        //////////////////////////////////////////
        inline bool contains(Vec2D<TValue> const& _p);

        //////////////////////////////////////////
        inline String toString() const { return StringHelper::ToString(position.x) + ";" + StringHelper::ToString(position.y) + ";" + StringHelper::ToString(size.x) + ";" + StringHelper::ToString(size.y); }

        //////////////////////////////////////////
        inline static Rect2D FromString(String const& _string)
        {
            Vector<String> words;
            StringHelper::SplitWords(_string, words, ';');
            if (words.size() == 4)
            {
                return Rect2D(
                    StringHelper::StringToF32(words[0]),
                    StringHelper::StringToF32(words[1]),
                    StringHelper::StringToF32(words[2]),
                    StringHelper::StringToF32(words[3]));
            }

            return Rect2D::c_zero;
        }
        
    public:
        Vec2D<TValue> position;
        Vec2D<TValue> size;
    }; 


    //////////////////////////////////////////
    using Rect2DS = Rect2D<S32>;
    using Rect2DU = Rect2D<U32>;
    using Rect2DF = Rect2D<F32>;


    //////////////////////////////////////////
    template <class TValue>
    inline std::ostream& operator<<(std::ostream& _o, Rect2D<TValue> const& _rect)
    {
        _o << "Rect2D(" << _rect.position << ", " << _rect.size <<  ")";
        return _o;
    }


    //////////////////////////////////////////
    template <typename>
    struct IsRect2D : std::false_type {};
    template <class TValue>
    struct IsRect2D<Maze::Rect2D<TValue>> : std::true_type {};


    //////////////////////////////////////////
    // Serialization
    //
    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsRect2D<TValue>::value), void>::type
        ValueToString(TValue const& _value, String& _data)
    {
        _data = _value.toString();
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsRect2D<TValue>::value), void>::type
        ValueFromString(TValue& _value, CString _data, Size _count)
    {
        _value = TValue::FromString(String(_data, _count));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsRect2D<TValue>::value), U32>::type
        GetValueSerializationSize(TValue const& _value)
    {
        return sizeof(TValue);
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsRect2D<TValue>::value), void>::type
        SerializeValue(TValue const& _value, U8* _data)
    {
        memcpy(_data, (U8 const*)(&_value), sizeof(TValue));
    }

    //////////////////////////////////////////
    template <typename TValue>
    inline typename ::std::enable_if<(IsRect2D<TValue>::value), void>::type
        DeserializeValue(TValue& _value, U8 const* _data)
    {
        memcpy((U8*)&_value, _data, sizeof(TValue));
    }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeRect2D.inl"


#endif // _MazeRect2D_hpp_
//////////////////////////////////////////
