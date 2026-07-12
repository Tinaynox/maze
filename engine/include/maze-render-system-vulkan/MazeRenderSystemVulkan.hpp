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
#if (!defined(_MazeRenderSystemVulkan_hpp_))
#define _MazeRenderSystemVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkanConfig.hpp"
#include "maze-render-system-vulkan/MazeStateMachineVulkan.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystemVulkan);
    MAZE_USING_SHARED_PTR(ShaderVulkan);
    class VertexArrayObjectVulkan;
    class ShaderVulkan;
    class Texture2DVulkan;
    class Texture2DMSVulkan;


    //////////////////////////////////////////
    // A single-use command buffer + fence, used for synchronous staging-buffer
    // uploads (texture/vertex-buffer initial data) outside the per-frame
    // command buffer - the Vulkan equivalent of DX11's ID3D11DeviceContext::Map
    // on an immediate context, which needs no explicit synchronization.
    struct MAZE_RENDER_SYSTEM_VULKAN_API VulkanOneTimeCommandBuffer
    {
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    };


    //////////////////////////////////////////
    // Per-frame-in-flight resources - command pool/buffer and the fence that
    // guards CPU reuse of that frame's resources.
    struct MAZE_RENDER_SYSTEM_VULKAN_API VulkanFrameResources
    {
        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        VkFence inFlightFence = VK_NULL_HANDLE;
    };


    //////////////////////////////////////////
    // A layout transition deferred until the frame's dynamic-rendering scope
    // is guaranteed closed - vkCmdPipelineBarrier is illegal between
    // vkCmdBeginRendering/vkCmdEndRendering, but callers like
    // RenderWindowVulkan::endDraw() need to queue the swapchain image's
    // present-layout transition while it's still current/bound, before the
    // rendering scope actually gets torn down in endFrame(). See
    // RenderSystemVulkan::addPendingImageTransition/endFrame().
    struct MAZE_RENDER_SYSTEM_VULKAN_API VulkanPendingImageTransition
    {
        VkImage image = VK_NULL_HANDLE;
        VkImageAspectFlags aspect = 0u;
        VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImageLayout newLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    };


    //////////////////////////////////////////
    // Class RenderSystemVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API RenderSystemVulkan
        : public RenderSystem
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderSystemVulkan, RenderSystem);

    public:

        //////////////////////////////////////////
        virtual ~RenderSystemVulkan();

        //////////////////////////////////////////
        static RenderSystemVulkanPtr Create(RenderSystemVulkanConfig const& _config = RenderSystemVulkanConfig());


        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline RenderSystemVulkanConfig const& getConfig() const { return m_config; }


        //////////////////////////////////////////
        inline VkInstance getInstance() const { return m_instance; }

        //////////////////////////////////////////
        inline VkPhysicalDevice getPhysicalDevice() const { return m_physicalDevice; }

        //////////////////////////////////////////
        inline VkDevice getDevice() const { return m_device; }

        //////////////////////////////////////////
        inline VkQueue getGraphicsQueue() const { return m_graphicsQueue; }

        //////////////////////////////////////////
        inline VkQueue getPresentQueue() const { return m_presentQueue; }

        //////////////////////////////////////////
        inline U32 getGraphicsQueueFamilyIndex() const { return m_graphicsQueueFamilyIndex; }

        //////////////////////////////////////////
        inline U32 getPresentQueueFamilyIndex() const { return m_presentQueueFamilyIndex; }

        //////////////////////////////////////////
        inline VmaAllocator getAllocator() const { return m_allocator; }

        //////////////////////////////////////////
        inline VkPipelineCache getPipelineCache() const { return m_pipelineCache; }

        //////////////////////////////////////////
        inline VkDescriptorPool getDescriptorPool() const { return m_descriptorPool; }

        //////////////////////////////////////////
        inline StateMachineVulkan* getStateMachine() const { return m_stateMachine.get(); }

        //////////////////////////////////////////
        inline U32 getFramesInFlight() const { return m_config.framesInFlight; }

        //////////////////////////////////////////
        inline U32 getCurrentFrameIndex() const { return m_currentFrameIndex; }

        //////////////////////////////////////////
        // Increments every time ensureFrameOpen() actually opens a new frame
        // (not on redundant calls within an already-open frame) - used by
        // ShaderVulkan's per-draw material-descriptor-set pool to detect
        // "this is the first acquire of a new frame for this frame-in-flight
        // slot" without needing an explicit per-shader "frame began" callback
        inline U64 getFrameGeneration() const { return m_frameGeneration; }


        //////////////////////////////////////////
        // Set-0 "global" descriptor set layout/set/UBO, shared by every
        // ShaderVulkan pipeline layout - see MazeShaderVulkan.hpp banner
        // comment for the exact binding convention (binding 0 = GlobalUniforms
        // UBO, bindings 1-3 = instance-stream SSBOs)
        inline VkDescriptorSetLayout getGlobalDescriptorSetLayout() const { return m_globalSetLayout; }

        //////////////////////////////////////////
        // Writes into the shared GlobalUniforms UBO shadow (binding 0 of the
        // global set) - used by ShaderVulkan for uniforms reflected as
        // isGlobal (set 0), so every shader's "u_viewMatrix"-style uniform
        // uploads land in the one shared shadow buffer instead of a
        // per-shader copy. Actually pushed to the GPU per draw call by
        // acquireGlobalDescriptorSet(), not here.
        void writeGlobalUniformData(U32 _offset, U8 const* _bytes, U32 _bytesCount);

        //////////////////////////////////////////
        // Returns a descriptor set snapshotting the CURRENT GlobalUniforms
        // shadow contents, drawn from a per-draw-call instance pool - mirrors
        // ShaderVulkan::acquireMaterialDescriptorSet()'s rationale exactly,
        // and fixes the same class of bug that fix addressed. A single
        // shared-per-frame global UBO (the original design) is wrong the
        // moment more than one camera/render-target draws within the same
        // frame: descriptor CONTENTS only resolve at GPU execution time,
        // after the whole frame has finished recording, so every draw would
        // observe whichever camera's projection/view/lighting uniforms were
        // written *last* during recording - not the camera it was actually
        // recorded for. This was the confirmed root cause of 3D scene draws
        // reading a 2D canvas's orthographic projection matrix, discovered
        // via RenderDoc (raw u_projectionMatrix buffer dump showed a
        // 640x400 screen-space ortho matrix instead of the 3D camera's
        // perspective one). Bindings 1-3 (the instance-stream SSBOs) are
        // stable per-frame-in-flight buffers, so each pool instance only
        // needs to write them once, at creation.
        VkDescriptorSet acquireGlobalDescriptorSet();

        //////////////////////////////////////////
        inline U32 getGlobalUniformBufferSize() const { return m_globalUniformBufferSize; }

        //////////////////////////////////////////
        // Registers how large the shared GlobalUniforms UBO needs to be
        // (grown lazily the first time a shader reflects a bigger layout than
        // previously seen - every shader's GlobalUniforms block is expected to
        // declare the identical fields in the identical order, since they all
        // #include the same MazeCommonShaderHeader.mzglslvk, but this guards
        // against a mismatch instead of silently truncating)
        void ensureGlobalUniformBufferSize(U32 _size);


        //////////////////////////////////////////
        // Shared instance-stream SSBOs (set-0 bindings 1/2/3 - see
        // MazeShaderVulkan.hpp banner comment), one buffer per frame-in-flight,
        // capacity shared across every render target drawn within a frame (see
        // allocateInstanceStreamSlots). Written directly by
        // InstanceStreamModelMatrixVulkan/ColorVulkan/UVVulkan
        // (instance-stream/MazeInstanceStreamsVulkan.cpp), not through the
        // named-uniform system.
        static U32 const c_instanceStreamCapacity = 16384u * 2u;

        //////////////////////////////////////////
        void* getModelMatricesStreamMapped(U32 _frameIndex) const;

        //////////////////////////////////////////
        void* getColorsStreamMapped(U32 _frameIndex) const;

        //////////////////////////////////////////
        // UV streams are interleaved per instance (2 vec4 per instance slot:
        // channel 0 then channel 1), matching MazeInstanceStream.mzglslvk's
        // 'UVStreamsBuffer' struct-per-instance layout - NOT two concatenated
        // per-channel regions
        void* getUVStreamMapped(U32 _frameIndex) const;

        //////////////////////////////////////////
        // Allocates _count contiguous slots from the current frame's shared
        // instance-stream cursor (reset to 0 every ensureFrameOpen()) and
        // returns the starting offset - callers write their data at that
        // offset in the mapped buffers above, and pass the same offset as the
        // 'u_instanceOffset' push constant for the following draw call(s)
        S32 allocateInstanceStreamSlots(S32 _count);


        //////////////////////////////////////////
        virtual bool isTextureFormatSupported(PixelFormat::Enum _pixelFormat) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getWindowMaxAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getWindowCurrentAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getTextureMaxSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getTextureMaxAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getTextureMaxAnisotropyLevel() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool setCurrentRenderTarget(RenderTarget* _renderTarget) MAZE_OVERRIDE;

        //////////////////////////////////////////
        // Closes any dynamic-rendering scope currently open on the frame's
        // command buffer and forgets the current render target, so a
        // subsequent setCurrentRenderTarget() call - even to the same target -
        // fully re-runs processRenderTargetSet() and reopens a fresh scope.
        // Used by standalone image-transfer operations (RenderBufferVulkan::blit())
        // that need to record vkCmdPipelineBarrier/vkCmdResolveImage/vkCmdCopyImage
        // outside of any render target's currently-bound rendering scope, since
        // none of those calls are legal while a vkCmdBeginRendering/vkCmdEndRendering
        // scope is open.
        void interruptActiveRenderTarget();

        //////////////////////////////////////////
        virtual void clearCurrentRenderTarget(
            bool _colorBuffer = true,
            bool _depthBuffer = true,
            bool _stencilBuffer = true) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual ShaderUniformPtr createShaderUniform(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderWindowPtr createRenderWindow(RenderWindowParams const& _params) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual VertexArrayObjectPtr createVertexArrayObject(RenderTarget* _renderTarget = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual VertexBufferObjectPtr createVertexBufferObject(
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false,
            RenderTarget* _renderTarget = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Texture2DPtr createTexture2D() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Texture2DMSPtr createTexture2DMS() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual TextureCubePtr createTextureCube() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual MaterialPtr createMaterial() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderPassPtr createRenderPass(
            RenderPassType _passType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual GPUVertexBufferPtr createGPUVertexBuffer(
            VertexDataDescription const& _vertexDataDescription,
            Size _vertexCount,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual GPUTextureBufferPtr createGPUTextureBuffer(
            Vec2U const& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderBufferPtr createRenderBuffer(
            RenderBuffer::RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>(),
            RenderTarget* _renderTarget = nullptr) MAZE_OVERRIDE;


        //////////////////////////////////////////
        // Sampler objects are shared and cached by their description
        VkSampler ensureSampler(
            TextureFilter _minFilter,
            TextureFilter _magFilter,
            TextureWrap _wrapS,
            TextureWrap _wrapT,
            TextureWrap _wrapR,
            F32 _anisotropyLevel,
            F32 _maxLod,
            Vec4F const& _borderColor);

        //////////////////////////////////////////
        // Small zero-filled buffer bound in place of vertex attributes the VAO doesn't provide
        VkBuffer getZeroVertexBuffer() const { return m_zeroVertexBuffer; }


        //////////////////////////////////////////
        // Command buffer currently being recorded for this frame - every
        // RenderQueueVulkan::draw() across every render target records into
        // this same buffer within a frame, mirroring DX11's single immediate
        // device context.
        VkCommandBuffer getCurrentCommandBuffer() const;

        //////////////////////////////////////////
        // Opens a frame (waits the next frame-in-flight fence, resets its
        // command pool, begins recording) if one isn't already open. A
        // "frame" here spans however many render targets get drawn to
        // between the last endFrame() and the next one - RenderTarget::beginDraw()
        // calls this lazily so consecutive targets (e.g. a shadow buffer then
        // the main window) share one command buffer/submission, and
        // RenderWindowVulkan::swapBuffers() is what actually closes it (a
        // present requires the work to already be submitted).
        void ensureFrameOpen();

        //////////////////////////////////////////
        inline bool isFrameOpen() const { return m_frameOpen; }

        //////////////////////////////////////////
        // Adds a semaphore the next endFrame() submission should wait on
        // before executing (e.g. a swapchain image-available semaphore) at
        // the given pipeline stage
        void addFrameWaitSemaphore(VkSemaphore _semaphore, VkPipelineStageFlags _waitStage);

        //////////////////////////////////////////
        // Queues an image-layout transition to be recorded once the current
        // frame's dynamic-rendering scope is closed (right before its command
        // buffer stops recording in endFrame()) - vkCmdPipelineBarrier can't
        // be called while a vkCmdBeginRendering/vkCmdEndRendering scope is
        // still open, so callers that need to transition a still-bound
        // render target's image (e.g. RenderWindowVulkan::endDraw() wanting
        // the swapchain image in PRESENT_SRC_KHR) must defer through here
        // instead of calling TransitionImageLayoutVulkan directly.
        void addPendingImageTransition(
            VkImage _image,
            VkImageAspectFlags _aspect,
            VkImageLayout _oldLayout,
            VkImageLayout _newLayout);

        //////////////////////////////////////////
        // Submits the currently open frame's command buffer (waiting on any
        // semaphores registered via addFrameWaitSemaphore since the frame was
        // opened) and advances the frame-in-flight index.
        //
        // _signalSemaphore is signaled once the submitted work completes, and
        // must be supplied by the caller rather than owned per-frame-in-flight
        // here: a frame-in-flight-indexed semaphore (the original design) can
        // get re-signaled by a new vkQueueSubmit before the swapchain's
        // presentation engine has finished consuming its *previous* signal,
        // whenever framesInFlight doesn't line up 1:1 with how long a
        // presented image actually stays in use - producing
        // VUID-vkQueueSubmit-pSignalSemaphores-00067 and a visibly corrupted/
        // black frame every time it happens. The correct semaphore lifetime
        // is tied to the swapchain IMAGE, not the frame-in-flight slot, so
        // RenderWindowVulkan::swapBuffers() supplies one indexed by
        // m_currentImageIndex (sized to swapchain image count, alongside
        // m_imageAvailableSemaphores).
        void endFrame(VkSemaphore _signalSemaphore);

        //////////////////////////////////////////
        // Safe replacement for a bare vkDeviceWaitIdle() before destroying
        // any GPU resource (image, view, buffer, descriptor set...) that a
        // command buffer might reference. vkDeviceWaitIdle() alone only
        // waits for previously SUBMITTED work - it does nothing for a
        // command buffer that's still open on the CPU (mid-recording), so
        // destroying something that buffer already referenced corrupts it;
        // recording then keeps going (more commands appended against the
        // now-dangling handle) until it's eventually submitted, which is a
        // validation-error minefield and can end in VK_ERROR_DEVICE_LOST.
        // Confirmed via a real repro: a mid-drag window resize recreating
        // the swapchain while its frame was still recording. Every
        // destroy-then-recreate GPU resource path in this backend
        // (textures, buffers, VAOs, the swapchain, the global-uniforms
        // pool...) can in principle be triggered mid-frame, so they should
        // all go through this instead of calling vkDeviceWaitIdle() directly.
        void waitDeviceIdleSafe();


        //////////////////////////////////////////
        // Synchronous one-off command buffer, used by texture/buffer classes
        // to record + submit + wait a staging-buffer upload or layout
        // transition outside of the per-frame command buffer.
        VkCommandBuffer beginSingleTimeCommands();

        //////////////////////////////////////////
        void endSingleTimeCommands(VkCommandBuffer _commandBuffer);


        //////////////////////////////////////////
        // Depth MSAA resolve. vkCmdResolveImage (the standalone copy-style
        // resolve command) is a COLOR-only operation - its valid-usage rules
        // require VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT on both images, which
        // no depth/stencil format has. VK_KHR_depth_stencil_resolve (core
        // 1.2+) only extends resolve support for depth/stencil via the
        // *render-pass-integrated* path (VkRenderingAttachmentInfo::
        // resolveMode/resolveImageView, set when the source's own rendering
        // scope begins - see StateMachineVulkan::bindRenderTarget()'s
        // _resolveColorViews, its color-only equivalent). RenderBufferVulkan::
        // blit() runs after that scope has already closed and against a
        // destination buffer not known at bind time, so that path can't be
        // retrofitted here - this instead mirrors RenderSystemDX11::
        // resolveDepthMSAA()'s reference approach: draw a fullscreen triangle
        // with a shader that reads one sample of the multisampled depth
        // texture per pixel (texelFetch, sample index 0) and writes it as
        // this draw's own depth output, into a normal (non-multisampled)
        // depth attachment.
        bool resolveDepthMSAA(
            Texture2DVulkan* _dstTexture,
            Texture2DMSVulkan* _srcTexture);

    protected:

        //////////////////////////////////////////
        RenderSystemVulkan();

        //////////////////////////////////////////
        virtual bool init(RenderSystemVulkanConfig const& _config);

        //////////////////////////////////////////
        bool createInstance();

        //////////////////////////////////////////
        bool pickPhysicalDevice();

        //////////////////////////////////////////
        bool createLogicalDevice();

        //////////////////////////////////////////
        bool createAllocator();

        //////////////////////////////////////////
        bool createFrameResources();

        //////////////////////////////////////////
        bool createInstanceStreamBuffers();

    protected:
        RenderSystemVulkanConfig m_config;

        VkInstance m_instance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties m_physicalDeviceProperties;
        VkDevice m_device = VK_NULL_HANDLE;

        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue = VK_NULL_HANDLE;
        U32 m_graphicsQueueFamilyIndex = 0u;
        U32 m_presentQueueFamilyIndex = 0u;

        VmaAllocator m_allocator = VK_NULL_HANDLE;

        VkPipelineCache m_pipelineCache = VK_NULL_HANDLE;
        VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;

        UniquePtr<StateMachineVulkan> m_stateMachine;

        UnorderedMap<U64, VkSampler> m_samplers;

        VkBuffer m_zeroVertexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_zeroVertexBufferAllocation = VK_NULL_HANDLE;

        Vector<VulkanFrameResources> m_frameResources;
        U32 m_currentFrameIndex = 0u;
        bool m_frameOpen = false;
        U64 m_frameGeneration = 0u;
        Vector<VkSemaphore> m_frameWaitSemaphores;
        Vector<VkPipelineStageFlags> m_frameWaitStages;
        Vector<VulkanPendingImageTransition> m_pendingImageTransitions;

        // One-off transient command pool for beginSingleTimeCommands/endSingleTimeCommands
        VkCommandPool m_transientCommandPool = VK_NULL_HANDLE;

        S32 m_windowMaxAntialiasingLevel = 0;

        // Depth-resolve fullscreen-pass state (lazily created on first
        // resolveDepthMSAA() call, never torn down until shutdown) - a
        // hand-rolled pipeline outside the normal ShaderVulkan/Material
        // system, mirroring RenderSystemDX11::resolveDepthMSAA()'s
        // equally hand-rolled D3D11 objects. See resolveDepthMSAA()'s
        // banner comment for why this exists at all.
        VkShaderModule m_depthResolveVertModule = VK_NULL_HANDLE;
        VkShaderModule m_depthResolveFragModule = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_depthResolveDescriptorSetLayout = VK_NULL_HANDLE;
        VkPipelineLayout m_depthResolvePipelineLayout = VK_NULL_HANDLE;
        VkPipeline m_depthResolvePipeline = VK_NULL_HANDLE;
        VkFormat m_depthResolvePipelineFormat = VK_FORMAT_UNDEFINED;
        VkSampler m_depthResolveSampler = VK_NULL_HANDLE;
        // One descriptor set per frame-in-flight slot, updated in place each
        // call - avoids updating a set a still-in-flight command buffer from
        // a previous frame might still reference (same hazard
        // ShaderVulkan::acquireMaterialDescriptorSet() solves for material
        // descriptor sets, just without needing a growable per-draw-call
        // pool since this runs at most once or twice a frame, not once per
        // draw call)
        Vector<VkDescriptorSet> m_depthResolveDescriptorSets;
        bool m_depthResolveInitFailed = false;

        // Set-0 "global" descriptor set layout - see
        // getGlobalDescriptorSetLayout() banner comment above
        VkDescriptorSetLayout m_globalSetLayout = VK_NULL_HANDLE;
        U32 m_globalUniformBufferSize = 0u;
        Vector<U8> m_globalUniformShadow;

        // Per-draw-call instance pool for the global descriptor set - see
        // acquireGlobalDescriptorSet() banner comment
        struct GlobalUniformInstance
        {
            VkBuffer buffer = VK_NULL_HANDLE;
            VmaAllocation allocation = VK_NULL_HANDLE;
            void* mapped = nullptr;
            VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
        };
        Vector<Vector<GlobalUniformInstance>> m_globalInstancePool;
        Vector<U32> m_globalInstancePoolUsed;
        Vector<U64> m_globalInstancePoolGeneration;

        // Instance-stream SSBOs (set-0 bindings 1/2/3), one per frame-in-flight
        Vector<VkBuffer> m_modelMatricesStreamBuffers;
        Vector<VmaAllocation> m_modelMatricesStreamAllocations;
        Vector<void*> m_modelMatricesStreamMapped;
        Vector<VkBuffer> m_colorsStreamBuffers;
        Vector<VmaAllocation> m_colorsStreamAllocations;
        Vector<void*> m_colorsStreamMapped;
        Vector<VkBuffer> m_uvStreamBuffers;
        Vector<VmaAllocation> m_uvStreamAllocations;
        Vector<void*> m_uvStreamMapped;
        U32 m_modelMatricesStreamSize = 0u;
        U32 m_colorsStreamSize = 0u;
        U32 m_uvStreamSize = 0u;
        S32 m_instanceStreamCursor = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemVulkan_hpp_
//////////////////////////////////////////
