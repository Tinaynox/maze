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
#if (!defined(_MazeBaseTypes_hpp_))
#define _MazeBaseTypes_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include <ctime>
#include <climits>
#include "maze-core/preprocessor/MazePreprocessor_Platform.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using S8                 = char;
    using U8                 = unsigned char;
    using S16                = short;
    using U16                = unsigned short;
    using S32                = int;
    using U32                = unsigned int;

#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
    using S64                = signed __int64;
    using U64                = unsigned __int64;
#else
    using S64                = signed long long;
    using U64                = unsigned long long;
#endif

    using F32                = float;
    using F64                = double;

    using Size               = std::size_t;
    using UnixTime           = std::time_t;

    using Char               = char;
    using WChar              = wchar_t;

    using CString            = char const*;
    using CWString           = wchar_t const*;

    
    //////////////////////////////////////////
    static_assert(sizeof(S8)  == 1, "sizeof(S8)  is not 8 bit");
    static_assert(sizeof(U8)  == 1, "sizeof(U8)  is not 8 bit");
    static_assert(sizeof(S16) == 2, "sizeof(S16) is not 16 bit");
    static_assert(sizeof(U16) == 2, "sizeof(U16) is not 16 bit");
    static_assert(sizeof(S32) == 4, "sizeof(S32) is not 32 bit");
    static_assert(sizeof(U32) == 4, "sizeof(U32) is not 32 bit");
    static_assert(sizeof(S64) == 8, "sizeof(S32) is not 64 bit");
    static_assert(sizeof(U64) == 8, "sizeof(U32) is not 64 bit");
    static_assert(sizeof(F32) == 4, "sizeof(F32) is not 32 bit");
    static_assert(sizeof(F64) == 8, "sizeof(F64) is not 64 bit");


    //////////////////////////////////////////
    #define S8_MAX            SCHAR_MAX
    #define S8_MIN            SCHAR_MIN

    #define S16_MAX           SHRT_MAX
    #define S16_MIN           SHRT_MIN

    #define S32_MAX           INT_MAX
    #define S32_MIN           INT_MIN

    #define S64_MAX           LLONG_MAX
    #define S64_MIN           LLONG_MIN

    #define U8_MAX            UCHAR_MAX
    #define U8_MIN            0

    #define U16_MAX           USHRT_MAX
    #define U16_MIN           0

    #define U32_MAX           UINT_MAX
    #define U32_MIN           0u

    #define U64_MAX           ULLONG_MAX
    #define U64_MIN           0ui64

    #define F32_MAX           3.402823466e+38F
    #define F32_MIN           1.175494351e-38F

    #define F64_MAX           1.7976931348623158e+308
    #define F64_MIN           2.2250738585072014e-308


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeBaseTypes_hpp_
//////////////////////////////////////////
