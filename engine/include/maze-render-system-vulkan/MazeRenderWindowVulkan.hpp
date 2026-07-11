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
#if (!defined(_MazeRenderWindowVulkan_hpp_))
#define _MazeRenderWindowVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeRenderWindow.hpp"
#include "maze-graphics/MazeRenderWindowParams.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(RenderWindowVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class RenderWindowVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API RenderWindowVulkan
        : public RenderWindow
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderWindowVulkan, RenderWindow);

    public:

        //////////////////////////////////////////
        virtual ~RenderWindowVulkan();

        //////////////////////////////////////////
        static RenderWindowVulkanPtr Create(
            RenderSystemVulkan* _renderSystem,
            RenderWindowParams const& _params);


        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;


        //////////////////////////////////////////
        virtual void swapBuffers() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void endDraw() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool processRenderTargetWillSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetSet() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void processRenderTargetWillReset() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setVSync(S32 _vsync) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool isReadyToRender() const MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        RenderWindowVulkan();

        //////////////////////////////////////////
        using RenderWindow::init;

        //////////////////////////////////////////
        bool init(
            RenderSystemVulkan* _renderSystem,
            RenderWindowParams const& _params);

        //////////////////////////////////////////
        virtual void notifyWindowSizeChanged(Window* _window) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool createSurface();

        //////////////////////////////////////////
        bool createSwapChain(S32 _antialiasingLevel);

        //////////////////////////////////////////
        bool createDepthBuffer();

        //////////////////////////////////////////
        void destroyDepthBuffer();

        //////////////////////////////////////////
        void destroySwapChain();

        //////////////////////////////////////////
        // Shared recreate-on-resize/out-of-date path used by
        // notifyWindowSizeChanged(), swapBuffers() and processRenderTargetWillSet() -
        // guards against recreating a swapchain for a zero-sized or
        // already-torn-down window (e.g. mid-shutdown, where the surface may
        // no longer be valid), which crashed the driver when done unconditionally.
        // Returns false if recreation was skipped or failed.
        bool recreateSwapChain();

        //////////////////////////////////////////
        void destroySurface();

    protected:
        RenderSystemVulkan* m_renderSystemVulkan = nullptr;

        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
        VkFormat m_swapChainFormat = VK_FORMAT_UNDEFINED;
        Vec2U m_swapChainExtent = Vec2U::c_zero;

        Vector<VkImage> m_swapChainImages;
        Vector<VkImageView> m_swapChainImageViews;
        Vector<VkSemaphore> m_imageAvailableSemaphores; // one per swapchain image
        U32 m_currentImageIndex = 0u;
        bool m_imageAcquired = false;

        // Guards against queueing the present-layout transition more than
        // once for the same acquired image - endDraw() can be called
        // multiple times per frame for the same render target (2D and 3D
        // render controller modules each call it on the same window when a
        // scene draws both), but m_imageAcquired alone doesn't prevent a
        // second queueing since it's only cleared in swapBuffers(). A
        // duplicate queued transition would record a second barrier whose
        // declared oldLayout (COLOR_ATTACHMENT_OPTIMAL) no longer matches
        // the image's actual layout after the first transition already ran.
        bool m_presentTransitionQueued = false;

        VkImage m_depthImage = VK_NULL_HANDLE;
        VmaAllocation m_depthImageAllocation = VK_NULL_HANDLE;
        VkImageView m_depthImageView = VK_NULL_HANDLE;
        VkFormat m_depthFormat = VK_FORMAT_D24_UNORM_S8_UINT;

        S32 m_antialiasingLevel = 0;
        S32 m_vsync = 1;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderWindowVulkan_hpp_
//////////////////////////////////////////
