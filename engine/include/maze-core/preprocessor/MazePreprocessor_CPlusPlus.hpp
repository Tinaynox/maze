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
#if (!defined(_MazePreprocessor_CPlusPlus_hpp_))
#define _MazePreprocessor_CPlusPlus_hpp_


//////////////////////////////////////////
#include "MazePreprocessor_Platform.hpp"
#include "maze-core/MazeStdTypes.hpp"
#include <csignal>


//////////////////////////////////////////
#define MAZE_CPP_STANDARD_CPP98      (1)
#define MAZE_CPP_STANDARD_CPP03      (2)
#define MAZE_CPP_STANDARD_CPP11      (3)
#define MAZE_CPP_STANDARD_CPP14      (4)
#define MAZE_CPP_STANDARD_CPP17      (5)
#define MAZE_CPP_STANDARD_CPP20      (6)

#if (__cplusplus > 201703L)
#    pragma error "Error! Unknown CPP standart!"
#elif (__cplusplus == 201703L)
#    define MAZE_CPP_STANDARD         MAZE_CPP_STANDARD_CPP17
#    define MAZE_CPP_STANDARD_NAME    "C++17"
#elif (__cplusplus == 201402L)
#    define MAZE_CPP_STANDARD         MAZE_CPP_STANDARD_CPP14
#    define MAZE_CPP_STANDARD_NAME    "C++14"
#elif (__cplusplus == 201103L) || (__cplusplus > 199711L) || \
    (defined(_MSC_VER) && _MSC_VER >= 1800) || \
    (defined(__GNUC__) && \
      (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ >= 40700))
#    define MAZE_CPP_STANDARD         MAZE_CPP_STANDARD_CPP11
#    define MAZE_CPP_STANDARD_NAME    "C++11"
#else
#   define MAZE_CPP_STANDARD         MAZE_CPP_STANDARD_CPP03
#    define MAZE_CPP_STANDARD_NAME    "C++03"
#endif


//////////////////////////////////////////
// Define the macroses for C++ keywords
//
//////////////////////////////////////////
#define MAZE_OVERRIDE                                        override
#define MAZE_FINAL                                           final
#define MAZE_ABSTRACT                                        = 0
#define MAZE_STATIC_ASSERT(__expression, __message)          static_assert(__expression, __message)
#if __cpp_constexpr
#    define MAZE_CONSTEXPR                                   constexpr 
#else
#    define MAZE_CONSTEXPR
#endif
#if (__cplusplus >= 201402L)
#    define MAZE_CONSTEXPR14                                 MAZE_CONSTEXPR
#else
#    define MAZE_CONSTEXPR14
#endif

//////////////////////////////////////////
#define MAZE_EXPLICIT_TEMPLATE_SPECIALIZATION                template <>



//////////////////////////////////////////
// Smart pointers

//////////////////////////////////////////
// Aliases
//
//////////////////////////////////////////
#define MAZE_USING_SHARED_PTR(__DClass)                     using __DClass##Ptr             = Maze::SharedPtr<class __DClass>; \
                                                            using __DClass##ConstPtr        = Maze::SharedPtr<class __DClass const>; \
                                                            using __DClass##WPtr            = Maze::WeakPtr<class __DClass>; \
                                                            using __DClass##ConstWPtr       = Maze::WeakPtr<class __DClass const>;


//////////////////////////////////////////
// OS functions
//
//////////////////////////////////////////

//////////////////////////////////////////
// Sprintf
//
//////////////////////////////////////////
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#   define MAZE_SPRINTF sprintf_s
#   define MAZE_SWPRINTF swprintf_s
#   define MAZE_SNPRINTF _snprintf
#   define MAZE_SNWPRINTF _snwprintf
#   define MAZE_VSNPRINTF(dstbuf, maxcount, format, arg) _vsnprintf_s(dstbuf, maxcount, maxcount, format, arg)
#   define MAZE_VSNWPRINTF(dstbuf, maxcount, format, arg) _vsnwprintf_s(dstbuf, maxcount, maxcount, format, arg)
#   define MAZE_VSCPRINTF(format, arg) _vscprintf(format, arg)
#   define MAZE_VSCWPRINTF(format, arg) _vscwprintf(format, arg)
#   define MAZE_SCPRINTF(format, arg) _scprintf(format, arg)
#   define MAZE_SCWPRINTF(format, arg) _scwprintf(format, arg)
#   define MAZE_STRCMPI _strcmpi
#   define MAZE_STRNICMP _strnicmp
#elif (MAZE_PLATFORM_UNIX_FAMILY)
#   define MAZE_SPRINTF sprintf
#   define MAZE_SWPRINTF swprintf
#   define MAZE_SNPRINTF snprintf
#   define MAZE_SNWPRINTF snwprintf
#   define MAZE_VSNPRINTF(dstbuf, maxcount, format, arg) vsnprintf(dstbuf, maxcount, format, arg)
#   define MAZE_VSNWPRINTF(dstbuf, maxcount, format, arg) vswprintf(dstbuf, maxcount, format, arg)
#   define MAZE_VSCPRINTF(format, arg) MAZE_vscprintf(format, arg)
#   define MAZE_VSCWPRINTF(format, arg) MAZE_vscwprintf(format, arg)
#   define MAZE_SCPRINTF(format, arg) MAZE_scprintf(format, arg)
#   define MAZE_SCWPRINTF(format, arg) MAZE_scwprintf(format, arg)
#   define MAZE_STRCMPI strcasecmp
#   define MAZE_STRNICMP strncmp
#else
#   define MAZE_SPRINTF sprintf
#   define MAZE_SWPRINTF swprintf
#   define MAZE_SNPRINTF snprintf
#   define MAZE_SNWPRINTF snwprintf
#   define MAZE_VSNPRINTF(dstbuf, maxcount, format, arg) vsnprintf(dstbuf, maxcount, format, arg)
#   define MAZE_VSNWPRINTF(dstbuf, maxcount, format, arg) vswprintf(dstbuf, maxcount, format, arg)
#   define MAZE_VSCPRINTF(format, arg) vscprintf(format, arg)
#   define MAZE_VSCWPRINTF(format, arg) vscwprintf(format, arg)
#   define MAZE_SCPRINTF(format, arg) scprintf(format, arg)
#   define MAZE_SCWPRINTF(format, arg) scwprintf(format, arg)
#   define MAZE_STRCMPI strcasecmp
#   define MAZE_STRNICMP strncmp
#endif


