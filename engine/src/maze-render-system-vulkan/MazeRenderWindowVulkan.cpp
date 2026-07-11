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
#include "maze-render-system-vulkan/MazeRenderWindowVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeStateMachineVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderQueueVulkan.hpp"
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"
#include "maze-core/services/MazeLogStream.hpp"

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#   include "maze-core/system/win/MazeWindowWin.hpp"
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderWindowVulkan
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderWindowVulkan, RenderWindow);

    //////////////////////////////////////////
    RenderWindowVulkan::RenderWindowVulkan()
    {
    }

    //////////////////////////////////////////
    RenderWindowVulkan::~RenderWindowVulkan()
    {
        if (m_renderSystemVulkan && m_renderSystemVulkan->getDevice() != VK_NULL_HANDLE)
            vkDeviceWaitIdle(m_renderSystemVulkan->getDevice());

        destroyDepthBuffer();
        destroySwapChain();
        destroySurface();

        destroySystemWindow();
    }

    //////////////////////////////////////////
    RenderWindowVulkanPtr RenderWindowVulkan::Create(
        RenderSystemVulkan* _renderSystem,
        RenderWindowParams const& _params)
    {
        RenderWindowVulkanPtr window;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(RenderWindowVulkan, window, init(_renderSystem, _params));
        return window;
    }

    //////////////////////////////////////////
    bool RenderWindowVulkan::init(
        RenderSystemVulkan* _renderSystem,
        RenderWindowParams const& _params)
    {
        m_renderSystemVulkan = _renderSystem;

        if (!RenderWindow::init(_params))
            return false;

        if (!createSurface())
            return false;

        if (!createSwapChain(_params.antialiasingLevel))
            return false;

        if (!createDepthBuffer())
            return false;

        m_renderQueue = RenderQueueVulkan::Create(this);
        if (!m_renderQueue)
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* RenderWindowVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystemVulkan;
    }

    //////////////////////////////////////////
    bool RenderWindowVulkan::createSurface()
    {
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
        MAZE_ERROR_RETURN_VALUE_IF(!m_window, false, "System window is null!");

        WindowWin* windowWin = m_window->castRaw<WindowWin>();
        HWND hwnd = windowWin->getHandle();
        MAZE_ERROR_RETURN_VALUE_IF(!hwnd, false, "System window handle is null!");

        VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.hinstance = GetModuleHandle(nullptr);
        surfaceInfo.hwnd = hwnd;

        MAZE_VK_CALL(vkCreateWin32SurfaceKHR(m_renderSystemVulkan->getInstance(), &surfaceInfo, nullptr, &m_surface));
        MAZE_ERROR_RETURN_VALUE_IF(m_surface == VK_NULL_HANDLE, false, "vkCreateWin32SurfaceKHR failed!");

        VkBool32 presentSupported = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(
            m_renderSystemVulkan->getPhysicalDevice(),
            m_renderSystemVulkan->getPresentQueueFamilyIndex(),
            m_surface, &presentSupported);
        MAZE_ERROR_RETURN_VALUE_IF(!presentSupported, false, "Graphics queue family does not support presentation to this surface!");

        return true;
#else
        MAZE_ERROR_RETURN_VALUE(false, "Vulkan render window is only implemented for Windows in this backend!");
#endif
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::destroySurface()
    {
        if (m_surface != VK_NULL_HANDLE && m_renderSystemVulkan)
        {
            vkDestroySurfaceKHR(m_renderSystemVulkan->getInstance(), m_surface, nullptr);
            m_surface = VK_NULL_HANDLE;
        }
    }

    //////////////////////////////////////////
    bool RenderWindowVulkan::createSwapChain(S32 _antialiasingLevel)
    {
        MAZE_ERROR_RETURN_VALUE_IF(m_surface == VK_NULL_HANDLE, false, "Surface is null!");

        // The system window can already be gone by the time a deferred
        // recreate (e.g. from a VK_ERROR_OUT_OF_DATE_KHR caught in
        // swapBuffers()/processRenderTargetWillSet()) runs, particularly
        // mid-shutdown - bail rather than dereferencing a null window
        if (!m_window)
            return false;

        RenderSystemVulkan* rs = m_renderSystemVulkan;
        VkPhysicalDevice physicalDevice = rs->getPhysicalDevice();

        VkSurfaceCapabilitiesKHR capabilities;
        VkResult capabilitiesResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &capabilities);
        // The surface can go invalid out from under us (e.g. mid-shutdown,
        // while the window is being torn down and the render loop races one
        // more frame against it) - bail instead of feeding vkCreateSwapchainKHR
        // a garbage/uninitialized VkSurfaceCapabilitiesKHR, which crashed some
        // drivers rather than failing gracefully. VK_ERROR_SURFACE_LOST_KHR
        // specifically is an expected, anticipated outcome of that race, not
        // a real bug - fail quietly instead of through the loud
        // log+debug-break MAZE_ERROR path so it doesn't look like (or act
        // like, via the debug break) a crash during normal shutdown.
        if (capabilitiesResult == VK_ERROR_SURFACE_LOST_KHR)
        {
            Debug::Log("RenderWindowVulkan: surface lost (window closing?), skipping swap chain (re)creation.");
            return false;
        }
        MAZE_ERROR_RETURN_VALUE_IF(capabilitiesResult != VK_SUCCESS, false,
            "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed! result=%d", (S32)capabilitiesResult);

        U32 formatCount = 0u;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, nullptr);
        Vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, formats.data());

        VkSurfaceFormatKHR chosenFormat = formats[0];
        for (VkSurfaceFormatKHR const& format : formats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                chosenFormat = format;
                break;
            }
        }
        m_swapChainFormat = chosenFormat.format;

        U32 presentModeCount = 0u;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, nullptr);
        Vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, presentModes.data());

        VkPresentModeKHR chosenPresentMode = VK_PRESENT_MODE_FIFO_KHR; // always available, vsync'd
        if (m_vsync <= 0)
        {
            for (VkPresentModeKHR mode : presentModes)
            {
                if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
                {
                    chosenPresentMode = mode;
                    break;
                }
            }
        }

        Vec2U clientSize = m_window->getClientSize();

        // A minimized/zero-area window is a normal, expected state per the
        // Vulkan spec (surfaceCapabilities.currentExtent can legally be
        // 0x0) - skip creating a degenerate swapchain rather than feeding
        // the driver a zero-sized extent, which some drivers don't handle
        // gracefully
        if (clientSize.x == 0u || clientSize.y == 0u)
            return false;

        m_swapChainExtent.x = Math::Clamp(clientSize.x, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        m_swapChainExtent.y = Math::Clamp(clientSize.y, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        U32 imageCount = capabilities.minImageCount + 1u;
        if (capabilities.maxImageCount > 0u && imageCount > capabilities.maxImageCount)
            imageCount = capabilities.maxImageCount;

        // Vulkan swapchain images are always single-sample - MSAA is not yet
        // wired up for window render targets in this backend (would need a
        // separate multisampled color attachment resolved into the swapchain
        // image before present, unlike DX11's legacy-blit-model MSAA
        // backbuffer). Accepted but currently ignored - known gap.
        m_antialiasingLevel = 1;
        MAZE_WARNING_IF(_antialiasingLevel > 1, "RenderWindowVulkan: window MSAA is not implemented yet, ignoring antialiasingLevel=%d", _antialiasingLevel);

        VkSwapchainCreateInfoKHR swapChainInfo = {};
        swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapChainInfo.surface = m_surface;
        swapChainInfo.minImageCount = imageCount;
        swapChainInfo.imageFormat = chosenFormat.format;
        swapChainInfo.imageColorSpace = chosenFormat.colorSpace;
        swapChainInfo.imageExtent = { m_swapChainExtent.x, m_swapChainExtent.y };
        swapChainInfo.imageArrayLayers = 1u;
        swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainInfo.preTransform = capabilities.currentTransform;
        swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapChainInfo.presentMode = chosenPresentMode;
        swapChainInfo.clipped = VK_TRUE;
        swapChainInfo.oldSwapchain = m_swapChain;

        VkSwapchainKHR newSwapChain = VK_NULL_HANDLE;
        MAZE_VK_CALL(vkCreateSwapchainKHR(rs->getDevice(), &swapChainInfo, nullptr, &newSwapChain));
        MAZE_ERROR_RETURN_VALUE_IF(newSwapChain == VK_NULL_HANDLE, false, "vkCreateSwapchainKHR failed!");

        destroySwapChain();
        m_swapChain = newSwapChain;

        U32 actualImageCount = 0u;
        vkGetSwapchainImagesKHR(rs->getDevice(), m_swapChain, &actualImageCount, nullptr);
        m_swapChainImages.resize(actualImageCount);
        vkGetSwapchainImagesKHR(rs->getDevice(), m_swapChain, &actualImageCount, m_swapChainImages.data());

        m_swapChainImageViews.resize(actualImageCount);
        m_imageAvailableSemaphores.resize(actualImageCount);
        for (U32 i = 0; i < actualImageCount; ++i)
        {
            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_swapChainImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_swapChainFormat;
            viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u };
            MAZE_VK_CALL(vkCreateImageView(rs->getDevice(), &viewInfo, nullptr, &m_swapChainImageViews[i]));

            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            MAZE_VK_CALL(vkCreateSemaphore(rs->getDevice(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]));
        }

        Debug::Log("RenderWindowVulkan: Swap chain created (%ux%u, %u images).", m_swapChainExtent.x, m_swapChainExtent.y, actualImageCount);

        return true;
    }

    //////////////////////////////////////////
    bool RenderWindowVulkan::createDepthBuffer()
    {
        RenderSystemVulkan* rs = m_renderSystemVulkan;

        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent = { m_swapChainExtent.x, m_swapChainExtent.y, 1u };
        imageInfo.mipLevels = 1u;
        imageInfo.arrayLayers = 1u;
        imageInfo.format = m_depthFormat;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        MAZE_VK_CALL(vmaCreateImage(rs->getAllocator(), &imageInfo, &allocInfo, &m_depthImage, &m_depthImageAllocation, nullptr));
        MAZE_ERROR_RETURN_VALUE_IF(m_depthImage == VK_NULL_HANDLE, false, "Depth image creation failed!");

        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_depthImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_depthFormat;
        viewInfo.subresourceRange = { (VkImageAspectFlags)(VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT), 0u, 1u, 0u, 1u };
        MAZE_VK_CALL(vkCreateImageView(rs->getDevice(), &viewInfo, nullptr, &m_depthImageView));

        VkCommandBuffer cmd = rs->beginSingleTimeCommands();
        TransitionImageLayoutVulkan(
            cmd, m_depthImage,
            VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        rs->endSingleTimeCommands(cmd);

        return true;
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::destroyDepthBuffer()
    {
        if (!m_renderSystemVulkan)
            return;

        VkDevice device = m_renderSystemVulkan->getDevice();
        if (m_depthImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(device, m_depthImageView, nullptr);
            m_depthImageView = VK_NULL_HANDLE;
        }
        if (m_depthImage != VK_NULL_HANDLE)
        {
            vmaDestroyImage(m_renderSystemVulkan->getAllocator(), m_depthImage, m_depthImageAllocation);
            m_depthImage = VK_NULL_HANDLE;
        }
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::destroySwapChain()
    {
        if (!m_renderSystemVulkan)
            return;

        VkDevice device = m_renderSystemVulkan->getDevice();

        for (VkSemaphore semaphore : m_imageAvailableSemaphores)
            if (semaphore != VK_NULL_HANDLE)
                vkDestroySemaphore(device, semaphore, nullptr);
        m_imageAvailableSemaphores.clear();

        for (VkImageView view : m_swapChainImageViews)
            if (view != VK_NULL_HANDLE)
                vkDestroyImageView(device, view, nullptr);
        m_swapChainImageViews.clear();
        m_swapChainImages.clear();

        if (m_swapChain != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(device, m_swapChain, nullptr);
            m_swapChain = VK_NULL_HANDLE;
        }
    }

    //////////////////////////////////////////
    bool RenderWindowVulkan::recreateSwapChain()
    {
        if (!m_renderSystemVulkan || !m_window || m_surface == VK_NULL_HANDLE)
            return false;

        vkDeviceWaitIdle(m_renderSystemVulkan->getDevice());

        destroyDepthBuffer();

        if (!createSwapChain(m_antialiasingLevel))
            return false; // skipped (zero-sized/minimized window) or failed - leave the depth buffer torn down until the next resize/frame retries

        return createDepthBuffer();
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::notifyWindowSizeChanged(Window* _window)
    {
        RenderWindow::notifyWindowSizeChanged(_window);

        if (!m_swapChain)
            return;

        recreateSwapChain();
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::endDraw()
    {
        // Queue the acquired swapchain image's present-layout transition -
        // this render target's dynamic-rendering scope (vkCmdBeginRendering)
        // is still open at this point (it only closes in
        // RenderSystemVulkan::endFrame()'s unbindRenderTarget() call, which
        // happens later, from swapBuffers()), and vkCmdPipelineBarrier is
        // illegal to record while that scope is open. Deferring through
        // addPendingImageTransition() ensures the transition is recorded
        // after the scope actually closes but before the command buffer
        // stops recording.
        if (m_imageAcquired && !m_presentTransitionQueued)
        {
            getRenderSystemVulkanRaw()->addPendingImageTransition(
                m_swapChainImages[m_currentImageIndex], VK_IMAGE_ASPECT_COLOR_BIT,
                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
            m_presentTransitionQueued = true;
        }

        RenderWindow::endDraw();
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::swapBuffers()
    {
        if (!m_swapChain || !m_imageAcquired)
            return;

        RenderSystemVulkan* rs = m_renderSystemVulkan;

        VkSemaphore renderFinished = rs->endFrame();

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = renderFinished != VK_NULL_HANDLE ? 1u : 0u;
        presentInfo.pWaitSemaphores = &renderFinished;
        presentInfo.swapchainCount = 1u;
        presentInfo.pSwapchains = &m_swapChain;
        presentInfo.pImageIndices = &m_currentImageIndex;

        VkResult result = vkQueuePresentKHR(rs->getPresentQueue(), &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            recreateSwapChain();
        }
        else
        if (result == VK_ERROR_SURFACE_LOST_KHR)
        {
            // Expected mid-shutdown race (see createSwapChain's identical
            // handling) - quiet, no debug-break
            Debug::Log("RenderWindowVulkan: surface lost while presenting (window closing?).");
        }
        else
        {
            MAZE_ERROR_IF(result != VK_SUCCESS, "vkQueuePresentKHR failed! result=%d", (S32)result);
        }

        m_imageAcquired = false;
    }

    //////////////////////////////////////////
    bool RenderWindowVulkan::processRenderTargetWillSet()
    {
        if (!m_swapChain)
            return false;

        RenderSystemVulkan* rs = m_renderSystemVulkan;

        // A frame is already open by this point (RenderSystemVulkan::setCurrentRenderTarget
        // calls ensureFrameOpen() before this) - acquire this window's next
        // swapchain image and register the image-available semaphore as a
        // wait dependency on the frame's eventual submission
        static U32 s_semaphoreCycler = 0u;
        VkSemaphore acquireSemaphore = m_imageAvailableSemaphores[s_semaphoreCycler % m_imageAvailableSemaphores.size()];
        ++s_semaphoreCycler;

        VkResult result = vkAcquireNextImageKHR(
            rs->getDevice(), m_swapChain, UINT64_MAX,
            acquireSemaphore, VK_NULL_HANDLE, &m_currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return false;
        }

        if (result == VK_ERROR_SURFACE_LOST_KHR)
        {
            // Expected mid-shutdown race (see createSwapChain's identical
            // handling) - quiet, no debug-break
            Debug::Log("RenderWindowVulkan: surface lost while acquiring an image (window closing?).");
            return false;
        }

        MAZE_ERROR_RETURN_VALUE_IF(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR, false, "vkAcquireNextImageKHR failed! result=%d", (S32)result);

        rs->addFrameWaitSemaphore(acquireSemaphore, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
        m_imageAcquired = true;
        m_presentTransitionQueued = false;

        return true;
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::processRenderTargetSet()
    {
        StateMachineVulkan* stateMachine = getRenderSystemVulkanRaw()->getStateMachine();

        // A previously-bound render target's dynamic-rendering scope may
        // still be open on this command buffer (bindRenderTarget() below
        // would close it, but only after the transition call right below
        // this comment has already tried to record a barrier - which is
        // illegal while that scope is still open)
        stateMachine->unbindRenderTarget();

        VkImageView colorView = m_swapChainImageViews[m_currentImageIndex];

        VkCommandBuffer cmd = getRenderSystemVulkanRaw()->getCurrentCommandBuffer();
        TransitionImageLayoutVulkan(
            cmd, m_swapChainImages[m_currentImageIndex], VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        stateMachine->bindRenderTarget(
            &colorView,
            1,
            m_depthImageView,
            getRenderTargetSize(),
            false,
            m_swapChainFormat,
            m_depthFormat);
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::processRenderTargetWillReset()
    {
        // The present-ready layout transition is handled deterministically in
        // endDraw() instead of here (see its comment) - this override
        // intentionally does nothing further
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::setVSync(S32 _vsync)
    {
        if (m_vsync == _vsync)
            return;

        m_vsync = _vsync;

        if (m_swapChain)
            recreateSwapChain();
    }

    //////////////////////////////////////////
    bool RenderWindowVulkan::isReadyToRender() const
    {
        if (!m_swapChain)
            return false;

        return RenderWindow::isReadyToRender();
    }


} // namespace Maze
//////////////////////////////////////////
