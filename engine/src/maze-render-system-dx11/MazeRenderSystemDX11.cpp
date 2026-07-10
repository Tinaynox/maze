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
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeShaderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeShaderDX11.hpp"
#include "maze-render-system-dx11/MazeShaderUniformDX11.hpp"
#include "maze-render-system-dx11/MazeTexture2DDX11.hpp"
#include "maze-render-system-dx11/MazeTexture2DMSDX11.hpp"
#include "maze-render-system-dx11/MazeTextureCubeDX11.hpp"
#include "maze-render-system-dx11/MazeVertexArrayObjectDX11.hpp"
#include "maze-render-system-dx11/MazeVertexBufferObjectDX11.hpp"
#include "maze-render-system-dx11/MazeMaterialDX11.hpp"
#include "maze-render-system-dx11/MazeRenderPassDX11.hpp"
#include "maze-render-system-dx11/MazeRenderBufferDX11.hpp"
#include "maze-render-system-dx11/MazeRenderWindowDX11.hpp"
#include "maze-graphics/MazeRenderTarget.hpp"
#include "maze-core/services/MazeLogStream.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class RenderSystemDX11
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(RenderSystemDX11, RenderSystem);

    //////////////////////////////////////////
    RenderSystemDX11::RenderSystemDX11()
    {
    }

    //////////////////////////////////////////
    RenderSystemDX11::~RenderSystemDX11()
    {
        m_spriteManager.reset();
        m_systemFontManager.reset();
        m_materialManager.reset();
        m_textureManager.reset();
        m_renderMeshManager.reset();
        m_shaderSystem.reset();

        for (auto& samplerData : m_samplerStates)
            SafeReleaseDX11(samplerData.second);
        m_samplerStates.clear();

        for (auto& layoutData : m_inputLayouts)
            SafeReleaseDX11(layoutData.second);
        m_inputLayouts.clear();

        SafeReleaseDX11(m_zeroVertexBuffer);

        m_stateMachine.reset();

        if (m_deviceContext)
            m_deviceContext->ClearState();

        SafeReleaseDX11(m_dxgiFactory);
        SafeReleaseDX11(m_deviceContext);
        SafeReleaseDX11(m_device);
    }

    //////////////////////////////////////////
    RenderSystemDX11Ptr RenderSystemDX11::Create(RenderSystemDX11Config const& _config)
    {
        RenderSystemDX11Ptr renderSystem;
        MAZE_CREATE_AND_INIT_SHARED_PTR(RenderSystemDX11, renderSystem, init(_config));
        return renderSystem;
    }

    //////////////////////////////////////////
    bool RenderSystemDX11::init(RenderSystemDX11Config const& _config)
    {
        if (!RenderSystem::init())
            return false;

        m_config = _config;

        if (!createDevice())
            return false;

        m_stateMachine = MakeUnique<StateMachineDX11>(this);

        // Zero-filled buffer for vertex attributes missing from a VAO
        {
            U8 zeroData[64];
            memset(zeroData, 0, sizeof(zeroData));

            D3D11_BUFFER_DESC bufferDesc;
            memset(&bufferDesc, 0, sizeof(bufferDesc));
            bufferDesc.ByteWidth = sizeof(zeroData);
            bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA initialData;
            memset(&initialData, 0, sizeof(initialData));
            initialData.pSysMem = zeroData;

            HRESULT hr = m_device->CreateBuffer(&bufferDesc, &initialData, &m_zeroVertexBuffer);
            MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "Zero vertex buffer creation failed! hr=0x%08x", (U32)hr);
        }

        ShaderSystemDX11::Initialize(m_shaderSystem, getSharedPtr());
        if (!m_shaderSystem)
            return false;

        processSystemInited();

        return true;
    }

    //////////////////////////////////////////
    bool RenderSystemDX11::createDevice()
    {
        UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        if (m_config.debugLayer)
            deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

        D3D_FEATURE_LEVEL const featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0
        };

        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            deviceFlags,
            featureLevels,
            (UINT)(sizeof(featureLevels) / sizeof(featureLevels[0])),
            D3D11_SDK_VERSION,
            &m_device,
            &m_featureLevel,
            &m_deviceContext);

        if (FAILED(hr) && (deviceFlags & D3D11_CREATE_DEVICE_DEBUG))
        {
            MAZE_WARNING("D3D11 debug layer is not available - creating device without it");
            deviceFlags &= ~D3D11_CREATE_DEVICE_DEBUG;
            hr = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr,
                deviceFlags,
                featureLevels,
                (UINT)(sizeof(featureLevels) / sizeof(featureLevels[0])),
                D3D11_SDK_VERSION,
                &m_device,
                &m_featureLevel,
                &m_deviceContext);
        }

        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "D3D11CreateDevice failed! hr=0x%08x", (U32)hr);

        // Fetch the DXGI factory the device was created on
        IDXGIDevice* dxgiDevice = nullptr;
        hr = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "IDXGIDevice query failed! hr=0x%08x", (U32)hr);

        IDXGIAdapter* dxgiAdapter = nullptr;
        hr = dxgiDevice->GetAdapter(&dxgiAdapter);
        dxgiDevice->Release();
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "IDXGIAdapter query failed! hr=0x%08x", (U32)hr);

        DXGI_ADAPTER_DESC adapterDesc;
        if (SUCCEEDED(dxgiAdapter->GetDesc(&adapterDesc)))
        {
            WString description = adapterDesc.Description;
            Debug::Log("DX11 Adapter: %S", description.c_str());
        }

        hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);
        dxgiAdapter->Release();
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "IDXGIFactory query failed! hr=0x%08x", (U32)hr);

        Debug::Log("DX11 Device created. Feature level: 0x%04x", (U32)m_featureLevel);

        // Detect max supported MSAA level for the backbuffer format
        m_windowMaxAntialiasingLevel = 1;
        for (UINT samples = 2; samples <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; samples *= 2)
        {
            UINT qualityLevels = 0;
            if (SUCCEEDED(m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, samples, &qualityLevels)) &&
                qualityLevels > 0)
                m_windowMaxAntialiasingLevel = (S32)samples;
        }

        return true;
    }

    //////////////////////////////////////////
    String const& RenderSystemDX11::getName()
    {
        static String s_name = "DX11";
        return s_name;
    }

    //////////////////////////////////////////
    bool RenderSystemDX11::isTextureFormatSupported(PixelFormat::Enum _pixelFormat)
    {
        return GetPixelFormatDX11(_pixelFormat) != DXGI_FORMAT_UNKNOWN;
    }

    //////////////////////////////////////////
    S32 RenderSystemDX11::getWindowMaxAntialiasingLevelSupport()
    {
        return m_windowMaxAntialiasingLevel;
    }

    //////////////////////////////////////////
    S32 RenderSystemDX11::getWindowCurrentAntialiasingLevelSupport()
    {
        return m_windowMaxAntialiasingLevel;
    }

    //////////////////////////////////////////
    S32 RenderSystemDX11::getTextureMaxSize()
    {
        switch (m_featureLevel)
        {
            case D3D_FEATURE_LEVEL_10_0:
            case D3D_FEATURE_LEVEL_10_1: return 8192;
            default: return 16384;
        }
    }

    //////////////////////////////////////////
    S32 RenderSystemDX11::getTextureMaxAntialiasingLevelSupport()
    {
        return m_windowMaxAntialiasingLevel;
    }

    //////////////////////////////////////////
    F32 RenderSystemDX11::getTextureMaxAnisotropyLevel()
    {
        return (F32)D3D11_MAX_MAXANISOTROPY;
    }

    //////////////////////////////////////////
    bool RenderSystemDX11::setCurrentRenderTarget(RenderTarget* _renderTarget)
    {
        if (m_currentRenderTarget == _renderTarget)
            return true;

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetWillReset();
            m_currentRenderTarget = nullptr;
        }

        if (_renderTarget)
        {
            if (!_renderTarget->processRenderTargetWillSet())
                return false;
        }

        m_currentRenderTarget = _renderTarget;

        if (m_currentRenderTarget)
        {
            m_currentRenderTarget->processRenderTargetSet();
        }

        return true;
    }

    //////////////////////////////////////////
    void RenderSystemDX11::clearCurrentRenderTarget(
        bool _colorBuffer,
        bool _depthBuffer,
        bool _stencilBuffer)
    {
        if (!m_currentRenderTarget)
            return;

        StateMachineDX11* stateMachine = m_stateMachine.get();

        if (_colorBuffer)
        {
            ColorU32 const& clearColor = m_currentRenderTarget->getClearColor();
            Vec4F clearColorVec = clearColor.toVec4F32();
            FLOAT const clearColorDX11[4] = { clearColorVec.x, clearColorVec.y, clearColorVec.z, clearColorVec.w };

            for (S32 i = 0; i < stateMachine->getRenderTargetViewsCount(); ++i)
            {
                ID3D11RenderTargetView* rtv = stateMachine->getRenderTargetViews()[i];
                if (rtv)
                    m_deviceContext->ClearRenderTargetView(rtv, clearColorDX11);
            }
        }

        if (_depthBuffer || _stencilBuffer)
        {
            ID3D11DepthStencilView* dsv = stateMachine->getDepthStencilView();
            if (dsv)
            {
                UINT clearFlags = 0;
                if (_depthBuffer)
                    clearFlags |= D3D11_CLEAR_DEPTH;
                if (_stencilBuffer)
                    clearFlags |= D3D11_CLEAR_STENCIL;

                F32 clearDepth = m_currentRenderTarget->getClearDepth();
                m_deviceContext->ClearDepthStencilView(dsv, clearFlags, clearDepth, 0);
            }
        }
    }

    //////////////////////////////////////////
    ShaderUniformPtr RenderSystemDX11::createShaderUniform(
        ShaderPtr const& _shader,
        ShaderUniformType _type)
    {
        return ShaderUniformDX11::Create(_shader, _type);
    }

    //////////////////////////////////////////
    RenderWindowPtr RenderSystemDX11::createRenderWindow(RenderWindowParams const& _params)
    {
        RenderWindowPtr window = RenderWindowDX11::Create(this, _params);
        if (!window)
        {
            MAZE_ERROR("RenderWindowDX11 cannot be created!");
            return RenderWindowPtr();
        }

        if (!processRenderWindowCreated(window))
        {
            MAZE_ERROR("processRenderWindowCreated failed!");
            return RenderWindowPtr();
        }

        return window;
    }

    //////////////////////////////////////////
    VertexArrayObjectPtr RenderSystemDX11::createVertexArrayObject(RenderTarget* _renderTarget)
    {
        return VertexArrayObjectDX11::Create(this);
    }

    //////////////////////////////////////////
    VertexBufferObjectPtr RenderSystemDX11::createVertexBufferObject(
        GPUByteBufferAccessType::Enum _accessType,
        bool _singleMapping,
        RenderTarget* _renderTarget)
    {
        return VertexBufferObjectDX11::Create(this, _accessType, _singleMapping);
    }

    //////////////////////////////////////////
    Texture2DPtr RenderSystemDX11::createTexture2D()
    {
        return Texture2DDX11::Create(this);
    }

    //////////////////////////////////////////
    Texture2DMSPtr RenderSystemDX11::createTexture2DMS()
    {
        return Texture2DMSDX11::Create(this);
    }

    //////////////////////////////////////////
    TextureCubePtr RenderSystemDX11::createTextureCube()
    {
        return TextureCubeDX11::Create(this);
    }

    //////////////////////////////////////////
    MaterialPtr RenderSystemDX11::createMaterial()
    {
        return MaterialDX11::Create(this);
    }

    //////////////////////////////////////////
    RenderPassPtr RenderSystemDX11::createRenderPass(RenderPassType _passType)
    {
        return RenderPassDX11::Create(this, _passType);
    }

    //////////////////////////////////////////
    GPUVertexBufferPtr RenderSystemDX11::createGPUVertexBuffer(
        VertexDataDescription const& _vertexDataDescription,
        Size _vertexCount,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        // Not required by the DX11 backend: instance streams work via constant buffer arrays
        MAZE_ERROR("GPUVertexBuffer is not supported by DX11 render system!");
        return nullptr;
    }

    //////////////////////////////////////////
    GPUTextureBufferPtr RenderSystemDX11::createGPUTextureBuffer(
        Vec2U const& _size,
        PixelFormat::Enum _pixelFormat,
        GPUByteBufferAccessType::Enum _accessType,
        void* _initialData)
    {
        // Not required by the DX11 backend: instance streams work via constant buffer arrays
        MAZE_ERROR("GPUTextureBuffer is not supported by DX11 render system!");
        return nullptr;
    }

    //////////////////////////////////////////
    RenderBufferPtr RenderSystemDX11::createRenderBuffer(
        RenderBuffer::RenderBufferDeleter const& _deleter,
        RenderTarget* _renderTarget)
    {
        return RenderBufferDX11::Create(this, _deleter);
    }

    //////////////////////////////////////////
    ID3D11SamplerState* RenderSystemDX11::ensureSamplerState(
        TextureFilter _minFilter,
        TextureFilter _magFilter,
        TextureWrap _wrapS,
        TextureWrap _wrapT,
        TextureWrap _wrapR,
        F32 _anisotropyLevel,
        Vec4F const& _borderColor)
    {
        U32 anisotropy = (U32)Math::Clamp((S32)_anisotropyLevel, 0, D3D11_MAX_MAXANISOTROPY);

        U64 key =
            ((U64)_minFilter & 0x7) |
            (((U64)_magFilter & 0x7) << 3) |
            (((U64)_wrapS & 0x3) << 6) |
            (((U64)_wrapT & 0x3) << 8) |
            (((U64)_wrapR & 0x3) << 10) |
            (((U64)anisotropy & 0xFF) << 12) |
            ((U64)((U32)(_borderColor.x * 255.0f) & 0xFF) << 20) |
            ((U64)((U32)(_borderColor.y * 255.0f) & 0xFF) << 28) |
            ((U64)((U32)(_borderColor.z * 255.0f) & 0xFF) << 36) |
            ((U64)((U32)(_borderColor.w * 255.0f) & 0xFF) << 44);

        auto it = m_samplerStates.find(key);
        if (it != m_samplerStates.end())
            return it->second;

        D3D11_SAMPLER_DESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.Filter = GetTextureFilterDX11(_minFilter, _magFilter, anisotropy > 1);
        desc.AddressU = GetTextureWrapDX11(_wrapS);
        desc.AddressV = GetTextureWrapDX11(_wrapT);
        desc.AddressW = GetTextureWrapDX11(_wrapR);
        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = anisotropy > 1 ? anisotropy : 1;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = _borderColor.x;
        desc.BorderColor[1] = _borderColor.y;
        desc.BorderColor[2] = _borderColor.z;
        desc.BorderColor[3] = _borderColor.w;
        desc.MinLOD = 0.0f;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        ID3D11SamplerState* sampler = nullptr;
        HRESULT hr = m_device->CreateSamplerState(&desc, &sampler);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), nullptr, "CreateSamplerState failed! hr=0x%08x", (U32)hr);

        m_samplerStates.emplace(key, sampler);
        return sampler;
    }

    //////////////////////////////////////////
    ID3D11InputLayout* RenderSystemDX11::ensureInputLayout(
        ShaderDX11* _shader,
        VertexArrayObjectDX11* _vao)
    {
        if (!_shader || !_vao || !_shader->getVertexShaderCode())
            return nullptr;

        U64 key = ((U64)_shader->getInputSignatureId() << 32) | (U64)_vao->getLayoutHash();

        auto it = m_inputLayouts.find(key);
        if (it != m_inputLayouts.end())
            return it->second;

        Vector<ShaderDX11InputParameter> const& inputParameters = _shader->getInputParameters();

        Vector<D3D11_INPUT_ELEMENT_DESC> elements;
        elements.reserve(inputParameters.size());

        for (ShaderDX11InputParameter const& param : inputParameters)
        {
            // Skip system-generated values (SV_InstanceID, SV_VertexID etc.)
            if (param.semanticName.size() > 3 &&
                param.semanticName[0] == 'S' && param.semanticName[1] == 'V' && param.semanticName[2] == '_')
                continue;

            D3D11_INPUT_ELEMENT_DESC element;
            memset(&element, 0, sizeof(element));
            element.SemanticName = param.semanticName.c_str();
            element.SemanticIndex = param.semanticIndex;
            element.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
            element.InstanceDataStepRate = 0;

            // Find the matching VAO attribute
            VertexArrayObjectDX11AttributeData const* attribute = nullptr;
            for (S32 s = 0; s < (S32)VertexAttributeSemantic::MAX; ++s)
            {
                VertexAttributeSemantic semantic = (VertexAttributeSemantic)s;
                CString semanticName = GetVertexAttributeSemanticNameDX11(semantic);
                if (!semanticName)
                    continue;

                if (param.semanticName == semanticName &&
                    param.semanticIndex == GetVertexAttributeSemanticIndexDX11(semantic))
                {
                    VertexArrayObjectDX11AttributeData const& attributeData = _vao->getAttribute(semantic);
                    if (attributeData.used)
                    {
                        attribute = &attributeData;
                        element.InputSlot = (UINT)s;
                    }
                    break;
                }
            }

            if (attribute)
            {
                element.Format = GetVertexAttributeFormatDX11(
                    attribute->description.type,
                    attribute->description.count,
                    attribute->description.normalized);
                element.AlignedByteOffset = 0;
            }
            else
            {
                // The VAO has no data for this attribute - read zeroes from the dummy buffer (stride 0)
                element.InputSlot = (UINT)VertexArrayObjectDX11::c_zeroBufferSlot;
                element.AlignedByteOffset = 0;
                switch (param.componentType)
                {
                    case D3D_REGISTER_COMPONENT_UINT32: element.Format = DXGI_FORMAT_R32G32B32A32_UINT; break;
                    case D3D_REGISTER_COMPONENT_SINT32: element.Format = DXGI_FORMAT_R32G32B32A32_SINT; break;
                    default: element.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
                }
            }

            elements.push_back(element);
        }

        ID3D11InputLayout* inputLayout = nullptr;

        if (!elements.empty())
        {
            HRESULT hr = m_device->CreateInputLayout(
                &elements[0],
                (UINT)elements.size(),
                _shader->getVertexShaderCode()->GetBufferPointer(),
                _shader->getVertexShaderCode()->GetBufferSize(),
                &inputLayout);
            MAZE_ERROR_IF(FAILED(hr), "CreateInputLayout failed! Shader: %s hr=0x%08x", _shader->getName().c_str(), (U32)hr);
        }

        m_inputLayouts.emplace(key, inputLayout);
        return inputLayout;
    }


} // namespace Maze
//////////////////////////////////////////
