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
#include "maze-render-system-vulkan/MazeTexture2DVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/helpers/MazePixelSheet2DHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Texture2DVulkan
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DVulkan, Texture2D);

    //////////////////////////////////////////
    Texture2DVulkan::Texture2DVulkan()
    {
    }

    //////////////////////////////////////////
    Texture2DVulkan::~Texture2DVulkan()
    {
        destroyTexture();
    }

    //////////////////////////////////////////
    Texture2DVulkanPtr Texture2DVulkan::Create(RenderSystemVulkan* _renderSystem)
    {
        Texture2DVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(Texture2DVulkan, object, init((RenderSystem*)_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::init(RenderSystem* _renderSystem)
    {
        if (!Texture2D::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* Texture2DVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystem->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::isValid()
    {
        return (m_image != VK_NULL_HANDLE);
    }

    //////////////////////////////////////////
    void Texture2DVulkan::destroyTexture()
    {
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        if (!renderSystem || (m_image == VK_NULL_HANDLE && m_imageView == VK_NULL_HANDLE))
        {
            m_image = VK_NULL_HANDLE;
            m_imageAllocation = VK_NULL_HANDLE;
            m_imageView = VK_NULL_HANDLE;
            m_sampledImageView = VK_NULL_HANDLE;
            m_attachmentImageView = VK_NULL_HANDLE;
            m_mipLevels = 1u;
            m_hasMipmapsGenerationSupport = false;
            m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            return;
        }

        // Vulkan/VMA resources must not be destroyed while a submitted-but-not-yet-completed
        // OR still-recording (e.g. a resize interrupting a frame mid-record - see
        // RenderSystemVulkan::waitDeviceIdleSafe()'s banner comment) command buffer may still
        // reference them (unlike D3D11's internally refcounted resources, which are safe to
        // Release() immediately). There's no per-resource deferred-deletion queue in this
        // backend yet (that belongs in RenderSystemVulkan/RenderQueueVulkan, out of scope
        // here), so this conservatively waits for the whole device to go idle before tearing
        // down. Correct, but a real deferred-deletion queue keyed off the frame-in-flight
        // fences would avoid the stall this causes if textures are destroyed frequently
        // mid-session.
        renderSystem->waitDeviceIdleSafe();

        if (m_imageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(renderSystem->getDevice(), m_imageView, nullptr);
            m_imageView = VK_NULL_HANDLE;
        }

        if (m_sampledImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(renderSystem->getDevice(), m_sampledImageView, nullptr);
            m_sampledImageView = VK_NULL_HANDLE;
        }

        if (m_attachmentImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(renderSystem->getDevice(), m_attachmentImageView, nullptr);
            m_attachmentImageView = VK_NULL_HANDLE;
        }

        if (m_image != VK_NULL_HANDLE)
        {
            vmaDestroyImage(renderSystem->getAllocator(), m_image, m_imageAllocation);
            m_image = VK_NULL_HANDLE;
            m_imageAllocation = VK_NULL_HANDLE;
        }

        m_mipLevels = 1u;
        m_hasMipmapsGenerationSupport = false;
        m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::createImageView()
    {
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();

        VkImageViewCreateInfo viewInfo;
        memset(&viewInfo, 0, sizeof(viewInfo));
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = m_image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = m_format;
        viewInfo.subresourceRange.aspectMask = m_aspect;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = m_mipLevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        MAZE_VK_CALL(vkCreateImageView(renderSystem->getDevice(), &viewInfo, nullptr, &m_imageView));
        if (m_imageView == VK_NULL_HANDLE)
            return false;

        // A combined depth+stencil aspect view is valid for framebuffer
        // attachment binding but not for a COMBINED_IMAGE_SAMPLER descriptor -
        // sampling requires a single aspect (see getSampledImageView()).
        // Create a DEPTH_BIT-only view alongside the main one for exactly
        // this case (DEPTH_U24 backed by VK_FORMAT_D24_UNORM_S8_UINT,
        // DEPTH_STENCIL_F32_U8 backed by D32_SFLOAT_S8_UINT).
        if ((m_aspect & VK_IMAGE_ASPECT_DEPTH_BIT) && (m_aspect & VK_IMAGE_ASPECT_STENCIL_BIT))
        {
            VkImageViewCreateInfo sampledViewInfo = viewInfo;
            sampledViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            MAZE_VK_CALL(vkCreateImageView(renderSystem->getDevice(), &sampledViewInfo, nullptr, &m_sampledImageView));
        }

        // See getAttachmentImageView()'s banner comment - m_imageView's
        // levelCount = m_mipLevels is wrong for attachment binding once this
        // texture has more than one mip, since every level covered by an
        // attachment's image view must share the attachment's layout
        if (m_mipLevels > 1u)
        {
            VkImageViewCreateInfo attachmentViewInfo = viewInfo;
            attachmentViewInfo.subresourceRange.levelCount = 1u;
            MAZE_VK_CALL(vkCreateImageView(renderSystem->getDevice(), &attachmentViewInfo, nullptr, &m_attachmentImageView));
        }

        return true;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::loadTextureImpl(
        Vector<PixelSheet2D> const& _pixelSheets,
        PixelFormat::Enum _internalPixelFormat)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_pixelSheets.empty(), false, "PixelSheets are empty!");

        if (_internalPixelFormat == PixelFormat::None)
            _internalPixelFormat = _pixelSheets[0].getFormat();

        // Most desktop Vulkan drivers don't support 24-bit RGB for optimal tiling - expand
        // to RGBA on upload (mirrors DX11/ExpandRGBToRGBADX11 exactly)
        bool expandRGB = (_internalPixelFormat == PixelFormat::RGB_U8);
        // DEPTH_U24 is nominally 3 bytes/texel at the engine level (see
        // PixelFormat::GetBytesPerPixel) but is backed by the 4-byte-packed
        // VK_FORMAT_D24_UNORM_S8_UINT on Vulkan (see GetPixelFormatVulkan) -
        // needs the same kind of upload-time expansion as RGB_U8, or the
        // staging buffer ends up 25% undersized for the copy
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

        Vec2S size = _pixelSheets[0].getSize();
        m_size = size;
        m_invSize = Vec2F(1.0f / (F32)size.x, 1.0f / (F32)size.y);
        m_internalPixelFormat = _internalPixelFormat;
        m_format = format;
        m_aspect = GetImageAspectFlagsVulkan(_internalPixelFormat);

        bool isDepth = IsDepthPixelFormatVulkan(_internalPixelFormat);
        bool isCompressed = PixelFormat::IsCompressed(_internalPixelFormat);

        m_hasMipmapsGenerationSupport = !isDepth && !isCompressed;

        if (_pixelSheets.size() > 1)
            m_mipLevels = (U32)_pixelSheets.size();
        else if (m_hasMipmapsGenerationSupport)
        {
            m_mipLevels = 1u;
            for (S32 s = Math::Max(size.x, size.y); s > 1; s >>= 1)
                ++m_mipLevels;
        }
        else
            m_mipLevels = 1u;

        VkImageCreateInfo imageInfo;
        memset(&imageInfo, 0, sizeof(imageInfo));
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = (U32)size.x;
        imageInfo.extent.height = (U32)size.y;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = m_mipLevels;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        if (isDepth)
            imageInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        else if (m_hasMipmapsGenerationSupport)
            imageInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo;
        memset(&allocInfo, 0, sizeof(allocInfo));
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        allocInfo.flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT;

        MAZE_VK_CALL(vmaCreateImage(allocator, &imageInfo, &allocInfo, &m_image, &m_imageAllocation, nullptr));
        MAZE_ERROR_RETURN_VALUE_IF(m_image == VK_NULL_HANDLE, false, "vmaCreateImage failed!");

        // Staging-buffer upload of every provided mip level, one command buffer for the
        // whole texture (initial UNDEFINED -> TRANSFER_DST_OPTIMAL, copy each mip, then
        // either TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL directly, or leave in
        // TRANSFER_DST_OPTIMAL if generateMipmaps() (blit-based) still needs to run)
        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        TransitionImageLayoutVulkan(
            commandBuffer, m_image, m_aspect,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            m_mipLevels, 1u);
        m_currentLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

        Vector<VkBuffer> stagingBuffers;
        Vector<VmaAllocation> stagingAllocations;
        stagingBuffers.reserve(_pixelSheets.size());
        stagingAllocations.reserve(_pixelSheets.size());

        for (Size mip = 0; mip < _pixelSheets.size() && mip < (Size)m_mipLevels; ++mip)
        {
            PixelSheet2D const& pixelSheet = _pixelSheets[mip];
            if (pixelSheet.getTotalBytesCount() == 0)
                continue;

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
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;
            // vkCmdCopyBufferToImage requires a single aspect bit per region -
            // unlike layout transitions/clears, which need the whole
            // DEPTH_BIT|STENCIL_BIT mask on a combined depth-stencil image
            // (see GetImageAspectFlagsVulkan's comment) - CPU-provided pixel
            // data for a nominally depth-only format is depth-only data, so
            // narrow to just the depth plane here
            region.imageSubresource.aspectMask =
                (m_aspect & VK_IMAGE_ASPECT_STENCIL_BIT) ? VK_IMAGE_ASPECT_DEPTH_BIT : m_aspect;
            region.imageSubresource.mipLevel = (U32)mip;
            region.imageSubresource.baseArrayLayer = 0;
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

        bool needsGeneratedMips = m_hasMipmapsGenerationSupport && _pixelSheets.size() == 1 && m_mipLevels > 1u;
        if (!needsGeneratedMips)
        {
            TransitionImageLayoutVulkan(
                commandBuffer, m_image, m_aspect,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                m_mipLevels, 1u);
            m_currentLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        renderSystem->endSingleTimeCommands(commandBuffer);

        for (Size i = 0; i < stagingBuffers.size(); ++i)
            vmaDestroyBuffer(allocator, stagingBuffers[i], stagingAllocations[i]);

        if (!createImageView())
            return false;

        // Only level 0 was uploaded but the full mip chain was allocated - generate the rest
        if (needsGeneratedMips)
            generateMipmaps();

        return true;
    }

    //////////////////////////////////////////
    VkSampler Texture2DVulkan::ensureSampler()
    {
        // D3D/Vulkan point and linear filters still mip-select even without explicit
        // mipmap interpolation - clamp maxLod to 0 for non-mipmap filters so a texture
        // with unused/stale upper mips (e.g. bloom downscale chain) doesn't get sampled
        // from garbage data (see project memory project_dx11_render_system.md item 7,
        // the same bug bit the DX11 backend)
        bool usesMipmapFilter =
            m_minFilter == TextureFilter::NearestMipmapNearest ||
            m_minFilter == TextureFilter::LinearMipmapNearest ||
            m_minFilter == TextureFilter::NearestMipmapLinear ||
            m_minFilter == TextureFilter::LinearMipmapLinear;
        F32 maxLod = usesMipmapFilter ? (F32)m_mipLevels : 0.0f;

        return getRenderSystemVulkanRaw()->ensureSampler(
            m_minFilter,
            m_magFilter,
            m_wrapS,
            m_wrapT,
            m_wrapT,
            m_anisotropyLevel,
            maxLod,
            m_borderColor.toVec4F32());
    }

    //////////////////////////////////////////
    void Texture2DVulkan::transitionTo(VkCommandBuffer _commandBuffer, VkImageLayout _newLayout)
    {
        if (m_currentLayout == _newLayout)
            return;

        TransitionImageLayoutVulkan(
            _commandBuffer, m_image, m_aspect,
            m_currentLayout, _newLayout,
            m_mipLevels, 1u);
        m_currentLayout = _newLayout;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::setMagFilter(TextureFilter _value)
    {
        m_magFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::setMinFilter(TextureFilter _value)
    {
        m_minFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::setWrapS(TextureWrap _value)
    {
        m_wrapS = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::setWrapT(TextureWrap _value)
    {
        m_wrapT = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::setBorderColor(ColorU32 _value)
    {
        m_borderColor = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::setAnisotropyLevel(F32 _value)
    {
        m_anisotropyLevel = _value;
        return true;
    }

    //////////////////////////////////////////
    void Texture2DVulkan::copyImageFrom(
        Texture2DPtr const& _texture,
        U32 _x,
        U32 _y)
    {
        MAZE_ERROR_RETURN_IF(!_texture, "Texture is null!");

        Texture2DVulkan* srcTexture = _texture->castRaw<Texture2DVulkan>();
        MAZE_ERROR_RETURN_IF(!srcTexture->m_image || !m_image, "Textures are not loaded!");

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        VkImageLayout srcOldLayout = srcTexture->m_currentLayout;
        VkImageLayout dstOldLayout = m_currentLayout;

        srcTexture->transitionTo(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        transitionTo(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VkImageCopy region;
        memset(&region, 0, sizeof(region));
        region.srcSubresource.aspectMask = srcTexture->m_aspect;
        region.srcSubresource.mipLevel = 0;
        region.srcSubresource.baseArrayLayer = 0;
        region.srcSubresource.layerCount = 1;
        region.dstSubresource.aspectMask = m_aspect;
        region.dstSubresource.mipLevel = 0;
        region.dstSubresource.baseArrayLayer = 0;
        region.dstSubresource.layerCount = 1;
        region.dstOffset = { (S32)_x, (S32)_y, 0 };
        region.extent.width = (U32)srcTexture->m_size.x;
        region.extent.height = (U32)srcTexture->m_size.y;
        region.extent.depth = 1;

        vkCmdCopyImage(
            commandBuffer,
            srcTexture->m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &region);

        // Restore both textures to their pre-copy layout so callers observe no
        // side effect on state they don't own
        srcTexture->transitionTo(commandBuffer, srcOldLayout == VK_IMAGE_LAYOUT_UNDEFINED ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : srcOldLayout);
        transitionTo(commandBuffer, dstOldLayout == VK_IMAGE_LAYOUT_UNDEFINED ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : dstOldLayout);

        renderSystem->endSingleTimeCommands(commandBuffer);
    }

    //////////////////////////////////////////
    void Texture2DVulkan::copyImageFrom(
        U8 const* _pixels,
        PixelFormat::Enum _pixelFormat,
        U32 _width,
        U32 _height,
        U32 _x,
        U32 _y)
    {
        MAZE_ERROR_RETURN_IF(!m_image, "Texture is not loaded!");
        MAZE_ERROR_RETURN_IF(
            _pixelFormat != m_internalPixelFormat,
            "Pixel format mismatch! texture=%d data=%d",
            (S32)m_internalPixelFormat,
            (S32)_pixelFormat);

        U8 const* data = _pixels;
        Size dataSize = (Size)PixelFormat::GetBytesPerPixel(_pixelFormat) * _width * _height;

        Vector<U8> expandedData;
        if (_pixelFormat == PixelFormat::RGB_U8)
        {
            Size pixelsCount = (Size)_width * (Size)_height;
            expandedData.resize(pixelsCount * 4);
            ExpandRGBToRGBAVulkan(data, &expandedData[0], pixelsCount);
            data = &expandedData[0];
            dataSize = pixelsCount * 4;
        }
        else
        if (_pixelFormat == PixelFormat::DEPTH_U24)
        {
            Size texelsCount = (Size)_width * (Size)_height;
            expandedData.resize(texelsCount * 4);
            ExpandDepth24ToDepth24Stencil8Vulkan(data, &expandedData[0], texelsCount);
            data = &expandedData[0];
            dataSize = texelsCount * 4;
        }

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        VmaAllocator allocator = renderSystem->getAllocator();

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

        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        VkImageLayout oldLayout = m_currentLayout;
        transitionTo(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

        VkBufferImageCopy region;
        memset(&region, 0, sizeof(region));
        // See the identical narrowing in loadTextureImpl() - buffer<->image
        // copies need a single aspect bit, unlike whole-resource transitions
        region.imageSubresource.aspectMask =
            (m_aspect & VK_IMAGE_ASPECT_STENCIL_BIT) ? VK_IMAGE_ASPECT_DEPTH_BIT : m_aspect;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = { (S32)_x, (S32)_y, 0 };
        region.imageExtent.width = _width;
        region.imageExtent.height = _height;
        region.imageExtent.depth = 1;

        vkCmdCopyBufferToImage(
            commandBuffer,
            stagingBuffer,
            m_image,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1, &region);

        transitionTo(commandBuffer, oldLayout == VK_IMAGE_LAYOUT_UNDEFINED ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : oldLayout);

        renderSystem->endSingleTimeCommands(commandBuffer);

        vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);
    }

    //////////////////////////////////////////
    bool Texture2DVulkan::readAsPixelSheet(
        PixelSheet2D& _outResult,
        PixelFormat::Enum _outputFormat)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_image, false, "Texture is not loaded!");

        if (_outputFormat == PixelFormat::None)
            _outputFormat = m_internalPixelFormat;

        MAZE_ERROR_RETURN_VALUE_IF(
            _outputFormat != m_internalPixelFormat,
            false,
            "Vulkan readAsPixelSheet format conversion is not supported! texture=%d requested=%d",
            (S32)m_internalPixelFormat,
            (S32)_outputFormat);

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        VmaAllocator allocator = renderSystem->getAllocator();

        Size texelsCount = (Size)m_size.x * (Size)m_size.y;

        // The GPU image is physically 4 bytes/texel (VK_FORMAT_D24_UNORM_S8_UINT)
        // even though DEPTH_U24 is nominally 3 bytes/texel at the engine level -
        // see the identical comment in loadTextureImpl() - so the staging buffer
        // must be sized for the wider GPU layout, narrowed back down after copy
        bool isDepth24 = (m_internalPixelFormat == PixelFormat::DEPTH_U24);
        Size dataSize = isDepth24 ? (texelsCount * 4) : ((Size)PixelFormat::GetBytesPerPixel(m_internalPixelFormat) * texelsCount);

        VkBufferCreateInfo stagingBufferInfo;
        memset(&stagingBufferInfo, 0, sizeof(stagingBufferInfo));
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.size = dataSize;
        stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo stagingAllocInfo;
        memset(&stagingAllocInfo, 0, sizeof(stagingAllocInfo));
        stagingAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        stagingAllocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;

        VkBuffer stagingBuffer = VK_NULL_HANDLE;
        VmaAllocation stagingAllocation = VK_NULL_HANDLE;
        VmaAllocationInfo stagingAllocationInfo;
        MAZE_VK_CALL(vmaCreateBuffer(allocator, &stagingBufferInfo, &stagingAllocInfo, &stagingBuffer, &stagingAllocation, &stagingAllocationInfo));

        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        VkImageLayout oldLayout = m_currentLayout;
        transitionTo(commandBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

        VkBufferImageCopy region;
        memset(&region, 0, sizeof(region));
        // See the identical narrowing in loadTextureImpl() - buffer<->image
        // copies need a single aspect bit, unlike whole-resource transitions
        region.imageSubresource.aspectMask =
            (m_aspect & VK_IMAGE_ASPECT_STENCIL_BIT) ? VK_IMAGE_ASPECT_DEPTH_BIT : m_aspect;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageExtent.width = (U32)m_size.x;
        region.imageExtent.height = (U32)m_size.y;
        region.imageExtent.depth = 1;

        vkCmdCopyImageToBuffer(
            commandBuffer,
            m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            stagingBuffer,
            1, &region);

        transitionTo(commandBuffer, oldLayout == VK_IMAGE_LAYOUT_UNDEFINED ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : oldLayout);

        renderSystem->endSingleTimeCommands(commandBuffer);

        _outResult.setFormat(_outputFormat);
        _outResult.setSize(m_size);

        if (isDepth24)
        {
            // Narrow the 4-byte-packed GPU data back down to 3 bytes/texel
            // to match DEPTH_U24's engine-level layout (reverse of
            // ExpandDepth24ToDepth24Stencil8Vulkan - drop the stencil byte)
            U8 const* packed = (U8 const*)stagingAllocationInfo.pMappedData;
            U8* dst = _outResult.getDataRW();
            for (Size i = 0; i < texelsCount; ++i)
            {
                dst[i * 3 + 0] = packed[i * 4 + 0];
                dst[i * 3 + 1] = packed[i * 4 + 1];
                dst[i * 3 + 2] = packed[i * 4 + 2];
            }
        }
        else
        {
            memcpy(_outResult.getDataRW(), stagingAllocationInfo.pMappedData, dataSize);
        }

        vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);

        return true;
    }

    //////////////////////////////////////////
    void Texture2DVulkan::saveToFileAsTGA(
        String const& _fileName,
        Vec2U _size,
        bool _resetAlpha)
    {
        PixelSheet2D pixelSheet;
        if (!readAsPixelSheet(pixelSheet))
            return;

        PixelSheet2DHelper::SaveTGA(pixelSheet, _fileName.c_str(), _resetAlpha);
    }

    //////////////////////////////////////////
    void Texture2DVulkan::generateMipmaps()
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

        // Vulkan has no built-in mip generation (unlike D3D11's GenerateMips) - blit each
        // mip level down from the previous one. Assumes the device supports linear
        // blitting for m_format (VK_FORMAT_FEATURE_BLIT_SRC/DST_BIT on optimal tiling),
        // which holds for every uncompressed 8/16/32-bit format this backend maps textures
        // to; compressed formats never reach here since m_hasMipmapsGenerationSupport is
        // false for them.
        //
        // If a frame is currently open (this is being called mid-frame, e.g.
        // from RenderBuffer::endDraw() right after rendering into a
        // render-to-texture's mip 0 - see MazeRenderBuffer.cpp), record into
        // THAT frame's own command buffer instead of a separate synchronous
        // beginSingleTimeCommands() one. The latter submits and blocks on
        // vkQueueWaitIdle() immediately, executing on the GPU before the
        // current frame's main command buffer (which contains the actual
        // render-into-mip-0 draws) has even been submitted, since that
        // submission is deferred to endFrame() - so mipmap generation ended
        // up reading stale (previous-frame or garbage) mip-0 content, and
        // m_currentLayout's CPU-side bookkeeping went out of sync with the
        // GPU's real per-submission execution order. That's what produced
        // the "expects COLOR_ATTACHMENT_OPTIMAL/SHADER_READ_ONLY_OPTIMAL...
        // instead..." validation errors on every render-to-texture-then-
        // sample-with-mips resource (e.g. a SkyboxReflection-style
        // material's render target). Recording into the same frame's command
        // buffer guarantees GPU execution order matches CPU recording order:
        // render into mip 0, generate mips 1..N from it, then any later draw
        // in the same frame that samples the texture - all in one submission.
        bool useFrameCommandBuffer = renderSystem->isFrameOpen();
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        if (useFrameCommandBuffer)
        {
            // Blits/barriers are illegal while a dynamic-rendering scope is
            // open on this command buffer (e.g. immediately after rendering
            // into this very texture as a color attachment) - mirrors
            // RenderBufferVulkan::blit()'s identical rationale/pattern
            renderSystem->interruptActiveRenderTarget();
            commandBuffer = renderSystem->getCurrentCommandBuffer();
        }
        if (commandBuffer == VK_NULL_HANDLE)
        {
            useFrameCommandBuffer = false;
            commandBuffer = renderSystem->beginSingleTimeCommands();
        }

        TransitionImageLayoutVulkan(
            commandBuffer, m_image, m_aspect,
            m_currentLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            m_mipLevels, 1u);

        S32 mipWidth = m_size.x;
        S32 mipHeight = m_size.y;

        for (U32 mip = 1; mip < m_mipLevels; ++mip)
        {
            // Source mip (mip-1) needs to be a blit source
            TransitionImageLayoutVulkan(
                commandBuffer, m_image, m_aspect,
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                1u, 1u, mip - 1u, 0u);

            S32 nextWidth = Math::Max(mipWidth / 2, 1);
            S32 nextHeight = Math::Max(mipHeight / 2, 1);

            VkImageBlit blit;
            memset(&blit, 0, sizeof(blit));
            blit.srcOffsets[0] = { 0, 0, 0 };
            blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
            blit.srcSubresource.aspectMask = m_aspect;
            blit.srcSubresource.mipLevel = mip - 1u;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = { 0, 0, 0 };
            blit.dstOffsets[1] = { nextWidth, nextHeight, 1 };
            blit.dstSubresource.aspectMask = m_aspect;
            blit.dstSubresource.mipLevel = mip;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(
                commandBuffer,
                m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1, &blit,
                VK_FILTER_LINEAR);

            // Source mip is done being read from - move it to its final sampled layout
            TransitionImageLayoutVulkan(
                commandBuffer, m_image, m_aspect,
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                1u, 1u, mip - 1u, 0u);

            mipWidth = nextWidth;
            mipHeight = nextHeight;
        }

        // Last mip level was only ever a blit destination - transition it too
        TransitionImageLayoutVulkan(
            commandBuffer, m_image, m_aspect,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            1u, 1u, m_mipLevels - 1u, 0u);

        if (!useFrameCommandBuffer)
            renderSystem->endSingleTimeCommands(commandBuffer);

        m_currentLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }


} // namespace Maze
//////////////////////////////////////////
