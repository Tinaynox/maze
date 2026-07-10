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
#include "maze-render-system-dx11/MazeTexture2DMSDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeHelpersDX11.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class Texture2DMSDX11
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(Texture2DMSDX11, Texture2DMS);

    //////////////////////////////////////////
    Texture2DMSDX11::Texture2DMSDX11()
    {
    }

    //////////////////////////////////////////
    Texture2DMSDX11::~Texture2DMSDX11()
    {
        destroyTexture();
    }

    //////////////////////////////////////////
    Texture2DMSDX11Ptr Texture2DMSDX11::Create(RenderSystemDX11* _renderSystem)
    {
        Texture2DMSDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(Texture2DMSDX11, object, init((RenderSystem*)_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    bool Texture2DMSDX11::init(RenderSystem* _renderSystem)
    {
        if (!Texture2DMS::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* Texture2DMSDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystem->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    bool Texture2DMSDX11::isValid()
    {
        return (m_texture != nullptr);
    }

    //////////////////////////////////////////
    void Texture2DMSDX11::destroyTexture()
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
    }

    //////////////////////////////////////////
    bool Texture2DMSDX11::loadEmpty(
        Vec2U const& _size,
        PixelFormat::Enum _internalPixelFormat,
        S32 _samples)
    {
        MAZE_ERROR_RETURN_VALUE_IF(_size.x == 0 || _size.y == 0, false, "Invalid texture size!");

        DXGI_FORMAT resourceFormat = GetPixelFormatResourceDX11(_internalPixelFormat);
        MAZE_ERROR_RETURN_VALUE_IF(
            resourceFormat == DXGI_FORMAT_UNKNOWN,
            false,
            "Unsupported texture pixel format: %d!",
            (S32)_internalPixelFormat);

        destroyTexture();

        ID3D11Device* device = getRenderSystemDX11Raw()->getDevice();

        m_size = Vec2S((S32)_size.x, (S32)_size.y);
        m_internalPixelFormat = _internalPixelFormat;
        m_samples = Math::Max(_samples, 1);

        bool isDepth = IsDepthPixelFormatDX11(_internalPixelFormat);

        // Clamp the sample count to what the device supports for this format
        UINT qualityLevels = 0;
        while (m_samples > 1)
        {
            if (SUCCEEDED(device->CheckMultisampleQualityLevels(resourceFormat, (UINT)m_samples, &qualityLevels)) &&
                qualityLevels > 0)
                break;
            m_samples >>= 1;
        }

        D3D11_TEXTURE2D_DESC textureDesc;
        memset(&textureDesc, 0, sizeof(textureDesc));
        textureDesc.Width = (UINT)_size.x;
        textureDesc.Height = (UINT)_size.y;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = resourceFormat;
        textureDesc.SampleDesc.Count = (UINT)m_samples;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = isDepth ? D3D11_BIND_DEPTH_STENCIL : (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);

        HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateTexture2D (MS) failed! hr=0x%08x", (U32)hr);

        if (!isDepth)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            memset(&srvDesc, 0, sizeof(srvDesc));
            srvDesc.Format = GetPixelFormatSRVDX11(_internalPixelFormat);
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;

            hr = device->CreateShaderResourceView(m_texture, &srvDesc, &m_shaderResourceView);
            MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateShaderResourceView (MS) failed! hr=0x%08x", (U32)hr);
        }

        return true;
    }

    //////////////////////////////////////////
    ID3D11RenderTargetView* Texture2DMSDX11::ensureRenderTargetView()
    {
        if (m_renderTargetView)
            return m_renderTargetView;

        if (!m_texture)
            return nullptr;

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        memset(&rtvDesc, 0, sizeof(rtvDesc));
        rtvDesc.Format = GetPixelFormatDX11(m_internalPixelFormat);
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

        HRESULT hr = getRenderSystemDX11Raw()->getDevice()->CreateRenderTargetView(m_texture, &rtvDesc, &m_renderTargetView);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateRenderTargetView (MS) failed! hr=0x%08x", (U32)hr);

        return m_renderTargetView;
    }

    //////////////////////////////////////////
    ID3D11DepthStencilView* Texture2DMSDX11::ensureDepthStencilView()
    {
        if (m_depthStencilView)
            return m_depthStencilView;

        if (!m_texture)
            return nullptr;

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        memset(&dsvDesc, 0, sizeof(dsvDesc));
        dsvDesc.Format = GetPixelFormatDX11(m_internalPixelFormat);
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

        HRESULT hr = getRenderSystemDX11Raw()->getDevice()->CreateDepthStencilView(m_texture, &dsvDesc, &m_depthStencilView);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateDepthStencilView (MS) failed! hr=0x%08x", (U32)hr);

        return m_depthStencilView;
    }

    //////////////////////////////////////////
    void Texture2DMSDX11::copyImageFrom(
        U8 const* _pixels,
        PixelFormat::Enum _pixelFormat,
        U32 _width,
        U32 _height,
        U32 _x,
        U32 _y)
    {
        MAZE_ERROR("Copying pixel data into a multisampled texture is not supported!");
    }

    //////////////////////////////////////////
    void Texture2DMSDX11::saveToFileAsTGA(String const& _fileName, Vec2U _size)
    {
        MAZE_NOT_IMPLEMENTED;
    }

    //////////////////////////////////////////
    PixelSheet2D Texture2DMSDX11::readAsPixelSheet(PixelFormat::Enum _outputFormat)
    {
        MAZE_NOT_IMPLEMENTED;
        return PixelSheet2D();
    }


} // namespace Maze
//////////////////////////////////////////
