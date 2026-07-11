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
#include "maze-render-system-vulkan/MazeHelpersVulkan.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    VkFormat GetPixelFormatVulkan(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8: return VK_FORMAT_R8_SNORM;
            case PixelFormat::R_U8: return VK_FORMAT_R8_UNORM;
            case PixelFormat::R_S16: return VK_FORMAT_R16_SNORM;
            case PixelFormat::R_U16: return VK_FORMAT_R16_UNORM;
            case PixelFormat::R_F16: return VK_FORMAT_R16_SFLOAT;
            case PixelFormat::R_S32: return VK_FORMAT_R32_SINT;
            case PixelFormat::R_U32: return VK_FORMAT_R32_UINT;
            case PixelFormat::R_F32: return VK_FORMAT_R32_SFLOAT;

            case PixelFormat::RG_S8: return VK_FORMAT_R8G8_SNORM;
            case PixelFormat::RG_U8: return VK_FORMAT_R8G8_UNORM;
            case PixelFormat::RG_S16: return VK_FORMAT_R16G16_SNORM;
            case PixelFormat::RG_U16: return VK_FORMAT_R16G16_UNORM;
            case PixelFormat::RG_F16: return VK_FORMAT_R16G16_SFLOAT;
            case PixelFormat::RG_S32: return VK_FORMAT_R32G32_SINT;
            case PixelFormat::RG_U32: return VK_FORMAT_R32G32_UINT;
            case PixelFormat::RG_F32: return VK_FORMAT_R32G32_SFLOAT;

            // Most desktop Vulkan drivers don't support VK_FORMAT_R8G8B8_UNORM for
            // optimal-tiled sampled images - upload as RGBA instead (see ExpandRGBToRGBAVulkan,
            // called from the texture upload path, mirrors DX11's approach 1:1 since DXGI
            // has no 24-bit formats at all)
            case PixelFormat::RGB_U8: return VK_FORMAT_R8G8B8A8_UNORM;
            case PixelFormat::RGB_U5_6_5: return VK_FORMAT_R5G6B5_UNORM_PACK16;
            // 3-channel 32-bit float is natively supported (no expansion needed)
            case PixelFormat::RGB_F32: return VK_FORMAT_R32G32B32_SFLOAT;

            case PixelFormat::RGBA_U5_5_5_1: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
            case PixelFormat::RGBA_S8: return VK_FORMAT_R8G8B8A8_SNORM;
            case PixelFormat::RGBA_U8: return VK_FORMAT_R8G8B8A8_UNORM;
            case PixelFormat::RGBA_S16: return VK_FORMAT_R16G16B16A16_SNORM;
            case PixelFormat::RGBA_U16: return VK_FORMAT_R16G16B16A16_UNORM;
            case PixelFormat::RGBA_F16: return VK_FORMAT_R16G16B16A16_SFLOAT;
            case PixelFormat::RGBA_F32: return VK_FORMAT_R32G32B32A32_SFLOAT;

            case PixelFormat::BGRA_U8: return VK_FORMAT_B8G8R8A8_UNORM;

            case PixelFormat::DXT1_RGB: return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
            case PixelFormat::DXT3_RGBA: return VK_FORMAT_BC2_UNORM_BLOCK;
            case PixelFormat::DXT5_RGBA: return VK_FORMAT_BC3_UNORM_BLOCK;
            case PixelFormat::RGTC1_R: return VK_FORMAT_BC4_UNORM_BLOCK;
            case PixelFormat::RGTC2_RG: return VK_FORMAT_BC5_UNORM_BLOCK;

            // D24_UNORM alone is only optionally supported (VK_FORMAT_X8_D24_UNORM_PACK32) -
            // D24_UNORM_S8_UINT is the format that's actually guaranteed to be supported
            // alongside D32_SFLOAT on the vast majority of desktop GPUs (matches DX11,
            // which has to do the same thing since DXGI has no D24-only format either)
            case PixelFormat::DEPTH_U16: return VK_FORMAT_D16_UNORM;
            case PixelFormat::DEPTH_U24: return VK_FORMAT_D24_UNORM_S8_UINT;
            case PixelFormat::DEPTH_U32: return VK_FORMAT_D32_SFLOAT;
            case PixelFormat::DEPTH_F32: return VK_FORMAT_D32_SFLOAT;
            case PixelFormat::DEPTH_STENCIL_U24_8: return VK_FORMAT_D24_UNORM_S8_UINT;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return VK_FORMAT_D32_SFLOAT_S8_UINT;

            default: return VK_FORMAT_UNDEFINED;
        }
    }

    //////////////////////////////////////////
    PixelFormat::Enum GetPixelFormatFromVulkan(VkFormat _format)
    {
        switch (_format)
        {
            case VK_FORMAT_R8_SNORM: return PixelFormat::R_S8;
            case VK_FORMAT_R8_UNORM: return PixelFormat::R_U8;
            case VK_FORMAT_R16_SNORM: return PixelFormat::R_S16;
            case VK_FORMAT_R16_UNORM: return PixelFormat::R_U16;
            case VK_FORMAT_R16_SFLOAT: return PixelFormat::R_F16;
            case VK_FORMAT_R32_SINT: return PixelFormat::R_S32;
            case VK_FORMAT_R32_UINT: return PixelFormat::R_U32;
            case VK_FORMAT_R32_SFLOAT: return PixelFormat::R_F32;

            case VK_FORMAT_R8G8_SNORM: return PixelFormat::RG_S8;
            case VK_FORMAT_R8G8_UNORM: return PixelFormat::RG_U8;
            case VK_FORMAT_R16G16_SNORM: return PixelFormat::RG_S16;
            case VK_FORMAT_R16G16_UNORM: return PixelFormat::RG_U16;
            case VK_FORMAT_R16G16_SFLOAT: return PixelFormat::RG_F16;
            case VK_FORMAT_R32G32_SINT: return PixelFormat::RG_S32;
            case VK_FORMAT_R32G32_UINT: return PixelFormat::RG_U32;
            case VK_FORMAT_R32G32_SFLOAT: return PixelFormat::RG_F32;

            case VK_FORMAT_R5G6B5_UNORM_PACK16: return PixelFormat::RGB_U5_6_5;
            case VK_FORMAT_R32G32B32_SFLOAT: return PixelFormat::RGB_F32;

            case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return PixelFormat::RGBA_U5_5_5_1;
            case VK_FORMAT_R8G8B8A8_SNORM: return PixelFormat::RGBA_S8;
            case VK_FORMAT_R8G8B8A8_UNORM: return PixelFormat::RGBA_U8;
            case VK_FORMAT_R16G16B16A16_SNORM: return PixelFormat::RGBA_S16;
            case VK_FORMAT_R16G16B16A16_UNORM: return PixelFormat::RGBA_U16;
            case VK_FORMAT_R16G16B16A16_SFLOAT: return PixelFormat::RGBA_F16;
            case VK_FORMAT_R32G32B32A32_SFLOAT: return PixelFormat::RGBA_F32;

            case VK_FORMAT_B8G8R8A8_UNORM: return PixelFormat::BGRA_U8;

            case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return PixelFormat::DXT1_RGB;
            case VK_FORMAT_BC2_UNORM_BLOCK: return PixelFormat::DXT3_RGBA;
            case VK_FORMAT_BC3_UNORM_BLOCK: return PixelFormat::DXT5_RGBA;
            case VK_FORMAT_BC4_UNORM_BLOCK: return PixelFormat::RGTC1_R;
            case VK_FORMAT_BC5_UNORM_BLOCK: return PixelFormat::RGTC2_RG;

            case VK_FORMAT_D16_UNORM: return PixelFormat::DEPTH_U16;
            case VK_FORMAT_D24_UNORM_S8_UINT: return PixelFormat::DEPTH_STENCIL_U24_8;
            case VK_FORMAT_D32_SFLOAT: return PixelFormat::DEPTH_F32;
            case VK_FORMAT_D32_SFLOAT_S8_UINT: return PixelFormat::DEPTH_STENCIL_F32_U8;

            default: return PixelFormat::Unknown;
        }
    }

    //////////////////////////////////////////
    bool IsDepthPixelFormatVulkan(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::DEPTH_U16:
            case PixelFormat::DEPTH_U24:
            case PixelFormat::DEPTH_U32:
            case PixelFormat::DEPTH_F32:
            case PixelFormat::DEPTH_STENCIL_U24_8:
            case PixelFormat::DEPTH_STENCIL_F32_U8:
                return true;
            default:
                return false;
        }
    }

    //////////////////////////////////////////
    VkImageAspectFlags GetImageAspectFlagsVulkan(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            // Pure depth formats - the underlying VkFormat has no stencil plane
            case PixelFormat::DEPTH_U16:
            case PixelFormat::DEPTH_U32:
            case PixelFormat::DEPTH_F32:
                return VK_IMAGE_ASPECT_DEPTH_BIT;

            // DEPTH_U24 is backed by VK_FORMAT_D24_UNORM_S8_UINT (see GetPixelFormatVulkan -
            // no stencil-less D24 format is guaranteed to be supported), so the image
            // physically carries a stencil plane even though the engine-level format is
            // nominally depth-only. Whole-resource operations (clears, layout transitions)
            // must reference both planes or validation will complain about a partial aspect
            // mask on a depth+stencil image.
            case PixelFormat::DEPTH_U24:
            case PixelFormat::DEPTH_STENCIL_U24_8:
            case PixelFormat::DEPTH_STENCIL_F32_U8:
                return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

            default:
                return VK_IMAGE_ASPECT_COLOR_BIT;
        }
    }

    //////////////////////////////////////////
    void ExpandRGBToRGBAVulkan(
        U8 const* _src,
        U8* _dst,
        Size _pixelsCount)
    {
        for (Size i = 0; i < _pixelsCount; ++i)
        {
            _dst[i * 4 + 0] = _src[i * 3 + 0];
            _dst[i * 4 + 1] = _src[i * 3 + 1];
            _dst[i * 4 + 2] = _src[i * 3 + 2];
            _dst[i * 4 + 3] = 255;
        }
    }

    //////////////////////////////////////////
    VkBlendFactor GetBlendFactorVulkan(BlendFactor _blendFactor)
    {
        switch (_blendFactor)
        {
            case BlendFactor::One: return VK_BLEND_FACTOR_ONE;
            case BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
            case BlendFactor::SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
            case BlendFactor::SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
            case BlendFactor::DstColor: return VK_BLEND_FACTOR_DST_COLOR;
            case BlendFactor::DstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
            case BlendFactor::OneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            case BlendFactor::OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::OneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            case BlendFactor::OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
            default: MAZE_ERROR("Unsupported BlendFactor: %d", (S32)_blendFactor); return VK_BLEND_FACTOR_ONE;
        }
    }

    //////////////////////////////////////////
    VkBlendOp GetBlendOperationVulkan(BlendOperation _blendOperation)
    {
        switch (_blendOperation)
        {
            case BlendOperation::Add: return VK_BLEND_OP_ADD;
            case BlendOperation::Subtract: return VK_BLEND_OP_SUBTRACT;
            case BlendOperation::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
            case BlendOperation::Min: return VK_BLEND_OP_MIN;
            case BlendOperation::Max: return VK_BLEND_OP_MAX;
            default: MAZE_ERROR("Unsupported BlendOperation: %d", (S32)_blendOperation); return VK_BLEND_OP_ADD;
        }
    }

    //////////////////////////////////////////
    VkCompareOp GetCompareFunctionVulkan(CompareFunction _compareFunction)
    {
        switch (_compareFunction)
        {
            case CompareFunction::Never: return VK_COMPARE_OP_NEVER;
            case CompareFunction::Less: return VK_COMPARE_OP_LESS;
            case CompareFunction::Equal: return VK_COMPARE_OP_EQUAL;
            case CompareFunction::LessEqual: return VK_COMPARE_OP_LESS_OR_EQUAL;
            case CompareFunction::Greater: return VK_COMPARE_OP_GREATER;
            case CompareFunction::NotEqual: return VK_COMPARE_OP_NOT_EQUAL;
            case CompareFunction::GreaterEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
            case CompareFunction::Always: return VK_COMPARE_OP_ALWAYS;
            default: MAZE_ERROR("Unsupported CompareFunction: %d", (S32)_compareFunction); return VK_COMPARE_OP_ALWAYS;
        }
    }

    //////////////////////////////////////////
    VkStencilOp GetStencilOperationVulkan(StencilOperation _stencilOperation)
    {
        switch (_stencilOperation)
        {
            case StencilOperation::Keep: return VK_STENCIL_OP_KEEP;
            case StencilOperation::Zero: return VK_STENCIL_OP_ZERO;
            case StencilOperation::Replace: return VK_STENCIL_OP_REPLACE;
            case StencilOperation::Increment: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
            case StencilOperation::Decrement: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
            case StencilOperation::Invert: return VK_STENCIL_OP_INVERT;
            case StencilOperation::IncrementWrap: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
            case StencilOperation::DecrementWrap: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
            default: MAZE_ERROR("Unsupported StencilOperation: %d", (S32)_stencilOperation); return VK_STENCIL_OP_KEEP;
        }
    }

    //////////////////////////////////////////
    VkPrimitiveTopology GetRenderDrawTopologyVulkan(RenderDrawTopology _renderDrawTopology)
    {
        switch (_renderDrawTopology)
        {
            case RenderDrawTopology::Points: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            case RenderDrawTopology::Lines: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            case RenderDrawTopology::LinesStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
            case RenderDrawTopology::Triangles: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            default: MAZE_ERROR("Unsupported RenderDrawTopology: %d", (S32)_renderDrawTopology); return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        }
    }

    //////////////////////////////////////////
    VkCullModeFlags GetCullModeVulkan(CullMode _cullMode)
    {
        switch (_cullMode)
        {
            case CullMode::Off: return VK_CULL_MODE_NONE;
            case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
            case CullMode::Back: return VK_CULL_MODE_BACK_BIT;
            default: MAZE_ERROR("Unsupported CullMode: %d", (S32)_cullMode); return VK_CULL_MODE_BACK_BIT;
        }
    }

    //////////////////////////////////////////
    VkFilter GetTextureFilterVulkan(TextureFilter _filter)
    {
        switch (_filter)
        {
            case TextureFilter::Nearest:
            case TextureFilter::NearestMipmapNearest:
            case TextureFilter::NearestMipmapLinear:
                return VK_FILTER_NEAREST;

            case TextureFilter::Linear:
            case TextureFilter::LinearMipmapNearest:
            case TextureFilter::LinearMipmapLinear:
                return VK_FILTER_LINEAR;

            default:
                return VK_FILTER_LINEAR;
        }
    }

    //////////////////////////////////////////
    VkSamplerMipmapMode GetTextureMipmapModeVulkan(TextureFilter _filter)
    {
        switch (_filter)
        {
            case TextureFilter::NearestMipmapLinear:
            case TextureFilter::LinearMipmapLinear:
                return VK_SAMPLER_MIPMAP_MODE_LINEAR;

            // Nearest/Linear (no mip chain sampled - see the maxLod=0 clamp applied by
            // texture classes for these filters) and *MipmapNearest both step discretely
            // between mip levels
            case TextureFilter::Nearest:
            case TextureFilter::Linear:
            case TextureFilter::NearestMipmapNearest:
            case TextureFilter::LinearMipmapNearest:
            default:
                return VK_SAMPLER_MIPMAP_MODE_NEAREST;
        }
    }

    //////////////////////////////////////////
    VkSamplerAddressMode GetTextureWrapVulkan(TextureWrap _wrap)
    {
        switch (_wrap)
        {
            case TextureWrap::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
            case TextureWrap::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            case TextureWrap::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            case TextureWrap::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
            default: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        }
    }

    //////////////////////////////////////////
    VkFormat GetVertexAttributeFormatVulkan(
        VertexAttributeType _type,
        U8 _count,
        bool _normalized)
    {
        switch (_type)
        {
            case VertexAttributeType::S8:
            {
                switch (_count)
                {
                    case 1: return _normalized ? VK_FORMAT_R8_SNORM : VK_FORMAT_R8_SINT;
                    case 2: return _normalized ? VK_FORMAT_R8G8_SNORM : VK_FORMAT_R8G8_SINT;
                    case 4: return _normalized ? VK_FORMAT_R8G8B8A8_SNORM : VK_FORMAT_R8G8B8A8_SINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::U8:
            {
                switch (_count)
                {
                    case 1: return _normalized ? VK_FORMAT_R8_UNORM : VK_FORMAT_R8_UINT;
                    case 2: return _normalized ? VK_FORMAT_R8G8_UNORM : VK_FORMAT_R8G8_UINT;
                    case 4: return _normalized ? VK_FORMAT_R8G8B8A8_UNORM : VK_FORMAT_R8G8B8A8_UINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::S16:
            {
                switch (_count)
                {
                    case 1: return _normalized ? VK_FORMAT_R16_SNORM : VK_FORMAT_R16_SINT;
                    case 2: return _normalized ? VK_FORMAT_R16G16_SNORM : VK_FORMAT_R16G16_SINT;
                    case 4: return _normalized ? VK_FORMAT_R16G16B16A16_SNORM : VK_FORMAT_R16G16B16A16_SINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::U16:
            {
                switch (_count)
                {
                    case 1: return _normalized ? VK_FORMAT_R16_UNORM : VK_FORMAT_R16_UINT;
                    case 2: return _normalized ? VK_FORMAT_R16G16_UNORM : VK_FORMAT_R16G16_UINT;
                    case 4: return _normalized ? VK_FORMAT_R16G16B16A16_UNORM : VK_FORMAT_R16G16B16A16_UINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::S32:
            {
                switch (_count)
                {
                    case 1: return VK_FORMAT_R32_SINT;
                    case 2: return VK_FORMAT_R32G32_SINT;
                    case 3: return VK_FORMAT_R32G32B32_SINT;
                    case 4: return VK_FORMAT_R32G32B32A32_SINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::U32:
            {
                switch (_count)
                {
                    case 1: return VK_FORMAT_R32_UINT;
                    case 2: return VK_FORMAT_R32G32_UINT;
                    case 3: return VK_FORMAT_R32G32B32_UINT;
                    case 4: return VK_FORMAT_R32G32B32A32_UINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::F16:
            {
                switch (_count)
                {
                    case 1: return VK_FORMAT_R16_SFLOAT;
                    case 2: return VK_FORMAT_R16G16_SFLOAT;
                    case 4: return VK_FORMAT_R16G16B16A16_SFLOAT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::F32:
            {
                switch (_count)
                {
                    case 1: return VK_FORMAT_R32_SFLOAT;
                    case 2: return VK_FORMAT_R32G32_SFLOAT;
                    case 3: return VK_FORMAT_R32G32B32_SFLOAT;
                    case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
                    default: break;
                }
                break;
            }
            default: break;
        }

        MAZE_ERROR("Unsupported vertex attribute format! type=%d count=%d", (S32)_type, (S32)_count);
        return VK_FORMAT_UNDEFINED;
    }

    //////////////////////////////////////////
    // Small lookup table describing the src/dst pipeline-stage and access masks for
    // every layout transition this backend actually issues. Vulkan barriers are only
    // as correct as the stage/access pair you pick - an overly broad mask is safe but
    // stalls the pipeline, an overly narrow one is a data race the validation layers
    // often (but not always) catch. Each case below is commented with the operation
    // it corresponds to so this is reviewable without re-deriving it from scratch.
    namespace
    {
        //////////////////////////////////////////
        void GetBarrierMasksVulkan(
            VkImageLayout _oldLayout,
            VkImageLayout _newLayout,
            VkPipelineStageFlags& _outSrcStage,
            VkAccessFlags& _outSrcAccess,
            VkPipelineStageFlags& _outDstStage,
            VkAccessFlags& _outDstAccess)
        {
            // UNDEFINED -> TRANSFER_DST_OPTIMAL
            // Initial upload of texture data via a staging buffer (vkCmdCopyBufferToImage).
            // Nothing to wait on for the source side (image contents are undefined).
            if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                _outSrcAccess = 0;
                _outDstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outDstAccess = VK_ACCESS_TRANSFER_WRITE_BIT;
                return;
            }

            // TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
            // After the staging copy (or the last mip's blit) completes, the texture is
            // ready to be sampled. Assumes fragment-stage sampling only (the common case
            // for this backend's material textures) - if a future caller needs vertex-stage
            // sampling too, widen dstStage to VK_PIPELINE_STAGE_VERTEX_SHADER_BIT as well.
            if (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outSrcAccess = VK_ACCESS_TRANSFER_WRITE_BIT;
                _outDstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                _outDstAccess = VK_ACCESS_SHADER_READ_BIT;
                return;
            }

            // TRANSFER_DST_OPTIMAL -> TRANSFER_SRC_OPTIMAL
            // Mid mip-chain generation: a mip level just finished being blitted into
            // (dst of the previous blit) and is about to be read as the source of the
            // next mip's blit.
            if (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outSrcAccess = VK_ACCESS_TRANSFER_WRITE_BIT;
                _outDstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outDstAccess = VK_ACCESS_TRANSFER_READ_BIT;
                return;
            }

            // TRANSFER_SRC_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
            // End of mip-chain generation: the last mip level (or any level left as a
            // blit source) becomes sampleable.
            if (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outSrcAccess = VK_ACCESS_TRANSFER_READ_BIT;
                _outDstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                _outDstAccess = VK_ACCESS_SHADER_READ_BIT;
                return;
            }

            // UNDEFINED -> TRANSFER_SRC_OPTIMAL
            // Used when a texture is only ever read back (e.g. staging-copy source for
            // readAsPixelSheet) and never written via the sampled path.
            if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                _outSrcAccess = 0;
                _outDstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outDstAccess = VK_ACCESS_TRANSFER_READ_BIT;
                return;
            }

            // UNDEFINED -> COLOR_ATTACHMENT_OPTIMAL
            // First-use of an offscreen color attachment (RenderBufferVulkan) before the
            // first vkCmdBeginRendering that targets it.
            if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                _outSrcAccess = 0;
                _outDstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                _outDstAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                return;
            }

            // UNDEFINED -> DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            // First-use of an offscreen depth/stencil attachment.
            if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                _outSrcAccess = 0;
                _outDstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                _outDstAccess = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                return;
            }

            // COLOR_ATTACHMENT_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
            // A RenderBufferVulkan's color attachment is finished being drawn to for this
            // pass and is about to be sampled by a later pass (e.g. a post-process material).
            if (_oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                _outSrcAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                _outDstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                _outDstAccess = VK_ACCESS_SHADER_READ_BIT;
                return;
            }

            // SHADER_READ_ONLY_OPTIMAL -> COLOR_ATTACHMENT_OPTIMAL
            // The reverse of the above - re-targeting a color attachment that was
            // previously sampled from (ping-pong render targets).
            if (_oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                _outSrcAccess = VK_ACCESS_SHADER_READ_BIT;
                _outDstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                _outDstAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                return;
            }

            // COLOR_ATTACHMENT_OPTIMAL -> TRANSFER_SRC_OPTIMAL / TRANSFER_DST_OPTIMAL
            // RenderBufferVulkan::blit() / MSAA resolve source or destination.
            if (_oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL &&
                (_newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL || _newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL))
            {
                _outSrcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                _outSrcAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                _outDstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outDstAccess = (_newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) ? VK_ACCESS_TRANSFER_READ_BIT : VK_ACCESS_TRANSFER_WRITE_BIT;
                return;
            }

            // TRANSFER_SRC_OPTIMAL / TRANSFER_DST_OPTIMAL -> COLOR_ATTACHMENT_OPTIMAL
            // The reverse - resuming rendering into an attachment that was just blit
            // source/destination.
            if ((_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL || _oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) &&
                _newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
            {
                _outSrcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outSrcAccess = (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) ? VK_ACCESS_TRANSFER_READ_BIT : VK_ACCESS_TRANSFER_WRITE_BIT;
                _outDstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                _outDstAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                return;
            }

            // DEPTH_STENCIL_ATTACHMENT_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL / TRANSFER_SRC_OPTIMAL
            // Depth buffer sampled by a later pass (e.g. shadow map, distortion) or used
            // as the source of a depth MSAA resolve (RenderSystemVulkan::resolveDepthMSAA).
            if (_oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL &&
                (_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL || _newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL))
            {
                _outSrcStage = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                _outSrcAccess = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                _outDstStage = (_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) ? VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT : VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outDstAccess = (_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) ? VK_ACCESS_SHADER_READ_BIT : VK_ACCESS_TRANSFER_READ_BIT;
                return;
            }

            // SHADER_READ_ONLY_OPTIMAL / TRANSFER_SRC_OPTIMAL -> DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            // The reverse - resuming depth writes into a depth texture that was just sampled/resolved.
            if ((_oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL || _oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) &&
                _newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
            {
                _outSrcStage = (_oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) ? VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT : VK_PIPELINE_STAGE_TRANSFER_BIT;
                _outSrcAccess = (_oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) ? VK_ACCESS_SHADER_READ_BIT : VK_ACCESS_TRANSFER_READ_BIT;
                _outDstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                _outDstAccess = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                return;
            }

            // Fallback for any transition not curated above - correct but pessimal
            // (stalls the whole pipeline). Logged so an unexpected transition shows up
            // during development instead of silently costing performance.
            MAZE_LOG(
                "TransitionImageLayoutVulkan: no curated stage/access mask for layout %d -> %d, "
                "falling back to ALL_COMMANDS/MEMORY_READ|WRITE (correct but slow)",
                (S32)_oldLayout,
                (S32)_newLayout);
            _outSrcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
            _outSrcAccess = VK_ACCESS_MEMORY_WRITE_BIT;
            _outDstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
            _outDstAccess = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT;
        }

    } // namespace
    //////////////////////////////////////////

    //////////////////////////////////////////
    void TransitionImageLayoutVulkan(
        VkCommandBuffer _commandBuffer,
        VkImage _image,
        VkImageAspectFlags _aspect,
        VkImageLayout _oldLayout,
        VkImageLayout _newLayout,
        U32 _mipLevels,
        U32 _layerCount,
        U32 _baseMipLevel,
        U32 _baseArrayLayer)
    {
        VkPipelineStageFlags srcStage;
        VkAccessFlags srcAccess;
        VkPipelineStageFlags dstStage;
        VkAccessFlags dstAccess;
        GetBarrierMasksVulkan(_oldLayout, _newLayout, srcStage, srcAccess, dstStage, dstAccess);

        VkImageMemoryBarrier barrier;
        memset(&barrier, 0, sizeof(barrier));
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = _oldLayout;
        barrier.newLayout = _newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = _image;
        barrier.subresourceRange.aspectMask = _aspect;
        barrier.subresourceRange.baseMipLevel = _baseMipLevel;
        barrier.subresourceRange.levelCount = _mipLevels;
        barrier.subresourceRange.baseArrayLayer = _baseArrayLayer;
        barrier.subresourceRange.layerCount = _layerCount;
        barrier.srcAccessMask = srcAccess;
        barrier.dstAccessMask = dstAccess;

        vkCmdPipelineBarrier(
            _commandBuffer,
            srcStage,
            dstStage,
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier);
    }

    //////////////////////////////////////////
    U64 CombineHashVulkan(U64 _seed, U64 _value)
    {
        return _seed ^ (_value + 0x9e3779b97f4a7c15ULL + (_seed << 6) + (_seed >> 2));
    }

    //////////////////////////////////////////
    VertexAttributeSemantic GetVertexAttributeSemanticByShaderNameVulkan(String const& _name)
    {
        // Fixed 'a_'-prefixed naming convention every built-in/asset .mzglslvk
        // vertex shader is expected to follow (see MazeShaderVulkan.cpp
        // reflectVulkanShader()) - also note VertexArrayObjectVulkan bakes
        // VkVertexInputAttributeDescription.location == (U32)semantic
        // directly (binding N == semantic N, see its class banner comment),
        // so every shader's 'layout(location = N)' for these inputs must use
        // the same VertexAttributeSemantic enum ordinal N, not a
        // declaration-order location - this table is the name side of that
        // same fixed mapping.
        if (_name == "a_position") return VertexAttributeSemantic::Position;
        if (_name == "a_normal") return VertexAttributeSemantic::Normal;
        if (_name == "a_tangent") return VertexAttributeSemantic::Tangent;
        if (_name == "a_bitangent") return VertexAttributeSemantic::Bitangent;
        if (_name == "a_color") return VertexAttributeSemantic::Color;
        if (_name == "a_texCoords0") return VertexAttributeSemantic::TexCoords0;
        if (_name == "a_texCoords1") return VertexAttributeSemantic::TexCoords1;
        if (_name == "a_texCoords2") return VertexAttributeSemantic::TexCoords2;
        if (_name == "a_texCoords3") return VertexAttributeSemantic::TexCoords3;
        if (_name == "a_texCoords4") return VertexAttributeSemantic::TexCoords4;
        if (_name == "a_texCoords5") return VertexAttributeSemantic::TexCoords5;
        if (_name == "a_texCoords6") return VertexAttributeSemantic::TexCoords6;
        if (_name == "a_texCoords7") return VertexAttributeSemantic::TexCoords7;
        if (_name == "a_blendWeights") return VertexAttributeSemantic::BlendWeights;
        if (_name == "a_blendIndices") return VertexAttributeSemantic::BlendIndices;

        return VertexAttributeSemantic::MAX;
    }


} // namespace Maze
//////////////////////////////////////////
