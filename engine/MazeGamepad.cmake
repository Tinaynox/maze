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
maze_add_module(
    maze-gamepad
    INCLUDE_DIR "include/maze-gamepad"
    SRC_DIR "src/maze-gamepad"
    FORWARD_HEADER MazeGamepadHeader)


##########################################
set(MODULE_LIBS maze-core maze-graphics)


##########################################
#libstem_gamepad
include("${MAZE_DIR}/third-party/libstem_gamepad.cmake")


##########################################
if(MAZE_TARGET_PLATFORM_IS_WINDOWS OR MAZE_TARGET_PLATFORM_IS_OSX OR MAZE_TARGET_PLATFORM_IS_LINUX)
   list(APPEND MODULE_LIBS libstem_gamepad)
endif()


##########################################
target_link_libraries(maze-gamepad ${MODULE_LIBS})


##########################################        
target_include_directories(
    maze-gamepad
    PUBLIC "${PROJECT_BINARY_DIR}/third-party/libstem_gamepad/source")