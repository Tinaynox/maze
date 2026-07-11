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
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class StateMachineDX11
    //
    //////////////////////////////////////////
    StateMachineDX11::StateMachineDX11(RenderSystemDX11* _renderSystem)
        : m_renderSystem(_renderSystem)
    {
        memset(m_renderTargetViews, 0, sizeof(m_renderTargetViews));
        memset(m_pixelShaderResources, 0, sizeof(m_pixelShaderResources));
        memset(m_pixelSamplers, 0, sizeof(m_pixelSamplers));
        memset(m_vertexShaderResources, 0, sizeof(m_vertexShaderResources));
        memset(m_vertexSamplers, 0, sizeof(m_vertexSamplers));
    }

    //////////////////////////////////////////
    StateMachineDX11::~StateMachineDX11()
    {
        for (auto& stateData : m_blendStates)
            SafeReleaseDX11(stateData.second);
        m_blendStates.clear();

        for (auto& stateData : m_depthStencilStates)
            SafeReleaseDX11(stateData.second);
        m_depthStencilStates.clear();

        for (auto& stateData : m_rasterizerStates)
            SafeReleaseDX11(stateData.second);
        m_rasterizerStates.clear();
    }

    //////////////////////////////////////////
    ID3D11Device* StateMachineDX11::getDevice() const
    {
        return m_renderSystem->getDevice();
    }

    //////////////////////////////////////////
    ID3D11DeviceContext* StateMachineDX11::getDeviceContext() const
    {
        return m_renderSystem->getDeviceContext();
    }

    //////////////////////////////////////////
    void StateMachineDX11::setBlendEnabled(bool _value)
    {
        if (m_blendEnabled == _value)
            return;

        m_blendEnabled = _value;
        m_blendStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setBlendFactors(BlendFactor _srcFactor, BlendFactor _destFactor)
    {
        if (m_blendSrcFactor == _srcFactor && m_blendDestFactor == _destFactor)
            return;

        m_blendSrcFactor = _srcFactor;
        m_blendDestFactor = _destFactor;
        m_blendStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setDepthTestEnabled(bool _value)
    {
        if (m_depthTestEnabled == _value)
            return;

        m_depthTestEnabled = _value;
        m_depthStencilStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setDepthTestCompareFunction(CompareFunction _value)
    {
        if (m_depthTestCompareFunction == _value)
            return;

        m_depthTestCompareFunction = _value;
        m_depthStencilStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setDepthWriteEnabled(bool _value)
    {
        if (m_depthWriteEnabled == _value)
            return;

        m_depthWriteEnabled = _value;
        m_depthStencilStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setCullEnabled(bool _value)
    {
        if (m_cullEnabled == _value)
            return;

        m_cullEnabled = _value;
        m_rasterizerStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setCullMode(CullMode _value)
    {
        if (m_cullMode == _value)
            return;

        m_cullMode = _value;
        m_rasterizerStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setStencilTestEnabled(bool _value)
    {
        if (m_stencilTestEnabled == _value)
            return;

        m_stencilTestEnabled = _value;
        m_depthStencilStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setStencilFunc(CompareFunction _function, S32 _referenceValue, U32 _readMask)
    {
        if (m_stencilTestCompareFunction == _function &&
            m_stencilReferenceValue == _referenceValue &&
            m_stencilReadMask == _readMask)
            return;

        m_stencilTestCompareFunction = _function;
        m_stencilReferenceValue = _referenceValue;
        m_stencilReadMask = _readMask;
        m_depthStencilStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setStencilOp(StencilOperation _fail, StencilOperation _depthFail, StencilOperation _pass)
    {
        if (m_stencilFailOperation == _fail &&
            m_stencilDepthFailOperation == _depthFail &&
            m_stencilPassOperation == _pass)
            return;

        m_stencilFailOperation = _fail;
        m_stencilDepthFailOperation = _depthFail;
        m_stencilPassOperation = _pass;
        m_depthStencilStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setStencilWriteMask(U32 _mask)
    {
        if (m_stencilWriteMask == _mask)
            return;

        m_stencilWriteMask = _mask;
        m_depthStencilStateDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setScissorTestEnabled(bool _value)
    {
        if (m_scissorTestEnabled == _value)
            return;

        m_scissorTestEnabled = _value;
        m_rasterizerStateDirty = true;
        m_scissorDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setScissorRect(Rect2S const& _rect)
    {
        if (m_scissorRect == _rect)
            return;

        m_scissorRect = _rect;
        m_scissorDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setViewportRect(Rect2S const& _rect)
    {
        if (m_viewportRect == _rect)
            return;

        m_viewportRect = _rect;
        m_viewportDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::setCurrentShader(ShaderDX11* _shader)
    {
        if (m_currentShader == _shader)
            return;

        m_currentShader = _shader;

        ID3D11DeviceContext* deviceContext = getDeviceContext();
        if (_shader)
        {
            deviceContext->VSSetShader(_shader->getVertexShader(), nullptr, 0);
            deviceContext->PSSetShader(_shader->getPixelShader(), nullptr, 0);
        }
        else
        {
            deviceContext->VSSetShader(nullptr, nullptr, 0);
            deviceContext->PSSetShader(nullptr, nullptr, 0);
        }
    }

    //////////////////////////////////////////
    void StateMachineDX11::bindRenderTarget(
        ID3D11RenderTargetView* const* _renderTargetViews,
        S32 _renderTargetViewsCount,
        ID3D11DepthStencilView* _depthStencilView,
        Vec2U const& _renderTargetSize,
        bool _flipY)
    {
        MAZE_DEBUG_ERROR_IF(_renderTargetViewsCount > c_renderTargetsMax, "Too many render targets!");

        m_renderTargetViewsCount = Math::Min(_renderTargetViewsCount, c_renderTargetsMax);
        for (S32 i = 0; i < m_renderTargetViewsCount; ++i)
            m_renderTargetViews[i] = _renderTargetViews[i];
        for (S32 i = m_renderTargetViewsCount; i < c_renderTargetsMax; ++i)
            m_renderTargetViews[i] = nullptr;

        m_depthStencilView = _depthStencilView;
        m_renderTargetSize = _renderTargetSize;

        if (m_flipY != _flipY)
        {
            m_flipY = _flipY;
            m_rasterizerStateDirty = true;
        }

        // Unbind the new render targets from shader resource slots first
        for (S32 i = 0; i < m_renderTargetViewsCount; ++i)
        {
            if (!m_renderTargetViews[i])
                continue;

            ID3D11Resource* resource = nullptr;
            m_renderTargetViews[i]->GetResource(&resource);
            if (resource)
            {
                for (S32 s = 0; s < c_shaderResourcesMax; ++s)
                {
                    if (m_pixelShaderResources[s])
                    {
                        ID3D11Resource* srvResource = nullptr;
                        m_pixelShaderResources[s]->GetResource(&srvResource);
                        if (srvResource == resource)
                            bindPixelShaderResource(s, nullptr, -1, nullptr);
                        if (srvResource)
                            srvResource->Release();
                    }
                }
                resource->Release();
            }
        }

        getDeviceContext()->OMSetRenderTargets(
            (UINT)m_renderTargetViewsCount,
            m_renderTargetViewsCount > 0 ? m_renderTargetViews : nullptr,
            m_depthStencilView);

        m_viewportDirty = true;
        m_scissorDirty = true;
    }

    //////////////////////////////////////////
    void StateMachineDX11::unbindRenderTarget()
    {
        for (S32 i = 0; i < c_renderTargetsMax; ++i)
            m_renderTargetViews[i] = nullptr;
        m_renderTargetViewsCount = 0;
        m_depthStencilView = nullptr;

        getDeviceContext()->OMSetRenderTargets(0, nullptr, nullptr);
    }

    //////////////////////////////////////////
    void StateMachineDX11::processRenderTargetViewsDestroyed(ID3D11RenderTargetView* const* _renderTargetViews, S32 _count)
    {
        bool bound = false;
        for (S32 i = 0; i < _count; ++i)
        {
            for (S32 j = 0; j < m_renderTargetViewsCount; ++j)
            {
                if (m_renderTargetViews[j] == _renderTargetViews[i])
                {
                    bound = true;
                    break;
                }
            }
        }

        if (bound)
            unbindRenderTarget();
    }

    //////////////////////////////////////////
    void StateMachineDX11::processDepthStencilViewDestroyed(ID3D11DepthStencilView* _depthStencilView)
    {
        if (m_depthStencilView == _depthStencilView)
            unbindRenderTarget();
    }

    //////////////////////////////////////////
    void StateMachineDX11::bindPixelShaderResource(S32 _srvSlot, ID3D11ShaderResourceView* _srv, S32 _samplerSlot, ID3D11SamplerState* _sampler)
    {
        if (_srvSlot >= 0 && _srvSlot < c_shaderResourcesMax)
        {
            if (m_pixelShaderResources[_srvSlot] != _srv)
            {
                m_pixelShaderResources[_srvSlot] = _srv;
                getDeviceContext()->PSSetShaderResources((UINT)_srvSlot, 1, &_srv);
            }
        }

        if (_samplerSlot >= 0 && _samplerSlot < c_shaderResourcesMax)
        {
            if (_sampler && m_pixelSamplers[_samplerSlot] != _sampler)
            {
                m_pixelSamplers[_samplerSlot] = _sampler;
                getDeviceContext()->PSSetSamplers((UINT)_samplerSlot, 1, &_sampler);
            }
        }
    }

    //////////////////////////////////////////
    void StateMachineDX11::bindVertexShaderResource(S32 _srvSlot, ID3D11ShaderResourceView* _srv, S32 _samplerSlot, ID3D11SamplerState* _sampler)
    {
        if (_srvSlot >= 0 && _srvSlot < c_shaderResourcesMax)
        {
            if (m_vertexShaderResources[_srvSlot] != _srv)
            {
                m_vertexShaderResources[_srvSlot] = _srv;
                getDeviceContext()->VSSetShaderResources((UINT)_srvSlot, 1, &_srv);
            }
        }

        if (_samplerSlot >= 0 && _samplerSlot < c_shaderResourcesMax)
        {
            if (_sampler && m_vertexSamplers[_samplerSlot] != _sampler)
            {
                m_vertexSamplers[_samplerSlot] = _sampler;
                getDeviceContext()->VSSetSamplers((UINT)_samplerSlot, 1, &_sampler);
            }
        }
    }

    //////////////////////////////////////////
    void StateMachineDX11::unbindShaderResource(ID3D11ShaderResourceView* _srv)
    {
        if (!_srv)
            return;

        for (S32 s = 0; s < c_shaderResourcesMax; ++s)
        {
            if (m_pixelShaderResources[s] == _srv)
                bindPixelShaderResource(s, nullptr, -1, nullptr);

            if (m_vertexShaderResources[s] == _srv)
                bindVertexShaderResource(s, nullptr, -1, nullptr);
        }
    }

    //////////////////////////////////////////
    void StateMachineDX11::invalidateDeviceState()
    {
        m_blendStateDirty = true;
        m_depthStencilStateDirty = true;
        m_rasterizerStateDirty = true;
        m_viewportDirty = true;
        m_scissorDirty = true;

        m_currentShader = nullptr;

        for (S32 s = 0; s < c_shaderResourcesMax; ++s)
        {
            m_pixelShaderResources[s] = nullptr;
            m_pixelSamplers[s] = nullptr;
            m_vertexShaderResources[s] = nullptr;
            m_vertexSamplers[s] = nullptr;
        }

        ID3D11ShaderResourceView* nullSRVs[c_shaderResourcesMax] = { nullptr };
        getDeviceContext()->PSSetShaderResources(0, (UINT)c_shaderResourcesMax, nullSRVs);
        getDeviceContext()->VSSetShaderResources(0, (UINT)c_shaderResourcesMax, nullSRVs);

        getDeviceContext()->OMSetRenderTargets(
            (UINT)m_renderTargetViewsCount,
            m_renderTargetViewsCount > 0 ? m_renderTargetViews : nullptr,
            m_depthStencilView);
    }

    //////////////////////////////////////////
    ID3D11BlendState* StateMachineDX11::ensureBlendState()
    {
        U32 key =
            (m_blendEnabled ? 1u : 0u) |
            ((U32)m_blendSrcFactor << 1) |
            ((U32)m_blendDestFactor << 9);

        auto it = m_blendStates.find(key);
        if (it != m_blendStates.end())
            return it->second;

        D3D11_BLEND_DESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;
        desc.RenderTarget[0].BlendEnable = m_blendEnabled ? TRUE : FALSE;
        desc.RenderTarget[0].SrcBlend = GetBlendFactorDX11(m_blendSrcFactor);
        desc.RenderTarget[0].DestBlend = GetBlendFactorDX11(m_blendDestFactor);
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].SrcBlendAlpha = GetBlendFactorDX11(m_blendSrcFactor);
        desc.RenderTarget[0].DestBlendAlpha = GetBlendFactorDX11(m_blendDestFactor);
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        // Color blend factors are not valid for the alpha channel
        if (desc.RenderTarget[0].SrcBlendAlpha == D3D11_BLEND_SRC_COLOR) desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        if (desc.RenderTarget[0].SrcBlendAlpha == D3D11_BLEND_INV_SRC_COLOR) desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        if (desc.RenderTarget[0].SrcBlendAlpha == D3D11_BLEND_DEST_COLOR) desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
        if (desc.RenderTarget[0].SrcBlendAlpha == D3D11_BLEND_INV_DEST_COLOR) desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
        if (desc.RenderTarget[0].DestBlendAlpha == D3D11_BLEND_SRC_COLOR) desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        if (desc.RenderTarget[0].DestBlendAlpha == D3D11_BLEND_INV_SRC_COLOR) desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        if (desc.RenderTarget[0].DestBlendAlpha == D3D11_BLEND_DEST_COLOR) desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
        if (desc.RenderTarget[0].DestBlendAlpha == D3D11_BLEND_INV_DEST_COLOR) desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;

        ID3D11BlendState* state = nullptr;
        HRESULT hr = getDevice()->CreateBlendState(&desc, &state);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateBlendState failed! hr=0x%08x", (U32)hr);

        m_blendStates.emplace(key, state);
        return state;
    }

    //////////////////////////////////////////
    ID3D11DepthStencilState* StateMachineDX11::ensureDepthStencilState()
    {
        U64 key =
            (U64)(m_depthTestEnabled ? 1u : 0u) |
            ((U64)m_depthTestCompareFunction << 1) |
            ((U64)(m_depthWriteEnabled ? 1u : 0u) << 5) |
            ((U64)(m_stencilTestEnabled ? 1u : 0u) << 6) |
            ((U64)m_stencilTestCompareFunction << 7) |
            ((U64)m_stencilFailOperation << 11) |
            ((U64)m_stencilDepthFailOperation << 15) |
            ((U64)m_stencilPassOperation << 19) |
            ((U64)(m_stencilReadMask & 0xFF) << 23) |
            ((U64)(m_stencilWriteMask & 0xFF) << 31);

        auto it = m_depthStencilStates.find(key);
        if (it != m_depthStencilStates.end())
            return it->second;

        D3D11_DEPTH_STENCIL_DESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.DepthEnable = m_depthTestEnabled ? TRUE : FALSE;
        desc.DepthWriteMask = m_depthWriteEnabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.DepthFunc = m_depthTestEnabled ? GetCompareFunctionDX11(m_depthTestCompareFunction) : D3D11_COMPARISON_ALWAYS;
        desc.StencilEnable = m_stencilTestEnabled ? TRUE : FALSE;
        desc.StencilReadMask = (UINT8)m_stencilReadMask;
        desc.StencilWriteMask = (UINT8)m_stencilWriteMask;
        desc.FrontFace.StencilFailOp = GetStencilOperationDX11(m_stencilFailOperation);
        desc.FrontFace.StencilDepthFailOp = GetStencilOperationDX11(m_stencilDepthFailOperation);
        desc.FrontFace.StencilPassOp = GetStencilOperationDX11(m_stencilPassOperation);
        desc.FrontFace.StencilFunc = GetCompareFunctionDX11(m_stencilTestCompareFunction);
        desc.BackFace = desc.FrontFace;

        ID3D11DepthStencilState* state = nullptr;
        HRESULT hr = getDevice()->CreateDepthStencilState(&desc, &state);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateDepthStencilState failed! hr=0x%08x", (U32)hr);

        m_depthStencilStates.emplace(key, state);
        return state;
    }

    //////////////////////////////////////////
    ID3D11RasterizerState* StateMachineDX11::ensureRasterizerState()
    {
        U32 key =
            (m_cullEnabled ? 1u : 0u) |
            ((U32)m_cullMode << 1) |
            ((m_scissorTestEnabled ? 1u : 0u) << 3) |
            ((m_flipY ? 1u : 0u) << 4);

        auto it = m_rasterizerStates.find(key);
        if (it != m_rasterizerStates.end())
            return it->second;

        D3D11_RASTERIZER_DESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.FillMode = D3D11_FILL_SOLID;

        if (m_cullEnabled)
            desc.CullMode = (m_cullMode == CullMode::Front) ? D3D11_CULL_FRONT : D3D11_CULL_BACK;
        else
            desc.CullMode = D3D11_CULL_NONE;

        // The GL backend runs with glFrontFace(GL_CW) (see StateMachineOpenGL::setup),
        // so Maze front faces are clockwise in screen space - same as the D3D11 default.
        // Rendering to offscreen targets flips clip-space Y, which reverses the winding
        desc.FrontCounterClockwise = m_flipY ? TRUE : FALSE;
        desc.DepthClipEnable = TRUE;
        desc.ScissorEnable = m_scissorTestEnabled ? TRUE : FALSE;
        desc.MultisampleEnable = TRUE;

        ID3D11RasterizerState* state = nullptr;
        HRESULT hr = getDevice()->CreateRasterizerState(&desc, &state);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateRasterizerState failed! hr=0x%08x", (U32)hr);

        m_rasterizerStates.emplace(key, state);
        return state;
    }

    //////////////////////////////////////////
    void StateMachineDX11::flushPipeline()
    {
        ID3D11DeviceContext* deviceContext = getDeviceContext();

        if (m_blendStateDirty)
        {
            static FLOAT const c_blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
            deviceContext->OMSetBlendState(ensureBlendState(), c_blendFactor, 0xFFFFFFFF);
            m_blendStateDirty = false;
        }

        if (m_depthStencilStateDirty)
        {
            deviceContext->OMSetDepthStencilState(ensureDepthStencilState(), (UINT)m_stencilReferenceValue);
            m_depthStencilStateDirty = false;
        }

        if (m_rasterizerStateDirty)
        {
            deviceContext->RSSetState(ensureRasterizerState());
            m_rasterizerStateDirty = false;
        }

        if (m_viewportDirty)
        {
            // Maze viewport rects are bottom-up (OpenGL convention).
            // When rendering flipped (offscreen), the image is upside down in DX terms,
            // so the viewport keeps the OpenGL origin
            D3D11_VIEWPORT viewport;
            viewport.TopLeftX = (FLOAT)m_viewportRect.position.x;
            if (m_flipY)
                viewport.TopLeftY = (FLOAT)m_viewportRect.position.y;
            else
                viewport.TopLeftY = (FLOAT)((S32)m_renderTargetSize.y - m_viewportRect.position.y - m_viewportRect.size.y);
            viewport.Width = (FLOAT)m_viewportRect.size.x;
            viewport.Height = (FLOAT)m_viewportRect.size.y;
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
            deviceContext->RSSetViewports(1, &viewport);
            m_viewportDirty = false;
        }

        if (m_scissorDirty)
        {
            D3D11_RECT rect;
            rect.left = (LONG)m_scissorRect.position.x;
            rect.right = (LONG)(m_scissorRect.position.x + m_scissorRect.size.x);
            if (m_flipY)
            {
                rect.top = (LONG)m_scissorRect.position.y;
                rect.bottom = (LONG)(m_scissorRect.position.y + m_scissorRect.size.y);
            }
            else
            {
                rect.top = (LONG)((S32)m_renderTargetSize.y - m_scissorRect.position.y - m_scissorRect.size.y);
                rect.bottom = (LONG)((S32)m_renderTargetSize.y - m_scissorRect.position.y);
            }
            deviceContext->RSSetScissorRects(1, &rect);
            m_scissorDirty = false;
        }
    }


} // namespace Maze
//////////////////////////////////////////
