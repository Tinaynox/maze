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
set(MONO_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/mono/msvc/include")
include_directories(MONO_INCLUDE_DIR)

if(MAZE_TARGET_PLATFORM_IS_WINDOWS)
    if(${MAZE_ARCH_SUFFIX} STREQUAL "x64")
        set(MONO_ARCH_TYPE "x64")
    else()
        set(MONO_ARCH_TYPE "Win32")
    endif()
    
    set(MONO_LIB "${CMAKE_CURRENT_LIST_DIR}/mono/msvc/build/sgen/${MONO_ARCH_TYPE}/lib/$<CONFIG>/mono-2.0-sgen.lib" CACHE PATH "mono-2.0-sgen" FORCE)
endif()