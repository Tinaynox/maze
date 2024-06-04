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
#if (!defined(_MazeSpan_hpp_))
#define _MazeSpan_hpp_s


//////////////////////////////////////////
// Include
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/preprocessor/MazePreprocessor_CPlusPlus.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    template <typename TValue>
    class Span;
    template <typename TValue>
    using ConstSpan = Span<TValue const>;


    //////////////////////////////////////////
    // Class Span
    //
    //////////////////////////////////////////
    template <typename TValue>
    class Span MAZE_FINAL   
    {
    public:

        //////////////////////////////////////////
        inline Span() = default;

        //////////////////////////////////////////
        inline Span(TValue* _ptr, Size _size)
            : m_ptr(_ptr)
            , m_size(_size)
        {}

        //////////////////////////////////////////
        inline ~Span() = default;

        //////////////////////////////////////////
        inline Span(Span const&) = default;

        //////////////////////////////////////////
        inline Span(Span&&) = default;

        //////////////////////////////////////////
        inline Span& operator=(Span const&) = default;

        //////////////////////////////////////////
        inline Span& operator=(Span&&) = default;


        //////////////////////////////////////////
        inline void set(TValue* _ptr, Size _size)
        {
            m_ptr = _ptr;
            m_size = _size;
        }

        //////////////////////////////////////////
        inline void setSize(Size _size) { m_size = _size; }

        //////////////////////////////////////////
        inline TValue* getPtr() const { return m_ptr; }

        //////////////////////////////////////////
        inline Size getSize() const { return m_size; }


        //////////////////////////////////////////
        MAZE_FORCEINLINE TValue const& operator[](Size _i) const
        {
            MAZE_DEBUG_ASSERT(_i < m_size);
            return m_ptr[_i];
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE TValue& operator[](Size _i)
        {
            MAZE_DEBUG_ASSERT(_i < m_size);
            return m_ptr[_i];
        }

        //////////////////////////////////////////
        inline TValue const& at(Size _i) const
        {
            MAZE_DEBUG_ASSERT(_i < m_size);
            return m_ptr[_i];
        }

        //////////////////////////////////////////
        inline TValue& at(Size _i)
        {
            MAZE_DEBUG_ASSERT(_i < m_size);
            return m_ptr[_i];
        }

    private:
        TValue* m_ptr = nullptr;
        Size m_size = 0;
    };

} // namespace Maze
//////////////////////////////////////////


#endif // _MazeSpan_h_
//////////////////////////////////////////
