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
if(MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN)
    set(M_LIBRARY "")
endif()
set(PNG_TESTS OFF CACHE BOOL "Build libpng tests" FORCE)
set(SKIP_INSTALL_ALL ON)
if(BUILD_SHARED_LIBS)
    set(PNGLIB_NAME png)
    set(PNG_SHARED ON)
    set(PNG_STATIC OFF)
else()
    set(PNGLIB_NAME png_static)
    set(PNG_SHARED OFF)
    set(PNG_STATIC ON)
endif()
add_subdirectory("${MAZE_DIR}/third-party/libpng" "${CMAKE_CURRENT_BINARY_DIR}/third-party/libpng")
set_property(TARGET ${PNGLIB_NAME} PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET genfiles PROPERTY FOLDER "MazeThirdParty")
if(BUILD_SHARED_LIBS)
    set_property(TARGET pngfix PROPERTY FOLDER "MazeThirdParty")
    set_property(TARGET png-fix-itxt PROPERTY FOLDER "MazeThirdParty")
endif()
target_include_directories(
    ${PNGLIB_NAME}
    PUBLIC $<BUILD_INTERFACE:${MAZE_DIR}/third-party/libpng>
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/third-party/libpng>)
set_target_properties(${PNGLIB_NAME}
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