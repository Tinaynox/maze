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
// VMA is a single-header library: declarations are pulled in (guarded by
// AMD_VULKAN_MEMORY_ALLOCATOR_H) via MazeRenderSystemVulkanHeader.hpp above,
// same as every other translation unit in this module. The IMPLEMENTATION
// section of vk_mem_alloc.h sits OUTSIDE that guard (gated only by
// '#ifdef VMA_IMPLEMENTATION'), so re-including the header here with
// VMA_IMPLEMENTATION defined compiles the function bodies into exactly this
// one translation unit, as the library's own docs require.
//
// This must happen here, AFTER MazeRenderSystemVulkanHeader.hpp is included
// above - this whole file compiles with a precompiled header (/Yu, see
// maze_add_module's FORWARD_HEADER wiring in Macros.cmake) built from that
// exact header with VMA_IMPLEMENTATION undefined, so a #define placed BEFORE
// this file's own PCH-boundary include line has no effect (MSVC treats
// everything up to and including that line as identical to the PCH and
// substitutes the precompiled state verbatim, without re-lexing it) - it has
// to be a second, later include instead.
#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>


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

        for (Vector<GlobalUniformInstance>& framePool : m_globalInstancePool)
            for (GlobalUniformInstance& inst : framePool)
                if (inst.buffer != VK_NULL_HANDLE)
                    vmaDestroyBuffer(m_allocator, inst.buffer, inst.allocation);
        m_globalInstancePool.clear();

        for (Size i = 0; i < m_modelMatricesStreamBuffers.size(); ++i)
            if (m_modelMatricesStreamBuffers[i] != VK_NULL_HANDLE)
                vmaDestroyBuffer(m_allocator, m_modelMatricesStreamBuffers[i], m_modelMatricesStreamAllocations[i]);
        for (Size i = 0; i < m_colorsStreamBuffers.size(); ++i)
            if (m_colorsStreamBuffers[i] != VK_NULL_HANDLE)
                vmaDestroyBuffer(m_allocator, m_colorsStreamBuffers[i], m_colorsStreamAllocations[i]);
        for (Size i = 0; i < m_uvStreamBuffers.size(); ++i)
            if (m_uvStreamBuffers[i] != VK_NULL_HANDLE)
                vmaDestroyBuffer(m_allocator, m_uvStreamBuffers[i], m_uvStreamAllocations[i]);

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

        // A generous shared descriptor pool. Every ShaderVulkan grows its own
        // pool of set-1 material descriptor set *instances* from here - one
        // per (frame-in-flight, concurrent draw within that frame) rather
        // than one long-lived set per shader (see
        // ShaderVulkan::acquireMaterialDescriptorSet()'s banner comment for
        // why a single shared-per-shader set isn't safe) - so this needs to
        // cover many more sets than a naive per-shader count would suggest,
        // plus the one shared set-0 global set
        VkDescriptorPoolSize poolSizes[3] = {};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = 32768u;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = 65536u;
        poolSizes[2].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSizes[2].descriptorCount = 256u;

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 32768u;
        poolInfo.poolSizeCount = 3u;
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

        // Global uniform buffers/descriptor sets are created lazily, per
        // draw call, by acquireGlobalDescriptorSet() (see its banner
        // comment) - just size the shadow buffer and pool bookkeeping here.
        {
            m_globalInstancePool.resize(m_config.framesInFlight);
            m_globalInstancePoolUsed.resize(m_config.framesInFlight, 0u);
            m_globalInstancePoolGeneration.resize(m_config.framesInFlight, ~0ull);

            m_globalUniformBufferSize = 256u;
            m_globalUniformShadow.resize(m_globalUniformBufferSize, 0u);
        }

        if (!createInstanceStreamBuffers())
            return false;

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
        features13.shaderDemoteToHelperInvocation = VK_TRUE;

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
    static bool CreateInstanceStreamBufferSet(
        RenderSystemVulkan* _rs,
        U32 _byteSize,
        U32 _framesInFlight,
        Vector<VkBuffer>& _buffers,
        Vector<VmaAllocation>& _allocations,
        Vector<void*>& _mapped)
    {
        _buffers.resize(_framesInFlight, VK_NULL_HANDLE);
        _allocations.resize(_framesInFlight, VK_NULL_HANDLE);
        _mapped.resize(_framesInFlight, nullptr);

        for (U32 i = 0; i < _framesInFlight; ++i)
        {
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = _byteSize;
            bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo = {};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

            VmaAllocationInfo allocResultInfo = {};
            MAZE_VK_CALL(vmaCreateBuffer(_rs->getAllocator(), &bufferInfo, &allocInfo, &_buffers[i], &_allocations[i], &allocResultInfo));
            _mapped[i] = allocResultInfo.pMappedData;
        }

        return true;
    }

    //////////////////////////////////////////
    static void WriteGlobalStorageBufferBinding(
        VkDevice _device,
        VkDescriptorSet _set,
        U32 _binding,
        VkBuffer _buffer,
        U32 _size)
    {
        VkDescriptorBufferInfo bufInfo = {};
        bufInfo.buffer = _buffer;
        bufInfo.offset = 0u;
        bufInfo.range = _size;

        VkWriteDescriptorSet write = {};
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = _set;
        write.dstBinding = _binding;
        write.descriptorCount = 1u;
        write.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        write.pBufferInfo = &bufInfo;
        vkUpdateDescriptorSets(_device, 1u, &write, 0u, nullptr);
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::createInstanceStreamBuffers()
    {
        U32 framesInFlight = m_config.framesInFlight;

        m_modelMatricesStreamSize = c_instanceStreamCapacity * (U32)(sizeof(F32) * 16u); // mat4 per instance
        m_colorsStreamSize = c_instanceStreamCapacity * (U32)(sizeof(F32) * 4u); // vec4 per instance
        m_uvStreamSize = c_instanceStreamCapacity * 2u * (U32)(sizeof(F32) * 4u); // 2 channels * vec4 per instance

        if (!CreateInstanceStreamBufferSet(this, m_modelMatricesStreamSize, framesInFlight, m_modelMatricesStreamBuffers, m_modelMatricesStreamAllocations, m_modelMatricesStreamMapped))
            return false;
        if (!CreateInstanceStreamBufferSet(this, m_colorsStreamSize, framesInFlight, m_colorsStreamBuffers, m_colorsStreamAllocations, m_colorsStreamMapped))
            return false;
        if (!CreateInstanceStreamBufferSet(this, m_uvStreamSize, framesInFlight, m_uvStreamBuffers, m_uvStreamAllocations, m_uvStreamMapped))
            return false;

        // Bindings 1-3 are written per pool-slot by acquireGlobalDescriptorSet()
        // now that the global set is per-draw-pooled rather than one shared
        // set per frame-in-flight - these buffers are stable/never recreated,
        // so writing the bindings once per pool slot (at its creation) is
        // sufficient, no need to do it here too.

        return true;
    }

    //////////////////////////////////////////
    void* RenderSystemVulkan::getModelMatricesStreamMapped(U32 _frameIndex) const
    {
        return _frameIndex < m_modelMatricesStreamMapped.size() ? m_modelMatricesStreamMapped[_frameIndex] : nullptr;
    }

    //////////////////////////////////////////
    void* RenderSystemVulkan::getColorsStreamMapped(U32 _frameIndex) const
    {
        return _frameIndex < m_colorsStreamMapped.size() ? m_colorsStreamMapped[_frameIndex] : nullptr;
    }

    //////////////////////////////////////////
    void* RenderSystemVulkan::getUVStreamMapped(U32 _frameIndex) const
    {
        return _frameIndex < m_uvStreamMapped.size() ? m_uvStreamMapped[_frameIndex] : nullptr;
    }

    //////////////////////////////////////////
    S32 RenderSystemVulkan::allocateInstanceStreamSlots(S32 _count)
    {
        if (m_instanceStreamCursor + _count > (S32)c_instanceStreamCapacity)
        {
            MAZE_ERROR("Vulkan instance-stream capacity (%u) exceeded in a single frame!", c_instanceStreamCapacity);
            _count = Math::Max(0, (S32)c_instanceStreamCapacity - m_instanceStreamCursor);
        }

        S32 offset = m_instanceStreamCursor;
        m_instanceStreamCursor += _count;
        return offset;
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
        {
            // Same C++ target object as last time - but that does NOT mean
            // "no work needed". RenderWindowVulkan needs a fresh
            // vkAcquireNextImageKHR once per real presented frame (tracked
            // via its own m_imageAcquired flag, reset in swapBuffers()),
            // even when the window is the only render target and therefore
            // never gets displaced as m_currentRenderTarget between logical
            // frames. Skipping this call entirely here (the original
            // behavior) meant the window's image only ever got acquired
            // ONCE, on the very first frame; every subsequent frame's
            // swapBuffers() then silently no-op'd (m_imageAcquired stayed
            // false forever after), which also meant vkQueuePresentKHR -
            // the call that normally paces the app to VSYNC - stopped being
            // invoked, so the engine's main loop spun unthrottled (500Hz+),
            // continuously re-queuing draws into a Vulkan frame that never
            // got closed, until instance-stream capacity was exhausted.
            // Each RenderTarget subclass is responsible for detecting
            // "already fresh this frame, no-op" vs "need to actually do
            // work" via its own state (m_imageAcquired for windows;
            // RenderBufferVulkan::processRenderTargetWillSet() is already a
            // harmless no-op, safe to call repeatedly). ensureFrameOpen()
            // must run first, exactly like the "different target" path
            // below does - otherwise a window that successfully
            // re-acquires here (m_imageAcquired becomes true) can end up
            // with no open frame to submit into, so endFrame() later
            // no-ops (its own "if (!m_frameOpen) return;" guard) and the
            // render-finished semaphore never gets signaled, while
            // swapBuffers() still unconditionally calls vkQueuePresentKHR
            // to wait on it - producing exactly the "waiting on semaphore
            // that has no way to be signaled" validation errors this was
            // root-caused from.
            if (_renderTarget)
            {
                ensureFrameOpen();

                if (!_renderTarget->processRenderTargetWillSet())
                    return false;

                // Must also run every time, not just on a "different
                // target" switch: the window's C++ object identity stays
                // constant across frames, but processRenderTargetWillSet()
                // just acquired a swapchain image whose INDEX can differ
                // frame to frame (round-robin over 2-3 images). Each actual
                // image needs its own UNDEFINED/PRESENT_SRC_KHR ->
                // COLOR_ATTACHMENT_OPTIMAL transition and
                // stateMachine->bindRenderTarget() call (both live in
                // processRenderTargetSet()) - skipping this here left
                // newly-(re)acquired images never transitioned, producing
                // "expects COLOR_ATTACHMENT_OPTIMAL...instead
                // PRESENT_SRC_KHR/UNDEFINED" validation errors and the
                // alternating correct-frame/black-frame flicker this was
                // root-caused from.
                _renderTarget->processRenderTargetSet();
            }
            return true;
        }

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetWillReset();
            m_currentRenderTarget = nullptr;
        }

        if (_renderTarget)
        {
            ensureFrameOpen();

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
    void RenderSystemVulkan::interruptActiveRenderTarget()
    {
        if (m_stateMachine)
            m_stateMachine->unbindRenderTarget();

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetWillReset();
            m_currentRenderTarget = nullptr;
        }
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
    void RenderSystemVulkan::ensureFrameOpen()
    {
        if (m_frameOpen)
            return;

        ++m_frameGeneration;

        VulkanFrameResources& frame = m_frameResources[m_currentFrameIndex];

        MAZE_VK_CALL(vkWaitForFences(m_device, 1u, &frame.inFlightFence, VK_TRUE, UINT64_MAX));
        MAZE_VK_CALL(vkResetFences(m_device, 1u, &frame.inFlightFence));

        MAZE_VK_CALL(vkResetCommandPool(m_device, frame.commandPool, 0u));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        MAZE_VK_CALL(vkBeginCommandBuffer(frame.commandBuffer, &beginInfo));

        m_frameOpen = true;
        m_instanceStreamCursor = 0;
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::addFrameWaitSemaphore(VkSemaphore _semaphore, VkPipelineStageFlags _waitStage)
    {
        m_frameWaitSemaphores.push_back(_semaphore);
        m_frameWaitStages.push_back(_waitStage);
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::addPendingImageTransition(
        VkImage _image,
        VkImageAspectFlags _aspect,
        VkImageLayout _oldLayout,
        VkImageLayout _newLayout)
    {
        VulkanPendingImageTransition transition;
        transition.image = _image;
        transition.aspect = _aspect;
        transition.oldLayout = _oldLayout;
        transition.newLayout = _newLayout;
        m_pendingImageTransitions.push_back(transition);
    }

    //////////////////////////////////////////
    void RenderSystemVulkan::endFrame(VkSemaphore _signalSemaphore)
    {
        if (!m_frameOpen)
            return;

        VulkanFrameResources& frame = m_frameResources[m_currentFrameIndex];

        if (m_stateMachine)
            m_stateMachine->unbindRenderTarget();

        // Any transitions deferred via addPendingImageTransition() (e.g. a
        // swapchain image's present-layout transition queued from
        // RenderWindowVulkan::endDraw()) are only legal to record now that
        // unbindRenderTarget() has closed the dynamic-rendering scope above
        for (VulkanPendingImageTransition const& transition : m_pendingImageTransitions)
        {
            TransitionImageLayoutVulkan(
                frame.commandBuffer, transition.image, transition.aspect,
                transition.oldLayout, transition.newLayout);
        }
        m_pendingImageTransitions.clear();

        MAZE_VK_CALL(vkEndCommandBuffer(frame.commandBuffer));

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1u;
        submitInfo.pCommandBuffers = &frame.commandBuffer;
        submitInfo.waitSemaphoreCount = (U32)m_frameWaitSemaphores.size();
        submitInfo.pWaitSemaphores = m_frameWaitSemaphores.empty() ? nullptr : m_frameWaitSemaphores.data();
        submitInfo.pWaitDstStageMask = m_frameWaitStages.empty() ? nullptr : m_frameWaitStages.data();
        submitInfo.signalSemaphoreCount = _signalSemaphore != VK_NULL_HANDLE ? 1u : 0u;
        submitInfo.pSignalSemaphores = _signalSemaphore != VK_NULL_HANDLE ? &_signalSemaphore : nullptr;
        MAZE_VK_CALL(vkQueueSubmit(m_graphicsQueue, 1u, &submitInfo, frame.inFlightFence));

        m_frameWaitSemaphores.clear();
        m_frameWaitStages.clear();
        m_frameOpen = false;

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

        // Preserve already-written shadow bytes (this is expected to only
        // ever grow once, the first time each built-in shader's
        // GlobalUniforms block is reflected, since every shader #includes
        // the same shared header - but a shader can be (re)loaded/
        // recompiled at any time, e.g. SystemFontManager::createSystemFont()'s
        // addLocalFeature()+recompile(), so this isn't guaranteed to happen
        // only before the first frame opens).
        //
        // All existing pool instances (across every frame-in-flight slot)
        // were sized for the OLD m_globalUniformBufferSize, so they must be
        // torn down and lazily recreated at the new size by
        // acquireGlobalDescriptorSet() - vkDeviceWaitIdle first guarantees
        // none of them are still in-flight on the GPU when they're
        // destroyed (mirrors ShaderVulkan::unloadVulkanShader()'s identical
        // rationale).
        MAZE_VK_CALL(vkDeviceWaitIdle(m_device));

        for (Vector<GlobalUniformInstance>& framePool : m_globalInstancePool)
        {
            for (GlobalUniformInstance& inst : framePool)
            {
                if (inst.descriptorSet != VK_NULL_HANDLE)
                    vkFreeDescriptorSets(m_device, m_descriptorPool, 1u, &inst.descriptorSet);
                if (inst.buffer != VK_NULL_HANDLE)
                    vmaDestroyBuffer(m_allocator, inst.buffer, inst.allocation);
            }
            framePool.clear();
        }
        for (U32& used : m_globalInstancePoolUsed)
            used = 0u;
        for (U64& generation : m_globalInstancePoolGeneration)
            generation = ~0ull;

        m_globalUniformShadow.resize(newSize, 0u);
        m_globalUniformBufferSize = newSize;
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

        memcpy(&m_globalUniformShadow[_offset], _bytes, _bytesCount);
    }

    //////////////////////////////////////////
    VkDescriptorSet RenderSystemVulkan::acquireGlobalDescriptorSet()
    {
        U32 frameIndex = m_currentFrameIndex;
        if (frameIndex >= m_globalInstancePool.size())
            return VK_NULL_HANDLE;

        // Mirrors ShaderVulkan::acquireMaterialDescriptorSet()'s frame-
        // generation reset exactly - see getFrameGeneration()'s banner comment
        if (m_globalInstancePoolGeneration[frameIndex] != m_frameGeneration)
        {
            m_globalInstancePoolUsed[frameIndex] = 0u;
            m_globalInstancePoolGeneration[frameIndex] = m_frameGeneration;
        }

        Vector<GlobalUniformInstance>& framePool = m_globalInstancePool[frameIndex];
        U32 slot = m_globalInstancePoolUsed[frameIndex]++;

        if (slot >= (U32)framePool.size())
        {
            GlobalUniformInstance inst;

            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = m_globalUniformBufferSize;
            bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo = {};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

            VmaAllocationInfo allocResultInfo = {};
            MAZE_VK_CALL(vmaCreateBuffer(
                m_allocator, &bufferInfo, &allocInfo,
                &inst.buffer, &inst.allocation, &allocResultInfo));
            inst.mapped = allocResultInfo.pMappedData;

            VkDescriptorSetAllocateInfo dsAllocInfo = {};
            dsAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            dsAllocInfo.descriptorPool = m_descriptorPool;
            dsAllocInfo.descriptorSetCount = 1u;
            dsAllocInfo.pSetLayouts = &m_globalSetLayout;
            MAZE_VK_CALL(vkAllocateDescriptorSets(m_device, &dsAllocInfo, &inst.descriptorSet));

            // The UBO's VkBuffer identity never changes after creation (only
            // its contents, via the memcpy below), so this binding-0 write
            // never needs repeating for this instance - mirrors
            // ShaderVulkan::acquireMaterialDescriptorSet()'s identical comment
            VkDescriptorBufferInfo bufInfo = {};
            bufInfo.buffer = inst.buffer;
            bufInfo.offset = 0u;
            bufInfo.range = m_globalUniformBufferSize;

            VkWriteDescriptorSet write = {};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = inst.descriptorSet;
            write.dstBinding = 0u;
            write.descriptorCount = 1u;
            write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            write.pBufferInfo = &bufInfo;
            vkUpdateDescriptorSets(m_device, 1u, &write, 0u, nullptr);

            // Bindings 1-3 (instance-stream SSBOs) are stable per-frame-in-
            // flight buffers (fixed capacity, never recreated after init) -
            // safe to write once here rather than on every acquire
            if (frameIndex < m_modelMatricesStreamBuffers.size())
                WriteGlobalStorageBufferBinding(m_device, inst.descriptorSet, 1u, m_modelMatricesStreamBuffers[frameIndex], m_modelMatricesStreamSize);
            if (frameIndex < m_colorsStreamBuffers.size())
                WriteGlobalStorageBufferBinding(m_device, inst.descriptorSet, 2u, m_colorsStreamBuffers[frameIndex], m_colorsStreamSize);
            if (frameIndex < m_uvStreamBuffers.size())
                WriteGlobalStorageBufferBinding(m_device, inst.descriptorSet, 3u, m_uvStreamBuffers[frameIndex], m_uvStreamSize);

            framePool.push_back(inst);
        }

        GlobalUniformInstance& inst = framePool[slot];

        if (inst.mapped && !m_globalUniformShadow.empty())
            memcpy(inst.mapped, m_globalUniformShadow.data(), m_globalUniformShadow.size());

        return inst.descriptorSet;
    }

    //////////////////////////////////////////
    bool RenderSystemVulkan::resolveDepthMSAA(
        Texture2DVulkan* _dstTexture,
        Texture2DMSVulkan* _srcTexture)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!_dstTexture || !_srcTexture, false, "Depth resolve textures are null!");

        VkCommandBuffer cmd = getCurrentCommandBuffer();
        MAZE_ERROR_RETURN_VALUE_IF(cmd == VK_NULL_HANDLE, false, "No active command buffer!");

        Vec2S const& sizeS = _dstTexture->getSize();
        Vec2U size((U32)sizeS.x, (U32)sizeS.y);

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
