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
#if (!defined(_MazeRenderSystemDX11_hpp_))
#define _MazeRenderSystemDX11_hpp_


//////////////////////////////////////////
#include "maze-render-system-dx11/MazeRenderSystemDX11Header.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11Config.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-graphics/MazeRenderSystem.hpp"
#include "maze-core/math/MazeMath.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(RenderSystemDX11);
    MAZE_USING_SHARED_PTR(ShaderDX11);
    class VertexArrayObjectDX11;
    class ShaderDX11;
    class Texture2DDX11;
    class Texture2DMSDX11;


    //////////////////////////////////////////
    // Class RenderSystemDX11
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_DX11_API RenderSystemDX11
        : public RenderSystem
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(RenderSystemDX11, RenderSystem);

    public:

        //////////////////////////////////////////
        virtual ~RenderSystemDX11();

        //////////////////////////////////////////
        static RenderSystemDX11Ptr Create(RenderSystemDX11Config const& _config = RenderSystemDX11Config());


        //////////////////////////////////////////
        virtual String const& getName() MAZE_OVERRIDE;


        //////////////////////////////////////////
        inline RenderSystemDX11Config const& getConfig() const { return m_config; }


        //////////////////////////////////////////
        inline ID3D11Device* getDevice() const { return m_device; }

        //////////////////////////////////////////
        inline ID3D11DeviceContext* getDeviceContext() const { return m_deviceContext; }

        //////////////////////////////////////////
        inline IDXGIFactory* getDXGIFactory() const { return m_dxgiFactory; }

        //////////////////////////////////////////
        inline StateMachineDX11* getStateMachine() const { return m_stateMachine.get(); }


        //////////////////////////////////////////
        virtual bool isTextureFormatSupported(PixelFormat::Enum _pixelFormat) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getWindowMaxAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getWindowCurrentAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getTextureMaxSize() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual S32 getTextureMaxAntialiasingLevelSupport() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual F32 getTextureMaxAnisotropyLevel() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool setCurrentRenderTarget(RenderTarget* _renderTarget) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void clearCurrentRenderTarget(
            bool _colorBuffer = true,
            bool _depthBuffer = true,
            bool _stencilBuffer = true) MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual ShaderUniformPtr createShaderUniform(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderWindowPtr createRenderWindow(RenderWindowParams const& _params) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual VertexArrayObjectPtr createVertexArrayObject(RenderTarget* _renderTarget = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual VertexBufferObjectPtr createVertexBufferObject(
            GPUByteBufferAccessType::Enum _accessType,
            bool _singleMapping = false,
            RenderTarget* _renderTarget = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Texture2DPtr createTexture2D() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual Texture2DMSPtr createTexture2DMS() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual TextureCubePtr createTextureCube() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual MaterialPtr createMaterial() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderPassPtr createRenderPass(
            RenderPassType _passType) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual GPUVertexBufferPtr createGPUVertexBuffer(
            VertexDataDescription const& _vertexDataDescription,
            Size _vertexCount,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual GPUTextureBufferPtr createGPUTextureBuffer(
            Vec2U const& _size,
            PixelFormat::Enum _pixelFormat,
            GPUByteBufferAccessType::Enum _accessType,
            void* _initialData = nullptr) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual RenderBufferPtr createRenderBuffer(
            RenderBuffer::RenderBufferDeleter const& _deleter = DefaultDelete<RenderBuffer>(),
            RenderTarget* _renderTarget = nullptr) MAZE_OVERRIDE;


        //////////////////////////////////////////
        // Sampler states are shared and cached by their description
        ID3D11SamplerState* ensureSamplerState(
            TextureFilter _minFilter,
            TextureFilter _magFilter,
            TextureWrap _wrapS,
            TextureWrap _wrapT,
            TextureWrap _wrapR,
            F32 _anisotropyLevel,
            Vec4F const& _borderColor);

        //////////////////////////////////////////
        // Input layouts are cached per (shader input signature, VAO vertex layout) pair
        ID3D11InputLayout* ensureInputLayout(
            ShaderDX11* _shader,
            VertexArrayObjectDX11* _vao);

        //////////////////////////////////////////
        // Small buffer bound in place of vertex attributes the VAO doesn't provide.
        // Holds the GL default attribute value (0, 0, 0, 1) per component type
        ID3D11Buffer* getZeroVertexBuffer() const { return m_zeroVertexBuffer; }

        //////////////////////////////////////////
        // ResolveSubresource doesn't support depth formats - resolves sample 0
        // via a fullscreen pass writing SV_Depth
        bool resolveDepthMSAA(
            Texture2DDX11* _dstTexture,
            Texture2DMSDX11* _srcTexture);

    protected:

        //////////////////////////////////////////
        RenderSystemDX11();

        //////////////////////////////////////////
        virtual bool init(RenderSystemDX11Config const& _config);

        //////////////////////////////////////////
        bool createDevice();

    protected:
        RenderSystemDX11Config m_config;

        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_deviceContext = nullptr;
        IDXGIFactory* m_dxgiFactory = nullptr;
        D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

        UniquePtr<StateMachineDX11> m_stateMachine;

        UnorderedMap<U64, ID3D11SamplerState*> m_samplerStates;
        UnorderedMap<U64, ID3D11InputLayout*> m_inputLayouts;

        ID3D11Buffer* m_zeroVertexBuffer = nullptr;

        // Depth MSAA resolve pass objects (created lazily)
        ID3D11VertexShader* m_depthResolveVS = nullptr;
        ID3D11PixelShader* m_depthResolvePS = nullptr;
        ID3D11DepthStencilState* m_depthResolveDepthState = nullptr;
        ID3D11RasterizerState* m_depthResolveRasterizerState = nullptr;
        bool m_depthResolveInitFailed = false;

        S32 m_windowMaxAntialiasingLevel = 0;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeRenderSystemDX11_hpp_
//////////////////////////////////////////
