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
#if (!defined(_MazeStdTypes_hpp_))
#define _MazeStdTypes_hpp_


//////////////////////////////////////////
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
#include <sstream>
#include <iostream>
#include <iomanip>
#include <functional>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using StdStringAllocator = std::allocator<char>;
    using StdWStringAllocator = std::allocator<wchar_t>;
    
    //////////////////////////////////////////
    using StdString = std::basic_string<char, std::char_traits<char>, StdStringAllocator>;
    
    //////////////////////////////////////////
    using StdStringStream = std::basic_stringstream<char, std::char_traits<char>, StdStringAllocator>;

    //////////////////////////////////////////
    using StdInStringStream = std::basic_istringstream<char, std::char_traits<char>, StdStringAllocator>;
    
    //////////////////////////////////////////
    using StdOutStringStream = std::basic_ostringstream<char, std::char_traits<char>, StdStringAllocator>;

    //////////////////////////////////////////
    using StdWString = std::basic_string<wchar_t, std::char_traits<wchar_t>, StdWStringAllocator>;
    
    //////////////////////////////////////////
    using StdWStringStream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, StdWStringAllocator>;
    
    //////////////////////////////////////////
    template <
        class _Ty,
        size_t _Size>
    using StdArray = std::array<_Ty, _Size>;

    //////////////////////////////////////////
    template <    
        class _Ty,
        class _Alloc = std::allocator<_Ty>>
    using StdVector = std::vector<_Ty, _Alloc>;
    
    //////////////////////////////////////////
    template <    
        class _Ty,
        class _Alloc = std::allocator<_Ty>>
    using StdList = std::list<_Ty, _Alloc>;
    
    //////////////////////////////////////////
    template <    
        class _Ty1,
        class _Ty2>
    using StdPair = std::pair<_Ty1, _Ty2>;
    
    //////////////////////////////////////////
    template <    
        class _Kty,
        class _Ty,
        class _Pr = std::less<_Kty>,
        class _Alloc = std::allocator<StdPair<const _Kty, _Ty>>>
    using StdMap = std::map<_Kty, _Ty, _Pr, _Alloc>;
    
    //////////////////////////////////////////
    template <    
        class _Kty,
        class _Ty,
        class _Hasher = std::hash<_Kty>,
        class _Keyeq = std::equal_to<_Kty>,
        class _Alloc = std::allocator< StdPair<const _Kty, _Ty>>>
    using StdUnorderedMap = std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>;
    
    //////////////////////////////////////////
    template <
        class _Kty,
        class _Pr = std::less<_Kty>,
        class _Alloc = std::allocator<_Kty>>
    using StdSet = std::set<_Kty, _Pr, _Alloc>;
    
    //////////////////////////////////////////
    template <     
        class _Ty,
        class _Alloc = std::allocator<_Ty>>
    using StdDeque = std::deque<_Ty, _Alloc>;
    
    //////////////////////////////////////////
    template <     
        class _Ty,
        class _Container = std::deque<_Ty>>
    using StdStack = std::stack<_Ty, _Container>;
    
    //////////////////////////////////////////
    template <     
        class _Ty,
        class _Container = std::deque<_Ty> >
    using StdQueue = std::queue<_Ty, _Container>;
    
    //////////////////////////////////////////
    template <    
        class _Ty,
        class _Dx = std::default_delete<_Ty>>
    using UniquePtr = std::unique_ptr<_Ty, _Dx>;

    //////////////////////////////////////////
    template <class _Ty, typename ...Args>
    inline UniquePtr<_Ty> MakeUnique(Args&& ..._args) { return std::make_unique<_Ty>(std::forward<Args>(_args)...); }
    
    //////////////////////////////////////////
    template <class _Ty>
    using SharedPtr = std::shared_ptr<_Ty>;

    //////////////////////////////////////////
    template <class _Ty, typename ...Args>
    inline SharedPtr<_Ty> MakeShared(Args&& ..._args) { return std::make_shared<_Ty>(std::forward<Args>(_args)...); }

    //////////////////////////////////////////
    template <typename>
    struct IsSharedPtr : std::false_type {};
    template <class _Ty>
    struct IsSharedPtr<Maze::SharedPtr<_Ty>> : std::true_type {};
    
    //////////////////////////////////////////
    template <class _Ty>
    using WeakPtr = std::weak_ptr<_Ty>;

    //////////////////////////////////////////
    template <typename>
    struct IsWeakPtr : std::false_type {};
    template <class _Ty>
    struct IsWeakPtr<Maze::WeakPtr<_Ty>> : std::true_type {};
    
    //////////////////////////////////////////
    using FileStream = std::fstream;
    
    //////////////////////////////////////////
    using InputFileStream = std::ifstream;
    
    //////////////////////////////////////////
    using OutputFileStream = std::ofstream;


    //////////////////////////////////////////
    template <typename>
    struct IsStdFunction : std::false_type {};
    template <class _Ty>
    struct IsStdFunction<std::function<_Ty>> : std::true_type {};


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeStdTypes_hpp_
//////////////////////////////////////////
