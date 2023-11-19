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


##########################################
set(FT_DISABLE_BZIP2 ON)
set(FT_DISABLE_PNG ON)
set(FT_DISABLE_HARFBUZZ ON)
set(FT_DISABLE_BROTLI ON)
set(SKIP_INSTALL_ALL ON)
add_subdirectory("${MAZE_DIR}/third-party/freetype2" "${CMAKE_CURRENT_BINARY_DIR}/third-party/freetype2")
set_property(TARGET freetype PROPERTY FOLDER "MazeThirdParty")
set_target_properties(freetype
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