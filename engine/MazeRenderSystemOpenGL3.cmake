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
    maze-render-system-opengl3
    INCLUDE_DIR "include/maze-render-system-opengl3"
    SRC_DIR "src/maze-render-system-opengl3"
    FORWARD_HEADER MazeRenderSystemOpenGL3Header)
    
target_link_libraries(
    maze-render-system-opengl3
    PUBLIC maze-render-system-opengl-core)

if((MAZE_TARGET_PLATFORM_IS_WINDOWS) OR
    (MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN ))
    
    target_include_directories(
        maze-render-system-opengl3
        PUBLIC "${MAZE_DIR}/third-party/pvr_sdk/include")
    
    if(MAZE_ARCH_SUFFIX STREQUAL "x64")
    
        target_link_libraries(
            maze-render-system-opengl3
            PUBLIC "${MAZE_DIR}/third-party/pvr_sdk/lib/Windows_x86_64/libEGL.lib")
            
    else()
    
        target_link_libraries(
            maze-render-system-opengl3
            PUBLIC "${MAZE_DIR}/third-party/pvr_sdk/lib/Windows_x86_32/libEGL.lib")
    
    endif()
    
elseif((MAZE_TARGET_PLATFORM_IS_ANDROID))

    target_link_libraries(
            maze-render-system-opengl3
            PUBLIC EGL
            PUBLIC GLESv3)
    
endif()
    
if((NOT MAZE_TARGET_PLATFORM_IS_IOS) AND
    (NOT MAZE_TARGET_PLATFORM_IS_ANDROID) AND
    (NOT MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN) AND
    (NOT MAZE_TARGET_PLATFORM_IS_LINUX))
    
    find_package(OpenGL REQUIRED)
    target_link_libraries(
        maze-render-system-opengl3
        PUBLIC ${OPENGL_gl_LIBRARY})
endif()

if(MAZE_TARGET_PLATFORM_IS_LINUX)

    find_package(OpenGL REQUIRED)

    target_include_directories(
        maze-render-system-opengl3
        PUBLIC "${MAZE_DIR}/third-party/glad/include")
    
    target_link_libraries(
        maze-render-system-opengl3
        PUBLIC ${OPENGL_glx_LIBRARY}
        # PUBLIC glad
        )

endif()