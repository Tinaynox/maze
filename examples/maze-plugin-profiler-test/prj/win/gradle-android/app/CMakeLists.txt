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
cmake_minimum_required(VERSION 3.6)


##########################################
project(maze-android-native-lib)


##########################################
set(EXAMPLE_NAME "maze-plugin-profiler-test")
set(EXAMPLE_LIB "${EXAMPLE_NAME}")


##########################################
get_filename_component(BUILD_ARCH_FOLDER ${CMAKE_CURRENT_BINARY_DIR} NAME)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../../../examples/${EXAMPLE_NAME}" "${CMAKE_CURRENT_BINARY_DIR}/../../../../../cmake/${BUILD_ARCH_FOLDER}")

set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")


add_library(maze-native-lib
            SHARED
            src/main/cpp/AndroidMain.cpp)
        
target_link_libraries(maze-native-lib
    ${EXAMPLE_LIB})