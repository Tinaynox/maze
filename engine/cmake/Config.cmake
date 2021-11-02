##########################################
#
# Maze Engine
# Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it freely,
# subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented;
#    you must not claim that you wrote the original software.
#    If you use this software in a product, an acknowledgment
#    in the product documentation would be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such,
#    and must not be misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.
#
##########################################


##########################################
if(NOT _Config_cmake_)
set(_Config_cmake_ 1)


##########################################
include("${CMAKE_CURRENT_LIST_DIR}/Utils.cmake")


##########################################
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

    set(MAZE_TARGET_PLATFORM_NAME "win")
    set(MAZE_TARGET_PLATFORM_IS_WINDOWS 1)
    
elseif(ANDROID)

    set(MAZE_TARGET_PLATFORM_NAME "android")
    set(MAZE_TARGET_PLATFORM_IS_ANDROID 1)
    set(MAZE_TARGET_PLATFORM_IS_UNIX 1)
    
elseif(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")

    set(MAZE_TARGET_PLATFORM_NAME "emscripten")
    set(MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN 1)
    set(MAZE_TARGET_PLATFORM_IS_UNIX 1)
    
elseif(UNIX AND NOT APPLE)

    set(MAZE_TARGET_PLATFORM_NAME "linux")
    set(MAZE_TARGET_PLATFORM_IS_LINUX 1)
    set(MAZE_TARGET_PLATFORM_IS_UNIX 1)
    
elseif(APPLE)

    if(IOS)
    
        set(MAZE_TARGET_PLATFORM_NAME "ios")
        set(MAZE_TARGET_PLATFORM_IS_IOS 1)
        set(MAZE_TARGET_PLATFORM_IS_UNIX 1)
        set(MAZE_TARGET_PLATFORM_IS_APPLE 1)
        
    else()
    
        set(MAZE_TARGET_PLATFORM_NAME "osx")
        set(MAZE_TARGET_PLATFORM_IS_OSX 1)
        set(MAZE_TARGET_PLATFORM_IS_UNIX 1)
        set(MAZE_TARGET_PLATFORM_IS_APPLE 1)

    endif()
    
else()

    message(FATAL_ERROR "Unknown platform!")
    return()
    
endif()


if(MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN)

    set(MAZE_COMPILER "emscripten")
    set(MAZE_COMPILER_IS_EMSCRIPTEN 1)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1 -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0 -s FULL_ES3=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")

elseif(CMAKE_CXX_COMPILER MATCHES "clang[+][+]" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")

   # CMAKE_CXX_COMPILER_ID is an internal CMake variable subject to change,
   # but there is no other way to detect CLang at the moment
   set(MAZE_COMPILER "clang")
   set(MAZE_COMPILER_IS_CLANG 1)
   
   execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
   string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" MAZE_CLANG_VERSION "${CLANG_VERSION_OUTPUT}")
   
elseif(CMAKE_COMPILER_IS_GNUCXX)

    set(MAZE_COMPILER "gcc")
    set(MAZE_COMPILER_IS_GCC 1)
    
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" MAZE_GCC_VERSION "${GCC_VERSION_OUTPUT}")
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" MAZE_GCC_TDM "${GCC_COMPILER_VERSION}")
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpmachine" OUTPUT_VARIABLE GCC_MACHINE)
    string(STRIP "${GCC_MACHINE}" GCC_MACHINE)
    
    if(GCC_MACHINE MATCHES ".*w64.*")
        set(MAZE_COMPILER_GCC_W64 1)
    endif()
    
elseif(MSVC)

    set(MAZE_COMPILER "msvc")
    set(MAZE_COMPILER_IS_MSVC 1)
    
    if(MSVC_VERSION EQUAL 1400)
        set(MAZE_MSVC_VERSION 8)
    elseif(MSVC_VERSION EQUAL 1500)
        set(MAZE_MSVC_VERSION 9)
    elseif(MSVC_VERSION EQUAL 1600)
        set(MAZE_MSVC_VERSION 10)
    elseif(MSVC_VERSION EQUAL 1700)
        set(MAZE_MSVC_VERSION 11)
    elseif(MSVC_VERSION EQUAL 1800)
        set(MAZE_MSVC_VERSION 12)
    elseif(MSVC_VERSION EQUAL 1900)
        set(MAZE_MSVC_VERSION 14)
    endif()
else()

    set(MAZE_COMPILER "undefined")
    message(FATAL_ERROR "Unsupported compiler")
    return()
    
endif()

if(MAZE_COMPILER_IS_CLANG OR MAZE_COMPILER_IS_EMSCRIPTEN)
    add_compile_options(-Wno-undefined-var-template)
endif()

set(ARCHDETECT_C_CODE "
#if(defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM))
    #if(defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64))
        #error cmake_ARCH armv64
    #elif defined(__ARM_ARCH_7__) \\
        || defined(__ARM_ARCH_7A__) \\
        || defined(__ARM_ARCH_7R__) \\
        || defined(__ARM_ARCH_7M__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 7) \\
        || (_M_ARM >= 7)
        #error cmake_ARCH armv7
    #elif defined(__ARM_ARCH_6__) \\
        || defined(__ARM_ARCH_6J__) \\
        || defined(__ARM_ARCH_6T2__) \\
        || defined(__ARM_ARCH_6Z__) \\
        || defined(__ARM_ARCH_6K__) \\
        || defined(__ARM_ARCH_6ZK__) \\
        || defined(__ARM_ARCH_6M__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 6) \\
        || (_M_ARM >= 6)
        #error cmake_ARCH armv6
    #elif defined(__ARM_ARCH_5TEJ__) \\
        || (defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM-0 >= 5) \\
        || (_M_ARM >= 5)
        #error cmake_ARCH armv5
    #else
        #error cmake_ARCH arm
    #endif
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
    #error cmake_ARCH x86
#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
    #error cmake_ARCH x64
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
    #error cmake_ARCH ia64
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) \\
      || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  \\
      || defined(_M_MPPC) || defined(_M_PPC)
    #if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
        #error cmake_ARCH ppc64
    #else
        #error cmake_ARCH ppc
    #endif
#elif defined(__EMSCRIPTEN__)
    #error cmake_ARCH emscripten
#endif
#error cmake_ARCH unknown
")


if(APPLE AND CMAKE_OSX_ARCHITECTURES)
    # On OS X we use CMAKE_OSX_ARCHITECTURES *if* it was set
    # First let's normalize the order of the values

    # Note that it's not possible to compile PowerPC applications if you are using
    # the OS X SDK version 10.6 or later - you'll need 10.4/10.5 for that, so we
    # disable it by default
    # See this page for more information:
    # http://stackoverflow.com/questions/5333490/how-can-we-restore-ppc-ppc64-as-well-as-full-10-4-10-5-sdk-support-to-xcode-4

    # Architecture defaults to i386 or ppc on OS X 10.5 and earlier, depending on the CPU type detected at runtime.
    # On OS X 10.6+ the default is x86_64 if the CPU supports it, i386 otherwise.

    foreach(OSX_ARCH ${CMAKE_OSX_ARCHITECTURES})
        if("${OSX_ARCH}" STREQUAL "ppc" AND ppc_support)
            set(OSX_ARCH_ppc TRUE)
        elseif("${OSX_ARCH}" STREQUAL "i386")
            set(OSX_ARCH_i386 TRUE)
        elseif("${OSX_ARCH}" STREQUAL "x86_64")
            set(OSX_ARCH_x86_64 TRUE)
        elseif("${OSX_ARCH}" STREQUAL "ppc64" AND ppc_support)
            set(OSX_ARCH_ppc64 TRUE)
        elseif("${OSX_ARCH}" STREQUAL "arm64")
            set(OSX_ARCH_arm64 TRUE)
        else()
            message(FATAL_ERROR "Invalid OS X arch name: ${OSX_ARCH}")
        endif()
    endforeach()

    # Now add all the architectures in our normalized order
    if(OSX_ARCH_ppc)
        list(APPEND MAZE_ARCH_SUFFIX ppc)
    endif()

    if(OSX_ARCH_i386)
        list(APPEND MAZE_ARCH_SUFFIX x86)
    endif()

    if(OSX_ARCH_x86_64)
        list(APPEND MAZE_ARCH_SUFFIX x64)
    endif()

    if(OSX_ARCH_ppc64)
        list(APPEND MAZE_ARCH_SUFFIX ppc64)
    endif()

    if(OSX_ARCH_arm64)
        list(APPEND MAZE_ARCH_SUFFIX arm64)
    endif()
    
else()
    file(WRITE "${CMAKE_BINARY_DIR}/arch.c" "${ARCHDETECT_C_CODE}")

    enable_language(C)

    # Detect the architecture in a rather creative way...
    # This compiles a small C program which is a series of ifdefs that selects a
    # particular #error preprocessor directive whose message string contains the
    # target architecture. The program will always fail to compile (both because
    # file is not a valid C program, and obviously because of the presence of the
    # #error preprocessor directives... but by exploiting the preprocessor in this
    # way, we can detect the correct target architecture even when cross-compiling,
    # since the program itself never needs to be run (only the compiler/preprocessor)
    try_run(
        run_result_unused
        compile_result_unused
        "${CMAKE_BINARY_DIR}"
        "${CMAKE_BINARY_DIR}/arch.c"
        COMPILE_OUTPUT_VARIABLE MAZE_ARCH_SUFFIX
        CMAKE_FLAGS CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    )

    # Parse the architecture name from the compiler output
    string(REGEX MATCH "cmake_ARCH ([a-zA-Z0-9_]+)" MAZE_ARCH_SUFFIX "${MAZE_ARCH_SUFFIX}")

    # Get rid of the value marker leaving just the architecture name
    string(REPLACE "cmake_ARCH " "" MAZE_ARCH_SUFFIX "${MAZE_ARCH_SUFFIX}")

    # If we are compiling with an unknown architecture this variable should
    # already be set to "unknown" but in the case that it's empty (i.e. due
    # to a typo in the code), then set it to unknown
    if(NOT MAZE_ARCH_SUFFIX)
        set(MAZE_ARCH_SUFFIX unknown)
    endif()
endif()

string_starts_with(${MAZE_ARCH_SUFFIX}, "arm" IS_ARM_ARCH)

if(       (MAZE_TARGET_PLATFORM_IS_WINDOWS AND (NOT IS_ARM_ARCH))
    OR    (MAZE_TARGET_PLATFORM_IS_OSX)
    OR    (MAZE_TARGET_PLATFORM_IS_LINUX)
    OR    (MAZE_TARGET_PLATFORM_IS_ANDROID)
    OR    (MAZE_TARGET_PLATFORM_IS_IOS)
    OR    (MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN))

    set(MAZE_RENDER_SYSTEM_OPENGL_ENABLED 1)
    
endif()

if(       (MAZE_TARGET_PLATFORM_IS_WINDOWS AND (NOT IS_ARM_ARCH))
    OR    (MAZE_TARGET_PLATFORM_IS_OSX)
    OR    (MAZE_TARGET_PLATFORM_IS_LINUX)
    OR    (MAZE_TARGET_PLATFORM_IS_ANDROID)
    OR    (MAZE_TARGET_PLATFORM_IS_IOS)
    OR    (MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN))

    set(MAZE_SOUND_SYSTEM_OPENAL_ENABLED 1)
    
endif()



endif(NOT _Config_cmake_)
##########################################