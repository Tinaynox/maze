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
#include "maze-render-system-dx11/MazeShaderDX11.hpp"
#include "maze-render-system-dx11/MazeShaderUniformDX11.hpp"
#include "maze-render-system-dx11/MazeShaderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeRenderSystemDX11.hpp"
#include "maze-render-system-dx11/MazeStateMachineDX11.hpp"
#include "maze-render-system-dx11/MazeTexture2DDX11.hpp"
#include "maze-render-system-dx11/MazeTextureCubeDX11.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static String const c_commonShaderHeaderDX11 =
#include "shaders/MazeCommonShaderHeader.mzhlsl"
    ;

    //////////////////////////////////////////
    static U32 s_inputSignatureCounter = 0u;

    //////////////////////////////////////////
    static UnorderedMap<String, String> s_shaderIncludeFilesCacheDX11;

    //////////////////////////////////////////
    inline static S32 ShaderStageFromString(String const& _type)
    {
        if (_type == "vertex")
            return (S32)ShaderDX11Stage::Vertex;
        if (_type == "fragment" || _type == "pixel")
            return (S32)ShaderDX11Stage::Pixel;

        MAZE_ERROR("Unknown shader type!");
        return -1;
    }


    //////////////////////////////////////////
    // Class ShaderDX11
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderDX11, Shader);

    //////////////////////////////////////////
    ShaderDX11::ShaderDX11()
    {
    }

    //////////////////////////////////////////
    ShaderDX11::~ShaderDX11()
    {
        unloadDXShader();
    }

    //////////////////////////////////////////
    ShaderPtr ShaderDX11::Create(RenderSystemPtr const& _renderSystem)
    {
        ShaderDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderDX11, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderDX11::CreateFromFile(
        RenderSystemPtr const& _renderSystem,
        AssetFilePtr const& _shaderFile)
    {
        ShaderDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderDX11, object, init(_renderSystem, _shaderFile));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderDX11::CreateFromSource(
        RenderSystemPtr const& _renderSystem,
        String const& _shaderSource,
        CString _shaderName)
    {
        ShaderDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderDX11, object, init(_renderSystem, _shaderSource, _shaderName));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderDX11::Create(ShaderDX11Ptr const& _shader)
    {
        ShaderDX11Ptr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderDX11, object, init(_shader));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderDX11::init(RenderSystemPtr const& _renderSystem)
    {
        if (!Shader::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ShaderDX11::init(
        RenderSystemPtr const& _renderSystem,
        AssetFilePtr const& _shaderFile)
    {
        if (!init(_renderSystem))
            return false;

        if (!loadFromAssetFile(_shaderFile))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ShaderDX11::init(
        RenderSystemPtr const& _renderSystem,
        String const& _shaderSource,
        CString _shaderName)
    {
        if (!init(_renderSystem))
            return false;

        if (_shaderName)
            setName(HashedString(_shaderName));

        if (!loadFromSource(_shaderSource))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ShaderDX11::init(ShaderDX11Ptr const& _shader)
    {
        if (!Shader::init(_shader))
            return false;

        AssetFilePtr assetFile;
        if (AssetManager::GetInstancePtr())
            assetFile = AssetManager::GetInstancePtr()->getAssetFile(_shader->getName());

        if (assetFile)
        {
            if (!loadFromAssetFile(assetFile))
                return false;
        }
        else
        if (!_shader->m_vertexShaderSource.empty() && !_shader->m_fragmentShaderSource.empty())
        {
            if (!loadFromSources(
                _shader->m_vertexShaderSource,
                _shader->m_fragmentShaderSource))
                return false;
        }

        for (auto const& uniformData : _shader->m_uniformsCache)
        {
            if (uniformData.second)
                setUniform(uniformData.second->getName(), uniformData.second->getValue());
        }

        return true;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderDX11::createCopy()
    {
        return ShaderDX11::Create(cast<ShaderDX11>());
    }

    //////////////////////////////////////////
    bool ShaderDX11::isValid()
    {
        return (m_vertexShader != nullptr) && (m_pixelShader != nullptr);
    }

    //////////////////////////////////////////
    RenderSystemDX11* ShaderDX11::getRenderSystemDX11Raw() const
    {
        return m_renderSystemRaw->castRaw<RenderSystemDX11>();
    }

    //////////////////////////////////////////
    bool ShaderDX11::loadFromSource(String const& _shaderSource)
    {
        auto shaderSources = preProcessSource(_shaderSource);

        return loadFromSources(
            shaderSources[(S32)ShaderDX11Stage::Vertex],
            shaderSources[(S32)ShaderDX11Stage::Pixel]);
    }

    //////////////////////////////////////////
    bool ShaderDX11::loadFromSources(String const& _vertexShaderSource, String const& _fragmentShaderSource)
    {
        return loadDXShader(_vertexShaderSource, _fragmentShaderSource);
    }

    //////////////////////////////////////////
    void ShaderDX11::recompile()
    {
        if (!m_vertexShaderSource.empty() && !m_fragmentShaderSource.empty())
        {
            cacheUniformVariants();
            loadDXShader(m_vertexShaderSource, m_fragmentShaderSource);
            applyCachedUniformVariants();
        }
    }

    //////////////////////////////////////////
    void ShaderDX11::unloadDXShader()
    {
        StateMachineDX11* stateMachine = m_renderSystemRaw ? getRenderSystemDX11Raw()->getStateMachine() : nullptr;
        if (stateMachine && stateMachine->getCurrentShader() == this)
            stateMachine->setCurrentShader(nullptr);

        SafeReleaseDX11(m_vertexShader);
        SafeReleaseDX11(m_pixelShader);
        SafeReleaseDX11(m_vertexShaderCode);

        for (Size stage = 0; stage < (Size)ShaderDX11Stage::MAX; ++stage)
        {
            for (ShaderDX11ConstantBuffer& constantBuffer : m_constantBuffers[stage])
                SafeReleaseDX11(constantBuffer.buffer);
            m_constantBuffers[stage].clear();
        }

        m_uniformsReflection.clear();
        m_inputParameters.clear();

        clearUniformsCache();
        resetDefaultUniforms();
    }

    //////////////////////////////////////////
    bool ShaderDX11::loadDXShader(String const& _vertexShaderSource, String const& _fragmentShaderSource)
    {
        MAZE_PROFILE_EVENT("ShaderDX11::loadDXShader");

        MAZE_ERROR_RETURN_VALUE_IF(_vertexShaderSource.empty(), false, "Vertex Shader Source is empty!");
        MAZE_ERROR_RETURN_VALUE_IF(_fragmentShaderSource.empty(), false, "Fragment Shader Source is empty!");

        m_vertexShaderSource = _vertexShaderSource;
        m_fragmentShaderSource = _fragmentShaderSource;

        Debug::Log("Shader %s loading...", getName().c_str());
        Timer timer;

        unloadDXShader();

        ID3D11Device* device = getRenderSystemDX11Raw()->getDevice();

        // Vertex shader
        String vertexShaderBody = makeInternalShaderPreprocessing(_vertexShaderSource);
        String completeVertexShader = buildShaderCodeHeader(ShaderDX11Stage::Vertex) + '\n' + vertexShaderBody;
        m_vertexShaderCode = compileDXShader(completeVertexShader, ShaderDX11Stage::Vertex);
        if (!m_vertexShaderCode)
            return false;

        HRESULT hr = device->CreateVertexShader(
            m_vertexShaderCode->GetBufferPointer(),
            m_vertexShaderCode->GetBufferSize(),
            nullptr,
            &m_vertexShader);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "CreateVertexShader failed! Shader: %s hr=0x%08x", getName().c_str(), (U32)hr);

        // Pixel shader
        String fragmentShaderBody = makeInternalShaderPreprocessing(_fragmentShaderSource);
        String completeFragmentShader = buildShaderCodeHeader(ShaderDX11Stage::Pixel) + '\n' + fragmentShaderBody;
        ID3DBlob* pixelShaderCode = compileDXShader(completeFragmentShader, ShaderDX11Stage::Pixel);
        if (!pixelShaderCode)
        {
            unloadDXShader();
            return false;
        }

        hr = device->CreatePixelShader(
            pixelShaderCode->GetBufferPointer(),
            pixelShaderCode->GetBufferSize(),
            nullptr,
            &m_pixelShader);
        if (FAILED(hr))
        {
            pixelShaderCode->Release();
            unloadDXShader();
            MAZE_ERROR_RETURN_VALUE(false, "CreatePixelShader failed! Shader: %s hr=0x%08x", getName().c_str(), (U32)hr);
        }

        // Reflection
        bool reflected =
            reflectDXShader(m_vertexShaderCode, ShaderDX11Stage::Vertex) &&
            reflectDXShader(pixelShaderCode, ShaderDX11Stage::Pixel);

        pixelShaderCode->Release();

        if (!reflected)
        {
            unloadDXShader();
            return false;
        }

        m_inputSignatureId = ++s_inputSignatureCounter;

        // Prefill uniforms cache
        for (auto const& uniformReflectionData : m_uniformsReflection)
            ensureUniform(MAZE_HASHED_CSTRING(uniformReflectionData.second.name.c_str()));

        assignDefaultUniforms();
        processShaderLoaded();

        F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
        Debug::Log("Shader %s loaded for %.1fms.", getName().c_str(), msTime);

        return true;
    }

    //////////////////////////////////////////
    ID3DBlob* ShaderDX11::compileDXShader(
        String const& _source,
        ShaderDX11Stage _stage)
    {
        CString target = nullptr;
        switch (getRenderSystemDX11Raw()->getDevice()->GetFeatureLevel())
        {
            case D3D_FEATURE_LEVEL_10_0: target = (_stage == ShaderDX11Stage::Vertex) ? "vs_4_0" : "ps_4_0"; break;
            case D3D_FEATURE_LEVEL_10_1: target = (_stage == ShaderDX11Stage::Vertex) ? "vs_4_1" : "ps_4_1"; break;
            default: target = (_stage == ShaderDX11Stage::Vertex) ? "vs_5_0" : "ps_5_0"; break;
        }

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
#if (MAZE_DEBUG)
        flags |= D3DCOMPILE_DEBUG;
#else
        flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

        ID3DBlob* shaderCode = nullptr;
        ID3DBlob* errorMessages = nullptr;

        HRESULT hr = D3DCompile(
            _source.c_str(),
            _source.size(),
            getName().c_str(),
            nullptr,
            nullptr,
            "main",
            target,
            flags,
            0,
            &shaderCode,
            &errorMessages);

        if (FAILED(hr))
        {
            Debug::LogError("%s shader compilation error!", (_stage == ShaderDX11Stage::Vertex) ? "Vertex" : "Pixel");
            Debug::LogError("Shader: %s", getName().c_str());

            if (errorMessages)
                Debug::LogError("%s", (CString)errorMessages->GetBufferPointer());

            Vector<String> words;
            StringHelper::SplitWords(_source, words, '\n');
            for (Size i = 0; i < words.size(); ++i)
                Debug::LogError("[%d]%s", (S32)i, words[i].c_str());

            SafeReleaseDX11(errorMessages);
            SafeReleaseDX11(shaderCode);
            return nullptr;
        }

        if (errorMessages)
        {
            CString warnings = (CString)errorMessages->GetBufferPointer();
            if (warnings && warnings[0])
                Debug::Log("Shader %s compile warnings:\n%s", getName().c_str(), warnings);
            errorMessages->Release();
        }

        return shaderCode;
    }

    //////////////////////////////////////////
    inline static String NormalizeResourceNameDX11(CString _name)
    {
        String name = _name ? _name : "";

        // Resource arrays are reported as 'name[0]'
        Size bracketPos = name.find('[');
        if (bracketPos != String::npos)
            name = name.substr(0, bracketPos);

        return name;
    }

    //////////////////////////////////////////
    bool ShaderDX11::reflectDXShader(
        ID3DBlob* _shaderCode,
        ShaderDX11Stage _stage)
    {
        ID3D11ShaderReflection* reflection = nullptr;
        HRESULT hr = D3DReflect(
            _shaderCode->GetBufferPointer(),
            _shaderCode->GetBufferSize(),
            __uuidof(ID3D11ShaderReflection),
            (void**)&reflection);
        MAZE_ERROR_RETURN_VALUE_IF(FAILED(hr), false, "D3DReflect failed! Shader: %s hr=0x%08x", getName().c_str(), (U32)hr);

        D3D11_SHADER_DESC shaderDesc;
        reflection->GetDesc(&shaderDesc);

        Size stage = (Size)_stage;

        // Vertex input signature
        if (_stage == ShaderDX11Stage::Vertex)
        {
            m_inputParameters.clear();
            m_inputParameters.reserve(shaderDesc.InputParameters);
            for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
            {
                D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
                reflection->GetInputParameterDesc(i, &paramDesc);

                ShaderDX11InputParameter param;
                param.semanticName = paramDesc.SemanticName;
                param.semanticIndex = paramDesc.SemanticIndex;
                param.componentType = paramDesc.ComponentType;
                m_inputParameters.push_back(param);
            }
        }

        // Constant buffers
        ID3D11Device* device = getRenderSystemDX11Raw()->getDevice();
        for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
        {
            ID3D11ShaderReflectionConstantBuffer* cb = reflection->GetConstantBufferByIndex(i);
            D3D11_SHADER_BUFFER_DESC bufferDesc;
            if (FAILED(cb->GetDesc(&bufferDesc)))
                continue;

            if (bufferDesc.Type != D3D_CT_CBUFFER)
                continue;

            // Find the bind point
            U32 bindPoint = 0u;
            for (UINT r = 0; r < shaderDesc.BoundResources; ++r)
            {
                D3D11_SHADER_INPUT_BIND_DESC bindDesc;
                if (SUCCEEDED(reflection->GetResourceBindingDesc(r, &bindDesc)) &&
                    bindDesc.Type == D3D_SIT_CBUFFER &&
                    strcmp(bindDesc.Name, bufferDesc.Name) == 0)
                {
                    bindPoint = bindDesc.BindPoint;
                    break;
                }
            }

            ShaderDX11ConstantBuffer constantBuffer;
            constantBuffer.name = bufferDesc.Name;
            constantBuffer.bindPoint = bindPoint;
            constantBuffer.shadow.resize(bufferDesc.Size, 0u);
            constantBuffer.dirty = true;

            D3D11_BUFFER_DESC gpuBufferDesc;
            memset(&gpuBufferDesc, 0, sizeof(gpuBufferDesc));
            gpuBufferDesc.ByteWidth = (bufferDesc.Size + 15u) & ~15u;
            gpuBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            gpuBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            gpuBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

            hr = device->CreateBuffer(&gpuBufferDesc, nullptr, &constantBuffer.buffer);
            if (FAILED(hr))
            {
                MAZE_ERROR("Constant buffer creation failed! Shader: %s hr=0x%08x", getName().c_str(), (U32)hr);
                reflection->Release();
                return false;
            }

            S32 constantBufferIndex = (S32)m_constantBuffers[stage].size();

            // Variables
            for (UINT v = 0; v < bufferDesc.Variables; ++v)
            {
                ID3D11ShaderReflectionVariable* variable = cb->GetVariableByIndex(v);
                D3D11_SHADER_VARIABLE_DESC variableDesc;
                if (FAILED(variable->GetDesc(&variableDesc)))
                    continue;

                U32 nameHash = MAZE_HASHED_CSTRING(variableDesc.Name).hash;
                ShaderDX11UniformData& uniformData = m_uniformsReflection[nameHash];
                uniformData.name = variableDesc.Name;
                uniformData.stageBindings[stage].constantBufferIndex = constantBufferIndex;
                uniformData.stageBindings[stage].offset = variableDesc.StartOffset;
                uniformData.stageBindings[stage].size = variableDesc.Size;
            }

            m_constantBuffers[stage].push_back(constantBuffer);
        }

        // Texture and sampler resources
        for (UINT r = 0; r < shaderDesc.BoundResources; ++r)
        {
            D3D11_SHADER_INPUT_BIND_DESC bindDesc;
            if (FAILED(reflection->GetResourceBindingDesc(r, &bindDesc)))
                continue;

            if (bindDesc.Type == D3D_SIT_TEXTURE)
            {
                String name = NormalizeResourceNameDX11(bindDesc.Name);
                U32 nameHash = MAZE_HASHED_CSTRING(name.c_str()).hash;
                ShaderDX11UniformData& uniformData = m_uniformsReflection[nameHash];
                uniformData.name = name;
                uniformData.isTexture = true;
                uniformData.stageBindings[stage].srvSlot = (S32)bindDesc.BindPoint;
                uniformData.stageBindings[stage].srvCount = bindDesc.BindCount > 0u ? bindDesc.BindCount : 1u;
            }
            else
            if (bindDesc.Type == D3D_SIT_SAMPLER)
            {
                String name = NormalizeResourceNameDX11(bindDesc.Name);

                // Samplers follow the '<textureName>Sampler' convention
                static String const c_samplerSuffix = "Sampler";
                if (name.size() > c_samplerSuffix.size() &&
                    name.compare(name.size() - c_samplerSuffix.size(), c_samplerSuffix.size(), c_samplerSuffix) == 0)
                {
                    String textureName = name.substr(0, name.size() - c_samplerSuffix.size());
                    U32 nameHash = MAZE_HASHED_CSTRING(textureName.c_str()).hash;
                    ShaderDX11UniformData& uniformData = m_uniformsReflection[nameHash];
                    uniformData.name = textureName;
                    uniformData.isTexture = true;
                    uniformData.stageBindings[stage].samplerSlot = (S32)bindDesc.BindPoint;
                }
            }
        }

        reflection->Release();
        return true;
    }

    //////////////////////////////////////////
    String ShaderDX11::buildShaderCodeHeader(ShaderDX11Stage _stage)
    {
        String header;
        header.reserve(4096);

        header += "#define MAZE_HLSL (1)\n";
        header += "#define MAZE_LHCS (1)\n";
        header += "#define MAZE_MODEL_MATRICES_VIA_ARRAY (1)\n";
        header += "#define MAZE_MODEL_MATRICES_VIA_TEXTURE (0)\n";
        header += "#define MAZE_INSTANCE_STREAM_VIA_ARRAY (1)\n";
        header += "#define MAZE_INSTANCE_STREAM_VIA_TEXTURE (0)\n";
        header += "#define MAZE_SKELETON_BONES_MAX " + StringHelper::ToString(MAZE_SKELETON_BONES_MAX) + "\n";
        header += "#define MAZE_DYNAMIC_LIGHTS_MAX " + StringHelper::ToString(MAZE_DYNAMIC_LIGHTS_MAX) + "\n";

        if (_stage == ShaderDX11Stage::Vertex)
            header += "#define MAZE_VERTEX_SHADER (1)\n";
        else
            header += "#define MAZE_PIXEL_SHADER (1)\n";

        // Features
        header += m_renderSystemRaw->getShaderSystem()->ensureGlobalFeaturesString() + '\n';
        header += buildLocalShaderFeatures();

        header += c_commonShaderHeaderDX11;

        return header;
    }

    //////////////////////////////////////////
    String ShaderDX11::buildLocalShaderFeatures()
    {
        String result;

        for (auto const& localFeatureData : m_localFeatures)
            result += (String)"#define " + localFeatureData.first + " " + localFeatureData.second + '\n';

        return result;
    }

    //////////////////////////////////////////
    void ShaderDX11::ClearShaderIncludeFilesCache()
    {
        s_shaderIncludeFilesCacheDX11.clear();
    }

    //////////////////////////////////////////
    String ShaderDX11::makeInternalShaderPreprocessing(String _shader)
    {
        Size index = 0;
        do
        {
            index = _shader.find("#include");
            if (index != String::npos)
            {
                Size endOfLineIndex = _shader.find_first_of('\n', index);
                MAZE_DEBUG_BP_IF(endOfLineIndex == String::npos);
                Size symbols = endOfLineIndex - index;
                String line = _shader.substr(index, symbols);

                String fileName = line.substr(line.find_first_of('"') + 1, line.find_last_of('"') - line.find_first_of('"') - 1);
                Size fileNameLastSlashIndex = fileName.find_last_of('/');
                if (fileNameLastSlashIndex != String::npos)
                    fileName = fileName.substr(fileNameLastSlashIndex + 1, fileName.size() - fileNameLastSlashIndex - 1);

                _shader.erase(index, symbols);

                String fileText;

                auto cacheIt = s_shaderIncludeFilesCacheDX11.find(fileName);
                if (cacheIt != s_shaderIncludeFilesCacheDX11.end())
                {
                    fileText = cacheIt->second;
                }
                else
                {
                    AssetFilePtr const& shaderFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(fileName);

                    if (shaderFile)
                        shaderFile->readToString(fileText);

                    s_shaderIncludeFilesCacheDX11.emplace(fileName, fileText);
                }

                if (fileText.size())
                    _shader.insert(index, fileText);
                else
                {
                    MAZE_ERROR("Undefined shader file include: %s!", fileName.c_str());
                }
            }

        }
        while (index != String::npos);

        return _shader;
    }

    //////////////////////////////////////////
    UnorderedMap<S32, String> ShaderDX11::preProcessSource(String const& _source)
    {
        UnorderedMap<S32, String> shaderSources;

        CString typeToken = "#type";
        Size typeTokenLength = strlen(typeToken);
        Size pos = _source.find(typeToken, 0);
        while (pos != String::npos)
        {
            Size eol = _source.find_first_of("\r\n", pos);
            MAZE_ERROR_IF(eol == String::npos, "Syntax error");
            Size begin = pos + typeTokenLength + 1;
            String type = _source.substr(begin, eol - begin);
            MAZE_ERROR_IF(ShaderStageFromString(type) < 0, "Invalid shader type specified");

            Size nextLinePos = _source.find_first_not_of("\r\n", eol);
            MAZE_ERROR_IF(nextLinePos == String::npos, "Syntax error");
            pos = _source.find(typeToken, nextLinePos);

            shaderSources[ShaderStageFromString(type)] = (pos == String::npos) ? _source.substr(nextLinePos) : _source.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    //////////////////////////////////////////
    ShaderDX11UniformData const* ShaderDX11::getUniformData(HashedCString _uniformName) const
    {
        auto it = m_uniformsReflection.find(_uniformName.hash);
        if (it == m_uniformsReflection.end())
            return nullptr;

        return &it->second;
    }

    //////////////////////////////////////////
    ShaderUniformPtr const& ShaderDX11::createUniformFromShader(HashedCString _uniformName, ShaderUniformType _type)
    {
        static ShaderUniformPtr const nullPointer;

        UnorderedMap<U32, ShaderUniformPtr>::const_iterator it = m_uniformsCache.find(_uniformName.hash);
        if (it != m_uniformsCache.end())
            return it->second;

        ShaderDX11UniformData const* uniformData = getUniformData(_uniformName);
        if (!uniformData)
        {
            // Mark with empty pointer
            m_uniformsCache.emplace(_uniformName.hash, nullptr);
            return nullPointer;
        }

        ShaderUniformDX11Ptr newUniform = static_pointer_cast<ShaderUniformDX11>(ShaderUniform::Create(getSharedPtr(), _type));
        MAZE_ERROR_RETURN_VALUE_IF(!newUniform, nullPointer, "Shader Uniform creation error!");
        newUniform->setName(_uniformName);
        newUniform->setUniformData(*uniformData);
        auto at = m_uniformsCache.emplace(_uniformName.hash, newUniform);
        if (at.second)
            return at.first->second;

        return nullPointer;
    }

    //////////////////////////////////////////
    void ShaderDX11::writeUniformData(
        ShaderDX11UniformData const& _uniformData,
        U8 const* _bytes,
        U32 _bytesCount)
    {
        for (Size stage = 0; stage < (Size)ShaderDX11Stage::MAX; ++stage)
        {
            ShaderDX11UniformData::StageBinding const& binding = _uniformData.stageBindings[stage];
            if (binding.constantBufferIndex < 0)
                continue;

            if (binding.constantBufferIndex >= (S32)m_constantBuffers[stage].size())
                continue;

            ShaderDX11ConstantBuffer& constantBuffer = m_constantBuffers[stage][binding.constantBufferIndex];

            U32 bytesCount = Math::Min(_bytesCount, binding.size);
            if (binding.offset + bytesCount > (U32)constantBuffer.shadow.size())
                bytesCount = (U32)constantBuffer.shadow.size() - binding.offset;

            if (bytesCount == 0u)
                continue;

            if (memcmp(&constantBuffer.shadow[binding.offset], _bytes, bytesCount) == 0)
                continue;

            memcpy(&constantBuffer.shadow[binding.offset], _bytes, bytesCount);
            constantBuffer.dirty = true;
        }
    }

    //////////////////////////////////////////
    void ShaderDX11::flushConstantBuffers()
    {
        ID3D11DeviceContext* deviceContext = getRenderSystemDX11Raw()->getDeviceContext();

        for (Size stage = 0; stage < (Size)ShaderDX11Stage::MAX; ++stage)
        {
            for (ShaderDX11ConstantBuffer& constantBuffer : m_constantBuffers[stage])
            {
                if (!constantBuffer.buffer)
                    continue;

                if (constantBuffer.dirty)
                {
                    D3D11_MAPPED_SUBRESOURCE mapped;
                    HRESULT hr = deviceContext->Map(constantBuffer.buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
                    if (SUCCEEDED(hr))
                    {
                        memcpy(mapped.pData, &constantBuffer.shadow[0], constantBuffer.shadow.size());
                        deviceContext->Unmap(constantBuffer.buffer, 0);
                    }
                    constantBuffer.dirty = false;
                }

                if (stage == (Size)ShaderDX11Stage::Vertex)
                    deviceContext->VSSetConstantBuffers(constantBuffer.bindPoint, 1, &constantBuffer.buffer);
                else
                    deviceContext->PSSetConstantBuffers(constantBuffer.bindPoint, 1, &constantBuffer.buffer);
            }
        }
    }

    //////////////////////////////////////////
    void ShaderDX11::bindTextures()
    {
        StateMachineDX11* stateMachine = getRenderSystemDX11Raw()->getStateMachine();

        for (auto& uniformCacheData : m_uniformsCache)
        {
            ShaderUniformPtr const& uniform = uniformCacheData.second;

            if (!uniform)
                continue;

            ShaderUniformDX11* uniformDX11 = uniform->castRaw<ShaderUniformDX11>();
            ShaderDX11UniformData const& uniformData = uniformDX11->getUniformData();
            if (!uniformData.isTexture)
                continue;

            for (Size stage = 0; stage < (Size)ShaderDX11Stage::MAX; ++stage)
            {
                ShaderDX11UniformData::StageBinding const& binding = uniformData.stageBindings[stage];
                if (binding.srvSlot < 0)
                    continue;

                if (uniform->getType() == ShaderUniformType::UniformTexture2D)
                {
                    ID3D11ShaderResourceView* srv = nullptr;
                    ID3D11SamplerState* sampler = nullptr;

                    Texture2D* texture = uniform->getTexture2DRaw();
                    if (texture)
                    {
                        Texture2DDX11* textureDX11 = texture->castRaw<Texture2DDX11>();
                        srv = textureDX11->getShaderResourceView();
                        sampler = textureDX11->ensureSamplerState();
                    }

                    if (stage == (Size)ShaderDX11Stage::Vertex)
                        stateMachine->bindVertexShaderResource(binding.srvSlot, srv, binding.samplerSlot, sampler);
                    else
                        stateMachine->bindPixelShaderResource(binding.srvSlot, srv, binding.samplerSlot, sampler);
                }
                else
                if (uniform->getType() == ShaderUniformType::UniformTextureCube)
                {
                    ID3D11ShaderResourceView* srv = nullptr;
                    ID3D11SamplerState* sampler = nullptr;

                    TextureCube* texture = uniform->getTextureCubeRaw();
                    if (texture)
                    {
                        TextureCubeDX11* textureDX11 = texture->castRaw<TextureCubeDX11>();
                        srv = textureDX11->getShaderResourceView();
                        sampler = textureDX11->ensureSamplerState();
                    }

                    if (stage == (Size)ShaderDX11Stage::Vertex)
                        stateMachine->bindVertexShaderResource(binding.srvSlot, srv, binding.samplerSlot, sampler);
                    else
                        stateMachine->bindPixelShaderResource(binding.srvSlot, srv, binding.samplerSlot, sampler);
                }
                else
                if (uniform->getType() == ShaderUniformType::UniformTexture2DArray)
                {
                    U32 count = Math::Min(uniform->getCount(), binding.srvCount);
                    for (U32 i = 0; i < count; ++i)
                    {
                        ID3D11ShaderResourceView* srv = nullptr;
                        ID3D11SamplerState* sampler = nullptr;

                        Texture2D* texture = *((Texture2D**)uniform->getPtr() + i);
                        if (texture)
                        {
                            Texture2DDX11* textureDX11 = texture->castRaw<Texture2DDX11>();
                            srv = textureDX11->getShaderResourceView();
                            sampler = textureDX11->ensureSamplerState();
                        }

                        S32 samplerSlot = binding.samplerSlot >= 0 ? binding.samplerSlot + (S32)i : -1;

                        if (stage == (Size)ShaderDX11Stage::Vertex)
                            stateMachine->bindVertexShaderResource(binding.srvSlot + (S32)i, srv, samplerSlot, sampler);
                        else
                            stateMachine->bindPixelShaderResource(binding.srvSlot + (S32)i, srv, samplerSlot, sampler);
                    }
                }
            }
        }
    }


} // namespace Maze
//////////////////////////////////////////
