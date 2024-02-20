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
#if (!defined(_MazePreprocessor_Platform_hpp_))
#define _MazePreprocessor_Platform_hpp_


//////////////////////////////////////////
#include "MazePreprocessor_Macro.hpp"


//////////////////////////////////////////
// Analize Operation System
//
//////////////////////////////////////////
#define MAZE_PLATFORM_WINDOWS          (1)
#define MAZE_PLATFORM_OSX              (2)
#define MAZE_PLATFORM_IOS              (3)
#define MAZE_PLATFORM_LINUX            (4)
#define MAZE_PLATFORM_SYMBIAN          (5)
#define MAZE_PLATFORM_ANDROID          (6)
#define MAZE_PLATFORM_EMSCRIPTEN       (7)


//////////////////////////////////////////
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
#   define MAZE_PLATFORM                    MAZE_PLATFORM_WINDOWS
#   define MAZE_PLATFORM_SUFFIX_KEBAB       win
#   define MAZE_PLATFORM_SUFFIX_CAMEL       Win
#elif (defined(__APPLE__))
#   include "TargetConditionals.h"
#   if (TARGET_IPHONE_SIMULATOR)
    // iOS Simulator
#       define MAZE_PLATFORM                MAZE_PLATFORM_IOS
#       define MAZE_PLATFORM_IOS_SIMULATOR  (1)
#   define MAZE_PLATFORM_SUFFIX_KEBAB       ios
#   define MAZE_PLATFORM_SUFFIX_CAMEL       IOS
#   elif (TARGET_OS_IPHONE)
#       define MAZE_PLATFORM                MAZE_PLATFORM_IOS
#   define MAZE_PLATFORM_SUFFIX_KEBAB       ios
#   define MAZE_PLATFORM_SUFFIX_CAMEL       IOS
    // iOS device
#   elif (TARGET_OS_MAC)
    // Mac OS
#       define MAZE_PLATFORM                MAZE_PLATFORM_OSX
#   define MAZE_PLATFORM_SUFFIX_KEBAB       osx
#   define MAZE_PLATFORM_SUFFIX_CAMEL       OSX
#   else
    // Unsupported platform
#   endif
#elif (defined(__ANDROID__))
#   define MAZE_PLATFORM                    MAZE_PLATFORM_ANDROID
#   define MAZE_PLATFORM_SUFFIX_KEBAB       android
#   define MAZE_PLATFORM_SUFFIX_CAMEL       Android
#elif (defined(__linux) || defined(__linux__))
#   define MAZE_PLATFORM                    MAZE_PLATFORM_LINUX
#   ifdef linux
#       undef linux
#   endif
#   define MAZE_PLATFORM_SUFFIX_KEBAB       linux
#   define MAZE_PLATFORM_SUFFIX_CAMEL       Linux
#elif (defined(__EMSCRIPTEN__))
#   define MAZE_PLATFORM                    MAZE_PLATFORM_EMSCRIPTEN
#   define MAZE_PLATFORM_SUFFIX_KEBAB       emscripten
#   define MAZE_PLATFORM_SUFFIX_CAMEL       Emscripten
#elif (defined(__unix) || defined(__unix__))
#   define MAZE_PLATFORM                    MAZE_PLATFORM_UNIX
#   define MAZE_PLATFORM_SUFFIX_KEBAB       unix
#   define MAZE_PLATFORM_SUFFIX_CAMEL       Unix
#elif (defined(__posix))
#   define MAZE_PLATFORM                    MAZE_PLATFORM_POSIX
#   define MAZE_PLATFORM_SUFFIX_KEBAB       posix
#   define MAZE_PLATFORM_SUFFIX_CAMEL       Posix
#else
#    pragma error "Error! Unidentified operation system!"
#endif


//////////////////////////////////////////
#if ((MAZE_PLATFORM == MAZE_PLATFORM_OSX)             || \
     (MAZE_PLATFORM == MAZE_PLATFORM_IOS)             || \
     (MAZE_PLATFORM == MAZE_PLATFORM_LINUX)           || \
     (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID)         || \
     (MAZE_PLATFORM == MAZE_PLATFORM_EMSCRIPTEN))
