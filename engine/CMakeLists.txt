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
project(Maze)


##########################################
include("cmake/Utils.cmake")
include("cmake/Config.cmake")
include("cmake/Macros.cmake")


##########################################
message("MAZE_TARGET_PLATFORM_NAME=${MAZE_TARGET_PLATFORM_NAME}")
message("MAZE_ARCH_SUFFIX=${MAZE_ARCH_SUFFIX}")
message("MAZE_COMPILER=${MAZE_COMPILER}")



##########################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


##########################################
# Third Party Libs
#
##########################################

# nedmalloc
add_subdirectory(third-party/nedmalloc)
set_property(TARGET nedmalloc PROPERTY FOLDER "MazeThirdParty") 

# tinyxml2
set(BUILD_TESTS OFF)
add_subdirectory(third-party/tinyxml2)
set_property(TARGET tinyxml2 PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET uninstall PROPERTY FOLDER "MazeThirdParty")
set_target_properties(tinyxml2
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}")

# jsoncpp
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Compile and (for jsoncpp_check) run JsonCpp test executables" FORCE)
set(JSONCPP_WITH_CMAKE_PACKAGE OFF CACHE BOOL "Generate and install cmake package files" FORCE)
# set(JSONCPP_LIB_BUILD_STATIC ON)
# set(JSONCPP_LIB_BUILD_SHARED OFF)
set(BUILD_OBJECT_LIBS OFF CACHE BOOL "Build jsoncpp_lib as a object library." FORCE)
add_subdirectory(third-party/jsoncpp)
set(JSONCPP_LIBRARY jsoncpp_static)
set_property(TARGET ${JSONCPP_LIBRARY} PROPERTY FOLDER "MazeThirdParty")
if(BUILD_OBJECT_LIBS)
    set(JSONCPP_OBJECT jsoncpp_object)
    set_property(TARGET ${JSONCPP_OBJECT} PROPERTY FOLDER "MazeThirdParty")
endif()
set_target_properties(${JSONCPP_LIBRARY}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}")


# zlib
if(BUILD_SHARED_LIBS)
    set(ZLIB_NAME zlib)
else()
    set(ZLIB_NAME zlibstatic)
endif()
set(ZLIB_LIBRARY ${ZLIB_NAME})
set(ZLIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/third-party/zlib")
add_subdirectory(third-party/zlib)
set_property(TARGET zlib PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET zlibstatic PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET minigzip PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET example PROPERTY FOLDER "MazeThirdParty")
target_include_directories(
    minigzip
    PUBLIC "${PROJECT_SOURCE_DIR}/third-party/zlib"
    PUBLIC "${PROJECT_BINARY_DIR}/third-party/zlib")
target_include_directories(
    example
    PUBLIC "${PROJECT_SOURCE_DIR}/third-party/zlib"
    PUBLIC "${PROJECT_BINARY_DIR}/third-party/zlib")
if(HAVE_OFF64_T)
    target_include_directories(
        minigzip64
        PUBLIC "${PROJECT_SOURCE_DIR}/third-party/zlib"
        PUBLIC "${PROJECT_BINARY_DIR}/third-party/zlib")
    target_include_directories(
        example64
        PUBLIC "${PROJECT_SOURCE_DIR}/third-party/zlib"
        PUBLIC "${PROJECT_BINARY_DIR}/third-party/zlib")
endif()
set_target_properties(${ZLIB_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}")

# libpng
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
add_subdirectory(third-party/libpng)
set_property(TARGET ${PNGLIB_NAME} PROPERTY FOLDER "MazeThirdParty")
set_property(TARGET genfiles PROPERTY FOLDER "MazeThirdParty")
target_include_directories(
    ${PNGLIB_NAME}
    PUBLIC "${PROJECT_SOURCE_DIR}/third-party/zlib"
    PUBLIC "${PROJECT_BINARY_DIR}/third-party/zlib"
    PUBLIC "${PROJECT_SOURCE_DIR}/third-party/libpng"
    PUBLIC "${PROJECT_BINARY_DIR}/third-party/libpng")
if(BUILD_SHARED_LIBS)
    target_include_directories(
        png-fix-itxt
        PUBLIC "${PROJECT_SOURCE_DIR}/third-party/zlib"
        PUBLIC "${PROJECT_BINARY_DIR}/third-party/zlib"
        PUBLIC "${PROJECT_SOURCE_DIR}/third-party/libpng"
        PUBLIC "${PROJECT_BINARY_DIR}/third-party/libpng")
endif()
set_target_properties(${PNGLIB_NAME}
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}")

# Box2d
set(BOX2D_BUILD_EXAMPLES OFF)
set(BOX2D_BUILD_TESTBED OFF)
set(BOX2D_BUILD_UNIT_TESTS OFF)
add_subdirectory(third-party/box2d)
set_property(TARGET box2d PROPERTY FOLDER "MazeThirdParty")
set_target_properties(box2d
    PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_SOURCE_DIR}/lib/${MAZE_ARCH_SUFFIX}")


##########################################
# Maze Core
#
##########################################
include("MazeCore.cmake")


##########################################
# Maze Graphics
#
##########################################
include("MazeGraphics.cmake")


##########################################
# Maze UI
#
##########################################
include("MazeUI.cmake")


##########################################
# Maze Gamepad
#
##########################################
include("MazeGamepad.cmake")


##########################################
# Maze Physics2D
#
##########################################
include("MazePhysics2D.cmake")


##########################################
# Maze Debugger
#
##########################################
include("MazeDebugger.cmake")


##########################################
# Maze Particles
#
##########################################
include("MazeParticles.cmake")


##########################################
# Maze Engine
#
##########################################
include("MazeEngine.cmake")



##########################################
# Maze Render System OpenGL
#
##########################################
if(MAZE_RENDER_SYSTEM_OPENGL_ENABLED)

    ##########################################
    # Maze Render System OpenGLCore
    #
    ##########################################
    include("MazeRenderSystemOpenGLCore.cmake")


    ##########################################
    # Maze Render System OpenGL3
    #
    ##########################################
    include("MazeRenderSystemOpenGL3.cmake")
    
endif()
