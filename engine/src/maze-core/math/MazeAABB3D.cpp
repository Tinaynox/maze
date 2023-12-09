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
#include "maze-core/math/MazeAABB3D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class AABB3D
    //
    //////////////////////////////////////////
    AABB3D const AABB3D::c_zero = AABB3D();

    //////////////////////////////////////////
    String AABB3D::toString(Char _separator) const
    {
        return StringHelper::ToString(m_min.x) + _separator +
               StringHelper::ToString(m_min.y) + _separator +
               StringHelper::ToString(m_min.z) + _separator +
               StringHelper::ToString(m_max.x) + _separator +
               StringHelper::ToString(m_max.y) + _separator +
               StringHelper::ToString(m_max.z);
    }

    //////////////////////////////////////////
    CString AABB3D::ParseString(CString _string, Size _size, AABB3D& _result, Char _separator)
    {
        CString end = _string + _size;

        _string = StringHelper::ParseF32(_string, end, _result.m_min.x);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseF32(_string, end, _result.m_min.y);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseF32(_string, end, _result.m_min.z);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseF32(_string, end, _result.m_max.x);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseF32(_string, end, _result.m_max.y);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseF32(_string, end, _result.m_max.z);
        return _string;
    }

    //////////////////////////////////////////
    AABB3D AABB3D::FromString(CString _string, Size _size, Char _separator)
    {
        AABB3D result = AABB3D::c_zero;
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    AABB3D AABB3D::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }
    

} // namespace Maze
//////////////////////////////////////////
