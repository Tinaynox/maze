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
#if (!defined(_MazeStdHelper_hpp_))
#define _MazeStdHelper_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/utils/MazeClassInfo.hpp"
#include "maze-core/system/MazePath.hpp"
#include <utility>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    namespace StdHelper
    {
        //////////////////////////////////////////
        MAZE_CORE_API extern FILE* OpenFile(Path const& _fullPath, Path const& _mode);

        //////////////////////////////////////////
        template <typename T>
        typename T::iterator EraseUnorderedVector(T& _container, typename T::iterator& _it)
        {
            const size_t idx = _it - _container.begin();
            *_it = _container.back();
            _container.pop_back();

            return _container.begin() + idx;
        }

        //////////////////////////////////////////
        template <typename UValue>
        inline ClassUID GetVectorElementUID(Vector<UValue> const&)
        {
            static ClassUID const s_elementUID = ClassInfo<UValue>::UID();
            return s_elementUID;
        }
        

        //////////////////////////////////////////
        template <typename TValue, typename = int>
        struct HasDefaultConstructor : std::false_type {};

        //////////////////////////////////////////
        template <typename TValue>
        struct HasDefaultConstructor<TValue, decltype(TValue(), 0)> : std::true_type {};


        //////////////////////////////////////////
        template <typename TValue>
        struct HasOperatorEquals
        {
            template <typename UValue>
            static auto test(UValue const* _p) -> decltype(*_p == *_p, std::true_type());

            template <typename>
            static auto test(...) -> std::false_type;

            static MAZE_CONSTEXPR bool value = decltype(test<TValue>(nullptr))::value;
        };

    } // namespace StdHelper
    //////////////////////////////////////////
    
    
} // namespace Maze
//////////////////////////////////////////


#endif // _MazeStdHelper_hpp_
//////////////////////////////////////////
