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
#include "maze-render-system-vulkan/MazeVertexBufferObjectVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VertexBufferObjectVulkan
    //
    //////////////////////////////////////////
    VertexBufferObjectVulkan::VertexBufferObjectVulkan()
    {
    }

    //////////////////////////////////////////
    VertexBufferObjectVulkan::~VertexBufferObjectVulkan()
    {
        destroyBuffer();
    }

    //////////////////////////////////////////
    VertexBufferObjectVulkanPtr VertexBufferObjectVulkan::Create(
        RenderSystemVulkan* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping)
    {
        VertexBufferObjectVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(VertexBufferObjectVulkan, object, init(_renderSystem, _accessType, _singleMapping));
        return object;
    }

    //////////////////////////////////////////
    bool VertexBufferObjectVulkan::init(
        RenderSystemVulkan* _renderSystem,
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping)
    {
        if (!VertexBufferObject::init((RenderSystem*)_renderSystem, _accessType, _singleMapping))
            return false;

        m_singleMapping = _singleMapping;

        // Write-once data always goes device-local (fastest possible GPU reads, upload
        // cost paid exactly once via a staging buffer). Otherwise, any of the "Dynamic*"
        // access types are assumed to be re-uploaded regularly (e.g. streamed mesh data)
        // and get a host-visible, persistently mapped allocation so upload() is a plain
        // memcpy with no staging buffer / command buffer / GPU sync at all. Immutable/
        // Default/None fall through to device-local too, since those access types are
        // documented as CPU-inaccessible.
        m_hostVisible = !m_singleMapping &&
            (_accessType == GPUByteBufferAccessType::DynamicDefault ||
             _accessType == GPUByteBufferAccessType::DynamicPersistent ||
             _accessType == GPUByteBufferAccessType::DynamicPersistentCoherent);

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* VertexBufferObjectVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystem->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    void VertexBufferObjectVulkan::destroyBuffer()
    {
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        if (!renderSystem || m_buffer == VK_NULL_HANDLE)
        {
            m_buffer = VK_NULL_HANDLE;
            m_allocation = VK_NULL_HANDLE;
            m_mappedData = nullptr;
            m_sizeBytes = 0u;
            return;
        }

        // See Texture2DVulkan::destroyTexture / RenderSystemVulkan::waitDeviceIdleSafe()
        // for why this must not be a bare vkDeviceWaitIdle() - same caveat applies
        // (a mesh's VBO can still be referenced by an in-flight OR still-recording command
        // buffer's vkCmdBindVertexBuffers/draw call when it's resized or destroyed).
        renderSystem->waitDeviceIdleSafe();

        vmaDestroyBuffer(renderSystem->getAllocator(), m_buffer, m_allocation);
        m_buffer = VK_NULL_HANDLE;
        m_allocation = VK_NULL_HANDLE;
        m_mappedData = nullptr;
        m_sizeBytes = 0u;
    }

    //////////////////////////////////////////
    bool VertexBufferObjectVulkan::ensureBuffer(Size _bytes)
    {
        if (m_buffer != VK_NULL_HANDLE && m_sizeBytes >= _bytes)
            return true;

        destroyBuffer();

        if (_bytes == 0u)
            return true;

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        VmaAllocator allocator = renderSystem->getAllocator();

        VkBufferCreateInfo bufferInfo;
        memset(&bufferInfo, 0, sizeof(bufferInfo));
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = _bytes;
        bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VmaAllocationCreateInfo allocInfo;
        memset(&allocInfo, 0, sizeof(allocInfo));
        if (m_hostVisible)
        {
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
        }
        else
        {
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        }

        VmaAllocationInfo allocationInfo;
        MAZE_VK_CALL(vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &m_buffer, &m_allocation, &allocationInfo));
        MAZE_ERROR_RETURN_VALUE_IF(m_buffer == VK_NULL_HANDLE, false, "Vertex buffer creation failed!");

        m_mappedData = m_hostVisible ? allocationInfo.pMappedData : nullptr;
        m_sizeBytes = _bytes;
        return true;
    }

    //////////////////////////////////////////
    void VertexBufferObjectVulkan::resize(Size _bytes)
    {
        ensureBuffer(_bytes);
    }

    //////////////////////////////////////////
    void VertexBufferObjectVulkan::upload(
        void const* _data,
        Size _bytes)
    {
        if (!ensureBuffer(_bytes))
            return;

        if (m_buffer == VK_NULL_HANDLE || _bytes == 0u)
            return;

        if (m_hostVisible)
        {
            // Persistently mapped - no map/unmap call needed, just write straight through.
            // HOST_ACCESS_SEQUENTIAL_WRITE_BIT means this memory type may not be host
            // coherent, so flush the written range explicitly (a no-op on coherent memory).
            memcpy(m_mappedData, _data, _bytes);

            RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
            MAZE_VK_CALL(vmaFlushAllocation(renderSystem->getAllocator(), m_allocation, 0, _bytes));
            return;
        }

        // Static/device-local path - one-off staging buffer + synchronous copy
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        VmaAllocator allocator = renderSystem->getAllocator();

        VkBufferCreateInfo stagingBufferInfo;
        memset(&stagingBufferInfo, 0, sizeof(stagingBufferInfo));
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.size = _bytes;
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
        memcpy(stagingAllocationInfo.pMappedData, _data, _bytes);
        MAZE_VK_CALL(vmaFlushAllocation(allocator, stagingAllocation, 0, _bytes));

        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        VkBufferCopy copyRegion;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = _bytes;
        vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_buffer, 1, &copyRegion);

        renderSystem->endSingleTimeCommands(commandBuffer);

        vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);
    }


} // namespace Maze
//////////////////////////////////////////
