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
#if (!defined(_MazeShaderUniformVulkan_hpp_))
#define _MazeShaderUniformVulkan_hpp_


//////////////////////////////////////////
#include "maze-render-system-vulkan/MazeRenderSystemVulkanHeader.hpp"
#include "maze-render-system-vulkan/MazeShaderVulkan.hpp"
#include "maze-graphics/MazeShaderUniform.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    MAZE_USING_MANAGED_SHARED_PTR(ShaderUniformVulkan);


    //////////////////////////////////////////
    // Class ShaderUniformVulkan
    //
    //////////////////////////////////////////
    class MAZE_RENDER_SYSTEM_VULKAN_API ShaderUniformVulkan
        : public ShaderUniform
    {
    public:

        //////////////////////////////////////////
        virtual ~ShaderUniformVulkan();

        //////////////////////////////////////////
        static ShaderUniformVulkanPtr Create(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None);


        //////////////////////////////////////////
        using ShaderUniform::setName;

        //////////////////////////////////////////
        // isGlobal fields live in the shared GlobalUniforms UBO (one
        // physical buffer written by every shader's own ShaderUniform
        // instance) - setAlwaysForceUpdate(true) ensures this instance's
        // own "value looks unchanged" cache can never mask a write that's
        // needed because a DIFFERENT shader overwrote the shared buffer in
        // between. See ShaderUniform::setAlwaysForceUpdate()'s banner
        // comment for the full rationale.
        //
        // Texture uniforms need the same treatment for a different reason:
        // ShaderUniform::set(Texture2DPtr const&)'s skip-if-unchanged check
        // compares Texture2D::getResourceId(), a stable per-object identity
        // that does NOT change when a render-buffer texture is resized in
        // place (Texture2DVulkan::loadEmpty() destroys and recreates the
        // underlying VkImageView on the SAME Texture2D object). A material
        // that re-sets the same texture reference every frame (e.g.
        // Distortion00's u_depthMap, bound once to a render buffer's depth
        // texture) would then skip processSimpleUniformChanged() forever
        // after the first successful set() - permanently binding the
        // pre-resize VkImageView, confirmed via
        // "vkUpdateDescriptorSets(): ... invalid VkImageView" validation
        // warnings recurring on every draw after a window resize. Forcing
        // every texture uniform to always re-resolve its current view keeps
        // this cheap: processSimpleUniformChanged() only refreshes a
        // lightweight shadow struct here, not an actual descriptor write
        // (that happens once per draw call regardless, in
        // ShaderVulkan::acquireMaterialDescriptorSet()).
        inline void setUniformData(ShaderVulkanUniformData const& _data)
        {
            m_uniformData = _data;
            setAlwaysForceUpdate(_data.isGlobal || _data.isTexture);
        }

        //////////////////////////////////////////
        inline ShaderVulkanUniformData const& getUniformData() const { return m_uniformData; }


        //////////////////////////////////////////
        virtual void processSimpleUniformChanged() MAZE_OVERRIDE;


        //////////////////////////////////////////
        virtual void upload(F32 const* _values, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec2F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec3F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Vec4F const* _vectors, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Mat3F const* _matrices, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(Mat4F const* _matrices, Size _count) MAZE_OVERRIDE;

        //////////////////////////////////////////
        virtual void upload(TMat const* _matrices, Size _count) MAZE_OVERRIDE;

    protected:

        //////////////////////////////////////////
        ShaderUniformVulkan();

        //////////////////////////////////////////
        virtual bool init(
            ShaderPtr const& _shader,
            ShaderUniformType _type = ShaderUniformType::None) MAZE_OVERRIDE;

        //////////////////////////////////////////
        ShaderVulkan* getShaderVulkanRaw() const;

        //////////////////////////////////////////
        void uploadBytes(void const* _bytes, U32 _byteSize);

    protected:
        ShaderVulkanUniformData m_uniformData;
    };


} // namespace Maze
//////////////////////////////////////////


#endif // _MazeShaderUniformVulkan_hpp_
//////////////////////////////////////////
