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
#if (!defined(_MazeHelpersDX11_hpp_))
#define _MazeHelpersDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
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
    MAZE_RENDER_SYSTEM_DX11_API DXGI_FORMAT GetPixelFormatDX11(PixelFormat::Enum _pixelFormat);

    //////////////////////////////////////////
    // For depth formats - the typeless format the texture resource should be created with
    MAZE_RENDER_SYSTEM_DX11_API DXGI_FORMAT GetPixelFormatResourceDX11(PixelFormat::Enum _pixelFormat);

    //////////////////////////////////////////
    // For depth formats - the format the SRV should be created with
    MAZE_RENDER_SYSTEM_DX11_API DXGI_FORMAT GetPixelFormatSRVDX11(PixelFormat::Enum _pixelFormat);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API bool IsDepthPixelFormatDX11(PixelFormat::Enum _pixelFormat);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API D3D11_BLEND GetBlendFactorDX11(BlendFactor _blendFactor);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API D3D11_BLEND_OP GetBlendOperationDX11(BlendOperation _blendOperation);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API D3D11_COMPARISON_FUNC GetCompareFunctionDX11(CompareFunction _compareFunction);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API D3D11_STENCIL_OP GetStencilOperationDX11(StencilOperation _stencilOperation);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API D3D11_PRIMITIVE_TOPOLOGY GetRenderDrawTopologyDX11(RenderDrawTopology _renderDrawTopology);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API D3D11_FILTER GetTextureFilterDX11(
        TextureFilter _minFilter,
        TextureFilter _magFilter,
        bool _anisotropy);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API D3D11_TEXTURE_ADDRESS_MODE GetTextureWrapDX11(TextureWrap _wrap);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API DXGI_FORMAT GetVertexAttributeFormatDX11(
        VertexAttributeType _type,
        U8 _count,
        bool _normalized);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API CString GetVertexAttributeSemanticNameDX11(VertexAttributeSemantic _semantic);

    //////////////////////////////////////////
    MAZE_RENDER_SYSTEM_DX11_API U32 GetVertexAttributeSemanticIndexDX11(VertexAttributeSemantic _semantic);


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeHelpersDX11_hpp_
//////////////////////////////////////////
