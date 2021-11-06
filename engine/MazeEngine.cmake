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
    maze-engine
    INCLUDE_DIR "include/maze-engine"
    SRC_DIR "src/maze-engine"
    FORWARD_HEADER MazeEngineHeader)


##########################################
set(ENGINE_LIBS maze-core maze-graphics maze-particles maze-physics2d maze-sound maze-gamepad maze-debugger)


##########################################
if(MAZE_RENDER_SYSTEM_OPENGL_ENABLED)
    list(APPEND ENGINE_LIBS maze-render-system-opengl-core maze-render-system-opengl3)
endif()


##########################################
if(MAZE_SOUND_SYSTEM_OPENAL_ENABLED)
    list(APPEND ENGINE_LIBS maze-sound-system-openal)
endif()


##########################################
target_link_libraries(maze-engine ${ENGINE_LIBS})