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
#include "maze-render-system-vulkan/MazeRenderBufferVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeStateMachineVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderQueueVulkan.hpp"
#include "maze-render-system-vulkan/MazeTexture2DVulkan.hpp"
#include "maze-render-system-vulkan/MazeTexture2DMSVulkan.hpp"
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeTexture2DMS.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderBufferVulkan
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderBufferVulkan, RenderBuffer);

    //////////////////////////////////////////
    RenderBufferVulkan::RenderBufferVulkan()
    {
    }

    //////////////////////////////////////////
    RenderBufferVulkan::~RenderBufferVulkan()
    {
    }

    //////////////////////////////////////////
    RenderBufferVulkanPtr RenderBufferVulkan::Create(
        RenderSystemVulkan* _renderSystem,
        RenderBufferDeleter const& _deleter)
    {
        RenderBufferVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(RenderBufferVulkan, object, _deleter, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    RenderBufferVulkanPtr RenderBufferVulkan::Create(
        RenderBufferVulkanPtr const& _renderBuffer,
        RenderBufferDeleter const& _deleter)
    {
        RenderBufferVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(RenderBufferVulkan, object, _deleter, init(_renderBuffer));
        return object;
    }

    //////////////////////////////////////////
    bool RenderBufferVulkan::init(RenderSystemVulkan* _renderSystem)
    {
        if (!RenderBuffer::init((RenderSystem*)_renderSystem))
            return false;

        m_renderQueue = RenderQueueVulkan::Create(this);
        if (!m_renderQueue)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool RenderBufferVulkan::init(RenderBufferVulkanPtr const& _renderBuffer)
    {
        if (!RenderBuffer::init(RenderBufferPtr(_renderBuffer)))
            return false;

        m_renderQueue = RenderQueueVulkan::Create(this);
        if (!m_renderQueue)
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBufferVulkan::createCopy()
    {
        return RenderBufferVulkan::Create(cast<RenderBufferVulkan>());
    }

    //////////////////////////////////////////
    RenderSystemVulkan* RenderBufferVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystem->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    bool RenderBufferVulkan::setSize(Vec2U const& _size)
    {
        if (!RenderBuffer::setSize(_size))
            return false;

        for (Size i = 0; i < c_renderBufferColorTexturesMax; ++i)
            resizeTexture(m_colorTextures[i], _size);

        resizeTexture(m_depthTexture, _size);
        resizeTexture(m_stencilTexture, _size);

        eventRenderBufferSizeChanged(cast<RenderBuffer>());

        return true;
    }

    //////////////////////////////////////////
    void RenderBufferVulkan::resizeTexture(TexturePtr const& _texture, Vec2U const& _size)
    {
        if (!_texture)
            return;

        switch (_texture->getType())
        {
            case TextureType::TwoDimensional:
            {
                Texture2D* texture2D = _texture->castRaw<Texture2D>();
                texture2D->loadEmpty(_size, texture2D->getInternalPixelFormat());
                break;
            }
            case TextureType::TwoDimensionalMultisample:
            {
                Texture2DMS* texture2D = _texture->castRaw<Texture2DMS>();
                texture2D->loadEmpty(_size, texture2D->getInternalPixelFormat(), texture2D->getSamples());
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }
    }

    //////////////////////////////////////////
    void RenderBufferVulkan::endDraw()
    {
        RenderBuffer::endDraw();

        eventRenderBufferEndDraw(this);
    }

    //////////////////////////////////////////
    bool RenderBufferVulkan::processRenderTargetWillSet()
    {
        return true;
    }

    //////////////////////////////////////////
    void RenderBufferVulkan::processRenderTargetSet()
    {
        StateMachineVulkan* stateMachine = getRenderSystemVulkanRaw()->getStateMachine();

        // Close any previously-bound render target's dynamic-rendering scope
        // before recording the attachment layout transitions below -
        // bindRenderTarget() further down would close it too, but only after
        // those transitions already tried (illegally) to record a barrier
        // while the scope was still open
        stateMachine->unbindRenderTarget();

        VkCommandBuffer cmd = getRenderSystemVulkanRaw()->getCurrentCommandBuffer();

        VkImageView colorViews[StateMachineVulkan::c_colorAttachmentsMax];
        S32 colorViewsCount = 0;
        VkFormat colorFormat = VK_FORMAT_UNDEFINED;
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

        for (S32 i = 0; i < StateMachineVulkan::c_colorAttachmentsMax; ++i)
        {
            TexturePtr const& colorTexture = getColorTexture((U32)i);
            if (!colorTexture)
                continue;

            VkImageView view = VK_NULL_HANDLE;
            VkImage image = VK_NULL_HANDLE;
            if (colorTexture->getType() == TextureType::TwoDimensional)
            {
                Texture2DVulkan* tex = colorTexture->castRaw<Texture2DVulkan>();
                view = tex->getImageView();
                image = tex->getImage();
                colorFormat = tex->getFormatVulkan();
            }
            else
            if (colorTexture->getType() == TextureType::TwoDimensionalMultisample)
            {
                Texture2DMSVulkan* tex = colorTexture->castRaw<Texture2DMSVulkan>();
                view = tex->getImageView();
                image = tex->getImage();
                colorFormat = tex->getFormatVulkan();
                samples = tex->getSampleCountVulkan();
            }

            if (view != VK_NULL_HANDLE)
            {
                if (cmd != VK_NULL_HANDLE && image != VK_NULL_HANDLE)
                    TransitionImageLayoutVulkan(cmd, image, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                colorViews[colorViewsCount++] = view;
            }
        }

        VkImageView depthView = VK_NULL_HANDLE;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        TexturePtr const& depthTexture = getDepthTexture();
        if (depthTexture)
        {
            VkImage depthImage = VK_NULL_HANDLE;
            if (depthTexture->getType() == TextureType::TwoDimensional)
            {
                Texture2DVulkan* tex = depthTexture->castRaw<Texture2DVulkan>();
                depthView = tex->getImageView();
                depthImage = tex->getImage();
                depthFormat = tex->getFormatVulkan();
            }
            else
            if (depthTexture->getType() == TextureType::TwoDimensionalMultisample)
            {
                Texture2DMSVulkan* tex = depthTexture->castRaw<Texture2DMSVulkan>();
                depthView = tex->getImageView();
                depthImage = tex->getImage();
                depthFormat = tex->getFormatVulkan();
                samples = tex->getSampleCountVulkan();
            }

            if (cmd != VK_NULL_HANDLE && depthImage != VK_NULL_HANDLE)
                TransitionImageLayoutVulkan(cmd, depthImage, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        }

        // Offscreen targets are rendered flipped so their memory layout matches OpenGL render buffers
        stateMachine->bindRenderTarget(
            colorViews,
            colorViewsCount,
            depthView,
            getSize(),
            true,
            colorFormat,
            depthFormat,
            samples);
    }

    //////////////////////////////////////////
    void RenderBufferVulkan::processRenderTargetWillReset()
    {
    }

    //////////////////////////////////////////
    void RenderBufferVulkan::blit(RenderBufferPtr const& _srcBuffer)
    {
        MAZE_ERROR_RETURN_IF(!_srcBuffer, "Source buffer is null!");

        // blit() runs outside the normal setCurrentRenderTarget() flow, so
        // whichever target was last bound (likely _srcBuffer, having just
        // finished rendering into it) may still have its dynamic-rendering
        // scope open - none of the barriers/vkCmdResolveImage/vkCmdCopyImage
        // calls below are legal while that's the case. This also forgets the
        // current render target so a later setCurrentRenderTarget() to the
        // same target properly reopens a scope instead of no-op'ing.
        getRenderSystemVulkanRaw()->interruptActiveRenderTarget();

        VkCommandBuffer cmd = getRenderSystemVulkanRaw()->getCurrentCommandBuffer();
        MAZE_ERROR_RETURN_IF(cmd == VK_NULL_HANDLE, "No active command buffer!");

        auto getImage =
            [](TexturePtr const& _texture) -> VkImage
            {
                if (!_texture)
                    return VK_NULL_HANDLE;

                if (_texture->getType() == TextureType::TwoDimensional)
                    return _texture->castRaw<Texture2DVulkan>()->getImage();

                if (_texture->getType() == TextureType::TwoDimensionalMultisample)
                    return _texture->castRaw<Texture2DMSVulkan>()->getImage();

                return VK_NULL_HANDLE;
            };

        auto blitTexture =
            [&](TexturePtr const& _dstTexture, TexturePtr const& _srcTexture, Vec2U const& _size)
            {
                VkImage dst = getImage(_dstTexture);
                VkImage src = getImage(_srcTexture);
                if (dst == VK_NULL_HANDLE || src == VK_NULL_HANDLE)
                    return;

                bool srcMultisampled = (_srcTexture->getType() == TextureType::TwoDimensionalMultisample);
                bool dstMultisampled = (_dstTexture->getType() == TextureType::TwoDimensionalMultisample);

                TransitionImageLayoutVulkan(cmd, src, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
                TransitionImageLayoutVulkan(cmd, dst, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

                if (srcMultisampled && !dstMultisampled)
                {
                    VkImageResolve region = {};
                    region.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 0u, 1u };
                    region.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 0u, 1u };
                    region.extent = { _size.x, _size.y, 1u };
                    vkCmdResolveImage(cmd, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u, &region);
                }
                else
                {
                    VkImageCopy region = {};
                    region.srcSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 0u, 1u };
                    region.dstSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0u, 0u, 1u };
                    region.extent = { _size.x, _size.y, 1u };
                    vkCmdCopyImage(cmd, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u, &region);
                }

                TransitionImageLayoutVulkan(cmd, dst, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            };

        for (U32 i = 0; i < StateMachineVulkan::c_colorAttachmentsMax; ++i)
        {
            TexturePtr const& dstTexture = getColorTexture(i);
            TexturePtr const& srcTexture = _srcBuffer->getColorTexture(i);
            if (dstTexture && srcTexture)
                blitTexture(dstTexture, srcTexture, getSize());
        }

        // Depth
        TexturePtr const& dstDepthTexture = getDepthTexture();
        TexturePtr const& srcDepthTexture = _srcBuffer->getDepthTexture();
        if (dstDepthTexture && srcDepthTexture)
        {
            bool srcMultisampled = (srcDepthTexture->getType() == TextureType::TwoDimensionalMultisample);
            bool dstMultisampled = (dstDepthTexture->getType() == TextureType::TwoDimensionalMultisample);

            VkImage dst = getImage(dstDepthTexture);
            VkImage src = getImage(srcDepthTexture);
            if (dst != VK_NULL_HANDLE && src != VK_NULL_HANDLE)
            {
                VkImageAspectFlags aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
                TransitionImageLayoutVulkan(cmd, src, aspect, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
                TransitionImageLayoutVulkan(cmd, dst, aspect, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

                if (srcMultisampled && !dstMultisampled)
                {
                    // Unlike color, vkCmdResolveImage requires
                    // VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT on both images and
                    // is not valid for depth-aspect resolves (VK_KHR_depth_stencil_resolve
                    // only extends *render-pass-integrated* resolve via
                    // VkRenderingAttachmentInfo::resolveMode/resolveImageView,
                    // which must be set when the source's rendering scope
                    // begins - blit() runs after that scope already closed, so
                    // it can't be retrofitted here). Also, vkCmdCopyImage
                    // requires matching sample counts, so a plain copy can't
                    // substitute either. A correct fix needs either a
                    // fullscreen-shader manual resolve (see DX11's
                    // resolveDepthMSAA/c_depthResolveShaderSourceDX11 for the
                    // reference implementation) or threading a resolve target
                    // through StateMachineVulkan::bindRenderTarget() so the
                    // original render pass performs the resolve on
                    // vkCmdEndRendering. Neither is implemented yet - skip
                    // rather than record an invalid/undefined command.
                    MAZE_ERROR("RenderBufferVulkan::blit(): multisampled depth resolve is not implemented - skipping (see comment)");
                }
                else
                if (srcMultisampled == dstMultisampled)
                {
                    VkImageCopy region = {};
                    region.srcSubresource = { VK_IMAGE_ASPECT_DEPTH_BIT, 0u, 0u, 1u };
                    region.dstSubresource = { VK_IMAGE_ASPECT_DEPTH_BIT, 0u, 0u, 1u };
                    region.extent = { getSize().x, getSize().y, 1u };
                    vkCmdCopyImage(cmd, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u, &region);
                }

                TransitionImageLayoutVulkan(cmd, dst, aspect, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
