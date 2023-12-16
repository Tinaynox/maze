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
#include "maze-core/MazeTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
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
        CString str = nullptr;
        U32 hash = 0u;

        //////////////////////////////////////////
        MAZE_FORCEINLINE HashedCString() : str(""), hash(Hash::CalculateFNV1("")) {}

        //////////////////////////////////////////
        MAZE_FORCEINLINE HashedCString(CString _str, U32 _hash) : str(_str), hash(_hash) {}

        //////////////////////////////////////////
        MAZE_FORCEINLINE HashedCString(CString _str) : str(_str), hash(Hash::CalculateFNV1(_str)) {}

        //////////////////////////////////////////
        inline HashedCString(HashedCString const& _other) = default;

        //////////////////////////////////////////
        inline HashedCString(HashedCString&& _other) = default;

        //////////////////////////////////////////
        inline HashedCString& operator=(HashedCString const& _other) = default;

        //////////////////////////////////////////
        inline HashedCString& operator=(HashedCString&& _other) = default;

        //////////////////////////////////////////
        inline bool operator<(HashedCString const& _value) const
        {
            return hash < _value.hash || (hash == _value.hash && strcmp(str, _value.str) < 0);
        }

        //////////////////////////////////////////
        inline bool operator==(HashedCString const& _value) const
        {
            if (hash != _value.hash)
                return false;

            return strcmp(str, _value.str) == 0;
        }

        //////////////////////////////////////////
        inline bool operator!=(HashedCString const& _value) const
        {
            return !this->operator==(_value);
        }

        //////////////////////////////////////////
        inline bool operator==(String const& _value) const { return _value == str; }

        //////////////////////////////////////////
        inline bool operator!=(String const& _value) const { return _value != str; }

        //////////////////////////////////////////
        friend inline bool operator==(String const& _value0, HashedCString const& _value1) noexcept;

        //////////////////////////////////////////
        friend inline bool operator!=(String const& _value0, HashedCString const& _value1) noexcept;

        //////////////////////////////////////////
        inline bool empty() const
        {
            return (str == nullptr || strcmp(str, "") == 0);
        }

        //////////////////////////////////////////
        inline operator CString() const { return str; }

        template <U32 THashValue>
        struct HashCalculator { static MAZE_CONSTEXPR U32 const hash = THashValue; };
    };

    //////////////////////////////////////////
    inline bool operator==(String const& _value0, HashedCString const& _value1) noexcept { return _value0 == _value1.str; }

    //////////////////////////////////////////
    inline bool operator!=(String const& _value0, HashedCString const& _value1) noexcept { return _value0 != _value1.str; }

    //////////////////////////////////////////
    inline std::ostream& operator<<(std::ostream& _o, HashedCString const& _v)
    {
        _o << _v.str;
        return _o;
    }
   

    //////////////////////////////////////////
    #define MAZE_HASHED_CSTRING(DText)    Maze::HashedCString                                                               \
                                          {                                                                                 \
                                              DText,                                                                        \
                                              MAZE_PRAGMA(warning(push))                                                    \
                                              MAZE_PRAGMA(warning(disable:4307))                                            \
                                              Maze::Hash::CalculateFNV1(DText)                                              \
                                              MAZE_PRAGMA(warning(pop))                                                     \
                                          }
    #define MAZE_HASHED_CSTRING_CT(DText) Maze::HashedCString                                                               \
                                          {                                                                                 \
                                              DText,                                                                        \
                                              MAZE_PRAGMA(warning(push))                                                    \
                                              MAZE_PRAGMA(warning(disable:4307))                                            \
                                              Maze::HashedCString::HashCalculator<Maze::Hash::CalculateFNV1(DText)>::hash   \
                                              MAZE_PRAGMA(warning(pop))                                                     \
                                          }

    //////////////////////////////////////////
    #define MAZE_HS(DText) MAZE_HASHED_CSTRING(DText)


    //////////////////////////////////////////
    // Struct HashedCWString
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API HashedCWString
    {
        CWString str = nullptr;
        U32 hash = 0u;

        //////////////////////////////////////////
        inline HashedCWString() : str(L""), hash(Hash::CalculateFNV1(L"")) {}

        //////////////////////////////////////////
        inline HashedCWString(CWString _str, U32 _hash) : str(_str), hash(_hash) {}

        //////////////////////////////////////////
        inline HashedCWString(CWString _str) : str(_str), hash(Hash::CalculateFNV1(_str)) {}

        //////////////////////////////////////////
        inline bool operator==(HashedCWString const& _value) const
        {
            if (hash != _value.hash)
                return false;

            return wcscmp(str, _value.str) == 0;
        }

        //////////////////////////////////////////
        inline bool operator!=(HashedCWString const& _value) const
        {
            return !this->operator==(_value);
        }

        //////////////////////////////////////////
        inline bool operator==(WString const& _value) const { return _value == str; }

        //////////////////////////////////////////
        inline bool operator!=(WString const& _value) const { return _value != str; }

        //////////////////////////////////////////
        friend inline bool operator==(WString const& _value0, HashedCWString const& _value1) noexcept;

        //////////////////////////////////////////
        friend inline bool operator!=(WString const& _value0, HashedCWString const& _value1) noexcept;

        template <U32 THashValue>
        struct HashCalculator { static MAZE_CONSTEXPR U32 const hash = THashValue; };
    };

    //////////////////////////////////////////
    inline bool operator==(WString const& _value0, HashedCWString const& _value1) noexcept { return _value0 == _value1.str; }

    //////////////////////////////////////////
    inline bool operator!=(WString const& _value0, HashedCWString const& _value1) noexcept { return _value0 != _value1.str; }

    //////////////////////////////////////////
    inline std::wostream& operator<<(std::wostream& _o, HashedCWString const& _v)
    {
        _o << _v.str;
        return _o;
    }


    //////////////////////////////////////////
    #define MAZE_HASHED_CWSTRING(DText) HashedCWString { DText, CalculateFNV1(DText) }
    #define MAZE_HASHED_CWSTRING_CT(DText) HashedCWString { DText, HashedCString::HashCalculator<CalculateFNV1(DText)>::hash }


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
