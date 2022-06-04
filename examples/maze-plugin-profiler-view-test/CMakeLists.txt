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
project(maze-plugin-profiler-view-test)


##########################################
set(EXAMPLE_NAME "${PROJECT_NAME}")
set(EXAMPLE_MAZE_LIBS
    maze-core
    maze-graphics
    maze-ui
    maze-engine
    maze-sound
    maze-editor-tools
    maze-plugin-profiler-view
    maze-plugin-console)


##########################################
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Utils.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Config.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../../engine/cmake/Macros.cmake")


##########################################
if(MAZE_RENDER_SYSTEM_OPENGL_ENABLED)
    list(APPEND EXAMPLE_MAZE_LIBS maze-render-system-opengl-core maze-render-system-opengl3)
endif()


##########################################
maze_add_sources(${CMAKE_CURRENT_SOURCE_DIR}/../templates/example-wrapper ADDITIONAL_EXAMPLE_FILES)
maze_sort_sources("${ADDITIONAL_EXAMPLE_FILES}" ADDITIONAL_EXAMPLE_FILES)


##########################################
include("${CMAKE_CURRENT_SOURCE_DIR}/../templates/CMakeExampleTemplate.cmake")


##########################################
add_subdirectory("${MAZE_DIR}/plugins/profiler-view" "${CMAKE_CURRENT_BINARY_DIR}/plugins/profiler-view")
add_subdirectory("${MAZE_DIR}/plugins/console" "${CMAKE_CURRENT_BINARY_DIR}/plugins/console")


##########################################
target_include_directories(
    ${EXAMPLE_NAME}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../templates/example-wrapper")