//////////////////////////////////////////
// Math
//
//////////////////////////////////////////
#if ((MAZE_PLATFORM == MAZE_PLATFORM_OSX) || (MAZE_PLATFORM == MAZE_PLATFORM_IOS))
#   define MAZE_ISNAN isnan
#else
#   define MAZE_ISNAN _isnan
#endif



//////////////////////////////////////////
// Debugger

//////////////////////////////////////////
// Break if
//
//////////////////////////////////////////

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#    if (MAZE_ARCH == MAZE_ARCH_X64)
#        include <crtdbg.h>
#        if (MAZE_DEBUG)
#            define MAZE_BP_IF(__condition) if (__condition) { if (IsDebuggerPresent()) { _CrtDbgBreak(); } }
#        else
#            define MAZE_BP_IF(__condition) if (__condition) { __debugbreak(); }
#        endif
#    elif (MAZE_ARCH == MAZE_ARCH_X86)
#        define MAZE_BP_IF(__condition) if (__condition) { __asm { int 3 } }
#    else
#        define MAZE_BP_IF(__condition)
#    endif
#elif ((MAZE_PLATFORM == MAZE_PLATFORM_IOS) || (MAZE_PLATFORM == MAZE_PLATFORM_OSX))
#    if (TARGET_OS_IPHONE)
#       define MAZE_BP_IF(__condition) if (__condition) { raise(SIGINT); }
#    elif (defined(__i386__) || defined(__x86_64__))
#        define MAZE_BP_IF(__condition) if (__condition) { asm("int $0x3"); }
#    else
#        define MAZE_BP_IF(__condition) assert(!(__condition));
#    endif
#elif (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)
#        define MAZE_BP_IF(__condition)
#elif (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)
#    if (defined(__i386__) || defined(__x86_64__))
#        define MAZE_BP_IF(__condition) if (__condition) { asm("int $3"); }
#    else
#        include <cassert>
#        define MAZE_BP_IF(__condition) assert(!(__condition));
#    endif
#elif (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN)
#    define MAZE_BP_IF(__condition)
#endif


//////////////////////////////////////////
// Break
//
//////////////////////////////////////////
#define MAZE_BP MAZE_BP_IF(true)


//////////////////////////////////////////
// Not implemented
//
//////////////////////////////////////////
#define MAZE_TODO MAZE_DEBUG_BP


//////////////////////////////////////////
// Return
//
//////////////////////////////////////////
#define MAZE_RETURN_IF(__condition)                       if (__condition) return;

//////////////////////////////////////////
#define MAZE_RETURN_VALUE_IF(__condition, __value)        if (__condition) return __value;

//////////////////////////////////////////
#define MAZE_BP_RETURN_IF(__condition)                    MAZE_BP_IF(__condition); if (__condition) return;

//////////////////////////////////////////
#define MAZE_BP_RETURN                                    MAZE_BP_RETURN_IF(true);

//////////////////////////////////////////
#define MAZE_BP_RETURN_VALUE_IF(__condition, __value)     MAZE_BP_IF(__condition); if (__condition) return __value;

//////////////////////////////////////////
#define MAZE_BP_RETURN_VALUE(__value)                     MAZE_BP_RETURN_VALUE_IF(true, __value);

//////////////////////////////////////////
#define MAZE_BP_CONTINUE_IF(__condition)                  MAZE_BP_IF(__condition); if (__condition) continue;

//////////////////////////////////////////
#define MAZE_BP_CONTINUE                                  MAZE_BP_CONTINUE_IF(true);


