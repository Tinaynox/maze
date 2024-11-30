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
#if (!defined(_MazeIdWithGeneration_hpp_))
#define _MazeIdWithGeneration_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/system/MazeMutex.hpp"
#include "maze-core/helpers/MazeLogHelper.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{

    //////////////////////////////////////////
    // Class IdWithGeneration
    //
    //////////////////////////////////////////
    template <typename TIdType = S32, S8 TGenerationBitsCount = 8>
    class IdWithGeneration
    {
    public:

        //////////////////////////////////////////
        static const MAZE_CONSTEXPR TIdType c_maxGenerationValue = ((1 << TGenerationBitsCount) - 1);

        //////////////////////////////////////////
        MAZE_FORCEINLINE IdWithGeneration() {}

        //////////////////////////////////////////
        MAZE_FORCEINLINE IdWithGeneration(TIdType _index, TIdType _generation)
        {
            set(_index, _generation);
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE IdWithGeneration(IdWithGeneration const& _value)
            : m_id(_value)
        {}

        //////////////////////////////////////////
        MAZE_FORCEINLINE IdWithGeneration(IdWithGeneration&& _value)
            : m_id(std::move(_value))
        {}

        //////////////////////////////////////////
        MAZE_FORCEINLINE explicit IdWithGeneration(TIdType _id)
            : m_id(_id)
        {}

        //////////////////////////////////////////
        MAZE_FORCEINLINE void operator=(IdWithGeneration const& _value)
        {
            m_id = _value.m_id;
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE void operator=(IdWithGeneration&& _value)
        {
            m_id = std::move(_value.m_id);
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE bool operator==(IdWithGeneration const& _value) const
        {
            return m_id == _value.m_id;
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE bool operator!=(IdWithGeneration const& _value) const
        {
            return m_id != _value.m_id;
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE TIdType set(TIdType _index, TIdType _generation)
        {
            MAZE_DEBUG_ASSERT(_generation <= c_maxGenerationValue);
            m_id = (_index << TGenerationBitsCount) | static_cast<TIdType>(_generation);
            return m_id;
        }

        //////////////////////////////////////////
        MAZE_FORCEINLINE TIdType getId() const { return m_id; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE operator TIdType() const { return m_id; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE TIdType getIndex() const { return m_id >> TGenerationBitsCount; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE TIdType getGeneration() const { return m_id & c_maxGenerationValue; }

        //////////////////////////////////////////
        MAZE_FORCEINLINE TIdType incrementGeneration()
        {
            TIdType generation = getGeneration();
            if (generation != c_maxGenerationValue)
                return set(getIndex(), generation + 1);
            else
                return set(getIndex(), 0);
        }

    protected:
        TIdType m_id = 0;
    };


} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeIdWithGeneration_hpp_
