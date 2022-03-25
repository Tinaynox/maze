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
include("${CMAKE_CURRENT_LIST_DIR}/zlib_info.cmake")
add_subdirectory("${MAZE_DIR}/third-party/zlib" "${CMAKE_CURRENT_BINARY_DIR}/third-party/zlib")
set_property(TARGET zlib PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET zlibstatic PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET minigzip PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET example PROPERTY FOLDER "MazeThirdParty")
target_include_directories(
    ${ZLIB_NAME}
    PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/zlib>
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/third-party/zlib>)
target_include_directories(
    minigzip
    PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/zlib>
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/third-party/zlib>)
target_include_directories(
    example
    PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/zlib>
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/third-party/zlib>)
if(HAVE_OFF64_T)
    target_include_directories(
        minigzip64
        PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/zlib>
        PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/third-party/zlib>)
    target_include_directories(
        example64
        PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/zlib>
        PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/third-party/zlib>)
endif()
set_target_properties(${ZLIB_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${MAZE_ENGINE_DIR}/lib/${MAZE_ARCH_SUFFIX}")