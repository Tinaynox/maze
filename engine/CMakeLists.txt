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
option(MAZE_PRODUCTION "Production mode" OFF)
option(MAZE_USE_OPTICK "Use Optick Profiler" OFF)
if(NOT DEFINED MAZE_OUTPUT_DIR)
    set(MAZE_OUTPUT_DIR "${MAZE_DIR}/_otp" CACHE STRING "Output dir")
endif()


##########################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


##########################################
# Third Party Libs
#
##########################################
include("MazeThirdParty.cmake")


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
# Maze EditorTools
#
##########################################
include("MazeEditorTools.cmake")


##########################################
# Maze Particles
#
##########################################
include("MazeParticles.cmake")


##########################################
# Maze Sound
#
##########################################
include("MazeSound.cmake")


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



##########################################
# Maze Sound System OpenAL
#
##########################################
if(MAZE_SOUND_SYSTEM_OPENAL_ENABLED)
    include("MazeSoundSystemOpenAL.cmake")
endif()