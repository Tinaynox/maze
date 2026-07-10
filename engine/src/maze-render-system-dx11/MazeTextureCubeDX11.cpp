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
#include "maze-render-system-dx11/MazeTextureCubeDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeHelpersDX11.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class TextureCubeDX11
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(TextureCubeDX11, TextureCube);

    //////////////////////////////////////////
    TextureCubeDX11::TextureCubeDX11()
    {
    }

    //////////////////////////////////////////
    TextureCubeDX11::~TextureCubeDX11()
    {
        destroyTexture();
    }

    //////////////////////////////////////////
    TextureCubeDX11Ptr TextureCubeDX11::Create(RenderSystemDX11* _renderSystem)
    {
        TextureCubeDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(TextureCubeDX11, object, init((RenderSystem*)_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::init(RenderSystem* _renderSystem)
    {
        if (!TextureCube::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* TextureCubeDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystem->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::isValid()
    {
        return (m_texture != nullptr);
    }

    //////////////////////////////////////////
    void TextureCubeDX11::destroyTexture()
    {
        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();
        if (renderSystem && renderSystem->getStateMachine() && m_shaderResourceView)
            renderSystem->getStateMachine()->unbindShaderResource(m_shaderResourceView);

        SafeReleaseDX11(m_shaderResourceView);
        SafeReleaseDX11(m_texture);
        m_hasMipmapsGenerationSupport = false;
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::loadTexture(
        Vector<PixelSheet2D> const _pixelSheets[6],
        PixelFormat::Enum _internalPixelFormat)
    {
        for (Size face = 0; face < 6; ++face)
            MAZE_ERROR_RETURN_VALUE_IF(_pixelSheets[face].empty(), false, "Cube face %d has no pixel sheets!", (S32)face);

        if (_internalPixelFormat == PixelFormat::None)
            _internalPixelFormat = _pixelSheets[0][0].getFormat();

        // DXGI has no 24-bit RGB - expand to RGBA
        bool expandRGB = (_internalPixelFormat == PixelFormat::RGB_U8);

        DXGI_FORMAT resourceFormat = GetPixelFormatResourceDX11(_internalPixelFormat);
        MAZE_ERROR_RETURN_VALUE_IF(
            resourceFormat == DXGI_FORMAT_UNKNOWN,
            false,
            "Unsupported texture pixel format: %d!",
            (S32)_internalPixelFormat);

        destroyTexture();

        ID3D11Device* device = getRenderSystemDX11Raw()->getDevice();
        ID3D11DeviceContext* deviceContext = getRenderSystemDX11Raw()->getDeviceContext();

        Vec2S size = _pixelSheets[0][0].getSize();
        m_size = size;
        m_internalPixelFormat = _internalPixelFormat;

        bool isCompressed = PixelFormat::IsCompressed(_internalPixelFormat);
        m_hasMipmapsGenerationSupport = !isCompressed;

        UINT mipLevels = (UINT)_pixelSheets[0].size();

        D3D11_TEXTURE2D_DESC textureDesc;
        memset(&textureDesc, 0, sizeof(textureDesc));
        textureDesc.Width = (UINT)size.x;
        textureDesc.Height = (UINT)size.y;
        textureDesc.MipLevels = mipLevels;
        textureDesc.ArraySize = 6;
        textureDesc.Format = resourceFormat;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        if (m_hasMipmapsGenerationSupport)
        {
            textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
            textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
        }

        HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateTexture2D (Cube) failed! hr=0x%08x", (U32)hr);

        for (Size face = 0; face < 6; ++face)
        {
            for (Size mip = 0; mip < _pixelSheets[face].size() && mip < (Size)mipLevels; ++mip)
            {
                PixelSheet2D const& pixelSheet = _pixelSheets[face][mip];
                if (pixelSheet.getTotalBytesCount() == 0)
                    continue;

                U8 const* data = pixelSheet.getDataRO();
                UINT rowPitch = (UINT)pixelSheet.getBytesPerRow();

                Vector<U8> expandedData;
                if (expandRGB)
                {
                    Size pixelsCount = (Size)pixelSheet.getSize().x * (Size)pixelSheet.getSize().y;
                    expandedData.resize(pixelsCount * 4);
                    ExpandRGBToRGBADX11(data, &expandedData[0], pixelsCount);
                    data = &expandedData[0];
                    rowPitch = (UINT)pixelSheet.getSize().x * 4u;
                }

                if (isCompressed)
                {
                    U32 blockSize = (m_internalPixelFormat == PixelFormat::DXT1_RGB || m_internalPixelFormat == PixelFormat::RGTC1_R) ? 8u : 16u;
                    U32 blocksPerRow = Math::Max(1, (pixelSheet.getSize().x + 3) / 4);
                    rowPitch = blocksPerRow * blockSize;
                }

                UINT subresource = D3D11CalcSubresource((UINT)mip, (UINT)face, mipLevels);
                deviceContext->UpdateSubresource(
                    m_texture,
                    subresource,
                    nullptr,
                    data,
                    rowPitch,
                    0);
            }
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        memset(&srvDesc, 0, sizeof(srvDesc));
        srvDesc.Format = GetPixelFormatSRVDX11(_internalPixelFormat);
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        srvDesc.TextureCube.MostDetailedMip = 0;
        srvDesc.TextureCube.MipLevels = (UINT)-1;

        hr = device->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateShaderResourceView (Cube) failed! hr=0x%08x", (U32)hr);

        return true;
    }

    //////////////////////////////////////////
    ID3D11SamplerState* TextureCubeDX11::ensureSamplerState()
    {
        return getRenderSystemDX11Raw()->ensureSamplerState(
            m_minFilter,
            m_magFilter,
            m_wrapS,
            m_wrapT,
            m_wrapR,
            0.0f,
            Vec4F::c_zero);
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::setMagFilter(TextureFilter _value)
    {
        m_magFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::setMinFilter(TextureFilter _value)
    {
        m_minFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::setWrapS(TextureWrap _value)
    {
        m_wrapS = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::setWrapT(TextureWrap _value)
    {
        m_wrapT = _value;
        return true;
    }

    //////////////////////////////////////////
    bool TextureCubeDX11::setWrapR(TextureWrap _value)
    {
        m_wrapR = _value;
        return true;
    }

    //////////////////////////////////////////
    void TextureCubeDX11::generateMipmaps()
    {
        if (!m_shaderResourceView)
            return;

        MAZE_ERROR_RETURN_IF(
            !m_hasMipmapsGenerationSupport,
            "Mipmaps generation is not supported for this texture format: %d!",
            (S32)m_internalPixelFormat);

        getRenderSystemDX11Raw()->getDeviceContext()->GenerateMips(m_shaderResourceView);
    }

    //////////////////////////////////////////
    void TextureCubeDX11::reload()
    {
        MAZE_NOT_IMPLEMENTED;
    }


} // namespace Maze
//////////////////////////////////////////
