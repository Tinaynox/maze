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
// subject to the folloUnixg restrictions:
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
#if (!defined(_MazePathUnix_hpp_))
#define _MazePathUnix_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"
#include "maze-core/system/MazeGenericPath.hpp"
#include "maze-core/serialization/MazeStringSerializable.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class PathUnix
    //
    //////////////////////////////////////////
    class MAZE_CORE_API PathUnix
        : public GenericPath<PathUnix, String>
        , public IStringSerializable
    {
    public:

        //////////////////////////////////////////
        using ParentClass = GenericPath<PathUnix, String>;

    public:

        //////////////////////////////////////////
        inline PathUnix() : ParentClass() {}

        //////////////////////////////////////////
        inline PathUnix(PathUnix const& _value) : ParentClass(_value) {}

        //////////////////////////////////////////
        inline PathUnix(PathUnix&& _value) : ParentClass(std::move(_value)) {}

        //////////////////////////////////////////
        inline PathUnix(String const& _path) : ParentClass(_path) {}

        //////////////////////////////////////////
        inline PathUnix(String&& _path) : ParentClass(std::move(_path)) {}

        //////////////////////////////////////////
        inline PathUnix(ParentClass::CharType const* _path) : ParentClass(_path) {}

        //////////////////////////////////////////
        inline PathUnix(ParentClass::CharType _char) : ParentClass(_char) {}


        //////////////////////////////////////////
        inline PathUnix& operator=(PathUnix const& _value) { return ParentClass::operator=(_value); }

        //////////////////////////////////////////
        inline PathUnix& operator=(PathUnix&& _value) { return ParentClass::operator=(std::move(_value)); }


        //////////////////////////////////////////
        virtual String toUTF8() const MAZE_OVERRIDE;
       
    public:

        //////////////////////////////////////////
        virtual String toString() const MAZE_OVERRIDE { return toUTF8(); }

        //////////////////////////////////////////
        virtual void setString(CString _data, Size _count) MAZE_OVERRIDE { (*this) = PathUnix(String(_data, _count)); }
    };
    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazePathUnix_hpp_
//////////////////////////////////////////
