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
#include "maze-render-system-vulkan/MazeVertexArrayObjectVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class VertexArrayObjectVulkan
    //
    //////////////////////////////////////////
    VertexArrayObjectVulkan::VertexArrayObjectVulkan()
    {
    }

    //////////////////////////////////////////
    VertexArrayObjectVulkan::~VertexArrayObjectVulkan()
    {
        destroyIndexBuffer();
    }

    //////////////////////////////////////////
    VertexArrayObjectVulkanPtr VertexArrayObjectVulkan::Create(RenderSystemVulkan* _renderSystem)
    {
        VertexArrayObjectVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(VertexArrayObjectVulkan, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool VertexArrayObjectVulkan::init(RenderSystemVulkan* _renderSystem)
    {
        if (!VertexArrayObject::init((RenderSystem*)_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemVulkan* VertexArrayObjectVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystem->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    void VertexArrayObjectVulkan::destroyIndexBuffer()
    {
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        if (!renderSystem || m_indexBuffer == VK_NULL_HANDLE)
        {
            m_indexBuffer = VK_NULL_HANDLE;
            m_indexBufferAllocation = VK_NULL_HANDLE;
            m_indexBufferSizeBytes = 0u;
            return;
        }

        // See Texture2DVulkan::destroyTexture / RenderSystemVulkan::waitDeviceIdleSafe()
        // for why this must not be a bare vkDeviceWaitIdle() - same caveat applies.
        renderSystem->waitDeviceIdleSafe();

        vmaDestroyBuffer(renderSystem->getAllocator(), m_indexBuffer, m_indexBufferAllocation);
        m_indexBuffer = VK_NULL_HANDLE;
        m_indexBufferAllocation = VK_NULL_HANDLE;
        m_indexBufferSizeBytes = 0u;
    }

    //////////////////////////////////////////
    void VertexArrayObjectVulkan::setIndices(
        U8 const* _indicesData,
        VertexAttributeType _indicesType,
        Size _indicesCount)
    {
        MAZE_ERROR_RETURN_IF(
            _indicesType != VertexAttributeType::U16 && _indicesType != VertexAttributeType::U32,
            "Unsupported indices type: %d!",
            (S32)_indicesType);

        m_indicesType = _indicesType;
        m_indicesCount = _indicesCount;

        Size bytes = GetVertexAttributeTypeSize(_indicesType) * _indicesCount;
        if (bytes == 0u)
        {
            destroyIndexBuffer();
            return;
        }

        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();
        VmaAllocator allocator = renderSystem->getAllocator();

        if (m_indexBuffer == VK_NULL_HANDLE || m_indexBufferSizeBytes < bytes)
        {
            destroyIndexBuffer();

            VkBufferCreateInfo bufferInfo;
            memset(&bufferInfo, 0, sizeof(bufferInfo));
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = bytes;
            bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo;
            memset(&allocInfo, 0, sizeof(allocInfo));
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;

            MAZE_VK_CALL(vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &m_indexBuffer, &m_indexBufferAllocation, nullptr));
            MAZE_ERROR_RETURN_IF(m_indexBuffer == VK_NULL_HANDLE, "Index buffer creation failed!");

            m_indexBufferSizeBytes = bytes;
        }

        // Index data is uploaded via a one-off staging buffer + copy - index buffers are
        // device-local for fastest possible GPU reads during indexed draws, same reasoning
        // as VertexBufferObjectVulkan's static/single-mapping path
        VkBufferCreateInfo stagingBufferInfo;
        memset(&stagingBufferInfo, 0, sizeof(stagingBufferInfo));
        stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        stagingBufferInfo.size = bytes;
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
        memcpy(stagingAllocationInfo.pMappedData, _indicesData, bytes);
        MAZE_VK_CALL(vmaFlushAllocation(allocator, stagingAllocation, 0, bytes));

        VkCommandBuffer commandBuffer = renderSystem->beginSingleTimeCommands();

        VkBufferCopy copyRegion;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = bytes;
        vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_indexBuffer, 1, &copyRegion);

        renderSystem->endSingleTimeCommands(commandBuffer);

        vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);
    }

    //////////////////////////////////////////
    void VertexArrayObjectVulkan::setVerticesData(
        U8 const* _data,
        VertexAttributeDescription _description,
        Size _verticesCount)
    {
        VertexArrayObjectVulkanAttributeData& attribute = m_attributes[(Size)_description.semantic];

        if (!attribute.vbo)
        {
            // Mesh vertex streams are typically re-uploaded whenever the mesh changes
            // (skinning, procedural geometry, editor live-edit) - DynamicDefault gives a
            // host-visible persistently mapped VBO so re-uploads are plain memcpys
            attribute.vbo = VertexBufferObjectVulkan::Create(
                getRenderSystemVulkanRaw(),
                GPUByteBufferAccessType::DynamicDefault,
                false);
            MAZE_ERROR_RETURN_IF(!attribute.vbo, "VBO cannot be created!");
        }

        attribute.used = true;
        attribute.description = _description;
        attribute.verticesCount = _verticesCount;

        Size bytesPerType = GetVertexAttributeTypeSize(_description.type);
        Size bytesPerVertexData = bytesPerType * _description.count;
        Size requiredBytes = bytesPerVertexData * _verticesCount;

        attribute.vbo->upload(_data, requiredBytes);

        updateVertexInputState();
    }

    //////////////////////////////////////////
    void VertexArrayObjectVulkan::updateVertexInputState()
    {
        m_bindingDescriptions.clear();
        m_attributeDescriptions.clear();

        // FNV-1a-style base, folded through CombineHashVulkan per populated attribute -
        // matches VertexArrayObjectDX11::updateLayoutHash in spirit (same fields hashed),
        // widened to U64 since PipelineKeyVulkan::vertexLayoutHash is a U64
        U64 hash = 14695981039346656037ULL;

        for (Size i = 0; i < (Size)VertexAttributeSemantic::MAX; ++i)
        {
            VertexArrayObjectVulkanAttributeData const& attribute = m_attributes[i];
            if (!attribute.used)
                continue;

            VkVertexInputBindingDescription bindingDesc;
            bindingDesc.binding = (U32)i;
            bindingDesc.stride = (U32)attribute.description.stride;
            bindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            m_bindingDescriptions.push_back(bindingDesc);

            VkFormat format = GetVertexAttributeFormatVulkan(
                attribute.description.type,
                attribute.description.count,
                attribute.description.normalized);

            VkVertexInputAttributeDescription attributeDesc;
            attributeDesc.location = (U32)i;
            attributeDesc.binding = (U32)i;
            attributeDesc.format = format;
            attributeDesc.offset = (U32)attribute.description.offset;
            m_attributeDescriptions.push_back(attributeDesc);

            hash = CombineHashVulkan(hash, (U64)i + 1u);
            hash = CombineHashVulkan(hash, (U64)attribute.description.type);
            hash = CombineHashVulkan(hash, (U64)attribute.description.count);
            hash = CombineHashVulkan(hash, attribute.description.normalized ? 1ull : 0ull);
            hash = CombineHashVulkan(hash, (U64)attribute.description.stride);
            hash = CombineHashVulkan(hash, (U64)attribute.description.offset);
        }

        m_layoutHash = hash;
    }

    //////////////////////////////////////////
    void VertexArrayObjectVulkan::bind(VkCommandBuffer _commandBuffer)
    {
        RenderSystemVulkan* renderSystem = getRenderSystemVulkanRaw();

        for (Size i = 0; i < (Size)VertexAttributeSemantic::MAX; ++i)
        {
            VertexArrayObjectVulkanAttributeData const& attribute = m_attributes[i];
            if (!attribute.used || !attribute.vbo)
                continue;

            VkBuffer buffer = attribute.vbo->getBufferVulkan();
            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(_commandBuffer, (U32)i, 1, &buffer, &offset);
        }

        // Shared zero-stride buffer feeds attributes the VAO doesn't provide but a shader
        // input layout expects (matches DX11's c_zeroBufferSlot / getZeroVertexBuffer())
        {
            VkBuffer zeroBuffer = renderSystem->getZeroVertexBuffer();
            VkDeviceSize offset = 0;
            vkCmdBindVertexBuffers(_commandBuffer, (U32)c_zeroBufferSlot, 1, &zeroBuffer, &offset);
        }

        if (m_indexBuffer != VK_NULL_HANDLE)
            vkCmdBindIndexBuffer(_commandBuffer, m_indexBuffer, 0, getIndexTypeVulkan());
    }

    //////////////////////////////////////////
    SubMeshPtr VertexArrayObjectVulkan::readAsSubMesh() const
    {
        MAZE_NOT_IMPLEMENTED;
        return nullptr;
    }

#if MAZE_DEBUG
    //////////////////////////////////////////
    void VertexArrayObjectVulkan::debug()
    {
    }
#endif


} // namespace Maze
//////////////////////////////////////////