#    define MAZE_PLATFORM_UNIX_FAMILY (1)
#endif

//////////////////////////////////////////
#if ((MAZE_PLATFORM == MAZE_PLATFORM_IOS)             || \
     (MAZE_PLATFORM == MAZE_PLATFORM_ANDROID))
#   define MAZE_PLATFORM_MOBILE (1)
#endif



//////////////////////////////////////////
#define MAZE_PLATFORM_SUFFIX_KEBAB_STR MAZE_STRINGIFY_ARG(MAZE_PLATFORM_SUFFIX_KEBAB)
#define MAZE_PLATFORM_SUFFIX_CAMEL_STR MAZE_STRINGIFY_ARG(MAZE_PLATFORM_SUFFIX_CAMEL)



//////////////////////////////////////////
// Analize compiler
//
//////////////////////////////////////////
#define MAZE_COMPILER_MSVC            (1)
#define MAZE_COMPILER_GNUC            (2)
#define MAZE_COMPILER_BORLAND         (3)
#define MAZE_COMPILER_WINSCW          (4)
#define MAZE_COMPILER_GCCE            (5)
#define MAZE_COMPILER_CLANG           (6)
#define MAZE_COMPILER_EMSCRIPTEN      (7)


//////////////////////////////////////////
// GCCE
#if (defined(__GMAZE__))
#   define MAZE_COMPILER MAZE_COMPILER_GCCE
#   define MAZE_COMPILER_NAME "GCCE"
#   define MAZE_COMPILER_VERSION _MSC_VER

// WINSCW
#elif (defined(__WINSCW__))
#   define MAZE_COMPILER MAZE_COMPILER_WINSCW
#   define MAZE_COMPILER_NAME "WINSCW"
#   define MAZE_COMPILER_VERSION _MSC_VER

// MSVC
#elif (defined(_MSC_VER))
#   define MAZE_COMPILER MAZE_COMPILER_MSVC
#   define MAZE_COMPILER_NAME "MSVC"
#   define MAZE_COMPILER_VERSION _MSC_VER

// CLANG
#elif (defined(__clang__))
#   define MAZE_COMPILER MAZE_COMPILER_CLANG
#   define MAZE_COMPILER_NAME "CLANG"
#   define MAZE_COMPILER_VERSION (((__clang_major__)*100) + \
        (__clang_minor__*10) + \
        __clang_patchlevel__)

// GNUC
#elif (defined(__GNUC__))
#   define MAZE_COMPILER MAZE_COMPILER_GNUC
#   define MAZE_COMPILER_NAME "GNUC"
#   define MAZE_COMPILER_VERSION (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)

// BORL
#elif (defined(__BORLANDC__))
#   define MAZE_COMPILER MAZE_COMPILER_BORLAND
#   define MAZE_COMPILER_NAME "BORLAND"
#   define MAZE_COMPILER_VERSION __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__

#elif (defined(__EMSCRIPTEN__))
#   define MAZE_COMPILER MAZE_COMPILER_EMSCRIPTEN
#   define MAZE_COMPILER_NAME "EMSCRIPTEN"
#   define MAZE_COMPILER_VERSION EMSCRIPTEN_VERSION

// Undefined
#else
#   pragma error "No known compiler. Abort! Abort!"
#endif


//////////////////////////////////////////
#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
#   pragma warning(disable : 4221)
#   pragma warning(disable : 4506)
#   pragma warning(disable : 4661)
#   pragma warning(disable : 26812)
#endif


//////////////////////////////////////////
// Debug mode
//
//////////////////////////////////////////
#if (MAZE_COMPILER == MAZE_COMPILER_CLANG)
#   if (!defined(NDEBUG))
#       define __DEBUG
#   endif
#endif

//////////////////////////////////////////
#if (defined(__DEBUG) || defined(_DEBUG) || defined(DEBUG))
#   define MAZE_DEBUG (1)
#   define MAZE_RELEASE (0)
#else
#   define MAZE_DEBUG (0)
#   define MAZE_RELEASE (1)
#endif


