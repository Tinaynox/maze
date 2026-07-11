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
        // Set-0 "global" descriptor set layout/set/UBO, shared by every
        // ShaderVulkan pipeline layout - see MazeShaderVulkan.hpp banner
        // comment for the exact binding convention (binding 0 = GlobalUniforms
        // UBO, bindings 1-3 = instance-stream SSBOs)
        inline VkDescriptorSetLayout getGlobalDescriptorSetLayout() const { return m_globalSetLayout; }

        //////////////////////////////////////////
        inline VkDescriptorSet getGlobalDescriptorSet(U32 _frameIndex) const { return m_globalDescriptorSets[_frameIndex]; }

        //////////////////////////////////////////
        // Writes into the shared GlobalUniforms UBO shadow (binding 0 of the
        // global set) - used by ShaderVulkan for uniforms reflected as
        // isGlobal (set 0), so every shader's "u_viewMatrix"-style uniform
        // uploads land in the one shared buffer instead of a per-shader copy
        void writeGlobalUniformData(U32 _offset, U8 const* _bytes, U32 _bytesCount);

        //////////////////////////////////////////
        // Uploads the dirty GlobalUniforms UBO into the current frame's mapped
        // buffer - called once per frame, before any draw
        void flushGlobalUniforms();

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
        // Called once per engine frame, before any render target is drawn to
        void beginFrame();

        //////////////////////////////////////////
        // Called once per engine frame, after every render target has been
        // drawn to (and, for windows, presented) - submits the frame's command
        // buffer and advances the frame-in-flight index
        void endFrame();


        //////////////////////////////////////////
        // Synchronous one-off command buffer, used by texture/buffer classes
        // to record + submit + wait a staging-buffer upload or layout
        // transition outside of the per-frame command buffer.
        VkCommandBuffer beginSingleTimeCommands();

        //////////////////////////////////////////
        void endSingleTimeCommands(VkCommandBuffer _commandBuffer);


        //////////////////////////////////////////
        // Depth MSAA resolve - unlike DX11 (which needs a fullscreen shader
        // workaround since ResolveSubresource doesn't support depth formats),
        // Vulkan 1.2+ VK_KHR_depth_stencil_resolve / vkCmdResolveImage on
        // depth-aspect images is supported directly by hardware that exposes
        // the relevant VkResolveModeFlagBits, so this just records a
        // vkCmdResolveImage into the current command buffer.
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

        // One-off transient command pool for beginSingleTimeCommands/endSingleTimeCommands
        VkCommandPool m_transientCommandPool = VK_NULL_HANDLE;

        S32 m_windowMaxAntialiasingLevel = 0;

        // Set-0 "global" descriptor set layout/sets/UBO - see
        // getGlobalDescriptorSetLayout() banner comment above
        VkDescriptorSetLayout m_globalSetLayout = VK_NULL_HANDLE;
        Vector<VkDescriptorSet> m_globalDescriptorSets;
        Vector<VkBuffer> m_globalUniformBuffers;
        Vector<VmaAllocation> m_globalUniformAllocations;
        Vector<void*> m_globalUniformMapped;
        U32 m_globalUniformBufferSize = 0u;
        Vector<U8> m_globalUniformShadow;
        bool m_globalUniformDirty[8] = {}; // indexed by frame-in-flight index, capped
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemVulkan_hpp_
//////////////////////////////////////////
