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
#if (!defined(_MazeTypes_hpp_))
#define _MazeTypes_hpp_

//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/memory/MazeNedMemoryAllocator.hpp"
#include "maze-core/memory/MazeStdMemoryAllocator.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/hash/MazeHashSuperFast.hpp"
#include "maze-core/hash/MazeHashFNV1.hpp"
#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <stack>
#include <deque>
#include <queue>
#include <memory>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using StringAllocator = StdMemoryAllocator<char, NedMemoryAllocator>;
    using WStringAllocator = StdMemoryAllocator<wchar_t, NedMemoryAllocator>;


    //////////////////////////////////////////
    using String = std::basic_string<char, std::char_traits<char>, StringAllocator>;

    //////////////////////////////////////////
    template <typename>
    struct IsString : std::false_type {};
    template <>
    struct IsString<Maze::String> : std::true_type {};


    //////////////////////////////////////////
    using StringStream = std::basic_stringstream<char, std::char_traits<char>, StringAllocator>;


    //////////////////////////////////////////
    using InStringStream = std::basic_istringstream<char, std::char_traits<char>, StringAllocator>;


    //////////////////////////////////////////
    using OutStringStream = std::basic_ostringstream<char, std::char_traits<char>, StringAllocator>;


    //////////////////////////////////////////
    using WString = std::basic_string<wchar_t, std::char_traits<wchar_t>, WStringAllocator>;

    //////////////////////////////////////////
    template <typename>
    struct IsWString : std::false_type {};
    template <>
    struct IsWString<Maze::WString> : std::true_type {};


    //////////////////////////////////////////
    using WStringStream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, WStringAllocator>;


    //////////////////////////////////////////
    // Array
    //
    //////////////////////////////////////////
    template <
        class _Ty,
        size_t _Size>
    using Array = std::array<_Ty, _Size>;

    //////////////////////////////////////////
    template <typename>
    struct IsArray : std::false_type {};
    template <
        class _Ty,
        size_t _Size>
    struct IsArray<Maze::Array<_Ty, _Size>> : std::true_type {};


    //////////////////////////////////////////
    // Vector
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Vector = std::vector<_Ty, StdMemoryAllocator<_Ty, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsVector : std::false_type {};
    template <class _Ty>
    struct IsVector<Maze::Vector<_Ty>> : std::true_type {};


    //////////////////////////////////////////
    // List
    //
    //////////////////////////////////////////
    template <class _Ty>
    using List = std::list<_Ty, StdMemoryAllocator<_Ty, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsList : std::false_type {};
    template <class _Ty>
    struct IsList<Maze::List<_Ty>> : std::true_type {};

    //////////////////////////////////////////
    // Pair
    //
    //////////////////////////////////////////
    template <
        class _Ty1,
        class _Ty2>
    using Pair = std::pair<_Ty1, _Ty2>;

    //////////////////////////////////////////
    template <typename>
    struct IsPair : std::false_type {};
    template <
        class _Ty1,
        class _Ty2>
    struct IsPair<Maze::Pair<_Ty1, _Ty2>> : std::true_type {};


    //////////////////////////////////////////
    // Map
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Ty,
        class _Pr = std::less<_Kty>>
    using Map = std::map<_Kty, _Ty, _Pr, StdMemoryAllocator<Pair<const _Kty, _Ty>, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsMap : std::false_type {};
    template <
        class _Kty,
        class _Ty,
        class _Pr>
        struct IsMap<Maze::Map<_Kty, _Ty, _Pr>> : std::true_type {};


    //////////////////////////////////////////
    // MultiMap
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Ty,
        class _Pr = std::less<_Kty>>
    using MultiMap = std::multimap<_Kty, _Ty, _Pr, StdMemoryAllocator<Pair<const _Kty, _Ty>, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsMultiMap : std::false_type {};
    template <
        class _Kty,
        class _Ty,
        class _Pr>
        struct IsMultiMap<Maze::MultiMap<_Kty, _Ty, _Pr>> : std::true_type {};


    //////////////////////////////////////////
    // UnorderedMap
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Ty,
        class _Hasher = std::hash<_Kty>,
        class _Keyeq = std::equal_to<_Kty>>
    using UnorderedMap = std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, StdMemoryAllocator<Pair<const _Kty, _Ty>, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsUnorderedMap : std::false_type {};
    template <
        class _Kty,
        class _Ty,
        class _Hasher,
        class _Keyeq>
        struct IsUnorderedMap<Maze::UnorderedMap<_Kty, _Ty, _Hasher, _Keyeq>> : std::true_type {};


    //////////////////////////////////////////
    // Set
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Pr = std::less<_Kty>>
    using Set = std::set<_Kty, _Pr, StdMemoryAllocator<_Kty, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsSet : std::false_type {};
    template <
        class _Kty,
        class _Pr>
        struct IsSet<Maze::Set<_Kty, _Pr>> : std::true_type {};


    //////////////////////////////////////////
    // MultiSet
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Pr = std::less<_Kty>>
        using MultiSet = std::multiset<_Kty, _Pr, StdMemoryAllocator<_Kty, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsMultiSet : std::false_type {};
    template <
        class _Kty,
        class _Pr>
        struct IsMultiSet<Maze::MultiSet<_Kty, _Pr>> : std::true_type {};


    //////////////////////////////////////////
    // Deque
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Deque = std::deque<_Ty, StdMemoryAllocator<_Ty, NedMemoryAllocator>>;

    //////////////////////////////////////////
    template <typename>
    struct IsDeque : std::false_type {};
    template <
        class _Ty>
        struct IsDeque<Maze::Deque<_Ty>> : std::true_type {};


    //////////////////////////////////////////
    // Stack
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Stack = std::stack<_Ty, Deque<_Ty>>;

    //////////////////////////////////////////
    template <typename>
    struct IsStack : std::false_type {};
    template <
        class _Ty>
        struct IsStack<Maze::Stack<_Ty>> : std::true_type {};


    //////////////////////////////////////////
    // Queue
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Queue = std::queue<_Ty, Deque<_Ty>>;

    //////////////////////////////////////////
    template <typename>
    struct IsQueue : std::false_type {};
    template <
        class _Ty>
        struct IsQueue<Maze::Queue<_Ty>> : std::true_type {};



    //////////////////////////////////////////
    template <class T, class... Args>
    std::shared_ptr<T> MakeShared(Args&&... args)
    {
        using Alloc = StdMemoryAllocator<T, NedMemoryAllocator>;
        return std::allocate_shared<T>(Alloc{}, std::forward<Args>(args)...);
    }


    //////////////////////////////////////////
    namespace Hash
    {
        //////////////////////////////////////////
        MAZE_CORE_API inline U32 CalculateCRC32(String const& _text, U32 _crc = 0)
        {
            return CalculateCRC32(_text.c_str(), _text.size(), _crc);
        }

        //////////////////////////////////////////
        MAZE_CORE_API inline U32 CalculateSuperFastHash(String const& _text)
        {
            return CalculateSuperFastHash(_text.c_str(), (S32)_text.size());
        }
    } // namespace Hash
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
namespace std
{
    //////////////////////////////////////////
    template <>
    struct hash<Maze::String>
    {
        inline std::size_t operator()(const Maze::String& k) const
        {
            return static_cast<std::size_t>(Maze::Hash::CalculateFNV1(k.c_str()));
        }
    };

    //////////////////////////////////////////
    template <>
    struct hash<Maze::WString>
    {
        inline std::size_t operator()(const Maze::WString& k) const
        {
            return static_cast<std::size_t>(Maze::Hash::CalculateFNV1(k.c_str()));
        }
    };

} // namespace std
//////////////////////////////////////////


#endif // _MazeTypes_hpp_
//////////////////////////////////////////
