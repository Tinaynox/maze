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
maze_add_module(
    maze-core
    INCLUDE_DIR "include/maze-core"
    SRC_DIR "src/maze-core"
    FORWARD_HEADER MazeCoreHeader)
    
    
target_link_libraries(maze-core 
    PUBLIC nedmalloc
    PUBLIC tinyxml2
    PUBLIC jsoncpp_static)
    
target_include_directories(
    maze-core
    PUBLIC "${MAZE_DIR}/third-party/utf8"
    PUBLIC "${MAZE_DIR}/third-party/tinyxml2"
    PUBLIC "${MAZE_DIR}/third-party/jsoncpp/include")

if(MAZE_TARGET_PLATFORM_IS_WINDOWS)

    if((MAZE_ARCH_SUFFIX STREQUAL "x86") OR (MAZE_ARCH_SUFFIX STREQUAL "x64"))
    
        target_link_libraries(
            maze-core
            PUBLIC Winmm.lib
            PUBLIC netapi32.lib
            PUBLIC IPHLPAPI.lib
            PUBLIC dwmapi.lib)
    
    endif()
    
    if (MAZE_USE_OPTICK)
        target_link_libraries(maze-core 
            PUBLIC OptickCore)
        target_include_directories(maze-core
            PUBLIC "${MAZE_DIR}/third-party/optick/src")
    endif()
    
elseif(MAZE_TARGET_PLATFORM_IS_LINUX)

    find_package(Threads)
    find_package(X11 REQUIRED)


    target_include_directories(
        maze-core
        PUBLIC ${X11_INCLUDE_DIR}
        PUBLIC ${X11_Xrandr_INCLUDE_PATH})

    target_link_libraries(
        maze-core
        PUBLIC ${CMAKE_THREAD_LIBS_INIT}
        PUBLIC dl
        PUBLIC ${X11_LIBRARIES}
        PUBLIC ${X11_Xrandr_LIB})


elseif(MAZE_TARGET_PLATFORM_IS_OSX)
    
    target_link_libraries(
        maze-core
        PUBLIC "-framework Foundation"
        PUBLIC "-framework AppKit"
        PUBLIC "-framework CoreFoundation"
        PUBLIC "-framework IOKit"
        PUBLIC "-framework CoreVideo")

elseif(MAZE_TARGET_PLATFORM_IS_ANDROID)
    find_library(log-lib log)
    find_library(android-lib android)
    
    add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
    
    target_link_libraries(
        maze-core    
        PUBLIC ${log-lib}
        PUBLIC ${android-lib}
        PUBLIC native_app_glue)
    
    target_include_directories(
        maze-core
        PUBLIC ${ANDROID_NDK}/sources/android/native_app_glue)
endif()