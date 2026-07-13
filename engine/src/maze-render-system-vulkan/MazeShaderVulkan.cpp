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
#include "MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeShaderVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderUniformVulkan.hpp"
#include "maze-render-system-vulkan/MazeShaderManagerVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeStateMachineVulkan.hpp"
#include "maze-render-system-vulkan/MazeTexture2DVulkan.hpp"
#include "maze-render-system-vulkan/MazeTextureCubeVulkan.hpp"
#include "maze-graphics/managers/MazeTextureManager.hpp"
#include "maze-core/managers/MazeAssetManager.hpp"
#include "maze-core/assets/MazeAssetFile.hpp"
#include "maze-core/helpers/MazeStringHelper.hpp"
#include "maze-core/system/MazeTimer.hpp"
#include "maze-core/services/MazeLogStream.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-graphics/config/MazeGraphicsConfig.hpp"

#include <shaderc/shaderc.hpp>
#include <spirv_reflect.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static String const c_commonShaderHeaderVulkan =
#include "shaders/MazeCommonShaderHeader.mzglslvk"
    ;

    //////////////////////////////////////////
    static UnorderedMap<String, String> s_shaderIncludeFilesCacheVulkan;

    //////////////////////////////////////////
    inline static S32 ShaderStageFromString(String const& _type)
    {
        if (_type == "vertex")
            return (S32)ShaderVulkanStage::Vertex;
        if (_type == "fragment" || _type == "pixel")
            return (S32)ShaderVulkanStage::Fragment;

        MAZE_ERROR("Unknown shader type - '%s'!", _type.c_str());
        return -1;
    }

    //////////////////////////////////////////
    inline static String NormalizeResourceNameVulkan(CString _name)
    {
        String name = _name ? _name : "";
        Size bracketPos = name.find('[');
        if (bracketPos != String::npos)
            name = name.substr(0, bracketPos);
        return name;
    }


    //////////////////////////////////////////
    // Class ShaderVulkan
    //
    //////////////////////////////////////////
    MAZE_IMPLEMENT_METACLASS_WITH_PARENT(ShaderVulkan, Shader);

    //////////////////////////////////////////
    ShaderVulkan::ShaderVulkan()
    {
        for (S32 i = 0; i < (S32)VertexAttributeSemantic::MAX; ++i)
            m_vertexInputLocations[i] = -1;
    }

    //////////////////////////////////////////
    ShaderVulkan::~ShaderVulkan()
    {
        unloadVulkanShader();
    }

    //////////////////////////////////////////
    ShaderPtr ShaderVulkan::Create(RenderSystemPtr const& _renderSystem)
    {
        ShaderVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderVulkan, object, init(_renderSystem));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderVulkan::CreateFromFile(
        RenderSystemPtr const& _renderSystem,
        AssetFilePtr const& _shaderFile)
    {
        ShaderVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderVulkan, object, init(_renderSystem, _shaderFile));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderVulkan::CreateFromSource(
        RenderSystemPtr const& _renderSystem,
        String const& _shaderSource,
        CString _shaderName)
    {
        ShaderVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderVulkan, object, init(_renderSystem, _shaderSource, _shaderName));
        return object;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderVulkan::Create(ShaderVulkanPtr const& _shader)
    {
        ShaderVulkanPtr object;
        MAZE_CREATE_AND_INIT_SHARED_PTR(ShaderVulkan, object, init(_shader));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderVulkan::init(RenderSystemPtr const& _renderSystem)
    {
        if (!Shader::init(_renderSystem))
            return false;

        return true;
    }

    //////////////////////////////////////////
    bool ShaderVulkan::init(
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
    bool ShaderVulkan::init(
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
    bool ShaderVulkan::init(ShaderVulkanPtr const& _shader)
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

        for (auto const& uniformData : _shader->m_uniforms)
        {
            if (uniformData.second)
                setUniform(uniformData.second->getName(), uniformData.second->getValue());
        }

        return true;
    }

    //////////////////////////////////////////
    ShaderPtr ShaderVulkan::createCopy()
    {
        return ShaderVulkan::Create(cast<ShaderVulkan>());
    }

    //////////////////////////////////////////
    bool ShaderVulkan::isValid()
    {
        return (m_vertexModule != VK_NULL_HANDLE) && (m_fragmentModule != VK_NULL_HANDLE);
    }

    //////////////////////////////////////////
    RenderSystemVulkan* ShaderVulkan::getRenderSystemVulkanRaw() const
    {
        return m_renderSystemRaw->castRaw<RenderSystemVulkan>();
    }

    //////////////////////////////////////////
    S32 ShaderVulkan::getVertexInputLocation(VertexAttributeSemantic _semantic) const
    {
        return m_vertexInputLocations[(S32)_semantic];
    }

    //////////////////////////////////////////
    bool ShaderVulkan::loadFromSource(String const& _shaderSource)
    {
        auto shaderSources = preProcessSource(_shaderSource);

        return loadFromSources(
            shaderSources[(S32)ShaderVulkanStage::Vertex],
            shaderSources[(S32)ShaderVulkanStage::Fragment]);
    }

    //////////////////////////////////////////
    bool ShaderVulkan::loadFromSources(String const& _vertexShaderSource, String const& _fragmentShaderSource)
    {
        return loadVulkanShader(_vertexShaderSource, _fragmentShaderSource);
    }

    //////////////////////////////////////////
    void ShaderVulkan::recompile()
    {
        if (!m_vertexShaderSource.empty() && !m_fragmentShaderSource.empty())
        {
            cacheUniformVariants();
            loadVulkanShader(m_vertexShaderSource, m_fragmentShaderSource);
            applyCachedUniformVariants();
        }
    }

    //////////////////////////////////////////
    void ShaderVulkan::unloadVulkanShader()
    {
        RenderSystemVulkan* rs = m_renderSystemRaw ? getRenderSystemVulkanRaw() : nullptr;

        StateMachineVulkan* stateMachine = rs ? rs->getStateMachine() : nullptr;
        if (stateMachine && stateMachine->getCurrentShader() == this)
            stateMachine->setCurrentShader(nullptr);

        if (rs)
        {
            VkDevice device = rs->getDevice();

            if (m_vertexModule != VK_NULL_HANDLE)
                vkDestroyShaderModule(device, m_vertexModule, nullptr);
            if (m_fragmentModule != VK_NULL_HANDLE)
                vkDestroyShaderModule(device, m_fragmentModule, nullptr);

            if (m_pipelineLayout != VK_NULL_HANDLE)
                vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
            if (m_materialSetLayout != VK_NULL_HANDLE)
                vkDestroyDescriptorSetLayout(device, m_materialSetLayout, nullptr);

            // These buffers were created with VMA_ALLOCATION_CREATE_MAPPED_BIT -
            // VMA maps them once for their whole lifetime and unmaps
            // automatically inside vmaDestroyBuffer; calling vmaUnmapMemory
            // on them manually is invalid (they were never mapped through
            // the explicit vmaMapMemory/vmaUnmapMemory ref-counted path) and
            // trips VMA's "Unmapping allocation not previously mapped" assert.
            for (Vector<MaterialUniformInstance> const& framePool : m_materialInstancePool)
                for (MaterialUniformInstance const& inst : framePool)
                    vmaDestroyBuffer(rs->getAllocator(), inst.buffer, inst.allocation);
            // Descriptor sets are freed wholesale with the pool at shutdown - not
            // individually freed here
        }

        m_vertexModule = VK_NULL_HANDLE;
        m_fragmentModule = VK_NULL_HANDLE;
        m_pipelineLayout = VK_NULL_HANDLE;
        m_materialSetLayout = VK_NULL_HANDLE;
        m_materialInstancePool.clear();
        m_materialInstancePoolUsed.clear();
        m_materialInstancePoolGeneration.clear();
        m_materialUniformBufferSize = 0u;
        m_materialUniformShadow.clear();
        m_textureBindings.clear();

        for (S32 i = 0; i < (S32)VertexAttributeSemantic::MAX; ++i)
            m_vertexInputLocations[i] = -1;

        m_uniformsReflection.clear();

        clearUniformsCache();
        resetDefaultUniforms();
    }

    //////////////////////////////////////////
    bool ShaderVulkan::loadVulkanShader(String const& _vertexShaderSource, String const& _fragmentShaderSource)
    {
        MAZE_PROFILE_EVENT("ShaderVulkan::loadVulkanShader");

        MAZE_ERROR_RETURN_VALUE_IF(_vertexShaderSource.empty(), false, "Vertex Shader Source is empty!");
        MAZE_ERROR_RETURN_VALUE_IF(_fragmentShaderSource.empty(), false, "Fragment Shader Source is empty!");

        m_vertexShaderSource = _vertexShaderSource;
        m_fragmentShaderSource = _fragmentShaderSource;

        Debug::Log("Shader %s loading...", getName().c_str());
        Timer timer;

        unloadVulkanShader();

        RenderSystemVulkan* rs = getRenderSystemVulkanRaw();
        VkDevice device = rs->getDevice();

        // Vertex stage
        String vertexBody = makeInternalShaderPreprocessing(_vertexShaderSource);
        String completeVertexShader = c_commonShaderHeaderVulkan + '\n' + buildLocalShaderFeatures() + '\n' + "#define MAZE_VERTEX_SHADER (1)\n" + vertexBody;
        Vector<U32> vertexSpirv = compileVulkanShader(completeVertexShader, ShaderVulkanStage::Vertex);
        if (vertexSpirv.empty())
            return false;

        VkShaderModuleCreateInfo vertexModuleInfo = {};
        vertexModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vertexModuleInfo.codeSize = vertexSpirv.size() * sizeof(U32);
        vertexModuleInfo.pCode = vertexSpirv.data();
        MAZE_VK_CALL(vkCreateShaderModule(device, &vertexModuleInfo, nullptr, &m_vertexModule));
        if (m_vertexModule == VK_NULL_HANDLE)
            return false;

        // Fragment stage
        String fragmentBody = makeInternalShaderPreprocessing(_fragmentShaderSource);
        String completeFragmentShader = c_commonShaderHeaderVulkan + '\n' + buildLocalShaderFeatures() + '\n' + "#define MAZE_FRAGMENT_SHADER (1)\n" + fragmentBody;
        Vector<U32> fragmentSpirv = compileVulkanShader(completeFragmentShader, ShaderVulkanStage::Fragment);
        if (fragmentSpirv.empty())
        {
            unloadVulkanShader();
            return false;
        }

        VkShaderModuleCreateInfo fragmentModuleInfo = {};
        fragmentModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        fragmentModuleInfo.codeSize = fragmentSpirv.size() * sizeof(U32);
        fragmentModuleInfo.pCode = fragmentSpirv.data();
        MAZE_VK_CALL(vkCreateShaderModule(device, &fragmentModuleInfo, nullptr, &m_fragmentModule));
        if (m_fragmentModule == VK_NULL_HANDLE)
        {
            unloadVulkanShader();
            return false;
        }

        // Reflection
        bool reflected =
            reflectVulkanShader(vertexSpirv, ShaderVulkanStage::Vertex) &&
            reflectVulkanShader(fragmentSpirv, ShaderVulkanStage::Fragment);

        if (!reflected)
        {
            unloadVulkanShader();
            return false;
        }

        if (!createDescriptorSetLayoutsAndPipelineLayout())
        {
            unloadVulkanShader();
            return false;
        }

        if (!createMaterialUniformBuffers())
        {
            unloadVulkanShader();
            return false;
        }

        // Prefill uniforms cache
        for (auto const& uniformReflectionData : m_uniformsReflection)
            createUniformFromShader(MAZE_HASHED_CSTRING(uniformReflectionData.second.name.c_str()));

        assignDefaultUniforms();
        processShaderLoaded();

        F32 msTime = F32(timer.getMicroseconds()) / 1000.0f;
        Debug::Log("Shader %s loaded for %.1fms.", getName().c_str(), msTime);

        return true;
    }

    //////////////////////////////////////////
    Vector<U32> ShaderVulkan::compileVulkanShader(
        String const& _source,
        ShaderVulkanStage _stage)
    {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetSourceLanguage(shaderc_source_language_glsl);
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
        options.SetTargetSpirv(shaderc_spirv_version_1_6);
#if (MAZE_DEBUG)
        options.SetOptimizationLevel(shaderc_optimization_level_zero);
        options.SetGenerateDebugInfo();
#else
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif

        shaderc_shader_kind kind = (_stage == ShaderVulkanStage::Vertex)
            ? shaderc_glsl_vertex_shader
            : shaderc_glsl_fragment_shader;

        // Pass raw pointer+size rather than _source directly - Maze's String
        // is not std::string, so it doesn't implicitly convert to the
        // std::string-taking overload
        shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(
            _source.c_str(), _source.size(), kind, getName().c_str(), options);

        if (result.GetCompilationStatus() != shaderc_compilation_status_success)
        {
            Debug::LogError("%s shader compilation error!", (_stage == ShaderVulkanStage::Vertex) ? "Vertex" : "Fragment");
            Debug::LogError("Shader: %s", getName().c_str());
            Debug::LogError("%s", result.GetErrorMessage().c_str());

            Vector<String> words;
            StringHelper::SplitWords(_source, words, '\n');
            for (Size i = 0; i < words.size(); ++i)
                Debug::LogError("[%d]%s", (S32)i, words[i].c_str());

            return Vector<U32>();
        }

        return Vector<U32>(result.cbegin(), result.cend());
    }

    //////////////////////////////////////////
    bool ShaderVulkan::reflectVulkanShader(
        Vector<U32> const& _spirv,
        ShaderVulkanStage _stage)
    {
        SpvReflectShaderModule module;
        SpvReflectResult spvResult = spvReflectCreateShaderModule(
            _spirv.size() * sizeof(U32), _spirv.data(), &module);
        MAZE_ERROR_RETURN_VALUE_IF(spvResult != SPV_REFLECT_RESULT_SUCCESS, false,
            "spvReflectCreateShaderModule failed! Shader: %s", getName().c_str());

        // Vertex input locations (vertex stage only)
        if (_stage == ShaderVulkanStage::Vertex)
        {
            U32 inputCount = 0u;
            spvReflectEnumerateInputVariables(&module, &inputCount, nullptr);
            Vector<SpvReflectInterfaceVariable*> inputs(inputCount);
            if (inputCount > 0u)
                spvReflectEnumerateInputVariables(&module, &inputCount, inputs.data());

            for (SpvReflectInterfaceVariable* input : inputs)
            {
                if (!input || !input->name)
                    continue;

                // Vertex attributes are declared 'in vec3 a_position;' etc -
                // match by name against the engine's semantic naming
                // convention (a_position, a_normal, a_tangent, a_color,
                // a_texCoords0, a_texCoords1, a_blendWeights, a_blendIndices...)
                VertexAttributeSemantic semantic = VertexAttributeSemantic::MAX;
                String name = input->name;
                if (name.size() > 0 && name[0] == 'a' && name.size() > 1 && name[1] == '_')
                {
                    // Resolved by the C++ integrator against
                    // MazeVertex.hpp's VertexAttributeSemantic name table -
                    // see GetVertexAttributeSemanticByShaderName in
                    // MazeHelpersVulkan (added alongside GetVertexAttributeFormatVulkan)
                    semantic = GetVertexAttributeSemanticByShaderNameVulkan(name);
                }

                if (semantic != VertexAttributeSemantic::MAX)
                    m_vertexInputLocations[(S32)semantic] = (S32)input->location;
            }
        }

        // Descriptor bindings (both sets - set 0 members are still recorded
        // with isGlobal=true so createUniformFromShader/writeUniformData can
        // route them into the shared global UBO instead of this shader's own)
        U32 bindingCount = 0u;
        spvReflectEnumerateDescriptorBindings(&module, &bindingCount, nullptr);
        Vector<SpvReflectDescriptorBinding*> bindings(bindingCount);
        if (bindingCount > 0u)
            spvReflectEnumerateDescriptorBindings(&module, &bindingCount, bindings.data());

        for (SpvReflectDescriptorBinding* binding : bindings)
        {
            if (!binding)
                continue;

            bool isGlobalSet = (binding->set == (U32)c_vulkanGlobalDescriptorSet);
            bool isMaterialSet = (binding->set == (U32)c_vulkanMaterialDescriptorSet);
            if (!isGlobalSet && !isMaterialSet)
                continue;

            if (binding->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            {
                // Walk block members - each becomes a named uniform, offset
                // relative to the start of this UBO
                SpvReflectBlockVariable const& block = binding->block;
                for (U32 m = 0; m < block.member_count; ++m)
                {
                    SpvReflectBlockVariable const& member = block.members[m];
                    if (!member.name)
                        continue;

                    U32 nameHash = MAZE_HASHED_CSTRING(member.name).hash;
                    ShaderVulkanUniformData& uniformData = m_uniformsReflection[nameHash];
                    uniformData.name = member.name;
                    uniformData.isGlobal = isGlobalSet;
                    uniformData.isTexture = false;
                    uniformData.offset = member.offset;
                    uniformData.size = member.size;

                    if (isGlobalSet)
                        getRenderSystemVulkanRaw()->ensureGlobalUniformBufferSize(member.offset + member.size);
                    else
                        m_materialUniformBufferSize = Math::Max(m_materialUniformBufferSize, member.offset + member.size);
                }
            }
            else
            if (isMaterialSet &&
                (binding->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                 binding->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE))
            {
                String name = NormalizeResourceNameVulkan(binding->name);
                U32 nameHash = MAZE_HASHED_CSTRING(name.c_str()).hash;
                ShaderVulkanUniformData& uniformData = m_uniformsReflection[nameHash];
                uniformData.name = name;
                uniformData.isGlobal = false;
                uniformData.isTexture = true;
                uniformData.textureBinding = (S32)binding->binding;

                if ((S32)m_textureBindings.size() <= uniformData.textureBinding)
                    m_textureBindings.resize(uniformData.textureBinding + 1);

                // Needed so a default/fallback descriptor write (see
                // createMaterialUniformBuffers()) picks a matching
                // VkImageViewType - writing a 2D fallback into a samplerCube
                // binding trips a validation error on every draw
                m_textureBindings[uniformData.textureBinding].isCube = (binding->image.dim == SpvDimCube);
            }
            // Set-0 storage-buffer instance streams (ModelMatrices/Colors/UV)
            // are bound once by RenderSystemVulkan/InstanceStreamsVulkan and
            // are not exposed as regular named ShaderUniforms - RenderQueueVulkan
            // writes them directly, see MazeInstanceStreamsVulkan.cpp
        }

        // Collapse "<base>N" (N = '0'..'7') sibling texture bindings created
        // by the loop above into a single logical UniformTexture2DArray
        // uniform named "<base>" - see ShaderVulkanUniformData::
        // isTextureArray's banner comment. This is how multi-page font
        // materials (MazeFontShader.mzglslvk, which declares
        // u_baseMaps0..u_baseMaps7 instead of a real GLSL sampler array -
        // see that shader's own comment for why) get exposed to C++ code
        // that sets a single "u_baseMaps" uniform via
        // Material::ensureUniform("u_baseMaps")->set(textures, count) (see
        // MazeFontMaterial.cpp), matching what GL/DX11 expose for the same
        // name. Without this, that ensureUniform("u_baseMaps") lookup never
        // resolves against this shader's reflection data at all (only
        // "u_baseMaps0".."u_baseMaps7" exist), so the font's texture is
        // never actually bound and the text renders fully transparent.
        {
            struct IndexedBinding { S32 index; S32 binding; };
            UnorderedMap<String, Vector<IndexedBinding>> arrayCandidates;
            for (auto const& kv : m_uniformsReflection)
            {
                ShaderVulkanUniformData const& data = kv.second;
                if (!data.isTexture || data.isTextureArray)
                    continue;

                String const& name = data.name;
                if (name.size() < 2)
                    continue;

                char last = name.back();
                if (last < '0' || last > '7')
                    continue;

                arrayCandidates[name.substr(0, name.size() - 1)].push_back({ (S32)(last - '0'), data.textureBinding });
            }

            for (auto& candidate : arrayCandidates)
            {
                // A lone name that happens to end in a digit isn't an array -
                // leave it as its own scalar uniform
                if (candidate.second.size() < 2)
                    continue;

                U32 baseNameHash = MAZE_HASHED_CSTRING(candidate.first.c_str()).hash;
                ShaderVulkanUniformData& arrayData = m_uniformsReflection[baseNameHash];
                arrayData.name = candidate.first;
                arrayData.isGlobal = false;
                arrayData.isTexture = true;
                arrayData.isTextureArray = true;
                arrayData.textureBindingsArray.assign(8, -1);
                for (IndexedBinding const& indexedBinding : candidate.second)
                    if (indexedBinding.index >= 0 && indexedBinding.index < 8)
                        arrayData.textureBindingsArray[indexedBinding.index] = indexedBinding.binding;
            }
        }

        spvReflectDestroyShaderModule(&module);
        return true;
    }

    //////////////////////////////////////////
    bool ShaderVulkan::createDescriptorSetLayoutsAndPipelineLayout()
    {
        RenderSystemVulkan* rs = getRenderSystemVulkanRaw();
        VkDevice device = rs->getDevice();

        Vector<VkDescriptorSetLayoutBinding> bindings;

        VkDescriptorSetLayoutBinding uboBinding = {};
        uboBinding.binding = 0u;
        uboBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboBinding.descriptorCount = 1u;
        uboBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        bindings.push_back(uboBinding);

        // m_textureBindings is indexed by the raw SPIR-V binding number
        // (reflectVulkanShader()/setTextureBinding() both write
        // m_textureBindings[spirvBinding] directly) - binding 0 is always the
        // MaterialUniforms UBO, so index 0 here is structurally always an
        // unused placeholder slot (nothing ever reflects a texture at
        // binding 0) and must be skipped, not remapped to binding 0 (which
        // would collide with the UBO) or offset by +1 (which shifted every
        // real texture binding declared here, and every write in
        // acquireMaterialDescriptorSet() below, one slot past where the
        // shader's SPIR-V actually expects it)
        for (Size i = 1; i < m_textureBindings.size(); ++i)
        {
            VkDescriptorSetLayoutBinding samplerBinding = {};
            samplerBinding.binding = (U32)i;
            samplerBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            samplerBinding.descriptorCount = 1u;
            samplerBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
            bindings.push_back(samplerBinding);
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = (U32)bindings.size();
        layoutInfo.pBindings = bindings.data();
        MAZE_VK_CALL(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_materialSetLayout));

        VkDescriptorSetLayout setLayouts[2] = { rs->getGlobalDescriptorSetLayout(), m_materialSetLayout };

        VkPushConstantRange pushConstantRange = {};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0u;
        pushConstantRange.size = sizeof(S32); // u_instanceOffset

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 2u;
        pipelineLayoutInfo.pSetLayouts = setLayouts;
        pipelineLayoutInfo.pushConstantRangeCount = 1u;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        MAZE_VK_CALL(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout));

        return m_pipelineLayout != VK_NULL_HANDLE;
    }

    //////////////////////////////////////////
    bool ShaderVulkan::createMaterialUniformBuffers()
    {
        RenderSystemVulkan* rs = getRenderSystemVulkanRaw();
        U32 framesInFlight = rs->getFramesInFlight();

        // Round up to 256 to keep alignment-friendly (min UBO alignment on
        // most desktop GPUs is <= 256, this avoids a device-query round-trip
        // at the cost of a little wasted memory)
        U32 bufferSize = Math::Max((U32)((m_materialUniformBufferSize + 255u) & ~255u), 256u);
        m_materialUniformBufferSize = bufferSize;
        m_materialUniformShadow.resize(bufferSize, 0u);

        // Actual UBO/descriptor-set instances are created lazily, on demand,
        // by acquireMaterialDescriptorSet() - just set up the per-frame-in-flight
        // pool bookkeeping here
        m_materialInstancePool.clear();
        m_materialInstancePool.resize(framesInFlight);
        m_materialInstancePoolUsed.assign(framesInFlight, 0u);
        m_materialInstancePoolGeneration.assign(framesInFlight, ~0ull);

        return true;
    }

    //////////////////////////////////////////
    String ShaderVulkan::buildLocalShaderFeatures()
    {
        String result;
        result += m_renderSystemRaw->getShaderManager()->ensureGlobalFeaturesString() + '\n';

        for (auto const& localFeatureData : m_localFeatures)
            result += (String)"#define " + localFeatureData.first + " " + localFeatureData.second + '\n';

        return result;
    }

    //////////////////////////////////////////
    void ShaderVulkan::ClearShaderIncludeFilesCache()
    {
        s_shaderIncludeFilesCacheVulkan.clear();
    }

    //////////////////////////////////////////
    String ShaderVulkan::makeInternalShaderPreprocessing(String _shader)
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

                auto cacheIt = s_shaderIncludeFilesCacheVulkan.find(fileName);
                if (cacheIt != s_shaderIncludeFilesCacheVulkan.end())
                {
                    fileText = cacheIt->second;
                }
                else
                {
                    AssetFilePtr const& shaderFile = AssetManager::GetInstancePtr()->getAssetFileByFileName(fileName);

                    if (shaderFile)
                        shaderFile->readToString(fileText);

                    s_shaderIncludeFilesCacheVulkan.emplace(fileName, fileText);
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
    UnorderedMap<S32, String> ShaderVulkan::preProcessSource(String const& _source)
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
    ShaderVulkanUniformData const* ShaderVulkan::getUniformData(HashedCString _uniformName) const
    {
        auto it = m_uniformsReflection.find(_uniformName.hash);
        if (it == m_uniformsReflection.end())
            return nullptr;

        return &it->second;
    }

    //////////////////////////////////////////
    ShaderUniformPtr const& ShaderVulkan::createUniformFromShader(HashedCString _uniformName, ShaderUniformType _type)
    {
        static ShaderUniformPtr const nullPointer;

        UnorderedMap<U32, ShaderUniformPtr>::const_iterator it = m_uniforms.find(_uniformName.hash);
        if (it != m_uniforms.end())
            return it->second;

        ShaderVulkanUniformData const* uniformData = getUniformData(_uniformName);
        if (!uniformData)
        {
            m_uniforms.emplace(_uniformName.hash, nullptr);
            return nullPointer;
        }

        ShaderUniformVulkanPtr newUniform = static_pointer_cast<ShaderUniformVulkan>(ShaderUniform::Create(getSharedPtr(), _type));
        MAZE_ERROR_RETURN_VALUE_IF(!newUniform, nullPointer, "Shader Uniform creation error!");
        newUniform->setName(_uniformName);
        newUniform->setUniformData(*uniformData);
        auto at = m_uniforms.emplace(_uniformName.hash, newUniform);
        if (at.second)
            return at.first->second;

        return nullPointer;
    }

    //////////////////////////////////////////
    void ShaderVulkan::writeUniformData(
        ShaderVulkanUniformData const& _uniformData,
        U8 const* _bytes,
        U32 _bytesCount)
    {
        if (_uniformData.isTexture)
            return;

        if (_uniformData.isGlobal)
        {
            getRenderSystemVulkanRaw()->writeGlobalUniformData(_uniformData.offset, _bytes, _bytesCount);
            return;
        }

        U32 bytesCount = Math::Min(_bytesCount, _uniformData.size);
        if (_uniformData.offset + bytesCount > (U32)m_materialUniformShadow.size())
        {
            if ((U32)m_materialUniformShadow.size() <= _uniformData.offset)
                return;
            bytesCount = (U32)m_materialUniformShadow.size() - _uniformData.offset;
        }

        if (bytesCount == 0u)
            return;

        memcpy(&m_materialUniformShadow[_uniformData.offset], _bytes, bytesCount);
    }

    //////////////////////////////////////////
    VkDescriptorSet ShaderVulkan::acquireMaterialDescriptorSet()
    {
        RenderSystemVulkan* rs = getRenderSystemVulkanRaw();
        VkDevice device = rs->getDevice();
        U32 frameIndex = rs->getCurrentFrameIndex();
        if (frameIndex >= m_materialInstancePool.size())
            return VK_NULL_HANDLE;

        // A new frame reaching this frame-in-flight index means
        // ensureFrameOpen() has already waited that slot's fence - every
        // pool instance handed out last time this index was used is
        // guaranteed to have finished executing on the GPU, so it's safe to
        // start recycling from the beginning of this frame-in-flight's pool
        U64 currentGeneration = rs->getFrameGeneration();
        if (m_materialInstancePoolGeneration[frameIndex] != currentGeneration)
        {
            m_materialInstancePoolUsed[frameIndex] = 0u;
            m_materialInstancePoolGeneration[frameIndex] = currentGeneration;
        }

        Vector<MaterialUniformInstance>& framePool = m_materialInstancePool[frameIndex];
        U32 slot = m_materialInstancePoolUsed[frameIndex]++;

        if (slot >= (U32)framePool.size())
        {
            MaterialUniformInstance inst;

            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = m_materialUniformBufferSize;
            bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            VmaAllocationCreateInfo allocInfo = {};
            allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
            allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;

            VmaAllocationInfo allocResultInfo = {};
            MAZE_VK_CALL(vmaCreateBuffer(
                rs->getAllocator(), &bufferInfo, &allocInfo,
                &inst.buffer, &inst.allocation, &allocResultInfo));
            inst.mapped = allocResultInfo.pMappedData;

            VkDescriptorSetAllocateInfo dsAllocInfo = {};
            dsAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            dsAllocInfo.descriptorPool = rs->getDescriptorPool();
            dsAllocInfo.descriptorSetCount = 1u;
            dsAllocInfo.pSetLayouts = &m_materialSetLayout;
            MAZE_VK_CALL(vkAllocateDescriptorSets(device, &dsAllocInfo, &inst.descriptorSet));

            // The UBO's VkBuffer identity never changes after creation (only
            // its contents, via the memcpy below), so this binding-0 write
            // never needs repeating for this instance
            VkDescriptorBufferInfo bufInfo = {};
            bufInfo.buffer = inst.buffer;
            bufInfo.offset = 0u;
            bufInfo.range = m_materialUniformBufferSize;

            VkWriteDescriptorSet write = {};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = inst.descriptorSet;
            write.dstBinding = 0u;
            write.descriptorCount = 1u;
            write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            write.pBufferInfo = &bufInfo;
            vkUpdateDescriptorSets(device, 1u, &write, 0u, nullptr);

            framePool.push_back(inst);
        }

        MaterialUniformInstance& inst = framePool[slot];

        if (inst.mapped && !m_materialUniformShadow.empty())
            memcpy(inst.mapped, m_materialUniformShadow.data(), m_materialUniformShadow.size());

        // Every set-1 combined-image-sampler binding must hold a valid
        // descriptor by the time it's ever sampled, even if the owning
        // material never explicitly assigns that texture uniform (unlike
        // GL/DX11, Vulkan has no concept of an implicitly-safe "unbound"
        // sampler) - fall back to the engine's white texture (matching
        // aspect: 2D or cube) for any binding never assigned a real one.
        // getWhiteTexture()/getWhiteCubeTexture() are passive accessors that
        // return null until something else has already lazily created the
        // builtin, so use the create-if-missing ensure*() variant instead.
        FixedVector<VkWriteDescriptorSet, 16> writes;
        FixedVector<VkDescriptorImageInfo, 16> imageInfos;
        imageInfos.reserve(m_textureBindings.size());

        // m_textureBindings is indexed by raw SPIR-V binding number (see
        // createDescriptorSetLayoutsAndPipelineLayout()'s comment) - index 0
        // is always the unused UBO-slot placeholder
        for (Size b = 1; b < m_textureBindings.size(); ++b)
        {
            PendingTextureBinding& binding = m_textureBindings[b];

            VkImageView view = binding.view;
            VkImageLayout layout = binding.layout;
            VkSampler sampler = binding.sampler;

            if (view == VK_NULL_HANDLE)
            {
                if (binding.isCube)
                {
                    if (TextureCubePtr const& whiteCube = rs->getTextureManager()->ensureBuiltinTextureCube(BuiltinTextureCubeType::White))
                    {
                        TextureCubeVulkan* whiteCubeVulkan = whiteCube->castRaw<TextureCubeVulkan>();
                        view = whiteCubeVulkan->getImageView();
                        sampler = whiteCubeVulkan->ensureSampler();
                        layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    }
                }
                else
                {
                    if (Texture2DPtr const& white2D = rs->getTextureManager()->ensureBuiltinTexture2D(BuiltinTexture2DType::White))
                    {
                        Texture2DVulkan* white2DVulkan = white2D->castRaw<Texture2DVulkan>();
                        view = white2DVulkan->getImageView();
                        sampler = white2DVulkan->ensureSampler();
                        layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    }
                }
            }

            if (view == VK_NULL_HANDLE)
                continue;

            VkDescriptorImageInfo imageInfo = {};
            imageInfo.imageView = view;
            imageInfo.imageLayout = layout;
            imageInfo.sampler = sampler;
            imageInfos.push_back(imageInfo);

            VkWriteDescriptorSet write = {};
            write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            write.dstSet = inst.descriptorSet;
            write.dstBinding = (U32)b;
            write.descriptorCount = 1u;
            write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            write.pImageInfo = &imageInfos.back();
            writes.push_back(write);
        }

        if (!writes.empty())
            vkUpdateDescriptorSets(device, (U32)writes.size(), writes.data(), 0u, nullptr);

        return inst.descriptorSet;
    }

    //////////////////////////////////////////
    void ShaderVulkan::setTextureBinding(S32 _binding, VkImageView _view, VkImageLayout _layout, VkSampler _sampler)
    {
        if (_binding < 0)
            return;

        if ((S32)m_textureBindings.size() <= _binding)
            m_textureBindings.resize(_binding + 1);

        PendingTextureBinding& binding = m_textureBindings[_binding];
        binding.view = _view;
        binding.layout = _layout;
        binding.sampler = _sampler;
    }


} // namespace Maze
//////////////////////////////////////////
