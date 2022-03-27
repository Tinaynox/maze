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
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Compile and (for jsoncpp_check) run JsonCpp test executables" FORCE)
set(JSONCPP_WITH_CMAKE_PACKAGE OFF CACHE BOOL "Generate and install cmake package files" FORCE)
# set(JSONCPP_LIB_BUILD_STATIC ON)
# set(JSONCPP_LIB_BUILD_SHARED OFF)
set(BUILD_OBJECT_LIBS OFF CACHE BOOL "Build jsoncpp_lib as a object library." FORCE)
add_subdirectory("${MAZE_DIR}/third-party/jsoncpp" "${CMAKE_CURRENT_BINARY_DIR}/third-party/jsoncpp")
set(JSONCPP_LIBRARY jsoncpp_static)
set_property(TARGET ${JSONCPP_LIBRARY} PROPERTY FOLDER "MazeThirdParty")
if(BUILD_OBJECT_LIBS)
    set(JSONCPP_OBJECT jsoncpp_object)
    set_property(TARGET ${JSONCPP_OBJECT} PROPERTY FOLDER "MazeThirdParty")
endif()
set_target_properties(${JSONCPP_LIBRARY}
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