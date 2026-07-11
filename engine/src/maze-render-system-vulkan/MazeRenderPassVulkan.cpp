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
#include "MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeRenderPassVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderPassVulkan
    //
    //////////////////////////////////////////
    RenderPassVulkan::RenderPassVulkan()
    {
    }

    //////////////////////////////////////////
    RenderPassVulkan::~RenderPassVulkan()
    {
    }

    //////////////////////////////////////////
    RenderPassVulkanPtr RenderPassVulkan::Create(
        RenderSystem* _renderSystem,
        RenderPassType _passType,
        RenderPassDeleter const& _deleter)
    {
        RenderPassVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR_EX(RenderPassVulkan, object, _deleter, init(_renderSystem, _passType));
        return object;
    }

    //////////////////////////////////////////
    RenderPassVulkanPtr RenderPassVulkan::Create(
        RenderPassVulkanPtr const& _renderPass,
        RenderPassDeleter const& _deleter)
    {
        RenderPassVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR_EX(RenderPassVulkan, object, _deleter, init(_renderPass));
        return object;
    }

    //////////////////////////////////////////
    bool RenderPassVulkan::init(
        RenderSystem* _renderSystem,
        RenderPassType _passType)
    {
        if (!RenderPass::init(_renderSystem, _passType))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool RenderPassVulkan::init(RenderPassVulkanPtr const& _renderPass)
    {
        if (!RenderPass::init(_renderPass))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderPassPtr RenderPassVulkan::createCopy()
    {
        return RenderPassVulkan::Create(cast<RenderPassVulkan>());
    }


} // namespace Maze
//////////////////////////////////////////
