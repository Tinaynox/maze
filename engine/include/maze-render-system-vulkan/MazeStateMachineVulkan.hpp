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
#if (!defined(_MazeStateMachineVulkan_hpp_))
#define _MazeStateMachineVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-graphics/MazeBlendMode.hpp"
#include "maze-graphics/MazeCompareFunction.hpp"
#include "maze-graphics/MazeStencilOperation.hpp"
#include "maze-graphics/MazeCullMode.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class RenderSystemVulkan;
    class ShaderVulkan;
    class VertexArrayObjectVulkan;


    //////////////////////////////////////////
    // Everything needed to bake/select a VkPipeline for the next draw call.
    // Cull/depth/stencil/scissor/viewport/topology are set as *dynamic* pipeline
    // state (VK_EXT_extended_dynamic_state, required device extension) so they
    // don't multiply the number of baked pipeline variants - only blend state
    // (not covered by that extension) and the vertex-input/shader/attachment-format
    // combination actually need distinct VkPipeline objects, mirroring how DX11
    // freely mixes-and-matches its separate blend/depth-stencil/rasterizer state
    // objects.
    struct MAZE_RENDER_SYSTEM_VULKAN_API PipelineKeyVulkan
    {
        ShaderVulkan* shader = nullptr;
        U64 vertexLayoutHash = 0u;
        RenderDrawTopology topology = RenderDrawTopology::Triangles;
        bool blendEnabled = false;
        BlendFactor blendSrcFactor = BlendFactor::One;
        BlendFactor blendDestFactor = BlendFactor::Zero;
        BlendOperation blendOperation = BlendOperation::Add;
        VkFormat colorFormat = VK_FORMAT_UNDEFINED;
        VkFormat depthFormat = VK_FORMAT_UNDEFINED;
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

        bool operator==(PipelineKeyVulkan const& _other) const;
    };


    //////////////////////////////////////////
    struct PipelineKeyVulkanHash
    {
        Size operator()(PipelineKeyVulkan const& _key) const;
    };


    //////////////////////////////////////////
    // Class StateMachineVulkan
    //
    // Method surface intentionally mirrors StateMachineDX11 so RenderQueueVulkan
    // and MaterialVulkan call sites read the same as the DX11/GL backends -
    // internals bake state into VkPipeline objects + dynamic-rendering
    // begin/end scopes instead of D3D state objects.
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API StateMachineVulkan
    {
    public:

        //////////////////////////////////////////
        static S32 const c_colorAttachmentsMax = 4;

    public:

        //////////////////////////////////////////
        StateMachineVulkan(RenderSystemVulkan* _renderSystem);

        //////////////////////////////////////////
        ~StateMachineVulkan();


        //////////////////////////////////////////
        void setBlendEnabled(bool _value);

        //////////////////////////////////////////
        void setBlendFactors(BlendFactor _srcFactor, BlendFactor _destFactor);

        //////////////////////////////////////////
        void setBlendOperation(BlendOperation _value);

        //////////////////////////////////////////
        void setDepthTestEnabled(bool _value);

        //////////////////////////////////////////
        void setDepthTestCompareFunction(CompareFunction _value);

        //////////////////////////////////////////
        void setDepthWriteEnabled(bool _value);

        //////////////////////////////////////////
        void setCullEnabled(bool _value);

        //////////////////////////////////////////
        void setCullMode(CullMode _value);

        //////////////////////////////////////////
        void setStencilTestEnabled(bool _value);

        //////////////////////////////////////////
        void setStencilFunc(CompareFunction _function, S32 _referenceValue, U32 _readMask);

        //////////////////////////////////////////
        void setStencilOp(StencilOperation _fail, StencilOperation _depthFail, StencilOperation _pass);

        //////////////////////////////////////////
        void setStencilWriteMask(U32 _mask);

        //////////////////////////////////////////
        void setScissorTestEnabled(bool _value);

        //////////////////////////////////////////
        inline bool getScissorTestEnabled() const { return m_scissorTestEnabled; }

        //////////////////////////////////////////
        void setScissorRect(Rect2S const& _rect);

        //////////////////////////////////////////
        void setViewportRect(Rect2S const& _rect);

        //////////////////////////////////////////
        void setTopology(RenderDrawTopology _topology);


        //////////////////////////////////////////
        void setCurrentShader(ShaderVulkan* _shader);

        //////////////////////////////////////////
        inline ShaderVulkan* getCurrentShader() const { return m_currentShader; }

        //////////////////////////////////////////
        void setCurrentVertexArrayObject(VertexArrayObjectVulkan* _vao);


        //////////////////////////////////////////
        // Begins a vkCmdBeginRendering scope over the given color/depth attachments.
        // Ends any scope already open (on this or a different target) first.
        // _resolveColorViews (optional, parallel array to _colorViews) - when
        // _samples > 1 and a given slot is non-null, that color attachment
        // gets an automatic end-of-scope resolve into the given view (native
        // dynamic-rendering resolve, VK_RESOLVE_MODE_AVERAGE_BIT) instead of
        // storing its own (multisampled) content - used for the window's
        // MSAA color buffer, which must end up resolved into the (always
        // single-sample) swapchain image before present.
        void bindRenderTarget(
            VkImageView const* _colorViews,
            S32 _colorViewsCount,
            VkImageView _depthView,
            Vec2U const& _renderTargetSize,
            bool _flipY,
            VkFormat _colorFormat,
            VkFormat _depthFormat,
            VkSampleCountFlagBits _samples = VK_SAMPLE_COUNT_1_BIT,
            VkImageView const* _resolveColorViews = nullptr);

        //////////////////////////////////////////
        void unbindRenderTarget();

        //////////////////////////////////////////
        inline bool getFlipY() const { return m_flipY; }

        //////////////////////////////////////////
        inline Vec2U const& getRenderTargetSize() const { return m_renderTargetSize; }

        //////////////////////////////////////////
        inline bool isRenderingActive() const { return m_renderingActive; }


        //////////////////////////////////////////
        // Resolves and binds the pipeline + dynamic state + descriptor sets
        // for the next draw call. Call right before vkCmdDraw*
        void flushPipeline();

        //////////////////////////////////////////
        // Call after issuing raw command-buffer calls that bypass the state
        // machine (e.g. a blit/resolve) - drops cached bindings so the next
        // flushPipeline() rebinds everything
        void invalidateDeviceState();


        //////////////////////////////////////////
        inline RenderSystemVulkan* getRenderSystem() const { return m_renderSystem; }

        //////////////////////////////////////////
        VkDevice getDevice() const;

        //////////////////////////////////////////
        VkCommandBuffer getCommandBuffer() const;

    protected:

        //////////////////////////////////////////
        VkPipeline ensurePipeline();

    protected:
        RenderSystemVulkan* m_renderSystem = nullptr;

        // Blend
        bool m_blendEnabled = false;
        BlendFactor m_blendSrcFactor = BlendFactor::One;
        BlendFactor m_blendDestFactor = BlendFactor::Zero;
        BlendOperation m_blendOperation = BlendOperation::Add;

        // Depth
        bool m_depthTestEnabled = false;
        CompareFunction m_depthTestCompareFunction = CompareFunction::Less;
        bool m_depthWriteEnabled = true;

        // Stencil
        bool m_stencilTestEnabled = false;
        CompareFunction m_stencilTestCompareFunction = CompareFunction::Always;
        S32 m_stencilReferenceValue = 0;
        U32 m_stencilReadMask = 0xFF;
        U32 m_stencilWriteMask = 0xFF;
        StencilOperation m_stencilFailOperation = StencilOperation::Keep;
        StencilOperation m_stencilDepthFailOperation = StencilOperation::Keep;
        StencilOperation m_stencilPassOperation = StencilOperation::Keep;

        // Rasterizer
        bool m_cullEnabled = false;
        CullMode m_cullMode = CullMode::Back;
        bool m_scissorTestEnabled = false;
        Rect2S m_scissorRect = Rect2S(0, 0, 0, 0);
        Rect2S m_viewportRect = Rect2S(0, 0, 0, 0);
        RenderDrawTopology m_topology = RenderDrawTopology::Triangles;

        // Render target (dynamic rendering)
        VkImageView m_colorViews[c_colorAttachmentsMax];
        S32 m_colorViewsCount = 0;
        VkImageView m_depthView = VK_NULL_HANDLE;
        Vec2U m_renderTargetSize = Vec2U::c_zero;
        bool m_flipY = false;
        bool m_renderingActive = false;
        VkFormat m_colorFormat = VK_FORMAT_UNDEFINED;
        VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;
        VkSampleCountFlagBits m_samples = VK_SAMPLE_COUNT_1_BIT;

        // Shaders / VAO
        ShaderVulkan* m_currentShader = nullptr;
        VertexArrayObjectVulkan* m_currentVAO = nullptr;

        // Dirty flags
        bool m_pipelineDirty = true;
        bool m_dynamicStateDirty = true;
        bool m_viewportDirty = true;
        bool m_scissorDirty = true;

        // Pipeline cache (keyed by the reduced set of state that actually needs
        // a distinct VkPipeline - see PipelineKeyVulkan)
        UnorderedMap<PipelineKeyVulkan, VkPipeline, PipelineKeyVulkanHash> m_pipelines;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeStateMachineVulkan_hpp_
//////////////////////////////////////////
