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
#include "maze-render-system-vulkan/MazeTexture2DMSVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Texture2DMSVulkan
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DMSVulkan, Texture2DMS);

    //////////////////////////////////////////
    Texture2DMSVulkan::Texture2DMSVulkan()
    {
    }

    //////////////////////////////////////////
    Texture2DMSVulkan::~Texture2DMSVulkan()
    {
        destroyTexture();
    }

    //////////////////////////////////////////
    Texture2DMSVulkanPtr Texture2DMSVulkan::Create(RenderSystemVulkan* _renderSystem)
    {
        Texture2DMSVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(Texture2DMSVulkan, object, init((RenderSystem*)_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DMSVulkan::init(RenderSystem* _renderSystem)
    {
        if (!Texture2DMS::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* Texture2DMSVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystem->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    bool Texture2DMSVulkan::isValid()
    {
        return (m_image != VK_NULL_HANDLE);
    }

    //////////////////////////////////////////
    void Texture2DMSVulkan::destroyTexture()
    {
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        if (!renderSystem || (m_image == VK_NULL_HANDLE && m_imageView == VK_NULL_HANDLE))
        {
            m_image = VK_NULL_HANDLE;
            m_imageAllocation = VK_NULL_HANDLE;
            m_imageView = VK_NULL_HANDLE;
            m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            return;
        }

        // See Texture2DVulkan::destroyTexture for why this waits for the device to go
        // idle rather than destroying in-flight resources directly - same caveat applies.
        MAZE_VK_CALL(vkDeviceWaitIdle(renderSystem->getDevice()));

        if (m_imageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(renderSystem->getDevice(), m_imageView, nullptr);
            m_imageView = VK_NULL_HANDLE;
        }

        if (m_image != VK_NULL_HANDLE)
        {
            vmaDestroyImage(renderSystem->getAllocator(), m_image, m_imageAllocation);
            m_image = VK_NULL_HANDLE;
            m_imageAllocation = VK_NULL_HANDLE;
        }

        m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    }

    //////////////////////////////////////////
    bool Texture2DMSVulkan::loadEmpty(
        Vec2U const& _size,
        PixelFormat::Enum _internalPixelFormat,
        S32 _samples)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_size.x == 0 || _size.y == 0, false, "Invalid texture size!");

        VkFormat format = GetPixelFormatVulkan(_internalPixelFormat);
        MAZE_ERROR_RETURN_VALUE_IF(
            format == VK_FORMAT_UNDEFINED,
            false,
            "Unsupported texture pixel format: %d!",
            (S32)_internalPixelFormat);

        destroyTexture();

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        VkDevice device = renderSystem->getDevice();
        VmaAllocator allocator = renderSystem->getAllocator();

        m_size = Vec2S((S32)_size.x, (S32)_size.y);
        m_internalPixelFormat = _internalPixelFormat;
        m_format = format;
        m_aspect = GetImageAspectFlagsVulkan(_internalPixelFormat);

        bool isDepth = IsDepthPixelFormatVulkan(_internalPixelFormat);

        // Clamp the requested sample count to what the device actually supports for
        // this attachment type (mirrors DX11's CheckMultisampleQualityLevels loop)
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(renderSystem->getPhysicalDevice(), &deviceProperties);
        VkSampleCountFlags supportedCounts = isDepth
            ? deviceProperties.limits.framebufferDepthSampleCounts
            : deviceProperties.limits.framebufferColorSampleCounts;

        m_samples = Math::Max(_samples, 1);
        VkSampleCountFlagBits sampleBit = VK_SAMPLE_COUNT_1_BIT;
        while (m_samples > 1)
        {
            sampleBit = (VkSampleCountFlagBits)m_samples;
            if (supportedCounts & sampleBit)
                break;
            m_samples >>= 1;
            sampleBit = VK_SAMPLE_COUNT_1_BIT;
        }
        m_sampleCount = (m_samples > 1) ? (VkSampleCountFlagBits)m_samples : VK_SAMPLE_COUNT_1_BIT;

        VkImageCreateInfo imageInfo;
        memset(&imageInfo, 0, sizeof(imageInfo));
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = _size.x;
        imageInfo.extent.height = _size.y;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // Unlike D3D11 (where multisampled depth SRVs require feature level 10.1+ and
        // silently fall back to a non-sampled depth-only texture), Vulkan has no such
        // feature-level split - SAMPLED_BIT is always legal alongside
        // (DEPTH_STENCIL_)ATTACHMENT_BIT on a multisampled image
        // TRANSFER_SRC_BIT is required by vkCmdResolveImage/vkCmdCopyImage,
        // which RenderBufferVulkan::blit() uses to downsample/copy this
        // texture into a non-multisampled destination
        imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
            (isDepth ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
        imageInfo.samples = m_sampleCount;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo;
        memset(&allocInfo, 0, sizeof(allocInfo));
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

        MAZE_VK_CALL(vmaCreateImage(allocator, &imageInfo, &allocInfo, &m_image, &m_imageAllocation, nullptr));
        MAZE_ERROR_RETURN_VALUE_IF(m_image == VK_NULL_HANDLE, false, "vmaCreateImage (MS) failed!");

        VkImageViewCreateInfo viewInfo;
        memset(&viewInfo, 0, sizeof(viewInfo));
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = m_aspect;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        MAZE_VK_CALL(vkCreateImageView(device, &viewInfo, nullptr, &m_imageView));
        MAZE_ERROR_RETURN_VALUE_IF(m_imageView == VK_NULL_HANDLE, false, "vkCreateImageView (MS) failed!");

        // Transition to its attachment-ready layout up front - RenderBufferVulkan will
        // transition it further (e.g. to SHADER_READ_ONLY_OPTIMAL) once it's actually
        // rendered to and then sampled/resolved
        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();
        VkImageLayout targetLayout = isDepth ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        TransitionImageLayoutVulkan(commandBuffer, m_image, m_aspect, VK_IMAGE_LAYOUT_UNDEFINED, targetLayout, 1u, 1u);
        m_currentLayout = targetLayout;
        renderSystem->endSingleTimeCommands(commandBuffer);

        return true;
    }

    //////////////////////////////////////////
    void Texture2DMSVulkan::transitionTo(VkCommandBuffer _commandBuffer, VkImageLayout _newLayout)
    {
        if (m_currentLayout == _newLayout)
            return;

        TransitionImageLayoutVulkan(
            _commandBuffer, m_image, m_aspect,
            m_currentLayout, _newLayout,
            1u, 1u);
        m_currentLayout = _newLayout;
    }

    //////////////////////////////////////////
    void Texture2DMSVulkan::copyImageFrom(
        U8 const* _pixels,
        PixelFormat::Enum _pixelFormat,
        U32 _width,
        U32 _height,
        U32 _x,
        U32 _y)
    {
        MAZE_ERROR("Copying pixel data into a multisampled texture is not supported!");
    }

    //////////////////////////////////////////
    void Texture2DMSVulkan::saveToFileAsTGA(String const& _fileName, Vec2U _size)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    PixelSheet2D Texture2DMSVulkan::readAsPixelSheet(PixelFormat::Enum _outputFormat)
    {
        MAZE_NOT_IMPLEMENTED;
        return PixelSheet2D();
    }


} // namespace Maze
//////////////////////////////////////////
