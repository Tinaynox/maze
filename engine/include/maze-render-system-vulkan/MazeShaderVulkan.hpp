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

        //////////////////////////////////////////
        inline bool isUsed() const { return isTexture ? (textureBinding >= 0) : (size > 0u); }
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
        // Uploads the dirty material UBO (mapped memory for the current
        // frame-in-flight index) and (re)writes the descriptor set if a bound
        // texture changed since the last flush
        void flushMaterialUniforms();

        //////////////////////////////////////////
        // Per-frame-in-flight material descriptor set (allocated once,
        // updated in place - never freed individually, the pool is reset
        // wholesale on shutdown)
        VkDescriptorSet getMaterialDescriptorSet(U32 _frameIndex) const;

        //////////////////////////////////////////
        // Binds a texture + sampler at the given set-1 binding index for every
        // frame-in-flight's descriptor set (deferred: actually written on the
        // next flushMaterialUniforms())
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

        // One descriptor set per frame-in-flight, allocated from
        // RenderSystemVulkan::getDescriptorPool()
        Vector<VkDescriptorSet> m_materialDescriptorSets;

        // Material UBO - one VkBuffer per frame-in-flight, persistently mapped
        Vector<VkBuffer> m_materialUniformBuffers;
        Vector<VmaAllocation> m_materialUniformAllocations;
        Vector<void*> m_materialUniformMapped;
        U32 m_materialUniformBufferSize = 0u;
        Vector<U8> m_materialUniformShadow;
        bool m_materialUniformDirty[8] = {}; // per frame-in-flight dirty flag, indexed by frame index (see RenderSystemVulkanConfig::framesInFlight, capped)

        // Texture bindings pending write into the descriptor set (binding -> view/layout/sampler)
        // isCube records the SPIR-V OpTypeImage's Dim (reflected at shader-load
        // time) so a default/fallback descriptor write (see
        // createMaterialUniformBuffers()) can pick a view of the matching
        // VkImageViewType - writing a 2D view into a samplerCube binding (or
        // vice versa) is a VkImageViewType mismatch the validation layer
        // flags on every draw that uses it.
        struct PendingTextureBinding { VkImageView view = VK_NULL_HANDLE; VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED; VkSampler sampler = VK_NULL_HANDLE; bool dirty = false; bool isCube = false; };
        Vector<PendingTextureBinding> m_textureBindings; // indexed by set-1 binding - 1 (binding 0 is the UBO)

        S32 m_vertexInputLocations[(S32)VertexAttributeSemantic::MAX];

        UnorderedMap<U32, ShaderVulkanUniformData> m_uniformsReflection;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderVulkan_hpp_
//////////////////////////////////////////
