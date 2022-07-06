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
#if (!defined(_MazeHashFNV1_hpp_))
#define _MazeHashFNV1_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include <fstream>


//////////////////////////////////////////
namespace Maze
{
    template <typename THashType>
    struct FNV1Data;

    template<>
    struct FNV1Data<U32>
    {
        MAZE_CONSTEXPR static U32 const c_Offset = 0x811C9DC5;
        MAZE_CONSTEXPR static U32 const c_Prime  = 0x01000193;
    };

    //////////////////////////////////////////
    template <typename TChar, typename THashType>
    inline MAZE_CORE_API MAZE_CONSTEXPR THashType CalculateFNV1(
        TChar const* const _text,
        THashType const _hash = FNV1Data<THashType>::c_Offset)
    {
        return ((*_text == '\0') ? _hash : CalculateFNV1<TChar, THashType>(
            _text + 1,
            (_hash * FNV1Data<THashType>::c_Prime) ^ THashType(*_text)));
    }

    //////////////////////////////////////////
    template <typename TChar, typename THashType>
    inline MAZE_CORE_API MAZE_CONSTEXPR THashType CalculateFNV1Sized(
        TChar const* const _text,
        Size const _length,
        THashType const _hash = FNV1Data<THashType>::c_Offset)
    {
        return (_length == 0) ? _hash : CalculateFNV1Sized<TChar, THashType>(
            _text + 1,
            _length - 1,
            (_hash * FNV1Data<THashType>::c_Prime) ^ THashType(*_text));
    }

    //////////////////////////////////////////
    template <typename TChar, typename THashType>
    inline MAZE_CORE_API MAZE_CONSTEXPR THashType CalculateFNV1A(
        TChar const* const _text,
        THashType const _hash = FNV1Data<THashType>::c_Offset)
    {
        return (*_text == '\0') ? _hash : CalculateFNV1A<TChar, THashType>(
            _text + 1,
            (_hash ^ THashType(*_text)) * FNV1Data<THashType>::c_Prime);
    }

    //////////////////////////////////////////
    template <typename TChar, typename THashType>
    inline MAZE_CORE_API MAZE_CONSTEXPR THashType CalculateFNV1ASized(
        TChar const* const _text,
        Size const _length,
        THashType const _hash = FNV1Data<THashType>::c_Offset)
    {
        return (_length == 0) ? _hash : CalculateFNV1ASized<TChar, THashType>(
            _text + 1,
            _length - 1,
            (_hash ^ THashType(*_text)) * FNV1Data<THashType>::c_Prime);
    }
    
    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1(Char const* const _text)
    {
        return CalculateFNV1<Char, U32>(_text);
    }
    
    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1(Char const* const _text, Size const _length)
    {
        return CalculateFNV1Sized<Char, U32>(_text, _length);
    }

    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1A(Char const* const _text)
    {
        return CalculateFNV1A<Char, U32>(_text);
    }

    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1A(Char const* const _text, Size const _length)
    {
        return CalculateFNV1ASized<Char, U32>(_text, _length);
    }


    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1(WChar const* const _text)
    {
        return CalculateFNV1<WChar, U32>(_text);
    }

    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1(WChar const* const _text, Size const _length)
    {
        return CalculateFNV1Sized<WChar, U32>(_text, _length);
    }

    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1A(WChar const* const _text)
    {
        return CalculateFNV1A<WChar, U32>(_text);
    }

    //////////////////////////////////////////
    inline MAZE_CORE_API MAZE_CONSTEXPR U32 CalculateFNV1A(WChar const* const _text, Size const _length)
    {
        return CalculateFNV1ASized<WChar, U32>(_text, _length);
    }
    
    

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHashFNV1_hpp_
//////////////////////////////////////////
