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
# SPIRV-Reflect (2-file C library, Apache-2.0 license)
# Vendored from https://github.com/KhronosGroup/SPIRV-Reflect
add_library(
    spirv-reflect
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/spirv-reflect/spirv_reflect.h"
    "${CMAKE_CURRENT_LIST_DIR}/spirv-reflect/spirv_reflect.c")
target_include_directories(spirv-reflect PUBLIC "${CMAKE_CURRENT_LIST_DIR}/spirv-reflect")
set_property(TARGET spirv-reflect PROPERTY FOLDER "MazeThirdParty")
set_target_properties(spirv-reflect
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_OUTPUT_DIR}/lib/${MAZE_ARCH_SUFFIX}/$<CONFIG>")