//////////////////////////////////////////
#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
#   if (MAZE_COMPILER_VERSION >= 1200)
#       define MAZE_FORCEINLINE __forceinline
#   endif
#elif (defined(__MINGW32__))
#   if (!defined(MAZE_FORCEINLINE))
#       define MAZE_FORCEINLINE __inline
#   endif
#else
#   define MAZE_FORCEINLINE __inline
#endif



//////////////////////////////////////////
// Analize ARCH
//
//////////////////////////////////////////
#define MAZE_ARCH_UNKNOWN        (0)
#define MAZE_ARCH_X86            (1)
#define MAZE_ARCH_X64            (2)
#define MAZE_ARCH_PPC            (3)
#define MAZE_ARCH_ARMV5          (4)
#define MAZE_ARCH_ARMV6          (5)
#define MAZE_ARCH_ARMV7          (6)
#define MAZE_ARCH_ARMV8          (7)
#define MAZE_ARCH_ARMV9          (8)
#define MAZE_ARCH_ARM64          (9)
#define MAZE_ARCH_MIPS           (10)
#define MAZE_ARCH_EMSCRIPTEN     (11)


//////////////////////////////////////////
#if ((MAZE_PLATFORM == MAZE_PLATFORM_IOS) && (defined(__i386__) || defined(__x86_64__)))
#   define MAZE_ARCH                        MAZE_ARCH_X64
#   define MAZE_ARCH_SUFFIX                 x64
#elif ((defined(_MSC_VER) && (defined(_M_IX86) || defined(_M_X64))) \
    || (defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))))
#   if (defined(__x86_64__) || defined(_M_X64) || defined(_WIN64))
#       define MAZE_ARCH                    MAZE_ARCH_X64
#       define MAZE_ARCH_SUFFIX             x64
#   else
#       define MAZE_ARCH                    MAZE_ARCH_X86
#       define MAZE_ARCH_SUFFIX             x86
#    endif
#elif ((MAZE_PLATFORM == MAZE_PLATFORM_OSX) && defined(__BIG_ENDIAN__))
#   define MAZE_ARCH                        MAZE_ARCH_PPC
#   define MAZE_ARCH_SUFFIX                 ppc
#   if (defined(__powerpc64__))

#   else

#   endif
#elif (MAZE_PLATFORM == MAZE_PLATFORM_OSX)
#   define MAZE_ARCH                        MAZE_ARCH_X64
#   define MAZE_ARCH_SUFFIX                 x64


#elif (defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64))
#   if (defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64))
#       define MAZE_ARCH                    MAZE_ARCH_ARM64
#       define MAZE_ARCH_SUFFIX             arm64
#   elif (     defined(__ARM_ARCH_9__)) \
        || (_M_ARM >= 9)
#        define MAZE_ARCH                    MAZE_ARCH_ARMV9
#        define MAZE_ARCH_SUFFIX             armv9
#   elif (     defined(__ARM_ARCH_8__) \
        ||     defined(__ARM_ARCH_8A) \
        ||     defined(__ARM_ARCH_8A__)) \
        || (_M_ARM >= 8)
#        define MAZE_ARCH                    MAZE_ARCH_ARMV8
#        define MAZE_ARCH_SUFFIX             armv8
#   elif (     defined(__ARM_ARCH_7__) \
        ||     defined(__ARM_ARCH_7A__) \
        ||     defined(__ARM_ARCH_7R__) \
        ||     defined(__ARM_ARCH_7M__) \
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7)) \
        || (_M_ARM >= 7)
#        define MAZE_ARCH                    MAZE_ARCH_ARMV7
#        define MAZE_ARCH_SUFFIX             armv7
#    elif (    defined(__ARM_ARCH_6__) \
        ||     defined(__ARM_ARCH_6J__) \
        ||     defined(__ARM_ARCH_6T2__) \
        ||     defined(__ARM_ARCH_6Z__) \
        ||     defined(__ARM_ARCH_6K__) \
        ||     defined(__ARM_ARCH_6ZK__) \
        ||     defined(__ARM_ARCH_6M__) \
        ||     (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6)) \
        || (_M_ARM >= 6)
