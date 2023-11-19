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
set(JPEG_BUILD_EXECUTABLES OFF)
add_subdirectory("${MAZE_DIR}/third-party/libjpeg-cmake" "${CMAKE_CURRENT_BINARY_DIR}/third-party/libjpeg-cmake")
set_property(TARGET libjpeg PROPERTY FOLDER "MazeThirdParty")
#set_property(TARGET cjpeg PROPERTY FOLDER "MazeThirdParty")
#set_property(TARGET djpeg PROPERTY FOLDER "MazeThirdParty")
#set_property(TARGET jpegtran PROPERTY FOLDER "MazeThirdParty")
#set_property(TARGET rdjpgcom PROPERTY FOLDER "MazeThirdParty")
#set_property(TARGET wrjpgcom PROPERTY FOLDER "MazeThirdParty")
target_include_directories(
    libjpeg
    PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/libjpeg-cmake>
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/third-party/libjpeg-cmake>)
set_target_properties(libjpeg
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