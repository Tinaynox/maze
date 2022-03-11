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
#if (!defined(_MazeHashedString_hpp_))
#define _MazeHashedString_hpp_


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
    // Struct HashedString
    //
    //////////////////////////////////////////
    struct MAZE_CORE_API HashedString
    {
    public:

        //////////////////////////////////////////
        inline HashedString()
            : m_hash(MAZE_HASHED_CSTRING_CT("").hash)
        {}

        //////////////////////////////////////////
        inline HashedString(CString _string)
            : m_string(_string ? _string : "")
            , m_hash(MAZE_HASHED_CSTRING(_string ? _string : "").hash)
        {}

        //////////////////////////////////////////
        inline HashedString(CString _string, Size _size)
            : m_string(_string, _string + _size)
            , m_hash(MAZE_HASHED_CSTRING(m_string.c_str()).hash)
        {}

        //////////////////////////////////////////
        inline HashedString(String const& _string)
            : m_string(_string)
            , m_hash(MAZE_HASHED_CSTRING(_string.c_str()).hash)
        {}

        //////////////////////////////////////////
        inline HashedString(HashedCString _other)
            : m_string(_other.str ? _other.str : "")
            , m_hash(_other.str ? _other.hash : MAZE_HASHED_CSTRING_CT("").hash)
        {}

        //////////////////////////////////////////
        inline HashedString(HashedString const& _other)
            : m_string(_other.m_string)
            , m_hash(_other.m_hash)
        {}

        //////////////////////////////////////////
        inline HashedString(HashedString&& _other) noexcept
            : m_string(std::move(_other.m_string))
            , m_hash(std::move(_other.m_hash))
        {}

        //////////////////////////////////////////
        inline String const& getString() const { return m_string; }

        //////////////////////////////////////////
        inline U32 getHash() const { return m_hash; }


        //////////////////////////////////////////
        inline CString c_str() const { return m_string.c_str(); }

        //////////////////////////////////////////
        inline Size size() const { return m_string.size(); }


        //////////////////////////////////////////
        inline HashedString& operator=(HashedString const& _other)
        {
            m_string = _other.m_string;
            m_hash = _other.m_hash;
            return *this;
        }

        //////////////////////////////////////////
        inline HashedString& operator=(HashedCString _other)
        {
            m_string = _other.str ? _other.str : "";
            m_hash = _other.str ? _other.hash : MAZE_HASHED_CSTRING_CT("").hash;
            return *this;
        }

        //////////////////////////////////////////
        inline HashedString& operator=(CString _other)
        {
            m_string = _other ? _other : "";
            m_hash = MAZE_HASHED_CSTRING(_other ? _other : "").hash;
            return *this;
        }

        //////////////////////////////////////////
        inline HashedString& operator=(HashedString&& _other) noexcept
        {
            m_string = std::move(_other.m_string);
            m_hash = std::move(_other.m_hash);
            return *this;
        }

        //////////////////////////////////////////
        inline bool operator==(HashedString const& _other) const
        {
            if (m_hash != _other.m_hash)
                return false;

            return m_string == _other.m_string;
        }

        //////////////////////////////////////////
        inline bool operator==(HashedCString _other) const
        {
            if (m_hash != _other.hash)
                return false;

            return m_string == _other.str;
        }

        //////////////////////////////////////////
        inline HashedCString asHashedCString() const { return HashedCString{m_string.c_str(), m_hash}; }

        //////////////////////////////////////////
        inline operator HashedCString() const { return asHashedCString(); }

        //////////////////////////////////////////
        inline operator String() const { return getString(); }

        //////////////////////////////////////////
        inline bool empty() const { return m_string.empty(); }

    protected:
        String m_string;
        U32 m_hash;
    };
   

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
namespace std
{
    //////////////////////////////////////////
    template<>
    struct hash<Maze::HashedString>
    {
        inline size_t operator()(Maze::HashedString const& _s) const noexcept
        {
            return _s.getHash();
        }
    };


} // namespace std
//////////////////////////////////////////


#endif // _MazeHashedString_h_
//////////////////////////////////////////
