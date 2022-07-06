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
#if (!defined(_MazeHashedWString_hpp_))
#define _MazeHashedWString_hpp_


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include "maze-core/serialization/MazeBinarySerializable.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"
#include "maze-core/utils/MazeSharedCopyable.hpp"
#include "maze-core/hash/MazeHashFNV1.hpp"
#include "maze-core/data/MazeHashedCString.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct HashedWString
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API HashedWString
    {
    public:

        //////////////////////////////////////////
        inline HashedWString()
            : m_hash(MAZE_HASHED_CWSTRING_CT(L"").hash)
        {}

        //////////////////////////////////////////
        inline HashedWString(CWString _string)
            : m_string(_string ? _string : L"")
            , m_hash(MAZE_HASHED_CWSTRING(_string ? _string : L"").hash)
        {}

        //////////////////////////////////////////
        inline HashedWString(CWString _string, Size _size)
            : m_string(_string, _string + _size)
            , m_hash(MAZE_HASHED_CWSTRING(m_string.c_str()).hash)
        {}

        //////////////////////////////////////////
        inline HashedWString(WString const& _string)
            : m_string(_string)
            , m_hash(MAZE_HASHED_CWSTRING(_string.c_str()).hash)
        {}

        //////////////////////////////////////////
        inline HashedWString(HashedCWString _other)
            : m_string(_other.str ? _other.str : L"")
            , m_hash(_other.str ? _other.hash : MAZE_HASHED_CWSTRING_CT(L"").hash)
        {}

        //////////////////////////////////////////
        inline HashedWString(HashedWString const& _other)
            : m_string(_other.m_string)
            , m_hash(_other.m_hash)
        {}

        //////////////////////////////////////////
        inline HashedWString(HashedWString&& _other) noexcept
            : m_string(std::move(_other.m_string))
            , m_hash(std::move(_other.m_hash))
        {}

        //////////////////////////////////////////
        inline WString const& getWString() const { return m_string; }

        //////////////////////////////////////////
        inline U32 getHash() const { return m_hash; }


        //////////////////////////////////////////
        inline CWString c_str() const { return m_string.c_str(); }

        //////////////////////////////////////////
        inline Size size() const { return m_string.size(); }


        //////////////////////////////////////////
        inline HashedWString& operator=(HashedWString const& _other)
        {
            m_string = _other.m_string;
            m_hash = _other.m_hash;
            return *this;
        }

        //////////////////////////////////////////
        inline HashedWString& operator=(HashedCWString _other)
        {
            m_string = _other.str ? _other.str : L"";
            m_hash = _other.str ? _other.hash : MAZE_HASHED_CWSTRING_CT(L"").hash;
            return *this;
        }

        //////////////////////////////////////////
        inline HashedWString& operator=(CWString _other)
        {
            m_string = _other ? _other : L"";
            m_hash = MAZE_HASHED_CWSTRING(_other ? _other : "").hash;
            return *this;
        }

        //////////////////////////////////////////
        inline HashedWString& operator=(HashedWString&& _other) noexcept
        {
            m_string = std::move(_other.m_string);
            m_hash = std::move(_other.m_hash);
            return *this;
        }

        //////////////////////////////////////////
        inline bool operator==(HashedWString const& _other) const
        {
            if (m_hash != _other.m_hash)
                return false;

            return m_string == _other.m_string;
        }

        //////////////////////////////////////////
        inline bool operator==(HashedCWString _other) const
        {
            if (m_hash != _other.hash)
                return false;

            return m_string == _other.str;
        }

        //////////////////////////////////////////
        inline Char const& operator [](Size _i) const
        {
            return m_string[_i];
        }

        //////////////////////////////////////////
        inline HashedCWString asHashedCWString() const { return HashedCWString{m_string.c_str(), m_hash}; }

        //////////////////////////////////////////
        inline operator HashedCWString() const { return asHashedCWString(); }

        //////////////////////////////////////////
        inline operator WString() const { return getWString(); }

        //////////////////////////////////////////
        inline bool empty() const { return m_string.empty(); }

    protected:
        WString m_string;
        U32 m_hash;
    };
   

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
namespace std
{
    //////////////////////////////////////////
    template<>
    struct hash<Maze::HashedWString>
    {
        inline size_t operator()(Maze::HashedWString const& _s) const noexcept
        {
            return _s.getHash();
        }
    };


} // namespace std
//////////////////////////////////////////


#endif // _MazeHashedWString_h_
//////////////////////////////////////////
