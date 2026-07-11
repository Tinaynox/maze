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
#if (!defined(_MazeVertexBufferObjectVulkan_hpp_))
#define _MazeVertexBufferObjectVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeVertexBufferObject.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(VertexBufferObjectVulkan);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    // Class VertexBufferObjectVulkan
    //
    // Two storage strategies, chosen from the access type / single-mapping hint the
    // buffer was created with (mirrors DX11's D3D11_USAGE_DYNAMIC-vs-DEFAULT split,
    // adapted to what actually matters for a VMA-backed Vulkan buffer):
    //  - Dynamic/streamed data (GPUByteBufferAccessType::DynamicDefault/DynamicPersistent/
    //    DynamicPersistentCoherent, e.g. VertexArrayObjectVulkan's per-attribute VBOs,
    //    which are re-uploaded whenever mesh data changes) - a host-visible, persistently
    //    mapped VMA allocation (VMA_ALLOCATION_CREATE_MAPPED_BIT +
    //    VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT) that upload() just
    //    memcpy's into directly, no command buffer involved.
    //  - Static/single-mapping data (GPUByteBufferAccessType::Immutable/Default, or
    //    _singleMapping == true, i.e. data uploaded once and never touched again) - a
    //    device-local VMA allocation, written via a one-off staging buffer + copy
    //    (RenderSystemVulkan::beginSingleTimeCommands/endSingleTimeCommands), trading
    //    upload-time cost for the fastest possible GPU read access.
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API VertexBufferObjectVulkan
        : public VertexBufferObject
    {
    public:

        //////////////////////////////////////////
        virtual ~VertexBufferObjectVulkan();

        //////////////////////////////////////////
        static VertexBufferObjectVulkanPtr Create(
            RenderSystemVulkan* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false);


        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;

        //////////////////////////////////////////
        inline VkBuffer getBufferVulkan() const { return m_buffer; }

        //////////////////////////////////////////
        inline Size getSizeBytes() const { return m_sizeBytes; }

        //////////////////////////////////////////
        inline bool isHostVisible() const { return m_hostVisible; }


        //////////////////////////////////////////
        virtual void resize(Size _bytes) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(
            void const* _data,
            Size _bytes) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        VertexBufferObjectVulkan();

        //////////////////////////////////////////
        using VertexBufferObject::init;

        //////////////////////////////////////////
        bool init(
            RenderSystemVulkan* _renderSystem,
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping);

        //////////////////////////////////////////
        bool ensureBuffer(Size _bytes);

        //////////////////////////////////////////
        void destroyBuffer();

    protected:
        VkBuffer m_buffer = VK_NULL_HANDLE;
        VmaAllocation m_allocation = VK_NULL_HANDLE;
        Size m_sizeBytes = 0u;

        // Persistently mapped pointer, valid only when m_hostVisible is true
        void* m_mappedData = nullptr;

        bool m_singleMapping = false;
        bool m_hostVisible = false;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeVertexBufferObjectVulkan_hpp_
//////////////////////////////////////////
