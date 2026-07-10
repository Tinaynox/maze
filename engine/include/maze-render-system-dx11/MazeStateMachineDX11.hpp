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
#if (!defined(_MazeStateMachineDX11_hpp_))
#define _MazeStateMachineDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeHelpersDX11.hpp"
#include "maze-core/math/MazeRect2.hpp"
#include "maze-core/math/MazeVec4.hpp"
#include "maze-graphics/MazeBlendMode.hpp"
#include "maze-graphics/MazeCompareFunction.hpp"
#include "maze-graphics/MazeStencilOperation.hpp"
#include "maze-graphics/MazeCullMode.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    class RenderSystemDX11;
    class ShaderDX11;


    //////////////////////////////////////////
    // Class StateMachineDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API StateMachineDX11
    {
    public:

        //////////////////////////////////////////
        static S32 const c_renderTargetsMax = 4;
        static S32 const c_shaderResourcesMax = 16;

    public:

        //////////////////////////////////////////
        StateMachineDX11(RenderSystemDX11* _renderSystem);

        //////////////////////////////////////////
        ~StateMachineDX11();


        //////////////////////////////////////////
        void setBlendEnabled(bool _value);

        //////////////////////////////////////////
        void setBlendFactors(BlendFactor _srcFactor, BlendFactor _destFactor);

        //////////////////////////////////////////
        void setDepthTestEnabled(bool _value);

        //////////////////////////////////////////
        void setDepthTestCompareFunction(CompareFunction _value);

        //////////////////////////////////////////
        void setDepthWriteEnabled(bool _value);

        //////////////////////////////////////////
        void setCullEnabled(bool _value);

        //////////////////////////////////////////
        void setCullMode(CullMode _value);

        //////////////////////////////////////////
        void setStencilTestEnabled(bool _value);

        //////////////////////////////////////////
        void setStencilFunc(CompareFunction _function, S32 _referenceValue, U32 _readMask);

        //////////////////////////////////////////
        void setStencilOp(StencilOperation _fail, StencilOperation _depthFail, StencilOperation _pass);

        //////////////////////////////////////////
        void setStencilWriteMask(U32 _mask);

        //////////////////////////////////////////
        void setScissorTestEnabled(bool _value);

        //////////////////////////////////////////
        inline bool getScissorTestEnabled() const { return m_scissorTestEnabled; }

        //////////////////////////////////////////
        void setScissorRect(Rect2S const& _rect);

        //////////////////////////////////////////
        void setViewportRect(Rect2S const& _rect);


        //////////////////////////////////////////
        void setCurrentShader(ShaderDX11* _shader);

        //////////////////////////////////////////
        inline ShaderDX11* getCurrentShader() const { return m_currentShader; }


        //////////////////////////////////////////
        void bindRenderTarget(
            ID3D11RenderTargetView* const* _renderTargetViews,
            S32 _renderTargetViewsCount,
            ID3D11DepthStencilView* _depthStencilView,
            Vec2U const& _renderTargetSize,
            bool _flipY);

        //////////////////////////////////////////
        void unbindRenderTarget();

        //////////////////////////////////////////
        inline bool getFlipY() const { return m_flipY; }

        //////////////////////////////////////////
        inline Vec2U const& getRenderTargetSize() const { return m_renderTargetSize; }

        //////////////////////////////////////////
        inline ID3D11RenderTargetView* const* getRenderTargetViews() const { return m_renderTargetViews; }

        //////////////////////////////////////////
        inline S32 getRenderTargetViewsCount() const { return m_renderTargetViewsCount; }

        //////////////////////////////////////////
        inline ID3D11DepthStencilView* getDepthStencilView() const { return m_depthStencilView; }

        //////////////////////////////////////////
        // Drops cached references to views that are being destroyed
        void processRenderTargetViewsDestroyed(ID3D11RenderTargetView* const* _renderTargetViews, S32 _count);

        //////////////////////////////////////////
        void processDepthStencilViewDestroyed(ID3D11DepthStencilView* _depthStencilView);


        //////////////////////////////////////////
        void bindPixelShaderResource(S32 _srvSlot, ID3D11ShaderResourceView* _srv, S32 _samplerSlot, ID3D11SamplerState* _sampler);

        //////////////////////////////////////////
        void bindVertexShaderResource(S32 _srvSlot, ID3D11ShaderResourceView* _srv, S32 _samplerSlot, ID3D11SamplerState* _sampler);

        //////////////////////////////////////////
        // Unbinds an SRV from all slots (before using the texture as a render target)
        void unbindShaderResource(ID3D11ShaderResourceView* _srv);


        //////////////////////////////////////////
        // Resolves and binds all pending pipeline states. Call right before a draw
        void flushPipeline();


        //////////////////////////////////////////
        inline RenderSystemDX11* getRenderSystem() const { return m_renderSystem; }

        //////////////////////////////////////////
        ID3D11Device* getDevice() const;

        //////////////////////////////////////////
        ID3D11DeviceContext* getDeviceContext() const;

    protected:

        //////////////////////////////////////////
        ID3D11BlendState* ensureBlendState();

        //////////////////////////////////////////
        ID3D11DepthStencilState* ensureDepthStencilState();

        //////////////////////////////////////////
        ID3D11RasterizerState* ensureRasterizerState();

    protected:
        RenderSystemDX11* m_renderSystem = nullptr;

        // Blend
        bool m_blendEnabled = false;
        BlendFactor m_blendSrcFactor = BlendFactor::One;
        BlendFactor m_blendDestFactor = BlendFactor::Zero;

        // Depth
        bool m_depthTestEnabled = false;
        CompareFunction m_depthTestCompareFunction = CompareFunction::Less;
        bool m_depthWriteEnabled = true;

        // Stencil
        bool m_stencilTestEnabled = false;
        CompareFunction m_stencilTestCompareFunction = CompareFunction::Always;
        S32 m_stencilReferenceValue = 0;
        U32 m_stencilReadMask = 0xFF;
        U32 m_stencilWriteMask = 0xFF;
        StencilOperation m_stencilFailOperation = StencilOperation::Keep;
        StencilOperation m_stencilDepthFailOperation = StencilOperation::Keep;
        StencilOperation m_stencilPassOperation = StencilOperation::Keep;

        // Rasterizer
        bool m_cullEnabled = false;
        CullMode m_cullMode = CullMode::Back;
        bool m_scissorTestEnabled = false;
        Rect2S m_scissorRect = Rect2S(0, 0, 0, 0);
        Rect2S m_viewportRect = Rect2S(0, 0, 0, 0);

        // Render target
        ID3D11RenderTargetView* m_renderTargetViews[c_renderTargetsMax];
        S32 m_renderTargetViewsCount = 0;
        ID3D11DepthStencilView* m_depthStencilView = nullptr;
        Vec2U m_renderTargetSize = Vec2U::c_zero;
        bool m_flipY = false;

        // Shaders
        ShaderDX11* m_currentShader = nullptr;

        // Shader resources
        ID3D11ShaderResourceView* m_pixelShaderResources[c_shaderResourcesMax];
        ID3D11SamplerState* m_pixelSamplers[c_shaderResourcesMax];
        ID3D11ShaderResourceView* m_vertexShaderResources[c_shaderResourcesMax];
        ID3D11SamplerState* m_vertexSamplers[c_shaderResourcesMax];

        // Dirty flags
        bool m_blendStateDirty = true;
        bool m_depthStencilStateDirty = true;
        bool m_rasterizerStateDirty = true;
        bool m_viewportDirty = true;
        bool m_scissorDirty = true;

        // State object caches
        UnorderedMap<U32, ID3D11BlendState*> m_blendStates;
        UnorderedMap<U64, ID3D11DepthStencilState*> m_depthStencilStates;
        UnorderedMap<U32, ID3D11RasterizerState*> m_rasterizerStates;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeStateMachineDX11_hpp_
//////////////////////////////////////////