//////////////////////////////////////////
#if (MAZE_DEBUG)
#   define MAZE_DEBUG_BP_IF(__condition)                            MAZE_BP_IF(__condition)
#   define MAZE_DEBUG_BP                                            MAZE_BP
#   define MAZE_DEBUG_BP_RETURN_IF(__condition)                     MAZE_BP_RETURN_IF(__condition)
#   define MAZE_DEBUG_BP_RETURN                                     MAZE_BP_RETURN
#   define MAZE_DEBUG_BP_RETURN_VALUE_IF(__condition, __value)      MAZE_BP_RETURN_VALUE_IF(__condition, __value)
#   define MAZE_DEBUG_BP_RETURN_VALUE(__value)                      MAZE_BP_RETURN_VALUE(__value)
#   define MAZE_DEBUG_BP_CONTINUE_IF(__condition)                   MAZE_BP_CONTINUE_IF(__condition)
#   define MAZE_DEBUG_BP_CONTINUE                                   MAZE_BP_CONTINUE
#else
#   define MAZE_DEBUG_BP_IF(__condition)
#   define MAZE_DEBUG_BP
#   define MAZE_DEBUG_BP_RETURN_IF(__condition)
#   define MAZE_DEBUG_BP_RETURN
#   define MAZE_DEBUG_BP_RETURN_VALUE_IF(__condition, __value)
#   define MAZE_DEBUG_BP_RETURN_VALUE(__value)
#   define MAZE_DEBUG_BP_CONTINUE_IF(__condition)
#   define MAZE_DEBUG_BP_CONTINUE
#endif


//////////////////////////////////////////
// Code macro
//
//////////////////////////////////////////
#if (MAZE_COMPILER == MAZE_COMPILER_GNUC)
#   define MAZE_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif (MAZE_COMPILER == MAZE_COMPILER_CLANG)
#   define MAZE_PRETTY_FUNCTION __PRETTY_FUNCTION__
#else
#   define MAZE_PRETTY_FUNCTION __FUNCTION__
#endif

#define MAZE_RESTRICT_ALIASING (0)

#if (MAZE_RESTRICT_ALIASING != 0)
#   if MAZE_COMPILER == MAZE_COMPILER_MSVC
#       define MAZE_RESTRICT_ALIAS __restrict
#       define MAZE_RESTRICT_ALIAS_RETURN __restrict
#   else
#       define MAZE_RESTRICT_ALIAS __restrict__
#       define MAZE_RESTRICT_ALIAS_RETURN
#endif
#else
#   define MAZE_RESTRICT_ALIAS
#   define MAZE_RESTRICT_ALIAS_RETURN
#endif


//////////////////////////////////////////
// Unused
#define MAZE_UNUSED(__expr) do { (void)(__expr); } while (0);


//////////////////////////////////////////
// Check memory
//
//////////////////////////////////////////
#if (MAZE_DEBUG)
#   if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#       define    MAZE_CHECK_MEMORY MAZE_BP_IF(_CrtCheckMemory() == 0)
#   else
#       define MAZE_CHECK_MEMORY
#   endif
#else
#   define MAZE_CHECK_MEMORY
#endif


//////////////////////////////////////////
// Assert
//
//////////////////////////////////////////
#if (MAZE_DEBUG)
#    define MAZE_ASSERT(__condition)        assert(__condition)
#else
#    define MAZE_ASSERT(__condition)
#endif


//////////////////////////////////////////
// Apply OS suffix
//
//////////////////////////////////////////
#define MAZE_PLATFORM_OBJECT(__name) MAZE_MACRO_COMBINE(__name, MAZE_PLATFORM_SUFFIX_CAMEL)

//////////////////////////////////////////
// OS file name
//
//////////////////////////////////////////
#define MAZE_PLATFORM_FILE(__file) MAZE_MACRO_SPLIT_POINT_STR(MAZE_MACRO_SPLIT_SLASH(MAZE_PLATFORM_SUFFIX_KEBAB, MAZE_PLATFORM_OBJECT(__file)), hpp)

//////////////////////////////////////////
#define MAZE_INCLUDE_OS_FILE(__path, __file) MAZE_MACRO_SPLIT_POINT_STR(MAZE_MACRO_SPLIT_SLASH3(__path, MAZE_PLATFORM_SUFFIX_KEBAB, MAZE_PLATFORM_OBJECT(__file)), hpp)


//////////////////////////////////////////
// Apply CPU suffix
//
//////////////////////////////////////////
#define MAZE_ARCH_OBJECT(__name) MAZE_MACRO_COMBINE(__name##_, MAZE_ARCH_SUFFIX)

//////////////////////////////////////////
// OS file name
//
//////////////////////////////////////////
#define MAZE_ARCH_FILE(__path, __file) MAZE_MACRO_SPLIT_POINT_STR(MAZE_MACRO_SPLIT_SLASH3(__path, MAZE_ARCH_SUFFIX, MAZE_ARCH_OBJECT(__file)), h)


#endif // _MazePreprocessor_CPlusPlus_hpp_
//////////////////////////////////////////
