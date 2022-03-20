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
project(maze-plugin-profiler-view)


##########################################
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Utils.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Config.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Macros.cmake")


##########################################
# add_subdirectory("${MAZE_ENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/MazeEngine")


##########################################
maze_add_module(
    ${PROJECT_NAME}
    INCLUDE_DIR "include/${PROJECT_NAME}"
    SRC_DIR "src"
    FORWARD_HEADER MazeProfilerViewHeader)


##########################################    
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


##########################################
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "MazePlugins")


##########################################
target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC maze-core
    PUBLIC maze-graphics
    PUBLIC maze-ui)