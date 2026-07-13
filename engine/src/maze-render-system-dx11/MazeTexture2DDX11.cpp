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
#include "maze-render-system-dx11/MazeTexture2DDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeHelpersDX11.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"
#include "maze-graphics/helpers/MazePixelSheet2DHelper.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Texture2DDX11
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DDX11, Texture2D);

    //////////////////////////////////////////
    Texture2DDX11::Texture2DDX11()
    {
    }

    //////////////////////////////////////////
    Texture2DDX11::~Texture2DDX11()
    {
        destroyTexture();
    }

    //////////////////////////////////////////
    Texture2DDX11Ptr Texture2DDX11::Create(RenderSystemDX11* _renderSystem)
    {
        Texture2DDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(Texture2DDX11, object, init((RenderSystem*)_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DDX11::init(RenderSystem* _renderSystem)
    {
        if (!Texture2D::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* Texture2DDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystem->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    bool Texture2DDX11::isValid()
    {
        return (m_texture != nullptr);
    }

    //////////////////////////////////////////
    void Texture2DDX11::destroyTexture()
    {
        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();
        if (renderSystem && renderSystem->getStateMachine())
        {
            if (m_shaderResourceView)
                renderSystem->getStateMachine()->unbindShaderResource(m_shaderResourceView);

            if (m_renderTargetView)
                renderSystem->getStateMachine()->processRenderTargetViewsDestroyed(&m_renderTargetView, 1);

            if (m_depthStencilView)
                renderSystem->getStateMachine()->processDepthStencilViewDestroyed(m_depthStencilView);
        }

        SafeReleaseDX11(m_depthStencilView);
        SafeReleaseDX11(m_renderTargetView);
        SafeReleaseDX11(m_shaderResourceView);
        SafeReleaseDX11(m_texture);
        m_mipLevels = 1u;
        m_hasMipmapsGenerationSupport = false;
    }

    //////////////////////////////////////////
    bool Texture2DDX11::loadTextureImpl(
        Vector<PixelSheet2D> const& _pixelSheets,
        PixelFormat::Enum _internalPixelFormat)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_pixelSheets.empty(), false, "PixelSheets are empty!");

        if (_internalPixelFormat == PixelFormat::None)
            _internalPixelFormat = _pixelSheets[0].getFormat();

        // DXGI has no 24-bit RGB - expand to RGBA
        bool expandRGB = (_internalPixelFormat == PixelFormat::RGB_U8);

        DXGI_FORMAT resourceFormat = GetPixelFormatResourceDX11(_internalPixelFormat);
        DXGI_FORMAT srvFormat = GetPixelFormatSRVDX11(_internalPixelFormat);
        MAZE_ERROR_RETURN_VALUE_IF(
            resourceFormat == DXGI_FORMAT_UNKNOWN,
            false,
            "Unsupported texture pixel format: %d!",
            (S32)_internalPixelFormat);

        destroyTexture();

        ID3D11Device* device = getRenderSystemDX11Raw()->getDevice();
        ID3D11DeviceContext* deviceContext = getRenderSystemDX11Raw()->getDeviceContext();

        Vec2S size = _pixelSheets[0].getSize();
        m_size = size;
        m_invSize = Vec2F(1.0f / (F32)size.x, 1.0f / (F32)size.y);
        m_internalPixelFormat = _internalPixelFormat;

        bool isDepth = IsDepthPixelFormatDX11(_internalPixelFormat);
        bool isCompressed = PixelFormat::IsCompressed(_internalPixelFormat);

        m_hasMipmapsGenerationSupport = !isDepth && !isCompressed;

        D3D11_TEXTURE2D_DESC textureDesc;
        memset(&textureDesc, 0, sizeof(textureDesc));
        textureDesc.Width = (UINT)size.x;
        textureDesc.Height = (UINT)size.y;
        textureDesc.ArraySize = 1;
        textureDesc.Format = resourceFormat;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        if (isDepth)
        {
            textureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
            textureDesc.MipLevels = 1;
            m_mipLevels = 1u;
        }
        else
        if (m_hasMipmapsGenerationSupport)
        {
            textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
            textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

            if (_pixelSheets.size() > 1)
            {
                textureDesc.MipLevels = (UINT)_pixelSheets.size();
                m_mipLevels = (U32)_pixelSheets.size();
            }
            else
            {
                // Allocate the full mip chain so mipmaps can be generated later
                textureDesc.MipLevels = 0;
                m_mipLevels = 1u;
                for (S32 s = Math::Max(size.x, size.y); s > 1; s >>= 1)
                    ++m_mipLevels;
            }
        }
        else
        {
            textureDesc.MipLevels = (UINT)_pixelSheets.size();
            m_mipLevels = (U32)_pixelSheets.size();
        }

        HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateTexture2D failed! hr=0x%08x", (U32)hr);

        // Upload pixel data
        for (Size mip = 0; mip < _pixelSheets.size(); ++mip)
        {
            PixelSheet2D const& pixelSheet = _pixelSheets[mip];

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
                // Block-compressed row pitch: bytes per row of 4x4 blocks
                U32 blockSize = (m_internalPixelFormat == PixelFormat::DXT1_RGB || m_internalPixelFormat == PixelFormat::RGTC1_R) ? 8u : 16u;
                U32 blocksPerRow = Math::Max(1, (pixelSheet.getSize().x + 3) / 4);
                rowPitch = blocksPerRow * blockSize;
            }

            deviceContext->UpdateSubresource(
                m_texture,
                (UINT)mip,
                nullptr,
                data,
                rowPitch,
                0);
        }

        // Shader resource view
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        memset(&srvDesc, 0, sizeof(srvDesc));
        srvDesc.Format = srvFormat;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = (UINT)-1;

        hr = device->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateShaderResourceView failed! hr=0x%08x", (U32)hr);

        // The full mip chain was allocated but only level 0 was uploaded - fill the rest
        if (m_hasMipmapsGenerationSupport && _pixelSheets.size() == 1 && m_mipLevels > 1u)
            deviceContext->GenerateMips(m_shaderResourceView);

        return true;
    }

    //////////////////////////////////////////
    ID3D11RenderTargetView* Texture2DDX11::ensureRenderTargetView()
    {
        if (m_renderTargetView)
            return m_renderTargetView;

        if (!m_texture)
            return nullptr;

        MAZE_ERROR_RETURN_VALUE_IF(
            IsDepthPixelFormatDX11(m_internalPixelFormat),
            nullptr,
            "Depth texture cannot be used as a render target view!");

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        memset(&rtvDesc, 0, sizeof(rtvDesc));
        rtvDesc.Format = GetPixelFormatDX11(m_internalPixelFormat);
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtvDesc.Texture2D.MipSlice = 0;

        HRESULT hr = getRenderSystemDX11Raw()->getDevice()->CreateRenderTargetView(m_texture, &rtvDesc, &m_renderTargetView);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateRenderTargetView failed! hr=0x%08x", (U32)hr);

        return m_renderTargetView;
    }

    //////////////////////////////////////////
    ID3D11DepthStencilView* Texture2DDX11::ensureDepthStencilView()
    {
        if (m_depthStencilView)
            return m_depthStencilView;

        if (!m_texture)
            return nullptr;

        MAZE_ERROR_RETURN_VALUE_IF(
            !IsDepthPixelFormatDX11(m_internalPixelFormat),
            nullptr,
            "Color texture cannot be used as a depth stencil view!");

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        memset(&dsvDesc, 0, sizeof(dsvDesc));
        dsvDesc.Format = GetPixelFormatDX11(m_internalPixelFormat);
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvDesc.Texture2D.MipSlice = 0;

        HRESULT hr = getRenderSystemDX11Raw()->getDevice()->CreateDepthStencilView(m_texture, &dsvDesc, &m_depthStencilView);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateDepthStencilView failed! hr=0x%08x", (U32)hr);

        return m_depthStencilView;
    }

    //////////////////////////////////////////
    ID3D11SamplerState* Texture2DDX11::ensureSamplerState()
    {
        return getRenderSystemDX11Raw()->ensureSamplerState(
            m_minFilter,
            m_magFilter,
            m_wrapS,
            m_wrapT,
            m_wrapT,
            m_anisotropyLevel,
            m_borderColor.toVec4F32());
    }

    //////////////////////////////////////////
    bool Texture2DDX11::setMagFilter(TextureFilter _value)
    {
        m_magFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DDX11::setMinFilter(TextureFilter _value)
    {
        m_minFilter = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DDX11::setWrapS(TextureWrap _value)
    {
        m_wrapS = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DDX11::setWrapT(TextureWrap _value)
    {
        m_wrapT = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DDX11::setBorderColor(ColorU32 _value)
    {
        m_borderColor = _value;
        return true;
    }

    //////////////////////////////////////////
    bool Texture2DDX11::setAnisotropyLevel(F32 _value)
    {
        m_anisotropyLevel = _value;
        return true;
    }

    //////////////////////////////////////////
    void Texture2DDX11::copyImageFrom(
        Texture2DPtr const& _texture,
        U32 _x,
        U32 _y)
    {
        MAZE_ERROR_RETURN_IF(!_texture, "Texture is null!");

        Texture2DDX11* srcTexture = _texture->castRaw<Texture2DDX11>();
        MAZE_ERROR_RETURN_IF(!srcTexture->m_texture || !m_texture, "Textures are not loaded!");

        getRenderSystemDX11Raw()->getDeviceContext()->CopySubresourceRegion(
            m_texture, 0,
            (UINT)_x, (UINT)_y, 0,
            srcTexture->m_texture, 0,
            nullptr);
    }

    //////////////////////////////////////////
    void Texture2DDX11::copyImageFrom(
        U8 const* _pixels,
        PixelFormat::Enum _pixelFormat,
        U32 _width,
        U32 _height,
        U32 _x,
        U32 _y)
    {
        MAZE_ERROR_RETURN_IF(!m_texture, "Texture is not loaded!");
        MAZE_ERROR_RETURN_IF(
            _pixelFormat != m_internalPixelFormat,
            "Pixel format mismatch! texture=%d data=%d",
            (S32)m_internalPixelFormat,
            (S32)_pixelFormat);

        U8 const* data = _pixels;
        UINT rowPitch = PixelFormat::GetBytesPerPixel(_pixelFormat) * _width;

        Vector<U8> expandedData;
        if (_pixelFormat == PixelFormat::RGB_U8)
        {
            Size pixelsCount = (Size)_width * (Size)_height;
            expandedData.resize(pixelsCount * 4);
            ExpandRGBToRGBADX11(data, &expandedData[0], pixelsCount);
            data = &expandedData[0];
            rowPitch = _width * 4u;
        }

        D3D11_BOX box;
        box.left = (UINT)_x;
        box.right = (UINT)(_x + _width);
        box.top = (UINT)_y;
        box.bottom = (UINT)(_y + _height);
        box.front = 0;
        box.back = 1;

        getRenderSystemDX11Raw()->getDeviceContext()->UpdateSubresource(
            m_texture,
            0,
            &box,
            data,
            rowPitch,
            0);
    }

    //////////////////////////////////////////
    bool Texture2DDX11::readAsPixelSheet(
        PixelSheet2D& _outResult,
        PixelFormat::Enum _outputFormat)
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_texture, false, "Texture is not loaded!");

        if (_outputFormat == PixelFormat::None)
            _outputFormat = m_internalPixelFormat;

        MAZE_ERROR_RETURN_VALUE_IF(
            _outputFormat != m_internalPixelFormat,
            false,
            "DX11 readAsPixelSheet format conversion is not supported! texture=%d requested=%d",
            (S32)m_internalPixelFormat,
            (S32)_outputFormat);

        ID3D11Device* device = getRenderSystemDX11Raw()->getDevice();
        ID3D11DeviceContext* deviceContext = getRenderSystemDX11Raw()->getDeviceContext();

        D3D11_TEXTURE2D_DESC textureDesc;
        m_texture->GetDesc(&textureDesc);
        textureDesc.MipLevels = 1;
        textureDesc.Usage = D3D11_USAGE_STAGING;
        textureDesc.BindFlags = 0;
        textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        textureDesc.MiscFlags = 0;

        ID3D11Texture2D* stagingTexture = nullptr;
        HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &stagingTexture);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "Staging texture creation failed! hr=0x%08x", (U32)hr);

        deviceContext->CopySubresourceRegion(stagingTexture, 0, 0, 0, 0, m_texture, 0, nullptr);

        D3D11_MAPPED_SUBRESOURCE mapped;
        hr = deviceContext->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &mapped);
        if (FAILED(hr))
        {
            stagingTexture->Release();
            MAZE_ERROR_RETURN_VALUE(false, "Staging texture map failed! hr=0x%08x", (U32)hr);
        }

        _outResult.setFormat(_outputFormat);
        _outResult.setSize(m_size);

        // 24-bit RGB textures are stored as RGBA in the GPU resource (see loadTextureImpl)
        bool contractRGBA = (m_internalPixelFormat == PixelFormat::RGB_U8);

        U32 bytesPerRow = (U32)_outResult.getBytesPerRow();
        U8* dstData = _outResult.getDataRW();
        for (S32 r = 0; r < m_size.y; ++r)
        {
            U8 const* srcRow = (U8 const*)mapped.pData + (Size)r * mapped.RowPitch;
            U8* dstRow = dstData + (Size)r * bytesPerRow;

            if (contractRGBA)
            {
                for (S32 x = 0; x < m_size.x; ++x)
                {
                    dstRow[x * 3 + 0] = srcRow[x * 4 + 0];
                    dstRow[x * 3 + 1] = srcRow[x * 4 + 1];
                    dstRow[x * 3 + 2] = srcRow[x * 4 + 2];
                }
            }
            else
            {
                memcpy(dstRow, srcRow, bytesPerRow);
            }
        }

        deviceContext->Unmap(stagingTexture, 0);
        stagingTexture->Release();

        return true;
    }

    //////////////////////////////////////////
    void Texture2DDX11::saveToFileAsTGA(
        String const& _fileName,
        Vec2U _size,
        bool _resetAlpha)
    {
        PixelSheet2D pixelSheet;
        if (!readAsPixelSheet(pixelSheet))
            return;

        if (_size != Vec2U::c_zero && Vec2S((S32)_size.x, (S32)_size.y) != pixelSheet.getSize())
        {
            Vec2S cropSize(
                Math::Min((S32)_size.x, pixelSheet.getSize().x),
                Math::Min((S32)_size.y, pixelSheet.getSize().y));

            PixelSheet2D croppedSheet(cropSize, pixelSheet.getFormat());
            for (S32 r = 0; r < cropSize.y; ++r)
            {
                memcpy(
                    croppedSheet.getDataRW() + (Size)r * croppedSheet.getBytesPerRow(),
                    pixelSheet.getDataRO() + (Size)r * pixelSheet.getBytesPerRow(),
                    croppedSheet.getBytesPerRow());
            }

            PixelSheet2DHelper::SaveTGA(croppedSheet, _fileName.c_str(), _resetAlpha);
            return;
        }

        PixelSheet2DHelper::SaveTGA(pixelSheet, _fileName.c_str(), _resetAlpha);
    }

    //////////////////////////////////////////
    void Texture2DDX11::generateMipmaps()
    {
        if (!m_shaderResourceView)
            return;

        MAZE_ERROR_RETURN_IF(
            !m_hasMipmapsGenerationSupport,
            "Mipmaps generation is not supported for this texture format: %d!",
            (S32)m_internalPixelFormat);

        getRenderSystemDX11Raw()->getDeviceContext()->GenerateMips(m_shaderResourceView);
    }


} // namespace Maze
//////////////////////////////////////////
