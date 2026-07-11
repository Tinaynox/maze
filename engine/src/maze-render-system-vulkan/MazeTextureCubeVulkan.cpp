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
#include "maze-render-system-vulkan/MazeTextureCubeVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TextureCubeVulkan
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TextureCubeVulkan, TextureCube);

    //////////////////////////////////////////
    TextureCubeVulkan::TextureCubeVulkan()
    {
    }

    //////////////////////////////////////////
    TextureCubeVulkan::~TextureCubeVulkan()
    {
        destroyTexture();
    }

    //////////////////////////////////////////
    TextureCubeVulkanPtr TextureCubeVulkan::Create(RenderSystemVulkan* _renderSystem)
    {
        TextureCubeVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(TextureCubeVulkan, object, init((RenderSystem*)_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::init(RenderSystem* _renderSystem)
    {
        if (!TextureCube::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* TextureCubeVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystem->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::isValid()
    {
        return (m_image != VK_NULL_HANDLE);
    }

    //////////////////////////////////////////
    void TextureCubeVulkan::destroyTexture()
    {
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        if (!renderSystem || (m_image == VK_NULL_HANDLE && m_imageView == VK_NULL_HANDLE))
        {
            m_image = VK_NULL_HANDLE;
            m_imageAllocation = VK_NULL_HANDLE;
            m_imageView = VK_NULL_HANDLE;
            m_hasMipmapsGenerationSupport = false;
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

        m_hasMipmapsGenerationSupport = false;
        m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::loadTexture(
        Vector<PixelSheet2D> const _pixelSheets[6],
        PixelFormat::Enum _internalPixelFormat)
    {
        for (Size face = 0; face < 6; ++face)
            MAZE_ERROR_RETURN_VALUE_IF(_pixelSheets[face].empty(), false, "Cube face %d has no pixel sheets!", (S32)face);

        if (_internalPixelFormat == PixelFormat::None)
            _internalPixelFormat = _pixelSheets[0][0].getFormat();

        // Most desktop Vulkan drivers don't support 24-bit RGB for optimal tiling - expand
        // to RGBA on upload, same as Texture2DVulkan/DX11's TextureCubeDX11
        bool expandRGB = (_internalPixelFormat == PixelFormat::RGB_U8);
        // See the identical DEPTH_U24 comment in Texture2DVulkan::loadTextureImpl
        bool expandDepth24 = (_internalPixelFormat == PixelFormat::DEPTH_U24);

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

        Vec2S size = _pixelSheets[0][0].getSize();
        m_size = size;
        m_internalPixelFormat = _internalPixelFormat;
        m_format = format;

        bool isCompressed = PixelFormat::IsCompressed(_internalPixelFormat);
        m_hasMipmapsGenerationSupport = !isCompressed;

        m_mipLevels = (U32)_pixelSheets[0].size();

        VkImageCreateInfo imageInfo;
        memset(&imageInfo, 0, sizeof(imageInfo));
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = (U32)size.x;
        imageInfo.extent.height = (U32)size.y;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = m_mipLevels;
        imageInfo.arrayLayers = 6;
        imageInfo.format = format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        if (m_hasMipmapsGenerationSupport)
            imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo;
        memset(&allocInfo, 0, sizeof(allocInfo));
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

        MAZE_VK_CALL(vmaCreateImage(allocator, &imageInfo, &allocInfo, &m_image, &m_imageAllocation, nullptr));
        MAZE_ERROR_RETURN_VALUE_IF(m_image == VK_NULL_HANDLE, false, "vmaCreateImage (Cube) failed!");

        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        TransitionImageLayoutVulkan(
            commandBuffer, m_image, VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            m_mipLevels, 6u);
        m_currentLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

        Vector<VkBuffer> stagingBuffers;
        Vector<VmaAllocation> stagingAllocations;

        for (Size face = 0; face < 6; ++face)
        {
            for (Size mip = 0; mip < _pixelSheets[face].size() && mip < (Size)m_mipLevels; ++mip)
            {
                // Pixel sheets are stored bottom-up (GL 2D convention), but cube map faces
                // use the top-down convention in every graphics API - flip before upload
                // (matches TextureCubeOpenGL/TextureCubeDX11; forgetting this mirrors the
                // skybox about the horizon - see project memory project_dx11_render_system.md
                // item 2)
                PixelSheet2D pixelSheet = _pixelSheets[face][mip];
                if (pixelSheet.getTotalBytesCount() == 0)
                    continue;
                pixelSheet.flipY();

                U8 const* data = pixelSheet.getDataRO();
                Size dataSize = pixelSheet.getTotalBytesCount();

                Vector<U8> expandedData;
                if (expandRGB)
                {
                    Size pixelsCount = (Size)pixelSheet.getSize().x * (Size)pixelSheet.getSize().y;
                    expandedData.resize(pixelsCount * 4);
                    ExpandRGBToRGBAVulkan(data, &expandedData[0], pixelsCount);
                    data = &expandedData[0];
                    dataSize = pixelsCount * 4;
                }
                else
                if (expandDepth24)
                {
                    Size texelsCount = (Size)pixelSheet.getSize().x * (Size)pixelSheet.getSize().y;
                    expandedData.resize(texelsCount * 4);
                    ExpandDepth24ToDepth24Stencil8Vulkan(data, &expandedData[0], texelsCount);
                    data = &expandedData[0];
                    dataSize = texelsCount * 4;
                }

                VkBufferCreateInfo stagingBufferInfo;
                memset(&stagingBufferInfo, 0, sizeof(stagingBufferInfo));
                stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                stagingBufferInfo.size = dataSize;
                stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
                stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

                VmaAllocationCreateInfo stagingAllocInfo;
                memset(&stagingAllocInfo, 0, sizeof(stagingAllocInfo));
                stagingAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;
                stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

                VkBuffer stagingBuffer = VK_NULL_HANDLE;
                VmaAllocation stagingAllocation = VK_NULL_HANDLE;
                VmaAllocationInfo stagingAllocationInfo;
                MAZE_VK_CALL(vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocInfo, &stagingBuffer, &stagingAllocation, &stagingAllocationInfo));
                memcpy(stagingAllocationInfo.pMappedData, data, dataSize);

                stagingBuffers.push_back(stagingBuffer);
                stagingAllocations.push_back(stagingAllocation);

                VkBufferImageCopy region;
                memset(&region, 0, sizeof(region));
                region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                region.imageSubresource.mipLevel = (U32)mip;
                region.imageSubresource.baseArrayLayer = (U32)face;
                region.imageSubresource.layerCount = 1;
                region.imageOffset = { 0, 0, 0 };
                region.imageExtent.width = Math::Max(1, pixelSheet.getSize().x);
                region.imageExtent.height = Math::Max(1, pixelSheet.getSize().y);
                region.imageExtent.depth = 1;

                vkCmdCopyBufferToImage(
                    commandBuffer,
                    stagingBuffer,
                    m_image,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1, &region);
            }
        }

        TransitionImageLayoutVulkan(
            commandBuffer, m_image, VK_IMAGE_ASPECT_COLOR_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            m_mipLevels, 6u);
        m_currentLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        renderSystem->endSingleTimeCommands(commandBuffer);

        for (Size i = 0; i < stagingBuffers.size(); ++i)
            vmaDestroyBuffer(allocator, stagingBuffers[i], stagingAllocations[i]);

        VkImageViewCreateInfo viewInfo;
        memset(&viewInfo, 0, sizeof(viewInfo));
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = m_mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 6;

        MAZE_VK_CALL(vkCreateImageView(device, &viewInfo, nullptr, &m_imageView));
        MAZE_ERROR_RETURN_VALUE_IF(m_imageView == VK_NULL_HANDLE, false, "vkCreateImageView (Cube) failed!");

        return true;
    }

    //////////////////////////////////////////
    VkSampler TextureCubeVulkan::ensureSampler()
    {
        return getRenderSystemVulkanRaw()->ensureSampler(
            m_minFilter,
            m_magFilter,
            m_wrapS,
            m_wrapT,
            m_wrapR,
            0.0f,
            (F32)m_mipLevels,
            Vec4F::c_zero);
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::setMagFilter(TextureFilter _value)
    {
        m_magFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::setMinFilter(TextureFilter _value)
    {
        m_minFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::setWrapS(TextureWrap _value)
    {
        m_wrapS = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::setWrapT(TextureWrap _value)
    {
        m_wrapT = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeVulkan::setWrapR(TextureWrap _value)
    {
        m_wrapR = _value;
        return true;
    }

    //////////////////////////////////////////
    void TextureCubeVulkan::generateMipmaps()
    {
        if (!m_image)
            return;

        MAZE_ERROR_RETURN_IF(
            !m_hasMipmapsGenerationSupport,
            "Mipmaps generation is not supported for this texture format: %d!",
            (S32)m_internalPixelFormat);

        if (m_mipLevels <= 1u)
            return;

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();

        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        TransitionImageLayoutVulkan(
            commandBuffer, m_image, VK_IMAGE_ASPECT_COLOR_BIT,
            m_currentLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            m_mipLevels, 6u);

        for (U32 face = 0; face < 6u; ++face)
        {
            S32 mipWidth = m_size.x;
            S32 mipHeight = m_size.y;

            for (U32 mip = 1; mip < m_mipLevels; ++mip)
            {
                TransitionImageLayoutVulkan(
                    commandBuffer, m_image, VK_IMAGE_ASPECT_COLOR_BIT,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    1u, 1u, mip - 1u, face);

                S32 nextWidth = Math::Max(mipWidth / 2, 1);
                S32 nextHeight = Math::Max(mipHeight / 2, 1);

                VkImageBlit blit;
                memset(&blit, 0, sizeof(blit));
                blit.srcOffsets[0] = { 0, 0, 0 };
                blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
                blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.srcSubresource.mipLevel = mip - 1u;
                blit.srcSubresource.baseArrayLayer = face;
                blit.srcSubresource.layerCount = 1;
                blit.dstOffsets[0] = { 0, 0, 0 };
                blit.dstOffsets[1] = { nextWidth, nextHeight, 1 };
                blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.dstSubresource.mipLevel = mip;
                blit.dstSubresource.baseArrayLayer = face;
                blit.dstSubresource.layerCount = 1;

                vkCmdBlitImage(
                    commandBuffer,
                    m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1, &blit,
                    VK_FILTER_LINEAR);

                TransitionImageLayoutVulkan(
                    commandBuffer, m_image, VK_IMAGE_ASPECT_COLOR_BIT,
                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    1u, 1u, mip - 1u, face);

                mipWidth = nextWidth;
                mipHeight = nextHeight;
            }

            TransitionImageLayoutVulkan(
                commandBuffer, m_image, VK_IMAGE_ASPECT_COLOR_BIT,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                1u, 1u, m_mipLevels - 1u, face);
        }

        renderSystem->endSingleTimeCommands(commandBuffer);

        m_currentLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    //////////////////////////////////////////
    void TextureCubeVulkan::reload()
    {
        MAZE_NOT_IMPLEMENTED;
    }


} // namespace Maze
//////////////////////////////////////////
