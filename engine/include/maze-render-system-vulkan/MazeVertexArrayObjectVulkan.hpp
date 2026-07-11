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
#if (!defined(_MazeVertexArrayObjectVulkan_hpp_))
#define _MazeVertexArrayObjectVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeVertexBufferObjectVulkan.hpp"
#include "maze-graphics/MazeVertexArrayObject.hpp"
#include "maze-graphics/MazeVertex.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexArrayObjectVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Struct VertexArrayObjectVulkanAttributeData
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_VULKAN_API VertexArrayObjectVulkanAttributeData
    {
        bool used = false;
        VertexAttributeDescription description;
        VertexBufferObjectVulkanPtr vbo;
        Size verticesCount = 0u;
    };


    //////////////////////////////////////////
    // Class VertexArrayObjectVulkan
    //
    // Vulkan has no VAO object - unlike DX11 (which owns an ID3D11InputLayout cache
    // keyed by shader+layout), this class is pure metadata: which VkBuffers are bound
    // to which semantic, and the VkVertexInputBindingDescription/AttributeDescription
    // arrays a VkPipeline needs at creation time (baked into
    // VkPipelineVertexInputStateCreateInfo by StateMachineVulkan::ensurePipeline(),
    // keyed by PipelineKeyVulkan::vertexLayoutHash - see getVertexLayoutHash()).
    //
    // Binding index convention: binding N == VkVertexInputBindingDescription for
    // VertexAttributeSemantic N (one binding/VBO per semantic, exactly mirroring DX11's
    // "one IA slot per semantic" scheme), and the corresponding shader input is expected
    // at layout(location = N). Semantics not populated by this VAO but referenced by a
    // shader fall back to the shared zero-stride buffer at binding c_zeroBufferSlot
    // (RenderSystemVulkan::getZeroVertexBuffer()), mirroring DX11's c_zeroBufferSlot.
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API VertexArrayObjectVulkan
        : public VertexArrayObject
    {
    public:

        //////////////////////////////////////////
        // Binding slot used to feed missing attributes with zeroes (stride 0)
        static Size const c_zeroBufferSlot = (Size)VertexAttributeSemantic::MAX;

    public:

        //////////////////////////////////////////
        virtual ~VertexArrayObjectVulkan();

        //////////////////////////////////////////
        static VertexArrayObjectVulkanPtr Create(RenderSystemVulkan* _renderSystem);


        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;


        //////////////////////////////////////////
        virtual void setIndices(
            U8 const* _indicesData,
            VertexAttributeType _indicesType,
            Size _indicesCount) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void setVerticesData(
            U8 const* _data,
            VertexAttributeDescription _description,
            Size _verticesCount) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual SubMeshPtr readAsSubMesh() const MAZE_OVERRIDE;

#if MAZE_DEBUG
        //////////////////////////////////////////
        virtual void debug() MAZE_OVERRIDE;
#endif


        //////////////////////////////////////////
        inline VertexArrayObjectVulkanAttributeData const& getAttribute(VertexAttributeSemantic _semantic) const
        {
            return m_attributes[(Size)_semantic];
        }

        //////////////////////////////////////////
        // Stable hash of the currently-populated attribute layout (semantic/type/count/
        // normalized for every used attribute) - used as PipelineKeyVulkan::vertexLayoutHash
        // so a distinct VkPipeline is only baked when the actual vertex input state differs
        inline U64 getVertexLayoutHash() const { return m_layoutHash; }

        //////////////////////////////////////////
        // VkVertexInputBindingDescription/AttributeDescription arrays for every currently
        // populated attribute (binding == location == semantic index, see class comment)
        Vector<VkVertexInputBindingDescription> const& getVertexInputBindingDescriptions() const { return m_bindingDescriptions; }

        //////////////////////////////////////////
        Vector<VkVertexInputAttributeDescription> const& getVertexInputAttributeDescriptions() const { return m_attributeDescriptions; }

        //////////////////////////////////////////
        inline VkBuffer getIndexBuffer() const { return m_indexBuffer; }

        //////////////////////////////////////////
        inline VkIndexType getIndexTypeVulkan() const { return (m_indicesType == VertexAttributeType::U32) ? VK_INDEX_TYPE_UINT32 : VK_INDEX_TYPE_UINT16; }

        //////////////////////////////////////////
        // Binds every populated attribute's VBO (at binding == semantic index), the
        // shared zero buffer (at c_zeroBufferSlot) and the index buffer (if any) into
        // _commandBuffer. Does not bind a pipeline or set topology/dynamic state - that's
        // StateMachineVulkan::flushPipeline()'s job.
        void bind(VkCommandBuffer _commandBuffer);

    protected:

        //////////////////////////////////////////
        VertexArrayObjectVulkan();

        //////////////////////////////////////////
        using VertexArrayObject::init;

        //////////////////////////////////////////
        bool init(RenderSystemVulkan* _renderSystem);

        //////////////////////////////////////////
        void updateVertexInputState();

        //////////////////////////////////////////
        void destroyIndexBuffer();

    protected:
        VertexArrayObjectVulkanAttributeData m_attributes[(Size)VertexAttributeSemantic::MAX];

        Vector<VkVertexInputBindingDescription> m_bindingDescriptions;
        Vector<VkVertexInputAttributeDescription> m_attributeDescriptions;
        U64 m_layoutHash = 0u;

        VkBuffer m_indexBuffer = VK_NULL_HANDLE;
        VmaAllocation m_indexBufferAllocation = VK_NULL_HANDLE;
        Size m_indexBufferSizeBytes = 0u;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexArrayObjectVulkan_hpp_
//////////////////////////////////////////
