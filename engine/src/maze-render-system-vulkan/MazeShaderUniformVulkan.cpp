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
#include "maze-render-system-vulkan/MazeShaderUniformVulkan.hpp"
#include "maze-render-system-vulkan/MazeRenderSystemVulkan.hpp"
#include "maze-render-system-vulkan/MazeTexture2DVulkan.hpp"
#include "maze-render-system-vulkan/MazeTextureCubeVulkan.hpp"
#include "maze-core/math/MazeMat3.hpp"
#include "maze-core/math/MazeMat4.hpp"
#include "maze-core/math/MazeTMat.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    // Class ShaderUniformVulkan
    //
    //////////////////////////////////////////
    ShaderUniformVulkan::ShaderUniformVulkan()
    {
    }

    //////////////////////////////////////////
    ShaderUniformVulkan::~ShaderUniformVulkan()
    {
    }

    //////////////////////////////////////////
    ShaderUniformVulkanPtr ShaderUniformVulkan::Create(
        ShaderPtr const& _shader,
        ShaderUniformType _type)
    {
        ShaderUniformVulkanPtr object;
        MAZE_CREATE_AND_INIT_MANAGED_SHARED_PTR(ShaderUniformVulkan, object, init(_shader, _type));
        return object;
    }

    //////////////////////////////////////////
    bool ShaderUniformVulkan::init(
        ShaderPtr const& _shader,
        ShaderUniformType _type)
    {
        if (!ShaderUniform::init(_shader, _type))
            return false;

        return true;
    }

    //////////////////////////////////////////
    ShaderVulkan* ShaderUniformVulkan::getShaderVulkanRaw() const
    {
        return getShaderRaw()->castRaw<ShaderVulkan>();
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::processSimpleUniformChanged()
    {
        if (m_uniformData.isTexture)
        {
            ShaderVulkan* shader = getShaderVulkanRaw();
            RenderSystemVulkan* rs = shader->getRenderSystemVulkanRaw();

            VkImageView view = VK_NULL_HANDLE;
            VkSampler sampler = VK_NULL_HANDLE;

            if (getType() == ShaderUniformType::UniformTexture2D)
            {
                Texture2D* texture = getTexture2DRaw();
                if (texture)
                {
                    Texture2DVulkan* textureVulkan = texture->castRaw<Texture2DVulkan>();
                    view = textureVulkan->getImageView();
                    sampler = textureVulkan->ensureSampler();
                }
            }
            else
            if (getType() == ShaderUniformType::UniformTextureCube)
            {
                TextureCube* texture = getTextureCubeRaw();
                if (texture)
                {
                    TextureCubeVulkan* textureVulkan = texture->castRaw<TextureCubeVulkan>();
                    view = textureVulkan->getImageView();
                    sampler = textureVulkan->ensureSampler();
                }
            }

            shader->setTextureBinding(m_uniformData.textureBinding, view, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, sampler);
            return;
        }

        switch (getType())
        {
            case ShaderUniformType::UniformS32: { S32 v = getS32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformF32: { F32 v = getF32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
                break;
            case ShaderUniformType::UniformVec2S32: { Vec2S v = getVec2S32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformVec3S32: { Vec3S v = getVec3S32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformVec4S32: { Vec4S v = getVec4S32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformVec2F32: { Vec2F v = getVec2F32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformVec3F32: { Vec3F v = getVec3F32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformVec4F32: { Vec4F v = getVec4F32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformMat3F32: { Mat3F v = getMat3F32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformMat4F32: { Mat4F v = getMat4F32(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformColorF128: { Vec4F v = getVec4F32(); uploadBytes(&v, sizeof(v)); break; }
            default:
                break;
        }
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::uploadBytes(void const* _bytes, U32 _byteSize)
    {
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_bytes, _byteSize);
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(F32 const* _values, Size _count)
    {
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_values, (U32)(_count * sizeof(F32)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Vec2F const* _vectors, Size _count)
    {
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_vectors, (U32)(_count * sizeof(Vec2F)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Vec3F const* _vectors, Size _count)
    {
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_vectors, (U32)(_count * sizeof(Vec3F)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Vec4F const* _vectors, Size _count)
    {
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_vectors, (U32)(_count * sizeof(Vec4F)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Mat3F const* _matrices, Size _count)
    {
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_matrices, (U32)(_count * sizeof(Mat3F)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Mat4F const* _matrices, Size _count)
    {
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_matrices, (U32)(_count * sizeof(Mat4F)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(TMat const* _matrices, Size _count)
    {
        // TMat is a 3x4 affine transform (3 rows of float4, matching the
        // GLSL/HLSL 'float3 u_modelMatrices[N*4]' packing convention used by
        // GetModelMatrix()) - see MazeCommonShaderHeader.mzglslvk. This path
        // is only used for the (non-instance-stream) per-uniform TMat
        // upload; the bulk instance-stream write goes through
        // InstanceStreamsVulkan directly, not through this ShaderUniform.
        getShaderVulkanRaw()->writeUniformData(m_uniformData, (U8 const*)_matrices, (U32)(_count * sizeof(TMat)));
    }


} // namespace Maze
//////////////////////////////////////////
