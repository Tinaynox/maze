//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021-2022 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
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
// |[Encoding]
// |    - Text file encoding is UTF8 with no BOM, using LF (unix) line endings.
// |    - 4-wide tab stops, using spaces only (no tab characters)
// |    - No trailing whitespace on lines, but always include a single newline at the end of the file.
// |
// |[Files]
// |    - Each engine file must have copyright text located at the very beginning of the file. The copyright text is given at the beginning of this file.
// |    - Maintain the style of surrounding code if it has its own separate standard.
// |    - Use PascalCasing for file names, typically matching the name of the dominant type in the file (or if none is dominant, use a reasonable category name).
// |
// |[Naming]
// |    - Spell words using correct US-English spelling.
// |    - Use descriptive and accurate names, even if it makes them longer. Favor readability over brevity.
// |    - Avoid abbreviations when possible unless the abbreviation is commonly accepted.
// |    - Use PascalCasing for the class and struct names.
// |    - Don't use the underline symbol '_' in the names of classes and structs.
// |    - Use camelCasing for the variable names.
// |    - Variables naming nomenclature:
// |        someName - struct members, local variables
// |        _someName - function arguments
// |        __someName - macro arguments
// |        ___someName - macro local variables
// |        m_someName - class members
// |        s_someName - static variables
// |        c_someName - constants
// |        g_someName - global variables
// |    - Use PascalCasing for the namespaces.
// |    - Use PascalCasing for the global function names.
// |    - Use camelCasing for the class member names.
// |    - Use semantically interesting names rather than language-specific keywords for type names (i.e. getLength > getInt).
// |    - Acronyms are PascalCasing, unless they are exactly two letters, in which case they are UPPERCASING. (ex. htmlText, GetCpuCycles(), IOStream)
// |    - Defines should use UPPERCASING
// |
// |[Spacing]
// |    - Space before opening parenthesis:
// |        - If it looks like a function call, no space (function calls, function definitions, typeof(), sizeof()).
// |        - If it opens a scope, add a space (if, while, catch, switch, for).
// |    - No spaces immediately inside any parens or brackets (e.g. no 'if ( foo )' or 'x = ( y * z[ 123 ] )').
// |    - Semicolon spacing as in English (e.g. 'for (int i = 0; i < 10; ++i)').
// |    - Exactly one space is required after the //.
// |    - Do not add a space between a unary operator and its operand (!expr, +30, -1.4f, i++, --j, &expr, *expr, (int)obj, etc.).
// |    - Do not add spaces around member access operators (a.b, a->b, etc.).
// |    - Spaces are required both before and after all other operators (math, assignment, comparison, etc.).
// |
// |[Comments]
// |    - Documenting the 'why' is far more important than the 'what' or 'how'.
// |    - Document anything that would surprise another engineer (or yourself in six months when you've forgotten it).
// |    - /**/-style comments are not permitted. They are reserved for commenting out big hunks of code locally (never to be committed).
// |    - Before the definition and declaration of each function and method, or the includes block,
// |      the "divider" comments line must be placed (Exact 42 slash characters):

//////////////////////////////////////////
#include "SomeFile.hpp"
#include "SomeOtherFile.hpp"


//////////////////////////////////////////
void SomeFun();

// |    - Such "diveders" must also be placed before the namespace opens and after it is closed:

//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace Special
    {
    } // namespace Special
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////

// |    - A comment with the name of this namespace is added after the closing bracket of the namespace.
// |
// |    - "Divider" before a class or a struct should be of the following form:

//////////////////////////////////////////
// Class ClassName
//
//////////////////////////////////////////
class ClassName
{
};


//////////////////////////////////////////
// Struct StructName
//
//////////////////////////////////////////
struct StructName
{
};


// |[Orders]
// |    - Function specifiers order:
// |    extern "C" MAZE_API_* static inline constexpr SomeFunction()
// |


//////////////////////////////////////////
#pragma once                                                                                  // | Header files should contain both pragma once preprocessor directive
#if (!defined(_CodeConvention_hpp_))                                                          // | and include guards block. Include guards block should be named
#define _CodeConvention_hpp_                                                                  // | exactly like current file name, but should be decorated by underline
                                                                                              // | symbols by both sides. Dots should be replaced by the underline symbol also.

//////////////////////////////////////////
#include "maze-core/math/MazeVec2D.hpp"                                                       // | Files from external modules are included first (Sort by modules).
#include "maze-core/math/MazeRect2D.hpp"
#include "maze-core/math/MazeQuaternion.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "SomeSpecialPool.hpp"                                                                // | Files from the current module are included last

                                                                                              // | The indent between different blocks should be two lines.
//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(SomeClass);
    MAZE_USING_SHARED_PTR(SomeAggregatedClass);
    
    
    //////////////////////////////////////////
    // Class SomeClass
    //
    //////////////////////////////////////////
    class MAZE_SOME_API SomeClass
        : public ParentClass                                                                  // | The inheritance list must start with a new line and go strictly to the column.
        , public OtherParentClass                                                             // | Colon and comma symbols should be placed one under the other
    {                                                                                         // | at the beginning of each row
    public:
                                                                                              // | The indent between access specifier and "divider" should be one line
        //////////////////////////////////////////                                            
        MAZE_DECLARE_METACLASS_WITH_PARENT(SomeClass, ParentClass);                           // | For the class meta information, friendship relations and static consts list
                                                                                              // | should be allocated a separated public section
        //////////////////////////////////////////
        static const F32 c_someConst;
        static const S32 c_currentIndex;
        
        //////////////////////////////////////////
        friend class FriendClass;

    public:
    
        //////////////////////////////////////////
        virtual ~SomeClass();
        
        //////////////////////////////////////////
        static SomeClassPtr Create();
       
       
        //////////////////////////////////////////
        virtual void execute() MAZE_OVERRIDE;                                                 // | Use MAZE_OVERRIDE and MAZE_ABSTRACT macros instead of 'override' and '= 0'
       
    protected:
    
        //////////////////////////////////////////
        SomeClass();

        //////////////////////////////////////////
        using ParentClass::init;
        
        //////////////////////////////////////////
        bool init();
        
    protected:
        SomeAggregatedClassPtr m_aggregatedField;
        SomeSpecialPoolPtr m_specialPool;
        Quaternion m_quaternion = Quaternion::c_identity;                                     // | The default value for fields in most cases should be
        Vec2DF m_position = Vec2DF::c_zero;                                                   // | set immediately upon declaration.
        Rect2DF m_rect = Rect2DF::c_zero;
        F32 m_someFloat = 0.1f;                                                               // | Avoid using .1f or 1f
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _GameLevelAdapter_hpp_                                                              // | And this is just a tradition. Maybe we'll get rid of it in the future.
//////////////////////////////////////////
