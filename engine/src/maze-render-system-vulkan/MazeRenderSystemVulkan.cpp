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
#define VMA_IMPLEMENTATION
#include "MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderUniformVulkan.hpp"
#include "maze-render-system-vulkan/MazeTexture2DVulkan.hpp"
#include "maze-render-system-vulkan/MazeTexture2DMSVulkan.hpp"
#include "maze-render-system-vulkan/MazeTextureCubeVulkan.hpp"
#include "maze-render-system-vulkan/MazeVertexArrayObjectVulkan.hpp"
#include "maze-render-system-vulkan/MazeVertexBufferObjectVulkan.hpp"
#include "maze-render-system-vulkan/MazeMaterialVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderPassVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderBufferVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderWindowVulkan.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallbackVulkan(
        VkDebugUtilsMessageSeverityFlagBitsEXT _severity,
        VkDebugUtilsMessageTypeFlagsEXT _type,
        VkDebugUtilsMessengerCallbackDataEXT const* _callbackData,
        void* _userData)
    {
        if (_severity & (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT))
            Debug::LogWarning("[Vulkan] %s", _callbackData->pMessage);
        return VK_FALSE;
    }


    //////////////////////////////////////////
    // Class RenderSystemVulkan
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderSystemVulkan, RenderSystem);

    //////////////////////////////////////////
    RenderSystemVulkan::RenderSystemVulkan()
    {
        memset(&m_physicalDeviceProperties, 0, sizeof(m_physicalDeviceProperties));
    }

    //////////////////////////////////////////
    RenderSystemVulkan::~RenderSystemVulkan()
    {
        m_spriteManager.reset();
        m_systemFontManager.reset();
        m_materialManager.reset();
        m_textureManager.reset();
        m_renderMeshManager.reset();
        m_shaderSystem.reset();

        if (m_device != VK_NULL_HANDLE)
            vkDeviceWaitIdle(m_device);

        for (auto& samplerData : m_samplers)
            if (samplerData.second != VK_NULL_HANDLE)
                vkDestroySampler(m_device, samplerData.second, nullptr);
        m_samplers.clear();

        if (m_zeroVertexBuffer != VK_NULL_HANDLE)
            vmaDestroyBuffer(m_allocator, m_zeroVertexBuffer, m_zeroVertexBufferAllocation);

        m_stateMachine.reset();

        for (VulkanFrameResources& frame : m_frameResources)
        {
            if (frame.inFlightFence != VK_NULL_HANDLE)
                vkDestroyFence(m_device, frame.inFlightFence, nullptr);
            if (frame.commandPool != VK_NULL_HANDLE)
                vkDestroyCommandPool(m_device, frame.commandPool, nullptr);
        }
        m_frameResources.clear();

        if (m_transientCommandPool != VK_NULL_HANDLE)
            vkDestroyCommandPool(m_device, m_transientCommandPool, nullptr);

        if (m_descriptorPool != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);

        if (m_pipelineCache != VK_NULL_HANDLE)
            vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr);

        if (m_globalSetLayout != VK_NULL_HANDLE)
            vkDestroyDescriptorSetLayout(m_device, m_globalSetLayout, nullptr);

        for (Size i = 0; i < m_globalUniformBuffers.size(); ++i)
            if (m_globalUniformBuffers[i] != VK_NULL_HANDLE)
                vmaDestroyBuffer(m_allocator, m_globalUniformBuffers[i], m_globalUniformAllocations[i]);

        if (m_allocator != VK_NULL_HANDLE)
            vmaDestroyAllocator(m_allocator);

        if (m_device != VK_NULL_HANDLE)
            vkDestroyDevice(m_device, nullptr);

        if (m_debugMessenger != VK_NULL_HANDLE)
        {
            auto destroyFn = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT");
            if (destroyFn)
                destroyFn(m_instance, m_debugMessenger, nullptr);
        }

        if (m_instance != VK_NULL_HANDLE)
            vkDestroyInstance(m_instance, nullptr);
    }

    //////////////////////////////////////////
    RenderSystemVulkanPtr RenderSystemVulkan::Create(RenderSystemVulkanConfig const& _config)
    {
        RenderSystemVulkanPtr renderSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemVulkan, renderSystem, init(_config));
        return renderSystem;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::init(RenderSystemVulkanConfig const& _config)
    {
        if (!RenderSystem::init())
            return false;

        m_config = _config;

        if (!createInstance())
            return false;

        if (!pickPhysicalDevice())
            return false;

        if (!createLogicalDevice())
            return false;

        if (!createAllocator())
            return false;

        // Pipeline cache
        VkPipelineCacheCreateInfo pipelineCacheInfo = {};
        pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        MAZE_VK_CALL(vkCreatePipelineCache(m_device, &pipelineCacheInfo, nullptr, &m_pipelineCache));

        // A generous shared descriptor pool - every shader/material allocates
        // its set-1 descriptor set from here, plus the one shared set-0
        // global set
        VkDescriptorPoolSize poolSizes[2] = {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = 4096u;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = 4096u;

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 4096u;
        poolInfo.poolSizeCount = 2u;
        poolInfo.pPoolSizes = poolSizes;
        MAZE_VK_CALL(vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool));

        // Global (set 0) descriptor set layout - binding 0 = GlobalUniforms
        // UBO, bindings 1-3 = instance-stream SSBOs (see MazeShaderVulkan.hpp
        // banner comment for the full convention)
        {
            VkDescriptorSetLayoutBinding bindings[4] = {};
            bindings[0].binding = 0u;
            bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            bindings[0].descriptorCount = 1u;
            bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            for (U32 i = 1; i <= 3; ++i)
            {
                bindings[i].binding = i;
                bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
                bindings[i].descriptorCount = 1u;
                bindings[i].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            }

            VkDescriptorSetLayoutCreateInfo layoutInfo = {};
            layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            layoutInfo.bindingCount = 4u;
            layoutInfo.pBindings = bindings;
            MAZE_VK_CALL(vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_globalSetLayout));
        }

        if (!createFrameResources())
            return false;

        // Global uniform buffers (one per frame-in-flight) + descriptor sets.
        // Instance-stream SSBO bindings (1-3) are written in by
        // InstanceStreamsVulkan once it's constructed (RenderQueueVulkan owns
        // the instance streams, created lazily on first use) - until then the
        // global descriptor set simply has those bindings unwritten, which is
        // fine since nothing references them before the first draw.
        {
            m_globalUniformBuffers.resize(m_config.framesInFlight, VK_NULL_HANDLE);
            m_globalUniformAllocations.resize(m_config.framesInFlight, VK_NULL_HANDLE);
            m_globalUniformMapped.resize(m_config.framesInFlight, nullptr);
            m_globalDescriptorSets.resize(m_config.framesInFlight, VK_NULL_HANDLE);

            U32 initialSize = 256u;
            for (U32 i = 0; i < m_config.framesInFlight; ++i)
            {
                VkBufferCreateInfo bufferInfo = {};
                bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
                bufferInfo.size = initialSize;
                bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
                bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

                VmaAllocationCreateInfo allocInfo = {};
                allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
                allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

                VmaAllocationInfo allocResultInfo = {};
                MAZE_VK_CALL(vmaCreateBuffer(
                    m_allocator, &bufferInfo, &allocInfo,
                    &m_globalUniformBuffers[i], &m_globalUniformAllocations[i], &allocResultInfo));
                m_globalUniformMapped[i] = allocResultInfo.pMappedData;

                VkDescriptorSetAllocateInfo dsAllocInfo = {};
                dsAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                dsAllocInfo.descriptorPool = m_descriptorPool;
                dsAllocInfo.descriptorSetCount = 1u;
                dsAllocInfo.pSetLayouts = &m_globalSetLayout;
                MAZE_VK_CALL(vkAllocateDescriptorSets(m_device, &dsAllocInfo, &m_globalDescriptorSets[i]));

                VkDescriptorBufferInfo bufInfo = {};
                bufInfo.buffer = m_globalUniformBuffers[i];
                bufInfo.offset = 0u;
                bufInfo.range = initialSize;

                VkWriteDescriptorSet write = {};
                write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                write.dstSet = m_globalDescriptorSets[i];
                write.dstBinding = 0u;
                write.descriptorCount = 1u;
                write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                write.pBufferInfo = &bufInfo;
                vkUpdateDescriptorSets(m_device, 1u, &write, 0u, nullptr);
            }
            m_globalUniformBufferSize = initialSize;
            m_globalUniformShadow.resize(initialSize, 0u);
        }

        m_stateMachine = MakeUnique<StateMachineVulkan>(this);

        // Zero-filled buffer for vertex attributes missing from a VAO
        {
            U8 zeroData[64];
            memset(zeroData, 0, sizeof(zeroData));

            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = sizeof(zeroData);
            bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo = {};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;

            MAZE_VK_CALL(vmaCreateBuffer(m_allocator, &bufferInfo, &allocInfo, &m_zeroVertexBuffer, &m_zeroVertexBufferAllocation, nullptr));

            VkCommandBuffer cmd = beginSingleTimeCommands();
            vkCmdFillBuffer(cmd, m_zeroVertexBuffer, 0, sizeof(zeroData), 0u);
            endSingleTimeCommands(cmd);
        }

        ShaderSystemVulkan::Initialize(m_shaderSystem, getSharedPtr());
        if (!m_shaderSystem)
            return false;

        processSystemInited();

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::createInstance()
    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Maze";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Maze Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;

        Vector<CString> extensions = { VK_KHR_SURFACE_EXTENSION_NAME };
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif

        Vector<CString> layers;
        if (m_config.validationLayer)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            layers.push_back("VK_LAYER_KHRONOS_validation");
        }

        VkInstanceCreateInfo instanceInfo = {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = (U32)extensions.size();
        instanceInfo.ppEnabledExtensionNames = extensions.data();
        instanceInfo.enabledLayerCount = (U32)layers.size();
        instanceInfo.ppEnabledLayerNames = layers.empty() ? nullptr : layers.data();

        VkResult result = vkCreateInstance(&instanceInfo, nullptr, &m_instance);
        if (result != VK_SUCCESS && m_config.validationLayer)
        {
            MAZE_WARNING("VK_LAYER_KHRONOS_validation is not available - creating instance without it");
            layers.clear();
            instanceInfo.enabledLayerCount = 0u;
            instanceInfo.ppEnabledLayerNames = nullptr;
            result = vkCreateInstance(&instanceInfo, nullptr, &m_instance);
        }
        MAZE_ERROR_RETURN_VALUE_IF(result != VK_SUCCESS, false, "vkCreateInstance failed! result=%d", (S32)result);

        if (m_config.validationLayer)
        {
            auto createFn = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT");
            if (createFn)
            {
                VkDebugUtilsMessengerCreateInfoEXT messengerInfo = {};
                messengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
                messengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
                messengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
                messengerInfo.pfnUserCallback = DebugUtilsMessengerCallbackVulkan;
                createFn(m_instance, &messengerInfo, nullptr, &m_debugMessenger);
            }
        }

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::pickPhysicalDevice()
    {
        U32 deviceCount = 0u;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        MAZE_ERROR_RETURN_VALUE_IF(deviceCount == 0u, false, "No Vulkan-capable physical devices found!");

        Vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

        S32 bestScore = -1;
        VkPhysicalDevice bestDevice = VK_NULL_HANDLE;

        for (U32 i = 0; i < deviceCount; ++i)
        {
            if (m_config.physicalDeviceIndex >= 0 && (S32)i != m_config.physicalDeviceIndex)
                continue;

            VkPhysicalDeviceProperties props;
            vkGetPhysicalDeviceProperties(devices[i], &props);

            S32 score = 0;
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                score += 1000;
            else
            if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
                score += 100;
            score += (S32)(props.limits.maxImageDimension2D / 1024u);

            if (score > bestScore)
            {
                bestScore = score;
                bestDevice = devices[i];
                m_physicalDeviceProperties = props;
            }
        }

        MAZE_ERROR_RETURN_VALUE_IF(bestDevice == VK_NULL_HANDLE, false, "No suitable Vulkan physical device found!");
        m_physicalDevice = bestDevice;

        Debug::Log("Vulkan Adapter: %s", m_physicalDeviceProperties.deviceName);

        // Max supported color MSAA
        VkSampleCountFlags counts = m_physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                     m_physicalDeviceProperties.limits.framebufferDepthSampleCounts;
        m_windowMaxAntialiasingLevel = 1;
        if (counts & VK_SAMPLE_COUNT_16_BIT) m_windowMaxAntialiasingLevel = 16;
        else if (counts & VK_SAMPLE_COUNT_8_BIT) m_windowMaxAntialiasingLevel = 8;
        else if (counts & VK_SAMPLE_COUNT_4_BIT) m_windowMaxAntialiasingLevel = 4;
        else if (counts & VK_SAMPLE_COUNT_2_BIT) m_windowMaxAntialiasingLevel = 2;

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::createLogicalDevice()
    {
        U32 queueFamilyCount = 0u;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
        Vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, queueFamilies.data());

        m_graphicsQueueFamilyIndex = UINT32_MAX;
        for (U32 i = 0; i < queueFamilyCount; ++i)
        {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_graphicsQueueFamilyIndex = i;
                break;
            }
        }
        MAZE_ERROR_RETURN_VALUE_IF(m_graphicsQueueFamilyIndex == UINT32_MAX, false, "No graphics queue family found!");

        // On most desktop drivers the graphics queue family also supports
        // presentation - actual WSI compatibility is verified again in
        // RenderWindowVulkan::createSurface via vkGetPhysicalDeviceSurfaceSupportKHR
        m_presentQueueFamilyIndex = m_graphicsQueueFamilyIndex;

        F32 queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueInfo = {};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
        queueInfo.queueCount = 1u;
        queueInfo.pQueuePriorities = &queuePriority;

        Vector<CString> deviceExtensions =
        {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME,
            VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
            VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
        };

        VkPhysicalDeviceVulkan13Features features13 = {};
        features13.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
        features13.dynamicRendering = VK_TRUE;
        features13.synchronization2 = VK_TRUE;

        VkPhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeatures = {};
        extendedDynamicStateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
        extendedDynamicStateFeatures.extendedDynamicState = VK_TRUE;
        extendedDynamicStateFeatures.pNext = &features13;

        VkPhysicalDeviceFeatures2 features2 = {};
        features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        features2.pNext = &extendedDynamicStateFeatures;
        features2.features.samplerAnisotropy = VK_TRUE;
        features2.features.fillModeNonSolid = VK_TRUE;

        VkDeviceCreateInfo deviceInfo = {};
        deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceInfo.pNext = &features2;
        deviceInfo.queueCreateInfoCount = 1u;
        deviceInfo.pQueueCreateInfos = &queueInfo;
        deviceInfo.enabledExtensionCount = (U32)deviceExtensions.size();
        deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

        MAZE_VK_CALL(vkCreateDevice(m_physicalDevice, &deviceInfo, nullptr, &m_device));
        MAZE_ERROR_RETURN_VALUE_IF(m_device == VK_NULL_HANDLE, false, "vkCreateDevice failed!");

        vkGetDeviceQueue(m_device, m_graphicsQueueFamilyIndex, 0u, &m_graphicsQueue);
        m_presentQueue = m_graphicsQueue;

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::createAllocator()
    {
        VmaAllocatorCreateInfo allocatorInfo = {};
        allocatorInfo.vulkanApiVersion = VK_API_VERSION_1_3;
        allocatorInfo.physicalDevice = m_physicalDevice;
        allocatorInfo.device = m_device;
        allocatorInfo.instance = m_instance;

        MAZE_VK_CALL(vmaCreateAllocator(&allocatorInfo, &m_allocator));
        return m_allocator != VK_NULL_HANDLE;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::createFrameResources()
    {
        m_frameResources.resize(m_config.framesInFlight);

        for (VulkanFrameResources& frame : m_frameResources)
        {
            VkCommandPoolCreateInfo poolInfo = {};
            poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            poolInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
            MAZE_VK_CALL(vkCreateCommandPool(m_device, &poolInfo, nullptr, &frame.commandPool));

            VkCommandBufferAllocateInfo cmdAllocInfo = {};
            cmdAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            cmdAllocInfo.commandPool = frame.commandPool;
            cmdAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            cmdAllocInfo.commandBufferCount = 1u;
            MAZE_VK_CALL(vkAllocateCommandBuffers(m_device, &cmdAllocInfo, &frame.commandBuffer));

            VkFenceCreateInfo fenceInfo = {};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
            MAZE_VK_CALL(vkCreateFence(m_device, &fenceInfo, nullptr, &frame.inFlightFence));
        }

        VkCommandPoolCreateInfo transientPoolInfo = {};
        transientPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        transientPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        transientPoolInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
        MAZE_VK_CALL(vkCreateCommandPool(m_device, &transientPoolInfo, nullptr, &m_transientCommandPool));

        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemVulkan::getName()
    {
        static String s_name = "Vulkan";
        return s_name;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::isTextureFormatSupported(PixelFormat::Enum _pixelFormat)
    {
        return GetPixelFormatVulkan(_pixelFormat) != VK_FORMAT_UNDEFINED;
    }

    //////////////////////////////////////////
    S32 RenderSystemVulkan::getWindowMaxAntialiasingLevelSupport()
    {
        return m_windowMaxAntialiasingLevel;
    }

    //////////////////////////////////////////
    S32 RenderSystemVulkan::getWindowCurrentAntialiasingLevelSupport()
    {
        return m_windowMaxAntialiasingLevel;
    }

    //////////////////////////////////////////
    S32 RenderSystemVulkan::getTextureMaxSize()
    {
        return (S32)m_physicalDeviceProperties.limits.maxImageDimension2D;
    }

    //////////////////////////////////////////
    S32 RenderSystemVulkan::getTextureMaxAntialiasingLevelSupport()
    {
        return m_windowMaxAntialiasingLevel;
    }

    //////////////////////////////////////////
    F32 RenderSystemVulkan::getTextureMaxAnisotropyLevel()
    {
        return m_physicalDeviceProperties.limits.maxSamplerAnisotropy;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::setCurrentRenderTarget(RenderTarget* _renderTarget)
    {
        if (m_currentRenderTarget == _renderTarget)
            return true;

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetWillReset();
            m_currentRenderTarget = nullptr;
        }

        if (_renderTarget)
        {
            if (!_renderTarget->processRenderTargetWillSet())
                return false;
        }

        m_currentRenderTarget = _renderTarget;

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetSet();
        }

        return true;
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::clearCurrentRenderTarget(
        bool _colorBuffer,
        bool _depthBuffer,
        bool _stencilBuffer)
    {
        // With dynamic rendering, clearing is expressed as the load-op of the
        // vkCmdBeginRendering call that StateMachineVulkan::bindRenderTarget
        // already issued when this render target became current - see
        // RenderQueueVulkan's ClearCurrentRenderTarget command handling,
        // which is where the actual VkClearValue is threaded through (this
        // entry point exists to satisfy the RenderSystem interface but the
        // real clear happens at begin-rendering time for this backend, unlike
        // OpenGL/DX11 which can clear a bound target at an arbitrary point via
        // a direct clear call).
        if (!m_currentRenderTarget)
            return;

        MAZE_TODO; // see RenderQueueVulkan.cpp for the actual clear path
    }

    //////////////////////////////////////////
    ShaderUniformPtr RenderSystemVulkan::createShaderUniform(
        ShaderPtr const& _shader,
        ShaderUniformType _type)
    {
        return ShaderUniformVulkan::Create(_shader, _type);
    }

    //////////////////////////////////////////
    RenderWindowPtr RenderSystemVulkan::createRenderWindow(RenderWindowParams const& _params)
    {
        RenderWindowPtr window = RenderWindowVulkan::Create(this, _params);
        if (!window)
        {
            MAZE_ERROR("RenderWindowVulkan cannot be created!");
            return RenderWindowPtr();
        }

        if (!processRenderWindowCreated(window))
        {
            MAZE_ERROR("processRenderWindowCreated failed!");
            return RenderWindowPtr();
        }

        return window;
    }

    //////////////////////////////////////////
    VertexArrayObjectPtr RenderSystemVulkan::createVertexArrayObject(RenderTarget* _renderTarget)
    {
        return VertexArrayObjectVulkan::Create(this);
    }

    //////////////////////////////////////////
    VertexBufferObjectPtr RenderSystemVulkan::createVertexBufferObject(
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping,
        RenderTarget* _renderTarget)
    {
        return VertexBufferObjectVulkan::Create(this, _accessType, _singleMapping);
    }

    //////////////////////////////////////////
    Texture2DPtr RenderSystemVulkan::createTexture2D()
    {
        return Texture2DVulkan::Create(this);
    }

    //////////////////////////////////////////
    Texture2DMSPtr RenderSystemVulkan::createTexture2DMS()
    {
        return Texture2DMSVulkan::Create(this);
    }

    //////////////////////////////////////////
    TextureCubePtr RenderSystemVulkan::createTextureCube()
    {
        return TextureCubeVulkan::Create(this);
    }

    //////////////////////////////////////////
    MaterialPtr RenderSystemVulkan::createMaterial()
    {
        return MaterialVulkan::Create(this);
    }

    //////////////////////////////////////////
    RenderPassPtr RenderSystemVulkan::createRenderPass(RenderPassType _passType)
    {
        return RenderPassVulkan::Create(this, _passType);
    }

    //////////////////////////////////////////
    GPUVertexBufferPtr RenderSystemVulkan::createGPUVertexBuffer(
        VertexDataDescription const& _vertexDataDescription,
        Size _vertexCount,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        // Not required by the Vulkan backend: instance streams work via SSBOs
        MAZE_ERROR("GPUVertexBuffer is not supported by Vulkan render system!");
        return nullptr;
    }

    //////////////////////////////////////////
    GPUTextureBufferPtr RenderSystemVulkan::createGPUTextureBuffer(
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        // Not required by the Vulkan backend: instance streams work via SSBOs
        MAZE_ERROR("GPUTextureBuffer is not supported by Vulkan render system!");
        return nullptr;
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderSystemVulkan::createRenderBuffer(
        RenderBuffer::RenderBufferDeleter const& _deleter,
        RenderTarget* _renderTarget)
    {
        return RenderBufferVulkan::Create(this, _deleter);
    }

    //////////////////////////////////////////
    VkSampler RenderSystemVulkan::ensureSampler(
        TextureFilter _minFilter,
        TextureFilter _magFilter,
        TextureWrap _wrapS,
        TextureWrap _wrapT,
        TextureWrap _wrapR,
        F32 _anisotropyLevel,
        F32 _maxLod,
        Vec4F const& _borderColor)
    {
        F32 anisotropy = Math::Clamp(_anisotropyLevel, 0.0f, m_physicalDeviceProperties.limits.maxSamplerAnisotropy);

        U64 key =
            ((U64)_minFilter & 0x7) |
            (((U64)_magFilter & 0x7) << 3) |
            (((U64)_wrapS & 0x3) << 6) |
            (((U64)_wrapT & 0x3) << 8) |
            (((U64)_wrapR & 0x3) << 10) |
            (((U64)(U32)anisotropy & 0xFF) << 12) |
            (((U64)(_maxLod > 0.0f ? 1u : 0u)) << 20) |
            ((U64)((U32)(_borderColor.x * 255.0f) & 0xFF) << 24) |
            ((U64)((U32)(_borderColor.y * 255.0f) & 0xFF) << 32) |
            ((U64)((U32)(_borderColor.z * 255.0f) & 0xFF) << 40) |
            ((U64)((U32)(_borderColor.w * 255.0f) & 0xFF) << 48);

        auto it = m_samplers.find(key);
        if (it != m_samplers.end())
            return it->second;

        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = GetTextureFilterVulkan(_magFilter);
        samplerInfo.minFilter = GetTextureFilterVulkan(_minFilter);
        samplerInfo.mipmapMode = GetTextureMipmapModeVulkan(_minFilter);
        samplerInfo.addressModeU = GetTextureWrapVulkan(_wrapS);
        samplerInfo.addressModeV = GetTextureWrapVulkan(_wrapT);
        samplerInfo.addressModeW = GetTextureWrapVulkan(_wrapR);
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.anisotropyEnable = anisotropy > 1.0f ? VK_TRUE : VK_FALSE;
        samplerInfo.maxAnisotropy = anisotropy > 1.0f ? anisotropy : 1.0f;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
        samplerInfo.minLod = 0.0f;
        // GL non-mipmap min filters (Nearest/Linear) sample the base level only - clamp LOD to match
        samplerInfo.maxLod = _maxLod > 0.0f ? _maxLod : VK_LOD_CLAMP_NONE;
        samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        VkSampler sampler = VK_NULL_HANDLE;
        MAZE_VK_CALL(vkCreateSampler(m_device, &samplerInfo, nullptr, &sampler));
        MAZE_ERROR_RETURN_VALUE_IF(sampler == VK_NULL_HANDLE, VK_NULL_HANDLE, "vkCreateSampler failed!");

        m_samplers.emplace(key, sampler);
        return sampler;
    }

    //////////////////////////////////////////
    VkCommandBuffer RenderSystemVulkan::getCurrentCommandBuffer() const
    {
        if (m_frameResources.empty())
            return VK_NULL_HANDLE;
        return m_frameResources[m_currentFrameIndex].commandBuffer;
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::beginFrame()
    {
        VulkanFrameResources& frame = m_frameResources[m_currentFrameIndex];

        MAZE_VK_CALL(vkWaitForFences(m_device, 1u, &frame.inFlightFence, VK_TRUE, UINT64_MAX));
        MAZE_VK_CALL(vkResetFences(m_device, 1u, &frame.inFlightFence));

        MAZE_VK_CALL(vkResetCommandPool(m_device, frame.commandPool, 0u));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        MAZE_VK_CALL(vkBeginCommandBuffer(frame.commandBuffer, &beginInfo));

        flushGlobalUniforms();
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::endFrame()
    {
        VulkanFrameResources& frame = m_frameResources[m_currentFrameIndex];

        if (m_stateMachine)
            m_stateMachine->unbindRenderTarget();

        MAZE_VK_CALL(vkEndCommandBuffer(frame.commandBuffer));

        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1u;
        submitInfo.pCommandBuffers = &frame.commandBuffer;
        // Per-window image-available/render-finished semaphores are chained
        // in by RenderWindowVulkan::swapBuffers, which calls vkQueueSubmit
        // itself for the present-wait dependency instead of here, since a
        // frame may touch zero, one, or several swapchains - see
        // RenderWindowVulkan.cpp
        MAZE_VK_CALL(vkQueueSubmit(m_graphicsQueue, 1u, &submitInfo, frame.inFlightFence));

        m_currentFrameIndex = (m_currentFrameIndex + 1u) % m_config.framesInFlight;
    }

    //////////////////////////////////////////
    VkCommandBuffer RenderSystemVulkan::beginSingleTimeCommands()
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = m_transientCommandPool;
        allocInfo.commandBufferCount = 1u;

        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        MAZE_VK_CALL(vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        MAZE_VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        return commandBuffer;
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::endSingleTimeCommands(VkCommandBuffer _commandBuffer)
    {
        MAZE_VK_CALL(vkEndCommandBuffer(_commandBuffer));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1u;
        submitInfo.pCommandBuffers = &_commandBuffer;

        // Simple synchronous wait - these are one-off setup/upload commands,
        // not per-frame hot path, so a full queue-idle wait is an acceptable
        // (if not maximally efficient) way to guarantee the staging buffer
        // stays alive until the copy completes without extra fence bookkeeping
        MAZE_VK_CALL(vkQueueSubmit(m_graphicsQueue, 1u, &submitInfo, VK_NULL_HANDLE));
        MAZE_VK_CALL(vkQueueWaitIdle(m_graphicsQueue));

        vkFreeCommandBuffers(m_device, m_transientCommandPool, 1u, &_commandBuffer);
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::ensureGlobalUniformBufferSize(U32 _size)
    {
        if (_size <= m_globalUniformBufferSize)
            return;

        U32 newSize = (_size + 255u) & ~255u;

        // Recreate the buffers at the new size, preserving already-written
        // shadow bytes (this is expected to only ever grow once, the first
        // time each built-in shader's GlobalUniforms block is reflected,
        // since every shader #includes the same shared header)
        m_globalUniformShadow.resize(newSize, 0u);

        for (U32 i = 0; i < m_globalUniformBuffers.size(); ++i)
        {
            if (m_globalUniformMapped[i])
                vmaUnmapMemory(m_allocator, m_globalUniformAllocations[i]);
            vmaDestroyBuffer(m_allocator, m_globalUniformBuffers[i], m_globalUniformAllocations[i]);

            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = newSize;
            bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo = {};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

            VmaAllocationInfo allocResultInfo = {};
            MAZE_VK_CALL(vmaCreateBuffer(
                m_allocator, &bufferInfo, &allocInfo,
                &m_globalUniformBuffers[i], &m_globalUniformAllocations[i], &allocResultInfo));
            m_globalUniformMapped[i] = allocResultInfo.pMappedData;

            VkDescriptorBufferInfo bufInfo = {};
            bufInfo.buffer = m_globalUniformBuffers[i];
            bufInfo.offset = 0u;
            bufInfo.range = newSize;

            VkWriteDescriptorSet write = {};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = m_globalDescriptorSets[i];
            write.dstBinding = 0u;
            write.descriptorCount = 1u;
            write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            write.pBufferInfo = &bufInfo;
            vkUpdateDescriptorSets(m_device, 1u, &write, 0u, nullptr);
        }

        m_globalUniformBufferSize = newSize;

        for (Size i = 0; i < MAZE_ARRAY_SIZE(m_globalUniformDirty); ++i)
            m_globalUniformDirty[i] = true;
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::writeGlobalUniformData(U32 _offset, U8 const* _bytes, U32 _bytesCount)
    {
        if (_offset + _bytesCount > (U32)m_globalUniformShadow.size())
        {
            if ((U32)m_globalUniformShadow.size() <= _offset)
                return;
            _bytesCount = (U32)m_globalUniformShadow.size() - _offset;
        }

        if (_bytesCount == 0u)
            return;

        if (memcmp(&m_globalUniformShadow[_offset], _bytes, _bytesCount) == 0)
            return;

        memcpy(&m_globalUniformShadow[_offset], _bytes, _bytesCount);
        for (Size i = 0; i < MAZE_ARRAY_SIZE(m_globalUniformDirty); ++i)
            m_globalUniformDirty[i] = true;
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::flushGlobalUniforms()
    {
        U32 frameIndex = m_currentFrameIndex;
        U32 dirtyIndex = Math::Min(frameIndex, (U32)MAZE_ARRAY_SIZE(m_globalUniformDirty) - 1u);

        if (frameIndex >= m_globalUniformMapped.size() || !m_globalUniformDirty[dirtyIndex])
            return;

        if (m_globalUniformMapped[frameIndex] && !m_globalUniformShadow.empty())
            memcpy(m_globalUniformMapped[frameIndex], m_globalUniformShadow.data(), m_globalUniformShadow.size());

        m_globalUniformDirty[dirtyIndex] = false;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::resolveDepthMSAA(
        Texture2DVulkan* _dstTexture,
        Texture2DMSVulkan* _srcTexture)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!_dstTexture || !_srcTexture, false, "Depth resolve textures are null!");

        VkCommandBuffer cmd = getCurrentCommandBuffer();
        MAZE_ERROR_RETURN_VALUE_IF(cmd == VK_NULL_HANDLE, false, "No active command buffer!");

        Vec2U size = _dstTexture->getSizeU();

        // Vulkan (VK_KHR_depth_stencil_resolve, core 1.2+) supports resolving
        // a depth-aspect multisampled image directly, unlike DX11's
        // ResolveSubresource which doesn't support depth formats
        VkImageResolve region = {};
        region.srcSubresource = { VK_IMAGE_ASPECT_DEPTH_BIT, 0u, 0u, 1u };
        region.dstSubresource = { VK_IMAGE_ASPECT_DEPTH_BIT, 0u, 0u, 1u };
        region.extent = { size.x, size.y, 1u };

        vkCmdResolveImage(
            cmd,
            _srcTexture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            _dstTexture->getImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            1u, &region);

        if (m_stateMachine)
            m_stateMachine->invalidateDeviceState();

        return true;
    }


} // namespace Maze
//////////////////////////////////////////
