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
#if (!defined(_MazeTexture2DVulkan_hpp_))
#define _MazeTexture2DVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeTexture2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(Texture2DVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class Texture2DVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API Texture2DVulkan
        : public Texture2D
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(Texture2DVulkan, Texture2D);

    public:

        //////////////////////////////////////////
        virtual ~Texture2DVulkan();

        //////////////////////////////////////////
        static Texture2DVulkanPtr Create(RenderSystemVulkan* _renderSystem);


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;

        //////////////////////////////////////////
        inline VkImage getImage() const { return m_image; }

        //////////////////////////////////////////
        inline VkImageView getImageView() const { return m_imageView; }

        //////////////////////////////////////////
        // A combined-depth-stencil-aspect view (m_imageView, used for
        // framebuffer attachment binding) is not a legal VkImageView for a
        // COMBINED_IMAGE_SAMPLER descriptor - sampling requires a single
        // aspect. Returns a DEPTH_BIT-only view for depth+stencil-backed
        // textures (see m_sampledImageView/createImageView()), or just
        // m_imageView for every other format where the two coincide.
        inline VkImageView getSampledImageView() const { return m_sampledImageView != VK_NULL_HANDLE ? m_sampledImageView : m_imageView; }

        //////////////////////////////////////////
        // m_imageView spans the whole mip chain (levelCount = m_mipLevels),
        // which is correct for sampling but NOT legal as a
        // vkCmdBeginRendering color/depth attachment when m_mipLevels > 1:
        // Vulkan requires every mip level covered by an attachment's image
        // view to be in the layout declared for that attachment, so a
        // full-chain view used as an attachment forces mips that are still
        // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL (e.g. from an earlier
        // generateMipmaps() call) to also satisfy
        // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, which they don't -
        // exactly the "expects COLOR_ATTACHMENT_OPTIMAL...instead
        // SHADER_READ_ONLY_OPTIMAL" validation errors this was root-caused
        // from. Render targets always render into mip 0, so a dedicated
        // single-mip view (m_attachmentImageView, only created when
        // m_mipLevels > 1 - see createImageView()) fixes this; falls back to
        // m_imageView when there's only one mip anyway.
        inline VkImageView getAttachmentImageView() const { return m_attachmentImageView != VK_NULL_HANDLE ? m_attachmentImageView : m_imageView; }

        //////////////////////////////////////////
        inline VkFormat getFormatVulkan() const { return m_format; }

        //////////////////////////////////////////
        inline U32 getMipLevels() const { return m_mipLevels; }

        //////////////////////////////////////////
        // Lazily creates a sampler matching the current filter/wrap/anisotropy state
        // (samplers are shared and cached by RenderSystemVulkan::ensureSampler)
        VkSampler ensureSampler();

        //////////////////////////////////////////
        // Current layout the image is guaranteed to be in outside of an in-progress
        // transfer/render scope - tracked here (rather than queried from the driver,
        // which isn't possible) so callers know what oldLayout to pass to
        // TransitionImageLayoutVulkan
        inline VkImageLayout getCurrentLayout() const { return m_currentLayout; }

        //////////////////////////////////////////
        // Records a barrier (on the given command buffer) transitioning the whole
        // resource to _newLayout, and updates getCurrentLayout()
        void transitionTo(VkCommandBuffer _commandBuffer, VkImageLayout _newLayout);


        //////////////////////////////////////////
        virtual bool setMagFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setMinFilter(TextureFilter _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapS(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setWrapT(TextureWrap _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setBorderColor(ColorU32 _value) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool setAnisotropyLevel(F32 _value) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void copyImageFrom(
            Texture2DPtr const& _texture,
            U32 _x = 0,
            U32 _y = 0) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void copyImageFrom(
            U8 const* _pixels,
            PixelFormat::Enum _pixelFormat,
            U32 _width,
            U32 _height,
            U32 _x,
            U32 _y) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void saveToFileAsTGA(
            String const& _fileName,
            Vec2U _size = Vec2U::c_zero,
            bool _resetAlpha = false) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool readAsPixelSheet(
            PixelSheet2D& _outResult,
            PixelFormat::Enum _outputFormat = PixelFormat::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void generateMipmaps() MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        Texture2DVulkan();

        //////////////////////////////////////////
        virtual bool init(RenderSystem* _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadTextureImpl(
            Vector<PixelSheet2D> const& _pixelSheets,
            PixelFormat::Enum _internalPixelFormat) MAZE_OVERRIDE;

        //////////////////////////////////////////
        void destroyTexture();

        //////////////////////////////////////////
        bool createImageView();

    protected:
        VkImage m_image = VK_NULL_HANDLE;
        VmaAllocation m_imageAllocation = VK_NULL_HANDLE;
        VkImageView m_imageView = VK_NULL_HANDLE;
        // Only created for combined depth+stencil formats (see getSampledImageView())
        VkImageView m_sampledImageView = VK_NULL_HANDLE;
        // Only created when m_mipLevels > 1 (see getAttachmentImageView())
        VkImageView m_attachmentImageView = VK_NULL_HANDLE;
        VkFormat m_format = VK_FORMAT_UNDEFINED;
        VkImageAspectFlags m_aspect = VK_IMAGE_ASPECT_COLOR_BIT;
        VkImageLayout m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        U32 m_mipLevels = 1u;
        bool m_hasMipmapsGenerationSupport = false;
        bool m_isRenderTarget = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeTexture2DVulkan_hpp_
//////////////////////////////////////////
