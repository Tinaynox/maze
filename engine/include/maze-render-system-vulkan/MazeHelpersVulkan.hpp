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
#if (!defined(_MazeHelpersVulkan_hpp_))
#define _MazeHelpersVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazeBlendMode.hpp"
#include "maze-graphics/MazeCompareFunction.hpp"
#include "maze-graphics/MazeStencilOperation.hpp"
#include "maze-graphics/MazeCullMode.hpp"
#include "maze-graphics/MazeRenderDrawTopology.hpp"
#include "maze-graphics/MazeTexture.hpp"
#include "maze-graphics/MazeVertex.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkFormat GetPixelFormatVulkan(PixelFormat::Enum _pixelFormat);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API PixelFormat::Enum GetPixelFormatFromVulkan(VkFormat _format);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API bool IsDepthPixelFormatVulkan(PixelFormat::Enum _pixelFormat);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkImageAspectFlags GetImageAspectFlagsVulkan(PixelFormat::Enum _pixelFormat);

    //////////////////////////////////////////
    // Expands tightly packed 24-bit RGB data to 32-bit RGBA (most Vulkan
    // drivers don't support VK_FORMAT_R8G8B8_UNORM as a sampled/optimal-tiled
    // format on desktop GPUs)
    MAZE_RENDER_SYSTEM_VULKAN_API void ExpandRGBToRGBAVulkan(
        U8 const* _src,
        U8* _dst,
        Size _pixelsCount);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkBlendFactor GetBlendFactorVulkan(BlendFactor _blendFactor);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkBlendOp GetBlendOperationVulkan(BlendOperation _blendOperation);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkCompareOp GetCompareFunctionVulkan(CompareFunction _compareFunction);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkStencilOp GetStencilOperationVulkan(StencilOperation _stencilOperation);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkPrimitiveTopology GetRenderDrawTopologyVulkan(RenderDrawTopology _renderDrawTopology);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkCullModeFlags GetCullModeVulkan(CullMode _cullMode);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkFilter GetTextureFilterVulkan(TextureFilter _filter);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkSamplerMipmapMode GetTextureMipmapModeVulkan(TextureFilter _filter);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkSamplerAddressMode GetTextureWrapVulkan(TextureWrap _wrap);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_VULKAN_API VkFormat GetVertexAttributeFormatVulkan(
        VertexAttributeType _type,
        U8 _count,
        bool _normalized);

    //////////////////////////////////////////
    // Maps a GLSL input variable name (as reflected by SPIRV-Reflect off a
    // built-in/asset .mzglslvk shader, e.g. "a_position", "a_normal",
    // "a_texCoords0") to the engine's VertexAttributeSemantic enum - every
    // .mzglslvk vertex shader is expected to name its inputs following this
    // fixed convention (mirrors the semantic names GetVertexAttributeSemanticNameDX11
    // maps to HLSL semantics)
    MAZE_RENDER_SYSTEM_VULKAN_API VertexAttributeSemantic GetVertexAttributeSemanticByShaderNameVulkan(String const& _name);

    //////////////////////////////////////////
    // Records a pipeline image-memory-barrier transitioning _image from
    // _oldLayout to _newLayout (whole resource, single mip/layer range unless
    // _mipLevels/_layerCount overridden)
    MAZE_RENDER_SYSTEM_VULKAN_API void TransitionImageLayoutVulkan(
        VkCommandBuffer _commandBuffer,
        VkImage _image,
        VkImageAspectFlags _aspect,
        VkImageLayout _oldLayout,
        VkImageLayout _newLayout,
        U32 _mipLevels = 1u,
        U32 _layerCount = 1u,
        U32 _baseMipLevel = 0u,
        U32 _baseArrayLayer = 0u);

    //////////////////////////////////////////
    // Combines two 32-bit values into a single hash - used throughout the
    // Vulkan backend for pipeline/sampler/layout cache keys (mirrors the
    // U32/U64 state-object cache keys DX11 builds by hand)
    MAZE_RENDER_SYSTEM_VULKAN_API U64 CombineHashVulkan(U64 _seed, U64 _value);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHelpersVulkan_hpp_
//////////////////////////////////////////
