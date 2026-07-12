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
            if (colorTexture->getType() == TextureType::TwoDimensional)
            {
                Texture2DVulkan* tex = colorTexture->castRaw<Texture2DVulkan>();
                // getImageView() spans the whole mip chain - not legal as a
                // vkCmdBeginRendering attachment once this texture has more
                // than one mip level (see getAttachmentImageView()'s banner
                // comment)
                view = tex->getAttachmentImageView();
                colorFormat = tex->getFormatVulkan();

                // Must go through transitionTo() (not a raw
                // TransitionImageLayoutVulkan(..., VK_IMAGE_LAYOUT_UNDEFINED, ...)
                // call) so tex->m_currentLayout stays accurate - a stale
                // tracked layout here was the root cause of
                // generateMipmaps() (see its banner comment) recording an
                // oldLayout that no longer matched the image's real GPU-side
                // layout once mipmap generation was fixed to run in the same
                // command buffer right after this render, producing
                // VUID-VkImageMemoryBarrier-oldLayout-01197 errors
                if (cmd != VK_NULL_HANDLE)
                    tex->transitionTo(cmd, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            }
            else
            if (colorTexture->getType() == TextureType::TwoDimensionalMultisample)
            {
                Texture2DMSVulkan* tex = colorTexture->castRaw<Texture2DMSVulkan>();
                view = tex->getImageView();
                colorFormat = tex->getFormatVulkan();
                samples = tex->getSampleCountVulkan();

                if (cmd != VK_NULL_HANDLE)
                    tex->transitionTo(cmd, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
            }

            if (view != VK_NULL_HANDLE)
                colorViews[colorViewsCount++] = view;
        }

        VkImageView depthView = VK_NULL_HANDLE;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        TexturePtr const& depthTexture = getDepthTexture();
        if (depthTexture)
        {
            if (depthTexture->getType() == TextureType::TwoDimensional)
            {
                Texture2DVulkan* tex = depthTexture->castRaw<Texture2DVulkan>();
                // Depth render targets are practically always single-mip,
                // but getAttachmentImageView() falls back to getImageView()
                // in that case anyway - see its banner comment
                depthView = tex->getAttachmentImageView();
                depthFormat = tex->getFormatVulkan();

                // See the identical transitionTo() rationale in the color
                // attachment loop above
                if (cmd != VK_NULL_HANDLE)
                    tex->transitionTo(cmd, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
            }
            else
            if (depthTexture->getType() == TextureType::TwoDimensionalMultisample)
            {
                Texture2DMSVulkan* tex = depthTexture->castRaw<Texture2DMSVulkan>();
                depthView = tex->getImageView();
                depthFormat = tex->getFormatVulkan();
                samples = tex->getSampleCountVulkan();

                if (cmd != VK_NULL_HANDLE)
                    tex->transitionTo(cmd, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
            }
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

                // blit()'s destination exists to be SAMPLED next (e.g. a
                // PostFX/distortion material's u_baseMap), not rendered into -
                // leave it ready for that. This must happen here rather than
                // at the point it's actually bound as a texture uniform
                // (ShaderUniformVulkan::processSimpleUniformChanged() runs
                // while the sampling draw's own render target scope is
                // typically already open, where an image layout transition
                // is illegal to record - confirmed via
                // VUID-vkCmdPipelineBarrier-None-09553 the hard way) -
                // blit() is guaranteed to run outside any open scope
                // (interruptActiveRenderTarget(), at the top of this
                // function), making it the correct place instead.
                TransitionImageLayoutVulkan(cmd, dst, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

                // The source almost always belongs to a render buffer that
                // gets rendered into again next frame - restore it to its
                // normal attachment-ready layout rather than leaving it in
                // TRANSFER_SRC_OPTIMAL, or the next frame's
                // vkCmdBeginRendering (which declares/expects
                // COLOR_ATTACHMENT_OPTIMAL) mismatches its actual tracked
                // layout - confirmed via "vkQueueSubmit(): ... expects
                // VkImage ... to be in layout COLOR_ATTACHMENT_OPTIMAL --
                // instead, current layout is TRANSFER_SRC_OPTIMAL"
                // validation warnings.
                TransitionImageLayoutVulkan(cmd, src, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
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
                if (srcMultisampled && !dstMultisampled)
                {
                    // A fullscreen-shader manual resolve
                    // (RenderSystemVulkan::resolveDepthMSAA(), see its
                    // banner comment for why vkCmdResolveImage/
                    // render-pass-integrated resolve can't be used here) -
                    // it does its own layout transitions via each texture's
                    // own tracked layout (Texture2DVulkan::transitionTo()),
                    // so no raw TransitionImageLayoutVulkan calls are needed
                    // (or correct - those don't update tracked layout, and
                    // declaring oldLayout=UNDEFINED here would discard the
                    // source's actual rendered depth content before this
                    // pass can read it).
                    getRenderSystemVulkanRaw()->resolveDepthMSAA(
                        dstDepthTexture->castRaw<Texture2DVulkan>(),
                        srcDepthTexture->castRaw<Texture2DMSVulkan>());
                }
                else
                if (srcMultisampled == dstMultisampled)
                {
                    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
                    TransitionImageLayoutVulkan(cmd, src, aspect, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
                    TransitionImageLayoutVulkan(cmd, dst, aspect, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

                    VkImageCopy region = {};
                    region.srcSubresource = { VK_IMAGE_ASPECT_DEPTH_BIT, 0u, 0u, 1u };
                    region.dstSubresource = { VK_IMAGE_ASPECT_DEPTH_BIT, 0u, 0u, 1u };
                    region.extent = { getSize().x, getSize().y, 1u };
                    vkCmdCopyImage(cmd, src, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dst, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1u, &region);

                    // See the identical banner comment in the color
                    // blitTexture lambda above - the destination exists to
                    // be sampled (u_depthMap) next, not rendered into.
                    TransitionImageLayoutVulkan(cmd, dst, aspect, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

                    // See the identical restoration in the color blitTexture
                    // lambda above - the source belongs to a render buffer
                    // reused as a depth attachment next frame, so it must
                    // not be left in TRANSFER_SRC_OPTIMAL.
                    TransitionImageLayoutVulkan(cmd, src, aspect, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                }
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
