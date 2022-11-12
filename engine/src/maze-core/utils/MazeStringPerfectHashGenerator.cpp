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
#include "MazeCoreHeader.hpp"
#include "maze-core/utils/MazeStringPerfectHashGenerator.hpp"
#include "maze-core/hash/MazeHashSuperFast.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class StringPerfectHashGenerator
    //
    //////////////////////////////////////////
    StringPerfectHashGenerator::StringPerfectHashGenerator()
    {
        
    }

    //////////////////////////////////////////
    Size StringPerfectHashGenerator::stringToHash(String const& _string)
    {
        auto it = m_stringToHash.find(_string);
        if (it != m_stringToHash.end())
        {
            return it->second;
        }
        else
        {
            return generateNewHash(_string);
        }
    }

    //////////////////////////////////////////
    Size StringPerfectHashGenerator::generateNewHash(String const& _string)
    {
        Size hash = static_cast<Size>(Hash::CalculateSuperFastHash(_string));
        while (m_hashToString.find(hash) != m_hashToString.end())
            ++hash;

        m_hashToString.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(hash),
            std::forward_as_tuple(_string));

        m_stringToHash.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(_string),
            std::forward_as_tuple(hash));

        return hash;
    }

    //////////////////////////////////////////
    String const& StringPerfectHashGenerator::hashToString(Size _hash)
    {
        static String nullString;

        auto it = m_hashToString.find(_hash);
        if (it != m_hashToString.end())
            return it->second;

        return nullString;
    }

} // namespace Maze
//////////////////////////////////////////
