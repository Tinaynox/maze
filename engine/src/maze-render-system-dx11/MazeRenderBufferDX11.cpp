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
#include "maze-render-system-dx11/MazeRenderBufferDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeRenderQueueDX11.hpp"
#include "maze-render-system-dx11/MazeTexture2DDX11.hpp"
#include "maze-render-system-dx11/MazeTexture2DMSDX11.hpp"
#include "maze-render-system-dx11/MazeHelpersDX11.hpp"
#include "maze-graphics/MazeTexture2D.hpp"
#include "maze-graphics/MazeTexture2DMS.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderBufferDX11
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderBufferDX11, RenderBuffer);

    //////////////////////////////////////////
    RenderBufferDX11::RenderBufferDX11()
    {
    }

    //////////////////////////////////////////
    RenderBufferDX11::~RenderBufferDX11()
    {
    }

    //////////////////////////////////////////
    RenderBufferDX11Ptr RenderBufferDX11::Create(
        RenderSystemDX11* _renderSystem,
        RenderBufferDeleter const& _deleter)
    {
        RenderBufferDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(RenderBufferDX11, object, _deleter, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    RenderBufferDX11Ptr RenderBufferDX11::Create(
        RenderBufferDX11Ptr const& _renderBuffer,
        RenderBufferDeleter const& _deleter)
    {
        RenderBufferDX11Ptr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR_EX(RenderBufferDX11, object, _deleter, init(_renderBuffer));
        return object;
    }

    //////////////////////////////////////////
    bool RenderBufferDX11::init(RenderSystemDX11* _renderSystem)
    {
        if (!RenderBuffer::init((RenderSystem*)_renderSystem))
            return false;

        m_renderQueue = RenderQueueDX11::Create(this);
        if (!m_renderQueue)
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool RenderBufferDX11::init(RenderBufferDX11Ptr const& _renderBuffer)
    {
        if (!RenderBuffer::init(RenderBufferPtr(_renderBuffer)))
            return false;

        m_renderQueue = RenderQueueDX11::Create(this);
        if (!m_renderQueue)
            return false;

        return true;
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderBufferDX11::createCopy()
    {
        return RenderBufferDX11::Create(cast<RenderBufferDX11>());
    }

    //////////////////////////////////////////
    RenderSystemDX11* RenderBufferDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystem->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    bool RenderBufferDX11::setSize(Vec2U const& _size)
    {
        if (!RenderBuffer::setSize(_size))
            return false;

        for (Size i = 0; i < c_renderBufferColorTexturesMax; ++i)
            resizeTexture(m_colorTextures[i], _size);

        resizeTexture(m_depthTexture, _size);
        resizeTexture(m_stencilTexture, _size);

        eventRenderBufferSizeChanged(cast<RenderBuffer>());

        return true;
    }

    //////////////////////////////////////////
    void RenderBufferDX11::resizeTexture(TexturePtr const& _texture, Vec2U const& _size)
    {
        if (!_texture)
            return;

        switch (_texture->getType())
        {
            case TextureType::TwoDimensional:
            {
                Texture2D* texture2D = _texture->castRaw<Texture2D>();
                texture2D->loadEmpty(_size, texture2D->getInternalPixelFormat());
                break;
            }
            case TextureType::TwoDimensionalMultisample:
            {
                Texture2DMS* texture2D = _texture->castRaw<Texture2DMS>();
                texture2D->loadEmpty(_size, texture2D->getInternalPixelFormat(), texture2D->getSamples());
                break;
            }
            default:
            {
                MAZE_NOT_IMPLEMENTED;
            }
        }
    }

    //////////////////////////////////////////
    void RenderBufferDX11::endDraw()
    {
        RenderBuffer::endDraw();

        eventRenderBufferEndDraw(this);
    }

    //////////////////////////////////////////
    bool RenderBufferDX11::processRenderTargetWillSet()
    {
        return true;
    }

    //////////////////////////////////////////
    void RenderBufferDX11::processRenderTargetSet()
    {
        StateMachineDX11* stateMachine = getRenderSystemDX11Raw()->getStateMachine();

        ID3D11RenderTargetView* renderTargetViews[StateMachineDX11::c_renderTargetsMax];
        S32 renderTargetViewsCount = 0;

        for (S32 i = 0; i < StateMachineDX11::c_renderTargetsMax; ++i)
        {
            TexturePtr const& colorTexture = getColorTexture((U32)i);
            if (!colorTexture)
                continue;

            ID3D11RenderTargetView* rtv = nullptr;
            if (colorTexture->getType() == TextureType::TwoDimensional)
                rtv = colorTexture->castRaw<Texture2DDX11>()->ensureRenderTargetView();
            else
            if (colorTexture->getType() == TextureType::TwoDimensionalMultisample)
                rtv = colorTexture->castRaw<Texture2DMSDX11>()->ensureRenderTargetView();

            if (rtv)
                renderTargetViews[renderTargetViewsCount++] = rtv;
        }

        ID3D11DepthStencilView* depthStencilView = nullptr;
        TexturePtr const& depthTexture = getDepthTexture();
        if (depthTexture)
        {
            if (depthTexture->getType() == TextureType::TwoDimensional)
                depthStencilView = depthTexture->castRaw<Texture2DDX11>()->ensureDepthStencilView();
            else
            if (depthTexture->getType() == TextureType::TwoDimensionalMultisample)
                depthStencilView = depthTexture->castRaw<Texture2DMSDX11>()->ensureDepthStencilView();
        }

        // Offscreen targets are rendered flipped so their memory layout matches OpenGL render buffers
        stateMachine->bindRenderTarget(
            renderTargetViews,
            renderTargetViewsCount,
            depthStencilView,
            getSize(),
            true);
    }

    //////////////////////////////////////////
    void RenderBufferDX11::processRenderTargetWillReset()
    {
    }

    //////////////////////////////////////////
    void RenderBufferDX11::blit(RenderBufferPtr const& _srcBuffer)
    {
        MAZE_ERROR_RETURN_IF(!_srcBuffer, "Source buffer is null!");

        ID3D11DeviceContext* deviceContext = getRenderSystemDX11Raw()->getDeviceContext();

        auto getTextureResource =
            [](TexturePtr const& _texture) -> ID3D11Texture2D*
            {
                if (!_texture)
                    return nullptr;

                if (_texture->getType() == TextureType::TwoDimensional)
                    return _texture->castRaw<Texture2DDX11>()->getTextureDX11();

                if (_texture->getType() == TextureType::TwoDimensionalMultisample)
                    return _texture->castRaw<Texture2DMSDX11>()->getTextureDX11();

                return nullptr;
            };

        auto blitTexture =
            [&](TexturePtr const& _dstTexture, TexturePtr const& _srcTexture, PixelFormat::Enum _pixelFormat)
            {
                ID3D11Texture2D* dst = getTextureResource(_dstTexture);
                ID3D11Texture2D* src = getTextureResource(_srcTexture);
                if (!dst || !src)
                    return;

                bool srcMultisampled = (_srcTexture->getType() == TextureType::TwoDimensionalMultisample);
                bool dstMultisampled = (_dstTexture->getType() == TextureType::TwoDimensionalMultisample);

                if (srcMultisampled && !dstMultisampled)
                    deviceContext->ResolveSubresource(dst, 0, src, 0, GetPixelFormatDX11(_pixelFormat));
                else
                    deviceContext->CopySubresourceRegion(dst, 0, 0, 0, 0, src, 0, nullptr);
            };

        for (U32 i = 0; i < StateMachineDX11::c_renderTargetsMax; ++i)
        {
            TexturePtr const& dstTexture = getColorTexture(i);
            TexturePtr const& srcTexture = _srcBuffer->getColorTexture(i);
            if (dstTexture && srcTexture)
            {
                PixelFormat::Enum pixelFormat = PixelFormat::None;
                if (dstTexture->getType() == TextureType::TwoDimensional)
                    pixelFormat = dstTexture->castRaw<Texture2DDX11>()->getInternalPixelFormat();
                else
                if (dstTexture->getType() == TextureType::TwoDimensionalMultisample)
                    pixelFormat = dstTexture->castRaw<Texture2DMSDX11>()->getInternalPixelFormat();

                blitTexture(dstTexture, srcTexture, pixelFormat);
            }
        }

        // Depth (ResolveSubresource doesn't support depth formats - see resolveDepthMSAA)
        TexturePtr const& dstDepthTexture = getDepthTexture();
        TexturePtr const& srcDepthTexture = _srcBuffer->getDepthTexture();
        if (dstDepthTexture && srcDepthTexture)
        {
            bool srcMultisampled = (srcDepthTexture->getType() == TextureType::TwoDimensionalMultisample);
            bool dstMultisampled = (dstDepthTexture->getType() == TextureType::TwoDimensionalMultisample);

            if (srcMultisampled && !dstMultisampled)
            {
                getRenderSystemDX11Raw()->resolveDepthMSAA(
                    dstDepthTexture->castRaw<Texture2DDX11>(),
                    srcDepthTexture->castRaw<Texture2DMSDX11>());
            }
            else
            if (srcMultisampled == dstMultisampled)
            {
                ID3D11Texture2D* dst = getTextureResource(dstDepthTexture);
                ID3D11Texture2D* src = getTextureResource(srcDepthTexture);
                if (dst && src)
                {
                    // CopySubresourceRegion is not allowed for multisampled resources
                    if (srcMultisampled)
                        deviceContext->CopyResource(dst, src);
                    else
                        deviceContext->CopySubresourceRegion(dst, 0, 0, 0, 0, src, 0, nullptr);
                }
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
