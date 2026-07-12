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
#if (!defined(_MazeShaderVulkan_hpp_))
#define _MazeShaderVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-graphics/MazeShader.hpp"
#include "maze-graphics/MazeVertex.hpp"
#include "maze-core/containers/MazeFastVector.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_SHARED_PTR(ShaderVulkan);
    MAZE_USING_SHARED_PTR(RenderSystem);
    class RenderSystemVulkan;


    //////////////////////////////////////////
    enum class ShaderVulkanStage : S32
    {
        Vertex = 0,
        Fragment = 1,
        MAX = 2
    };


    //////////////////////////////////////////
    // Descriptor set indices - fixed convention shared by every built-in and
    // asset .mzglslvk shader (see MazeCommonShaderHeader.mzglslvk for the
    // authoritative documentation of this layout):
    //   set 0 = engine-global data, one shared instance across all shaders:
    //           binding 0 = GlobalUniforms UBO (view/projection/clip distance/etc)
    //           binding 1 = ModelMatrices SSBO (instance stream)
    //           binding 2 = ColorsStream SSBO (instance stream)
    //           binding 3 = UVStreams SSBO (instance stream, both channels)
    //   set 1 = per-shader material data, one instance per ShaderVulkan:
    //           binding 0 = MaterialUniforms UBO (every other named uniform
    //                       the shader declares)
    //           binding 1..N = sampler2D/samplerCube textures, in declaration order
    //   push constant = { int u_instanceOffset } (instance-stream batch offset,
    //           set directly by RenderQueueVulkan, not through the uniform system)
    static S32 const c_vulkanGlobalDescriptorSet = 0;
    static S32 const c_vulkanMaterialDescriptorSet = 1;


    //////////////////////////////////////////
    // Struct ShaderVulkanUniformData
    //
    //////////////////////////////////////////
    struct MAZE_RENDER_SYSTEM_VULKAN_API ShaderVulkanUniformData
    {
        String name;

        // True if this uniform lives in the shared set-0 GlobalUniforms UBO
        // (owned by RenderSystemVulkan) rather than this shader's own set-1
        // MaterialUniforms UBO.
        bool isGlobal = false;

        // Byte offset/size within the owning UBO (global or material)
        U32 offset = 0u;
        U32 size = 0u;

        // True if this is a combined-image-sampler binding within set 1
        // (the two texture-related fields below are then valid instead of
        // offset/size)
        bool isTexture = false;
        S32 textureBinding = -1;

        // True for a logical UniformTexture2DArray uniform assembled from
        // several individually-named sibling bindings (e.g. "u_baseMaps0"..
        // "u_baseMaps7") rather than one real GLSL array-of-samplers
        // binding - see reflectVulkanShader()'s array-collapsing pass for
        // why: the current descriptor-set-layout builder only supports
        // descriptorCount=1 per binding, so multi-page font materials (the
        // only current user of this) are ported as N separate bindings
        // instead. textureBindingsArray[i] is the raw SPIR-V binding number
        // for array element i, or -1 if that element wasn't declared by the
        // shader.
        bool isTextureArray = false;
        Vector<S32> textureBindingsArray;

        //////////////////////////////////////////
        inline bool isUsed() const { return isTexture ? (textureBinding >= 0 || isTextureArray) : (size > 0u); }
    };


    //////////////////////////////////////////
    // Class ShaderVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API ShaderVulkan
        : public Shader
    {
    public:

        //////////////////////////////////////////
        MAZE_DECLARE_METACLASS_WITH_PARENT(ShaderVulkan, Shader);

    public:

        //////////////////////////////////////////
        virtual ~ShaderVulkan();

        //////////////////////////////////////////
        static ShaderPtr Create(RenderSystemPtr const& _renderSystem);

        //////////////////////////////////////////
        static ShaderPtr CreateFromFile(
            RenderSystemPtr const& _renderSystem,
            AssetFilePtr const& _shaderFile);

        //////////////////////////////////////////
        static ShaderPtr CreateFromSource(
            RenderSystemPtr const& _renderSystem,
            String const& _shaderSource,
            CString _shaderName = nullptr);

        //////////////////////////////////////////
        static ShaderPtr Create(ShaderVulkanPtr const& _shader);

        //////////////////////////////////////////
        virtual ShaderPtr createCopy() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual bool isValid() MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual CString getLanguage() const MAZE_OVERRIDE { return "SPIRV"; }


        //////////////////////////////////////////
        virtual bool loadFromSource(String const& _shaderSource) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual bool loadFromSources(String const& _vertexShaderSource, String const& _fragmentShaderSource) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void recompile() MAZE_OVERRIDE;


        //////////////////////////////////////////
        RenderSystemVulkan* getRenderSystemVulkanRaw() const;

        //////////////////////////////////////////
        inline VkPipelineLayout getPipelineLayout() const { return m_pipelineLayout; }

        //////////////////////////////////////////
        inline VkDescriptorSetLayout getMaterialDescriptorSetLayout() const { return m_materialSetLayout; }

        //////////////////////////////////////////
        inline VkShaderModule getVertexModule() const { return m_vertexModule; }

        //////////////////////////////////////////
        inline VkShaderModule getFragmentModule() const { return m_fragmentModule; }

        //////////////////////////////////////////
        // Vertex input locations reflected from the vertex stage, keyed by
        // Maze VertexAttributeSemantic - used by VertexArrayObjectVulkan to
        // build the pipeline's VkVertexInputAttributeDescription array
        S32 getVertexInputLocation(VertexAttributeSemantic _semantic) const;


        //////////////////////////////////////////
        // Drops cached #include contents (dev-time shader reload)
        static void ClearShaderIncludeFilesCache();


        //////////////////////////////////////////
        ShaderVulkanUniformData const* getUniformData(HashedCString _uniformName) const;

        //////////////////////////////////////////
        // Writes packed bytes into the material (or global) UBO shadow
        void writeUniformData(
            ShaderVulkanUniformData const& _uniformData,
            U8 const* _bytes,
            U32 _bytesCount);

        //////////////////////////////////////////
        // Hands out a material descriptor set holding a fresh snapshot of the
        // current UBO shadow + texture bindings, valid for exactly one draw
        // call. A Shader object is shared across every Material that
        // references the same shader file/name (Material only deep-copies
        // its own ShaderUniformVariant *values*, not the GPU-side Shader) -
        // so a single mutable per-shader descriptor set (the original design)
        // breaks the moment two materials sharing this shader both draw
        // within the same frame: Vulkan resolves a bound descriptor set's
        // contents at GPU *execution* time, not at command-buffer *recording*
        // time, so updating it for material B after material A's draw was
        // already recorded would make material A's draw retroactively render
        // with material B's data - and separately, updating a descriptor set
        // already bound to a still-recording/in-flight command buffer is an
        // outright validation violation without
        // VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT (not used here).
        // Pulls the next slot from a per-frame-in-flight pool (see
        // m_materialInstancePool), growing it on demand and implicitly
        // recycling slots once their frame-in-flight fence has been waited
        // on again (see RenderSystemVulkan::getFrameGeneration()).
        VkDescriptorSet acquireMaterialDescriptorSet();

        //////////////////////////////////////////
        // Updates the shadow texture-binding state (view/sampler at the
        // given set-1 binding index) - the actual descriptor write happens
        // lazily, per draw call, inside acquireMaterialDescriptorSet()
        void setTextureBinding(S32 _binding, VkImageView _view, VkImageLayout _layout, VkSampler _sampler);

    protected:

        //////////////////////////////////////////
        ShaderVulkan();

        //////////////////////////////////////////
        virtual bool init(RenderSystemPtr const& _renderSystem) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool init(
            RenderSystemPtr const& _renderSystem,
            AssetFilePtr const& _shaderFile);

        //////////////////////////////////////////
        bool init(
            RenderSystemPtr const& _renderSystem,
            String const& _shaderSource,
            CString _shaderName);

        //////////////////////////////////////////
        bool init(ShaderVulkanPtr const& _shader);

        //////////////////////////////////////////
        virtual ShaderUniformPtr const& createUniformFromShader(HashedCString _uniformName, ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        bool loadVulkanShader(String const& _vertexShaderSource, String const& _fragmentShaderSource);

        //////////////////////////////////////////
        void unloadVulkanShader();

        //////////////////////////////////////////
        // Compiles GLSL source to SPIR-V via shaderc (Vulkan SDK shaderc_combined)
        Vector<U32> compileVulkanShader(
            String const& _source,
            ShaderVulkanStage _stage);

        //////////////////////////////////////////
        // Reflects a SPIR-V module via SPIRV-Reflect, discovering set-0/set-1
        // bindings and populating m_uniformsReflection
        bool reflectVulkanShader(
            Vector<U32> const& _spirv,
            ShaderVulkanStage _stage);

        //////////////////////////////////////////
        bool createDescriptorSetLayoutsAndPipelineLayout();

        //////////////////////////////////////////
        bool createMaterialUniformBuffers();

        //////////////////////////////////////////
        String buildLocalShaderFeatures();

        //////////////////////////////////////////
        // Resolves '#include "File.mzglslvk"' directives via the asset manager
        String makeInternalShaderPreprocessing(String _shader);

        //////////////////////////////////////////
        UnorderedMap<S32, String> preProcessSource(String const& _source);

    protected:
        String m_vertexShaderSource;
        String m_fragmentShaderSource;

        VkShaderModule m_vertexModule = VK_NULL_HANDLE;
        VkShaderModule m_fragmentModule = VK_NULL_HANDLE;

        VkDescriptorSetLayout m_materialSetLayout = VK_NULL_HANDLE;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;

        // Material UBO shadow - the "current" value every acquireMaterialDescriptorSet()
        // call snapshots into whichever instance it hands out
        U32 m_materialUniformBufferSize = 0u;
        Vector<U8> m_materialUniformShadow;

        // Texture bindings shadow (binding -> current view/layout/sampler).
        // isCube records the SPIR-V OpTypeImage's Dim (reflected at
        // shader-load time) so acquireMaterialDescriptorSet()'s fallback for
        // an unset binding picks a view of the matching VkImageViewType -
        // writing a 2D view into a samplerCube binding (or vice versa) is a
        // VkImageViewType mismatch the validation layer flags on every draw.
        struct PendingTextureBinding { VkImageView view = VK_NULL_HANDLE; VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED; VkSampler sampler = VK_NULL_HANDLE; bool isCube = false; };
        Vector<PendingTextureBinding> m_textureBindings; // indexed by the raw SPIR-V binding number (binding 0 is always the UBO, so index 0 is unused)

        // One UBO + descriptor set per (frame-in-flight, pool slot) - see
        // acquireMaterialDescriptorSet()'s banner comment for why a single
        // shared instance per shader is unsafe. The pool grows on demand and
        // stabilizes once it reaches this shader's peak per-frame draw count;
        // slots are recycled (safely, since acquireMaterialDescriptorSet()'s
        // generation check only resets the "used" cursor after
        // ensureFrameOpen() has waited that frame-in-flight slot's fence)
        // rather than ever freed individually.
        struct MaterialUniformInstance
        {
            VkBuffer buffer = VK_NULL_HANDLE;
            VmaAllocation allocation = VK_NULL_HANDLE;
            void* mapped = nullptr;
            VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
        };
        Vector<Vector<MaterialUniformInstance>> m_materialInstancePool; // [frameIndex][poolSlot]
        Vector<U32> m_materialInstancePoolUsed;                         // [frameIndex] - pool slots handed out so far this frame
        Vector<U64> m_materialInstancePoolGeneration;                   // [frameIndex] - RenderSystemVulkan::getFrameGeneration() last seen for this frame index

        S32 m_vertexInputLocations[(S32)VertexAttributeSemantic::MAX];

        UnorderedMap<U32, ShaderVulkanUniformData> m_uniformsReflection;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderVulkan_hpp_
//////////////////////////////////////////
