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
    maze-sound-system-openal
    INCLUDE_DIR "include/maze-sound-system-openal"
    SRC_DIR "src/maze-sound-system-openal"
    FORWARD_HEADER MazeSoundSystemOpenALHeader)
    
target_link_libraries(
    maze-sound-system-openal
    PUBLIC maze-sound)

if((MAZE_TARGET_PLATFORM_IS_WINDOWS) OR
   (MAZE_TARGET_PLATFORM_IS_EMSCRIPTEN ))
    
    target_include_directories(
        maze-sound-system-openal
        PUBLIC "${PROJECT_SOURCE_DIR}/third-party/OpenAL 1.1 SDK/include")
    
    if(MAZE_ARCH_SUFFIX STREQUAL "x64")
    
        target_link_libraries(
            maze-sound-system-openal
            PUBLIC "${PROJECT_SOURCE_DIR}/third-party/OpenAL 1.1 SDK/libs/Win64/OpenAL32.lib")
            
    else()
    
        target_link_libraries(
            maze-sound-system-openal
            PUBLIC "${PROJECT_SOURCE_DIR}/third-party/OpenAL 1.1 SDK/libs/Win32/OpenAL32.lib")
    
    endif()
    
elseif((MAZE_TARGET_PLATFORM_IS_ANDROID))

    target_link_libraries(
            maze-sound-system-openal
            PUBLIC OpenAL)

elseif((MAZE_TARGET_PLATFORM_IS_OSX))
    find_package(OpenAL REQUIRED)
    target_link_libraries(
            maze-sound-system-openal
            PUBLIC ${OPENAL_LIBRARY})

endif()
    

if(MAZE_TARGET_PLATFORM_IS_LINUX)

    find_package(OpenAL REQUIRED)
    
    target_link_libraries(
        maze-sound-system-openal
        PUBLIC ${OPENAL_alx_LIBRARY}
        )

endif()