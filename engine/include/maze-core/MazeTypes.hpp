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
#include "maze-core/data/MazeSpan.hpp"
#include <EASTL/string.h>
#include <EASTL/string_view.h>
#include <EASTL/array.h>
#include <EASTL/vector.h>
#include <EASTL/list.h>
#include <EASTL/utility.h>
#include <EASTL/tuple.h>
#include <EASTL/map.h>
#include <EASTL/unordered_map.h>
#include <EASTL/set.h>
#include <EASTL/deque.h>
#include <EASTL/stack.h>
#include <EASTL/queue.h>
#include <EASTL/functional.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/sort.h>
#include <string>
#include <sstream>
#include <memory>


//////////////////////////////////////////
// Note: the containers below (String, Vector, Map, etc) are backed by EASTL,
// using EASTL's default allocator (EASTLAllocatorType, see MazeEASTLAllocator.cpp
// for the global operator new[] overloads it requires) rather than
// NedMemoryAllocator. EASTL has no stringstream equivalent, so StringStream/
// InStringStream/OutStringStream/WStringStream remain std::basic_stringstream-based;
// converting between them and String/WString requires going through c_str().
//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using CStringSpan = Span<Char const>;

    //////////////////////////////////////////
    using String = eastl::basic_string<char>;

    //////////////////////////////////////////
    using StringView = eastl::basic_string_view<char>;

    //////////////////////////////////////////
    template <typename>
    struct IsString : eastl::false_type {};
    template <>
    struct IsString<Maze::String> : eastl::true_type {};


    //////////////////////////////////////////
    using StringAllocator = StdMemoryAllocator<char, NedMemoryAllocator>;
    using WStringAllocator = StdMemoryAllocator<wchar_t, NedMemoryAllocator>;


    //////////////////////////////////////////
    using StringStream = std::basic_stringstream<char, std::char_traits<char>, StringAllocator>;


    //////////////////////////////////////////
    using InStringStream = std::basic_istringstream<char, std::char_traits<char>, StringAllocator>;


    //////////////////////////////////////////
    using OutStringStream = std::basic_ostringstream<char, std::char_traits<char>, StringAllocator>;


    //////////////////////////////////////////
    using WString = eastl::basic_string<wchar_t>;

    //////////////////////////////////////////
    using WStringView = eastl::basic_string_view<wchar_t>;

    //////////////////////////////////////////
    template <typename>
    struct IsWString : eastl::false_type {};
    template <>
    struct IsWString<Maze::WString> : eastl::true_type {};


    //////////////////////////////////////////
    using WStringStream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, WStringAllocator>;


    //////////////////////////////////////////
    // Array
    //
    //////////////////////////////////////////
    template <
        class _Ty,
        size_t _Size>
    using Array = eastl::array<_Ty, _Size>;

    //////////////////////////////////////////
    template <typename>
    struct IsArray : eastl::false_type {};
    template <
        class _Ty,
        size_t _Size>
    struct IsArray<Maze::Array<_Ty, _Size>> : eastl::true_type {};


    //////////////////////////////////////////
    // Vector
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Vector = eastl::vector<_Ty>;

    //////////////////////////////////////////
    template <typename>
    struct IsVector : eastl::false_type {};
    template <class _Ty>
    struct IsVector<Maze::Vector<_Ty>> : eastl::true_type {};


    //////////////////////////////////////////
    // List
    //
    //////////////////////////////////////////
    template <class _Ty>
    using List = eastl::list<_Ty>;

    //////////////////////////////////////////
    template <typename>
    struct IsList : eastl::false_type {};
    template <class _Ty>
    struct IsList<Maze::List<_Ty>> : eastl::true_type {};

    //////////////////////////////////////////
    // Pair
    //
    //////////////////////////////////////////
    template <
        class _Ty1,
        class _Ty2>
    using Pair = eastl::pair<_Ty1, _Ty2>;

    //////////////////////////////////////////
    template <typename>
    struct IsPair : eastl::false_type {};
    template <
        class _Ty1,
        class _Ty2>
    struct IsPair<Maze::Pair<_Ty1, _Ty2>> : eastl::true_type {};


    //////////////////////////////////////////
    // Map
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Ty,
        class _Pr = eastl::less<_Kty>>
    using Map = eastl::map<_Kty, _Ty, _Pr>;

    //////////////////////////////////////////
    template <typename>
    struct IsMap : eastl::false_type {};
    template <
        class _Kty,
        class _Ty,
        class _Pr>
        struct IsMap<Maze::Map<_Kty, _Ty, _Pr>> : eastl::true_type {};


    //////////////////////////////////////////
    // MultiMap
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Ty,
        class _Pr = eastl::less<_Kty>>
    using MultiMap = eastl::multimap<_Kty, _Ty, _Pr>;

    //////////////////////////////////////////
    template <typename>
    struct IsMultiMap : eastl::false_type {};
    template <
        class _Kty,
        class _Ty,
        class _Pr>
        struct IsMultiMap<Maze::MultiMap<_Kty, _Ty, _Pr>> : eastl::true_type {};


    //////////////////////////////////////////
    // UnorderedMap
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Ty,
        class _Hasher = eastl::hash<_Kty>,
        class _Keyeq = eastl::equal_to<_Kty>>
    using UnorderedMap = eastl::unordered_map<_Kty, _Ty, _Hasher, _Keyeq>;

    //////////////////////////////////////////
    template <typename>
    struct IsUnorderedMap : eastl::false_type {};
    template <
        class _Kty,
        class _Ty,
        class _Hasher,
        class _Keyeq>
        struct IsUnorderedMap<Maze::UnorderedMap<_Kty, _Ty, _Hasher, _Keyeq>> : eastl::true_type {};


    //////////////////////////////////////////
    // Set
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Pr = eastl::less<_Kty>>
    using Set = eastl::set<_Kty, _Pr>;

    //////////////////////////////////////////
    template <typename>
    struct IsSet : eastl::false_type {};
    template <
        class _Kty,
        class _Pr>
        struct IsSet<Maze::Set<_Kty, _Pr>> : eastl::true_type {};


    //////////////////////////////////////////
    // MultiSet
    //
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Pr = eastl::less<_Kty>>
        using MultiSet = eastl::multiset<_Kty, _Pr>;

    //////////////////////////////////////////
    template <typename>
    struct IsMultiSet : eastl::false_type {};
    template <
        class _Kty,
        class _Pr>
        struct IsMultiSet<Maze::MultiSet<_Kty, _Pr>> : eastl::true_type {};


    //////////////////////////////////////////
    // Deque
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Deque = eastl::deque<_Ty>;

    //////////////////////////////////////////
    template <typename>
    struct IsDeque : eastl::false_type {};
    template <
        class _Ty>
        struct IsDeque<Maze::Deque<_Ty>> : eastl::true_type {};


    //////////////////////////////////////////
    // Stack
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Stack = eastl::stack<_Ty, Deque<_Ty>>;

    //////////////////////////////////////////
    template <typename>
    struct IsStack : eastl::false_type {};
    template <
        class _Ty>
        struct IsStack<Maze::Stack<_Ty>> : eastl::true_type {};


    //////////////////////////////////////////
    // Queue
    //
    //////////////////////////////////////////
    template <class _Ty>
    using Queue = eastl::queue<_Ty, Deque<_Ty>>;

    //////////////////////////////////////////
    template <typename>
    struct IsQueue : eastl::false_type {};
    template <
        class _Ty>
        struct IsQueue<Maze::Queue<_Ty>> : eastl::true_type {};


    //////////////////////////////////////////
    // UniquePtr
    //
    //////////////////////////////////////////
    template <
        class _Ty,
        class _Dx = eastl::default_delete<_Ty>>
    using UniquePtr = eastl::unique_ptr<_Ty, _Dx>;

    //////////////////////////////////////////
    template <class _Ty, typename ...Args>
    inline UniquePtr<_Ty> MakeUnique(Args&& ..._args) { return UniquePtr<_Ty>(new _Ty(eastl::forward<Args>(_args)...)); }


    //////////////////////////////////////////
    // SharedPtr / WeakPtr
    //
    //////////////////////////////////////////
    template <class _Ty>
    using SharedPtr = eastl::shared_ptr<_Ty>;

    //////////////////////////////////////////
    template <typename>
    struct IsSharedPtr : eastl::false_type {};
    template <class _Ty>
    struct IsSharedPtr<Maze::SharedPtr<_Ty>> : eastl::true_type {};

    //////////////////////////////////////////
    template <class _Ty>
    using WeakPtr = eastl::weak_ptr<_Ty>;

    //////////////////////////////////////////
    template <typename>
    struct IsWeakPtr : eastl::false_type {};
    template <class _Ty>
    struct IsWeakPtr<Maze::WeakPtr<_Ty>> : eastl::true_type {};


    //////////////////////////////////////////
    template <class T, class... Args>
    SharedPtr<T> MakeShared(Args&&... args)
    {
        return eastl::allocate_shared<T>(GetDefaultEastlNedAllocator(), eastl::forward<Args>(args)...);
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

    
    //////////////////////////////////////////
    inline std::ostream& operator<<(std::ostream& _o, String const& _v)
    {
        _o << _v.c_str();
        return _o;
    }

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTypes_hpp_
//////////////////////////////////////////