#        define MAZE_ARCH                    MAZE_ARCH_ARMV6
#        define MAZE_ARCH_SUFFIX             armv6
#   elif (     defined(__ARM_ARCH_5TEJ__) \
        ||     (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5)) \
        || (_M_ARM >= 5)
#        define MAZE_ARCH                    MAZE_ARCH_ARMV5
#        define MAZE_ARCH_SUFFIX             armv5
#   else
#       pragma error "Undefined ARM architecture!"
#       define MAZE_ARCH                     MAZE_ARCH_UNKNOWN
#       define MAZE_ARCH_SUFFIX              unknown
#   endif

/*
#elif (defined(__arm__) || defined(_M_ARM) || defined(__arm64__) || defined(__aarch64__))
#   if defined(__LP64__)
#   define MAZE_ARCH                         MAZE_ARCH_ARM64
#    define MAZE_ARCH_SUFFIX                 arm64
#   else
#   define MAZE_ARCH                         MAZE_ARCH_ARM
#    define MAZE_ARCH_SUFFIX                 arm
#   endif
*/
#elif (defined(__mips64) || defined(__mips64_))
#   define MAZE_ARCH                         MAZE_ARCH_MIPS
#   define MAZE_ARCH_SUFFIX                  mips
#elif (defined(__EMSCRIPTEN__))
#   define MAZE_ARCH                         MAZE_ARCH_EMSCRIPTEN
#   define MAZE_ARCH_SUFFIX                  emscripten
#else
#   pragma error "Undefined CPU architecture!"
#   define MAZE_ARCH                         MAZE_ARCH_UNKNOWN
#   define MAZE_ARCH_SUFFIX                  unknown
#endif

//////////////////////////////////////////
#define MAZE_ARCH_SUFFIX_STR MAZE_STRINGIFY_ARG(MAZE_ARCH_SUFFIX)
#define MAZE_ARCH_SUFFIX_WSTR MAZE_STRINGIFY_ARG_WIDE(MAZE_ARCH_SUFFIX)



//////////////////////////////////////////
// Endian
//
//////////////////////////////////////////
#define MAZE_ENDIAN_LITTLE    (1)
#define MAZE_ENDIAN_BIG       (2)

#if (defined(__BIG_ENDIAN__))
#   define MAZE_ENDIAN   MAZE_ENDIAN_BIG
#else
#   define MAZE_ENDIAN   MAZE_ENDIAN_LITTLE
#endif


//////////////////////////////////////////
#if (MAZE_COMPILER == MAZE_COMPILER_MSVC)
#define MAZE_DECL_MALLOC __declspec(restrict) __declspec(noalias)
#else
#define MAZE_DECL_MALLOC __attribute__ ((malloc))
#endif


//////////////////////////////////////////
// Shared/Static
#if (!defined(MAZE_STATIC))

#   if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)

        // Windows compilers need specific (and different) keywords for export and import
#       define MAZE_API_EXPORT __declspec(dllexport)
#       define MAZE_API_IMPORT __declspec(dllimport)

        // For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#       ifdef _MSC_VER
#           pragma warning(disable: 4251)
#       endif

#   else // Linux, FreeBSD, Mac OS X

#       if (__GNUC__ >= 4)

            // GCC 4 has special keywords for showing/hidding symbols,
            // the same keyword is used for both importing and exporting
#           define MAZE_API_EXPORT __attribute__ ((__visibility__ ("default")))
#           define MAZE_API_IMPORT __attribute__ ((__visibility__ ("default")))

#       else

            // GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
#           define MAZE_API_EXPORT
#           define MAZE_API_IMPORT

#       endif

#   endif

#else

    // Static build doesn't need import/export macros
#   define MAZE_API_EXPORT
#   define MAZE_API_IMPORT

#endif


#endif // _MazePreprocessor_Platform_hpp_
//////////////////////////////////////////
