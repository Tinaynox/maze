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
    maze-render-system-vulkan
    INCLUDE_DIR "include/maze-render-system-vulkan"
    SRC_DIR "src/maze-render-system-vulkan"
    FORWARD_HEADER MazeRenderSystemVulkanHeader)

include("${MAZE_DIR}/third-party/vma.cmake")
include("${MAZE_DIR}/third-party/spirv-reflect.cmake")

target_link_libraries(
    maze-render-system-vulkan
    PUBLIC maze-graphics
    PUBLIC Vulkan::Vulkan
    PUBLIC Vulkan::shaderc_combined
    PUBLIC spirv-reflect)
