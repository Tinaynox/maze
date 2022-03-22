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
    maze-physics2d
    INCLUDE_DIR "include/maze-physics2d"
    SRC_DIR "src/maze-physics2d"
    FORWARD_HEADER MazePhysics2DHeader)

target_link_libraries(
    maze-physics2d
    PUBLIC maze-core
    PUBLIC maze-graphics
    PUBLIC ${ZLIB_NAME}
    PUBLIC ${PNGLIB_NAME}
    PUBLIC box2d)
        
target_include_directories(
    maze-physics2d
    PUBLIC "${MAZE_DIR}/third-party/zlib"
    PUBLIC "${PROJECT_BINARY_DIR}/third-party/zlib"
    PUBLIC "${MAZE_DIR}/third-party/libpng"
    PUBLIC "${PROJECT_BINARY_DIR}/third-party/libpng"
    PUBLIC "${MAZE_DIR}/third-party/box2d/include")