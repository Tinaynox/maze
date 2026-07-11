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
#if (!defined(_MazeRenderPassVulkan_hpp_))
#define _MazeRenderPassVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeRenderPass.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderPassVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class RenderPassVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API RenderPassVulkan
        : public RenderPass
    {
    public:

        //////////////////////////////////////////
        using RenderPassDeleter = std::function<void(RenderPassVulkan* _ptr)>;

    public:

        //////////////////////////////////////////
        virtual ~RenderPassVulkan();

        //////////////////////////////////////////
        static RenderPassVulkanPtr Create(
            RenderSystem* _renderSystem,
            RenderPassType _passType,
            RenderPassDeleter const& _deleter = DefaultDelete<RenderPassVulkan>());

        //////////////////////////////////////////
        static RenderPassVulkanPtr Create(
            RenderPassVulkanPtr const& _renderPass,
            RenderPassDeleter const& _deleter = DefaultDelete<RenderPassVulkan>());

        //////////////////////////////////////////
        virtual RenderPassPtr createCopy() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderPassVulkan();

        //////////////////////////////////////////
        virtual bool init(
            RenderSystem* _renderSystem,
            RenderPassType _passType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool init(RenderPassVulkanPtr const& _renderPass);
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderPassVulkan_hpp_
//////////////////////////////////////////
