//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#pragma once
#if (!defined(_MazeRenderSystemVulkanConfig_hpp_))
#define _MazeRenderSystemVulkanConfig_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Struct RenderSystemVulkanConfig
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_VULKAN_API RenderSystemVulkanConfig
    {
        // Enables VK_LAYER_KHRONOS_validation + debug utils messenger (requires Vulkan SDK installed)
        bool validationLayer = false;

        // Physical device index to create the logical device on (-1 - pick best discrete GPU)
        S32 physicalDeviceIndex = -1;

        // Number of frames that may be in flight concurrently (double/triple buffering)
        U32 framesInFlight = 2u;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemVulkanConfig_hpp_
//////////////////////////////////////////
