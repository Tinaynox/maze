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
#include "MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeHelpersDX11.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    DXGI_FORMAT GetPixelFormatDX11(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::R_S8: return DXGI_FORMAT_R8_SNORM;
            case PixelFormat::R_U8: return DXGI_FORMAT_R8_UNORM;
            case PixelFormat::R_S16: return DXGI_FORMAT_R16_SNORM;
            case PixelFormat::R_U16: return DXGI_FORMAT_R16_UNORM;
            case PixelFormat::R_F16: return DXGI_FORMAT_R16_FLOAT;
            case PixelFormat::R_S32: return DXGI_FORMAT_R32_SINT;
            case PixelFormat::R_U32: return DXGI_FORMAT_R32_UINT;
            case PixelFormat::R_F32: return DXGI_FORMAT_R32_FLOAT;

            case PixelFormat::RG_S8: return DXGI_FORMAT_R8G8_SNORM;
            case PixelFormat::RG_U8: return DXGI_FORMAT_R8G8_UNORM;
            case PixelFormat::RG_S16: return DXGI_FORMAT_R16G16_SNORM;
            case PixelFormat::RG_U16: return DXGI_FORMAT_R16G16_UNORM;
            case PixelFormat::RG_F16: return DXGI_FORMAT_R16G16_FLOAT;
            case PixelFormat::RG_S32: return DXGI_FORMAT_R32G32_SINT;
            case PixelFormat::RG_U32: return DXGI_FORMAT_R32G32_UINT;
            case PixelFormat::RG_F32: return DXGI_FORMAT_R32G32_FLOAT;

            // 24-bit RGB formats are uploaded as RGBA (see Texture2DDX11)
            case PixelFormat::RGB_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case PixelFormat::RGB_U5_6_5: return DXGI_FORMAT_B5G6R5_UNORM;
            case PixelFormat::RGB_F32: return DXGI_FORMAT_R32G32B32_FLOAT;

            case PixelFormat::RGBA_U5_5_5_1: return DXGI_FORMAT_B5G5R5A1_UNORM;
            case PixelFormat::RGBA_S8: return DXGI_FORMAT_R8G8B8A8_SNORM;
            case PixelFormat::RGBA_U8: return DXGI_FORMAT_R8G8B8A8_UNORM;
            case PixelFormat::RGBA_S16: return DXGI_FORMAT_R16G16B16A16_SNORM;
            case PixelFormat::RGBA_U16: return DXGI_FORMAT_R16G16B16A16_UNORM;
            case PixelFormat::RGBA_F16: return DXGI_FORMAT_R16G16B16A16_FLOAT;
            case PixelFormat::RGBA_F32: return DXGI_FORMAT_R32G32B32A32_FLOAT;

            case PixelFormat::BGRA_U8: return DXGI_FORMAT_B8G8R8A8_UNORM;

            case PixelFormat::DXT1_RGB: return DXGI_FORMAT_BC1_UNORM;
            case PixelFormat::DXT3_RGBA: return DXGI_FORMAT_BC2_UNORM;
            case PixelFormat::DXT5_RGBA: return DXGI_FORMAT_BC3_UNORM;
            case PixelFormat::RGTC1_R: return DXGI_FORMAT_BC4_UNORM;
            case PixelFormat::RGTC2_RG: return DXGI_FORMAT_BC5_UNORM;

            case PixelFormat::DEPTH_U16: return DXGI_FORMAT_D16_UNORM;
            case PixelFormat::DEPTH_U24: return DXGI_FORMAT_D24_UNORM_S8_UINT;
            case PixelFormat::DEPTH_U32: return DXGI_FORMAT_D32_FLOAT;
            case PixelFormat::DEPTH_F32: return DXGI_FORMAT_D32_FLOAT;
            case PixelFormat::DEPTH_STENCIL_U24_8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;

            default: return DXGI_FORMAT_UNKNOWN;
        }
    }

    //////////////////////////////////////////
    DXGI_FORMAT GetPixelFormatResourceDX11(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::DEPTH_U16: return DXGI_FORMAT_R16_TYPELESS;
            case PixelFormat::DEPTH_U24: return DXGI_FORMAT_R24G8_TYPELESS;
            case PixelFormat::DEPTH_U32: return DXGI_FORMAT_R32_TYPELESS;
            case PixelFormat::DEPTH_F32: return DXGI_FORMAT_R32_TYPELESS;
            case PixelFormat::DEPTH_STENCIL_U24_8: return DXGI_FORMAT_R24G8_TYPELESS;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return DXGI_FORMAT_R32G8X24_TYPELESS;
            default: return GetPixelFormatDX11(_pixelFormat);
        }
    }

    //////////////////////////////////////////
    DXGI_FORMAT GetPixelFormatSRVDX11(PixelFormat::Enum _pixelFormat)
    {
        switch (_pixelFormat)
        {
            case PixelFormat::DEPTH_U16: return DXGI_FORMAT_R16_UNORM;
            case PixelFormat::DEPTH_U24: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            case PixelFormat::DEPTH_U32: return DXGI_FORMAT_R32_FLOAT;
            case PixelFormat::DEPTH_F32: return DXGI_FORMAT_R32_FLOAT;
            case PixelFormat::DEPTH_STENCIL_U24_8: return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
            case PixelFormat::DEPTH_STENCIL_F32_U8: return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
            default: return GetPixelFormatDX11(_pixelFormat);
        }
    }

    //////////////////////////////////////////
    bool IsDepthPixelFormatDX11(PixelFormat::Enum _pixelFormat)
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
    D3D11_BLEND GetBlendFactorDX11(BlendFactor _blendFactor)
    {
        switch (_blendFactor)
        {
            case BlendFactor::One: return D3D11_BLEND_ONE;
            case BlendFactor::Zero: return D3D11_BLEND_ZERO;
            case BlendFactor::SrcColor: return D3D11_BLEND_SRC_COLOR;
            case BlendFactor::SrcAlpha: return D3D11_BLEND_SRC_ALPHA;
            case BlendFactor::DstColor: return D3D11_BLEND_DEST_COLOR;
            case BlendFactor::DstAlpha: return D3D11_BLEND_DEST_ALPHA;
            case BlendFactor::OneMinusSrcColor: return D3D11_BLEND_INV_SRC_COLOR;
            case BlendFactor::OneMinusSrcAlpha: return D3D11_BLEND_INV_SRC_ALPHA;
            case BlendFactor::OneMinusDstColor: return D3D11_BLEND_INV_DEST_COLOR;
            case BlendFactor::OneMinusDstAlpha: return D3D11_BLEND_INV_DEST_ALPHA;
            default: MAZE_ERROR("Unsupported BlendFactor: %d", (S32)_blendFactor); return D3D11_BLEND_ONE;
        }
    }

    //////////////////////////////////////////
    D3D11_BLEND_OP GetBlendOperationDX11(BlendOperation _blendOperation)
    {
        switch (_blendOperation)
        {
            case BlendOperation::Add: return D3D11_BLEND_OP_ADD;
            case BlendOperation::Subtract: return D3D11_BLEND_OP_SUBTRACT;
            case BlendOperation::ReverseSubtract: return D3D11_BLEND_OP_REV_SUBTRACT;
            case BlendOperation::Min: return D3D11_BLEND_OP_MIN;
            case BlendOperation::Max: return D3D11_BLEND_OP_MAX;
            default: MAZE_ERROR("Unsupported BlendOperation: %d", (S32)_blendOperation); return D3D11_BLEND_OP_ADD;
        }
    }

    //////////////////////////////////////////
    D3D11_COMPARISON_FUNC GetCompareFunctionDX11(CompareFunction _compareFunction)
    {
        switch (_compareFunction)
        {
            case CompareFunction::Never: return D3D11_COMPARISON_NEVER;
            case CompareFunction::Less: return D3D11_COMPARISON_LESS;
            case CompareFunction::Equal: return D3D11_COMPARISON_EQUAL;
            case CompareFunction::LessEqual: return D3D11_COMPARISON_LESS_EQUAL;
            case CompareFunction::Greater: return D3D11_COMPARISON_GREATER;
            case CompareFunction::NotEqual: return D3D11_COMPARISON_NOT_EQUAL;
            case CompareFunction::GreaterEqual: return D3D11_COMPARISON_GREATER_EQUAL;
            case CompareFunction::Always: return D3D11_COMPARISON_ALWAYS;
            default: MAZE_ERROR("Unsupported CompareFunction: %d", (S32)_compareFunction); return D3D11_COMPARISON_ALWAYS;
        }
    }

    //////////////////////////////////////////
    D3D11_STENCIL_OP GetStencilOperationDX11(StencilOperation _stencilOperation)
    {
        switch (_stencilOperation)
        {
            case StencilOperation::Keep: return D3D11_STENCIL_OP_KEEP;
            case StencilOperation::Zero: return D3D11_STENCIL_OP_ZERO;
            case StencilOperation::Replace: return D3D11_STENCIL_OP_REPLACE;
            case StencilOperation::Increment: return D3D11_STENCIL_OP_INCR_SAT;
            case StencilOperation::Decrement: return D3D11_STENCIL_OP_DECR_SAT;
            case StencilOperation::Invert: return D3D11_STENCIL_OP_INVERT;
            case StencilOperation::IncrementWrap: return D3D11_STENCIL_OP_INCR;
            case StencilOperation::DecrementWrap: return D3D11_STENCIL_OP_DECR;
            default: MAZE_ERROR("Unsupported StencilOperation: %d", (S32)_stencilOperation); return D3D11_STENCIL_OP_KEEP;
        }
    }

    //////////////////////////////////////////
    D3D11_PRIMITIVE_TOPOLOGY GetRenderDrawTopologyDX11(RenderDrawTopology _renderDrawTopology)
    {
        switch (_renderDrawTopology)
        {
            case RenderDrawTopology::Points: return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
            case RenderDrawTopology::Lines: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
            case RenderDrawTopology::LinesStrip: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
            case RenderDrawTopology::Triangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            default: MAZE_ERROR("Unsupported RenderDrawTopology: %d", (S32)_renderDrawTopology); return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        }
    }

    //////////////////////////////////////////
    D3D11_FILTER GetTextureFilterDX11(
        TextureFilter _minFilter,
        TextureFilter _magFilter,
        bool _anisotropy)
    {
        if (_anisotropy)
            return D3D11_FILTER_ANISOTROPIC;

        bool minLinear = false;
        bool mipLinear = false;
        switch (_minFilter)
        {
            case TextureFilter::Nearest: minLinear = false; mipLinear = false; break;
            case TextureFilter::Linear: minLinear = true; mipLinear = false; break;
            case TextureFilter::NearestMipmapNearest: minLinear = false; mipLinear = false; break;
            case TextureFilter::LinearMipmapNearest: minLinear = true; mipLinear = false; break;
            case TextureFilter::NearestMipmapLinear: minLinear = false; mipLinear = true; break;
            case TextureFilter::LinearMipmapLinear: minLinear = true; mipLinear = true; break;
            default: break;
        }

        bool magLinear = (_magFilter != TextureFilter::Nearest);

        if (minLinear)
        {
            if (magLinear)
                return mipLinear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            else
                return mipLinear ? D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        }
        else
        {
            if (magLinear)
                return mipLinear ? D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR : D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
            else
                return mipLinear ? D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
        }
    }

    //////////////////////////////////////////
    D3D11_TEXTURE_ADDRESS_MODE GetTextureWrapDX11(TextureWrap _wrap)
    {
        switch (_wrap)
        {
            case TextureWrap::Repeat: return D3D11_TEXTURE_ADDRESS_WRAP;
            case TextureWrap::MirroredRepeat: return D3D11_TEXTURE_ADDRESS_MIRROR;
            case TextureWrap::ClampToEdge: return D3D11_TEXTURE_ADDRESS_CLAMP;
            case TextureWrap::ClampToBorder: return D3D11_TEXTURE_ADDRESS_BORDER;
            default: return D3D11_TEXTURE_ADDRESS_CLAMP;
        }
    }

    //////////////////////////////////////////
    DXGI_FORMAT GetVertexAttributeFormatDX11(
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
                    case 1: return _normalized ? DXGI_FORMAT_R8_SNORM : DXGI_FORMAT_R8_SINT;
                    case 2: return _normalized ? DXGI_FORMAT_R8G8_SNORM : DXGI_FORMAT_R8G8_SINT;
                    case 4: return _normalized ? DXGI_FORMAT_R8G8B8A8_SNORM : DXGI_FORMAT_R8G8B8A8_SINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::U8:
            {
                switch (_count)
                {
                    case 1: return _normalized ? DXGI_FORMAT_R8_UNORM : DXGI_FORMAT_R8_UINT;
                    case 2: return _normalized ? DXGI_FORMAT_R8G8_UNORM : DXGI_FORMAT_R8G8_UINT;
                    case 4: return _normalized ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::S16:
            {
                switch (_count)
                {
                    case 1: return _normalized ? DXGI_FORMAT_R16_SNORM : DXGI_FORMAT_R16_SINT;
                    case 2: return _normalized ? DXGI_FORMAT_R16G16_SNORM : DXGI_FORMAT_R16G16_SINT;
                    case 4: return _normalized ? DXGI_FORMAT_R16G16B16A16_SNORM : DXGI_FORMAT_R16G16B16A16_SINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::U16:
            {
                switch (_count)
                {
                    case 1: return _normalized ? DXGI_FORMAT_R16_UNORM : DXGI_FORMAT_R16_UINT;
                    case 2: return _normalized ? DXGI_FORMAT_R16G16_UNORM : DXGI_FORMAT_R16G16_UINT;
                    case 4: return _normalized ? DXGI_FORMAT_R16G16B16A16_UNORM : DXGI_FORMAT_R16G16B16A16_UINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::S32:
            {
                switch (_count)
                {
                    case 1: return DXGI_FORMAT_R32_SINT;
                    case 2: return DXGI_FORMAT_R32G32_SINT;
                    case 3: return DXGI_FORMAT_R32G32B32_SINT;
                    case 4: return DXGI_FORMAT_R32G32B32A32_SINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::U32:
            {
                switch (_count)
                {
                    case 1: return DXGI_FORMAT_R32_UINT;
                    case 2: return DXGI_FORMAT_R32G32_UINT;
                    case 3: return DXGI_FORMAT_R32G32B32_UINT;
                    case 4: return DXGI_FORMAT_R32G32B32A32_UINT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::F16:
            {
                switch (_count)
                {
                    case 1: return DXGI_FORMAT_R16_FLOAT;
                    case 2: return DXGI_FORMAT_R16G16_FLOAT;
                    case 4: return DXGI_FORMAT_R16G16B16A16_FLOAT;
                    default: break;
                }
                break;
            }
            case VertexAttributeType::F32:
            {
                switch (_count)
                {
                    case 1: return DXGI_FORMAT_R32_FLOAT;
                    case 2: return DXGI_FORMAT_R32G32_FLOAT;
                    case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
                    case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
                    default: break;
                }
                break;
            }
            default: break;
        }

        MAZE_ERROR("Unsupported vertex attribute format! type=%d count=%d", (S32)_type, (S32)_count);
        return DXGI_FORMAT_UNKNOWN;
    }

    //////////////////////////////////////////
    CString GetVertexAttributeSemanticNameDX11(VertexAttributeSemantic _semantic)
    {
        switch (_semantic)
        {
            case VertexAttributeSemantic::Position: return "POSITION";
            case VertexAttributeSemantic::Normal: return "NORMAL";
            case VertexAttributeSemantic::Tangent: return "TANGENT";
            case VertexAttributeSemantic::Bitangent: return "BINORMAL";
            case VertexAttributeSemantic::Color: return "COLOR";
            case VertexAttributeSemantic::TexCoords0:
            case VertexAttributeSemantic::TexCoords1:
            case VertexAttributeSemantic::TexCoords2:
            case VertexAttributeSemantic::TexCoords3:
            case VertexAttributeSemantic::TexCoords4:
            case VertexAttributeSemantic::TexCoords5:
            case VertexAttributeSemantic::TexCoords6:
            case VertexAttributeSemantic::TexCoords7: return "TEXCOORD";
            case VertexAttributeSemantic::BlendWeights: return "BLENDWEIGHT";
            case VertexAttributeSemantic::BlendIndices: return "BLENDINDICES";
            default: return nullptr;
        }
    }

    //////////////////////////////////////////
    U32 GetVertexAttributeSemanticIndexDX11(VertexAttributeSemantic _semantic)
    {
        switch (_semantic)
        {
            case VertexAttributeSemantic::TexCoords1: return 1u;
            case VertexAttributeSemantic::TexCoords2: return 2u;
            case VertexAttributeSemantic::TexCoords3: return 3u;
            case VertexAttributeSemantic::TexCoords4: return 4u;
            case VertexAttributeSemantic::TexCoords5: return 5u;
            case VertexAttributeSemantic::TexCoords6: return 6u;
            case VertexAttributeSemantic::TexCoords7: return 7u;
            default: return 0u;
        }
    }


} // namespace Maze
//////////////////////////////////////////
