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
#include "maze-core/math/MazeVec3B.hpp"


//////////////////////////////////////////
namespace Maze
{


    //////////////////////////////////////////
    // Class Vec3B
    //
    //////////////////////////////////////////
    Vec3B const Vec3B::c_false = { false, false, false };
    Vec3B const Vec3B::c_true = { true, true, true };

    //////////////////////////////////////////
    String Vec3B::toString(Char _separator) const
    {
        return StringHelper::ToString(x) + _separator + StringHelper::ToString(y) + _separator + StringHelper::ToString(z);
    }

    //////////////////////////////////////////
    CString Vec3B::ParseString(CString _string, Size _size, Vec3B& _result, Char _separator)
    {
        CString end = _string + _size;
        _string = StringHelper::ParseBool(_string, end, _result.x);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseBool(_string, end, _result.y);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseBool(_string, end, _result.z);
        return _string;
    }

    //////////////////////////////////////////
    CString Vec3B::ParseStringPretty(CString _string, Size _size, Vec3B& _result, Char _separator)
    {
        CString end = _string + _size;
        _string = StringHelper::ParseBoolPretty(_string, end, _result.x);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseBoolPretty(_string, end, _result.y);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ExpectSkipChar(_string, end, _separator);
        _string = StringHelper::SkipChar(_string, end, ' ');
        _string = StringHelper::ParseBoolPretty(_string, end, _result.z);
        return _string;
    }

    //////////////////////////////////////////
    Vec3B Vec3B::FromString(CString _string, Size _size, Char _separator)
    {
        Vec3B result = Vec3B(false);
        ParseString(_string, _size, result, _separator);
        return result;
    }

    //////////////////////////////////////////
    Vec3B Vec3B::FromString(String const& _string, Char _separator)
    {
        return FromString(&_string[0], _string.size(), _separator);
    }
    

} // namespace Maze
//////////////////////////////////////////
