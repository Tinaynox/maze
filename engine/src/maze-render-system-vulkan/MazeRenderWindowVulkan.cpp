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
            m_renderSystemVulkan->waitDeviceIdleSafe();

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

        // Vulkan swapchain images are always single-sample (unlike DX11's
        // legacy-blit-model swap effect, which supports a multisampled
        // backbuffer directly) - MSAA renders into a separate multisampled
        // color image instead, resolved into the swapchain image at the end
        // of each frame's rendering scope (see createMSAAColorBuffer() and
        // processRenderTargetSet()). Clamp to what the device actually
        // supports for a window render target, same as
        // RenderWindowDX11::createSwapChain().
        m_antialiasingLevel = Math::Clamp(_antialiasingLevel, 1, rs->getWindowMaxAntialiasingLevelSupport());

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
        m_renderFinishedSemaphores.resize(actualImageCount);
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
            MAZE_VK_CALL(vkCreateSemaphore(rs->getDevice(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]));
        }

        if (m_antialiasingLevel > 1)
            MAZE_ERROR_RETURN_VALUE_IF(!createMSAAColorBuffer(), false, "MSAA color buffer creation failed!");

        Debug::Log("RenderWindowVulkan: Swap chain created (%ux%u, %u images, AA=%d).", m_swapChainExtent.x, m_swapChainExtent.y, actualImageCount, m_antialiasingLevel);

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
        // Must match the color attachment's sample count - Vulkan requires
        // every attachment in a rendering scope to share the same sample
        // count. m_msaaSampleCount is set (or reset to 1) by
        // createMSAAColorBuffer()/destroyMSAAColorBuffer(), called before
        // this from createSwapChain(). The depth buffer itself is never
        // resolved (nothing needs to read it back after the window's own
        // rendering scope ends) - it only needs to exist at the right
        // sample count for per-sample depth testing during MSAA rendering.
        imageInfo.samples = m_msaaSampleCount;
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
    bool RenderWindowVulkan::createMSAAColorBuffer()
    {
        destroyMSAAColorBuffer();

        RenderSystemVulkan* rs = m_renderSystemVulkan;

        // Clamp the requested level to what the device supports for a color
        // attachment (mirrors Texture2DMSVulkan::loadEmpty()'s identical
        // clamping loop) - m_antialiasingLevel was already clamped against
        // getWindowMaxAntialiasingLevelSupport() in createSwapChain(), so
        // this should always find an exact match, but stays defensive.
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(rs->getPhysicalDevice(), &deviceProperties);
        VkSampleCountFlags supportedCounts = deviceProperties.limits.framebufferColorSampleCounts;
        S32 samples = m_antialiasingLevel;
        VkSampleCountFlagBits sampleBit = VK_SAMPLE_COUNT_1_BIT;
        while (samples > 1)
        {
            sampleBit = (VkSampleCountFlagBits)samples;
            if (supportedCounts & sampleBit)
                break;
            samples >>= 1;
            sampleBit = VK_SAMPLE_COUNT_1_BIT;
        }
        m_msaaSampleCount = sampleBit;

        if (m_msaaSampleCount == VK_SAMPLE_COUNT_1_BIT)
            return true; // nothing supported above 1x - render directly to the swapchain image

        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent = { m_swapChainExtent.x, m_swapChainExtent.y, 1u };
        imageInfo.mipLevels = 1u;
        imageInfo.arrayLayers = 1u;
        imageInfo.format = m_swapChainFormat;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        imageInfo.samples = m_msaaSampleCount;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        MAZE_VK_CALL(vmaCreateImage(rs->getAllocator(), &imageInfo, &allocInfo, &m_msaaColorImage, &m_msaaColorImageAllocation, nullptr));
        MAZE_ERROR_RETURN_VALUE_IF(m_msaaColorImage == VK_NULL_HANDLE, false, "MSAA color image creation failed!");

        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_msaaColorImage;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_swapChainFormat;
        viewInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 1u, 0u, 1u };
        MAZE_VK_CALL(vkCreateImageView(rs->getDevice(), &viewInfo, nullptr, &m_msaaColorImageView));

        // Unlike the swapchain image (which cycles between
        // COLOR_ATTACHMENT_OPTIMAL and PRESENT_SRC_KHR every frame), this
        // image is never presented and never used for anything else - a
        // single, permanent transition here is all it ever needs
        VkCommandBuffer cmd = rs->beginSingleTimeCommands();
        TransitionImageLayoutVulkan(
            cmd, m_msaaColorImage, VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        rs->endSingleTimeCommands(cmd);

        return true;
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::destroyMSAAColorBuffer()
    {
        m_msaaSampleCount = VK_SAMPLE_COUNT_1_BIT;

        if (!m_renderSystemVulkan)
            return;

        VkDevice device = m_renderSystemVulkan->getDevice();
        if (m_msaaColorImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(device, m_msaaColorImageView, nullptr);
            m_msaaColorImageView = VK_NULL_HANDLE;
        }
        if (m_msaaColorImage != VK_NULL_HANDLE)
        {
            vmaDestroyImage(m_renderSystemVulkan->getAllocator(), m_msaaColorImage, m_msaaColorImageAllocation);
            m_msaaColorImage = VK_NULL_HANDLE;
        }
    }

    //////////////////////////////////////////
    void RenderWindowVulkan::destroySwapChain()
    {
        if (!m_renderSystemVulkan)
            return;

        destroyMSAAColorBuffer();

        VkDevice device = m_renderSystemVulkan->getDevice();

        for (VkSemaphore semaphore : m_imageAvailableSemaphores)
            if (semaphore != VK_NULL_HANDLE)
                vkDestroySemaphore(device, semaphore, nullptr);
        m_imageAvailableSemaphores.clear();

        for (VkSemaphore semaphore : m_renderFinishedSemaphores)
            if (semaphore != VK_NULL_HANDLE)
                vkDestroySemaphore(device, semaphore, nullptr);
        m_renderFinishedSemaphores.clear();

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

        // A resize can arrive mid-frame - on Windows, dragging the window
        // border enters a modal sizing loop where the OS pumps WM_SIZE
        // synchronously, which can interrupt whatever frame was already
        // being recorded. waitDeviceIdleSafe() forces that frame closed
        // first (submitted while its recorded commands still reference the
        // still-valid old resources) before waiting on the device, so
        // nothing recording or executing can reference what's about to be
        // destroyed below. Confirmed via a VK_ERROR_DEVICE_LOST crash during
        // a window resize-drag - see waitDeviceIdleSafe()'s banner comment.
        m_renderSystemVulkan->waitDeviceIdleSafe();

        destroyDepthBuffer();

        // The swapchain image (if any) acquired for the frame just
        // force-closed above belongs to the swapchain being destroyed next -
        // that tracking must not carry over into the next real frame's
        // acquire/present cycle
        m_imageAcquired = false;
        m_presentTransitionQueued = false;
        m_colorImageTransitionedFromUndefined = false;

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

        // Indexed by the acquired swapchain image, not the frame-in-flight
        // slot - see m_renderFinishedSemaphores' banner comment and
        // RenderSystemVulkan::endFrame()'s
        VkSemaphore renderFinished = m_renderFinishedSemaphores[m_currentImageIndex];
        rs->endFrame(renderFinished);

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1u;
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

        // setCurrentRenderTarget() can (and does, every real frame) target
        // this same window more than once - e.g. RenderControllerModule3D's
        // pass followed later by RenderControllerModule2D's pass, both
        // targeting the window (see endDraw()'s identical "can be called
        // multiple times per frame" banner comment). Without this guard,
        // each extra call re-acquired a NEW swapchain image via
        // vkAcquireNextImageKHR and overwrote m_currentImageIndex, silently
        // orphaning whichever image the earlier pass's draws had just been
        // recorded into - it was never presented, since only the LAST
        // acquired image's index makes it to swapBuffers()'s
        // vkQueuePresentKHR. That produced a frozen/stale backdrop (the
        // orphaned image's ground/skybox draws never shown) with only the
        // later pass's draws (2D/UI) visibly layering onto whatever stale
        // content the newly-(re)acquired image already had. m_imageAcquired
        // is reset once per real frame, in swapBuffers(), so it's the
        // correct signal for "already acquired this frame, reuse it."
        if (m_imageAcquired)
            return true;

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
        m_colorImageTransitionedFromUndefined = false;

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

        // setCurrentRenderTarget() targets this same window more than once
        // per real frame (e.g. RenderControllerModule3D's pass followed
        // later by RenderControllerModule2D's pass - see
        // processRenderTargetWillSet()'s identical "multiple times per
        // frame" banner comment), and processRenderTargetSet() runs again on
        // every one of those rebinds. Declaring oldLayout=UNDEFINED here
        // tells Vulkan the image's previous contents don't need to be
        // preserved - correct for the FIRST bind of a freshly-acquired
        // image (which really is undefined at that point), but wrong for
        // every later rebind within the same frame, where the image already
        // holds this frame's earlier-pass draws (in
        // COLOR_ATTACHMENT_OPTIMAL) that must survive into the next pass's
        // loadOp=LOAD. Without this guard, a later pass (e.g. 2D/UI text)
        // would layer its draws on top of a declared-undefined image,
        // confirmed via RenderDoc showing the swapchain image as fully
        // UNDEFINED_IMG by the time of the text draw despite valid earlier
        // draws with C=Store in the same frame.
        if (!m_colorImageTransitionedFromUndefined)
        {
            VkCommandBuffer cmd = getRenderSystemVulkanRaw()->getCurrentCommandBuffer();
            TransitionImageLayoutVulkan(
                cmd, m_swapChainImages[m_currentImageIndex], VK_IMAGE_ASPECT_COLOR_BIT,
                VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            m_colorImageTransitionedFromUndefined = true;
        }

        // With MSAA active, the actual color attachment rendered into is the
        // (permanently COLOR_ATTACHMENT_OPTIMAL, never presented) MSAA
        // image, and the swapchain image becomes a resolve target instead -
        // it still needs the same UNDEFINED->COLOR_ATTACHMENT_OPTIMAL
        // transition above (a resolve write targets it exactly like a
        // regular attachment write would, and expects a valid declared
        // layout), it just no longer receives draws directly.
        if (m_msaaSampleCount != VK_SAMPLE_COUNT_1_BIT)
        {
            VkImageView resolveView = colorView;
            colorView = m_msaaColorImageView;
            stateMachine->bindRenderTarget(
                &colorView,
                1,
                m_depthImageView,
                getRenderTargetSize(),
                false,
                m_swapChainFormat,
                m_depthFormat,
                m_msaaSampleCount,
                &resolveView);
        }
        else
        {
            stateMachine->bindRenderTarget(
                &colorView,
                1,
                m_depthImageView,
                getRenderTargetSize(),
                false,
                m_swapChainFormat,
                m_depthFormat);
        }
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
