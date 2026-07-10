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
#include "maze-render-system-dx11/MazeRenderWindowDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeRenderQueueDX11.hpp"
#include "maze-core/services/MazeLogStream.hpp"

#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
#   include "maze-core/system/win/MazeWindowWin.hpp"
#endif


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderWindowDX11
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderWindowDX11, RenderWindow);

    //////////////////////////////////////////
    RenderWindowDX11::RenderWindowDX11()
    {
    }

    //////////////////////////////////////////
    RenderWindowDX11::~RenderWindowDX11()
    {
        destroyBackBufferViews();
        destroySwapChain();

        destroySystemWindow();
    }

    //////////////////////////////////////////
    RenderWindowDX11Ptr RenderWindowDX11::Create(
        RenderSystemDX11* _renderSystem,
        RenderWindowParams const& _params)
    {
        RenderWindowDX11Ptr window;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(RenderWindowDX11, window, init(_renderSystem, _params));
        return window;
    }

    //////////////////////////////////////////
    bool RenderWindowDX11::init(
        RenderSystemDX11* _renderSystem,
        RenderWindowParams const& _params)
    {
        m_renderSystemDX11 = _renderSystem;

        if (!RenderWindow::init(_params))
            return false;

        if (!createSwapChain(_params.antialiasingLevel))
            return false;

        if (!createBackBufferViews())
            return false;

        m_renderQueue = RenderQueueDX11::Create(this);
        if (!m_renderQueue)
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderSystemDX11* RenderWindowDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystemDX11;
    }

    //////////////////////////////////////////
    bool RenderWindowDX11::createSwapChain(S32 _antialiasingLevel)
    {
#if (MAZE_PLATFORM == MAZE_PLATFORM_WINDOWS)
        MAZE_ERROR_RETURN_VALUE_IF(!m_window, false, "System window is null!");

        WindowWin* windowWin = m_window->castRaw<WindowWin>();
        HWND hwnd = windowWin->getHandle();
        MAZE_ERROR_RETURN_VALUE_IF(!hwnd, false, "System window handle is null!");

        destroySwapChain();

        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();

        Vec2U clientSize = m_window->getClientSize();

        m_antialiasingLevel = Math::Clamp(_antialiasingLevel, 0, renderSystem->getWindowMaxAntialiasingLevelSupport());

        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        memset(&swapChainDesc, 0, sizeof(swapChainDesc));
        swapChainDesc.BufferDesc.Width = (UINT)clientSize.x;
        swapChainDesc.BufferDesc.Height = (UINT)clientSize.y;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.SampleDesc.Count = m_antialiasingLevel > 1 ? (UINT)m_antialiasingLevel : 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.OutputWindow = hwnd;
        swapChainDesc.Windowed = TRUE;
        // Legacy blit model - supports MSAA backbuffers
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        HRESULT hr = renderSystem->getDXGIFactory()->CreateSwapChain(
            renderSystem->getDevice(),
            &swapChainDesc,
            &m_swapChain);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateSwapChain failed! hr=0x%08x", (U32)hr);

        // The engine handles fullscreen transitions itself
        renderSystem->getDXGIFactory()->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

        Debug::Log("RenderWindowDX11: Swap chain created (%ux%u, AA=%d).", clientSize.x, clientSize.y, m_antialiasingLevel);

        return true;
#else
        MAZE_ERROR_RETURN_VALUE(false, "DX11 render window is only supported on Windows!");
#endif
    }

    //////////////////////////////////////////
    bool RenderWindowDX11::createBackBufferViews()
    {
        MAZE_ERROR_RETURN_VALUE_IF(!m_swapChain, false, "Swap chain is null!");

        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();
        ID3D11Device* device = renderSystem->getDevice();

        // Backbuffer RTV
        ID3D11Texture2D* backBuffer = nullptr;
        HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "Swap chain GetBuffer failed! hr=0x%08x", (U32)hr);

        hr = device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
        backBuffer->Release();
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "Backbuffer CreateRenderTargetView failed! hr=0x%08x", (U32)hr);

        // Depth stencil buffer
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        m_swapChain->GetDesc(&swapChainDesc);

        D3D11_TEXTURE2D_DESC depthDesc;
        memset(&depthDesc, 0, sizeof(depthDesc));
        depthDesc.Width = swapChainDesc.BufferDesc.Width;
        depthDesc.Height = swapChainDesc.BufferDesc.Height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc = swapChainDesc.SampleDesc;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = device->CreateTexture2D(&depthDesc, nullptr, &m_depthStencilTexture);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "Depth stencil texture creation failed! hr=0x%08x", (U32)hr);

        hr = device->CreateDepthStencilView(m_depthStencilTexture, nullptr, &m_depthStencilView);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateDepthStencilView failed! hr=0x%08x", (U32)hr);

        return true;
    }

    //////////////////////////////////////////
    void RenderWindowDX11::destroyBackBufferViews()
    {
        RenderSystemDX11* renderSystem = getRenderSystemDX11Raw();
        if (renderSystem && renderSystem->getStateMachine())
        {
            if (m_renderTargetView)
                renderSystem->getStateMachine()->processRenderTargetViewsDestroyed(&m_renderTargetView, 1);

            if (m_depthStencilView)
                renderSystem->getStateMachine()->processDepthStencilViewDestroyed(m_depthStencilView);
        }

        SafeReleaseDX11(m_depthStencilView);
        SafeReleaseDX11(m_depthStencilTexture);
        SafeReleaseDX11(m_renderTargetView);
    }

    //////////////////////////////////////////
    void RenderWindowDX11::destroySwapChain()
    {
        SafeReleaseDX11(m_swapChain);
    }

    //////////////////////////////////////////
    void RenderWindowDX11::notifyWindowSizeChanged(Window* _window)
    {
        RenderWindow::notifyWindowSizeChanged(_window);

        if (!m_swapChain)
            return;

        Vec2U clientSize = m_window->getClientSize();
        if (clientSize.x == 0u || clientSize.y == 0u)
            return;

        destroyBackBufferViews();

        HRESULT hr = m_swapChain->ResizeBuffers(
            0,
            (UINT)clientSize.x,
            (UINT)clientSize.y,
            DXGI_FORMAT_UNKNOWN,
            0);
        MAZE_ERROR_RETURN_IF(FAILED(hr), "Swap chain ResizeBuffers failed! hr=0x%08x", (U32)hr);

        createBackBufferViews();
    }

    //////////////////////////////////////////
    void RenderWindowDX11::swapBuffers()
    {
        if (!m_swapChain)
            return;

        m_swapChain->Present(m_vsync > 0 ? (UINT)m_vsync : 0, 0);
    }

    //////////////////////////////////////////
    bool RenderWindowDX11::processRenderTargetWillSet()
    {
        return (m_swapChain != nullptr) && (m_renderTargetView != nullptr);
    }

    //////////////////////////////////////////
    void RenderWindowDX11::processRenderTargetSet()
    {
        StateMachineDX11* stateMachine = getRenderSystemDX11Raw()->getStateMachine();

        stateMachine->bindRenderTarget(
            &m_renderTargetView,
            1,
            m_depthStencilView,
            getRenderTargetSize(),
            false);
    }

    //////////////////////////////////////////
    void RenderWindowDX11::processRenderTargetWillReset()
    {
    }

    //////////////////////////////////////////
    void RenderWindowDX11::setVSync(S32 _vsync)
    {
        m_vsync = _vsync;
    }

    //////////////////////////////////////////
    bool RenderWindowDX11::isReadyToRender() const
    {
        if (!m_swapChain || !m_renderTargetView)
            return false;

        return RenderWindow::isReadyToRender();
    }


} // namespace Maze
//////////////////////////////////////////
