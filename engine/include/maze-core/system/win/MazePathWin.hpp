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
#if (!defined(_MazePathWin_hpp_))
#define _MazePathWin_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeGenericPath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PathWin
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PathWin : public GenericPath<PathWin, WString>
    {
    public:

        //////////////////////////////////////////
        using ParentClass = GenericPath<PathWin, WString>;

    public:

        //////////////////////////////////////////
        inline PathWin() : ParentClass() {}

        //////////////////////////////////////////
        inline PathWin(PathWin const& _value) : ParentClass(_value) {}

        //////////////////////////////////////////
        inline PathWin(PathWin&& _value) noexcept : ParentClass(std::move(_value)) {}

        //////////////////////////////////////////
        inline PathWin(WString const& _path) : ParentClass(_path) {}

        //////////////////////////////////////////
        inline PathWin(WString&& _path) : ParentClass(std::move(_path)) {}

        //////////////////////////////////////////
        inline PathWin(ParentClass::CharType const* _path) : ParentClass(_path) {}

        //////////////////////////////////////////
        inline PathWin(ParentClass::CharType _char) : ParentClass(_char) {}

        //////////////////////////////////////////
        PathWin(String const& _value);

        //////////////////////////////////////////
        PathWin(Char const* _value);

        //////////////////////////////////////////
        PathWin(Char _value);


        //////////////////////////////////////////
        inline PathWin& operator=(PathWin const& _value) { return ParentClass::operator=(_value); }

        //////////////////////////////////////////
        inline PathWin& operator=(PathWin&& _value) noexcept { return ParentClass::operator=(std::move(_value)); }


        //////////////////////////////////////////
        operator String() const;


        //////////////////////////////////////////
        virtual String toUTF8() const MAZE_OVERRIDE;
    };
    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazePathWin_hpp_
//////////////////////////////////////////
