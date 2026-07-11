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
    // GLSL std140 array elements are aligned to 16 bytes (same rule as HLSL
    // constant buffers - see ShaderUniformDX11.cpp's identical c_registerSize)
    static U32 const c_std140RegisterSize = 16u;


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
            case ShaderUniformType::UniformF64: { F32 v = (F32)getF64(); uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformBool: { S32 v = getBool() ? 1 : 0; uploadBytes(&v, sizeof(v)); break; }
            case ShaderUniformType::UniformTexture2D:
            case ShaderUniformType::UniformTextureCube:
            case ShaderUniformType::UniformTexture2DArray:
                break;
            case ShaderUniformType::UniformVec2F32: { Vec4F const& v = getVecF(); uploadBytes(&v, sizeof(F32) * 2); break; }
            case ShaderUniformType::UniformVec3F32: { Vec4F const& v = getVecF(); uploadBytes(&v, sizeof(F32) * 3); break; }
            case ShaderUniformType::UniformVec4F32:
            case ShaderUniformType::UniformColorF128: { Vec4F const& v = getVecF(); uploadBytes(&v, sizeof(F32) * 4); break; }
            case ShaderUniformType::UniformVec2S32: { Vec4S const& v = getVecS(); uploadBytes(&v, sizeof(S32) * 2); break; }
            case ShaderUniformType::UniformVec3S32: { Vec4S const& v = getVecS(); uploadBytes(&v, sizeof(S32) * 3); break; }
            case ShaderUniformType::UniformVec4S32: { Vec4S const& v = getVecS(); uploadBytes(&v, sizeof(S32) * 4); break; }
            case ShaderUniformType::UniformVec2U32: { Vec4U const& v = getVecU(); uploadBytes(&v, sizeof(U32) * 2); break; }
            case ShaderUniformType::UniformVec3U32: { Vec4U const& v = getVecU(); uploadBytes(&v, sizeof(U32) * 3); break; }
            case ShaderUniformType::UniformVec4U32: { Vec4U const& v = getVecU(); uploadBytes(&v, sizeof(U32) * 4); break; }
            case ShaderUniformType::UniformVec2B:
            case ShaderUniformType::UniformVec3B:
            case ShaderUniformType::UniformVec4B:
            {
                Vec4B const& v = getVecB();
                S32 values[4] = { v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, v.w ? 1 : 0 };
                Size count = (getType() == ShaderUniformType::UniformVec2B) ? 2 : (getType() == ShaderUniformType::UniformVec3B) ? 3 : 4;
                uploadBytes(values, (U32)(sizeof(S32) * count));
                break;
            }
            case ShaderUniformType::UniformMat3F32: { Mat3F v = getMat3F32(); upload(&v, 1); break; }
            case ShaderUniformType::UniformMat4F32: { Mat4F v = getMat4F32(); upload(&v, 1); break; }
            case ShaderUniformType::UniformTMat: { TMat v = getTMat(); upload(&v, 1); break; }
            default:
                MAZE_ERROR("Unsupported ShaderUniformType: %s!", getType().toCString());
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
        if (_count == 1)
        {
            uploadBytes(_values, sizeof(F32));
            return;
        }

        Vector<U8> packed(_count * c_std140RegisterSize, 0u);
        for (Size i = 0; i < _count; ++i)
            memcpy(&packed[i * c_std140RegisterSize], &_values[i], sizeof(F32));
        uploadBytes(packed.data(), (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Vec2F const* _vectors, Size _count)
    {
        if (_count == 1)
        {
            uploadBytes(_vectors, sizeof(Vec2F));
            return;
        }

        Vector<U8> packed(_count * c_std140RegisterSize, 0u);
        for (Size i = 0; i < _count; ++i)
            memcpy(&packed[i * c_std140RegisterSize], &_vectors[i], sizeof(Vec2F));
        uploadBytes(packed.data(), (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Vec3F const* _vectors, Size _count)
    {
        if (_count == 1)
        {
            uploadBytes(_vectors, sizeof(Vec3F));
            return;
        }

        Vector<U8> packed(_count * c_std140RegisterSize, 0u);
        for (Size i = 0; i < _count; ++i)
            memcpy(&packed[i * c_std140RegisterSize], &_vectors[i], sizeof(Vec3F));
        uploadBytes(packed.data(), (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Vec4F const* _vectors, Size _count)
    {
        // vec4 is already 16-byte aligned - no padding needed between elements
        uploadBytes(_vectors, (U32)(_count * sizeof(Vec4F)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Mat3F const* _matrices, Size _count)
    {
        // Each 3-component column is padded to a 16-byte register (mat3 in
        // std140 is stored as 3 columns of vec4, same as HLSL float3x3)
        Vector<U8> packed(_count * 3 * c_std140RegisterSize, 0u);
        for (Size i = 0; i < _count; ++i)
        {
            F32 const* planeMatrix = _matrices[i].getPlaneMatrix();
            for (Size c = 0; c < 3; ++c)
                memcpy(&packed[(i * 3 + c) * c_std140RegisterSize], planeMatrix + c * 3, sizeof(F32) * 3);
        }
        uploadBytes(packed.data(), (U32)packed.size());
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(Mat4F const* _matrices, Size _count)
    {
        // mat4 is already 16-byte-aligned per column - no padding needed
        uploadBytes(_matrices, (U32)(_count * sizeof(Mat4F)));
    }

    //////////////////////////////////////////
    void ShaderUniformVulkan::upload(TMat const* _matrices, Size _count)
    {
        // TMat is a 3x4 affine transform (4 rows of 3 floats - see
        // MazeTMat.hpp/getRow()); each row is padded to a 16-byte register to
        // match the 'vec3 u_viewMatrix[4]' std140 array declaration in
        // MazeCommonShaderHeader.mzglslvk (std140 arrays always pad each
        // element to 16 bytes, same rule HLSL constant buffers use - see
        // ShaderUniformDX11::upload(TMat const*, Size)). This path is only
        // used for the (non-instance-stream) per-uniform TMat upload; the
        // bulk instance-stream write goes through InstanceStreamsVulkan
        // directly (as a full mat4, not this packing), not through this
        // ShaderUniform.
        Vector<U8> packed(_count * 4 * c_std140RegisterSize, 0u);
        for (Size i = 0; i < _count; ++i)
        {
            for (Size r = 0; r < 4; ++r)
            {
                Vec3F row = _matrices[i].getRow(r);
                memcpy(&packed[(i * 4 + r) * c_std140RegisterSize], &row, sizeof(F32) * 3);
            }
        }
        uploadBytes(packed.data(), (U32)packed.size());
    }


} // namespace Maze
//////////////////////////////////////////
