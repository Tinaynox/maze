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
#if (!defined(_MazeClassInfo_hpp_))
#define _MazeClassInfo_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/hash/MazeHashCRC.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include <cstring>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    using ClassUID = U32;
    

    //////////////////////////////////////////
    class ClassInfoParser 
    {
    public:

        //////////////////////////////////////////
        static inline StdString QualifiedName(CString _str)
        {
#if (MAZE_COMPILER == MAZE_COMPILER_GNUC)
            StdString name = _str;

            Size eq = name.find_first_of("=");

            if (eq != StdString::npos) 
            {
                eq += 2;
                Size end = name.find_first_of(";", eq);
                return name.substr(eq, end - eq);
            }
#elif (MAZE_COMPILER == MAZE_COMPILER_CLANG)
            StdString name = _str;
            
            Size eq = name.find_first_of("=");
            
            if (eq != StdString::npos)
            {
                eq += 2;
                Size end = name.find_first_of("]", eq);
                return name.substr(eq, end - eq);
            }
#else
            StdString name = _str;
#endif 

            Char const* class_ = "class ";
            Char const* struct_ = "struct ";

            // Get the template argument
            Size start = name.find_first_of("<");
            Size end = name.find_last_of(">");
            name = name.substr(start + 1, end - start - 1);

            // Remove the 'class ' from type string.
            Size pos = name.find(class_);
            while (pos != StdString::npos)
            {
                pos = name.replace(pos, strlen(class_), "").find(class_);
            }

            // Remove the 'struct ' from type string.
            pos = name.find(struct_);
            while (pos != StdString::npos) 
            {
                pos = name.replace(pos, strlen(struct_), "").find(struct_);
            }

            // Remove spaces
            pos = name.find(' ');
            while (pos != StdString::npos) 
            {
                pos = name.replace(pos, 1, "").find(' ');
            }

            return name;
        }

        //////////////////////////////////////////
        static inline StdString Name(CString _str)
        {
            StdString name = QualifiedName(_str);

            Size start = name.find_last_of(":");
            if (start != StdString::npos) 
            {
                name = name.substr(start + 1);
            }

            return name;
        }
    };
    
    //////////////////////////////////////////
    static inline ClassUID GetClassUIDByQualifiedName(CString _name)
    {
        return Hash::CalculateCRC32(_name, strlen(_name));
    }

    //////////////////////////////////////////
    template <typename T>
    class ClassInfo 
    {
    public:

        //////////////////////////////////////////
        static inline CString QualifiedName()
        {
            static StdString value = ClassInfoParser::QualifiedName(MAZE_PRETTY_FUNCTION);
            return value.c_str();
        }

        //////////////////////////////////////////
        static inline CString Name()
        {
            static StdString value = ClassInfoParser::Name(MAZE_PRETTY_FUNCTION);
            return value.c_str();
        }

        //////////////////////////////////////////
        static inline ClassUID UID()
        {
            static ClassUID classUID = GenerateUID(QualifiedName());
            return classUID;
        }

    protected:

        //////////////////////////////////////////
        static inline ClassUID GenerateUID(Char const* _name)
        {
            static StdMap<ClassUID, Char const*> s_registeredClasses;

            ClassUID classUID = Hash::CalculateCRC32(_name, strlen(_name));
            if (s_registeredClasses.find(classUID) != s_registeredClasses.end())
            {
                return GenerateUID((StdString(_name) + '_').c_str());
            }
            
            s_registeredClasses[classUID] = _name;
            return classUID;
        }
    };

    
} // namespace Maze
//////////////////////////////////////////


//////////////////////////////////////////
#endif // _MazeClassInfo_hpp_
