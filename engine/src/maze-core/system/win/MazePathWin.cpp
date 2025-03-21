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
#include "maze-core/system/win/MazePathWin.hpp"
#include "maze-core/math/MazeMath.hpp"
#include "maze-core/helpers/win/MazeTextHelperWin.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    PathWin::PathWin(String const& _value)
    {
        m_path = TextHelper::ConvertUTF8ToUCS2(_value.c_str());
    }

    //////////////////////////////////////////
    PathWin::PathWin(Char const* _value)
    {
        if (_value)
            m_path = TextHelper::ConvertUTF8ToUCS2(_value);
        else
            m_path.clear();
    }

    //////////////////////////////////////////
    PathWin::PathWin(Char _value)
    {
        m_path = _value;
    }

    //////////////////////////////////////////
    PathWin::operator String() const
    {
        return TextHelper::ConvertUCS2ToUTF8(m_path.c_str());
    }

    //////////////////////////////////////////
    PathWin::operator WString() const
    {
        return m_path;
    }

    //////////////////////////////////////////
    String PathWin::toUTF8() const
    {
        return TextHelper::ConvertUCS2ToUTF8(m_path.c_str());
    }

} // namespace Maze
//////////////////////////////////////////
