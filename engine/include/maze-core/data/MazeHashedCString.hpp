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
#if (!defined(_MazeHashedCString_hpp_))
#define _MazeHashedCString_hpp_


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/hash/MazeHashFNV1.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct HashedCString
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API HashedCString
    {
        CString str;
        U32 hash;

        //////////////////////////////////////////
        inline bool operator==(HashedCString const& _value) const
        {
            return strcmp(str, _value.str) == 0;
        }

        template <U32 THashValue>
        struct HashCalculator { static MAZE_CONSTEXPR U32 const hash = THashValue; };
    };
   

    //////////////////////////////////////////
    #define MAZE_HASHED_CSTRING(DText) HashedCString { DText, CalculateFNV1(DText) }
    #define MAZE_HASHED_CSTRING_CT(DText) HashedCString { DText, HashedCString::HashCalculator<CalculateFNV1(DText)>::hash }


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
namespace std
{
    //////////////////////////////////////////
    template<>
    struct hash<Maze::HashedCString>
    {
        inline size_t operator()(Maze::HashedCString const& _s) const noexcept
        {
            return _s.hash;
        }
    };


} // namespace std
//////////////////////////////////////////


#endif // _MazeHashedCString_h_
//////////////////////////////////////////
